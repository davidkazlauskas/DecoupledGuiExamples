
#include <iostream>
#include <gtkmm.h>
#include <templatious/FullPack.hpp>

#include "mainwindow_interface.h"
#include "messeagable.h"
#include "domain.h"

TEMPLATIOUS_TRIPLET_STD;

struct GtkMainWindow : public Messageable {
    GtkMainWindow(Glib::RefPtr<Gtk::Builder>& bld) :
        _dvmf(true), _msgHandler(genHandler())
    {
        //Get the GtkBuilder-instantiated Dialog:
        bld->get_widget("window1", _wnd);
        if (nullptr != _wnd) {
            _btn = nullptr;
            bld->get_widget("button1", _btn);
            if (nullptr != _btn) {
                _btn->signal_clicked().connect(
                    sigc::mem_fun(*this,&GtkMainWindow::btnClicked) );
            }
            bld->get_widget("entry1",_ent);
            bld->get_widget("progressbar1",_prog);
            bld->get_widget("label1",_lbl);
            _wnd->set_size_request(400,122);
            _wnd->signal_draw().connect(
                sigc::mem_fun(*this,&GtkMainWindow::onPaint) );
        }
    }

    ~GtkMainWindow() {
        delete _wnd;
    }

    void message(std::shared_ptr< templatious::VirtualPack > msg) override {
        {
            Guard g(_mtx);
            SA::add(_queue,msg);
        }
        this->_wnd->queue_draw();
    };

    // this is for sending stack allocated (faster)
    // if we know we're on the same thread as GUI
    void message(templatious::VirtualPack& msg) override {
        procMessage(msg);
    }

    Gtk::Window* getPtr() {
        return _wnd;
    }
private:
    typedef std::unique_ptr< templatious::VirtualMatchFunctor > Handler;
    typedef std::shared_ptr< templatious::VirtualPack > MsgPtr;
    typedef std::lock_guard< std::mutex > Guard;
    typedef MainWindowInterface Msg;

    void btnClicked() {
        auto msg = SF::vpack< Msg::OutButtonClicked >(
            Msg::OutButtonClicked()
        );
        _dvmf.tryMatch(msg);
    }

    bool onPaint(const Cairo::RefPtr<Cairo::Context>& cr) {
        processMessages();
        return false;
    }

    void procMessage(templatious::VirtualPack& pack) {
        _msgHandler->tryMatch(pack);
    }

    void processMessages() {
        std::vector< MsgPtr > currQueue;
        {
            Guard g(_mtx);
            currQueue = std::move(_queue);
        }

        TEMPLATIOUS_FOREACH(auto& i,currQueue) {
            procMessage(*i);
        }
    }

    Handler genHandler() {
        return SF::virtualMatchFunctorPtr(
            SF::virtualMatch< Msg::InAttachCallback, Handler >(
                [=](Msg::InAttachCallback,Handler& h) {
                    this->_dvmf.attach(std::move(h));
                }
            ),
            SF::virtualMatch< Msg::InSetButtonEnabled, bool >(
                [=](Msg::InSetButtonEnabled,bool value) {
                    this->_btn->set_sensitive(value);
                }
            ),
            SF::virtualMatch< Msg::InSetStatusText, const std::string >(
                [=](Msg::InSetStatusText,const std::string& str) {
                    this->_lbl->set_text(str);
                }
            ),
            SF::virtualMatch< Msg::InSetProgress, const int >(
                [=](Msg::InSetProgress,int prog) {
                    float conv = prog;
                    conv /= 100;
                    this->_prog->set_fraction(conv);
                }
            ),
            SF::virtualMatch< Msg::InShow >(
                [=](Msg::InShow) {
                    // window already shown,
                    // don't care.
                }
            ),
            SF::virtualMatch< Msg::QueryLabelText, std::string >(
                [=](Msg::QueryLabelText, std::string& s) {
                    s = this->_ent->get_text().c_str();
                }
            )
        );
    }

    Gtk::Window* _wnd;
    Gtk::Button* _btn;
    Gtk::Entry* _ent;
    Gtk::ProgressBar* _prog;
    Gtk::Label* _lbl;
    std::mutex _mtx;
    std::vector< MsgPtr > _queue;
    templatious::DynamicVMatchFunctor _dvmf;
    Handler _msgHandler;
};

int main (int argc, char **argv)
{
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  //Load the GtkBuilder file and instantiate its widgets:
  Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("ui.glade");
  }
  catch(const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Glib::MarkupError& ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Gtk::BuilderError& ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  auto mwnd = std::make_shared< GtkMainWindow >(refBuilder);
  initDomain(mwnd);
  app->run(*mwnd->getPtr());

  return 0;
}
