
#include "stdafx.h"
#include "UnifyUI_FacTestTool.h"
#include "UnifyUI_FacTestToolDlg.h"
#include "DownLoadDlg.h"
#include "IMEIFCDlg.h"
#include "OtherStationDlg.h"
#include "OthersMultiDlg.h"
#include "../../Modules/Common/CommonUtil/Win32SHELLUtility.h"


#pragma comment (lib, "Version.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void SetAppVersion(CString str_version);
private:
	CString m_str_vesion;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::SetAppVersion(CString str_version)
{
	m_str_vesion = _T("Version ") + str_version;
}

void CAboutDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	GetDlgItem(IDC_STATIC_VER)->SetWindowText(m_str_vesion);

	// TODO: Add your message handler code here
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

CUnifyUI_FacTestToolDlg::CUnifyUI_FacTestToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUnifyUI_FacTestToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	/* Initial UI parameter */
	m_st_idParameter.i_parameterNum   = 0;
	m_st_idParameter.b_tsFlag         = false;
	m_st_idParameter.str_firstProcess = _T("");
	m_st_idParameter.str_station      = _T("");
	m_st_idParameter.str_modelName    = _T("");
	m_st_idParameter.str_line         = _T("");
	m_st_idParameter.str_daynight         = _T("");
	m_st_idParameter.str_pcba         = _T("");
	m_st_idParameter.str_so           = _T("");
	m_st_idParameter.str_partNumber   = _T("");
	m_st_idParameter.str_toolVer      = _T("");
	m_st_idParameter.str_employee     = _T("");
	m_st_idParameter.str_factoryVer   = _T("");
	m_st_idParameter.str_userLoadVer  = _T("");
	m_st_idParameter.str_imagePath    = _T("");
	m_st_idParameter.str_accessoryList= _T("");
	m_st_idParameter.str_soType       = _T("");
	m_st_idParameter.str_segment      = _T("");
	m_st_idParameter.str_Reserve1     = _T("");
	m_st_idParameter.str_Reserve2     = _T("");
	m_st_idParameter.i_idLength       = 0;
	m_st_idParameter.b_OpenbyEntranceTool = false;

	/* Initial INI parameter */
	for(int i=0;i<PS_NUMBER; i++)
	{
		m_st_uiControl.i_PowerSupply[i]      = 0;
		m_st_uiControl.i_PowerOnDelay[i]     = 0;
		m_st_uiControl.i_PSAddr[i]           = 0;
		m_st_uiControl.i_PSInterface[i]      = 0;
		m_st_uiControl.lf_Current[i]         = 0;
		m_st_uiControl.lf_Voltage[i]         = 0;
	}
	
	m_st_uiControl.i_DevControl       = 0;
	m_st_uiControl.i_DeviceInDelay    = 0;
	m_st_uiControl.str_DevInCommand   = _T("");
	m_st_uiControl.str_DevOutCommand  = _T("");
	m_st_uiControl.str_DevPort        = _T("COM1");

	m_st_uiControl.b_ScanPicasso      = false;
	m_st_uiControl.b_PreScan          = false;
	m_st_uiControl.b_AutoRun          = false;
	m_st_uiControl.b_AutoRunPreScan   = false;
	//m_st_uiControl.b_WriteTag			  = false;
	//m_st_uiControl.b_ReadTag			  = false;
	m_st_uiControl.b_WriteTagFrame		= false;
	//m_st_uiControl.b_ScanTag				= false;

	m_st_uiControl.b_WriteSn			  = false;
	m_st_uiControl.b_ReadSn			  = false;
	m_st_uiControl.b_WriteSnFrame			  = false;
	m_st_uiControl.b_ScanSn				= false;

	m_st_uiControl.b_ReadFA           = true;//default read fa, lion
	m_st_uiControl.b_WriteFA          = false;
	m_st_uiControl.i_ToolTimeout            = 480; //default set 8 min, 8 x 60 = 480 s

	m_st_uiControl.b_S3DDC			= false;

	m_st_uiControl.b_Multi      = false;

	m_map_picasso.clear();

	m_map_slotPICSData.clear();
	m_str_titlePicturePath = _T("");
	m_map_StationIndexMap.clear();
	m_map_TestItem.clear();

	m_i_timeCount = 0;
	m_i_today = 0;

	m_str_computerName = _T("");

	m_i_totalNo = 0;
	m_i_passNo = 0;
	m_i_failNo = 0;

	m_b_isTesting = false;
	m_st_result.i_slot = 0;
	m_st_result.str_errorcode = _T("");
	m_str_errorCode = _T("");
	m_str_errorMsg = _T("");
	m_str_ResultCsvPath = _T("");
	m_b_NewErrCode = false;
	m_i_RBNumber = 0;
	m_b_RBMask = false;
	m_b_ChangeGB = false;

	m_i_cameraNo = 0;//for multi-camera

	//for cycle test
	m_i_testtime = 2000;

	m_str_InsertCIM = _T("-1");
}

CUnifyUI_FacTestToolDlg::~CUnifyUI_FacTestToolDlg()
{
	m_brushDiagBk.DeleteObject();
	m_font_button.DeleteObject();


#ifndef RD_MODE
	m_obj_QueryCIM.UpdateEmployeeID_StatusFromCIM();
	m_obj_QueryCIM.UpLoadDBAccessDLL();
	LogMsg(_T("Update employee ID"));
#endif
	CString str_command;
	::GetModuleFileName(NULL, str_command.GetBuffer(MAX_PATH), MAX_PATH);
	str_command.ReleaseBuffer();
	str_command = str_command.Left(str_command.ReverseFind('\\'));

	str_command = str_command + _T("\\adb.exe kill-server");

	ExeCmd(str_command,2000);
}

void CUnifyUI_FacTestToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_StationPage, m_tab_stationPageCtrl);
	DDX_Control(pDX, IDC_STATIC_ID, m_static_id);
	DDX_Control(pDX, IDC_STATIC_Model, m_static_model);
	DDX_Control(pDX, IDC_STATIC_Line, m_static_line);
	DDX_Control(pDX, IDC_STATIC_PCBA, m_static_pcba);
	DDX_Control(pDX, IDC_STATIC_SO, m_static_so);
	DDX_Control(pDX, IDC_STATIC_PN, m_static_partNumber);
	DDX_Control(pDX, IDC_STATIC_ToolVer, m_static_toolVer);
	DDX_Control(pDX, IDC_STATIC_Employee, m_static_employee);
	DDX_Control(pDX, IDC_STATIC_FactoryVer, m_static_factoryVer);
	DDX_Control(pDX, IDC_STATIC_UserLoadVer, m_static_userLoadVer);
	DDX_Control(pDX, IDC_STATIC_Record, m_static_record);
	DDX_Control(pDX, IDC_STATIC_Total, m_static_total);
	DDX_Control(pDX, IDC_STATIC_PASS, m_static_pass);
	DDX_Control(pDX, IDC_STATIC_FAIL, m_static_fail);
	DDX_Control(pDX, IDC_STATIC_Time, m_static_time);
	DDX_Control(pDX, IDC_STATIC_Execute, m_static_execute);
	DDX_Control(pDX, IDC_EDIT_Model, m_edit_model);
	DDX_Control(pDX, IDC_EDIT_Line, m_edit_line);
	DDX_Control(pDX, IDC_EDIT_PCBA, m_edit_pcba);
	DDX_Control(pDX, IDC_EDIT_SO, m_edit_so);
	DDX_Control(pDX, IDC_EDIT_PN, m_edit_partNumber);
	DDX_Control(pDX, IDC_EDIT_ToolVer, m_edit_toolVer);
	DDX_Control(pDX, IDC_EDIT_Employee, m_edit_employee);
	DDX_Control(pDX, IDC_EDIT_UserLoadVer, m_edit_userLoadVer);
	DDX_Control(pDX, IDC_EDIT_FactoryVer, m_edit_factoryVer);
	DDX_Control(pDX, IDC_EDIT_Total, m_edit_total);
	DDX_Control(pDX, IDC_EDIT_PASS, m_edit_pass);
	DDX_Control(pDX, IDC_EDIT_FAIL, m_edit_fail);
	DDX_Control(pDX, IDC_EDIT_Time, m_edit_time);
	DDX_Control(pDX, IDC_BUTTON_GO, m_button_start);
	DDX_Control(pDX, IDC_BUTTON_Stop, m_button_stop);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_button_exit);
}

BEGIN_MESSAGE_MAP(CUnifyUI_FacTestToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_StationPage, &CUnifyUI_FacTestToolDlg::OnTcnSelchangeTabStationpage)
	ON_BN_CLICKED(IDC_BUTTON_GO, &CUnifyUI_FacTestToolDlg::OnBnClickedButtonGo)
	ON_BN_CLICKED(IDC_BUTTON_Stop, &CUnifyUI_FacTestToolDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CUnifyUI_FacTestToolDlg::OnBnClickedButtonExit)
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER_SETRESULT, OnEnd)
	ON_MESSAGE(WM_USER_SETFOCUS, OnFocus)
	ON_BN_CLICKED(IDC_BUTTON_CYCLETEST, &CUnifyUI_FacTestToolDlg::OnBnClickedButtonCycletest)
END_MESSAGE_MAP()

void CUnifyUI_FacTestToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.SetAppVersion(m_str_appver);
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CUnifyUI_FacTestToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		InitialUITitle();
	}
}

HBRUSH CUnifyUI_FacTestToolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor ==CTLCOLOR_DLG)
		hbr = (HBRUSH)m_brushDiagBk.GetSafeHandle();  

	return hbr;
}

HCURSOR CUnifyUI_FacTestToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CUnifyUI_FacTestToolDlg::findXML()
{
	CFileFind finder;
	TCHAR szModulePath[MAX_PATH] = {0};
	TCHAR szQisdaPath[MAX_PATH] = {0};

	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);

	_stprintf(szQisdaPath, _T("%s\\Qisda\\*TestItem_RD.xml"), szModulePath ) ;
	//char Path_UpLog_Bat[MAX_PATH] ={0};
	//strcpy

    CString str_search_folder, str_item;
    // search all
    str_search_folder.Format(_T("%s"), szQisdaPath );
	BOOL bExist = finder.FindFile(str_search_folder);
	if ( bExist ) {
		while (bExist) {
			bExist = finder.FindNextFile(); 
			str_item = finder.GetFilePath();
			str_item = finder.GetFileName();

			int curPos = 0;
			CAtlString resToken;
			cs_Model_Name = str_item.Tokenize(_T("_"),curPos); 
			cs_Station_Name  = str_item.Tokenize(_T("_"),curPos);

			if ( cs_Station_Name.Compare (_T("OS")) == 0 ) {
				cs_Station_Name = _T("OS_DL");
				if (!GetDLLIniFile() ) return false;
				if ( !Get_SWVersion() ) return false;
			}
			else if ( cs_Station_Name.Find (_T("RUN")) != -1 ){
				cs_Station_Name = _T("RUN_IN");
			}
			else if ( cs_Station_Name.Find (_T("PREDL")) != -1 ){
				if (!GetDLLIniFile() ) return false;
				if ( !Get_SWVersion() ) return false;
			//	cs_Station_Name = _T("PRE_DL");
			}
			//while (resToken != _T(""))
			//{
			//   _tprintf_s(_T("Resulting token: %s\n"), resToken);
			//   resToken = str_item.Tokenize(_T("_"), curPos);
			//};   
		}
		finder.Close();
		return true;
	}
	return false;;
}

bool CUnifyUI_FacTestToolDlg::GetDLLIniFile(void)
{
	/* Get current DLL path */
	TCHAR sz_currentPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
	::PathRemoveFileSpec(sz_currentPath);

	/* Check data file exist */
	CString str_iniFile;

	str_iniFile.Format(_T("%s\\QISDA\\%s_%s_TestItem_RD.xml"), sz_currentPath, cs_Model_Name, cs_Station_Name );
	if (_taccess(str_iniFile, 0) != 0) 
	{
		LogMsg(_T("Can not find \\Qisda\\TestItem_RD.xml,"));
	}
	else
	{
		LogMsg(_T("Access \\Qisda\\TestItem_RD.xml ok"));
	}

	m_str_iniFileName = str_iniFile; 
	return true;
}


bool CUnifyUI_FacTestToolDlg::Get_SWVersion(void)
{
	TCHAR sz_temp[2048] = {0};
	TCHAR sz_iniFolderName[512] = {0};

 	/* Check ini file exist */
 	wsprintf(sz_iniFolderName, _T("%s"), m_str_iniFileName);

	::GetPrivateProfileString(_T("Setting"), _T("SWVersion"), _T("DefaultName"), sz_temp, sizeof(sz_temp), sz_iniFolderName);

	m_st_idParameter.str_userLoadVer.Format(_T("%s"), sz_temp);// = _T("20151008_un2416h_evt1_dels2317w_ship");
	m_st_idParameter.str_factoryVer.Format(_T("%s"), sz_temp);

	if ( m_st_idParameter.str_userLoadVer.Find(_T("DefaultName")) != -1 ) {
		LogMsg(_T("Access fail  \\Qisda\\TestItem_RD.xml SWVersion"));
		return false;
	}


	LogMsg(_T("Access OK  \\Qisda\\TestItem_RD.xml SWVersion"));
	return true;
}


BOOL CUnifyUI_FacTestToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	//get application version
	m_str_appver = getAppVersion();
	
	LogMsg(_T("***************************************************************************"));
	LogMsg(_T("*    *      *      *                                                       *"));
	LogMsg(_T("*       *          *                                                       *"));
	LogMsg(_T("***************************************************************************"));

#ifndef FACTORY_MODE
	//if not factory mode, initdlg need to show for user to input some parameters
	TCHAR sz_temp[2048] = {0};
	TCHAR sz_iniFolderName[512] = {0};
	CString strTemp;

	/* Check ini file exist */
	CFileFind file_findCtrl;

 	TCHAR sz_folder[MAX_PATH] = {0};
 	GetModuleFileName(NULL, sz_folder, MAX_PATH);
 	PathRemoveFileSpec(sz_folder);
 
 	/* Check ini file exist */
 	wsprintf(sz_iniFolderName, _T("%s\\Qisda\\config.ini"), sz_folder);

	if (!file_findCtrl.FindFile(sz_iniFolderName))
	{
		LogMsg(_T("Can not find \\Qisda\\config.ini, use default value!"));
	}
	else
	{
		//support s3 ddc link station
		::GetPrivateProfileString(_T("s3"), _T("s3DDCStation"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.b_S3DDC = (StrToInt(sz_temp)!=0);
		//support s3 csd_write, csd_os_dl  station
		::GetPrivateProfileString(_T("s3"), _T("s3SO"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.b_S3SO = (StrToInt(sz_temp)!=0);
	}

	if (m_st_uiControl.b_S3DDC) {
		if (m_dlg_initDlg_S3.DoModal() == IDCANCEL)
		{
			CDialog::OnCancel();
			return TRUE;
		}
		m_dlg_initDlg_S3.GetInitData(m_st_idParameter); 
	}

	if (m_st_uiControl.b_S3SO) {
		if (m_dlg_initDlg_SO.DoModal() == IDCANCEL)
		{
			CDialog::OnCancel();
			return TRUE;
		}
		m_dlg_initDlg_SO.GetInitData(m_st_idParameter); 
	}



	
	//m_st_idParameter.
	if ( findXML() ){
		m_st_idParameter.str_line = m_st_idParameter.str_line; 
		m_st_idParameter.str_daynight = m_st_idParameter.str_daynight; 
		m_st_idParameter.i_parameterNum = 12; 
		m_st_idParameter.b_tsFlag = false;
		m_st_idParameter.str_station  = cs_Station_Name;// _T("MMI");
		m_st_idParameter.str_modelName = cs_Model_Name;// _T("GBROB1A");
	//	m_st_idParameter.str_so = _T("1234567");
		m_st_idParameter.str_so = m_st_idParameter.str_so; 
		m_st_idParameter.str_partNumber = _T("0");
		m_st_idParameter.str_employee = _T("1234567");
	//	m_st_idParameter.str_userLoadVer = _T("20151008_un2416h_evt1_dels2317w_ship");
	//	m_st_idParameter.str_factoryVer = _T("20151008_un2416h_evt1_dels2317w_ship");
		

	}
	else
	{
		LogMsg(_T("Can not find \\Qisda\\TESTITEM_RD.XML"));
		return false;
	}

	m_map_StationIndexMap[STATION_PREDL] = _T("01");
	m_map_StationIndexMap[STATION_DL] = _T("01");
	m_st_idParameter.str_firstProcess = STATION_DL; 

	//this is used for the situation that need to write FA at first station
	if (m_st_idParameter.str_station.Find(_T("_01")) != -1)
	{
		m_st_idParameter.str_station = m_st_idParameter.str_station.Left(m_st_idParameter.str_station.Find(_T("_01")));
		m_st_idParameter.str_firstProcess = m_st_idParameter.str_station; 
		m_map_StationIndexMap[m_st_idParameter.str_station] = _T("01");
	}
	if ((m_st_idParameter.str_station == STATION_OS_DL) || 
		(m_st_idParameter.str_station == STATION_PREDL) )
	{
		m_st_idParameter.str_segment = _T("PACKAGE");
	}


#else

	/* Check transfer parameter from Entrance tool */
	if (m_st_idParameter.i_parameterNum == 0 || m_st_idParameter.str_so == _T(""))
	{
		ShowResultMsgBox(CommErr_UI_CIM_SO_NULL, _T("获取的工单信息为空!\n Get SO info is NULL!"));
		EndDialog(0);
		return FALSE;
	}
#endif

#ifndef RD_MODE
	/* Load DB Access Dll*/
	if (!m_obj_QueryCIM.LoadDBAccessDLL())
	{
		ShowResultMsgBox(CommErr_UI_DLL_Load_Fail, _T("Load DBAceess.dll Fail!"));
		EndDialog(0);
		return FALSE;
	}

	//query data from DB
	if (!QueryDataFromCIM())
	{
		ShowResultMsgBox(m_str_errorCode, m_str_errorMsg);
		EndDialog(0);
		return FALSE;
	}

	//2012.08.20 If Re-DL station, need to check so type and ImageVer
	if (m_st_idParameter.str_station == STATION_REDL)
	{
		if (m_st_idParameter.str_soType != _T("R"))
		{
			ShowResultMsgBox(CommErr_UI_Init_Diag_Fail, _T("Non-Rework SO enter Re-DL Fail!"));
			EndDialog(0);
			return FALSE;
		}

		if (m_st_idParameter.str_imagePath.Find(m_st_idParameter.str_factoryVer) == -1)
		{			
			ShowResultMsgBox(CommErr_UI_Init_Diag_Fail, _T("Fail to load factory image!"));
			EndDialog(0);
			return FALSE;
		}
	}

#endif
//QueryDataFromSQN();
	/* Initial procedure */
	if (!InitialProcedure())
	{
		ShowResultMsgBox(m_str_errorCode, m_str_errorMsg);
		EndDialog(0);
		return FALSE;
	}

	//*************** Nancy add for Generator error code **************************/
	if (!m_obj_QueryErrCode.IsInitDLL())
	{
		ShowResultMsgBox(CommErr_UI_DLL_Load_Fail, m_obj_QueryErrCode.GetErrorMsg());
		EndDialog(0);
		return FALSE;
	}
	
	//*************** Nancy add for Generator error code **************************/

	return TRUE;
}

bool CUnifyUI_FacTestToolDlg::InitialProcedure()
{
	bool b_res = true;


	if (b_res)
	{
		b_res = InitialUIControl();//UIControl.ini
	}

	if (b_res)
	{
		b_res = InitialWidgetProperty();
	}

	

	if (b_res)
	{
		b_res = InitialResultCounter();
	}

	if (b_res)
	{
		b_res = InitialTimeCounter();
	}
	
	if (b_res)
	{
		//when UI init, if control power supply, power off it
		for (int i=PS_NUMBER -1; i>= 0; i--)
		{
			if (m_st_uiControl.i_PowerSupply[i])
			{
				if (!SetPowerSupply(1,i+1,1e-6,m_st_uiControl.lf_Current[i]))
				{
					LogMsg(_T("Set powersupply off Fail!"));
					m_str_errorCode = CommErr_UI_PowerSupply_Off_Fail;
					m_str_errorMsg = _T("SetPowerSupply OFF Fail!");
					return false;
				}

			}
		}

		if (m_st_uiControl.b_RelayBoard)
		{
			if (!m_obj_relayBoard.InitialRB())
			{
				m_str_errorCode = CommErr_UI_RelayBoard_Init_Fail;
				m_str_errorMsg = _T("Fail to init relay board!");
				LogMsg(m_str_errorMsg);
				return false;
			}

			if (!m_obj_relayBoard.SetRBAll(m_i_RBNumber,true))
			{
				m_str_errorCode = CommErr_UI_RelayBoard_Set_Fail;
				m_str_errorMsg = _T("Fail to set relay board! slot=15, OnOff=true");
				LogMsg(m_str_errorMsg);
				return false;
			}
			else
			{
				LogMsg(_T("Set relay board success"));
			}
			
		}

		// Set COM port
		m_obj_Communicate.SetCOM(m_st_uiControl.str_DevPort);
	}

#ifdef FACTORY_MODE	
	if (!m_st_idParameter.b_tsFlag && b_res && m_st_uiControl.b_SQN)
	{
		b_res = QueryDataFromSQN();
	}

	//no test key value, find it in pn config file
	if (!m_st_uiControl.b_SQN)
#endif	
	{
		//if (b_res)
		//{
		//	b_res = InitialParameter();//9j.ini
		//}	
	}

	if (b_res)
	{
		CString str_toolversion = m_st_idParameter.str_toolVer;

		b_res = InitialTabCtrl();

#ifdef FACTORY_MODE
		if(m_st_uiControl.b_SQN)
		{
			if (str_toolversion.CompareNoCase(m_st_idParameter.str_toolVer) != 0)
			{
				m_str_errorCode = CommErr_UI_CIM_Tool_Version_Invalide;
				m_str_errorMsg = _T("Tool Version invalide.\r\n DLL版本与CIM中不匹配！");
				LogMsg(m_str_errorMsg);
				return false;
			}
		}		
#endif
	}

	//must after initial tab , toolversion get from dll
	if (b_res)
	{
		if (m_st_idParameter.str_toolVer == _T(""))
		{
			m_st_idParameter.str_toolVer = m_str_appver;
		}
		b_res = InitialUIID();
	}



#ifndef RD_MODE
	if (m_b_syntime)
	{
		SynTime();
		::SetTimer(m_hWnd,2,m_i_interval*60000,NULL);
	}
#endif	
	return b_res;
}

/*luffy modify, for all station,// only  initial IMEI/FC station parameter */ 
//bool CUnifyUI_FacTestToolDlg::InitialParameter()
//{
//	TCHAR sz_iniFolderName[512] = {0};
//	CFileFind file_findCtrl;
//
//	LogMsg(_T("RD mode or SQN=0, get information from 9j.ini!"));
//
//	wsprintf(sz_iniFolderName, _T("%s\\%s\\PartNumber\\%s.ini"), CONFIG_PATH, m_st_idParameter.str_modelName, m_st_idParameter.str_partNumber);
//
//	if ((m_st_idParameter.str_station == STATION_IMEI) || (m_st_idParameter.str_station == STATION_FCHECK))
//	{
//		if (!file_findCtrl.FindFile(sz_iniFolderName))
//		{
//			m_str_errorCode = CommErr_UI_Config_File_Not_Found;
//			m_str_errorMsg.Format(_T("未找到ini配置文件!\nCan not find %s file!"),sz_iniFolderName);
//			return false;
//		}
//	}
//	
//	TCHAR sz_SectionBuffer[1024]={0};
//	int len = GetPrivateProfileSectionNames(sz_SectionBuffer,sizeof(sz_SectionBuffer),sz_iniFolderName);
//	TCHAR *p_begin = sz_SectionBuffer;
//	TCHAR *p_end = sz_SectionBuffer + len;
//
//	while(p_end > p_begin)
//	{
//		CString str_secName;
//		str_secName = p_begin;
//		p_begin += str_secName.GetLength();
//		p_begin++;
//
//		TCHAR sz_keys[4096]={0};
//		
//		len = GetPrivateProfileSection(str_secName, sz_keys,sizeof(sz_keys),sz_iniFolderName);
//
//		if (str_secName == m_st_idParameter.str_station)//Just send message of the section which is equal with current station   Nancy.Yang changed 20130614
//		{
//			TCHAR *p_keybegin = sz_keys;
//
//			CString str_keyvalue = _T("");
//			CString str_key = _T("");
//			CString str_value = _T("");
//
//			while(*p_keybegin)
//			{
//				str_keyvalue = p_keybegin;
//
//				p_keybegin += str_keyvalue.GetLength();
//				p_keybegin++;
//
//				if (str_keyvalue.Find(_T("=")) == -1)
//				{
//					continue;
//				}
//
//				str_key = str_keyvalue.Left(str_keyvalue.Find(_T("=")));
//				str_value = str_keyvalue.Mid(str_keyvalue.Find(_T("="))+1);
//				if ((str_key != _T(""))&&(str_value != _T("")))
//				{
//					m_map_TestItem[str_key] = str_value;
//				}	
//			}
//		}	
//	}
//
//	return true;
//}

bool CUnifyUI_FacTestToolDlg::InitialUIControl()
{
	TCHAR sz_temp[2048] = {0};
	TCHAR sz_iniFolderName[512] = {0};
	CString strTemp;

	/* Check ini file exist */
	CFileFind file_findCtrl;

 	TCHAR sz_folder[MAX_PATH] = {0};
 	GetModuleFileName(NULL, sz_folder, MAX_PATH);
 	PathRemoveFileSpec(sz_folder);
 
 	/* Check ini file exist */
 	wsprintf(sz_iniFolderName, _T("%s\\Qisda\\config.ini"), sz_folder);

	if (!file_findCtrl.FindFile(sz_iniFolderName))
	{
		LogMsg(_T("Can not find \\Qisda\\config.ini, use default value!"));
		m_st_uiControl.i_FAArea = 1;
		m_st_uiControl.b_SQN = true;
		m_b_NewErrCode = true;
	}
	else
	{
		::GetPrivateProfileString(_T("COMMON"), _T("FAArea"), _T("1"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.i_FAArea = StrToInt(sz_temp);
		strTemp.Format(_T("FAArea = %d"),StrToInt(sz_temp));
		LogMsg(strTemp);

		//fdcs related
		::GetPrivateProfileString(_T("COMMON"), _T("SQN"), _T("1"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.b_SQN = (StrToInt(sz_temp)!=0);

		::GetPrivateProfileString(_T("COMMON"), _T("NewErrCode"), _T("1"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_b_NewErrCode = (StrToInt(sz_temp)!=0);
	}

	memset(sz_iniFolderName,0,sizeof(sz_iniFolderName));
	/* Check ini file exist */	
	wsprintf(sz_iniFolderName, _T("%s\\%s\\Setting\\UIControl.ini"), CONFIG_PATH, m_st_idParameter.str_modelName);


	if (!file_findCtrl.FindFile(sz_iniFolderName))
	{
		m_str_errorCode = CommErr_UI_Config_File_Not_Found;
		m_str_errorMsg.Format(_T("未找到UI配置文件!\nCan not find %s file!"),sz_iniFolderName);
		LogMsg(m_str_errorMsg);
		return false;
	}

	::GetPrivateProfileString(_T("COMMON"), _T("NAL_6"), _T("116508"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.str_NAL6 = (CString)sz_temp;

	//syn time with server related
	::GetPrivateProfileString(_T("COMMON"), _T("SYNC"), _T("1"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_b_syntime = (StrToInt(sz_temp)!=0);

	::GetPrivateProfileString(_T("COMMON"), _T("IP"), _T("10.85.68.13"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_str_ip = sz_temp;

	::GetPrivateProfileString(_T("COMMON"), _T("FREQ"), _T("30"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_i_interval = StrToInt(sz_temp);


#ifdef FACTORY_MODE
	/* COMMON */
	if (!m_st_uiControl.b_SQN)
	{
		::GetPrivateProfileString(_T("COMMON"), _T("FACTORY_VER"), _T("CN"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_idParameter.str_factoryVer = sz_temp;
	}

	if (!m_st_uiControl.b_SQN)
	{
		::GetPrivateProfileString(_T("COMMON"), _T("SW_VER"), _T(""), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_idParameter.str_userLoadVer = sz_temp;
	}
#endif

	/*if (m_st_idParameter.str_pcba == _T(""))
	{
		::GetPrivateProfileString(_T("COMMON"), _T("HW_VER"), _T("101A0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_idParameter.str_pcba = sz_temp;
	}*/
	if (m_st_idParameter.str_station == _T(""))
	{
		m_str_errorCode = CommErr_UI_Init_Diag_Fail;
		m_str_errorMsg = _T("站别信息为空!\nStation is null!");
		return false;
	}

	/* FA */
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("ReadFA"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_ReadFA = (StrToInt(sz_temp)!=0);
	strTemp.Format(_T("ReadFA = %d"),StrToInt(sz_temp));
	LogMsg(strTemp);

	::GetPrivateProfileString(m_st_idParameter.str_station, _T("WriteFA"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_WriteFA = (StrToInt(sz_temp)!=0);
	strTemp.Format(_T("WriteFA = %d"),StrToInt(sz_temp));
	LogMsg(strTemp);

	::GetPrivateProfileString(m_st_idParameter.str_station, _T("idtype2"), _T("2"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.nIdType = (StrToInt(sz_temp));
	strTemp.Format(_T("nIdType = %d"),StrToInt(sz_temp));
	LogMsg(strTemp);


	/* Device control */
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("DevicePort"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.str_DevPort = sz_temp;

#ifdef RD_MODE
	/*Add by Magan for id type and id length*/
	if(m_st_idParameter.str_station == STATION_IMEI || m_st_idParameter.str_station == STATION_FCHECK)
	{
		::GetPrivateProfileString(m_st_idParameter.str_station, _T("IDTYPE"), _T("IMEI"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_idParameter.str_idtype = sz_temp;
		::GetPrivateProfileString(m_st_idParameter.str_station, _T("IDLENGTH"), _T("15"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_idParameter.i_idLength = StrToInt(sz_temp);
	}
#endif

	/*Device control multi for camera station*/
	if (m_st_idParameter.str_station == STATION_CAMERA || m_st_idParameter.str_station == STATION_LTE || m_st_idParameter.str_station == STATION_2G3GTEST)
	{
		::GetPrivateProfileString(m_st_idParameter.str_station, _T("Multi"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.b_Multi = (StrToInt(sz_temp)!=0);
		strTemp.Format(_T("Multi = %d"),StrToInt(sz_temp));
		LogMsg(strTemp);

		::GetPrivateProfileString(m_st_idParameter.str_station, _T("Device1"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.str_Device1 = sz_temp;

		::GetPrivateProfileString(m_st_idParameter.str_station, _T("Device2"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.str_Device2 = sz_temp;

		::GetPrivateProfileString(m_st_idParameter.str_station, _T("Device3"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.str_Device3 = sz_temp;

		::GetPrivateProfileString(m_st_idParameter.str_station, _T("Device4"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.str_Device4 = sz_temp;
	}

	if (m_st_uiControl.str_DevPort.Find(_T("COM")) == 0)
	{
		m_st_uiControl.str_DevPort = _T("\\\\.\\") + m_st_uiControl.str_DevPort;
	}

	::GetPrivateProfileString(m_st_idParameter.str_station, _T("DeviceControl"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.i_DevControl = StrToInt(sz_temp);

	::GetPrivateProfileString(m_st_idParameter.str_station, _T("DevInCommand"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.str_DevInCommand = sz_temp;
	m_st_uiControl.str_DevInCommand.Replace(_T("\\r"),_T("\r"));

	::GetPrivateProfileString(m_st_idParameter.str_station, _T("DeviceInDelay"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.i_DeviceInDelay = StrToInt(sz_temp);

	::GetPrivateProfileString(m_st_idParameter.str_station, _T("DevOutCommand"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.str_DevOutCommand = sz_temp;
	m_st_uiControl.str_DevOutCommand.Replace(_T("\\r"),_T("\r"));

	/* Scan picasso */
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("ScanPicasso"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_ScanPicasso = (StrToInt(sz_temp)!=0);

	//if prescan edit box display
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("PreScan"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_PreScan = (StrToInt(sz_temp)!=0) & m_st_uiControl.b_ScanPicasso;

	//if prescan,if success, if auto run next test
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("AutoRunPreScan"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
    m_st_uiControl.b_AutoRunPreScan = (StrToInt(sz_temp)!=0) & m_st_uiControl.b_ScanPicasso & m_st_uiControl.b_PreScan;
	
	
	//if WriteTAG
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("ScanTAG"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_WriteTagFrame =  (StrToInt(sz_temp)!=0);

	//if b_ReadTag
	//::GetPrivateProfileString(m_st_idParameter.str_station, _T("ReadTag"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	//m_st_uiControl.b_ReadTag =  (StrToInt(sz_temp)!=0);

	/* Scan Tag */
	//::GetPrivateProfileString(m_st_idParameter.str_station, _T("ScanTag"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	//m_st_uiControl.b_ScanTag = (StrToInt(sz_temp)!=0);

	//if WriteSN
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("WriteSN"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_WriteSn =  (StrToInt(sz_temp)!=0);

	//if b_ReadSn
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("ReadSn"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_ReadSn =  (StrToInt(sz_temp)!=0);

	/* Scan Sn */
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("ScanSn"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_ScanSn = (StrToInt(sz_temp)!=0);
	m_st_uiControl.b_WriteSnFrame = (StrToInt(sz_temp)!=0);

	/* Time out */
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("Timeout"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.i_ToolTimeout   = StrToInt(sz_temp);

	
	//m_st_uiControl.b_ScanTag				= false;
	//for wirte station
//	if (m_st_idParameter.str_station.Compare(L"WRITE") == 0){
	//	m_st_uiControl.b_WriteTagFrame = true;
//		m_st_uiControl.b_WriteSnFrame = true;
//	}

	//auto run only for OS_DL
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("AutoRun"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	if (m_st_idParameter.str_station == STATION_OS_DL)
	{
		m_st_uiControl.b_AutoRun = (StrToInt(sz_temp)!=0);
		strTemp.Format(_T("AutoRun = %d"),StrToInt(sz_temp));
		LogMsg(strTemp);
	}

	//if display NAL input edit box in imei
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("NAL"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_NAL = (StrToInt(sz_temp)!=0);

    
	//if responds to enter key down
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("VKEnter"), _T("1"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_VKEnter = (StrToInt(sz_temp)!=0);

	//if display Acer22 input edit box in imei
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("ACER22"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_ACER22 = (StrToInt(sz_temp)!=0);

	/* Power supply */
	CString str_attName = _T("");
	for (int i = 0;i < PS_NUMBER; i++)
	{
		str_attName.Format(_T("PowerSupplyControl%d"),i);
		::GetPrivateProfileString(m_st_idParameter.str_station, str_attName, _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.i_PowerSupply[i] = StrToInt(sz_temp);

		str_attName.Format(_T("PSAddr%d"),i);
		::GetPrivateProfileString(m_st_idParameter.str_station, str_attName, _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.i_PSAddr[i] = StrToInt(sz_temp);

		str_attName.Format(_T("PSInterface%d"),i);
		::GetPrivateProfileString(m_st_idParameter.str_station, str_attName, _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.i_PSInterface[i] = StrToInt(sz_temp);

		str_attName.Format(_T("PowerOnDelay%d"),i);
		::GetPrivateProfileString(m_st_idParameter.str_station, str_attName, _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.i_PowerOnDelay[i] = StrToInt(sz_temp);

		str_attName.Format(_T("Current%d"),i);
		::GetPrivateProfileString(m_st_idParameter.str_station, str_attName, _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.lf_Current[i] = _tstof(sz_temp);

		str_attName.Format(_T("Voltage%d"),i);
		::GetPrivateProfileString(m_st_idParameter.str_station, str_attName, _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.lf_Voltage[i] = _tstof(sz_temp);

		str_attName.Format(_T("ShutVol%d"),i);
		::GetPrivateProfileString(m_st_idParameter.str_station, str_attName, _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
		m_st_uiControl.lf_ShutVol[i] = _tstof(sz_temp);
	}
	
	//if Need relayboard
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("RelayBoard"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_st_uiControl.b_RelayBoard = (StrToInt(sz_temp)!=0);

	//relayboard number
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("RBNumber"), _T("0"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_i_RBNumber = StrToInt(sz_temp);
	//relayboard mask
	::GetPrivateProfileString(m_st_idParameter.str_station, _T("RBMask"), _T("1"), sz_temp, sizeof(sz_temp), sz_iniFolderName);
	m_b_RBMask = (StrToInt(sz_temp)!=0);

	/*delete by Nancy*/
	/*if (m_st_uiControl.b_NAL)
	{
		if (m_st_uiControl.str_NAL6.GetLength() != 6)
		{
			m_str_errorCode = CommErr_UI_Config_File_Miss_Parameter;
			m_str_errorMsg = _T("配置文件损坏!\nError in UI config file!");
			return false;
		}
	}*/

	return true;
}


bool CUnifyUI_FacTestToolDlg::InitialWidgetProperty()
{
	/* Initial Background color */

	COLORREF colour_dialogBKG;
	COLORREF colour_widgetText = RGB(0,0,0);

#ifdef RD_MODE
	colour_dialogBKG = BKGCOLOUR_RD;
#else
	if (m_st_idParameter.b_tsFlag == true)
	{
		colour_dialogBKG = BKGCOLOUR_TS;
	}
	else if(!m_st_idParameter.b_OpenbyEntranceTool)
	{
		LogMsg(_T("CUnifyUI_FacTestToolDlg::InitialWidgetProperty b_OpenbyEntranceTool = false"));
		colour_dialogBKG = BKGCOLOUR_FACTORY_BAT;
	}
	else
	{
		LogMsg(_T("CUnifyUI_FacTestToolDlg::InitialWidgetProperty b_OpenbyEntranceTool = true"));
		colour_dialogBKG = BKGCOLOUR_FACTORY;
	}
#endif

	m_brushDiagBk.CreateSolidBrush(colour_dialogBKG);

	m_static_id.SetTextColor(colour_widgetText);
	m_static_id.SetBkColor(colour_dialogBKG);

	m_static_model.SetTextColor(colour_widgetText);
	m_static_model.SetBkColor(colour_dialogBKG);

	m_static_line.SetTextColor(colour_widgetText);
	m_static_line.SetBkColor(colour_dialogBKG);

	m_static_pcba.SetTextColor(colour_widgetText);
	m_static_pcba.SetBkColor(colour_dialogBKG);

	m_static_so.SetTextColor(colour_widgetText);
	m_static_so.SetBkColor(colour_dialogBKG);

	m_static_partNumber.SetTextColor(colour_widgetText);
	m_static_partNumber.SetBkColor(colour_dialogBKG);

	m_static_toolVer.SetTextColor(colour_widgetText);
	m_static_toolVer.SetBkColor(colour_dialogBKG);

	m_static_employee.SetTextColor(colour_widgetText);
	m_static_employee.SetBkColor(colour_dialogBKG);

	m_static_factoryVer.SetTextColor(colour_widgetText);
	m_static_factoryVer.SetBkColor(colour_dialogBKG);

	m_static_userLoadVer.SetTextColor(colour_widgetText);
	m_static_userLoadVer.SetBkColor(colour_dialogBKG);

	m_static_record.SetTextColor(colour_widgetText);
	m_static_record.SetBkColor(colour_dialogBKG);

	m_static_total.SetTextColor(colour_widgetText);
	m_static_total.SetBkColor(colour_dialogBKG);

	m_static_pass.SetTextColor(colour_widgetText);
	m_static_pass.SetBkColor(colour_dialogBKG);

	m_static_fail.SetTextColor(colour_widgetText);
	m_static_fail.SetBkColor(colour_dialogBKG);

	m_static_time.SetTextColor(colour_widgetText);
	m_static_time.SetBkColor(colour_dialogBKG);

	m_static_execute.SetTextColor(colour_widgetText);
	m_static_execute.SetBkColor(colour_dialogBKG);

	/* Set ID edit property */
	EnableDisableEdit(false);

	m_font_button.CreateFont(
		30,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,				   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial"));

	m_button_start.SetFont(&m_font_button, 1);
	m_button_stop.SetFont(&m_font_button, 1);
	m_button_exit.SetFont(&m_font_button, 1);

	EnableDisableButton(true);

	return true;
}

bool CUnifyUI_FacTestToolDlg::InitialUIID()
{

	//lion
	m_edit_model.SetWindowText(m_st_idParameter.str_modelName);
	m_edit_partNumber.SetWindowText(m_st_idParameter.str_daynight);
	m_edit_line.SetWindowText(m_st_idParameter.str_line);
	m_edit_line.SetWindowText(m_st_idParameter.str_so);

	CString str_so_type;
	if (m_st_idParameter.str_CRType != _T(""))
	{
		str_so_type = m_st_idParameter.str_so + _T(" [") + m_st_idParameter.str_CRType + _T("]");
	}
	else
	{
		str_so_type = m_st_idParameter.str_so + _T(" [") + m_st_idParameter.str_soType + _T("]");
	}
	m_edit_so.SetWindowText(str_so_type);
	//m_edit_partNumber.SetWindowText(m_st_idParameter.str_partNumber);
	m_edit_toolVer.SetWindowText(m_st_idParameter.str_toolVer);
	m_edit_employee.SetWindowText(m_st_idParameter.str_employee);
	m_edit_factoryVer.SetWindowText(m_st_idParameter.str_factoryVer);
	m_edit_userLoadVer.SetWindowText(m_st_idParameter.str_userLoadVer);

	return true;
}

bool CUnifyUI_FacTestToolDlg::InitialResultCounter()
{
	CString str_tempPass;
	CString str_tempFail;
	CString str_tempTotal;

#ifdef FACTORY_MODE
	if (!m_st_idParameter.b_tsFlag)
	{
		if (!QueryRecordInfoFromCIM())
		{
			return false;
		}
	}
#endif

	str_tempPass.Format(_T("%d"), m_i_totalNo);
	str_tempFail.Format(_T("%d"), m_i_passNo);
	str_tempTotal.Format(_T("%d"), m_i_failNo);

	m_edit_total.SetWindowText(str_tempPass);
	m_edit_pass.SetWindowText(str_tempFail);
	m_edit_fail.SetWindowText(str_tempTotal);

	return true;
}

bool CUnifyUI_FacTestToolDlg::InitialTimeCounter()
{
	m_i_timeCount = 0;
	m_edit_time.SetWindowText(_T("0"));

	return true;
}

bool CUnifyUI_FacTestToolDlg::InitialUITitle()
{
	char sz_Temp[MAX_PATH] = {0};

	GetModuleFileNameA(NULL, sz_Temp, MAX_PATH);
	PathRemoveFileSpecA(sz_Temp);
	m_str_titlePicturePath = sz_Temp;
#ifdef RD_MODE
	m_str_titlePicturePath += _T("\\res\\RD.jpg");
#else
	m_str_titlePicturePath += _T("\\res\\Header.jpg");
#endif


	//get image rectangle
	CRect rect;
	CStatic *pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TITLE);
	if (pWnd == NULL)
	{
		return false;
	} 
	else
	{
		pWnd->GetClientRect(&rect);
	}

	//draw image
	CDC* pDC = pWnd->GetWindowDC();  
	pDC->SetStretchBltMode(HALFTONE);
	pDC->SetBrushOrg(0,0);//improve image quality	

	CImage image;      
	image.Load(m_str_titlePicturePath);
	if (image.IsNull())
	{
		return false;
	}
	image.Draw( pDC->m_hDC, rect); 

	//draw Text
	CString str_toolname = m_st_idParameter.str_station + _T(" UNIFY TOOL");
	if (m_st_idParameter.b_tsFlag)
	{
		str_toolname = _T("TS ") + str_toolname;
	}
	//rect.left = rect.left + 200;
	//rect.right = rect.right - 280;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));
	CFont font,*pOldFont;
	font.CreateFont(50, //font hight 
		18, // font width 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("微软雅黑")); // lpszFac 
	pOldFont=pDC->SelectObject(&font);	
	pDC->DrawText(str_toolname,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);	

	return true;

}

bool CUnifyUI_FacTestToolDlg::InitialTabCtrl()
{
	if ((m_st_idParameter.str_station == STATION_PREDL)||
		(m_st_idParameter.str_station == STATION_DL)||
		(m_st_idParameter.str_station == STATION_OS_DL)||
		(m_st_idParameter.str_station == STATION_BIST)||
		(m_st_idParameter.str_station == STATION_REDL))    //2012.08.20 
	{
		/* Add DownLoad Page */
		CDownLoadDlg* p_DownLoad = new CDownLoadDlg(this);
		if (p_DownLoad == NULL)
		{
			m_str_errorCode = CommErr_UI_Init_Diag_Fail;
			m_str_errorMsg = _T("初始化tab页失败!\nInitial station tab page fail!");
			LogMsg(m_str_errorMsg);
			return false;
		}
		else
		{
			p_DownLoad->Create(CDownLoadDlg::IDD, &m_tab_stationPageCtrl);
			m_tab_stationPageCtrl.InsertItem(m_tab_stationPageCtrl.GetItemCount(), m_st_idParameter.str_station);
			m_pageArray.Add(p_DownLoad);
		}
	} 
	else if ((m_st_idParameter.str_station == STATION_IMEI)||
			 (m_st_idParameter.str_station == STATION_FCHECK)||
			 (m_st_idParameter.str_station == STATION_NAL_SCAN))
	{
		/* Add IMEI/FC Page */
		CIMEIFCDlg* p_IMEI_FC = new CIMEIFCDlg(this);
		if (p_IMEI_FC == NULL)
		{
			m_str_errorCode = CommErr_UI_Init_Diag_Fail;
			m_str_errorMsg = _T("初始化tab页失败!\nInitial station tab page fail!");
			LogMsg(m_str_errorMsg);
			return false;
		}
		else
		{
			p_IMEI_FC->Create(CIMEIFCDlg::IDD, &m_tab_stationPageCtrl);
			m_tab_stationPageCtrl.InsertItem(m_tab_stationPageCtrl.GetItemCount(), m_st_idParameter.str_station);
			m_pageArray.Add(p_IMEI_FC);	
		}
	}
	else if ((m_st_idParameter.str_station == STATION_CAMERA || m_st_idParameter.str_station == STATION_LTE || m_st_idParameter.str_station == STATION_2G3GTEST) && m_st_uiControl.b_Multi)
	{
		COthersMultiDlg* p_Other_Station = new COthersMultiDlg(this);
		if (p_Other_Station == NULL)
		{
			m_str_errorCode = CommErr_UI_Init_Diag_Fail;
			m_str_errorMsg = _T("初始化tab页失败!\nInitial station tab page fail!");
			LogMsg(m_str_errorMsg);
			return false;
		}
		else
		{
			p_Other_Station->Create(COthersMultiDlg::IDD, &m_tab_stationPageCtrl);
			m_tab_stationPageCtrl.InsertItem(m_tab_stationPageCtrl.GetItemCount(), m_st_idParameter.str_station);
			m_pageArray.Add(p_Other_Station);
		}
	}
	else
	{
		/* Add RF/FT/Wireless/MMI Page */
		COtherStationDlg* p_Other_Station = new COtherStationDlg(this);
		if (p_Other_Station == NULL)
		{
			m_str_errorCode = CommErr_UI_Init_Diag_Fail;
			m_str_errorMsg = _T("初始化tab页失败!\nInitial station tab page fail!");
			LogMsg(m_str_errorMsg);
			return false;
		}
		else
		{
			p_Other_Station->Create(COtherStationDlg::IDD, &m_tab_stationPageCtrl);
			m_tab_stationPageCtrl.InsertItem(m_tab_stationPageCtrl.GetItemCount(), m_st_idParameter.str_station);
			m_pageArray.Add(p_Other_Station);
		}
	}

	/* Adjust the pages to the appropriate location */
	CRect rect;
	m_tab_stationPageCtrl.GetClientRect(&rect);
	m_tab_stationPageCtrl.AdjustRect(FALSE, &rect);
	for (int i = 0; i < m_pageArray.GetSize(); i++)
	{
		m_pageArray.GetAt(i)->MoveWindow(rect);
	}

	m_tab_stationPageCtrl.SetCurSel(0);

	LRESULT lr;
	OnTcnSelchangeTabStationpage(0, &lr);

	return true;
}

void CUnifyUI_FacTestToolDlg::OnTcnSelchangeTabStationpage(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	for (int i_pageIdx = 0; i_pageIdx < m_pageArray.GetSize(); i_pageIdx++) 
	{
		if (m_pageArray[i_pageIdx] == NULL) 
		{
			continue;
		}

		if (i_pageIdx == m_tab_stationPageCtrl.GetCurSel()) 
		{
			m_pageArray[i_pageIdx]->ShowWindow(SW_SHOW);
		}
		else 
		{
			m_pageArray[i_pageIdx]->ShowWindow(SW_HIDE);
		}
	}

	*pResult = 0;
}

/* Display or make grayed functions */
void CUnifyUI_FacTestToolDlg::EnableDisableButton(bool b_flag)
{
	m_button_start.EnableWindow(b_flag);
	m_button_exit.EnableWindow(b_flag);
	m_button_stop.EnableWindow(false);
	if (m_st_idParameter.str_station == STATION_OS_DL && m_st_uiControl.b_AutoRun)
	{
		m_button_start.EnableWindow(false);                     //For auto Run to disable Go Button
		m_button_exit.EnableWindow(false);
	}
}

void CUnifyUI_FacTestToolDlg::EnableDisableEdit(bool b_flag)
{
	/* Set ID edit property */
#ifdef RD_MODE
	m_edit_model.SetReadOnly(b_flag);
	m_edit_line.SetReadOnly(b_flag);
	m_edit_pcba.SetReadOnly(b_flag);
	m_edit_so.SetReadOnly(b_flag);
	m_edit_partNumber.SetReadOnly(b_flag);
	m_edit_toolVer.SetReadOnly(b_flag);
	m_edit_employee.SetReadOnly(b_flag);
	m_edit_factoryVer.SetReadOnly(b_flag);
	m_edit_userLoadVer.SetReadOnly(b_flag);
#else
	m_edit_model.SetReadOnly(TRUE);
	m_edit_line.SetReadOnly(TRUE);
	m_edit_pcba.SetReadOnly(TRUE);
	m_edit_so.SetReadOnly(TRUE);
	m_edit_partNumber.SetReadOnly(TRUE);
	m_edit_toolVer.SetReadOnly(TRUE);
	m_edit_employee.SetReadOnly(TRUE);
	m_edit_factoryVer.SetReadOnly(TRUE);
	m_edit_userLoadVer.SetReadOnly(TRUE);
#endif

	m_edit_total.SetReadOnly(TRUE);
	m_edit_pass.SetReadOnly(TRUE);
	m_edit_fail.SetReadOnly(TRUE);
	m_edit_time.SetReadOnly(TRUE);

	/* Renew window*/
	UpdateWindow();

}

void CUnifyUI_FacTestToolDlg::OnCancel()
{
	for (int i = 0; i < m_pageArray.GetSize(); i++)	
	{
		CDialog* pPage = m_pageArray.GetAt(i);

		if (pPage != NULL)
			delete pPage;
	}

	m_pageArray.RemoveAll();

	CDialog::OnCancel();
}

LRESULT CUnifyUI_FacTestToolDlg::OnFocus(WPARAM wParam, LPARAM lParam)
{
	Sleep(20);
	int i_pageIdx = 0;

	i_pageIdx = m_tab_stationPageCtrl.GetCurSel();
	if (m_pageArray[i_pageIdx] != NULL)
	{
		m_pageArray.GetAt(i_pageIdx)->ResetPage();
	}

	return 0;
}

bool CUnifyUI_FacTestToolDlg::Begin()
{
	m_map_slotPICSData.clear();
	m_map_picasso.clear();
	m_str_errorCode = _T("");
	m_str_errorMsg = _T("");

	/* Count time*/
	InitialTimeCounter();
	::SetTimer(m_hWnd,1,1000,NULL);

	m_map_slotPICSData.clear();

	/* Enable button */
	EnableDisableButton(false);

#ifdef RD_MODE
	EnableDisableEdit(true);

	if (!GetIDFromUI())
	{
		return false;
	}
#endif

	return true;
}

LRESULT CUnifyUI_FacTestToolDlg::OnEnd(WPARAM wParam, LPARAM lParam)  
{  
	CSingleLock singleLock(&m_obj_end, TRUE);

	CString strTmp;

	if ((m_st_idParameter.str_station == STATION_CAMERA || m_st_idParameter.str_station == STATION_LTE || m_st_idParameter.str_station == STATION_2G3GTEST) && m_st_uiControl.b_Multi)
	{
		m_i_cameraNo++;

		if (m_i_cameraNo == 4)
		{
			//when test finished, power off
			for (int i=PS_NUMBER -1; i>= 0; i--)
			{
				if (m_st_uiControl.i_PowerSupply[i] & POWEROFF)
				{
					if (!SetPowerSupply(1,i+1,m_st_uiControl.lf_ShutVol[i],m_st_uiControl.lf_Current[i]))
					{
						LogMsg(_T("Set powersupply off Fail!"));
					}
					else
					{
						LogMsg(_T("Set powersupply off Success!"));
					}
					Sleep(m_st_uiControl.i_PowerOnDelay[i]);
				}
			}

			//when test finished, power off
			for (int i=PS_NUMBER -1; i>= 0; i--)
			{
				if (m_st_uiControl.i_PowerSupply[i] & POWEROFF)
				{
					if (!SetPowerSupply(1,i+1,1e-6,m_st_uiControl.lf_Current[i]))
					{
						LogMsg(_T("Set powersupply off Fail!"));
					}
					else
					{
						LogMsg(_T("Set powersupply off Success!"));
					}
				}
			}
		}
	}
	else
	{
		//when test finished, power off
		for (int i=PS_NUMBER -1; i>= 0; i--)
		{
			if (m_st_uiControl.i_PowerSupply[i] & POWEROFF)
			{
				if (!SetPowerSupply(1,i+1,m_st_uiControl.lf_ShutVol[i],m_st_uiControl.lf_Current[i]))
				{
					LogMsg(_T("Set powersupply off Fail!"));
				}
				else
				{
					LogMsg(_T("Set powersupply off Success!"));
				}
				Sleep(m_st_uiControl.i_PowerOnDelay[i]);
			}
		}

		//when test finished, power off
		for (int i=PS_NUMBER -1; i>= 0; i--)
		{
			if (m_st_uiControl.i_PowerSupply[i] & POWEROFF)
			{
				if (!SetPowerSupply(1,i+1,1e-6,m_st_uiControl.lf_Current[i]))
				{
					LogMsg(_T("Set powersupply off Fail!"));
				}
				else
				{
					LogMsg(_T("Set powersupply off Success!"));
				}
			}
		}
	}
	

	//when test finished, make device out
	if (m_st_uiControl.i_DevControl & DEVOUT)
	{
		if ((m_st_idParameter.str_station == STATION_CAMERA || m_st_idParameter.str_station == STATION_LTE || m_st_idParameter.str_station == STATION_2G3GTEST) && m_st_uiControl.b_Multi)
		{
			if (m_i_cameraNo==4)
			{
				if (!SetDeviceOut())
				{
					LogMsg(_T("Set Device Out Fail!"));
				}
				else
				{
					LogMsg(_T("Set Device Out Success!"));
				}
			}
		}
		else
		{
			if (!SetDeviceOut())
			{
				LogMsg(_T("Set Device Out Fail!"));
			}
			else
			{
				LogMsg(_T("Set Device Out Success!"));
			}
		}
	}
	
	st_pageReturn* p_st_result = (st_pageReturn*)(lParam);
	bool b_setResult = false;

	m_st_result.i_slot = p_st_result->i_slot;
	CString str_errorCode = p_st_result->str_errorcode;
	CString str_errorMsg = p_st_result->str_errorMsg;

	strTmp.Format(_T("[%d] OnEnd before convert-> ErrCode:%s, ErrMsg:%s"),p_st_result->i_slot,str_errorCode,str_errorMsg);
	LogMsg(strTmp);
	
	CString str_errcodeBef = str_errorCode;//Record for RecordResultToFile

	//*************** Nancy add for Generator error code **************************/
	bool bInCIM = true;

	if (m_b_NewErrCode)
	{
		if ((str_errorCode.Left(2) != _T("DB")) && (str_errorCode.Left(2) != _T("UI")))
		{
			char sz_errorCode[200]={0};
			char sz_genErrcode[20]={0};
			char sz_genErrMsg[512]={0};
			char sz_errmsg[200]={0};
			
			CStringToChar(str_errorCode,sz_errorCode);

			if (!m_obj_QueryErrCode.QueryErrCode(sz_errorCode, sz_genErrcode, sz_genErrMsg, sz_errmsg, bInCIM))
			{		
				strTmp.Format(_T("[%d] Disp2:Error Not Found:%s:%s"),p_st_result->i_slot,str_errorCode,str_errorMsg);
				LogMsg(strTmp);//No this error defined
				/**
				* @brief Insert fail record even the error code was not defined, 20150330
				*
				* 1. If the error code length = 0, then set the error code value = NULL
				* 2. If the error code length > 30, then error code value only keep first 30 characters
				* 3. Modify InsertFAIL_data function In DBAccess, do not return false if error code was not defined in PICSECD table
				*/
				if (str_errorCode.GetLength() == 0)
				{
					str_errorCode = _T("NULL");
				}
				else if(str_errorCode.GetLength() > 30)
				{
					str_errorCode.Mid(0, 30);
				}

				strTmp.Format(_T("[%d] Error_Not_Found:%s\n%s"),p_st_result->i_slot,str_errorCode, str_errorMsg);
				str_errorMsg = strTmp;
			}
			else
			{
				str_errorCode = ConvertString(sz_genErrcode);
				str_errorMsg = ConvertString(sz_genErrMsg);

				strTmp.Format(_T("[%d] Disp3:Error Found:%s:%s"),p_st_result->i_slot,str_errorCode,str_errorMsg);
				LogMsg(strTmp);//No this error defined
			}	
		}
	}

	//*************** Nancy add for Generator error code **************************/

	/* if get empty error code,set as unknown */
	if (str_errorCode.GetLength() < 4)
	{
		if (m_str_errorCode.IsEmpty())
		{
			str_errorCode = CommErr_UI_Unknown_Error_From_DLL;
			str_errorMsg = _T("Unknown ErrorCode!");
			LogMsg(str_errorMsg);
		}
		else
		{
			str_errorCode = m_str_errorCode;
			str_errorMsg = m_str_errorMsg;
		}
	}
	else if (str_errorCode == _T("PASS"))
	{
		//if test pass for trigger or osdl, need to set last result to tab page,
		b_setResult = true;
	}
	else if (str_errorCode.Find(_T("DB1024")) != -1)
	{
		str_errorMsg = _T("请送TS解锁");
		LogMsg(str_errorMsg);
	}

	m_st_result.str_errorcode = str_errorCode;

	/* Record test result to csv file */
	CString str_id = m_map_picasso[m_st_result.i_slot];

	if (!str_id.IsEmpty())
	{
#ifdef FACTORY_MODE
		/**
		* @brief Insert and check accessory used times (20140724)
		*
		* 1. Accessory list is send from Entrance tool(v2.018+) or CIM bat cmd line (Ex. "USB:A111111,Headset:B11112
		* 2. InsertAndCheckAccessory will increase the accessory used times first, then check the times if lower than (max times-50
		* 3. Do InsertAndCheckAccessory every time, no matter the test result is pass or fail
		*
		*     DB Table: ConsumeInfo, consumePCS
		*/
		bool bAccessoryMessage = false;
		if (m_st_idParameter.str_accessoryList.GetLength() != 0)
		{
			LogMsg(_T("AccessoryList: ") + m_st_idParameter.str_accessoryList);
			bool bNeedChange = false;
			bool bRet = InsertAndCheckAccessory(bNeedChange);
			if (bNeedChange)
			{
				bAccessoryMessage = true;
				if (b_setResult) // keep show real error code in first priority 
				{
					str_errorCode = _T("请更换附件");
					str_errorMsg = m_str_errorMsg;
				}
				strTmp.Format(_T("[%d] Need ChangeAccessory!"),p_st_result->i_slot);
				LogMsg(strTmp);
			}
			else if (!bRet)
			{
				bAccessoryMessage = true;
				if (b_setResult) // keep show real error code in first priority 
				{
					str_errorCode = m_str_errorCode;
					str_errorMsg = m_str_errorMsg;
				}
				strTmp.Format(_T("[%d] InsertAndCheckAccessory Fail"),p_st_result->i_slot);
				LogMsg(strTmp);
			}
		}

		//1, check if need show retest,must before insert test record
		if(b_setResult && !m_st_idParameter.b_tsFlag && (m_st_idParameter.str_segment.CompareNoCase(_T("PACKAGE"))!= 0))
		{
			char sz_id[20] = {0};
			char sz_model[20]={0};
			char sz_process[20]={0};

			CStringToChar(m_st_idParameter.str_modelName,sz_model);
			CStringToChar(m_st_idParameter.str_station,sz_process);
			CStringToChar(str_id,sz_id);
			if (NeedReTest(sz_id,sz_model,sz_process))
			{
				str_errorCode = _T("请重测一次");
				str_errorMsg = _T("请重测一次");
				strTmp.Format(_T("[%d] Need ReTest!"),p_st_result->i_slot);
				LogMsg(strTmp);
			}
			else
			{
				if (!bAccessoryMessage)
					str_errorMsg = _T("");
			}
		}
#endif
		//2, insert to CIM
		strTmp.Format(_T("[%d] Picasso is: %s"),p_st_result->i_slot,str_id);
		LogMsg(strTmp);

		if(m_st_idParameter.str_station == STATION_IMEI)
		{
			if(!b_setResult)
			{
				if(m_str_InsertCIM != _T("Y"))
					m_str_InsertCIM = _T("N");
			}
			else //IMEI station IMEIUSED flag not used tool error
			{
				if (m_str_InsertCIM != _T("Y"))
				{
					m_str_errorCode = _T("IMEI0012");
					m_str_errorMsg = _T("Check IMEI flag fail");
					LogMsg(_T("IMEI station: Test pass, but IMEIUSED is not Y, please check your code!"));
					b_setResult = false;
					m_str_InsertCIM = _T("Y");
				}
			}
		}
		else
			m_str_InsertCIM = _T("Y");

		if (!SetTestRecord(b_setResult,bInCIM))
		{
			if (b_setResult)
			{
				str_errorCode = m_str_errorCode;
				str_errorMsg = m_str_errorMsg;	
			}
		}
		
		//3,other show message query
		else if(b_setResult && !m_st_idParameter.b_tsFlag)
		{
#ifdef FACTORY_MODE
			if ( m_st_idParameter.str_segment.CompareNoCase(_T("PACKAGE")) == 0)
			{			
				if (m_obj_QueryCIM.IsCheckOOB())
				{
					str_errorCode = _T("送检OOB");
					strTmp.Format(_T("[%d] Need Check OOB!"),p_st_result->i_slot);
					LogMsg(strTmp);
				}

				if (m_st_idParameter.str_station == STATION_NAL_SCAN)
				{
					char sz_id[20] = {0};
					char sz_model[20]={0};
					CStringToChar(m_st_idParameter.str_modelName,sz_model);
					CStringToChar(str_id,sz_id);

					if (!m_obj_QueryCIM.UnlinkKeypartsFromCIM(sz_id,sz_model))
					{
						str_errorCode = m_obj_QueryCIM.GetErrorCode();
						str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
					}

					strTmp.Format(_T("[%d] Unlink Keyparts From CIM!"),p_st_result->i_slot);
					LogMsg(strTmp);
				}
			}
#endif
		}
	}
	

	m_str_InsertCIM = _T("-1");

	RecordResultToFile(str_id, str_errorCode, str_errcodeBef);

	//stop timer
	if(m_st_idParameter.str_station == STATION_OS_DL && m_st_uiControl.b_AutoRun)
	{
		
	}
	else if ((m_st_idParameter.str_station == STATION_CAMERA || m_st_idParameter.str_station == STATION_LTE || m_st_idParameter.str_station == STATION_2G3GTEST) && m_st_uiControl.b_Multi)
	{
		if (m_i_cameraNo == 4)
		{
			m_i_cameraNo=0;
			KillTimer(1);
			ResetUIForNextRun();
		}
	}
	else
	{
         KillTimer(1);
	}

	// Show error code and error message if not multi-slot test
	if ((m_st_idParameter.str_station != STATION_PREDL) 
		&& (m_st_idParameter.str_station != STATION_OS_DL) 
		&& (m_st_idParameter.str_station != STATION_DL)
		&& (m_st_idParameter.str_station != STATION_BIST)
		&& (m_st_idParameter.str_station != STATION_REDL)
		&& !((m_st_idParameter.str_station == STATION_CAMERA || m_st_idParameter.str_station == STATION_LTE || m_st_idParameter.str_station == STATION_2G3GTEST) && m_st_uiControl.b_Multi)
		)
	{	
		LogMsg(_T("Set Result to UI --- [not Multi]"));
		ShowResultMsgBox(str_errorCode, str_errorMsg);
	}
	else if((m_st_idParameter.str_station == STATION_CAMERA || m_st_idParameter.str_station == STATION_LTE || m_st_idParameter.str_station == STATION_2G3GTEST) && m_st_uiControl.b_Multi)
	{   
		LogMsg(_T("Set Result to UI --- [Multi for CAMERA,LTE,2G3G]"));
        SetTestResultToStation(m_st_result.i_slot,str_errorCode);
	}
	else //if (b_setResult) //if download success, set each slot final result according to insert result,
	{
		if (b_setResult || (m_st_idParameter.str_station == STATION_OS_DL && m_st_uiControl.b_AutoRun))
		{
			strTmp.Format(_T("[%d] Set Result to UI --- [Multi DL success or OS_DL auto run]"),m_st_result.i_slot);
			LogMsg(strTmp);
			SetTestResultToStation(m_st_result.i_slot,str_errorCode);
		}		
	}

	/* Run stop flag */
	if (!((m_st_idParameter.str_station == STATION_CAMERA || m_st_idParameter.str_station == STATION_LTE || m_st_idParameter.str_station == STATION_2G3GTEST) && m_st_uiControl.b_Multi))
	{
		LogMsg(_T("Reset UI for not multi-CAMERA,LTE,2G3G"));
		ResetUIForNextRun();
	}
	
	m_str_errorCode = _T("");
	m_str_errorMsg = _T("");
	m_b_isTesting = false;

	
	strTmp.Format(_T("[%d] ====================== Message END(V%s) ==========================="), m_st_result.i_slot,m_str_appver);
	LogMsg(strTmp);

	if (!b_setResult)
	{
		m_i_testtime = 0;
	}

	return 0;
}

void CUnifyUI_FacTestToolDlg::OnBnClickedButtonGo()
{
	CString str_tmp;
	str_tmp.Format(_T("====================== Run BEGIN(V%s) ==========================="), m_str_appver);
	LogMsg(str_tmp);

	/**
	* @brief The code, change Windows tile was moved from InitialProcedure
	*        for EntranceTool PostMessage to UnifiyUI.
	*        
	*/
	CString str_title;	
	if (m_st_idParameter.b_tsFlag)
	{
		str_title.Format(_T("UnifyUI_TS_%s"), m_st_idParameter.str_toolVer);
	}
	else
	{
		str_title.Format(_T("UnifyUI_%s"), m_st_idParameter.str_toolVer);
	}

	SetWindowText(str_title);
	//=======================//

	/* Run flag */
	m_b_isTesting = true;
	m_i_cameraNo = 0;

	/* Begin */
	if (!Begin())
	{
		KillTimer(1);
		ResetUIForNextRun();
		return;
	}

	/* Run thread */
	CWinThread* pCWinThread = AfxBeginThread(RunThread, (void*)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	if (pCWinThread == NULL)
	{
		return;
	}
}

void CUnifyUI_FacTestToolDlg::OnBnClickedButtonStop()
{
	int i_pageIdx = 0;
	bool b_res = true;

	i_pageIdx = m_tab_stationPageCtrl.GetCurSel();
	if (m_pageArray[i_pageIdx] == NULL)
	{
		b_res = false;
	}
	else
	{
		b_res = m_pageArray.GetAt(i_pageIdx)->Stop();
	}

	if (b_res)
	{
		ResetUIForNextRun();
	} 
	else
	{
		AfxMessageBox(_T("无法停止运行!\nCan not stop running!"), MB_OK);
	}

}

void CUnifyUI_FacTestToolDlg::OnBnClickedButtonExit()
{
	OnCancel();
}

/* Thread function for running station page test items */
UINT CUnifyUI_FacTestToolDlg::RunThread(LPVOID pParam)
{
	CUnifyUI_FacTestToolDlg* pThis = (CUnifyUI_FacTestToolDlg*)pParam;

	CSingleLock singleLock(&(pThis->m_obj_run), TRUE);

	st_pageReturn st_return;

	st_return.i_slot = 0;

	//kill msswchx.exe avoid comport occupy
	pThis->ExeCmd(_T("taskkill /IM msswchx.exe /F"),5000);
	
	/* Power OFF All powersupply */
	for (int i=0;i< PS_NUMBER;i++)
	{
		if (pThis->m_st_uiControl.i_PowerSupply[i] & POWERON)
		{
			if (!pThis->SetPowerSupply(1,i+1,1e-6,pThis->m_st_uiControl.lf_Current[i]))
			{
				st_return.str_errorMsg = _T("SetPowerSupply OFF Fail");
				st_return.str_errorcode = CommErr_UI_PowerSupply_Off_Fail;
				pThis->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&st_return);
				pThis->LogMsg(_T("MainUI Run() SetPowerSupply OFF Fail!"));
				//pThis->ShowResultMsgBox(st_return.str_errorcode, st_return.str_errorMsg);
				return 0;
			}
			pThis->LogMsg(_T("MainUI Run() SetPowerSupply OFF Suceess!"));
		}	
	}
	
	/* Device In */
	if (pThis->m_st_uiControl.i_DevControl)
	{
		
		if (!pThis->SetDeviceIn())
		{
			st_return.str_errorMsg = pThis->m_str_errorMsg;
			st_return.str_errorcode = pThis->m_str_errorCode;
			pThis->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&st_return);
			pThis->LogMsg(_T("MainUI Run() SetDeviceIn Fail!"));
			return 0;
		}
		pThis->LogMsg(_T("MainUI Run() SetDeviceIn Suceess!"));
	}

	/* Power ON step-1 to ShutVol */
	for (int i=0;i< PS_NUMBER;i++)
	{
		if (pThis->m_st_uiControl.i_PowerSupply[i] & POWERON)
		{
			if (!pThis->SetPowerSupply(1,i+1,pThis->m_st_uiControl.lf_ShutVol[i],pThis->m_st_uiControl.lf_Current[i]))
			{
				st_return.str_errorMsg = _T("SetPowerSupply ON-1 Fail");
				st_return.str_errorcode = CommErr_UI_PowerSupply_On_Fail;
				pThis->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&st_return);
				pThis->LogMsg(_T("MainUI Run() SetPowerSupply ON-1 Fail!"));
				//pThis->ShowResultMsgBox(st_return.str_errorcode, st_return.str_errorMsg);
				return 0;
			}
			pThis->LogMsg(_T("MainUI Run() SetPowerSupply ON-1 Suceess!"));
		}	
	}
	
	/* Power ON step-2 to Voltage */
	for(int i=0; i<PS_NUMBER; i++)
	{
		if (pThis->m_st_uiControl.i_PowerSupply[i] & POWERON)
		{
			Sleep(pThis->m_st_uiControl.i_PowerOnDelay[i]);

			if (!pThis->SetPowerSupply(1,i+1,pThis->m_st_uiControl.lf_Voltage[i],pThis->m_st_uiControl.lf_Current[i]))
			{
				st_return.str_errorMsg = pThis->m_str_errorMsg;
				st_return.str_errorcode = pThis->m_str_errorCode;
				pThis->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&st_return);
				pThis->LogMsg(_T("MainUI Run() SetPowerSupply ON Fail!"));
				//pThis->ShowResultMsgBox(st_return.str_errorcode,st_return.str_errorMsg);
				return 0;
			}
			Sleep(pThis->m_st_uiControl.i_PowerOnDelay[i]);
			pThis->LogMsg(_T("MainUI Run() SetPowerSupply ON Suceess!"));
		}
	}
	

	/* Get Picasso from UI Station page(RD and CIM must Call) */
	if (!pThis->GetPicassoFromStationPage())
	{
		return 0;
	}

	if (!pThis->GetTagFromStationPage())
	{
		return 0;
	}

	if (!pThis->GetSnFromStationPage())
	{
		return 0;
	}

	pThis->LogMsg(_T("MainUI Run() GetPicasso From Station Page Suceess!"));


#ifdef FACTORY_MODE
	/* Check Picasso rule(TS need not to check) */
	if (!(pThis->m_st_idParameter.b_tsFlag))
	{
		if (!pThis->CheckPicassoRule() 
			&& (pThis->m_st_idParameter.str_station != STATION_PREDL) 
			&& (pThis->m_st_idParameter.str_station != STATION_OS_DL)
			&& (pThis->m_st_idParameter.str_station != STATION_DL) 
			&& (pThis->m_st_idParameter.str_station != STATION_REDL)
			&& !((pThis->m_st_idParameter.str_station == STATION_CAMERA || pThis->m_st_idParameter.str_station == STATION_LTE || pThis->m_st_idParameter.str_station == STATION_2G3GTEST) && pThis->m_st_uiControl.b_Multi)
			)
		{   //2012.08.20 
			st_return.str_errorMsg = _T("Picasso invalide!\nPicasso无效！");
			st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
			pThis->PostRun();
			pThis->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&st_return);
			pThis->LogMsg(_T("MainUI Run() CheckPicassoRule Fail!"));
			return 0;
		}
	}
	pThis->LogMsg(_T("MainUI Run() CheckPicassoRule Suceess!"));


	/*  Check Station Routing(TS need not to check) */
	if (!(pThis->m_st_idParameter.b_tsFlag) && (pThis->m_st_idParameter.str_station != STATION_BIST) && (pThis->m_st_idParameter.str_station != STATION_REDL))
	{   //2012.08.20 
		if (!pThis->CheckStationRouting()
			&& (pThis->m_st_idParameter.str_station != STATION_PREDL)
			&& (pThis->m_st_idParameter.str_station != STATION_OS_DL)
			&& (pThis->m_st_idParameter.str_station != STATION_DL)
			&& !((pThis->m_st_idParameter.str_station == STATION_CAMERA || pThis->m_st_idParameter.str_station == STATION_LTE || pThis->m_st_idParameter.str_station == STATION_2G3GTEST) && pThis->m_st_uiControl.b_Multi)
			)
		{
			if (pThis->m_str_errorCode == _T(""))
			{
				pThis->m_str_errorCode = CommErr_UI_CIM_Routing_Check_Fail;
			}

			if (pThis->m_str_errorMsg == _T(""))
			{
				pThis->m_str_errorMsg = _T("检查站别例行消息失败!\nCheck station routing Fail!");
			}			

			st_return.str_errorMsg = pThis->m_str_errorMsg;
			st_return.str_errorcode = pThis->m_str_errorCode;
			pThis->PostRun();
			pThis->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&st_return);
			pThis->LogMsg(_T("MainUI Run() CheckStationRouting Fail!"));
			return 0;
		}
	}
	pThis->LogMsg(_T("MainUI Run()CheckStationRouting Suceess!"));

#endif

	/* Page Run */
	for (int i_pageIdx = 0; i_pageIdx < pThis->m_pageArray.GetSize(); i_pageIdx++) 
	{
		if (pThis->m_pageArray[i_pageIdx] == NULL) 
		{
			continue;
		}

		if (i_pageIdx == pThis->m_tab_stationPageCtrl.GetCurSel()) 
		{
			pThis->m_pageArray.GetAt(i_pageIdx)->Run();
		}
	}

	return 0;
}

/* Get ID from UI Edit */
bool CUnifyUI_FacTestToolDlg::GetIDFromUI()
{
	m_edit_model.GetWindowText(m_st_idParameter.str_modelName);
	m_edit_line.GetWindowText(m_st_idParameter.str_line);
	m_edit_so.GetWindowText(m_st_idParameter.str_so);
	
	//m_edit_pcba.GetWindowText(m_st_idParameter.str_pcba);
	m_edit_so.GetWindowText(m_st_idParameter.str_so);
	m_edit_partNumber.GetWindowText(m_st_idParameter.str_partNumber);
	m_edit_toolVer.GetWindowText(m_st_idParameter.str_toolVer);
	m_edit_employee.GetWindowText(m_st_idParameter.str_employee);
	m_edit_factoryVer.GetWindowText(m_st_idParameter.str_factoryVer);
	m_edit_userLoadVer.GetWindowText(m_st_idParameter.str_userLoadVer);

	return true;
}

/* Get Picasso Number from Station page function */
bool CUnifyUI_FacTestToolDlg::GetPicassoFromStationPage()
{
	m_map_picasso.clear();

	for (int i_pageIdx = 0; i_pageIdx < m_pageArray.GetSize(); i_pageIdx++) 
	{
		if (m_pageArray[i_pageIdx] == NULL) 
		{
			continue;
		}

		if (i_pageIdx == m_tab_stationPageCtrl.GetCurSel()) 
		{
			return m_pageArray.GetAt(i_pageIdx)->GetPicasso(m_map_picasso);
		}
	}

	return false;
}

/* Get Picasso Number from Station page function */
bool CUnifyUI_FacTestToolDlg::GetTagFromStationPage()
{
	m_map_tag.clear();

	for (int i_pageIdx = 0; i_pageIdx < m_pageArray.GetSize(); i_pageIdx++) 
	{
		if (m_pageArray[i_pageIdx] == NULL) 
		{
			continue;
		}

		if (i_pageIdx == m_tab_stationPageCtrl.GetCurSel()) 
		{
			return m_pageArray.GetAt(i_pageIdx)->GetTag(m_map_tag);
		}
	}

	return false;
}

bool CUnifyUI_FacTestToolDlg::GetSnFromStationPage()
{
	m_map_sn.clear();

	for (int i_pageIdx = 0; i_pageIdx < m_pageArray.GetSize(); i_pageIdx++) 
	{
		if (m_pageArray[i_pageIdx] == NULL) 
		{
			continue;
		}

		if (i_pageIdx == m_tab_stationPageCtrl.GetCurSel()) 
		{
			return m_pageArray.GetAt(i_pageIdx)->GetSn(m_map_sn);
		}
	}

	return false;
}


/* Check Station routing function */
bool CUnifyUI_FacTestToolDlg::CheckStationRouting()
{
	bool b_res = false;
	int i_pageIdx = 0;
	char sz_picasso[15];
	char sz_model[20]={0};
	CStringToChar(m_st_idParameter.str_modelName,sz_model);

	std::map<int,CString>::iterator Itr;
	for (Itr = m_map_picasso.begin(); Itr != m_map_picasso.end(); Itr++)
	{
		memset(sz_picasso, 0, sizeof(sz_picasso));
		CStringToChar(Itr->second,sz_picasso);

		if (m_map_invalidPicasso.find(Itr->first) != m_map_invalidPicasso.end())
		{
			continue;
		}
		/* Check CIM routing */ 	
		if (!QueryStationRoutingInfo(sz_picasso))
		{
			/*save invalid Picasso */
			m_map_invalidPicasso[Itr->first] = Itr->second + _T(",") + m_str_errorCode;
		}
		else
		{
			b_res = true;
		}
	}

	i_pageIdx = m_tab_stationPageCtrl.GetCurSel();
	if (m_pageArray[i_pageIdx] == NULL)
	{
		b_res = false;
	}
	else
	{
		m_pageArray.GetAt(i_pageIdx)->SetPicasso(m_map_invalidPicasso);
	}

	return b_res;
}

/* Result counter functions */
bool CUnifyUI_FacTestToolDlg::SetTestRecord(bool b_pass, bool b_CIM)
{
	bool b_insertFlag = true;

#ifdef RD_MODE
	++m_i_totalNo;
	if (b_pass)
	{
		++m_i_passNo;
	}
	else
	{
		++m_i_failNo;
	}
#else
	
	if (!m_st_idParameter.b_tsFlag)
	{	
		if (b_CIM)
		{
			b_insertFlag = InsertTestRecordToCIM(b_pass);
		}		
	}
	else
	{
		if (b_pass && (m_st_idParameter.str_station == STATION_OS_DL))
		{
			char sz_id[15]={0};
			char sz_model[20]={0};
			char sz_computer[30]={0};
			char sz_ngprocess[30]={0};
			char sz_rtnprocess[30]={0};
			CStringToChar(m_map_picasso[m_st_result.i_slot],sz_id);
			CStringToChar(m_st_idParameter.str_modelName,sz_model);
			CStringToChar(STATION_OS_DL,sz_ngprocess);
			CStringToChar(STATION_MMI_BB,sz_rtnprocess);
			CStringToChar(m_str_computerName,sz_computer);

			b_insertFlag = m_obj_QueryCIM.InsertTS_DataToCIM(sz_id,sz_model,sz_rtnprocess,sz_ngprocess,sz_computer);		
		}

		++m_i_totalNo;
		if (b_pass)
		{
			++m_i_passNo;
		}
		else
		{
			++m_i_failNo;
		}
	}
#endif

	return b_insertFlag;
}

bool CUnifyUI_FacTestToolDlg::InsertTestRecordToCIM(bool b_pass)
{
	CString strTemp;
	CString str_error = m_st_result.str_errorcode.Left(2);
	if ((str_error == _T("DB") ) ||(str_error == _T("UI")) || (m_str_InsertCIM == _T("N")))
	{
		return true;
	}

	//set cycle time to pics data
	char sz_cycle[20]={0};
	SetPICSData(m_st_result.i_slot,"cycletime",itoa(m_i_timeCount,sz_cycle,10));

	m_obj_QueryCIM.SetPicassoMap(m_map_picasso);
	m_obj_QueryCIM.SetslotPICSData(m_map_slotPICSData);
	m_obj_QueryCIM.SetSTResult(m_st_result);
	m_obj_QueryCIM.SetidParameter(m_st_idParameter);

	if (!m_obj_QueryCIM.InsertTestRecordToCIM(b_pass))
	{		
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		strTemp.Format(_T("[%d] Insert data fail:%s == %s"),m_st_result.i_slot,m_str_errorCode,m_str_errorMsg);
		LogMsg(strTemp);
		return false;
	}
	
	return true;
}

/* Reset UI for another test run */
void CUnifyUI_FacTestToolDlg::ResetUIForNextRun()
{
	int i_pageIdx = 0;
	
	i_pageIdx = m_tab_stationPageCtrl.GetCurSel();
	if (m_pageArray[i_pageIdx] != NULL)
	{
		m_pageArray.GetAt(i_pageIdx)->ResetPage();
	}

	EnableDisableButton(true);

#ifdef RD_MODE
	EnableDisableEdit(false);
#endif

	InitialResultCounter();
}

bool CUnifyUI_FacTestToolDlg::SetTestResultToStation( int i_slot, CString str_result )
{
	int i_pageIdx = 0;

	i_pageIdx = m_tab_stationPageCtrl.GetCurSel();
	if (m_pageArray[i_pageIdx] != NULL)
	{
		m_pageArray.GetAt(i_pageIdx)->SetTestResult(i_slot,str_result);
	}

	return true;
}

bool CUnifyUI_FacTestToolDlg::PostRun( )
{
	int i_pageIdx = 0;

	i_pageIdx = m_tab_stationPageCtrl.GetCurSel();
	if (m_pageArray[i_pageIdx] != NULL)
	{
		m_pageArray.GetAt(i_pageIdx)->PostRun();
	}

	return true;
}

/* Result message box function */
bool CUnifyUI_FacTestToolDlg::ShowResultMsgBox(CString str_errorCode, CString str_message)
{
	CString str_tmp = _T("");
	CString str_errmsg = str_message;
	str_errmsg.Replace(_T("\n"),_T(" "));
	str_errmsg.Replace(_T("\r"),_T(" "));
	str_tmp.Format(_T("[%d]: (%s),(%s),(%s)"),m_st_result.i_slot,m_map_picasso[m_st_result.i_slot],str_errorCode,str_message);
	LogMsg(str_tmp);

	m_dlg_resultDlg.SetText(str_errorCode, str_message);
	//m_dlg_resultDlg.SetTextSize(20);
	m_dlg_resultDlg.DoModal();

	return true;
}

/* Get/Set functions between main UI and station pages */
bool CUnifyUI_FacTestToolDlg::SetUIIDParameter(st_UIIDparameter st_idParameter)
{
	m_st_idParameter.i_parameterNum = st_idParameter.i_parameterNum;
	m_st_idParameter.b_tsFlag = st_idParameter.b_tsFlag;
	m_st_idParameter.str_firstProcess.Format(_T("%s"), st_idParameter.str_firstProcess);
	m_st_idParameter.str_station.Format(_T("%s"), st_idParameter.str_station);
	m_st_idParameter.str_modelName.Format(_T("%s"), st_idParameter.str_modelName);
	m_st_idParameter.str_line.Format(_T("%s"), st_idParameter.str_line);
	m_st_idParameter.str_pcba.Format(_T("%s"), st_idParameter.str_daynight);
	//m_st_idParameter.str_pcba.Format(_T("%s"), st_idParameter.str_pcba);
	m_st_idParameter.str_so.Format(_T("%s"), st_idParameter.str_so);
	m_st_idParameter.str_partNumber.Format(_T("%s"), st_idParameter.str_partNumber);
	m_st_idParameter.str_toolVer.Format(_T("%s"), st_idParameter.str_toolVer);
	m_st_idParameter.str_employee.Format(_T("%s"), st_idParameter.str_employee);
	m_st_idParameter.str_factoryVer.Format(_T("%s"), st_idParameter.str_factoryVer);
	m_st_idParameter.str_userLoadVer.Format(_T("%s"), st_idParameter.str_userLoadVer);
	m_st_idParameter.str_imagePath.Format(_T("%s"), st_idParameter.str_imagePath);
	m_st_idParameter.str_segment.Format(_T("%s"), st_idParameter.str_segment);
	m_st_idParameter.str_soType.Format(_T("%s"), st_idParameter.str_soType);
	m_st_idParameter.str_CRType.Format(_T("%s"), st_idParameter.str_CRType);
	m_st_idParameter.str_Reserve1.Format(_T("%s"), st_idParameter.str_Reserve1);
	m_st_idParameter.str_Reserve2.Format(_T("%s"), st_idParameter.str_Reserve2);

	//Add by magan
	m_st_idParameter.str_idtype.Format(_T("%s"), st_idParameter.str_idtype);
	m_st_idParameter.i_idLength = st_idParameter.i_idLength;

	//Add by karen
	m_st_idParameter.str_accessoryList.Format(_T("%s"), st_idParameter.str_accessoryList);
	//m_st_idParameter.b_OpenbyEntranceTool = st_idParameter.b_OpenbyEntranceTool;
	return true;
}

bool CUnifyUI_FacTestToolDlg::GetUIIDParameter(st_UIIDparameter &st_idParameter)
{
	st_idParameter.i_parameterNum = m_st_idParameter.i_parameterNum;
	st_idParameter.b_tsFlag = m_st_idParameter.b_tsFlag;
	st_idParameter.str_modelName.Format(_T("%s"), m_st_idParameter.str_modelName);
	st_idParameter.str_firstProcess.Format(_T("%s"), m_st_idParameter.str_firstProcess);	
	st_idParameter.str_station.Format(_T("%s"), m_st_idParameter.str_station);
	st_idParameter.str_line.Format(_T("%s"), m_st_idParameter.str_line);//lion
	st_idParameter.str_daynight.Format(_T("%s"), m_st_idParameter.str_daynight);//lion
//	st_idParameter.str_pcba.Format(_T("%s"), m_st_idParameter.str_pcba);
	st_idParameter.str_so.Format(_T("%s"), m_st_idParameter.str_so);
	st_idParameter.str_partNumber.Format(_T("%s"), m_st_idParameter.str_partNumber);
	st_idParameter.str_toolVer.Format(_T("%s"), m_st_idParameter.str_toolVer);
	st_idParameter.str_employee.Format(_T("%s"), m_st_idParameter.str_employee);
	st_idParameter.str_factoryVer.Format(_T("%s"), m_st_idParameter.str_factoryVer);
	st_idParameter.str_userLoadVer.Format(_T("%s"), m_st_idParameter.str_userLoadVer);
	st_idParameter.str_imagePath.Format(_T("%s"), m_st_idParameter.str_imagePath);
	st_idParameter.str_soType.Format(_T("%s"), m_st_idParameter.str_soType);
	st_idParameter.str_segment.Format(_T("%s"), m_st_idParameter.str_segment);
	st_idParameter.str_CRType.Format(_T("%s"), m_st_idParameter.str_CRType);
	st_idParameter.str_Reserve1.Format(_T("%s"), m_st_idParameter.str_Reserve1);
	st_idParameter.str_Reserve2.Format(_T("%s"), m_st_idParameter.str_Reserve2);
	// Add by magan
	st_idParameter.str_idtype.Format(_T("%s"), m_st_idParameter.str_idtype);
	st_idParameter.i_idLength = m_st_idParameter.i_idLength;

	// Add by karen
	st_idParameter.str_accessoryList.Format(_T("%s"), m_st_idParameter.str_accessoryList);
	st_idParameter.b_OpenbyEntranceTool = m_st_idParameter.b_OpenbyEntranceTool;
	return true;
}

bool CUnifyUI_FacTestToolDlg::GetUIControlParameter( st_UIControl &st_uiControl )
{
	st_uiControl.b_ScanPicasso = m_st_uiControl.b_ScanPicasso;
	st_uiControl.b_AutoRun = m_st_uiControl.b_AutoRun;
	st_uiControl.b_AutoRunPreScan = m_st_uiControl.b_AutoRunPreScan;
	st_uiControl.b_PreScan = m_st_uiControl.b_PreScan;
	st_uiControl.b_NAL = m_st_uiControl.b_NAL;
	st_uiControl.b_VKEnter = m_st_uiControl.b_VKEnter;
	st_uiControl.b_ACER22 = m_st_uiControl.b_ACER22;
	st_uiControl.nIdType = m_st_uiControl.nIdType;
	st_uiControl.b_WriteTagFrame = m_st_uiControl.b_WriteTagFrame;
//	st_uiControl.b_WriteTag = m_st_uiControl.b_WriteTag;
//	st_uiControl.b_ReadTag = m_st_uiControl.b_ReadTag;
//	st_uiControl.b_ScanTag = m_st_uiControl.b_ScanTag;

	st_uiControl.b_WriteSnFrame = m_st_uiControl.b_WriteSnFrame;
	st_uiControl.b_WriteSn = m_st_uiControl.b_WriteSn;
	st_uiControl.b_ReadSn = m_st_uiControl.b_ReadSn;
	st_uiControl.b_ScanSn = m_st_uiControl.b_ScanSn;
	st_uiControl.i_ToolTimeout = m_st_uiControl.i_ToolTimeout;

	st_uiControl.i_DevControl = m_st_uiControl.i_DevControl;
	st_uiControl.str_DevPort.Format(_T("%s"),m_st_uiControl.str_DevPort);
	st_uiControl.i_DeviceInDelay = m_st_uiControl.i_DeviceInDelay;
	st_uiControl.str_DevInCommand.Format(_T("%s"),m_st_uiControl.str_DevInCommand);
	st_uiControl.str_DevOutCommand.Format(_T("%s"),m_st_uiControl.str_DevOutCommand);

	st_uiControl.b_ReadFA = m_st_uiControl.b_ReadFA;
	st_uiControl.b_WriteFA = m_st_uiControl.b_WriteFA;
	
	for (int i=0;i<PS_NUMBER ;i++)
	{
		st_uiControl.i_PowerSupply[i] = m_st_uiControl.i_PowerSupply[i];
		st_uiControl.i_PowerOnDelay[i] = m_st_uiControl.i_PowerOnDelay[i];
		st_uiControl.lf_Voltage[i] = m_st_uiControl.lf_Voltage[i];
		st_uiControl.lf_Current[i] = m_st_uiControl.lf_Current[i];
		st_uiControl.i_PSAddr[i] = m_st_uiControl.i_PSAddr[i];
		st_uiControl.i_PSInterface[i] = m_st_uiControl.i_PSInterface[i];
		st_uiControl.lf_ShutVol[i] = m_st_uiControl.lf_ShutVol[i];
	}
	
	st_uiControl.i_FAArea = m_st_uiControl.i_FAArea;
	st_uiControl.str_NAL6 = m_st_uiControl.str_NAL6;
	st_uiControl.b_SQN = m_st_uiControl.b_SQN;
	st_uiControl.b_RelayBoard = m_st_uiControl.b_RelayBoard;

	st_uiControl.str_Device1 = m_st_uiControl.str_Device1;
    st_uiControl.str_Device2 = m_st_uiControl.str_Device2;
	st_uiControl.str_Device3 = m_st_uiControl.str_Device3;
	st_uiControl.str_Device4 = m_st_uiControl.str_Device4;
	return true;
}


/* set CIM PICS data */
bool CUnifyUI_FacTestToolDlg::SetPICSData(int i_slot, char* sz_key, char* sz_value)
{
	CString str_key;
	CString str_value;

	str_key = ConvertString(sz_key);
	str_value = ConvertString(sz_value);

	m_map_slotPICSData[i_slot][str_key] = str_value;

	if(str_key == _T("IMEIUSED"))
	{
		m_str_InsertCIM = str_value ;
	}
	return true;
}

void CUnifyUI_FacTestToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CString str_temp;

	switch(nIDEvent)
	{
	case 1:
		{
			//if(m_st_idParameter.str_station == STATION_OS_DL && m_st_uiControl.b_AutoRun)
			//{
			//}
			//else
			//{
				str_temp.Format(_T("%d"), ++m_i_timeCount);
				if ( m_i_timeCount == m_st_uiControl.i_ToolTimeout  ){
					CString cs;
					cs.Format(_T("系统超时请重启软件，TIME OUT please reopen , Time= %d"),  m_i_timeCount);
					//AfxMessageBox( cs.GetBuffer(0));
					::MessageBox(NULL, cs.GetBuffer(0), _T("Warnning!!"), MB_TASKMODAL|MB_TOPMOST);
					cs.ReleaseBuffer();
				}
				m_edit_time.SetWindowText(str_temp);
				
	//		}
			break;
		}
	case 2:
		{
			if (m_b_syntime)
			{
				SynTime();
			}			
			break;
		}
	case 3:
		{
			if(m_i_testtime != 0)
			{
				if(!m_b_isTesting)
				{
					m_i_testtime--;
					OnBnClickedButtonGo();		
				}	
			}
			else
				KillTimer(3);
		}
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CUnifyUI_FacTestToolDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
		case WM_EntranceToolToUnifyUI:
			if(pMsg->lParam==1)
			{
				m_st_idParameter.b_OpenbyEntranceTool = true;
				LogMsg(_T("CUnifyUI_FacTestToolDlg b_OpenbyEntranceTool = true"));
				ChangeBG();
			}
			break;
		case WM_KEYDOWN:
			switch (pMsg->wParam)
			{
				case VK_ESCAPE:
					return true;
					break;
				case VK_RETURN:
					if (!m_b_isTesting)
					{
						if (m_st_uiControl.b_VKEnter)
						{
							//if (GetDlgItem(IDC_BUTTON_GO)->IsWindowEnabled())
							{
								OnBnClickedButtonGo();
							}
							
						}
					}
					return true;
					break;
				default:
					break;
			}
		default:
			break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

bool CUnifyUI_FacTestToolDlg::GetStationListMap( std::map<CString, CString> &map_StationVsIndex )
{
	if (m_map_StationIndexMap.empty())
	{
		return false;
	}

	std::map<CString,CString>::iterator Itr;
	for (Itr = m_map_StationIndexMap.begin();Itr!= m_map_StationIndexMap.end();Itr++)
	{
		map_StationVsIndex[Itr->first] = Itr->second;
	}

	return true;
}


bool CUnifyUI_FacTestToolDlg::QueryStationRoutingInfo(char *p_sz_picasso )
{

	//m_obj_QueryCIM.SetidParameter(m_st_idParameter);

	if (!m_obj_QueryCIM.QueryStationRoutingInfoFromCIM(p_sz_picasso))
	{	
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();	
		LogMsg(m_str_errorMsg);
		return false;
	}

	return true;
}

bool CUnifyUI_FacTestToolDlg::CheckPicassoStat( char * sz_picasso ,char *sz_TimeInfo, char *sz_status )
{
	if (!m_obj_QueryCIM.CheckPicassoStatusFromCIM(sz_picasso,sz_TimeInfo,sz_status))
	{
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		LogMsg(m_str_errorMsg);
		return false;
	}	
	return true;
}

bool CUnifyUI_FacTestToolDlg::QueryRecordInfoFromCIM()
{

	//m_obj_QueryCIM.SetidParameter(m_st_idParameter);
	//m_obj_QueryCIM.SetComputerName(m_str_computerName);
	if (m_obj_QueryCIM.QueryRecordInfoFromCIM())
	{
		m_i_totalNo = m_obj_QueryCIM.GetTotalNo();
		m_i_failNo = m_obj_QueryCIM.GetFailNo();
		m_i_passNo = m_obj_QueryCIM.GetPassNo();
		return true;	
	}

	m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
	m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
	LogMsg(m_str_errorMsg);

	return false;

}

bool CUnifyUI_FacTestToolDlg::InsertPicassoControlToCIM( char *sz_picasso, char *sz_mobileid )
{
	char sz_status[2] = {'N'};

	if (!m_obj_QueryCIM.InsertPicassoControlToCIM(sz_picasso,sz_mobileid,sz_status))
	{
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		LogMsg(m_str_errorMsg);
		return false;
	}
	return true;
}

bool CUnifyUI_FacTestToolDlg::CheckPicassoDuplicateFromCIM( char *sz_Picasso, char *sz_timeinfoHex )
{
	if (!m_obj_QueryCIM.CheckPicassoDuplicateFromCIM(sz_Picasso,sz_timeinfoHex))
	{
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		LogMsg(m_str_errorMsg);
		return false;
	}
	return true;

}

bool CUnifyUI_FacTestToolDlg::QueryIMEIresultFromCIM( char *sz_imei, char *modelName, char *sz_Picasso )
{
	if (!m_obj_QueryCIM.QueryIMEIresultFromCIM(sz_imei, modelName, sz_Picasso))
	{
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		LogMsg(m_str_errorMsg);
		return false;		
	}

	return true;	
}

bool CUnifyUI_FacTestToolDlg::QueryIMEIStatusFromCIM( char *sz_imei, char *sz_status )
{
	if (!m_obj_QueryCIM.QueryIMEIStatusFromCIM(sz_imei, sz_status))
	{
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		LogMsg(m_str_errorMsg);
		return false;		
	}

	return true;	
}

bool CUnifyUI_FacTestToolDlg::QueryDataFromCIM()
{
	m_obj_QueryCIM.SetidParameter(m_st_idParameter);

	if(m_obj_QueryCIM.QueryDataFromCIM())
	{		
		m_obj_QueryCIM.GetUIIDParameter(m_st_idParameter);
		m_obj_QueryCIM.GetStationListMap(m_map_StationIndexMap);	
		m_i_failNo = m_obj_QueryCIM.GetFailNo();
		m_i_passNo = m_obj_QueryCIM.GetPassNo(); 
		m_i_totalNo = m_obj_QueryCIM.GetTotalNo();
		m_str_computerName = m_obj_QueryCIM.GetComputerName();
	}
	else
	{
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		LogMsg(m_str_errorMsg);
		return false;
	}

	return true;
}


bool CUnifyUI_FacTestToolDlg::QueryDataFromSQN()
{
	LogMsg(_T("Factory mode and SQN=1, get information from PVCS!"));

	m_obj_QueryCIM.SetidParameter(m_st_idParameter);
	if (m_obj_QueryCIM.QueryDataFromSQN())
	{
		m_obj_QueryCIM.GetTestKeyValue(m_map_TestItem);
		m_obj_QueryCIM.GetUIIDParameter(m_st_idParameter);
	}
	else
	{
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		LogMsg(m_str_errorMsg);
		return false;
	}

	return true;
}

bool CUnifyUI_FacTestToolDlg::CheckPicassoRule()
{
	bool b_res = false;
	int i_pageIdx = 0;

	bool b_invalide = false;
	USES_CONVERSION;

	std::map<int,CString>::iterator Itr,ItrItr;
	m_map_invalidPicasso.clear();

	for (Itr = m_map_picasso.begin(); Itr != m_map_picasso.end(); Itr++)
	{
		b_invalide = false;
		if (!Check_Picasso_Rule_All(Itr->second))
		{
			/*save invalid Picasso */
			m_map_invalidPicasso[Itr->first] =  Itr->second + _T(",") + CommErr_UI_Picasso_Invalide;
		
			b_invalide = true;
		}
		else 
		{
			ItrItr = Itr;
			for (ItrItr++; ItrItr != m_map_picasso.end(); ItrItr++)
			{		
				if (ItrItr->second == Itr->second)
				{
					m_map_invalidPicasso[Itr->first] = Itr->second + _T(",") + CommErr_UI_Picasso_Duplicated;
					m_map_invalidPicasso[ItrItr->first] = Itr->second + _T(",") + CommErr_UI_Picasso_Duplicated;
					b_invalide = true;
				}
			}
		}
		if (!b_invalide)
		{
			b_res = true;
		}
	}

#ifdef RD_MODE
	if ((m_st_idParameter.str_station == STATION_PREDL)||(m_st_idParameter.str_station == STATION_DL))
	{
		i_pageIdx = m_tab_stationPageCtrl.GetCurSel();
		if (m_pageArray[i_pageIdx] == NULL)
		{
			b_res = false;
		}
		else
		{
			m_pageArray.GetAt(i_pageIdx)->SetPicasso(m_map_invalidPicasso);
		}
	}	

	return true;
#endif

	return b_res;
}

void CUnifyUI_FacTestToolDlg::RecordResultToFile( CString str_picasso, CString str_errorCode, CString str_errorMsg )
{
	//store result to csv file
	CTime time = CTime::GetCurrentTime();
	
	//open first or day changed
	if ((m_str_ResultCsvPath == _T("")) || (time.GetDay() != m_i_today))
	{
		/* Get current app directory */		
		m_i_today = time.GetDay();
		m_str_ResultCsvPath.Format(_T("%s\\%s_%s.csv"), LOG_PATH,m_st_idParameter.str_station,time.Format(_T("%Y%m%d")));
	}

	try
	{
		CStdioFile file(m_str_ResultCsvPath,CFile::modeNoTruncate|CFile::modeWrite|CFile::modeCreate);
		CString str_result = _T("");
		str_result.Format(_T("%s,%s,%s,%s\n"),time.Format(_T("%H:%M:%S")),str_picasso,str_errorCode,str_errorMsg);
		file.SeekToEnd();
		file.WriteString(str_result); 
		file.Close();
	}
	catch(CFileException* pe)
	{
		TCHAR   szCause[1024];
		pe->GetErrorMessage(szCause, 1024);

		CString str_Reason = CString("Cannot open ") + m_str_ResultCsvPath + CString(" cause = ") + CString(szCause);
		LogMsg (str_Reason);
		pe->Delete();
	}
}

CString CUnifyUI_FacTestToolDlg::getErrCode()
{
	return m_str_errorCode;
}

CString CUnifyUI_FacTestToolDlg::getErrMsg()
{
	return m_str_errorMsg;
}

// Device In
bool CUnifyUI_FacTestToolDlg::SetDeviceIn()
{
	if (m_st_uiControl.i_DevControl & DEVIN)
	{
		int i_commandLen = m_st_uiControl.str_DevInCommand.GetLength();
		BYTE b_command[50] = {0};
		CStringToChar(m_st_uiControl.str_DevInCommand,(char*)b_command);

		if (!SendCommand(b_command,i_commandLen))
		{
			m_str_errorCode = m_obj_Communicate.getErrCode();
			m_str_errorMsg = m_obj_Communicate.getErrMsg();
			LogMsg(m_str_errorMsg);
			return false;
		}

		/* Wait device in */
		Sleep(m_st_uiControl.i_DeviceInDelay);
	}

	return true;
}

// Device Out
bool CUnifyUI_FacTestToolDlg::SetDeviceOut()
{

	int i_commandLen = m_st_uiControl.str_DevOutCommand.GetLength();
	BYTE b_command[50] = {0};
	CStringToChar(m_st_uiControl.str_DevOutCommand,(char*)b_command);
	if (!SendCommand(b_command,i_commandLen))
	{
		return false;
	}

	return true;
}

bool CUnifyUI_FacTestToolDlg::SendCommand( const BYTE* b_command, DWORD dw_writeLen )
{
	return m_obj_Communicate.SendCommand(b_command,dw_writeLen);
}


bool CUnifyUI_FacTestToolDlg::SetPowerSupply( int i_slot, int i_seq, double dw_vol, double dw_current )
{
	CString str_msg;
	if (!m_obj_ps[i_seq-1].IsInit())
	{
		if (!m_obj_ps[i_seq-1].InitPowerSupply(m_st_uiControl.i_PSInterface[i_seq-1],m_st_uiControl.i_PSAddr[i_seq-1]))
		{	
			str_msg.Format(_T("Detect GPIB Device fail,Interface:%d, Address:%d"),m_st_uiControl.i_PSInterface[i_seq-1],m_st_uiControl.i_PSAddr[i_seq-1]);
			LogMsg(str_msg);
			return false;
		}
	}

	if (dw_current > 0.001)
	{
		if (!m_obj_ps[i_seq-1].SetCurrentLimit(dw_current))
		{
			str_msg.Format(_T("Fail to set power supply %d current %fA"),i_seq-1, dw_current);
			LogMsg(str_msg);
			return false;
		}
	}

	if (dw_vol > 0.001)
	{
		if (!m_obj_ps[i_seq-1].SetVoltage(dw_vol))
		{
			str_msg.Format(_T("Fail to set power supply %d voltage %fV"),i_seq-1, dw_vol);
			LogMsg(str_msg);
			return false;
		}
	}
	else
	{
		if (!m_obj_ps[i_seq-1].SetPSOnOff(false))
		{
			str_msg.Format(_T("Fail to set power supply %d OFF"),i_seq-1);
			LogMsg(str_msg);
			return false;
		}
		else
		{
			str_msg.Format(_T("Success to set power supply %d OFF"),i_seq-1);
			LogMsg(str_msg);
			return true;
		}
	}

	if (!m_obj_ps[i_seq-1].SetPSOnOff(true))
	{
		str_msg.Format(_T("Fail to set power supply %d ON"),i_seq-1);
		LogMsg(str_msg);
		return false;
	}
	
	return true;
}


void CUnifyUI_FacTestToolDlg::LogMsg(CString logContent)
{
	m_obj_log.SaveLog(logContent);
}

bool CUnifyUI_FacTestToolDlg::CheckNal( char* sz_imei, char* sz_nal )
{
	if (!m_obj_QueryCIM.CheckNALFromCIM(sz_imei,sz_nal))
	{
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		LogMsg(m_str_errorMsg);
		return false;
	}
	return true;
}


bool CUnifyUI_FacTestToolDlg::QueryNAL( char* sz_imei, char* sz_nal )
{
	if (!m_obj_QueryCIM.QueryNALFromCIM(sz_imei,sz_nal))
	{
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		LogMsg(m_str_errorMsg);
		return false;
	}
	return true;
}

bool CUnifyUI_FacTestToolDlg::QueryPicassoFromCIM( char* sz_imei, char *sz_model, char* sz_picasso )
{
	if (!m_obj_QueryCIM.QueryPicassoFromCIM(sz_imei,sz_model,sz_picasso))
	{
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		LogMsg(m_str_errorMsg);
		return false;
	}
	return true;
}

bool CUnifyUI_FacTestToolDlg::NeedReTest( char *sz_id,char *sz_model, char *sz_process )
{
	char sz_cnt[20]={0};

	if (!m_obj_QueryCIM.QueryIfTestAgainFromCIM(sz_id,sz_model,sz_process,sz_cnt))
	{
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		LogMsg(m_str_errorMsg);
		return false;
	}
	int i_count = atoi(sz_cnt);

	if (i_count == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CUnifyUI_FacTestToolDlg::GetTestKeyMap( std::map<CString, CString> &map_TestItem )
{
	std::map<CString,CString>::iterator Itr;
	for (Itr = m_map_TestItem.begin();Itr!= m_map_TestItem.end();Itr++)
	{
		map_TestItem[Itr->first] = Itr->second;
	}

	return true;
}

CString CUnifyUI_FacTestToolDlg::getAppVersion()
{
	CString   AppVersion = _T("");  

	TCHAR     sz_appName[256]={0};
	GetModuleFileName(NULL,sz_appName,sizeof(sz_appName));

	int   ResVerInfoSize;
	ResVerInfoSize = (int)GetFileVersionInfoSize(sz_appName,NULL); 

	if(ResVerInfoSize != 0 ) 
	{		 
		TCHAR*   VersionInfoPtr;

		VersionInfoPtr = new TCHAR[ResVerInfoSize];

		if(!GetFileVersionInfo(sz_appName,0,ResVerInfoSize,VersionInfoPtr)) 
		{
			delete[]   VersionInfoPtr; 
			return NULL; 
		} 

		VS_FIXEDFILEINFO* vsFileInfo = 0;
		UINT iFileInfoSize = sizeof(VS_FIXEDFILEINFO);

		if(!VerQueryValue( VersionInfoPtr, _T("\\"),(void **)&vsFileInfo,&iFileInfoSize)) 
		{ 
			delete[]   VersionInfoPtr; 
			return NULL; 
		} 

		AppVersion.Format(_T("%d.%d.%d.%d"),HIWORD(vsFileInfo->dwProductVersionMS),LOWORD(vsFileInfo->dwProductVersionMS),HIWORD(vsFileInfo->dwProductVersionLS),LOWORD(vsFileInfo->dwProductVersionLS));

		delete[]   VersionInfoPtr; 
	}
	return   AppVersion; 
}

bool CUnifyUI_FacTestToolDlg::ExeCmd(CString Command, int i_timeout)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	bool b_NeedResponse = false;
	CString str_errorMsg;

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize)) 
	{
		str_errorMsg = _T("ERROR: CreatePipe fail!");
		LogMsg(str_errorMsg);
		return false;
	}

	HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	

	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo)) 
	{
		Command.ReleaseBuffer();
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, i_timeout);

		//if timeout then exit the process
		if(TimeOutSignal == WAIT_TIMEOUT) 
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			
			str_errorMsg.Format(_T("ERROR:Excute %s timeout"),Command);
			LogMsg(str_errorMsg);
		}
		else 
		{
			isOk = true;

			if (b_NeedResponse)
			{
				DWORD bytesRead;
				char *message = new char[nPipeSize];
				memset(message, 0, sizeof(message));
				::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
				message[bytesRead] = '\0';
				LogMsg(_T("Execute command: ") + ConvertString(message));
				delete [] message;
			}
		}

	}
	else 
	{
		isOk = false;
		str_errorMsg.Format(_T("ERROR: Execute %s fail!"),Command);
		LogMsg(str_errorMsg);
	}

	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(hProcess);
	hProcess = NULL;

	return isOk;
}

bool CUnifyUI_FacTestToolDlg::QueryHold( char *sz_id)
{
	if (!m_obj_QueryCIM.QueryholdInfoFromCIM(sz_id))
	{
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		LogMsg(m_str_errorMsg);
		return false;
	}

	return true;
}

bool CUnifyUI_FacTestToolDlg::SynTime()
{
	CString str_command;
	str_command.Format(_T("net time \\\\%s /set /y"),m_str_ip);
	if (ExeCmd(str_command,2000))
	{
		return true;
	}
	else
	{
		//ShowResultMsgBox(CommErr_UI_Cmd_Time_Syn_Fail,_T("此项与测试无关，Fail to syn time with server:") + str_command);
		return false;
	}

	return true;
}

bool CUnifyUI_FacTestToolDlg::SetRelayBoard( int i_slot, int i_seq, bool b_onoff )
{
	if (!m_obj_relayBoard.IsInit())
	{
		if (!m_obj_relayBoard.InitialRB())
		{
			m_str_errorCode = CommErr_UI_RelayBoard_Init_Fail;
			m_str_errorMsg = _T("Fail to initial relay board!");
			LogMsg(m_str_errorMsg);
			return false;
		}
		LogMsg(_T("Realy board init..."));
	}

	if (i_seq == ALLSLOT)
	{
		if (!m_obj_relayBoard.SetRBAll(m_i_RBNumber,b_onoff))
		{
			m_str_errorCode = CommErr_UI_RelayBoard_Set_Fail;
			m_str_errorMsg.Format(_T("Fail to set relay board! slot=%d, Seq=%d, OnOff=%d"),i_slot,i_seq,b_onoff);
			LogMsg(m_str_errorMsg);
			return false;
		}
	}
	else
	{
		int iRealSlot = m_b_RBMask? i_seq : 5-i_seq;

		if (!m_obj_relayBoard.SetRB(m_i_RBNumber,iRealSlot,b_onoff))
		{
			m_str_errorCode = CommErr_UI_RelayBoard_Set_Fail;
			m_str_errorMsg.Format(_T("Fail to set relay board! slot=%d, Seq=%d, OnOff=%d"),i_slot,i_seq,b_onoff);
			LogMsg(m_str_errorMsg);
			return false;
		}
		else
		{
			CString str_msg;
			str_msg.Format(_T("Success to set relay board! slot=%d, Seq=%d, OnOff=%d"),i_slot,i_seq,b_onoff);
			LogMsg(str_msg);
		}	

	}
	return true;
}

void CUnifyUI_FacTestToolDlg::OnBnClickedButtonCycletest()
{
	SetTimer(3, 1000, NULL);
	if(m_i_testtime == 0)
		KillTimer(3);
}

bool CUnifyUI_FacTestToolDlg::InsertAndCheckAccessory(bool &bNeedChange)
{
	CString str_error = m_st_result.str_errorcode.Left(2);
	if ((str_error == _T("DB") ) ||(str_error == _T("UI")) || (m_str_InsertCIM == _T("N")))
	{
		return true;
	}
	if (!m_obj_QueryCIM.InsertAndCheckAccessory(bNeedChange))
	{
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		LogMsg(m_str_errorMsg);
		return false;
	}
	
	if (bNeedChange)
	{
		m_str_errorCode = m_obj_QueryCIM.GetErrorCode();
		m_str_errorMsg = m_obj_QueryCIM.GetErrorMsg();
		LogMsg(m_str_errorMsg);
		return true;
	}
	return true;
}


void CUnifyUI_FacTestToolDlg::ChangeBG()
{
	if (!m_b_ChangeGB)
	{
		m_brushDiagBk.CreateSolidBrush(BKGCOLOUR_FACTORY);

		m_static_id.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_model.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_line.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_pcba.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_so.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_partNumber.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_toolVer.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_employee.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_factoryVer.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_userLoadVer.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_record.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_total.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_pass.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_fail.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_time.SetBkColor(BKGCOLOUR_FACTORY);

		m_static_execute.SetBkColor(BKGCOLOUR_FACTORY);

		int i_pageIdx = 0;

		i_pageIdx = m_tab_stationPageCtrl.GetCurSel();
		if (m_pageArray[i_pageIdx] != NULL)
		{
			m_pageArray.GetAt(i_pageIdx)->ChangeGB();
		}
		else
			LogMsg(_T("CUnifyUI_FacTestToolDlg::ChangeBG m_pageArray[i_pageIdx] == NULL"));


		ShowWindow(SW_HIDE);
		ShowWindow(SW_SHOW);
		m_b_ChangeGB = true;
	}

}
