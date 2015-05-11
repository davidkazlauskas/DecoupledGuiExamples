/***************************************************************
 * Name:      NthPrimeNumberGeneratorApp.cpp
 * Purpose:   Code for Application Class
 * Author:    David Kazlauskas (david@templatious.org)
 * Created:   2015-04-30
 * Copyright: David Kazlauskas (www.templatious.org)
 * License:
 **************************************************************/

#include "NthPrimeNumberGeneratorApp.h"
#include "domain.h"

//(*AppHeaders
#include "NthPrimeNumberGeneratorMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(NthPrimeNumberGeneratorApp);

bool NthPrimeNumberGeneratorApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
        auto ptr = std::make_shared< NthPrimeNumberGeneratorDialog >(nullptr,-1);
    	SetTopWindow(ptr.get());
    	initDomain(ptr);

    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
