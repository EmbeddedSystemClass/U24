
// UnifyUI_FacTestTool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CUnifyUI_FacTestToolApp:
// See UnifyUI_FacTestTool.cpp for the implementation of this class
//

class CUnifyUI_FacTestToolApp : public CWinAppEx
{
public:
	CUnifyUI_FacTestToolApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CUnifyUI_FacTestToolApp theApp;