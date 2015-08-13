// UnifyUI_ServiceTool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CUnifyUI_ServiceToolApp:
// See UnifyUI_ServiceTool.cpp for the implementation of this class
//

class CUnifyUI_ServiceToolApp : public CWinApp
{
public:
	CUnifyUI_ServiceToolApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CUnifyUI_ServiceToolApp theApp;