
// MFCApplication1_Multi_Alpha.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCApplication1_Multi_AlphaApp:
// See MFCApplication1_Multi_Alpha.cpp for the implementation of this class
//

class CMFCApplication1_Multi_AlphaApp : public CWinApp
{
public:
	CMFCApplication1_Multi_AlphaApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication1_Multi_AlphaApp theApp;