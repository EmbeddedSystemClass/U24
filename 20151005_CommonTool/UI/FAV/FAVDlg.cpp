// FAVDlg.cpp : implementation file
#include "stdafx.h"
#include "FAV.h"
#include "FAVDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CFAVDlg dialog

CFAVDlg::CFAVDlg(CWnd* pParent /*=NULL*/)
: CDialog(CFAVDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CFAVDlg::~CFAVDlg()
{
	m_brushDiagBk.DeleteObject();
	m_obj_QueryCIM.UpdateEmployeeID_StatusFromCIM();
	m_obj_QueryCIM.UpLoadDBAccessDLL();
}
void CFAVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
	DDX_Control(pDX, IDC_LIST_KEYPART, m_LC_KeyPartList);
	DDX_Control(pDX, IDC_COMBO_ERRORCODE, m_comboBox_Errcode);
	DDX_Control(pDX, IDC_EDIT_KEYPARTBARCODE, m_Edit_KeyPartBarcode);
	DDX_Control(pDX, IDC_EDIT_SN, m_Edit_SN);
}

BEGIN_MESSAGE_MAP(CFAVDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()	
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_PASS, &CFAVDlg::OnBnClickedButtonPass)
	ON_BN_CLICKED(IDC_BUTTON_FAIL, &CFAVDlg::OnBnClickedButtonFail)
	ON_BN_CLICKED(IDC_BUTTON_UNLOCK, &CFAVDlg::OnBnClickedButtonUnlock)
	ON_BN_CLICKED(IDC_BUTTON_LOCK, &CFAVDlg::OnBnClickedButtonLock)
	ON_BN_CLICKED(IDC_BUTTON_ReadIMEI, &CFAVDlg::OnBnClickedButtonReadIMEI)
	ON_CBN_SELENDOK(IDC_COMBO_ERRORCODE, &CFAVDlg::OnCbnSelendokComboErrorcode)	
END_MESSAGE_MAP()

// CFAVDlg message handlers

BOOL CFAVDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	do 
	{
		// 1. Load DBAccess DLL
		if (!m_obj_QueryCIM.LoadDBAccessDLL())
		{
			AfxMessageBox(_T("Load DBAceess.dll Fail!"));
			break;
		}

		// 2. Query Info from PreDB
		if (!Init_CIMInfo())
		{
			break;
		}

		// 3. Initial Dialog
		//QA mode, should initial dialog to input lot number and lot count
		if(m_st_idParameter.str_station == _T("FQC") || m_st_idParameter.str_station == _T("OOB"))
		{
			if (dlg_Init.DoModal() != IDOK)
			{
				break;
			}
		}

		// 4. Check ID type
		if(m_st_idParameter.str_station == _T("OOB") || m_st_idParameter.str_station == _T("SHIPPINGOS"))
		{
			if (m_st_idParameter.i_idLength == 0 || m_st_idParameter.str_idtype == _T(""))
			{
				AfxMessageBox(_T("PN对应的ID type或者ID Length没有定义到系统中"));
				break;
			}
		}

		// 5. Initial Result Counter from PreDB
		if (!InitialResultCounter())
		{
			break;
		}

		// 6. Parse APK test items
		if (m_st_idParameter.str_station == _T("SHIPPINGOS") && m_st_idParameter.b_isReadIMEI)
		{
			ParseApkTestItem();
		}

		// 7. Initial UI Setting
		InitialUIParameter();
		InsertErrorCodeToUI();

		// 8. Sync server time
		SynTime();

		// 9. kill-server
		KillADBServer();

		return TRUE;

	} while (0);


	CDialog::OnCancel();
	return FALSE;
}

void CFAVDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CFAVDlg::OnPaint()
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
		InitialUITitle();
	}
}

HCURSOR CFAVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFAVDlg::InitialFunctionList()
{
	GetDlgItem(IDC_BUTTON_LOCK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UNLOCK)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_RESET)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_ACCESSORY)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_READPICASSO)->EnableWindow(FALSE);
	if (m_st_idParameter.str_station == _T("FQC") || m_st_idParameter.str_station == _T("VI") || m_st_idParameter.str_station == _T("P_VI"))
	{
		/*Rule1: FQC/VI/P_VI may be get picasso from phone
		checked: read picasso form phone
		unchecked: scan picasso*/
		GetDlgItem(IDC_CHECK_READPICASSO)->EnableWindow(TRUE);
	}
	else if(m_st_idParameter.str_station == _T("OOB") )
	{
		//Rule2: Only for OOB to select
		GetDlgItem(IDC_CHECK_ACCESSORY)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_RESET)->EnableWindow(TRUE);		
	}
	else//ShippingOS/OQC/AOI
	{
		GetDlgItem(IDC_BUTTON_LOCK)->EnableWindow(FALSE);
	}
}
/*
* @brief: UI Initial
*/
void CFAVDlg::InitialUIParameter()
{
	GetDlgItem(IDC_STATIC_SO)->SetWindowText(m_st_idParameter.str_so);
	GetDlgItem(IDC_STATIC_MODEL)->SetWindowText(m_st_idParameter.str_modelName);
	GetDlgItem(IDC_EDIT_EMPLOYEE_ID)->SetWindowText(m_st_idParameter.str_employee);
	GetDlgItem(IDC_STATIC_LOT)->SetWindowText(dlg_Init.m_strLotNumber);
	GetDlgItem(IDC_STATIC_LOTCOUNT)->SetWindowText(dlg_Init.m_strLotCount);
	GetDlgItem(IDC_STATIC_PARTNUMBER)->SetWindowText(m_st_idParameter.str_partNumber);

	if (m_st_idParameter.str_CRType != _T(""))
	{
		GetDlgItem(IDC_STATIC_WORKTYPE)->SetWindowText(_T("CR"));
	}
	else
	{
		GetDlgItem(IDC_STATIC_WORKTYPE)->SetWindowText(_T("Normal"));
	}

	//Rule1: Show ship image in PLine, others show factory image
	if (m_st_idParameter.str_station == _T("OOB") || m_st_idParameter.str_station == _T("SHIPPINGOS") || m_st_idParameter.str_station == _T("P_VI"))
	{
		m_strSW = m_st_idParameter.str_userLoadVer;
	}
	else
		m_strSW = m_st_idParameter.str_factoryVer;

	GetDlgItem(IDC_STATIC_SW)->SetWindowText(m_strSW);

	if (m_st_idParameter.str_station != _T("FQC") && m_st_idParameter.str_station != _T("OOB"))
	{
		((CEdit*)GetDlgItem(IDC_EDIT_EMPLOYEE_ID))->SetPasswordChar(0);
	}

	InitialFunctionList();

	GetDlgItem(IDC_BUTTON_PASS)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FAIL)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_ERRORCODE)->EnableWindow(FALSE);
	
	m_Edit_KeyPartBarcode.EnableWindow(false);
	m_Edit_KeyPartBarcode.SetLimitText(KEYPARTBARCODE_MAXLENGTH);

	m_listResult.InsertColumn(0, _T("SerNo"), LVCFMT_CENTER, 195);
	m_listResult.InsertColumn(1, _T("Result"), LVCFMT_LEFT, 100);
	m_listResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listResult.DeleteAllItems();

	/* Initial for Acessory check */
	m_LC_KeyPartList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EDITLABELS | LVS_EX_INFOTIP);
	if(m_st_idParameter.str_station == _T("OOB"))
	{
		m_LC_KeyPartList.InsertColumn(ITEM_INDEX, _T("Item"), LVCFMT_LEFT, 39);
		m_LC_KeyPartList.InsertColumn(PN_INDEX, _T("Part N.O."), LVCFMT_LEFT,118);
		m_LC_KeyPartList.InsertColumn(DESCRIPTION_INDEX, _T("Description"), LVCFMT_LEFT,118);
		m_LC_KeyPartList.InsertColumn(RULE_INDEX, _T("ID Rule"), LVCFMT_LEFT,220);
		m_LC_KeyPartList.InsertColumn(KEYPART_INDEX, _T("Accessory ID"), LVCFMT_LEFT,220);
		m_LC_KeyPartList.InsertColumn(RESULT_INDEX, _T("Result"), LVCFMT_LEFT,100);
	}

	/*b_isReadIMEI is got from table PICSTESTITEM of PreDB(function Init_CIMInfo)
	**b_isReadIMEI == true: Read IMEI from phone
	**b_isReadIMEI == false: Scan IMEI
	*/
	if (m_st_idParameter.str_station==_T("SHIPPINGOS") && m_st_idParameter.b_isReadIMEI)
	{
		m_Edit_SN.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ReadIMEI)->ShowWindow(SW_SHOW);
	}
	else
	{
		m_Edit_SN.SetFocus();
		GetDlgItem(IDC_BUTTON_ReadIMEI)->ShowWindow(SW_HIDE);
	}

	CFont font;
	font.CreateFont(
		30,                       // nHeight
		0,                        // nWidth
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

	GetDlgItem(IDC_BUTTON_PASS)->SetFont(&font, 1);
	GetDlgItem(IDC_BUTTON_FAIL)->SetFont(&font, 1);

	m_brushDiagBk.CreateSolidBrush(BKGCOLOUR_FACTORY);

	return;
}
/*
* @brief: UI Title initial
*/
bool CFAVDlg::InitialUITitle()
{
	char sz_Temp[MAX_PATH] = {0};
	GetModuleFileNameA(NULL, sz_Temp, MAX_PATH);
	PathRemoveFileSpecA(sz_Temp);

	CString strIniPath;
	strIniPath = sz_Temp;
	strIniPath += _T("\\INI\\setup.ini");

	WCHAR temp[50]={0};
	if(m_st_idParameter.str_station == _T("FQC") || m_st_idParameter.str_station == _T("OOB"))
	{
		::GetPrivateProfileString(_T("version"), _T("QATool_Version"), _T(""), temp,50, strIniPath);
	}
	else
	{
		::GetPrivateProfileString(_T("version"), _T("VITool_Version"), _T(""), temp,50, strIniPath);
	}
	m_strToolVer = temp;
	
	CString strPath;
	strPath = sz_Temp;
	strPath += _T("\\res\\Header.jpg");
	CImage image;      
	image.Load(strPath);
	if (image.IsNull())
	{
		return false;
	}

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

	CDC* pDC = pWnd->GetWindowDC();  
	pDC->SetStretchBltMode(HALFTONE);
	pDC->SetBrushOrg(0,0);//improve image quality
	CString str_toolname;
	str_toolname.Format(_T("%s %s Tool %s"), m_st_idParameter.str_modelName,m_st_idParameter.str_station,m_strToolVer);
	image.Draw( pDC->m_hDC, rect); 
	rect.left = rect.left + 100;
	rect.right = rect.right - 180;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));
	CFont font,*pOldFont;
	font.CreateFont(45, //font hight 
		15, // font width 
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
/**
* @brief: Insert Error Code from \\DB\\DB_Log\\ErrorInfo.ini to error comboBox
*/
void CFAVDlg::InsertErrorCodeToUI()
{
	int iCount = 0;

	CString strPath;
	CString strTemp;

	TCHAR sz_folder[MAX_PATH] = {0};
	GetModuleFileName(NULL, sz_folder, MAX_PATH);
	PathRemoveFileSpec(sz_folder);

	strPath = sz_folder;
	strPath += _T("\\DB\\DB_Log\\ErrorInfo.ini");

	WCHAR temp[50]={0};
	WCHAR ModelList[200] = {0};
	iCount = GetPrivateProfileInt(_T("ERROR_INFO"),_T("ERROR_COUNT"),1,strPath);

	for( int n=0; n<iCount+2; n++)
	{
		ZeroMemory(temp,sizeof(temp));
		wsprintf(temp,_T("%d"),n);
		GetPrivateProfileString(_T("ERROR_INFO"),temp,_T(" "),ModelList,50,strPath);
		m_comboBox_Errcode.InsertString(-1,ModelList);
	}

	strPath.ReleaseBuffer();
	m_comboBox_Errcode.SetCurSel(-1);
}

void CFAVDlg::SetUIIDParameter(st_UIIDparameter st_idParameter)
{
	m_st_idParameter = st_idParameter;
}

/**
* @brief: test pass
* ####1. set pics data to CQueryCIM ####
* ####2. Wirte pics data to PreDB ####
* ####3. If need reset phone, it will start a thread to run factoryreset.exe and will insert picsdata to the thread
*/
void CFAVDlg::OnBnClickedButtonPass()
{	
	GetDlgItem(IDC_BUTTON_PASS)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FAIL)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_ERRORCODE)->EnableWindow(FALSE);

	//set pics data
	SetPICSData(true);

	if (((CButton *)GetDlgItem(IDC_CHECK_RESET))->GetCheck()==1)
	{
		if(KillResetProcess())
		{   
			AfxBeginThread(ThreadFunc,this,0,0,0,0);
		}
		else
		{
			return;
		}
	}
	else 
	{
		//Insert result to PreDB
		if (!InsertTestResultToCIM(true))
		{
			ResetForReDO();
			return;
		}
	}
	
	/*set result to UI*/
	if (((CButton *)GetDlgItem(IDC_CHECK_RESET))->GetCheck()!=1)
	{	
		int iIndex = m_listResult.GetItemCount();
		m_listResult.InsertItem(iIndex, m_strSerialNo);
		m_listResult.SetItemText(iIndex, 1, _T("PASS"));
		m_listResult.EnsureVisible(iIndex, TRUE);
		m_listResult.SetItemColor(iIndex,RGB(0,255,0));

		ResetForReDO();
	}
}
/**
* @brief: Insert a fail record to PreDB
*/
void CFAVDlg::OnBnClickedButtonFail()
{
	GetDlgItem(IDC_BUTTON_PASS)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FAIL)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_ERRORCODE)->EnableWindow(FALSE);

	//set pics data
	SetPICSData(false);

	//Insert result to PreDB
	if (!InsertTestResultToCIM(false))
	{
		m_comboBox_Errcode.SetCurSel(-1);
		m_strErrorCode = _T("");
		ResetForReDO();
		return;
	}

	/*set result to UI*/
	m_Edit_SN.GetWindowText(m_strSerialNo);
	int iIndex = m_listResult.GetItemCount();
	m_listResult.InsertItem(iIndex, m_strSerialNo);

	if (m_st_idParameter.str_station==_T("SHIPPINGOS") && m_st_idParameter.b_isReadIMEI)
	{
		m_listResult.SetItemText(iIndex, 1, m_strErrorCode);
	}
	else
	{
		if (((CButton *)GetDlgItem(IDC_CHECK_ACCESSORY))->GetCheck()==1)
			m_listResult.SetItemText(iIndex, 1, _T("FAIL"));
		else
			m_listResult.SetItemText(iIndex, 1, m_strErrorCode);

	}
	m_listResult.EnsureVisible(iIndex, TRUE);
	m_listResult.SetItemColor(iIndex,RGB(255,0,0));

	ResetForReDO();
}

BOOL CFAVDlg::PreTranslateMessage(MSG* pMsg)
{
	const int WM_PostMessageToOOB = WM_USER + 0X102;//Will send by factoryReset.exe
	switch (pMsg->message)
	{
	case WM_PostMessageToOOB://used for factory reset
		if(pMsg->lParam==1)
		{
			m_bFactoryReset = true;
		}
		else if(pMsg->lParam==0)
		{
			m_bFactoryReset = false;
		}
		m_bResponse = true;
		m_Edit_SN.EnableWindow(true);
		m_Edit_SN.SetFocus();
		
		break;
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			return true;
			break;
		case VK_RETURN:

			if(pMsg->hwnd==GetDlgItem(IDC_EDIT_SN)->m_hWnd)
			{		
				return CheckInputID();//scan id
			}
			if(pMsg->hwnd==GetDlgItem(IDC_EDIT_KEYPARTBARCODE)->m_hWnd)
			{
				CheckAndDealWithBarcode();//accessory check
				return true;
			}
			break;

		default:
			break;
		}
	default:
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

/*According to the result selected, it will auto enable the pass or fail button*/
void CFAVDlg::OnCbnSelendokComboErrorcode()
{
	m_comboBox_Errcode.GetWindowText(m_strErrorCode);

	if (m_strErrorCode.GetLength()==0)
	{
		AfxMessageBox(_T("请选择错误代码!"));
		return;
	}

	if (m_strErrorCode!=_T("PASS"))
	{
		GetDlgItem(IDC_EDIT_KEYPARTBARCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FAIL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PASS)->EnableWindow(FALSE);
	}
	else
	{   
		if (m_st_idParameter.str_station == _T("OOB") && ((CButton *)GetDlgItem(IDC_CHECK_ACCESSORY))->GetCheck()==1)
		{
			GetDlgItem(IDC_BUTTON_PASS)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_FAIL)->EnableWindow(FALSE);
			ResetForScanAcessoryBarcode();
		}
		else
		{
			GetDlgItem(IDC_BUTTON_FAIL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_PASS)->EnableWindow(TRUE);
		}
	}
}
//Lock button is only enabled in FQC/OOB station for lock the selection
void CFAVDlg::OnBnClickedButtonLock()
{   
	::SendMessage(GetDlgItem(IDC_BUTTON_LOCK)->m_hWnd,WM_KILLFOCUS,0,0);

	m_LC_KeyPartList.SetTextBkColor(RGB(255,255,255));

	GetDlgItem(IDC_BUTTON_UNLOCK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LOCK)->EnableWindow(FALSE);

	GetDlgItem(IDC_CHECK_ACCESSORY)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_RESET)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_READPICASSO)->EnableWindow(FALSE);

	//if check read picasso, the button for read picasso is enabled. only support in FQC station
	if(((CButton *)GetDlgItem(IDC_CHECK_READPICASSO))->GetCheck()==1)
	{
		GetDlgItem(IDC_BUTTON_ReadIMEI)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ReadIMEI)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ReadIMEI)->SetWindowText(_T("R_Picasso"));
		m_Edit_SN.EnableWindow(false);
	}

	//if accessory checked, it will query the key parts and list in UI
	if (((CButton *)GetDlgItem(IDC_CHECK_ACCESSORY))->GetCheck()==1)
	{
		if (!GetKeypartInfo())
		{
			AfxMessageBox(_T("获取keypartsInfo信息fail"));
			ResetForReDO();
			return;
		}

		ResetKeyPartList();//update key parts in list
	}
	else
		m_Edit_KeyPartBarcode.EnableWindow(false);

	GetDlgItem(IDC_BUTTON_FAIL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PASS)->EnableWindow(FALSE);

	m_Edit_SN.EnableWindow(true);
	m_Edit_SN.SetFocus();
	m_Edit_SN.SetWindowText(_T(""));
}

//Unlock button is enabled while click Lock button
void CFAVDlg::OnBnClickedButtonUnlock()
{   
	m_LC_KeyPartList.SetTextBkColor(RGB(255,255,255));
	m_LC_KeyPartList.DeleteAllItems();

	m_vKeyPartInfo.clear();

	m_Edit_SN.EnableWindow(true);
	m_Edit_SN.SetWindowText(_T(""));

	m_comboBox_Errcode.SetCurSel(-1);

	InitialFunctionList();

	((CButton *)GetDlgItem(IDC_CHECK_READPICASSO))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_CHECK_ACCESSORY))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_CHECK_RESET))->SetCheck(0);

	GetDlgItem(IDC_BUTTON_LOCK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UNLOCK)->EnableWindow(FALSE);

	GetDlgItem(IDC_COMBO_ERRORCODE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FAIL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PASS)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ReadIMEI)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_ReadIMEI)->EnableWindow(FALSE);
}


void CFAVDlg::SetPICSData(bool bResult)
{
	CHAR Temp[512]={0};

	//common data
	m_map_PICSData[_T("TOOL_VERSION")] = m_strToolVer;

	if (m_st_idParameter.str_station == _T("OOB") || m_st_idParameter.str_station == _T("SHIPPINGOS")||m_st_idParameter.str_station == _T("P_VI"))
	{
		m_map_PICSData[_T("IMEI")] = m_strSerialNo;
	}

	m_map_PICSData[_T("SwVersion")] = m_strSW;

	if (m_st_idParameter.str_station==_T("SHIPPINGOS") && m_st_idParameter.b_isReadIMEI)
	{
		m_map_PICSData[_T("APKERROR")] = m_strErrorCode;
	}

	//Set QA data to oba_accessorycheck, oba_accessorycheck is fixed table and has fixed data
	if (m_st_idParameter.str_station == _T("FQC") || m_st_idParameter.str_station == _T("OOB"))
	{
		m_map_PICSData[_T("LotNo")] = dlg_Init.m_strLotNumber;
		m_map_PICSData[_T("LotNum")] = dlg_Init.m_strLotCount;
		m_map_PICSData[_T("Customer")] = _T("Q");//BenQ


		m_map_PICSData[_T("SerNo")] = m_strSerialNo;
		m_map_PICSData[_T("SN")] = m_strSerialNo;

		m_map_PICSData[_T("PartNo")] = m_st_idParameter.str_partNumber;
		m_map_PICSData[_T("Model")] = m_st_idParameter.str_modelName;

		CTime time = CTime::GetCurrentTime();
		m_map_PICSData[_T("DateID")] = time.Format(_T("%Y%m%d"));
		m_map_PICSData[_T("InDate")] = time.Format(_T("%Y-%m-%d %H:%M:%S"));

		if(m_st_idParameter.str_station == _T("FQC") || ((CButton *)GetDlgItem(IDC_CHECK_ACCESSORY))->GetCheck()==1)
		{
			m_map_PICSData[_T("InDatePacking")] = time.Format(_T("%Y-%m-%d %H:%M:%S"));
		}

		m_map_PICSData[_T("WeekID")] = time.Format(_T("%Y%U"));

		char Temp1[512]={0};
		ZeroMemory(Temp,sizeof(Temp));
		_itoa_s((time.GetYear()*100+time.GetMonth()),Temp,10);
		m_map_PICSData[_T("MonthID")] = Temp;

		int quarter=0;
		switch(time.GetMonth())
		{
		case 12:case 11:case 10:quarter=4;break;
		case 9:case 8:case 7:quarter=3;break;
		case 6:case 5:case 4:quarter=2;break;
		default:quarter=1;
		}

		quarter += time.GetYear()*10;
		ZeroMemory(Temp,sizeof(Temp));
		_itoa_s(quarter,Temp,10);
		m_map_PICSData[_T("QuarterID")] = Temp;

		m_map_PICSData[_T("OobLine")] = m_st_idParameter.str_line;


		CString strTemp;
		GetDlgItem(IDC_STATIC_WORKTYPE)->GetWindowText(strTemp);
		m_map_PICSData[_T("WorkType")] = strTemp;


		if (bResult)
			m_map_PICSData[_T("Note")] = _T("OK");
		else
			m_map_PICSData[_T("Note")] = _T("NG");

		if (m_st_idParameter.str_station == _T("FQC"))
		{
			m_map_PICSData[_T("Region")] = _T("FQC");
		}
		else if (m_st_idParameter.str_station == _T("OOB"))
		{
			m_map_PICSData[_T("Region")] = _T("OOB");
		}

		m_map_PICSData[_T("PONo")] = m_st_idParameter.str_so;

		m_map_PICSData[_T("EmploeeID")] = m_st_idParameter.str_employee;

		
		if (((CButton *)GetDlgItem(IDC_CHECK_ACCESSORY))->GetCheck()==1)
		{
			m_map_PICSData[_T("CheckAccessory")] = _T("Y");//set to PTOOB
			for(int i = 0 ; i<m_i_keyPartCount;i++)
			{
				CString strValue;
				CString strKey;
				strValue = m_LC_KeyPartList.GetItemText(i,KEYPART_INDEX);				
				strKey.Format(_T("Accessory%d"),i+1);
				m_map_PICSData[strKey] = strValue; //Set Accessory to oba_accessorycheck and PTOOB
			}
		}
		else
		{
			m_map_PICSData[_T("CheckAccessory")] = _T("N");
		}

		//set to PTOOB
		if (((CButton *)GetDlgItem(IDC_CHECK_RESET))->GetCheck()==1)
		{
			m_map_PICSData[_T("FactoryReset")] = _T("Y");
		}
		else
		{
			m_map_PICSData[_T("FactoryReset")] = _T("N");
		}


		//set to PTFQC
		if (((CButton *)GetDlgItem(IDC_CHECK_READPICASSO))->GetCheck()==1)
		{
			m_map_PICSData[_T("ReadPicassoFromPhone")] = _T("Y");
		}
		else
		{
			m_map_PICSData[_T("ReadPicassoFromPhone")] = _T("N");
		}

	}

	m_obj_QueryCIM.SetPICSData(m_map_PICSData);
}

bool CFAVDlg::InsertTestResultToCIM(bool bResult)
{
	bool m_DBresult = false;
	CString strErrorcode=_T("");
	CString strErrorMsg = _T("");
	char c_SerNo[30]={0};
	CHAR Temp[512]={0};

	CStringToChar(m_strPicasso,c_SerNo);

	if (bResult)
	{
		m_DBresult = m_obj_QueryCIM.InsertTestRecordToCIM(bResult);
	}
	else
	{
		if(m_strErrorCode.Find(_T("_")) != -1)
			strErrorcode = m_strErrorCode.Left(m_strErrorCode.Find(_T("_")));  //error code OP selected
		else if(m_strErrorCode.Find(_T(",")) !=-1)
			strErrorcode = m_strErrorCode.Left(m_strErrorCode.Find(_T(",")));  //shipping OS error code all
		else
			strErrorcode = m_strErrorCode;

		//if shipping os and auto test, set auto test result
		if (m_st_idParameter.str_station==_T("SHIPPINGOS") && m_st_idParameter.b_isReadIMEI)
		{
			m_obj_QueryCIM.SetErrorCodeToCIM(m_strErrorCode);
		}
		else
		{
			m_obj_QueryCIM.SetErrorCodeToCIM(strErrorcode);
		}

		m_DBresult = m_obj_QueryCIM.InsertTestRecordToCIM(bResult);
	}

	if (!m_DBresult)
	{
		AfxMessageBox(m_obj_QueryCIM.GetErrorMsg());
		return false;
	}

	if (m_st_idParameter.str_station == _T("FQC") || m_st_idParameter.str_station == _T("OOB"))
	{  
		//Button pass but not Factory Reset
		if (bResult)
		{
			if (((CButton *)GetDlgItem(IDC_CHECK_RESET))->GetCheck()==0)//if factory reset, it will be InsertOOBDataToCIM("1") in factory reset thread
			{
				m_DBresult = m_obj_QueryCIM.InsertOOBDataToCIM("1");
			}
		}
		//Button fail
		else
		{
			m_DBresult = m_obj_QueryCIM.InsertOOBDataToCIM("0");
		}

		if (!m_DBresult)
		{
			AfxMessageBox(m_obj_QueryCIM.GetErrorMsg());
			return false;
		}

		//remove this function, carton remove already have it  ////////Nancy.Yang 20140825
		/*if((m_st_idParameter.str_station == _T("OOB")) && (!bResult))
		{
			if(((CButton *)GetDlgItem(IDC_CHECK_ACCESSORY))->GetCheck()==1)
			{
				m_DBresult = m_obj_QueryCIM.UnlinkKeypartsFromCIM(c_SerNo);
				if (!m_DBresult)
				{
					AfxMessageBox(m_obj_QueryCIM.GetErrorMsg());
					return false;
				}
			}
		}*/
	}
	return true;
}

bool CFAVDlg::Init_CIMInfo()
{
	m_obj_QueryCIM.SetidParameter(m_st_idParameter);

	if(m_obj_QueryCIM.QueryDataFromCIM())
	{		
		m_obj_QueryCIM.GetUIIDParameter(m_st_idParameter);
	}
	else
	{
		AfxMessageBox( m_obj_QueryCIM.GetErrorMsg());
		return false;
	}

	return true;
}

/*
m_st_idParameter.str_APKTestItem get from PreDB
eg.devices_IMEI:Y,sos_ver:Y,total_items:Y,imei:Y,meid:N,.....
*/
void CFAVDlg::ParseApkTestItem()
{
	m_LC_KeyPartList.InsertColumn(0,_T("Item"), LVCFMT_LEFT, 40);
	if (m_st_idParameter.str_APKTestItem != _T(""))
	{
		int i_index = 0;
		int iPos = 1;

		while(m_st_idParameter.str_APKTestItem.Find(_T(",")) != -1)
		{
			i_index = m_st_idParameter.str_APKTestItem.Find(_T(","));

			CString str_Item_Parse = m_st_idParameter.str_APKTestItem.Left(i_index);

			m_st_idParameter.str_APKTestItem = m_st_idParameter.str_APKTestItem.Mid(i_index+1);

			if(str_Item_Parse.Right(1)==_T("Y"))
			{
				int i = str_Item_Parse.Find(_T(":"));
				m_map_apkTestItemIndex[str_Item_Parse.Left(i).MakeUpper()] = iPos;
				m_LC_KeyPartList.InsertColumn(iPos,str_Item_Parse.Left(i), LVCFMT_LEFT, 65);
				iPos++;
			}

		}//while
	}//if
}

bool CFAVDlg::InitialResultCounter()
{
	CString str_tempPass;
	CString str_tempFail;
	CString str_tempTotal;

	if (!m_obj_QueryCIM.QueryRecordInfoFromCIM())
	{
		AfxMessageBox(m_obj_QueryCIM.GetErrorMsg());
		return false;
	}

	str_tempPass.Format(_T("%d"), m_obj_QueryCIM.GetPassNo());
	str_tempFail.Format(_T("%d"), m_obj_QueryCIM.GetFailNo());
	str_tempTotal.Format(_T("%d"), m_obj_QueryCIM.GetTotalNo());

	GetDlgItem(IDC_STATIC_TOTAL)->SetWindowText(str_tempTotal);
	GetDlgItem(IDC_STATIC_FAIL)->SetWindowText(str_tempFail);
	GetDlgItem(IDC_STATIC_PASS)->SetWindowText(str_tempPass);

	return true;
}

HBRUSH CFAVDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor ==CTLCOLOR_DLG)
		hbr = (HBRUSH)m_brushDiagBk.GetSafeHandle(); 

	return hbr;
}

bool CFAVDlg::GetKeypartInfo()
{
	int nStart = 0;
	int nSubStart = 0;
	int nPos = 0;
	int nSubPos = 0;
	int ActualKeyPartCount = 0;
	int IndexID = 0;
	int ActualKeyPartCountInBACH = 0;

	std::vector <KeyPartInfoArray> vKeyPartInfo;

	CString str_KeypartInfo;
	m_i_keyPartCount = 0;

	//eg. if m_i_keyPartCount=2, str_keyPartsInfo = "kptype,description,kpword,kppn,chkduplic;kptype,description,kpword,kppn,chkduplic;"
	if (!m_obj_QueryCIM.QueryKeypartsInfoFromCIM(str_KeypartInfo,m_i_keyPartCount))
	{
		AfxMessageBox(m_obj_QueryCIM.GetErrorMsg());
		return false;
	}

	// Analyze and Deal with m_strKeyPartsInfo
	if (str_KeypartInfo == _T(""))
	{
		AfxMessageBox(_T("pQueryKeyparts_Info -> No Keyparts Found! "));
		return false;
	}

	CString delims = _T(";");
	CString Subdelims = _T(",");
	CString OneKeyPartInfo;

	KeyPartInfoArray tempstrArray;

	while (str_KeypartInfo.Find(delims,nStart) > 0 || nStart < str_KeypartInfo.GetLength())
	{
		if (str_KeypartInfo.Find(delims,nStart) > 0)
			nPos = str_KeypartInfo.Find(delims,nStart);
		else
			nPos = str_KeypartInfo.GetLength();
		OneKeyPartInfo = str_KeypartInfo.Mid(nStart,nPos-nStart);

		while (OneKeyPartInfo.Find(Subdelims,nSubStart) > 0 || nSubStart < OneKeyPartInfo.GetLength())
		{
			if (OneKeyPartInfo.Find(Subdelims,nSubStart) > 0)
				nSubPos = OneKeyPartInfo.Find(Subdelims,nSubStart);
			else
				nSubPos = OneKeyPartInfo.GetLength();
			CString strTemp = OneKeyPartInfo.Mid(nSubStart,nSubPos-nSubStart);
			if(IndexID == 0)
				tempstrArray.strKP_Type = OneKeyPartInfo.Mid(nSubStart,nSubPos-nSubStart);//kptype
			if(IndexID == 1)
				tempstrArray.strKP_Description = OneKeyPartInfo.Mid(nSubStart,nSubPos-nSubStart);//description
			if(IndexID == 2)
				tempstrArray.strKP_ID_Rule = OneKeyPartInfo.Mid(nSubStart,nSubPos-nSubStart);//kpword
			if(IndexID == 3)
			{
				tempstrArray.strKP_Part_Number = OneKeyPartInfo.Mid(nSubStart,nSubPos-nSubStart);//kppn
				break;
			}
			nSubStart = nSubPos + 1;

			IndexID++;
		}
		if(IndexID != 3)
		{
			AfxMessageBox(_T("Check KeyPart Info Error! Info = ") + OneKeyPartInfo );
			return false;
		}

		char c_qty[10]={0};
		char c_KeyPartPN[32]={0};
		//Check The KeyPart Part Number if exist in BACH SO BOM      
		CStringToChar(tempstrArray.strKP_Part_Number,c_KeyPartPN);
		tempstrArray.strKPQuantityInSO = _T("");

		if (m_obj_QueryCIM.QueryPNinBACHBySOFromCIM(c_KeyPartPN,c_qty))
		{
			tempstrArray.bExistInSOBOM = true;
			tempstrArray.strKPQuantityInSO = ConvertString(c_qty);
			vKeyPartInfo.insert(vKeyPartInfo.begin() + ActualKeyPartCountInBACH,tempstrArray);
			ActualKeyPartCountInBACH ++;
		}
		else
		{
			tempstrArray.bExistInSOBOM = false;
			vKeyPartInfo.push_back(tempstrArray);
		}
		nStart = nPos + 1;
		ActualKeyPartCount++;
		nSubStart = 0;
		nSubPos = 0;
		IndexID = 0;

	}
	if ( ActualKeyPartCount != m_i_keyPartCount)
	{
		AfxMessageBox(_T("QueryKeyparts_Info Err = ") + str_KeypartInfo );
		return false;
	}

	//Check The KeyPart Part Number if exist in BACH SO BOM and Quantity
	//if Quantity = 0 and there is another kp with same kptype, the Quantity = 0 one will not be included. Modified by Lennon 20120118
	//sunlight modify 20120424
	for(unsigned int i = 0; i< vKeyPartInfo.size();i++)
	{
		if (vKeyPartInfo[i].strKPQuantityInSO == _T("0"))
		{
			for(unsigned int j = 0; j< vKeyPartInfo.size();j++)
			{   
				if (vKeyPartInfo[j].bExistInSOBOM == false)
					continue;
				if(j == i)
					continue;
				if((vKeyPartInfo[j].strKP_Type == vKeyPartInfo[i].strKP_Type)&& vKeyPartInfo[j].strKPQuantityInSO!=_T("0"))
				{
					vKeyPartInfo[i].bExistInSOBOM = false;
					break;
				}
			}
		}
	}

	//Copy to m_vKeyPartInfo
	m_vKeyPartInfo = vKeyPartInfo;

	return true;
}
/**
* check key part rule
check the length
each non '*' character should be equal with the rule
*/
bool CFAVDlg::KeyPartIDRuleCompare(CString strKeyPartBarcode, CString strKP_ID_Rule)
{
	if(strKeyPartBarcode.GetLength() == 0 || strKeyPartBarcode.GetLength() !=strKP_ID_Rule.GetLength())
		return false;
	for(int i = 0; i < strKeyPartBarcode.GetLength() ; i++)
	{
		if(strKP_ID_Rule.Mid(i,1) != _T("*") && strKeyPartBarcode.Mid(i,1) != strKP_ID_Rule.Mid(i,1))
			return false;
	}
	return true;
}
/**
* check accessory for OOB station
*/
void CFAVDlg::CheckAndDealWithBarcode(void)
{
	CString str_barcodeScan;
	m_Edit_KeyPartBarcode.GetWindowText(str_barcodeScan);

	ResetForScanAcessoryBarcode();
	if(str_barcodeScan.GetLength() > KEYPARTBARCODE_MAXLENGTH || str_barcodeScan.GetLength() < KEYPARTBARCODE_MINLENGTH)
	{
		AfxMessageBox(_T("Barcode length error!"));
		return;
	}

	CString str_keypartPN;//kpWord to kpPN
	for(int i = 0; i< m_i_keyPartCount;i++)
	{
		if(KeyPartIDRuleCompare(str_barcodeScan,m_vKeyPartInfo[i].strKP_ID_Rule))//find the matched one
		{
			str_keypartPN = m_vKeyPartInfo[i].strKP_Part_Number;//replace with the real part number
			break;
		}
	}

	if(str_keypartPN == _T(""))
	{
		AfxMessageBox(_T("No Qisda Part Number match in list! KeyPartBarcode = ") + str_barcodeScan);
		return;
	}

	for(int i = 0 ; i<m_i_keyPartCount;i++)
	{
		if (str_keypartPN == m_LC_KeyPartList.GetItemText(i,PN_INDEX))
		{
			if (m_LC_KeyPartList.GetItemText(i,RESULT_INDEX) == _T("PASS"))
			{
				AfxMessageBox(_T("The KeyPart:") +str_barcodeScan + _T("(") + str_keypartPN +_T(") is dulplicate with ") + m_LC_KeyPartList.GetItemText(i,KEYPART_INDEX));
				return;
			}
			if (m_LC_KeyPartList.GetItemText(i,RESULT_INDEX) == _T("N/A"))
			{
				AfxMessageBox(_T("The KeyPart:") +str_barcodeScan +_T( "(") + str_keypartPN +_T(") is N/A in SO BOM!" ));
				return;
			}

			m_LC_KeyPartList.SetItemText(i,KEYPART_INDEX, str_barcodeScan);
			m_LC_KeyPartList.SetItemText(i,RESULT_INDEX, _T("PASS"));
			m_LC_KeyPartList.SetItemColor(i,RGB(0,255,0));

			bool m_b_KeyPartScanOK = true;
			for(int i = 0; i< m_i_keyPartCount; i++)
			{
				m_b_KeyPartScanOK = m_b_KeyPartScanOK && (m_LC_KeyPartList.GetItemText(i,RESULT_INDEX) == _T("PASS") ||( m_LC_KeyPartList.GetItemText(i,RESULT_INDEX) == _T("N/A")));
				if (!m_b_KeyPartScanOK)
					return;//go on check
			}

			// all accessory have checked, enable pass button
			if(m_b_KeyPartScanOK  && (m_i_keyPartCount > 0))
			{
				GetDlgItem(IDC_BUTTON_PASS)->EnableWindow(TRUE);
			}
			return;
		}
	}

	AfxMessageBox(_T("The KeyPart:") + str_barcodeScan + _T(" is unnecessary!"));
	return;
}
/**
* Reset UI for redo
*/
void CFAVDlg::ResetForReDO()
{
	m_strSerialNo.Empty();
	m_Edit_SN.SetWindowText(_T(""));

	m_strPicasso = _T("");
	m_comboBox_Errcode.SetCurSel(-1);
	m_strErrorCode = _T("");

	if (m_st_idParameter.str_station ==_T("SHIPPINGOS") && m_st_idParameter.b_isReadIMEI)
	{
		m_Edit_SN.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_ReadIMEI)->EnableWindow(TRUE);
	}
	else if(((CButton *)GetDlgItem(IDC_CHECK_READPICASSO))->GetCheck()==1)
	{
		m_Edit_SN.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_ReadIMEI)->EnableWindow(TRUE);
	}
	else
	{
		m_Edit_SN.EnableWindow(true);
		m_Edit_SN.SetFocus();
	}

	m_Edit_KeyPartBarcode.EnableWindow(false);
	m_Edit_KeyPartBarcode.SetWindowText(_T(""));

	GetDlgItem(IDC_COMBO_ERRORCODE)->EnableWindow(FALSE);

	ResetKeyPartList();
	InitialResultCounter();
}
/**
* Empty key part Result Column for Re-Scan
*/
void CFAVDlg::ResetKeyPartList()
{
	if (((CButton *)GetDlgItem(IDC_CHECK_ACCESSORY))->GetCheck()==1)
	{
		m_LC_KeyPartList.DeleteAllItems();

		TCHAR itemID[32];
		for(int i = 0; i< m_i_keyPartCount; i++)
		{
			m_LC_KeyPartList.SetItemColor(i,RGB(255,255,255));
			wsprintf(itemID,_T("%d"), i+1);
			m_LC_KeyPartList.InsertItem(i,itemID);
			m_LC_KeyPartList.SetItemText(i, PN_INDEX, m_vKeyPartInfo[i].strKP_Part_Number);//设置行索引（行号）和列索引（列号）的值
			m_LC_KeyPartList.SetItemText(i, DESCRIPTION_INDEX, m_vKeyPartInfo[i].strKP_Description);
			m_LC_KeyPartList.SetItemText(i, RULE_INDEX, m_vKeyPartInfo[i].strKP_ID_Rule);
			m_LC_KeyPartList.SetItemText(i, KEYPART_INDEX, _T(""));
			m_LC_KeyPartList.SetItemText(i, RESULT_INDEX, _T(""));

			if (m_vKeyPartInfo[i].bExistInSOBOM == false)
			{
				m_LC_KeyPartList.SetItemColor(i,RGB(114,114,114));
				m_LC_KeyPartList.SetItemText(i, RESULT_INDEX, _T("N/A"));
			}
		}
	}
}

void CFAVDlg::ResetForScanAcessoryBarcode(void)
{
	m_Edit_KeyPartBarcode.EnableWindow(true);
	m_Edit_KeyPartBarcode.SetWindowText(_T(""));
	m_Edit_KeyPartBarcode.SetFocus();
}
/**
* 1.Check ID length
* 2.Pline: Check IMEI status
* 3.Check routing
*/
bool CFAVDlg::CheckInputID()
{
	bool m_DBresult = false;
	char c_InputIMEI[50]={0};
	CString strTemp;

	m_strErrorCode = _T("");
	m_LC_KeyPartList.SetTextBkColor(RGB(255,255,255));
	m_Edit_SN.EnableWindow(false);
	m_Edit_SN.GetWindowText(m_strSerialNo);

	do
	{
		//1. OOB/SHIPPINGOS/P_VI should scan IMEI 
		if (m_st_idParameter.str_station == _T("OOB") || m_st_idParameter.str_station == _T("SHIPPINGOS")||m_st_idParameter.str_station == _T("P_VI"))
		{
			if(m_strSerialNo.GetLength() != m_st_idParameter.i_idLength)
			{
				strTemp.Format(_T("%s:%d"),m_strSerialNo,m_st_idParameter.i_idLength);
				AfxMessageBox(_T("输入ID的长度不对，请重新输入!")+strTemp);
				break;				
			}
			else
			{
				/* Check IMEI status */
				char c_status[10]={0};
				CStringToChar(m_strSerialNo,c_InputIMEI);
				m_DBresult = m_obj_QueryCIM.QueryIMEIStatusFromCIM(c_InputIMEI,c_status);
				if (!m_DBresult)
				{
					AfxMessageBox(m_obj_QueryCIM.GetErrorMsg());
					break;
				}
				else
				{   
					if (strcmp(c_status,"S")==0)
					{					
						strTemp.Format(_T("IMEI status is [%s]\nIMEI状态位为%s，已包装不能走此站正常流程"),c_status,c_status);
						AfxMessageBox(strTemp);
						break;
					}

					if (m_st_idParameter.str_station == _T("OOB"))
					{
						if(((CButton *)GetDlgItem(IDC_CHECK_ACCESSORY))->GetCheck()==1)
						{
							if (strcmp(c_status,"P") != 0)
							{
								strTemp.Format(_T("IMEI status is [%s]\nIMEI状态位为%s，不能走QA正常流程"),c_status,c_status);
								AfxMessageBox(strTemp);
								break;
							}
						}
						else
						{
							if (strcmp(c_status,"I") != 0 && strcmp(c_status,"P") != 0)
							{
								strTemp.Format(_T("IMEI status is [%s]\nIMEI状态位为%s，不能走QA正常流程"),c_status,c_status);
								AfxMessageBox(strTemp);
								break;
							}
						}
					}	
				}

				/* check SSS hold information by IMEI*/
				m_DBresult = m_obj_QueryCIM.QueryholdInfoFromCIM(c_InputIMEI);
				if (!m_DBresult)
				{
					AfxMessageBox(m_obj_QueryCIM.GetErrorMsg());
					break;
				}

				/* Get Picasso by IMEI to check routing */
				char c_Picasso[11]={0};
				m_DBresult = m_obj_QueryCIM.QueryPicassoFromCIM(c_InputIMEI,c_Picasso);
				if (!m_DBresult)
				{
					AfxMessageBox(m_obj_QueryCIM.GetErrorMsg());
					break;
				}

				m_strPicasso = ConvertString(c_Picasso);

				/*Check WiFi MAC login info */
				if (CHECK_WIFI &&  m_st_idParameter.str_station == _T("OOB"))
				{
					char c_WiFiMAC[20];
					char c_LoginWifiInfo[10];

					m_DBresult = m_obj_QueryCIM.QueryWifiLoginInfoFromCIM(c_InputIMEI,c_WiFiMAC,c_LoginWifiInfo);
					if (!m_DBresult)
					{
						AfxMessageBox(m_obj_QueryCIM.GetErrorMsg());
						break;
					}
					else
					{
						if (ConvertString(c_LoginWifiInfo) != _T("Success"))
						{
							AfxMessageBox(_T("此WiFi MAC 没有测试PASS!"));
							break;
						}
					}
				}/*Check WiFi MAC login info */
			}
		}//if OOB/SHIPPINGOS/P_VI should scan IMEI 
		else // FQC/VI/OQC should scan picasso
		{
			if(m_strSerialNo.GetLength() != PICASSO_LENGTH)
			{	
				AfxMessageBox(_T("输入ID的长度不对，请重新输入!"));
				break;
			}
			m_strPicasso = m_strSerialNo;
		}

		/* Check Routing */
		char c_SerNo[15]={0};
		CStringToChar(m_strPicasso, c_SerNo);

		if(m_st_idParameter.str_station == _T("OOB") && ((CButton *)GetDlgItem(IDC_CHECK_ACCESSORY))->GetCheck() != 1)
		{
			m_DBresult = m_obj_QueryCIM.QueryStationRoutingInfoFromCIM(c_SerNo,true);
		}
		else
		{
			m_DBresult = m_obj_QueryCIM.QueryStationRoutingInfoFromCIM(c_SerNo,false);
		}

		if (!m_DBresult)
		{
			AfxMessageBox(m_obj_QueryCIM.GetErrorMsg());
			break;
		}

		m_comboBox_Errcode.SetCurSel(-1);

		GetDlgItem(IDC_COMBO_ERRORCODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_FAIL)->EnableWindow(FALSE);

		if(((CButton *)GetDlgItem(IDC_CHECK_ACCESSORY))->GetCheck()==1)
		{
			ResetForScanAcessoryBarcode();
			GetDlgItem(IDC_BUTTON_PASS)->EnableWindow(FALSE);
		}
		else
			GetDlgItem(IDC_BUTTON_PASS)->EnableWindow(TRUE);

		return true;

	}while(0);

	ResetForReDO();
	return false;
}

bool CFAVDlg::KillResetProcess()
{
	CString str_winExecCommand;
	CString str_commandOne;        
	CString str_commandTwo;         

	str_commandOne = _T("cmd.exe /c ");

	str_commandTwo = _T("taskkill /f /im FactoryReset.exe");
	str_winExecCommand = str_commandOne + str_commandTwo;

	if(!ExeCmd(str_winExecCommand,false,NULL)) 
	{   
		AfxMessageBox(_T("无法删除FactoryReset.exe请手动删除"));
		return false;
	}
	else
	{  
		return true;
	}
}

bool CFAVDlg::SynTime()
{
	WCHAR temp[50]={0};
	CString m_str_UIsetting;
	m_str_UIsetting.Format(_T("C:\\PE_Config\\%s\\setting\\UIControl.ini"), m_st_idParameter.str_modelName);
	::GetPrivateProfileString(_T("COMMON"), _T("IP"), _T("10.85.68.13"), temp, 50, m_str_UIsetting);

	CString str_command;
	str_command.Format(_T("net time \\\\%s /set /y"),temp);

	if(ExeCmd(str_command,false,NULL))
		return true;
	else
	{
		AfxMessageBox(_T("同步server时间失败,请检查网络连接"));
		return false;
	}

	return true;
}
/**
* Read imei/picasso/SOSResult from phone
* Related Function: ReadPicassoFromPhone,PullSOSResultFromPhone,ForShipingosToParseSOS
*/
void CFAVDlg::OnBnClickedButtonReadIMEI()
{   
	GetDlgItem(IDC_BUTTON_ReadIMEI)->EnableWindow(FALSE);

	CString strSerialNo;
	//For FQC read picasso from phone
	if(((CButton *)GetDlgItem(IDC_CHECK_READPICASSO))->GetCheck() == 1) 
	{

		if (!ReadPicassoFromPhone(strSerialNo))
		{
			GetDlgItem(IDC_BUTTON_ReadIMEI)->EnableWindow(TRUE);
			return;
		}
		m_Edit_SN.SetWindowText(strSerialNo);

		if(!CheckInputID())
			return;
	}
	//For shipping os read imei and SOSResult from phone
	else 
	{
		if(!PullSOSResultFromPhone())
		{
			ResetForReDO();
			return;
		}

		CString m_str_Result;
		m_str_Result=_T("PASS");
		m_strErrorCode = _T("");

		if (!ForShipingosToParseSOS(m_str_Result,m_strErrorCode,strSerialNo))
		{
			ResetForReDO();
			return;
		}

		m_Edit_SN.SetWindowText(strSerialNo);

		if(!CheckInputID())
			return;

		if (m_str_Result == _T("PASS"))              //parse sosresult.log  PASS
		{
			OnBnClickedButtonPass();
		}
		else if (m_str_Result == _T("FAIL"))         //Parese Result Fail
		{
			OnBnClickedButtonFail();
		}
		else if (m_str_Result == _T("NA"))         //Result exist NA
		{
			AfxMessageBox(_T("存在未测试项,请测试完成"));
			GetDlgItem(IDC_BUTTON_FAIL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_PASS)->EnableWindow(FALSE);
			ResetForReDO();
		}
		else
		{
			AfxMessageBox(_T("测试结果不正常，请重新测试"));
			GetDlgItem(IDC_BUTTON_FAIL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_PASS)->EnableWindow(FALSE);
			ResetForReDO();
		}

		DeleteAPKLog();		
	}
}
bool CFAVDlg::KillADBServer()
{
	ExeCmd(_T("kill-server"), true, NULL);
	return true;
}
/**
* Pull SOSResult from phone to 'D:\LOG\SOSResult.log'
*/
bool CFAVDlg::PullSOSResultFromPhone()
{
	CString str_PullCommand1;
	CString str_PullCommand2;
	CString str_ErrorCode;

	CFileFind obj_fileFind;
	CString str_PathFile = _T("D:\\LOG\\SOSResult.log");
	//delete the old log first
	if(obj_fileFind.FindFile(str_PathFile))
	{
		DeleteFile(str_PathFile);
	}
	// set pull command
	if (m_st_idParameter.str_SDstatus == _T("IN"))
	{
		str_PullCommand1 =  _T("pull /storage/sdcard0/SOSResult.log D:\\LOG\\SOSResult.log");
		str_PullCommand2 =  _T("pull /storage/sdcard1/SOSResult.log D:\\LOG\\SOSResult.log");
	}
	else if (m_st_idParameter.str_SDstatus ==_T("OUT"))
	{
		str_PullCommand1 =  _T("pull /sdcard/SOSResult.log D:\\LOG\\SOSResult.log");
		str_PullCommand2 =  _T("pull /storage/sdcard0/SOSResult.log D:\\LOG\\SOSResult.log");
	}
	else
	{
		str_PullCommand1 = _T("pull ") + m_st_idParameter.str_SDstatus + _T(" D:\\LOG\\SOSResult.log");
		str_PullCommand2 = str_PullCommand1;
	}

	//run command to pull result
	ExeCmd(str_PullCommand1,true,NULL);

	if(!obj_fileFind.FindFile(str_PathFile))
	{
		ExeCmd(str_PullCommand2,true,NULL);
	}

	if(!obj_fileFind.FindFile(str_PathFile))
	{
		AfxMessageBox(_T("Pull SOSResult Fail"));
		return false;
	}

	return true;	

}
/**
* Launch Picasso.exe to read picasso
*/
bool CFAVDlg::ReadPicassoFromPhone(CString & str_picasso)
{
	CString strExe = _T("C:\\Tool\\PicassoTool\\Picasso.exe");//Run read picasso.exe to create picasso.txt
	if (_taccess(strExe, 0) == -1)
	{
		AfxMessageBox(strExe + _T(" is not exist, please check it!"));
		return false;
	}

	CString strCommand;
	strCommand.Format(_T("cmd.exe /c %s"),strExe);

	if(!ExeCmd(strCommand,false,NULL))
	{
		return false;
	}

	CString strPicasso = _T("C:\\Tool\\PicassoTool\\PICASSO.txt");
	CStdioFile PicassoFile;
	for(int i = 0; i< 50; i++)
	{
		if(!PicassoFile.Open(strPicasso.GetBuffer(), CFile::modeRead))
		{
			strPicasso.ReleaseBuffer();
			Sleep(500);
		}
		else
		{
			PicassoFile.ReadString(str_picasso);
			PicassoFile.Close();
			DeleteFile(strPicasso.GetBuffer());
			strPicasso.ReleaseBuffer();
			return true;
		}
	}
	AfxMessageBox(_T("Read Picasso Fail!"));
	return false;
}
/**
* Read IMEI and SOSResult form "D:\\LOG\\SOSResult.log"

eg.SOSResult.log
devices_IMEI: 862573010000605
sos_ver: 1.00
total_items: 16
imei: PASS
meid: PASS
...
*/
bool CFAVDlg::ForShipingosToParseSOS(CString &str_Result,CString &str_ErrorCode,CString &str_PhoneImei)
{
	CStdioFile filePath;
	bool bNA = false;
	bool bFAIL = false;

	CString strFilePath=_T("D:\\LOG\\SOSResult.log");
	if (_taccess(strFilePath, 0) == -1)
	{
		AfxMessageBox(_T("Pull SOSResult Fail"));
		return false;
	}

	m_LC_KeyPartList.DeleteAllItems();
	m_LC_KeyPartList.InsertItem(0,_T("0"));

	bool b_getIMEI = false;
	int nTestCount = 0;
	CString strTemp = _T("");

	if(filePath.Open (strFilePath, CFile::typeText|CFile::modeRead))
	{
		while(filePath.ReadString(strTemp))
		{
			if(strTemp.IsEmpty())
				break;

			CString str_Item_Name = strTemp.Left(strTemp.Find(_T(":"))).MakeUpper();
			CString str_Item_result = strTemp.Mid(strTemp.Find(_T(":")) + 2);

			if (!b_getIMEI)
			{
				str_PhoneImei = str_Item_result;//Get IMEI in first line
				b_getIMEI = true;
			}

			if (m_map_apkTestItemIndex.count(str_Item_Name)!=0)
			{
				nTestCount++;

				int index = m_map_apkTestItemIndex[str_Item_Name];
				m_LC_KeyPartList.SetItemText(0,index, str_Item_result);

				//save test error code to str_ErrorCode
				if(str_Item_result != _T("PASS"))
				{
					if(str_Item_result != _T("NA"))
					{
						if((str_Item_Name != _T("DEVICES_IMEI")) && (str_Item_Name != _T("SOS_VER")) && (str_Item_Name != _T("TOTAL_ITEMS")))
						{
							str_Result = _T("FAIL");
							str_ErrorCode += str_Item_Name + _T(":") + str_Item_result + _T(",");
							bFAIL = true;
						}
					}
					else //if(strValueTemp == _T("NA"))
					{
						bNA  = true;
					}
				}
			}
		}
	}
	filePath.Close();

	if((!bFAIL && bNA) || (m_map_apkTestItemIndex.size() != nTestCount))
	{
		str_Result = _T("NA");
	}

	return true;
}
/*adb command to delete apk test log in sd card
*/
void CFAVDlg::DeleteAPKLog()
{
	bool b_res = false;
	CString m_str_Command = _T("");
	CString m_str_Command2 = _T("");
	if(m_st_idParameter.str_SDstatus==_T("IN"))
	{
		m_str_Command = _T("shell rm /storage/sdcard0/SOSResult.log");
		m_str_Command2 = _T("shell rm /storage/sdcard1/SOSResult.log");
	}
	else if(m_st_idParameter.str_SDstatus==_T("OUT"))
	{
		m_str_Command = _T("shell rm /sdcard/SOSResult.log");
		m_str_Command2 = _T("shell rm /storage/sdcard0/SOSResult.log");
	}

	b_res = ExeCmd(m_str_Command,true,NULL);
	b_res = ExeCmd(m_str_Command2,true,NULL);
	if (!b_res)
	{
		AfxMessageBox(_T("删除SOSResult.log失败"));
	}
}
/*Run command
*/
bool CFAVDlg::ExeCmd(CString Command, bool b_isAdbCmd, char* sz_Output, int i_timeout)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	CString path_adb;
	if (b_isAdbCmd)
	{
		CString pthToolDir;
		::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
		pthToolDir.ReleaseBuffer();
		pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));

		path_adb = pthToolDir + _T("\\adb.exe");
		if (_taccess(path_adb, 0) == -1) {
			AfxMessageBox(_T("ERROR: No adb.exe exist!"));
			return false;
		}
	}

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
		AfxMessageBox(str_errorMsg);
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

	if (b_isAdbCmd)
	{
		Command = _T("\"") + path_adb + _T("\" ") + Command;
		TRACE(_T("Cmd: %s\n"), Command);
	}

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
			AfxMessageBox(str_errorMsg);
		}
		else 
		{
			isOk = true;

			if (sz_Output != NULL)
			{
				DWORD bytesRead;
				char *message = new char[nPipeSize];
				memset(message, 0, sizeof(message));
				::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
				message[bytesRead] = '\0';
				strcpy(sz_Output, message);
				delete [] message;
			}
		}
	}
	else 
	{
		isOk = false;
		str_errorMsg.Format(_T("ERROR: Execute %s fail!"),Command);
		AfxMessageBox(str_errorMsg);
	}

	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(hProcess);
	hProcess = NULL;

	return isOk;
}
/**
* Launch factory reset exe
*/
UINT CFAVDlg::ThreadFunc(LPVOID lpParam)
{
	CFAVDlg* pFAVDlg=(CFAVDlg*)lpParam;

	int i_timeout = 0;
	CString str_winExecCommand;
	CString str_commandOne;        
	CString str_commandTwo;         

	pFAVDlg->m_bResponse = false;
	pFAVDlg->m_bFactoryReset = false;

	CString m_FactoryResetPath;
	m_FactoryResetPath = pFAVDlg->m_st_idParameter.str_imagePath;

	str_commandOne = _T("cmd.exe /c ");
	str_commandTwo = _T("start /d \"")+ m_FactoryResetPath +_T("\" ")+_T("FactoryReset.exe");
	str_winExecCommand = str_commandOne+str_commandTwo;

	if(!pFAVDlg->ExeCmd(str_winExecCommand,false,NULL)) 
	{   
		AfxMessageBox(_T("无法开启FactoryReset.exe请手动删除"));
		return 0;
	}

	//if m_bResponse is true or timeout < 180s continue
	while(!pFAVDlg->m_bResponse)
	{ 
		Sleep(1000);
		i_timeout = i_timeout + 1000;
		if ( pFAVDlg->m_bResponse || (i_timeout >= 180000) )
		{
			break;
		}
	}

	if (i_timeout >= 180000)	
	{ 
		AfxMessageBox(_T("FactoryReset.exe : WAIT TIMEOUT"));
	}
	else
	{  
		if(pFAVDlg->m_bResponse)
		{ 
			if(pFAVDlg->m_bFactoryReset)
			{   
				bool m_DBresult=false;

				m_DBresult =  pFAVDlg->m_obj_QueryCIM.InsertTestRecordToCIM(pFAVDlg->m_bFactoryReset);

				if (m_DBresult)
				{
					m_DBresult = pFAVDlg->m_obj_QueryCIM.InsertOOBDataToCIM("1");
				}

				if (!m_DBresult)
				{   
					AfxMessageBox(pFAVDlg->m_obj_QueryCIM.GetErrorMsg());
				}
				else
				{
					int iIndex = pFAVDlg->m_listResult.GetItemCount();
					pFAVDlg->m_listResult.InsertItem(iIndex, pFAVDlg->m_strSerialNo);
					pFAVDlg->m_listResult.SetItemText(iIndex, 1, _T("PASS"));
					pFAVDlg->m_listResult.EnsureVisible(iIndex, TRUE);
					pFAVDlg->m_listResult.SetItemColor(iIndex,RGB(0,255,0));
				}
			}
			else
			{
				AfxMessageBox(_T("Factory Reset Fail,OP选择是否插入Fail记录"));
			}
		}
		else
		{
			AfxMessageBox(_T("FactoryReset.exe : No Message Recv"));
		}
	}

	i_timeout = 0;
	pFAVDlg->ResetForReDO();
	return 0;
}