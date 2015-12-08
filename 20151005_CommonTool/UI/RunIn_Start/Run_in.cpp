
// Run_in.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Run_in.h"
#include "Run_inDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRun_inApp

BEGIN_MESSAGE_MAP(CRun_inApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRun_inApp construction

CRun_inApp::CRun_inApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CRun_inApp object

CRun_inApp theApp;


// CRun_inApp initialization

BOOL CRun_inApp::InitInstance()
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

	CWinAppEx::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CRun_inDlg dlg;
	m_pMainWnd = &dlg;


	#ifdef FACTORY_MODE
	st_UIIDparameter st_tempIDParameter;
	if (__argc >= (NUM_TRANSFER_PARAMETER + 1))
	{
		int i_index = 1;
		st_tempIDParameter.i_parameterNum = __argc-1;
		st_tempIDParameter.str_so.Format(_T("%s"), __argv[i_index++]);
		st_tempIDParameter.str_employee.Format(_T("%s"), __argv[i_index++]);		
		st_tempIDParameter.str_userLoadVer.Format(_T("%s"), __argv[i_index++]);
		st_tempIDParameter.str_toolVer.Format(_T("%s"), __argv[i_index++]);
		st_tempIDParameter.str_imagePath.Format(_T("%s"), __argv[i_index++]);
	}
	else
	{
		CString str_message = _T("");
		str_message = _T("Entrance tool 传递的参数不正确!\nThe parameters Transferred from Entrance tool are not correct!");
		AfxMessageBox(str_message, MB_OK);
		return FALSE;
	}
	dlg.SetUIIDParameter(st_tempIDParameter);
#endif

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
