
#include "stdafx.h"
#include "UnifyUI_FacTestTool.h"
#include "UnifyUI_FacTestToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef UNICODE
#define argv __wargv
#else
#define argv __argv
#endif //UNICODE

BEGIN_MESSAGE_MAP(CUnifyUI_FacTestToolApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CUnifyUI_FacTestToolApp::CUnifyUI_FacTestToolApp()
{

}

CUnifyUI_FacTestToolApp theApp;

BOOL CUnifyUI_FacTestToolApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinAppEx::InitInstance();
	AfxEnableControlContainer();
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	/* Check duplicated application startup */
	HANDLE h_event = NULL;
	h_event = CreateEvent(NULL, FALSE, FALSE, _T("UnifyUI_FacTestTool"));
	if(h_event)
	{
		if(ERROR_ALREADY_EXISTS == GetLastError())
		{
			return FALSE;
		}
	}

	/* MainUI dialog */
	CUnifyUI_FacTestToolDlg dlg;
	m_pMainWnd = &dlg;

#ifdef FACTORY_MODE
	st_UIIDparameter st_tempIDParameter;
	if (__argc >= (NUM_TRANSFER_PARAMETER + 1))
	{
		int i_index = 1;
		st_tempIDParameter.i_parameterNum = __argc-1;
		st_tempIDParameter.str_so.Format(_T("%s"), argv[i_index++]);
		st_tempIDParameter.str_employee.Format(_T("%s"), argv[i_index++]);		
		st_tempIDParameter.str_userLoadVer.Format(_T("%s"), argv[i_index++]);
		st_tempIDParameter.str_toolVer.Format(_T("%s"), argv[i_index++]);
		st_tempIDParameter.str_imagePath.Format(_T("%s"), argv[i_index++]);
		if (__argc >= (NUM_TRANSFER_PARAMETER + 2))
			st_tempIDParameter.str_accessoryList.Format(_T("%s"), argv[i_index++]); //20140722. accessorylist
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

	}
	else if (nResponse == IDCANCEL)
	{

	}

	return FALSE;
}
