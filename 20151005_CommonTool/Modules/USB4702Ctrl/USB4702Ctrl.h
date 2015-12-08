// USB4702_CtrlDLL.h : main header file for the USB4702_CtrlDLL DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CUSB4702CtrlApp
// See USB4702Ctrl.cpp for the implementation of this class
//

class CUSB4702CtrlApp : public CWinApp
{
public:
	CUSB4702CtrlApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
