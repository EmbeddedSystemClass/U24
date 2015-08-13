// SPU_UI_Extend.h : main header file for the SPU_UI_Extend DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSPU_UI_ExtendApp
// See SPU_UI_Extend.cpp for the implementation of this class
//

class CSPU_UI_ExtendApp : public CWinApp
{
public:
	CSPU_UI_ExtendApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

