
// Run_in.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols



// CRun_inApp:
// See Run_in.cpp for the implementation of this class
//

/*==================== Structure ==========================*/
/* Main UI display ID for all station use */
#pragma pack (1)
struct st_UIIDparameter
{
	int     i_parameterNum;          // Parameter num (NUM_TRANSFER_PARAMETER)
	bool    b_tsFlag;	             // TS_Flag
	CString str_firstProcess;		 // First process(station)
	CString str_station;             // Station (DL,FT...)
	CString str_modelName;           // Model Name
	CString str_line;                // Line (T-Line,A-Line...)
	CString str_pcba;
	CString str_so;
	CString str_partNumber;
	CString str_toolVer;
	CString str_employee;
	CString str_factoryVer;
	CString str_userLoadVer;
	CString str_imagePath;
	CString str_soType;
	CString str_segment;
	CString str_CRType;
};
#pragma pack()

class CRun_inApp : public CWinAppEx
{
public:
	CRun_inApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CRun_inApp theApp;