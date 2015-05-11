/***************************************************************
 * Name:      NthPrimeNumberGeneratorMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    David Kazlauskas (david@templatious.org)
 * Created:   2015-04-30
 * Copyright: David Kazlauskas (www.templatious.org)
 * License:
 **************************************************************/

#include "NthPrimeNumberGeneratorMain.h"
#include "mainwindow_interface.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(NthPrimeNumberGeneratorDialog)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(NthPrimeNumberGeneratorDialog)
const long NthPrimeNumberGeneratorDialog::ID_TEXTCTRL1 = wxNewId();
const long NthPrimeNumberGeneratorDialog::ID_STATICTEXT1 = wxNewId();
const long NthPrimeNumberGeneratorDialog::ID_GAUGE1 = wxNewId();
const long NthPrimeNumberGeneratorDialog::ID_BUTTON1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(NthPrimeNumberGeneratorDialog,wxDialog)
    //(*EventTable(NthPrimeNumberGeneratorDialog)
    //*)
    EVT_BUTTON(ID_BUTTON1, NthPrimeNumberGeneratorDialog::OnButton1)
    EVT_PAINT(NthPrimeNumberGeneratorDialog::OnPaint)
END_EVENT_TABLE()

TEMPLATIOUS_TRIPLET_STD;

NthPrimeNumberGeneratorDialog::NthPrimeNumberGeneratorDialog(wxWindow* parent,wxWindowID id)
    : _hndl(makeHandler()), _dvmf(true)
{
    //(*Initialize(NthPrimeNumberGeneratorDialog)
    Create(parent, wxID_ANY, _("Nth prime number generator"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(400,122));
    SetMinSize(wxSize(400,122));
    SetMaxSize(wxSize(400,122));
    TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxPoint(8,8), wxSize(384,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Press GO to calculate nth prime number"), wxPoint(16,40), wxSize(368,16), 0, _T("ID_STATICTEXT1"));
    Gauge1 = new wxGauge(this, ID_GAUGE1, 100, wxPoint(8,64), wxSize(384,24), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    Button1 = new wxButton(this, ID_BUTTON1, _("GO"), wxPoint(8,88), wxSize(384,29), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    //*)
}

NthPrimeNumberGeneratorDialog::~NthPrimeNumberGeneratorDialog()
{
    //(*Destroy(NthPrimeNumberGeneratorDialog)
    //*)
}

void NthPrimeNumberGeneratorDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void NthPrimeNumberGeneratorDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

auto NthPrimeNumberGeneratorDialog::makeHandler() -> Handler {
    typedef MainWindowInterface Msg;
    return SF::virtualMatchFunctorPtr(
        SF::virtualMatch< Msg::InAttachCallback, Handler >(
            [=](Msg::InAttachCallback,Handler& h) {
                this->_dvmf.attach(std::move(h));
            }
        ),
        SF::virtualMatch< Msg::InSetButtonEnabled, bool >(
            [=](Msg::InSetButtonEnabled,bool value) {
                this->Button1->Enable(value);
            }
        ),
        SF::virtualMatch< Msg::InSetStatusText, const std::string >(
            [=](Msg::InSetStatusText,const std::string& str) {
                wxString conv = wxString::From8BitData(str.c_str());
                this->StaticText1->SetLabel(conv);
            }
        ),
        SF::virtualMatch< Msg::InSetProgress, const int >(
            [=](Msg::InSetProgress,int prog) {
                this->Gauge1->SetValue(prog);
            }
        ),
        SF::virtualMatch< Msg::InShow >(
            [=](Msg::InShow) {
                this->ShowModal();
            }
        ),
        SF::virtualMatch< Msg::QueryLabelText, std::string >(
            [=](Msg::QueryLabelText, std::string& s) {
                wxString out = this->TextCtrl1->GetValue();
                s = std::string(out.mb_str());
            }
        )
    );
}

typedef std::lock_guard< std::mutex > MtxGuard;


void NthPrimeNumberGeneratorDialog::OnButton1(wxCommandEvent& event) {
    typedef MainWindowInterface Msg;
    auto pack = SF::vpack< Msg::OutButtonClicked >(Msg::OutButtonClicked());
    _dvmf.tryMatch(pack);
}

void NthPrimeNumberGeneratorDialog::processMessage(templatious::VirtualPack& pack) {
    _hndl->tryMatch(pack);
}

void NthPrimeNumberGeneratorDialog::processQueue() {
    std::vector< MsgPtr > steal;
    {
    MtxGuard g(_mtx);
    steal = std::move(_queue);
    // just stole the whole queue.
    // nothing happened, really!
    }

    TEMPLATIOUS_FOREACH(auto& i,steal) {
        processMessage(*i);
    }
}

void NthPrimeNumberGeneratorDialog::OnPaint(wxPaintEvent& event) {
    processQueue();
}

void NthPrimeNumberGeneratorDialog::message(templatious::VirtualPack& msg) {
    processMessage(msg);
}

void NthPrimeNumberGeneratorDialog::message(MsgPtr msg) {
    {
    MtxGuard g(_mtx);
    _queue.push_back(msg);
    }
    Refresh();
}
