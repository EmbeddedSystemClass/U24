// TestToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestTool.h"
#include "TestToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/**********************************************************
Global function and variable for callback notify
***********************************************************/
CTestToolDlg* g_pTestToolDlg = NULL;

void NotifyToTestToolDlg(const char* szEvent, long nParam)
{
	if (NULL == g_pTestToolDlg)
	{
		return;
	}

	g_pTestToolDlg->Event(szEvent, nParam);
}

// CTestToolDlg dialog
CTestToolDlg::CTestToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	g_pTestToolDlg = this;
	m_h_module = NULL;
	m_p_FTTInterface = NULL;

	m_map_Picasso.clear();
	m_map_slotPICSData.clear();
	m_map_COMPort.clear();
	m_map_runResult.clear();
	m_map_running.clear();
	m_map_testItem.clear();

	m_iRecordTotal = 0;
	m_iRecordPass = 0;
	m_iRecordFail = 0;
	m_b_sync = false;
	m_iStatus = 0;
}

CTestToolDlg::~CTestToolDlg()
{
	/* Free DLL */
	if (m_h_module)
	{
		/* End */
		m_p_FTTInterface->End();

		p_fn_deleteFacTestToolInterface p_delInterface = NULL;
		p_delInterface = (p_fn_deleteFacTestToolInterface)::GetProcAddress(m_h_module, (LPCSTR)DLL_DEL_INTERFACE);
		if(p_delInterface && m_p_FTTInterface)
		{
			p_delInterface(m_p_FTTInterface);
		}

		::FreeLibrary(m_h_module);
		m_h_module = NULL;
	}
}

void CTestToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	for(int i = 0; i < MAX_SLOT; i++)
	{
		DDX_Control(pDX, IDC_STATIC_SLOT0_COM + i, m_label_deviceCOM[i]);
		DDX_Control(pDX, IDC_STATIC_SLOT0_RESULT + i, m_label_deviceResult[i]);
		DDX_Control(pDX, IDC_EDIT_SLOT0_PICASSO + i, m_edit_devicePicasso[i]);
		DDX_Control(pDX, IDC_EDIT_SLOT0_QCSSN + i, m_edit_deviceQCSSN[i]);
	}

	DDX_Control(pDX, IDC_LIST_TESTITEM, m_listbox_testItem);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_Label_Title);
	DDX_Control(pDX, IDC_STATIC_DEVICE_COM, m_label_device_title_COM);
	DDX_Control(pDX, IDC_STATIC_DEVICE_RESULT, m_label_device_title_Result);
	DDX_Control(pDX, IDC_STATIC_DEVICE_PICASSO, m_label_device_title_Picasso);

	DDX_Control(pDX, IDC_LIST_MESSAGEPANEL, m_listbox_messagepanel);

	DDX_Control(pDX, IDC_EDIT_RECORD_TOTAL, m_edit_Record_Total);
	DDX_Control(pDX, IDC_EDIT_RECORD_PASS, m_edit_Record_Pass);
	DDX_Control(pDX, IDC_EDIT_RECORD_FAIL, m_edit_Record_Fail);

	DDX_Control(pDX, IDOK, m_bBtnGo);
	DDX_Control(pDX, IDCLOSE, m_bBtnExit);
}

BEGIN_MESSAGE_MAP(CTestToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDOK, &CTestToolDlg::OnBtnGo)
	ON_BN_CLICKED(IDCLOSE, &CTestToolDlg::OnBtnExit)
	ON_EN_CHANGE(IDC_EDIT_SLOT0_PICASSO, &CTestToolDlg::OnEnChangeEditSlot0Picasso)
	ON_EN_CHANGE(IDC_EDIT_SLOT0_QCSSN, &CTestToolDlg::OnEnChangeEditSlot0Qcssn)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CTestToolDlg message handlers

BOOL CTestToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	/* Init */
	if (!Init())
	{
		CDialog::OnCancel();

		return FALSE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestToolDlg::OnBtnGo()
{
#ifdef MI_MODE
	m_bBtnGo.EnableWindow(FALSE);
	m_bBtnExit.EnableWindow(FALSE);
	m_edit_devicePicasso[0].EnableWindow(FALSE);
	CString str_tmp;
	str_tmp.Format(_T("====================== Run BEGIN ==========================="));
	LogMsg(str_tmp);

	ResetResult();
	m_listbox_messagepanel.ResetContent();
	Run();
#else
#ifdef FA_MODE
	if ( 0 == m_iStatus )
	{	m_edit_devicePicasso[0].EnableWindow(FALSE);
		m_edit_deviceQCSSN[0].EnableWindow(TRUE);
		m_edit_deviceQCSSN[0].SetFocus();
		m_iStatus = 1;
	}
	else if( 1 == m_iStatus )
	{	m_bBtnGo.EnableWindow(FALSE);
		m_bBtnExit.EnableWindow(FALSE);
		m_edit_devicePicasso[0].EnableWindow(FALSE);
		m_edit_deviceQCSSN[0].EnableWindow(FALSE);
		CString str_tmp;
		str_tmp.Format(_T("====================== Run BEGIN ==========================="));
		LogMsg(str_tmp);

		ResetResult();
		m_listbox_messagepanel.ResetContent();

		m_iStatus = 0;
		Run();
	}
#else
#ifdef CSD_MODE
	if ( 0 == m_iStatus )
	{	m_edit_devicePicasso[0].EnableWindow(FALSE);
		m_edit_deviceQCSSN[0].EnableWindow(TRUE);
		m_edit_deviceQCSSN[0].SetFocus();
		m_iStatus = 1;
	}
	else if( 1 == m_iStatus )
	{	m_bBtnGo.EnableWindow(FALSE);
		m_bBtnExit.EnableWindow(FALSE);
		m_edit_devicePicasso[0].EnableWindow(FALSE);
		m_edit_deviceQCSSN[0].EnableWindow(FALSE);
		CString str_tmp;
		str_tmp.Format(_T("====================== Run BEGIN ==========================="));
		LogMsg(str_tmp);

		ResetResult();
		m_listbox_messagepanel.ResetContent();
		m_iStatus = 0;
		Run();
	}
#endif
#endif
#endif
}

void CTestToolDlg::OnBtnExit()
{
	CDialog::OnCancel();
}

bool CTestToolDlg::Init()
{
	Begin();

	/* Init UIControl */
	InitialUIControl();

	/* Load DLL */
	if (!LoadDLL())
	{
		ShowResultDlg(CommErr_UI_DLL_Load_Fail, _T("Load DLL fail!"));
		return false;
	}

	if (!SetParameterToDLL())
	{
		return false;
	}

	/* DLL Begin */
	if (!DLLBegin())
	{
		ShowResultDlg(CommErr_UI_Interface_Begin_Fail, _T("Run Begin fail!"));
		return false;
	}

	if (!GetParameterFromDLL())
	{
		return false;
	}

	/* UI Show COM Port */
	if (!UIShowCOMPort())
	{
		ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("COM Port number is null!"));
		return false;
	}

	/* Init UI */
	InitUI();
	IniRecord();

	/* Init test item list */
	if (!InitTestItemList())
	{
		ShowResultDlg(CommErr_UI_Init_TestItem_Fail, _T("Get TestItem fail!"));
		return false;
	}

	if (!InitRecoverItemList())
	{
		ShowResultDlg(CommErr_UI_Init_TestItem_Fail, _T("Get RecoverItem fail!"));
		return false;
	}

	return true;
}


void CTestToolDlg::Begin()
{
	/* Initial UI parameter */
	m_st_uiParameter.i_parameterNum   = 0;
	m_st_uiParameter.b_tsFlag         = false;
	m_st_uiParameter.str_firstProcess = _T("");

#define FA_MODE
#ifdef MI_MODE
	m_st_uiParameter.str_station      = _T("BBTEST");
#else
#ifdef FA_MODE
	m_st_uiParameter.str_station      = _T("IMEI");
#else
#ifdef CSD_MODE
	m_st_uiParameter.str_station      = _T("CSD_IMEI");
#else
	m_st_uiParameter.str_station      = _T("TEST");
#endif
#endif
#endif

	m_st_uiParameter.str_modelName    = _T("GLUFB1A");
	m_st_uiParameter.str_line         = _T("");
	m_st_uiParameter.str_pcba         = _T("");
	m_st_uiParameter.str_so           = _T("");
	m_st_uiParameter.str_partNumber   = _T("");
	m_st_uiParameter.str_toolVer      = _T("");
	m_st_uiParameter.str_employee     = _T("");
	m_st_uiParameter.str_factoryVer   = _T("1.0");
	m_st_uiParameter.str_userLoadVer  = _T("1.0");
	m_st_uiParameter.str_imagePath    = _T("");
	m_st_uiParameter.str_soType       = _T("");
	m_st_uiParameter.str_segment      = _T("");
	m_st_uiParameter.str_CRType      = _T("");
}

bool CTestToolDlg::LoadDLL()
{
	/* Load dll */
	CString str_dllName = DLLNAME;
	m_h_module = ::LoadLibrary(str_dllName);
	if (m_h_module == NULL)
	{
		LogMsg(_T("Load ") + str_dllName + _T(" Fail!"));
		return false;
	}

	/* Get function */
	p_fn_newFacTestToolInterface p_newInterface = NULL;
	p_newInterface = (p_fn_newFacTestToolInterface)::GetProcAddress(m_h_module, DLL_NEW_INTERFACE);
	if (p_newInterface == NULL)
	{
		return false;
	}

	if (!p_newInterface(&m_p_FTTInterface))
	{
		return false;
	}

	if (m_p_FTTInterface == NULL)
	{
		return false;
	}

	reg_FacTestToolInterface_callback();

	return true;
}

bool CTestToolDlg::SetParameterToDLL()
{
	CString strTemp;
	USES_CONVERSION;


	if (!m_p_FTTInterface->SetParameterValue(LOGPATH, T2A(LOG_PATH)))
	{
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("Set log path fail!"));
		return false;
	}

#ifdef RD_MODE
	if (!m_p_FTTInterface->SetParameterValue(TOOLMODE, "RD"))
	{
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("Set TOOLMODE fail!"));
		return false;
	}
#else
	if (!m_p_FTTInterface->SetParameterValue(TOOLMODE, "FACTORY"))
	{
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("Set TOOLMODE fail!"));
		return false;
	}
#endif


	/* Set Factory Version */
	strTemp.Format(_T("FACTORY_VERSION---%s"),m_st_uiParameter.str_factoryVer);
	LogMsg(strTemp);
	if (!m_p_FTTInterface->SetParameterValue(FACTORY_VERSION, T2A(m_st_uiParameter.str_factoryVer)))
	{
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("Set FACTORY_VERSION fail!"));
		return false;
	}

	CString str_imagePath;


	/* Set SW Version */
	strTemp.Format(_T("SW_VERSION---%s"),m_st_uiParameter.str_userLoadVer);
	LogMsg(strTemp);
	if (!m_p_FTTInterface->SetParameterValue(SW_VERSION, T2A(m_st_uiParameter.str_userLoadVer)))
	{
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("Set SW_VERSION fail!"));
		return false;
	}


	strTemp.Format(_T("STATION_NAME---%s"),m_st_uiParameter.str_station);
	LogMsg(strTemp);
	if (!m_p_FTTInterface->SetParameterValue(STATION_NAME, T2A(m_st_uiParameter.str_station)))
	{
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("Set STATION_NAME fail!"));
		return false;
	}

	strTemp.Format(_T("MODEL_NAME---%s"),m_st_uiParameter.str_modelName);
	LogMsg(strTemp);
	if (!m_p_FTTInterface->SetParameterValue(MODEL_NAME, T2A(m_st_uiParameter.str_modelName)))
	{
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("Set MODEL_NAME fail!"));
		return false;
	}

	return true;
}

bool CTestToolDlg::DLLBegin()
{
	/* Begin */
	if(!m_p_FTTInterface->Begin())
	{
		return false;
	}

	return true;
}

bool CTestToolDlg::GetParameterFromDLL()
{
	/* Get Tool Version */
	char sz_toolversion[30]={0};

	if (!m_p_FTTInterface->GetParameterValue(TOOL_VERSION, sz_toolversion,30))
	{
		ShowResultDlg(CommErr_UI_Interface_Getparameter_Fail, _T("Get TOOL_VERSION fail"));
		return false;
	}
	m_st_uiParameter.str_toolVer = ConvertString(sz_toolversion);

	return true;
}

bool CTestToolDlg::UIShowCOMPort()
{
	/* Get COM Port */
	char sz_comPort[MAX_BUFFER_SIZE] = {0};
	if (m_p_FTTInterface->GetParameterValue("COM", sz_comPort, sizeof(sz_comPort)) != true)
	{
		return false;
	}

	/* Convert char* to CString */
	CString str_temp;
	str_temp = ConvertString(sz_comPort);

	/* Spit COM Port */
	CStringArray stra_comPortList;
	if (SpiltString(str_temp, _T(","), stra_comPortList) != true)
	{
		return false;
	}
	if (stra_comPortList.GetSize() != MAX_SLOT)
	{
		return false;
	}

	int i_count = 0;
	for(int i = 0; i < MAX_SLOT; i++)
	{
		m_map_COMPort[i] = stra_comPortList.GetAt(i);

		/* COM Port default value is 0 */
		if (m_map_COMPort[i] != _T("0"))
		{
			i_count ++;
		}
	}

	/* COMPort all is 0 */
	if (i_count == 0)
	{
		return false;
	}

	return true;
}

bool CTestToolDlg::InitialUIControl()
{
	TCHAR sz_temp[2048] = {0};
	TCHAR sz_iniFolderName[512] = {0};
	CString strTemp;
	CFileFind file_findCtrl;

	/* Check ini file exist */	
	wsprintf(sz_iniFolderName, _T("%s\\%s\\Setting\\UIControl.ini"), CONFIG_PATH, m_st_uiParameter.str_modelName);

	if (!file_findCtrl.FindFile(sz_iniFolderName))
	{
		CString strMsg;
		strMsg.Format(_T("Can not find %s file!"), sz_iniFolderName);

		ShowResultDlg(CommErr_UI_Config_File_Not_Found, strMsg);
		return false;
	}

	if (m_st_uiParameter.str_station == _T(""))
	{
		ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("Station is null!"));
		return false;
	}

	/* FA */
	::GetPrivateProfileString(m_st_uiParameter.str_station, _T("ReadFA"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_ReadFA = (StrToInt(sz_temp)!=0);
	strTemp.Format(_T("ReadFA = %d"),StrToInt(sz_temp));
	LogMsg(strTemp);

	::GetPrivateProfileString(m_st_uiParameter.str_station, _T("WriteFA"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_WriteFA = (StrToInt(sz_temp)!=0);
	strTemp.Format(_T("WriteFA = %d"),StrToInt(sz_temp));
	LogMsg(strTemp);

	/* MI */
	::GetPrivateProfileString(m_st_uiParameter.str_station, _T("ScanPicasso"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_ScanPicasso = (StrToInt(sz_temp)!=0);
	strTemp.Format(_T("ScanPicasso = %d"),StrToInt(sz_temp));
	LogMsg(strTemp);
	
	return true;
}

void CTestToolDlg::InitUI()
{
	/* Device Group */
	m_label_device_title_COM.SetBkColor(RGB(224,224,224));
	m_label_device_title_Result.SetBkColor(RGB(224,224,224));
	m_label_device_title_Picasso.SetBkColor(RGB(224,224,224));

#ifdef MI_MODE
	m_label_device_title_Picasso.SetText(_T("PICASSO"));
	m_edit_deviceQCSSN[0].EnableWindow(FALSE);
	m_edit_deviceQCSSN[0].ShowWindow(FALSE);
#else
#ifdef FA_MODE
	m_label_device_title_Picasso.SetText(_T("QCS_SN"));
	//m_edit_deviceQCSSN[0].Set;
	m_edit_deviceQCSSN[0].EnableWindow(FALSE);
	m_edit_deviceQCSSN[0].ShowWindow(TRUE);
#else
#ifdef CSD_MODE
	m_label_device_title_Picasso.SetText(_T("QCS_SN"));
	m_edit_deviceQCSSN[0].EnableWindow(FALSE);
	m_edit_deviceQCSSN[0].ShowWindow(TRUE);
#endif
#endif
#endif

	for(int i = 0; i < MAX_SLOT; i++)
	{
		/* COM Port default value is 0 */
		if (m_map_COMPort[i] != _T("0"))
		{
			m_label_deviceCOM[i].SetBkColor(RGB(202,255,202));
			m_label_deviceCOM[i].SetFontBold(TRUE);
			m_label_deviceCOM[i].SetFontSize(30);
			m_label_deviceCOM[i].SetWindowText(m_map_COMPort[i]);

			m_label_deviceResult[i].SetBkColor(RGB(255,255,255));
			m_label_deviceResult[i].SetFontBold(TRUE);
			m_label_deviceResult[i].SetFontSize(14);
			m_label_deviceResult[i].SetWindowText(_T(""));

			if (g_pTestToolDlg->m_st_uiControl.b_ReadFA)
			{
				m_edit_devicePicasso[i].SetReadOnly(FALSE);
			}
			else
			{
				m_edit_devicePicasso[i].SetReadOnly(TRUE);
			}
		}
		else
		{
			m_label_deviceCOM[i].SetBkColor(RGB(221, 202, 162));
			m_label_deviceResult[i].SetBkColor(RGB(221, 202, 162));

			m_edit_devicePicasso[i].SetReadOnly(TRUE);
		}

		m_edit_devicePicasso[i].SetLimitText(13);
		m_edit_devicePicasso[i].SetWindowText(_T(""));
	}

	m_edit_Record_Total.SetReadOnly(TRUE);
	m_edit_Record_Pass.SetReadOnly(TRUE);
	m_edit_Record_Fail.SetReadOnly(TRUE);
}

void CTestToolDlg::IniRecord()
{
	m_obj_db.SetidParameter(m_st_uiParameter);
    
	if (! m_obj_db.ReadRecordFromDB(m_iRecordTotal, m_iRecordPass, m_iRecordFail))
	{
		m_iRecordTotal = 0;
		m_iRecordPass = 0;
		m_iRecordFail = 0;
	}

	//Set value
	UpdateRecord();
}

void CTestToolDlg::UpdateRecord()
{
	CString strTotal, strPass, strFail;
	strTotal.Format(_T("%d"), m_iRecordTotal);
	strPass.Format(_T("%d"), m_iRecordPass);
	strFail.Format(_T("%d"), m_iRecordFail);

	m_edit_Record_Total.SetWindowText(strTotal);
	m_edit_Record_Pass.SetWindowText(strPass);
	m_edit_Record_Fail.SetWindowText(strFail);
}

bool CTestToolDlg::InitTestItemList()
{
	char sz_testItem[100] = {0};
	CString str_testItem;
	int i_itemCount = m_p_FTTInterface->GetItemCount();

	for (int i=0; i<i_itemCount; i++)
	{
		ZeroMemory(sz_testItem,sizeof(sz_testItem));//SZ_testItem �Դ by yu

		if (m_p_FTTInterface->GetItemByIndex(i, sz_testItem, 100))
		{
			str_testItem = ConvertString(sz_testItem);
			m_listbox_testItem.InsertString(i, str_testItem);
			m_map_testItem[str_testItem] = i;
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CTestToolDlg::InitRecoverItemList()
{
	char sz_testItem[100] = {0};
	CString str_testItem;
	int i_itemCount = m_p_FTTInterface->GetRecoverItemCount();

	for (int i = 0; i < i_itemCount; i++)
	{
		ZeroMemory(sz_testItem,sizeof(sz_testItem));

		if (m_p_FTTInterface->GetRecoverItemByIndex(i, sz_testItem, 100))
		{
			str_testItem = ConvertString(sz_testItem);
			m_listbox_testItem.InsertString( m_listbox_testItem.GetCount(), str_testItem);
			m_map_testItem[str_testItem] = i;
		}
		else
		{
			return false;
		}
	}

	return true;
}

void CTestToolDlg::ResetResult()
{
	for(int i = 0; i < MAX_SLOT; i++)
	{
		/* COM Port default value is 0 */
		if (m_map_COMPort[i] != _T("0"))
		{
			m_label_deviceResult[i].SetBkColor(RGB(255,255,255));
			m_label_deviceResult[i].SetWindowText(_T(""));
		}
		else
		{
			m_label_deviceResult[i].SetBkColor(RGB(221, 202, 162));
		}
	}
}

bool CTestToolDlg::Run()
{
	for(int i = 0; i < MAX_SLOT; i++)
	{
		if (m_map_COMPort[i] != _T("0"))
		{	
#ifdef MI_MODE
			if (m_st_uiControl.b_ScanPicasso)
			{	CString str;
				m_edit_devicePicasso[i].GetWindowTextW(str);
				if(str.IsEmpty())
				{
					st_UIMessage st_message;
					st_message.i_slot = i;
					sprintf_s(st_message.sz_message,4096,"PICASSO is null");
					Event(UI_MESSAGE,(long)&st_message);

					st_UIResult st_result;
					st_result.i_slot = i;
					//CStringToChar(CommErr_UI_Picasso_Not_Input, st_result.sz_errCode); 
					strcpy_s(st_result.sz_errCode, "FAIL");
					Event(UI_RESULT, (long)&st_result);
					return false;
				}
				int i = str.GetLength();
				int n = WideCharToMultiByte(CP_ACP,0,str,i,NULL,0,NULL,NULL);
				char* szBuf = new char[n + 1];
				memset(szBuf,'\0',n + 1);
				WideCharToMultiByte(CP_OEMCP,0,str,i,szBuf,n,NULL,NULL);
				szBuf[n] = '\0';
				//strncpy_s(szBuf,100,(LPSTR)(LPCTSTR)&m_str_picasso,i);
				m_p_FTTInterface->SetParameterValue("PICASSO",szBuf);
				delete []szBuf;
			}
#else
#ifdef FA_MODE
			//if (m_st_uiControl.b_ReadFA)
			//{  //ReadId
			//	HINSTANCE hDll;
			//	hDll = LoadLibrary(_T("I2C.dll"));
			//	if (hDll != NULL )
			//	{
			//		typedef void(_stdcall *lpReadId)(char*,int);
			//		char szId[15] = "";
			//		lpReadId ReadId;
			//		ReadId =(lpReadId)::GetProcAddress(hDll,"ReadId");
			//		ReadId(szId,15);
			//		FreeLibrary(hDll);
			//		if(strlen(szId) == 11)
			//		{	st_UIMessage st_message;
			//			st_message.i_slot = i;
			//			sprintf_s(st_message.sz_message,4096,"Get ID:%s",szId);
			//			Event(UI_MESSAGE,(long)&st_message);

			//			CString strId = _T("");
			//			int wcsLen = ::MultiByteToWideChar(CP_ACP,NULL,szId,strlen(szId),NULL,0);
			//			wchar_t* wszString = new wchar_t[wcsLen + 1];
			//			::MultiByteToWideChar(CP_ACP,NULL,szId,strlen(szId),wszString,wcsLen);
			//			wszString[wcsLen] = '\0';
			//			//strId.Append(szId);
			//			//strId.Append(wszString);
			//			strId.Format(wszString);
			//			m_edit_devicePicasso[i].SetWindowTextW(strId);
			//			m_p_FTTInterface->SetParameterValue("PICASSO",szId);
			//		}
			//		else
			//		{	st_UIMessage st_message;
			//			st_message.i_slot = i;
			//			sprintf_s(st_message.sz_message,4096,"ID:%s is error",szId);
			//			Event(UI_MESSAGE,(long)&st_message);

			//			st_UIResult st_result;
			//			st_result.i_slot = i;
			//			//CStringToChar(CommErr_UI_Picasso_Not_Input, st_result.sz_errCode); 
			//			strcpy_s(st_result.sz_errCode, "FAIL");
			//			Event(UI_RESULT, (long)&st_result);
			//			return false;
			//		}
			//	}
			//	else
			//	{	st_UIMessage st_message;
			//		st_message.i_slot = i;
			//		sprintf_s(st_message.sz_message,4096,"Load I2C.dll error");
			//		Event(UI_MESSAGE,(long)&st_message);

			//		st_UIResult st_result;
			//		st_result.i_slot = i;
			//		//CStringToChar(CommErr_UI_Picasso_Not_Input, st_result.sz_errCode); 
			//		strcpy_s(st_result.sz_errCode, "FAIL");
			//		Event(UI_RESULT, (long)&st_result);
			//		return false;
			//	}
			//	//Load monitor info
			//}
#endif
#endif
			/* Run */
			m_map_running[i] = new CMultiSlot(&m_st_uiParameter, (i), m_p_FTTInterface);
			if (m_map_running[i] == NULL)
			{
				/* Show to UI */
				st_UIResult st_result;
				st_result.i_slot = i;
				CStringToChar(CommErr_UI_DL_Run_Thread_Fail, st_result.sz_errCode); 
				Event(UI_RESULT, (long)&st_result);

				st_result.i_slot = i;
				strcpy_s(st_result.sz_errCode, "FAIL");
				Event(UI_RESULT, (long)&st_result);
				return false;
			}

			if (!m_map_running[i]->DeviceRun())
			{
				/* Show to UI */
				st_UIResult st_result;
				st_result.i_slot = i;
				CStringToChar(CommErr_UI_DL_Run_Thread_Fail, st_result.sz_errCode); 
				Event(UI_RESULT, (long)&st_result);

				st_result.i_slot = i;
				strcpy_s(st_result.sz_errCode, "FAIL");
				Event(UI_RESULT, (long)&st_result);
				return false;
			}
			Sleep(500);
		}
	}

	return true;
}

// Reg callback function for SendEventToProcessor() ,  set to DLL
bool CTestToolDlg::reg_FacTestToolInterface_callback()
{	
	m_p_FTTInterface->RegCallBack(NotifyToTestToolDlg);

	return true;
}

void CTestToolDlg::Event(const char* str_event, long nParam)
{
	/* UI_MESSAGE */
	if (strcmp(str_event, UI_MESSAGE) == 0)
	{
		CSingleLock singleLock(&m_obj_UIMessage, TRUE);
		st_UIMessage* p_st_message = (st_UIMessage*)nParam;
		CString str_message;
		CString str_temp;
		str_temp = ConvertString(p_st_message->sz_message);
		str_message.Format(_T("[%d] %s"), p_st_message->i_slot, str_temp);
		//m_Label_Title.SetWindowText(str_message);
		//str_message = IDC_STATIC_TITLE.caption;
		int i;
		i = m_listbox_messagepanel.GetCount();
		m_listbox_messagepanel.InsertString(i,str_message);
		m_listbox_messagepanel.SetCurSel(i);
		LogMsg(str_message);
	}
	/* PICS Data */
	else if (strcmp(str_event, UI_PICSDATA) == 0)
	{
		CSingleLock singleLock(&m_obj_UIPicsdata, TRUE);
		st_PICSData* p_st_picsData = (st_PICSData*)nParam;
		SetPICSData(p_st_picsData->i_slot, p_st_picsData->sz_key, p_st_picsData->sz_value);
	}
	/* UI_RESULT */
	else if (strcmp(str_event, UI_RESULT) == 0)
	{		
		CSingleLock singleLock(&m_obj_UIResult, TRUE);

		st_UIResult* p_st_message = (st_UIResult*)nParam;
		CString str_result;
		str_result = ConvertString(p_st_message->sz_errCode);

		while (m_b_sync != false)
		{
			Sleep(20);
			continue;
		}
		m_b_sync = true;

		CString str_message;
		str_message.Format(_T("[%d] Received ErrorCode:%s"), p_st_message->i_slot, str_result);
		LogMsg(str_message);
		SetResult(p_st_message->i_slot,str_result);
	}
	else if (strcmp(str_event, UI_NEXTITEM) == 0)
	{
		m_listbox_testItem.SetCurSel(int(nParam));
	}

	return;
}

/* set CIM PICS data */
bool CTestToolDlg::SetPICSData(int i_slot, char* sz_key, char* sz_value)
{
	CString str_key;
	CString str_value;

	str_key = ConvertString(sz_key);
	str_value = ConvertString(sz_value);

	m_map_slotPICSData[i_slot][str_key] = str_value;

	return true;
}

bool CTestToolDlg::SetResult(int i_slot, CString str_result)
{
	CSingleLock singleLock(&m_obj_resultCritSection, TRUE);

	m_b_sync = false;

	CString strTemp;

	if ((i_slot < 0) || (i_slot >= MAX_SLOT) || (str_result == _T("")))
	{
		strTemp.Format(_T("[%d] got a error code is null"),i_slot);
		LogMsg(strTemp);
	}

	/* Check Result */
	m_map_runResult[i_slot] = str_result;
	if (str_result.CompareNoCase(_T("PASS")) == 0)
	{
		/* PASS --- End */
		m_label_deviceResult[i_slot].SetWindowText(str_result);
		m_label_deviceResult[i_slot].SetBkColor(RGB(0,255,0));
		m_map_slotPICSData[i_slot][_T("Result")] = _T("PASS");
		m_iRecordPass++;
	}
	else
	{	
		if (str_result.CompareNoCase(_T("FAIL")) != 0)
		{
			m_label_deviceResult[i_slot].SetWindowText(str_result);
			m_label_deviceResult[i_slot].SetBkColor(RGB(255,0,0));
			m_map_slotPICSData[i_slot][_T("ErrorCode")] = str_result;
			
			m_iRecordFail++;
			//return true;
		}
		else
		{
			/* FAIL --- End */
			m_label_deviceResult[i_slot].SetWindowText(str_result);
			m_label_deviceResult[i_slot].SetBkColor(RGB(255,0,0));
			m_map_slotPICSData[i_slot][_T("Result")] = _T("FAIL");
			m_iRecordFail++;
		}
	}
	m_iRecordTotal++;
	UpdateRecord();

	/* Check handset if is running */
	if (m_map_running[i_slot])
	{
		::WaitForSingleObject(m_map_running[i_slot]->GetThreadHandle(), INFINITE);
		delete m_map_running[i_slot];
		m_map_running[i_slot] = NULL;
	}
	m_map_running.erase(i_slot);

	if (m_map_running.empty())
	{
		/* Notify MainUI */
		for(int i = 0; i < MAX_SLOT; i++)
		{
			if (m_map_COMPort[i] != _T("0"))
			{
				if (!m_map_Picasso[i].IsEmpty())
				{
					// Insert to CIM
					CString strMsg;
					strMsg.Format(_T("[%d] InsertPICSDataToDB"), i);
					LogMsg(strMsg);

					InsertPICSDataToDB(i);
				}
			}
		}

		m_obj_db.WriteRecordToDB(m_iRecordTotal, m_iRecordPass, m_iRecordFail);

		/* Clear map */
		m_map_Picasso.clear();
		m_map_runResult.clear();

		/* Enable button */
		m_bBtnGo.EnableWindow(TRUE);
		m_bBtnExit.EnableWindow(TRUE);
		m_edit_devicePicasso[0].EnableWindow(TRUE);
		m_edit_devicePicasso[0].SetFocus();
	}

	return true;
}

bool CTestToolDlg::InsertPICSDataToDB(int iSlot)
{
	m_obj_db.SetslotPICSData(m_map_slotPICSData);

	if (!m_obj_db.InsertPICSDataToDB(iSlot))
	{
		CString strMsg;
		strMsg.Format(_T("[%d] Insert data fail:%s == %s"), iSlot, m_obj_db.m_str_errorCode, m_obj_db.m_str_errorMsg);
		LogMsg(strMsg);
		return false;
	}

	return true;
}

bool CTestToolDlg::ShowResultDlg(CString str_errorCode, CString str_message)
{
	m_dlg_result.SetText(str_errorCode, str_message);
	m_dlg_result.SetTextSize(30);
	m_dlg_result.DoModal();

	LogMsg(str_errorCode + str_message);

	return true;
}

void CTestToolDlg::LogMsg( CString logContent )
{
	if (logContent.Find(_T("%")) == -1)
	{
		m_obj_log.SaveLog(logContent);
	}
}


//=================================================================================================//
//======================================= CMultiSlot ================================================//
//=================================================================================================//
CMultiSlot::CMultiSlot (st_UIIDparameter* p_st_uiParameter, 
						int i_slot, 
						IFacTestToolInterface* p_FTTInterface
						)
						: m_p_st_uiParameter(p_st_uiParameter)
						, m_i_slot(i_slot)
						, m_p_DeviceFTTInterface(p_FTTInterface)
{	
	m_p_h_thread = NULL;
	m_b_runRes = false;
	sprintf_s(m_szFailItem,sizeof(m_szFailItem)/sizeof(char),"");
}

CMultiSlot::~CMultiSlot()
{
	m_p_DeviceFTTInterface = NULL;
}

bool CMultiSlot::DeviceRun(void)
{
	m_p_h_thread = ::AfxBeginThread(MainFunction, (void*)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	if (m_p_h_thread == NULL)
	{
		return false;
	}

	return true;
}

UINT CMultiSlot::MainFunction(LPVOID pParam)
{	
 	CMultiSlot* p_MultiSlot = (CMultiSlot*)pParam;
	char sz_picasso[20] = {0};


#ifdef MI_MODE
	if (!p_MultiSlot->CreateAndInitPhone())
	{
		//char szErrCode[100] = {0};

		//CStringToChar(CommErr_UI_Interface_GetFA_Fail, szErrCode);
		//p_MultiSlot->FireUIResult(szErrCode);
		g_pTestToolDlg->m_edit_devicePicasso[0].SetWindowTextW(_T(""));
		g_pTestToolDlg->m_edit_deviceQCSSN[0].SetWindowTextW(_T(""));
		p_MultiSlot->FireUIResult("Init phone fail");
		//p_MultiSlot->FireUIResult("FAIL");

		return 0;
	}

	if (!p_MultiSlot->GetPicassoFromDlg())
	{
		//char szErrCode[100] = {0};

		//CStringToChar(CommErr_UI_Picasso_Invalide, szErrCode);
		//p_MultiSlot->FireUIResult(szErrCode);
		//p_MultiSlot->FireUIResult("FAIL");
		g_pTestToolDlg->m_edit_devicePicasso[0].SetWindowTextW(_T(""));
		g_pTestToolDlg->m_edit_deviceQCSSN[0].SetWindowTextW(_T(""));
		p_MultiSlot->FireUIResult("Get picasso fail");

		return 0;
	}
#else
#ifdef FA_MODE
	if (!p_MultiSlot->CreateAndInitPhone())
	{
		g_pTestToolDlg->m_edit_devicePicasso[0].SetWindowTextW(_T(""));
		g_pTestToolDlg->m_edit_deviceQCSSN[0].SetWindowTextW(_T(""));

		char szErrCode[100] = {0};

		CStringToChar(CommErr_UI_Picasso_Invalide, szErrCode);
		p_MultiSlot->FireUIResult(szErrCode);
		p_MultiSlot->FireUIResult("FAIL");

		return 0;
	}

	if(!p_MultiSlot->ReadId())
	{
		g_pTestToolDlg->m_edit_devicePicasso[0].SetWindowTextW(_T("")) ;
		g_pTestToolDlg->m_edit_deviceQCSSN[0].SetWindowTextW(_T(""));

		//p_MultiSlot->m_p_DeviceFTTInterface->End(p_MultiSlot->m_i_slot);

		char szErrCode[100] = {0};

		CStringToChar(CommErr_UI_Picasso_Invalide, szErrCode);
		p_MultiSlot->FireUIResult(szErrCode);
		p_MultiSlot->FireUIResult("FAIL");

		return 0;
	}
	else
	{
	}

	if (!p_MultiSlot->GetPicassoFromDlg())
	{	
		g_pTestToolDlg->m_edit_devicePicasso[0].SetWindowTextW(_T("")) ;
		g_pTestToolDlg->m_edit_deviceQCSSN[0].SetWindowTextW(_T(""));

		//p_MultiSlot->m_p_DeviceFTTInterface->End(p_MultiSlot->m_i_slot);
		//没有IMEI号
		char szErrCode[100] = {0};

		CStringToChar(CommErr_UI_Picasso_Invalide, szErrCode);
		p_MultiSlot->FireUIResult(szErrCode);
		p_MultiSlot->FireUIResult("FAIL");

		return 0;
	}
	else
	{
		int i = p_MultiSlot->m_str_picasso.GetLength();
		int n = WideCharToMultiByte(CP_ACP,0,p_MultiSlot->m_str_picasso,i,NULL,0,NULL,NULL);
		char* szBuf = new char[n + 1];
		memset(szBuf,'\0',n + 1);
		WideCharToMultiByte(CP_OEMCP,0,p_MultiSlot->m_str_picasso,i,szBuf,n,NULL,NULL);
		szBuf[n] = '\0';
		p_MultiSlot->m_p_DeviceFTTInterface->SetIMEI(p_MultiSlot->m_i_slot, szBuf, n+1);
		delete []szBuf;
	}

	if(!p_MultiSlot->GetQCSSNFromdlg())
	{
		g_pTestToolDlg->m_edit_devicePicasso[0].SetWindowTextW(_T("")) ;
		g_pTestToolDlg->m_edit_deviceQCSSN[0].SetWindowTextW(_T(""));

		//p_MultiSlot->m_p_DeviceFTTInterface->End(p_MultiSlot->m_i_slot);
		
		char szErrCode[100] = {0};

		CStringToChar(CommErr_UI_Picasso_Invalide, szErrCode);
		p_MultiSlot->FireUIResult(szErrCode);
		p_MultiSlot->FireUIResult("FAIL");

		return 0;
	}
	else
	{
		int i = p_MultiSlot->m_str_QCSSN.GetLength();
		int n = WideCharToMultiByte(CP_ACP,0,p_MultiSlot->m_str_QCSSN,i,NULL,0,NULL,NULL);
		char* szBuf = new char[n + 1];
		memset(szBuf,'\0',n + 1);
		WideCharToMultiByte(CP_OEMCP,0,p_MultiSlot->m_str_QCSSN,i,szBuf,n,NULL,NULL);
		szBuf[n] = '\0';
		p_MultiSlot->m_p_DeviceFTTInterface->SetSN(p_MultiSlot->m_i_slot, szBuf, n+1);
		delete []szBuf;
	}
#else
#ifdef CSD_MODE
	if (!p_MultiSlot->CreateAndInitPhone())
	{
		//char szErrCode[100] = {0};

		//CStringToChar(CommErr_UI_Interface_GetFA_Fail, szErrCode);
		//p_MultiSlot->FireUIResult(szErrCode);
		//p_MultiSlot->FireUIResult("FAIL");

		g_pTestToolDlg->m_edit_devicePicasso[0].SetWindowTextW(_T(""));
		g_pTestToolDlg->m_edit_deviceQCSSN[0].SetWindowTextW(_T(""));
		p_MultiSlot->FireUIResult("Init phone fail");

		return 0;
	}

	if (!p_MultiSlot->GetPicassoFromDlg())
	{	
		g_pTestToolDlg->m_edit_devicePicasso[0].SetWindowTextW(_T("")) ;
		g_pTestToolDlg->m_edit_deviceQCSSN[0].SetWindowTextW(_T(""));

		//p_MultiSlot->m_p_DeviceFTTInterface->End(p_MultiSlot->m_i_slot);
		//没有IMEI号
		//char szErrCode[100] = {0};

		//CStringToChar(CommErr_UI_Picasso_Invalide, szErrCode);
		//p_MultiSlot->FireUIResult(szErrCode);
		//p_MultiSlot->FireUIResult("FAIL");
		p_MultiSlot->FireUIResult("Get picasso fail");
		return 0;
	}
	else
	{
		int i = p_MultiSlot->m_str_picasso.GetLength();
		int n = WideCharToMultiByte(CP_ACP,0,p_MultiSlot->m_str_picasso,i,NULL,0,NULL,NULL);
		char* szBuf = new char[n + 1];
		memset(szBuf,'\0',n + 1);
		WideCharToMultiByte(CP_OEMCP,0,p_MultiSlot->m_str_picasso,i,szBuf,n,NULL,NULL);
		szBuf[n] = '\0';
		p_MultiSlot->m_p_DeviceFTTInterface->SetIMEI(p_MultiSlot->m_i_slot, szBuf, n+1);
		delete []szBuf;
	}

	if(!p_MultiSlot->GetQCSSNFromdlg())
	{
		g_pTestToolDlg->m_edit_devicePicasso[0].SetWindowTextW(_T("")) ;
		g_pTestToolDlg->m_edit_deviceQCSSN[0].SetWindowTextW(_T(""));

		//p_MultiSlot->m_p_DeviceFTTInterface->End(p_MultiSlot->m_i_slot);
		
		//char szErrCode[100] = {0};

		//CStringToChar(CommErr_UI_Picasso_Invalide, szErrCode);
		//p_MultiSlot->FireUIResult(szErrCode);
		//p_MultiSlot->FireUIResult("FAIL");
		p_MultiSlot->FireUIResult("Get QCS SN fail");

		return 0;
	}
	else
	{
		int i = p_MultiSlot->m_str_QCSSN.GetLength();
		int n = WideCharToMultiByte(CP_ACP,0,p_MultiSlot->m_str_QCSSN,i,NULL,0,NULL,NULL);
		char* szBuf = new char[n + 1];
		memset(szBuf,'\0',n + 1);
		WideCharToMultiByte(CP_OEMCP,0,p_MultiSlot->m_str_QCSSN,i,szBuf,n,NULL,NULL);
		szBuf[n] = '\0';
		p_MultiSlot->m_p_DeviceFTTInterface->SetSN(p_MultiSlot->m_i_slot, szBuf, n+1);
		delete []szBuf;
	}
#endif
#endif
#endif
	///* Set Picasso */
	//CStringToChar(p_MultiSlot->m_str_picasso, sz_picasso);

	//if (!p_MultiSlot->m_p_DeviceFTTInterface->SetParameterValue(PICASSO, sz_picasso, p_MultiSlot->m_i_slot))
	//{
	//	char sz_errCode[100] = {0};

	//	p_MultiSlot->FireUIMessage("Set Picasso Fail.");
	//	CStringToChar(CommErr_UI_Interface_Setparameter_Fail,sz_errCode);
	//	p_MultiSlot->FireUIResult(sz_errCode);
	//	p_MultiSlot->FireUIResult("FAIL");
	//	return 0;
	//}
	//p_MultiSlot->FireUIMessage("Set Picasso Success.");
	//g_pTestToolDlg->m_map_slotPICSData[p_MultiSlot->m_i_slot][_T("PICASSO")] = p_MultiSlot->m_str_picasso;

	/* PreRun */
	bool b_res = false;
	p_MultiSlot->FireUIMessage("PreRun start.");
	if (!(b_res = p_MultiSlot->m_p_DeviceFTTInterface->PreRun(p_MultiSlot->m_i_slot)))
	{
		p_MultiSlot->FireUIMessage("PreRun Fail.");
	}
	else
	{
		p_MultiSlot->FireUIMessage("PreRun Success.");
	}

	/* Run */
	if (b_res)
	{
		p_MultiSlot->FireUIMessage("Run start.");
		b_res = p_MultiSlot->m_p_DeviceFTTInterface->Run(p_MultiSlot->m_i_slot);
		if (!b_res)
		{
			p_MultiSlot->FireUIMessage("Run Fail.");
			p_MultiSlot->m_b_runRes = false;
		}
		else
		{
			p_MultiSlot->FireUIMessage("Run Success.");
			p_MultiSlot->m_b_runRes = true;
		}
	}

	p_MultiSlot->LogMsg(p_MultiSlot->m_p_st_uiParameter->str_station);

	/* Post Run */
	p_MultiSlot->m_p_DeviceFTTInterface->PostRun(p_MultiSlot->m_i_slot);

	p_MultiSlot->FireUIMessage("PostRun Success.");

	g_pTestToolDlg->m_edit_devicePicasso[0].SetWindowTextW(_T(""));
	g_pTestToolDlg->m_edit_deviceQCSSN[0].SetWindowTextW(_T(""));
	//g_pTestToolDlg->m_edit_devicePicasso[0].Clear();
	if (!b_res)
	{
		p_MultiSlot->m_p_DeviceFTTInterface->GetFailItem(p_MultiSlot->m_i_slot,p_MultiSlot->m_szFailItem, 256);
		//p_MultiSlot->FireUIResult("FAIL");
		p_MultiSlot->FireUIResult(p_MultiSlot->m_szFailItem);
	}
	else
	{
		p_MultiSlot->FireUIResult("PASS");
	}

	return 1;
 }

bool CMultiSlot::GetPicasso()
{	
	if (!g_pTestToolDlg->m_st_uiControl.b_ReadFA)
	{
		g_pTestToolDlg->m_edit_devicePicasso[m_i_slot].GetWindowText(m_str_picasso);

		if (m_str_picasso.IsEmpty())
		{
			FireUIMessage("Get Picasso from UI Fail");

			return false;
		}
	}
	else
	{
		char sz_picasso[11] = {0};
		CString str_tmp = _T("");

		if (!m_p_DeviceFTTInterface->GetSerialNumber(m_i_slot, sz_picasso, sizeof(sz_picasso)))
		{
			FireUIMessage("GetSerialNumber Fail");

			return false;
		}
		else
		{
			m_str_picasso = ConvertString(sz_picasso);
			g_pTestToolDlg->m_edit_devicePicasso[m_i_slot].SetWindowText(m_str_picasso);

			char szMsg[128] = {0};
			sprintf_s(szMsg, 128, "GetSerialNumber Success, Picasso = %s", sz_picasso);
			FireUIMessage(szMsg);
		}
	}

	g_pTestToolDlg->m_map_Picasso[m_i_slot] = m_str_picasso;

	return true;
}

bool CMultiSlot::GetPicassoFromDlg()
{		
	g_pTestToolDlg->m_edit_devicePicasso[m_i_slot].GetWindowText(m_str_picasso);
	//if (m_str_picasso.IsEmpty())
	//{
	//	FireUIMessage("Get Picasso from UI Fail");

	//	return false;
	//}
	return true;
}

bool CMultiSlot::GetQCSSNFromdlg()
{
	g_pTestToolDlg->m_edit_deviceQCSSN[m_i_slot].GetWindowTextW(m_str_QCSSN);
	return true;
}

bool CMultiSlot::VerifyPicassoAndTimestamp()
{
	char szPicasso[11] = {0};
	char szTimestamp[12]={0};
	CString strMsg = _T("");

	if (!m_p_DeviceFTTInterface->GetSerialNumber(m_i_slot, szPicasso, sizeof(szPicasso)))
	{
		FireUIMessage("GetSerialNumber Fail");
		return false;
	}

	if (!m_p_DeviceFTTInterface->GetTimeStamp(m_i_slot, szTimestamp, sizeof(szTimestamp)))
	{
		FireUIMessage("GetTimeStamp Fail");
		return false;
	}


	if (IsValidePicasso(szPicasso) && IsValideTimestamp(szTimestamp))
	{		
		char szTimestampNew[30] = {0};
		GetTimestampHexStr(szTimestamp, szTimestampNew);
		strMsg.Format(_T("[%d] Mobile Picasso:%s, MobileID:%s"), m_i_slot, ConvertString(szPicasso), ConvertString(szTimestampNew));
		LogMsg(strMsg);		
	}
	else
	{
		FireUIMessage("Picasso or Timestamp is invalid.");
		return false;
	}

	return true;
}

bool CMultiSlot::IsValidePicasso(char *szPicasso)
{
	for (int i=0; i<10; i++)
	{
		if ((szPicasso[i] == 0)||!isalnum((int)szPicasso[i]))
		{
			return false;
		}
	}

	return true;
}

bool CMultiSlot::IsValideTimestamp(char *szTimestamp)
{
	char c = szTimestamp[0];

	for (int i = 1; i< 11; i++)
	{
		if (szTimestamp[i] != c)
		{
			return true;
		}
	}

	return false;
}

void CMultiSlot::GetTimestampHexStr(char *szTimestampOrig, char *szTimestampNew)
{
	char szTemp[5] = {0};
	int i;
	for(i=0; i < 11; i++)
	{
		TransferHexToInt_BCD((int)szTimestampOrig[i], szTemp);
		szTimestampNew[2*i] = szTemp[0];
		szTimestampNew[2*i+1] = szTemp[1];
	}
	szTimestampNew[2*i] = '\0';
}

bool CMultiSlot::TransferHexToInt_BCD(const int nHex, char* sz)
{
	// Warning: This function changes 0x12 to "12", not "18"  !!
	bool bRes = ( (nHex >> 4) <= 9)	&& ((nHex & 0x0F) <= 9);

	if (bRes)
	{
		*sz = (nHex >> 4) + '0'; 
		*(sz+1) = (nHex & 0x0F) + '0';  
		*(sz+2) = 0;
	}

	return bRes;
}

void CMultiSlot::FireUIMessage(const char* sz_message)
{	
	/* Check input */
	if (sz_message == NULL)
	{
		return;
	}

	/* Send message */
	st_UIMessage st_message;
	st_message.i_slot = m_i_slot;
	strcpy_s(st_message.sz_message, sz_message);

	if (this->m_p_DeviceFTTInterface != NULL)
	{
		g_pTestToolDlg->Event(UI_MESSAGE, (long)&st_message);
	}

	return;
}

void CMultiSlot::FireUIResult(const char* sz_result)
{
	/* Check input */
	if (sz_result == NULL)
	{
		return;
	}

	/* Send result */
	st_UIResult st_result;
	st_result.i_slot = m_i_slot;
	strcpy_s(st_result.sz_errCode, sz_result);

	if (this->m_p_DeviceFTTInterface != NULL)
	{
		g_pTestToolDlg->Event(UI_RESULT, (long)&st_result);
	}

	return;
}


void CMultiSlot::LogMsg( CString logContent )
{
	m_obj_log.SaveLog(logContent);
}

bool CMultiSlot::CreateAndInitPhone(void)
{

	if (!m_p_DeviceFTTInterface->CreateAndInitPhone(m_i_slot))
	{
		FireUIMessage("CreateAndInitPhone Fail");
		return false;
	}

	return true;
}

bool CMultiSlot::ReadId(void)
{
	char szId[15] = "";
	st_UIMessage st_message;
	st_message.i_slot = m_i_slot;
	if(!m_p_DeviceFTTInterface->ReadId(m_i_slot,szId,15))
	{
		FireUIMessage("Read id fail.");
		sprintf_s(st_message.sz_message,4096,"ReadId fail ,ID= %s",szId);
		g_pTestToolDlg->Event(UI_MESSAGE,(long)&st_message);
		return false;
	}
	else
	{	
		FireUIMessage("Read id pass.");
		sprintf_s(st_message.sz_message,4096,"ReadId pass ,ID= %s",szId);
		g_pTestToolDlg->Event(UI_MESSAGE,(long)&st_message);
		m_p_DeviceFTTInterface->SetParameterValue("PICASSO",szId);
		return true;
	}
}

void CTestToolDlg::OnEnChangeEditSlot0Picasso()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CTestToolDlg::OnEnChangeEditSlot0Qcssn()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}