/***************************************************************
 * Name:      NthPrimeNumberGeneratorMain.h
 * Purpose:   Defines Application Frame
 * Author:    David Kazlauskas (david@templatious.org)
 * Created:   2015-04-30
 * Copyright: David Kazlauskas (www.templatious.org)
 * License:
 **************************************************************/

#ifndef NTHPRIMENUMBERGENERATORMAIN_H
#define NTHPRIMENUMBERGENERATORMAIN_H

//(*Headers(NthPrimeNumberGeneratorDialog)
#include <wx/gauge.h>
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include <templatious/FullPack.hpp>

#include "messeagable.h"

class NthPrimeNumberGeneratorDialog: public wxDialog, public Messageable
{
    public:

        NthPrimeNumberGeneratorDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~NthPrimeNumberGeneratorDialog();

        typedef std::shared_ptr< templatious::VirtualPack > MsgPtr;
        typedef std::unique_ptr< templatious::VirtualMatchFunctor > Handler;

        void message(templatious::VirtualPack& msg) override;
        void message(MsgPtr msg) override;

        void OnPaint(wxPaintEvent& event);

    private:

        //(*Handlers(NthPrimeNumberGeneratorDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(NthPrimeNumberGeneratorDialog)
        static const long ID_TEXTCTRL1;
        static const long ID_STATICTEXT1;
        static const long ID_GAUGE1;
        static const long ID_BUTTON1;
        //*)

        //(*Declarations(NthPrimeNumberGeneratorDialog)
        wxButton* Button1;
        wxStaticText* StaticText1;
        wxTextCtrl* TextCtrl1;
        wxGauge* Gauge1;
        //*)

        void OnButton1(wxCommandEvent& event);

        void processQueue();
        void processMessage(templatious::VirtualPack& pack);
        Handler makeHandler();

        std::mutex _mtx;
        std::vector< MsgPtr > _queue;

        Handler _hndl;
        templatious::DynamicVMatchFunctor _dvmf;

        DECLARE_EVENT_TABLE()
};

#endif // NTHPRIMENUMBERGENERATORMAIN_H
