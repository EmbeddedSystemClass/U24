// FAV.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FAV.h"
#include "FAVDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* 20120314,Sunlight add for use entrance tool */
#ifdef UNICODE
#define argv __wargv
#else
#define argv __argv
#endif 

#define  NUM_TRANSFER_PARAMETER 5
/* 20120314,Sunlight add end */

// CFAVApp

BEGIN_MESSAGE_MAP(CFAVApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFAVApp construction

CFAVApp::CFAVApp()
{
	
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CFAVApp::~CFAVApp()
{
}

// The one and only CFAVApp object

CFAVApp theApp;


// CFAVApp initialization

BOOL CFAVApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    st_UIIDparameter st_tempIDParameter;

	if (__argc >= (NUM_TRANSFER_PARAMETER + 1))
	{
		int i_index = 1;
//		st_tempIDParameter.i_parameterNum = __argc-1;
		st_tempIDParameter.str_so.Format(_T("%s"), argv[i_index++]);
		st_tempIDParameter.str_employee.Format(_T("%s"), argv[i_index++]);
		st_tempIDParameter.str_userLoadVer.Format(_T("%s"), argv[i_index++]);
		st_tempIDParameter.str_toolVer.Format(_T("%s"), argv[i_index++]);
		st_tempIDParameter.str_imagePath.Format(_T("%s"), argv[i_index++]);
	}
	else
	{
		CString str_message = _T("");
		str_message = _T("Entrance tool 传递的参数不正确!\nThe parameters Transferred from entrance tool are not correct!");
		AfxMessageBox(str_message, MB_OK);
		return FALSE;
	}
	
	CFAVDlg dlg;
	m_pMainWnd = &dlg;

	dlg.SetUIIDParameter(st_tempIDParameter);

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}



