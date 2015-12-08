
// KeyPartsLink.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "KeyPartsLink.h"
#include "KeyPartsLinkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKeyPartsLinkApp

BEGIN_MESSAGE_MAP(CKeyPartsLinkApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CKeyPartsLinkApp construction

CKeyPartsLinkApp::CKeyPartsLinkApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_hLib = 0;
}

CKeyPartsLinkApp::~CKeyPartsLinkApp()
{
	if (m_hLib)
	{
		FreeLibrary(m_hLib);
	}

}

// The one and only CKeyPartsLinkApp object

CKeyPartsLinkApp theApp;


// CKeyPartsLinkApp initialization

BOOL CKeyPartsLinkApp::InitInstance()
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

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	/* Check duplicated application startup */
	HANDLE h_event = NULL;
	h_event = CreateEvent(NULL, FALSE, FALSE, _T("KeyPartsLink"));
	if(h_event)
	{
		if(ERROR_ALREADY_EXISTS == GetLastError())
		{
			return FALSE;
		}
	}

	if (!InitialDLL())
	{
		exit(0);
	}
	
	CKeyPartsLinkDlg dlg;
	m_pMainWnd = &dlg;

	//Below Parameter parse Code is Copied from UnifyUI_FacTestTool.cpp
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

		dlg.m_InitialDlg.m_strSONumber = st_tempIDParameter.str_so;
		dlg.m_InitialDlg.m_strEmployeeID = st_tempIDParameter.str_employee;
		dlg.m_InitialDlg.m_b_IsUsedEntranceTool = true;

	}
	/*else
	{
		CString str_message = _T("");
		str_message = _T("Entrance tool 传递的参数不正确!\nThe parameters Transferred from Entrance tool are not correct!");
		AfxMessageBox(str_message, MB_OK);
		exit(0);
		//return FALSE;
	}*/

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

	// Release Employee ID status to be "Y" 
	dlg.UpdateEmployeeID_StatusEx( dlg.m_InitialDlg.m_strEmployeeID, _T("Y"),dlg.m_InitialDlg.m_strPCName);

	return FALSE;
}

bool CKeyPartsLinkApp::InitialDLL(void)
{
	m_hLib = ::LoadLibrary( _T("DBAccess.dll") );

	if (!m_hLib)
	{
		AfxMessageBox(_T("Load DBAccess.dll Fail!") ) ;
		return false;
	}

	pConnectDB = (ConnectDB)::GetProcAddress( (HMODULE)m_hLib, "ConnectDB") ;
	if (!pConnectDB) 
	{
		AfxMessageBox(_T("Load DBAccess.dll ConnectDB Function Fail!") ) ;
		
		return false;
	}

	pCloseDB = (CloseDB)::GetProcAddress( (HMODULE)m_hLib, "CloseDB") ;
	if (!pCloseDB) 
	{
		AfxMessageBox(_T("Load DBAccess.dll CloseDB Function Fail!") ) ;
		return false;
	}

	pQueryEmployeeID_Right_V2 = (QueryEmployeeID_Right_V2)::GetProcAddress( (HMODULE)m_hLib, "QueryEmployeeID_Right_V2") ;
	if (!pQueryEmployeeID_Right_V2) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QueryEmployeeID_Right_V2 Function Fail!") ) ;
		return false;
	}

	pUpdateEmployeeID_Status = (UpdateEmployeeID_Status)::GetProcAddress( (HMODULE)m_hLib, "UpdateEmployeeID_Status") ;
	if (!pUpdateEmployeeID_Status) 
	{
		AfxMessageBox(_T("Load DBAccess.dll UpdateEmployeeID_Status Function Fail!") ) ;
		return false;
	}

	/*pQuerySO_Info = (QuerySO_Info)::GetProcAddress( (HMODULE)m_hLib, "QuerySO_Info") ;
	if (!pQuerySO_Info) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QuerySO_Info Function Fail!") ) ;
		return false;
	}*/

	pQuerySO_Info_All_V4 = (QuerySO_Info_All_V4)::GetProcAddress( (HMODULE)m_hLib, "QuerySO_Info_All_V4") ;
	if (!pQuerySO_Info_All_V4) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QuerySO_Info_All_V4 Function Fail!") ) ;
		return false;
	}

	pCheckSO = (CheckSO)::GetProcAddress( (HMODULE)m_hLib, "CheckSO") ;
	if (!pCheckSO) 
	{
		AfxMessageBox(_T("Load DBAccess.dll CheckSO Function Fail!") ) ;
		return false;
	}

	pQueryStation_Info = (QueryStation_Info)::GetProcAddress( (HMODULE)m_hLib, "QueryStation_Info") ;
	if (!pQueryStation_Info) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QueryStation_Info Function Fail!") ) ;
		return false;
	}

	pQueryKeyparts_Info = (QueryKeyparts_Info)::GetProcAddress( (HMODULE)m_hLib, "QueryKeyparts_Info") ;
	if (!pQueryKeyparts_Info) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QueryKeyparts_Info Function Fail!") ) ;
		return false;
	}

	pQueryPNinBACH_BySO = (QueryPNinBACH_BySO)::GetProcAddress( (HMODULE)m_hLib, "QueryPNinBACH_BySO") ;
	if (!pQueryPNinBACH_BySO) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QueryPNinBACH_BySO Function Fail!") ) ;
		return false;
	}

	pCheckKeyparts_duplicate = (CheckKeyparts_duplicate)::GetProcAddress( (HMODULE)m_hLib, "CheckKeyparts_duplicate") ;
	if (!pCheckKeyparts_duplicate) 
	{
		AfxMessageBox(_T("Load DBAccess.dll CheckKeyparts_duplicate Function Fail!") ) ;
		return false;
	}

	pQuerySegmentProcess = (QuerySegmentProcess)::GetProcAddress( (HMODULE)m_hLib, "QuerySegmentProcess") ;
	if (!pQuerySegmentProcess) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QuerySegmentProcess Function Fail!") ) ;
		return false;
	}

	pGetPicasso = (GetPicasso)::GetProcAddress( (HMODULE)m_hLib, "GetPicasso") ;
	if (!pGetPicasso) 
	{
		AfxMessageBox(_T("Load DBAccess.dll GetPicasso Function Fail!") ) ;
		return false;
	}

	pQueryIMEI_ByPicasso = (QueryIMEI_ByPicasso)::GetProcAddress( (HMODULE)m_hLib, "QueryIMEI_ByPicasso") ;
	if (!pQueryIMEI_ByPicasso) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QueryIMEI_ByPicasso Function Fail!") ) ;
		return false;
	}

	pQuerySOCount = (QuerySOCount)::GetProcAddress( (HMODULE)m_hLib, "QuerySOCount") ;
	if (!pQuerySOCount) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QuerySOCount Function Fail!") ) ;
		return false;
	}

	pCheckRouting = (CheckRouting)::GetProcAddress( (HMODULE)m_hLib, "CheckRouting") ;
	if (!pCheckRouting) 
	{
		AfxMessageBox(_T("Load DBAccess.dll CheckRouting Function Fail!") ) ;
		return false;
	}

	pCheckRouting_New = (CheckRouting_New)::GetProcAddress( (HMODULE)m_hLib, "CheckRouting_V2") ;
	if (!pCheckRouting_New) 
	{
		AfxMessageBox(_T("Load DBAccess.dll CheckRouting_New Function Fail!") ) ;
		return false;
	}

	pInsertFAIL_data = (InsertFAIL_data)::GetProcAddress( (HMODULE)m_hLib, "InsertFAIL_data") ;
	if (!pInsertFAIL_data) 
	{
		AfxMessageBox(_T("Load DBAccess.dll InsertFAIL_data Function Fail!") ) ;
		return false;
	}

	pInsertPASS_data = (InsertPASS_data)::GetProcAddress( (HMODULE)m_hLib, "InsertPASS_data") ;
	if (!pInsertPASS_data) 
	{
		AfxMessageBox(_T("Load DBAccess.dll InsertPASS_data Function Fail!") ) ;
		return false;
	}

	pInsertKeypartsInfo = (InsertKeypartsInfo)::GetProcAddress( (HMODULE)m_hLib, "InsertKeypartsInfo") ;
	if (!pInsertKeypartsInfo) 
	{
		AfxMessageBox(_T("Load DBAccess.dll InsertKeypartsInfo Function Fail!") ) ;
		return false;
	}

	pSetValue = (SetValue)::GetProcAddress( (HMODULE)m_hLib, "SetValue") ;
	if (!pSetValue) 
	{
		AfxMessageBox(_T("Load DBAccess.dll SetValue Function Fail!") ) ;
		return false;
	}

	pInsertOOB_data = (InsertOOB_data)::GetProcAddress( (HMODULE)m_hLib, "InsertOOB_data") ;
	if (!pInsertOOB_data) 
	{
		AfxMessageBox(_T("Load DBAccess.dll InsertOOB_data Function Fail!") ) ;
		return false;
	}

	pInsertCRHoldInfo = (InsertCRHoldInfo)::GetProcAddress( (HMODULE)m_hLib, "InsertCRHoldInfo") ;
	if (!pInsertCRHoldInfo) 
	{
		AfxMessageBox(_T("Load DBAccess.dll InsertCRHoldInfo Function Fail!") ) ;
		return false;
	}

	pQueryholdInfo = (QueryholdInfo)::GetProcAddress( (HMODULE)m_hLib, "QueryholdInfo") ;
	if (!pQueryholdInfo) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QueryholdInfo Function Fail!") ) ;
		return false;
	}

	pQueryCRHoldInfo = (QueryCRHoldInfo)::GetProcAddress( (HMODULE)m_hLib, "QueryCRHoldInfo") ;
	if (!pQueryCRHoldInfo) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QueryCRHoldInfo Function Fail!") ) ;
		return false;
	}

	pCheckProcessTestDate = (CheckProcessTestDate)::GetProcAddress( (HMODULE)m_hLib, "CheckProcessTestDate") ;
	if (!pCheckProcessTestDate) 
	{
		AfxMessageBox(_T("Load DBAccess.dll CheckProcessTestDate Function Fail!") ) ;
		return false;
	}

	pQueryKeypartsByID = (QueryKeypartsByID)::GetProcAddress( (HMODULE)m_hLib, "QueryKeypartsByID") ;
	if (!pQueryKeypartsByID) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QueryKeypartsByID Function Fail!") ) ;
		return false;
	}

	pQuery_PCBA_SerialNO = (Query_PCBA_SerialNO)::GetProcAddress( (HMODULE)m_hLib, "Query_PCBA_SerialNO") ;
	if (!pQuery_PCBA_SerialNO) 
	{
		AfxMessageBox(_T("Load DBAccess.dll Query_PCBA_SerialNO Function Fail!") ) ;
		return false;
	}

	pQuery_PCBA_NO = (Query_PCBA_NO)::GetProcAddress( (HMODULE)m_hLib, "Query_PCBA_NO") ;
	if (!pQuery_PCBA_NO) 
	{
		AfxMessageBox(_T("Load DBAccess.dll Query_PCBA_NO Function Fail!") ) ;
		return false;
	}

	pQueryNAL = (QueryNAL)::GetProcAddress( (HMODULE)m_hLib, "QueryNAL") ;
	if (!pQueryNAL) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QueryNAL Function Fail!") ) ;
		return false;
	}

	pQueryIMEIbyNAL = (QueryIMEIbyNAL)::GetProcAddress( (HMODULE)m_hLib, "QueryIMEIbyNAL") ;
	if (!pQueryIMEIbyNAL) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QueryIMEIbyNAL Function Fail!") ) ;
		return false;
	}

	pCheckNAL_Validity = (CheckNAL_Validity)::GetProcAddress( (HMODULE)m_hLib, "CheckNAL_Validity") ;
	if (!pCheckNAL_Validity) 
	{
		AfxMessageBox(_T("Load DBAccess.dll CheckNAL_Validity Function Fail!") ) ;
		return false;
	}

	pCheckBadge_Validity = (CheckBadge_Validity)::GetProcAddress( (HMODULE)m_hLib, "CheckBadge_Validity") ;
	if (!pCheckBadge_Validity) 
	{
		AfxMessageBox(_T("Load DBAccess.dll CheckBadge_Validity Function Fail!") ) ;
		return false;
	}

	//****For Wolf key part link	Dave.Wu 20111007
	pQueryRuninID = (QueryRuninID)::GetProcAddress( (HMODULE)m_hLib, "QueryRuninID") ;
	if (!pQueryRuninID) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QueryRuninID Function Fail!") ) ;
		return false;
	}
	//****Dave.Wu 20111007

	pGetIMEI_Info = (GetIMEI_Info)::GetProcAddress( (HMODULE)m_hLib, "GetIMEI_Info");
	if (!pGetIMEI_Info) 
	{
		AfxMessageBox(_T("Load DBAccess.dll GetIMEI_Info Function Fail!") ) ;
		return false;
	}

	pUnlinkKeyparts = (UnlinkKeyparts)::GetProcAddress( (HMODULE)m_hLib, "UnlinkKeyparts");
	if (!pUnlinkKeyparts) 
	{
		AfxMessageBox(_T("Load DBAccess.dll UnlinkKeyparts Function Fail!") ) ;
		return false;
	}

	pUnlinkKeyparts_FA = (UnlinkKeyparts_FA)::GetProcAddress( (HMODULE)m_hLib, "UnlinkKeyparts_FA");
	if (!pUnlinkKeyparts_FA) 
	{
		AfxMessageBox(_T("Load DBAccess.dll UnlinkKeyparts_FA Function Fail!") ) ;
		return false;
	}

	pQueryIMEIStatus = (QueryIMEIStatus)::GetProcAddress( (HMODULE)m_hLib, "QueryIMEIStatus");
	if (!pQueryIMEIStatus) 
	{
		AfxMessageBox(_T("Load DBAccess.dll QueryIMEIStatus Function Fail!") ) ;
		return false;
	}
	return true;
}

