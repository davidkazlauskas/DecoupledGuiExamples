#include <CtrlLib/CtrlLib.h>
#include <templatious/FullPack.hpp>

#include "mainwindow_interface.h"
#include "messeagable.h"
#include "domain.h"

TEMPLATIOUS_TRIPLET_STD;

using namespace Upp;

struct MainCalcWindow : TopWindow, public Messageable {
	typedef MainCalcWindow CLASSNAME;
	
	MainCalcWindow() : _handler(MakeHandler()), _dvmf(true) {
		Title("Nth prime number calculator");
	    MinimizeBox();
		SetRect(0, 0, 400, 122);
		
		_progBar.SetTotal(100);
		_calcButton <<= THISBACK(Click);
		
		*this
			<< _numString.HSizePos(10,10).TopPos(10,25)
			<< _infoLabel.SetLabel("Press GO to calculate Nth prime").HSizePos(10,10).TopPos(35,25)
			<< _progBar.Percent(true).HSizePos(10,10).TopPos(65,20)
			<< _calcButton.SetLabel("GO").HSizePos(10,10).TopPos(90,25)
		;
	}
	
	typedef std::shared_ptr< templatious::VirtualPack > SharedMessage;
	
	void message(SharedMessage msg) override {
		{
		std::lock_guard< std::mutex > lg(_mtx);
		_eventQueue.push_back(msg);
		}
		Refresh();
	}
	
	void message(templatious::VirtualPack& msg) override {
		ProcessMessage(msg);
	}
	
	void Paint(Draw& w) override {
		ProcessQueue();
	}
private:
	void Click() {
		typedef MainWindowInterface Msg;
		auto msg = SF::vpack< Msg::OutButtonClicked >(
			Msg::OutButtonClicked()
		);
		_dvmf.tryMatch(msg);
	}

	typedef std::unique_ptr< templatious::VirtualMatchFunctor > HandlerPtr;

	void ProcessMessage(templatious::VirtualPack& msg) {
		_handler->tryMatch(msg);
	}

    HandlerPtr MakeHandler() {
        typedef MainWindowInterface Msg;
        return SF::virtualMatchFunctorPtr(
            SF::virtualMatch< Msg::InAttachCallback, HandlerPtr >(
	            [=](Msg::InAttachCallback,HandlerPtr& h) {
	                this->_dvmf.attach(std::move(h));
	            }
	        ),
	        SF::virtualMatch< Msg::InSetButtonEnabled, bool >(
	            [=](Msg::InSetButtonEnabled,bool value) {
	                this->_calcButton.SetEditable(value);
	            }
	        ),
	        SF::virtualMatch< Msg::InSetStatusText, const std::string >(
	            [=](Msg::InSetStatusText,const std::string& str) {
	                this->_infoLabel.SetText(str.c_str());
	            }
	        ),
	        SF::virtualMatch< Msg::InSetProgress, const int >(
	            [=](Msg::InSetProgress,int prog) {
	                this->_progBar.Set(prog);
	            }
	        ),
	        SF::virtualMatch< Msg::InShow >(
	            [=](Msg::InShow) {
	                this->Show(true);
	            }
	        ),
	        SF::virtualMatch< Msg::QueryLabelText, std::string >(
	            [=](Msg::QueryLabelText, std::string& s) {
	                String out = (String) this->_numString.GetData();
	                s = out.Begin();
	            }
	        )
        );
    }

	void ProcessQueue() {
		std::vector< SharedMessage > steal;
		{
		std::lock_guard< std::mutex > lg(_mtx);
		steal = std::move(_eventQueue);
		}

	    TEMPLATIOUS_FOREACH(auto& i,steal) {
	        ProcessMessage(*i);
	    }
	}

    std::vector< SharedMessage > _eventQueue;
    std::mutex _mtx;
    HandlerPtr _handler;
    templatious::DynamicVMatchFunctor _dvmf;
    
    // GUI
    Button _calcButton;
    EditString _numString;
    Label _infoLabel;
    ProgressIndicator _progBar;
};

GUI_APP_MAIN
{
    auto mw = std::make_shared<MainCalcWindow>();
    initDomain(mw);
	mw->Run();
}
