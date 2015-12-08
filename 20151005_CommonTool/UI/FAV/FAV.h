/**********************************************************
*   Copyright (C) 2011 Qisda Corporation
*   All Rights Reserved.
*   Class name : CPdaRec                                                     
*   Summary    : Record management for FA station and QA team
*   Version     Author            Date                Abstract  
*   1.0         Nancy.Yang        2011/05/12          First version
************************************************************
*/

// FAV.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CFAVApp:
// See FAV.cpp for the implementation of this class
//

class CFAVApp : public CWinApp
{
public:
	CFAVApp();
	virtual ~CFAVApp();

// Overrides
	public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CFAVApp theApp;