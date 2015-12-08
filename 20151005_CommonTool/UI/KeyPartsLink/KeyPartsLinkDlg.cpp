
// KeyPartsLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KeyPartsLink.h"
#include "KeyPartsLinkDlg.h"
#include "..\..\Modules\Common\PrintLabel\PrintLabel.h"

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


// CKeyPartsLinkDlg dialog




CKeyPartsLinkDlg::CKeyPartsLinkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyPartsLinkDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_sz_Process,0,sizeof(m_sz_Process));
	b_DoRemove=true;
}

void CKeyPartsLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STATIC_TITLE, m_LTitle);
	DDX_Control(pDX, IDC_STATIC_SHOWPASS, m_LShowPass);
	DDX_Control(pDX, IDC_COMBO_PROCESS, m_CBProcessSelect);
    DDX_Control(pDX, IDC_COMBO_Index, m_CBIndexSelect);
	DDX_Control(pDX, IDC_EDIT_SN, m_Edit_SN);
	DDX_Control(pDX, IDC_EDIT_KEYPARTBARCODE, m_Edit_KeyPartBarcode);
	DDX_Control(pDX, IDC_LIST_KEYPART, m_LC_KeyPartList);
	DDX_Control(pDX, IDC_LIST_RESULT, m_LC_Result);
	DDX_Control(pDX, IDC_LIST_PTKPWIPLIST, m_UN_Link);
	DDX_Control(pDX, IDC_EDIT_UNLINK_SN, m_Link_Text);
}

BEGIN_MESSAGE_MAP(CKeyPartsLinkDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOCK, &CKeyPartsLinkDlg::OnBnClickedButtonLock)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_OCA_PATA, &CKeyPartsLinkDlg::OnBnClickedButtonEditOCAPara)
	ON_CBN_SELENDOK(IDC_COMBO_PROCESS, &CKeyPartsLinkDlg::OnCbnSelendokComboProcess)
	ON_BN_CLICKED(IDC_BUTTON_INQUIRY, &CKeyPartsLinkDlg::OnBnClickedButtonInquiry)
	ON_BN_CLICKED(IDC_BUTTON_UNLINK, &CKeyPartsLinkDlg::OnBnClickedButtonUnlink)
	ON_BN_CLICKED(IDC_CHECK_UNLINK, &CKeyPartsLinkDlg::OnBnClickedCheckUnLink)
END_MESSAGE_MAP()


// CKeyPartsLinkDlg message handlers

BOOL CKeyPartsLinkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	if (m_InitialDlg.DoModal() != IDOK)
	{
		exit(0);
	}

	/*m_LTitle.SetBkColor(RGB(34,132,224));
	m_LTitle.SetTextColor(RGB(255,255,255));
	m_LTitle.SetFontSize(22);
	m_LTitle.SetFontBold(true);
	CString strTitle = _T(" KEYPART LINK TOOL ");
	strTitle = strTitle + _T("(") + _T(TOOL_VER) + _T(")");
	m_LTitle.SetText(strTitle);*/
	InitialUITitle();

	m_LShowPass.SetTextColor(RGB(0,255,0));
	m_LShowPass.SetFontSize(30);
	m_LShowPass.SetFontBold(true);
	m_LShowPass.SetText(_T(""));

	((CStatic*)GetDlgItem(IDC_STATIC_MODELNAME))->SetWindowText(m_InitialDlg.m_strModelName);
	((CStatic*)GetDlgItem(IDC_STATIC_PARTNUMBER))->SetWindowText(m_InitialDlg.m_strPartNumber);
	((CStatic*)GetDlgItem(IDC_STATIC_EMPLOYEEID))->SetWindowText(m_InitialDlg.m_strEmployeeID);
	((CStatic*)GetDlgItem(IDC_STATIC_SONUMBER))->SetWindowText(m_InitialDlg.m_strSONumber);
	((CStatic*)GetDlgItem(IDC_STATIC_LINENAME))->SetWindowText(m_InitialDlg.m_strLineName);
	((CStatic*)GetDlgItem(IDC_STATIC_PCNAME))->SetWindowText(m_InitialDlg.m_strPCName);
	((CStatic*)GetDlgItem(IDC_STATIC_SEGMENT))->SetWindowText(m_InitialDlg.m_strSegment);
	//((CStatic*)GetDlgItem(IDC_STATIC_PROCESS))->SetWindowText(m_InitialDlg.m_strProcess);

	((CButton*)GetDlgItem(IDC_BUTTON_LOCK))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_EDIT_OCA_PATA))->EnableWindow(false);

	((CButton*)GetDlgItem(IDC_RADIO_ONLY_ONE))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_RADIO_ATLEAST_ONE))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK_PRINT))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK_REPRINTLABEL))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK_PRINT_PCBA_LABEL))->EnableWindow(false);

	if(m_InitialDlg.m_strProcess.Find(_T("FALINK"),0) != -1 || m_InitialDlg.m_strProcess.Find(_T("CSD_LINK"),0) != -1)
	{
		((CButton*)GetDlgItem(IDC_CHECK_PRINT))->SetCheck(BST_CHECKED);
	}
	if(m_InitialDlg.m_strProcess.Find(_T("FAPRINT"),0) != -1 || m_InitialDlg.m_strProcess.Find(_T("CSD_PRINT"),0) != -1)
	{
		((CButton*)GetDlgItem(IDC_CHECK_PRINT_PCBA_LABEL))->SetCheck(BST_CHECKED);
		m_LC_KeyPartList.EnableWindow(false);
		((CButton *)GetDlgItem(IDC_CHECK_UNLINK))->EnableWindow(false);
	}
	if (m_InitialDlg.m_strProcess.Find(_T("PPIDLINK"),0) != -1 && m_InitialDlg.m_strModelName.Mid(1,3) == "ORA")
		((CButton*)GetDlgItem(IDC_RADIO_ONLY_ONE))->SetCheck(BST_CHECKED);

	m_Edit_SN.EnableWindow(false);
	m_Edit_SN.SetLimitText(PPID_LENGTH);
	m_Edit_KeyPartBarcode.EnableWindow(false);
	m_Edit_KeyPartBarcode.SetLimitText(KEYPARTBARCODE_MAXLENGTH);
	//m_Result_Count = 0;

	/* ListCtrl */
    m_LC_KeyPartList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EDITLABELS | LVS_EX_INFOTIP);

	if(RUN_MODE_OFFLINE == true)
	{
		m_LC_KeyPartList.InsertColumn(0, _T("参数"), LVCFMT_LEFT, 120);
		m_LC_KeyPartList.InsertColumn(1, _T("值"), LVCFMT_LEFT,120);
		m_LC_KeyPartList.InsertColumn(2, _T("单位"), LVCFMT_LEFT,120);
	}
	else
	{
		m_LC_KeyPartList.InsertColumn(ITEM_INDEX, _T("Item"), LVCFMT_LEFT, 50);
		m_LC_KeyPartList.InsertColumn(PN_INDEX, _T("Part N.O."), LVCFMT_LEFT,90);
		m_LC_KeyPartList.InsertColumn(KPNAME_INDEX, _T("KeyPart Name"), LVCFMT_LEFT,120);
		m_LC_KeyPartList.InsertColumn(RULE_INDEX, _T("ID Rule"), LVCFMT_LEFT,160);
		m_LC_KeyPartList.InsertColumn(CHKDUPLIC_INDEX, _T("Dup Check"), LVCFMT_LEFT,80);
		m_LC_KeyPartList.InsertColumn(KEYPART_INDEX, _T("KeyPart ID"), LVCFMT_LEFT,160);
		m_LC_KeyPartList.InsertColumn(RESULT_INDEX, _T("Result"), LVCFMT_LEFT,50);
	}

    m_LC_Result.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EDITLABELS | LVS_EX_INFOTIP);
	if((m_InitialDlg.m_strProcess.Find(_T("FALINK"),0) != -1) || (m_InitialDlg.m_strProcess.Find(_T("CSD_LINK"),0) != -1) || (m_InitialDlg.m_strProcess.Find(_T("PPIDLINK"),0) != -1))
	{
		m_LC_Result.InsertColumn(0, _T("Serial N.O."), LVCFMT_LEFT, 110);
		m_LC_Result.InsertColumn(1, _T("Segment"), LVCFMT_LEFT,80);
		m_LC_Result.InsertColumn(2, _T("Process"), LVCFMT_LEFT,60);
		m_LC_Result.InsertColumn(3, _T("Result"), LVCFMT_LEFT,50);
	}
	if(m_InitialDlg.m_strProcess.Find(_T("FAPRINT"),0) != -1 || m_InitialDlg.m_strProcess.Find(_T("CSD_PRINT"),0) != -1 )
	{
		m_LC_Result.InsertColumn(0, _T("Serial N.O."), LVCFMT_LEFT, 110);
		m_LC_Result.InsertColumn(1, _T("PCBA N.O."), LVCFMT_LEFT,190);
	}

	if(RUN_MODE_OFFLINE == true)
	{
		m_LC_Result.InsertColumn(0, _T("LCDM 条码"), LVCFMT_LEFT, 190);
		m_LC_Result.InsertColumn(1, _T("结果"), LVCFMT_LEFT,110);
	}
	// Add by Fox 
	m_UN_Link.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EDITLABELS | LVS_EX_INFOTIP);
	m_UN_Link.InsertColumn(0, _T("Index"), LVCFMT_LEFT, 50);
    m_UN_Link.InsertColumn(1, _T("Picasso"), LVCFMT_LEFT,110);
	m_UN_Link.InsertColumn(2, _T("IMEI"), LVCFMT_LEFT,150);
	m_UN_Link.InsertColumn(3, _T("KeyPart ID"), LVCFMT_LEFT,110);
	m_UN_Link.InsertColumn(4, _T("KeyPart Name"), LVCFMT_LEFT,90);
	m_UN_Link.InsertColumn(5, _T("Keypart PtNO"), LVCFMT_LEFT,150);
	m_UN_Link.InsertColumn(6, _T("Result"), LVCFMT_LEFT,50);
	//CString str_PCname = _T("");
	char sz_Line[32]={0};
	char sz_Process[32]={0};
	char sz_Segment[32]={0};
	char sz_PCName[512]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};
    //m_InitialDlg.GetWorkStationName(str_PCname);
	//str_PCname = _T("P05C01");    //test code  release need mark
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strPCName.GetBuffer(),-1,sz_PCName,m_InitialDlg.m_strPCName.GetLength(),NULL, NULL);
	if(RUN_MODE_OFFLINE == false)
	{
		bool DBresult = theApp.pQueryStation_Info(sz_PCName,sz_Line,sz_Process,sz_Segment,sz_Errorcode,sz_ErrMsg);
	}
    strcpy(m_sz_Process,sz_Process);
	if(strstr(sz_Process,"PPIDLINK")==NULL&&strstr(sz_Process,"FALINK")==NULL&&strstr(sz_Process,"CSD_LINK")==NULL)
	{   
		m_UN_Link.EnableWindow(false);
        m_CBIndexSelect.EnableWindow(false);
		m_Link_Text.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_INQUIRY)->EnableWindow(false);
        GetDlgItem(IDC_BUTTON_UNLINK)->EnableWindow(false);
	}
	else
	{
       
		m_UN_Link.EnableWindow(false);
		m_CBIndexSelect.EnableWindow(false);
		m_Link_Text.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_INQUIRY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_UNLINK)->EnableWindow(false);

	}
	m_CBProcessSelect.ResetContent();
	m_CBProcessSelect.SetFocus();
	if ( m_InitialDlg.m_b_IsUsedEntranceTool == false && RUN_MODE_OFFLINE == false )
	{
		for(unsigned int i = 0; i< m_InitialDlg.m_vProcessSelect.size();i++)
			m_CBProcessSelect.AddString(m_InitialDlg.m_vProcessSelect[i]);
	}
	else
	{
		m_CBProcessSelect.AddString(m_InitialDlg.m_strProcess);
		m_CBProcessSelect.SelectString(0,m_InitialDlg.m_strProcess);
		OnCbnSelendokComboProcess();
		m_CBProcessSelect.EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_LOCK))->EnableWindow(true);
	}
    m_CBIndexSelect.AddString(_T("PICASSO"));
	m_CBIndexSelect.AddString(_T("IMEI"));
	//ResetForReDO();
	
	if(RUN_MODE_OFFLINE == true)
	{
		((CButton *)GetDlgItem(IDC_CHECK_UNLINK))->EnableWindow(false);
		((CButton*)GetDlgItem(IDC_BUTTON_LOCK))->EnableWindow(false);
		((CButton*)GetDlgItem(IDC_BUTTON_EDIT_OCA_PATA))->EnableWindow(true);
	}
	if(RUN_MODE_OFFLINE == true)
	{
		SYSTEMTIME time;
		::GetLocalTime(&time);

		m_strLogFileName.Format(_T("%s_%04d%02d%02d%02d%02d%02d.log "),
			m_InitialDlg.m_strModelName,
			time.wYear, time.wMonth,  time.wDay,
			time.wHour, time.wMinute, time.wSecond);
	}

	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CKeyPartsLinkDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKeyPartsLinkDlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKeyPartsLinkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CKeyPartsLinkDlg::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message != WM_KEYDOWN)||(pMsg->wParam != VK_RETURN))
		return false;
    if(((CButton *)GetDlgItem(IDC_CHECK_UNLINK))->GetCheck()==1)
	{
		return false;
	}

	m_CBProcessSelect.GetWindowTextW(m_strProcessSelect);
	if(m_strProcessSelect.GetLength() == 0)
	{
		OnBnClickedButtonLock();
		return false;
	}

	if (((CButton*)GetDlgItem(IDC_BUTTON_LOCK))->IsWindowEnabled())
	{
		OnBnClickedButtonLock();
		return false;
	}

	if(RUN_MODE_OFFLINE == true)
	{
		m_Edit_KeyPartBarcode.EnableWindow(false);
		m_Edit_KeyPartBarcode.GetWindowTextW(m_strKeyPartBarcode);
		m_LShowPass.SetText(_T(""));
		if(m_strKeyPartBarcode.GetLength() > KEYPARTBARCODE_MAXLENGTH || m_strKeyPartBarcode.GetLength() < KEYPARTBARCODE_MINLENGTH)
		{
			ResetForReDO();
			return false;
		}

		for(int i = 0;i < m_strKeyPartBarcode.GetLength();i++)
		{
			if(m_strKeyPartBarcode.GetAt(i) >= '0' && m_strKeyPartBarcode.GetAt(i) <= '9')
				continue;
			if(m_strKeyPartBarcode.GetAt(i) >= 'A' && m_strKeyPartBarcode.GetAt(i) <= 'Z')
				continue;
			if(m_strKeyPartBarcode.GetAt(i) >= 'a' && m_strKeyPartBarcode.GetAt(i) <= 'z')
				continue;
			//AfxMessageBox(_T("Illegal char in barcode! -> ") + m_strKeyPartBarcode.GetAt(i));
			AfxMessageBox(_T("Illegal char in barcode!") );
			ResetForReDO();
			return false;
		}

		m_LC_Result.InsertItem(0,_T(""));
		m_LC_Result.SetItemText(0, 0,m_strKeyPartBarcode );//设置行索引（行号）和列索引（列号）的值
		m_LC_Result.SetItemText(0, 1,_T("OK"));//设置行索引（行号）和列索引（列号）的值

		// Save Log in Local Disk;
		CString str_log;
		str_log.Format(_T("RollerPressure:%s,Z_AxisHeight:%s,MoundLayerHeight:%s,%s,%s,%s"),
			m_OCA_Para_Setting.m_strRollerPressure,
			m_OCA_Para_Setting.m_strZ_AxisHeight,
			m_OCA_Para_Setting.m_strMoundLayerHeight,
			m_InitialDlg.m_strSONumber,
			m_InitialDlg.m_strPartNumber,
			m_strKeyPartBarcode);
		theApp.SaveLog(m_strLogFileName,str_log);

		//Only Dispay the latest 10 records
		if ( m_LC_Result.GetItemCount() > 10 )
			m_LC_Result.DeleteItem(m_LC_Result.GetItemCount()-1);
		m_LShowPass.SetTextColor(RGB(0,255,0));
		m_LShowPass.SetText(_T("PASS"));
		ResetForReDO();
		return false;
	}

	if(m_strPicassoNumber.GetLength() == 0)
	{
		m_LShowPass.SetText(_T(""));
		if(!CheckSNBarCodeAndGetPicasso())
			return false;
		if(!CheckRoutingCurrentStation())
			return false;

		// Add for FAPRINT Station to print PCBA Label
		if (((CButton*)GetDlgItem(IDC_CHECK_PRINT_PCBA_LABEL))->GetCheck())
		{
			if( GenerateAndPrintPCBALabel() )
			{
				m_LShowPass.SetTextColor(RGB(0,255,0));
				m_LShowPass.SetText(_T("PASS"));
			}
			else
			{
				m_LShowPass.SetTextColor(RGB(255,0,0));
				m_LShowPass.SetText(_T("FAIL"));
			}
			ExecuteOperationsIfCompleted();
			ResetForReDO();
			return false;
		}

		// Add Below is just for Re_Print FA Tracking Label
		if (((CButton*)GetDlgItem(IDC_CHECK_REPRINTLABEL))->GetCheck())
		{
			if(CheckRoutingNextStation() && DoPrintTravelLabel())
			{
				m_LShowPass.SetTextColor(RGB(0,255,0));
				m_LShowPass.SetText(_T("PASS"));
			}
			else
			{
				m_LShowPass.SetTextColor(RGB(255,0,0));
				m_LShowPass.SetText(_T("FAIL"));
			}
			ResetForReDO();
			return false;
		}

		// Add Below is for Check ptKPWIP before Scanning Accessory
		if (!CheckPTKPWIP_BeforeScan())
		{
			m_LShowPass.SetTextColor(RGB(255,0,0));
			m_LShowPass.SetText(_T("FAIL"));
			ResetForReDO();
			return false;
		}


		ResetForScanAcessoryBarcode();

		// Add Below is for reducing one time scanning for picasso when picasso itself is one keypart in the list 
		for(unsigned int i = 0 ; i< m_InitialDlg.m_vKeyPartInfo.size();i++)
		{
			if(KeyPartIDRuleCompare(m_strPicassoNumber,m_LC_KeyPartList.GetItemText(i,RULE_INDEX)))
			{
				m_Edit_KeyPartBarcode.SetWindowTextW(m_strPicassoNumber);
				CheckAndDealWithKeyPartBarcode();
				ExecuteOperationsIfCompleted();
			}
		}
		return false;
	}

	CheckAndDealWithKeyPartBarcode();
	ExecuteOperationsIfCompleted();
	return false;
	//return CDialog::PreTranslateMessage(pMsg);
}

void CKeyPartsLinkDlg::OnBnClickedButtonLock()
{
	// TODO: Add your control notification handler code here




	for(unsigned int i = 0; i< m_InitialDlg.m_vKeyPartInfo.size();i++)
	{
		if (m_InitialDlg.m_vKeyPartInfo[i].bExistInSOBOM == true && 
			m_InitialDlg.m_vKeyPartInfo[i].strKP_Name.CompareNoCase(_T("NAL")) == 0)
		{
			if (!Initial_SSSO2Dll())
			{
				AfxMessageBox(_T("Initial_SSSO2Dll Fail!\
								 \n	Config Step:\
								 \n		Run 1. \\\\10.85.58.13\\EasyUpdate\\CIMPortal\\xxxConfig.bat\
								 \n		Run 2. \\\\10.85.58.13\\EasyUpdate\\CIMPortal\\system parameter.reg\
								 \n		(This tool would regsvr32 SSS_O2.Dll automatically,Manual Reg SSS_O2.Dll is not neccessary)"));
				return;
			}
			else
			{
				// SSSO2 Function Test
				//bool b_res;
				//b_res = SSSO2_UpdateNALEx(_T("865921010044098"),_T("124124000011770"));
				break;
			}
		}

	}

	m_CBProcessSelect.GetWindowTextW(m_strProcessSelect);
	if(m_strProcessSelect.GetLength() == 0)
	{
		AfxMessageBox(_T("Please Select Process!"));
		return;
	}
	if (m_strProcessSelect != m_InitialDlg.m_strProcess)
	{
		AfxMessageBox(_T("The Process of this WorkStation is set as ") + m_InitialDlg.m_strProcess  + _T(" in CIM System!"));
		return;
	}

	if ( m_InitialDlg.m_b_IsUsedEntranceTool == false && RUN_MODE_OFFLINE == false )
	{
		if(!UpdateEmployeeID_StatusEx(m_InitialDlg.m_strEmployeeID, _T("N"),m_InitialDlg.m_strPCName))
		{
			return;
		}
	}

	((CButton*)GetDlgItem(IDC_CHECK_PRINT))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK_REPRINTLABEL))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK_PRINT_PCBA_LABEL))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_LOCK))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_LOCK))->SetButtonStyle(BS_PUSHBUTTON);

	if(RUN_MODE_OFFLINE == true)
	{
		((CButton*)GetDlgItem(IDC_BUTTON_EDIT_OCA_PATA))->EnableWindow(false);
		((CButton*)GetDlgItem(IDC_BUTTON_EDIT_OCA_PATA))->SetButtonStyle(BS_PUSHBUTTON);
	}

	m_CBProcessSelect.EnableWindow(false);
	((CButton*)GetDlgItem(IDC_RADIO_ONLY_ONE))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_RADIO_ATLEAST_ONE))->EnableWindow(false);
	ResetForReDO();
}

void CKeyPartsLinkDlg::OnBnClickedButtonEditOCAPara()
{
	// TODO: Add your control notification handler code here
	if (m_OCA_Para_Setting.DoModal() != IDOK)
	{
		((CButton*)GetDlgItem(IDC_BUTTON_LOCK))->EnableWindow(false);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_BUTTON_LOCK))->EnableWindow(true);
		m_LC_KeyPartList.DeleteAllItems();
		m_LC_KeyPartList.InsertItem(0,_T("滚轮压力"));
		m_LC_KeyPartList.InsertItem(1,_T("Z轴高度"));
		m_LC_KeyPartList.InsertItem(2,_T("压条高度"));

		m_LC_KeyPartList.SetItemText(0,1, m_OCA_Para_Setting.m_strRollerPressure);
		m_LC_KeyPartList.SetItemText(1,1, m_OCA_Para_Setting.m_strZ_AxisHeight);
		m_LC_KeyPartList.SetItemText(2,1, m_OCA_Para_Setting.m_strMoundLayerHeight);

		m_LC_KeyPartList.SetItemText(0,2, _T("kg/cm*cm"));
		m_LC_KeyPartList.SetItemText(1,2, _T("mm"));
		m_LC_KeyPartList.SetItemText(2,2, _T("mm"));
	}
}

void CKeyPartsLinkDlg::ResetForScanAcessoryBarcode(void)
{
	m_Edit_KeyPartBarcode.EnableWindow(true);
	m_Edit_KeyPartBarcode.SetWindowTextW(_T(""));
	m_strKeyPartBarcode.Empty();
	m_Edit_KeyPartBarcode.SetFocus();
}

void CKeyPartsLinkDlg::ResetForReDO()
{
	// TODO: Add your control notification handler code here

	if(RUN_MODE_OFFLINE == true)
	{
		m_Edit_SN.EnableWindow(false);
		m_Edit_KeyPartBarcode.EnableWindow(true);
		m_Edit_KeyPartBarcode.SetWindowTextW(_T(""));
		m_Edit_KeyPartBarcode.SetFocus();

		m_LC_KeyPartList.DeleteAllItems();
		m_LC_KeyPartList.InsertItem(0,_T("滚轮压力"));
		m_LC_KeyPartList.InsertItem(1,_T("Z轴高度"));
		m_LC_KeyPartList.InsertItem(2,_T("压条高度"));

		m_LC_KeyPartList.SetItemText(0,1, m_OCA_Para_Setting.m_strRollerPressure);
		m_LC_KeyPartList.SetItemText(1,1, m_OCA_Para_Setting.m_strZ_AxisHeight);
		m_LC_KeyPartList.SetItemText(2,1, m_OCA_Para_Setting.m_strMoundLayerHeight);

		m_LC_KeyPartList.SetItemText(0,2, _T("kg/cm*cm"));
		m_LC_KeyPartList.SetItemText(1,2, _T("mm"));
		m_LC_KeyPartList.SetItemText(2,2, _T("mm"));
		return;
	}

	m_strSerialNo.Empty();
	m_strPicassoNumber.Empty();

	m_strKeyPartBarcode.Empty();

	m_BatteryCount_InBACH = 0;
	m_BatteryCount_Scanned = 0;
	m_BatteryCount_Scanned_PassRunIn = 0;

	m_Edit_SN.EnableWindow(true);
	m_Edit_SN.SetFocus();
	m_Edit_SN.SetWindowTextW(_T(""));

	m_Edit_KeyPartBarcode.EnableWindow(false);

	if( m_InitialDlg.m_strProcess.Find(_T("FAPRINT"),0) == -1 && m_InitialDlg.m_strProcess.Find(_T("CSD_PRINT"),0) == -1)
		m_Edit_KeyPartBarcode.SetWindowTextW(_T(""));

	//Empty Result Column for Re-Scan
	for(unsigned int i = 0 ; i< m_InitialDlg.m_vKeyPartInfo.size();i++)
	{
		m_LC_KeyPartList.SetItemColor(i,RGB(255,255,255));
		m_LC_KeyPartList.SetItemText(i,KEYPART_INDEX, _T(""));
		m_LC_KeyPartList.SetItemText(i,RESULT_INDEX, _T(""));
	}
	m_LC_KeyPartList.DeleteAllItems();
	TCHAR itemID[32];
	for(unsigned int i = 0; i< m_InitialDlg.m_vKeyPartInfo.size();i++)
	{
		wsprintf(itemID,_T("%d"), i+1);
		m_LC_KeyPartList.InsertItem(i,itemID);
		m_LC_KeyPartList.SetItemText(i, PN_INDEX, m_InitialDlg.m_vKeyPartInfo[i].strKP_Part_Number);//设置行索引（行号）和列索引（列号）的值
		m_LC_KeyPartList.SetItemText(i, KPNAME_INDEX, m_InitialDlg.m_vKeyPartInfo[i].strKP_Name);
		m_LC_KeyPartList.SetItemText(i, RULE_INDEX, m_InitialDlg.m_vKeyPartInfo[i].strKP_ID_Rule);
		m_LC_KeyPartList.SetItemText(i, CHKDUPLIC_INDEX, m_InitialDlg.m_vKeyPartInfo[i].strChkDuplic);
		m_LC_KeyPartList.SetItemText(i, KEYPART_INDEX, _T(""));//设置行索引（行号）和列索引（列号）的值
		m_LC_KeyPartList.SetItemText(i, RESULT_INDEX, _T(""));//设置行索引（行号）和列索引（列号）的值

		if (m_InitialDlg.m_vKeyPartInfo[i].bExistInSOBOM == false)
		{
			m_LC_KeyPartList.SetItemColor(i,RGB(GRAY_NA,GRAY_NA,GRAY_NA));
			m_LC_KeyPartList.SetItemText(i, RESULT_INDEX, _T("N/A"));
		}
		
		if (m_InitialDlg.m_vKeyPartInfo[i].bExistInSOBOM == true)
		{
			for(unsigned int j = 0; j< m_InitialDlg.m_vKeyPartInfo.size();j++)
			{
				if ( j == i )
					continue;
				if (m_InitialDlg.m_vKeyPartInfo[j].bExistInSOBOM == true &&
					m_InitialDlg.m_vKeyPartInfo[j].strKP_Part_Number ==  m_InitialDlg.m_vKeyPartInfo[i].strKP_Part_Number &&
					m_InitialDlg.m_vKeyPartInfo[j].strKP_Name ==  m_InitialDlg.m_vKeyPartInfo[i].strKP_Name
					)
				{
					m_LC_KeyPartList.SetItemColor(j,RGB(GRAY_MUTEX,GRAY_MUTEX,GRAY_MUTEX));
					m_LC_KeyPartList.SetItemColor(i,RGB(GRAY_MUTEX,GRAY_MUTEX,GRAY_MUTEX));
				}
			}
		}
		if (m_InitialDlg.m_vKeyPartInfo[i].strKP_ID_Rule.Find(_T("11S31052573ZZ0PA"),0)>= 0)
			m_BatteryCount_InBACH++;

	}
}
bool CKeyPartsLinkDlg::KeyPartIDRuleCompare(CString strKeyPartBarcode, CString strKP_ID_Rule)
{
	if(strKeyPartBarcode.GetLength() == 0 || strKeyPartBarcode.GetLength() !=strKP_ID_Rule.GetLength())
		return false;
	/*
	for(unsigned int i = 0; i < strKeyPartBarcode.GetLength() ; i++)
	{
		if(strKP_ID_Rule.Mid(i,1) != _T("*") && strKeyPartBarcode.Mid(i,1) != strKP_ID_Rule.Mid(i,1))
			return false;
	}
	return true;
	*/

	for(unsigned int i = 0; i < strKeyPartBarcode.GetLength() ; i++)
	{
		if(strKP_ID_Rule.Mid(i,1) == _T("*") )
		{
			if( strKeyPartBarcode.GetAt(i) >= '0' && strKeyPartBarcode.GetAt(i) <= '9')
				continue;
			if( strKeyPartBarcode.GetAt(i) >= 'A' && strKeyPartBarcode.GetAt(i) <= 'Z')
				continue;
			if( strKeyPartBarcode.GetAt(i) >= 'a' && strKeyPartBarcode.GetAt(i) <= 'z')
				continue;
			return false;
		}
		else
		{
			if(strKeyPartBarcode.Mid(i,1) != strKP_ID_Rule.Mid(i,1))
				return false;
		}
	}

	return true;

	/*CString strRule=strKP_ID_Rule;
	strRule.TrimLeft(_T("*"));
	strRule.TrimRight(_T("*"));

	if (strRule.Find(_T("*"),0)>= 0)
	{
		AfxMessageBox(_T("KeyPart ID Rule Abnormal! Rule = " + strKP_ID_Rule));
		return false;
	}

	if(strKeyPartBarcode.Find(strRule,0) < 0)
		return false;
	return true;*/
}

bool CKeyPartsLinkDlg::SetValueAndInsertPASSData(void)
{
	//Base on \DB\XML\*.XML Item
	char sz_Picasso[32]={0};
	char sz_SerialNo[32]={0};
	char sz_IMEI[32]={0};
	char sz_szSONumber[32]={0};
	char sz_SoCRtype[32]={0};
	char sz_EmployId[32]={0};

	char sz_Model[32]={0};
	char sz_PN[32]={0};
	char sz_Process[32]={0};
	char sz_Segment[32]={0};
	//char sz_PCName[512]={0};

	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};
	char sz_Temp[512]={0};
	WCHAR WTemp[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,m_strPicassoNumber.GetBuffer(),-1,sz_Picasso,m_strPicassoNumber.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_strSerialNo.GetBuffer(),-1,sz_SerialNo,m_strSerialNo.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strSONumber.GetBuffer(),-1,sz_szSONumber,m_InitialDlg.m_strSONumber.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strSOCRtype.GetBuffer(),-1,sz_SoCRtype,m_InitialDlg.m_strSOCRtype.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strEmployeeID.GetBuffer(),-1,sz_EmployId,m_InitialDlg.m_strEmployeeID.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strModelName.GetBuffer(),-1,sz_Model,m_InitialDlg.m_strModelName.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strPartNumber.GetBuffer(),-1,sz_PN,m_InitialDlg.m_strPartNumber.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strProcess.GetBuffer(),-1,sz_Process,m_InitialDlg.m_strProcess.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strSegment.GetBuffer(),-1,sz_Segment,m_InitialDlg.m_strSegment.GetLength()*3,NULL, NULL);

	if (m_strSerialNo.GetLength() == IMEI_LENGTH)
		WideCharToMultiByte(CP_ACP,NULL,m_strSerialNo.GetBuffer(),-1,sz_IMEI,m_strSerialNo.GetLength()*3,NULL, NULL);

	// Insert PASS Date to pt Table
	char sz_SOCount[32]={0};
	WideCharToMultiByte(CP_ACP,NULL,m_strSOCount.GetBuffer(),-1,sz_SOCount,m_strSOCount.GetLength()*3,NULL, NULL);
	
	theApp.pSetValue("SerNo",sz_SOCount);
	if (m_strSerialNo.GetLength() != PICASSO_LENGTH)
		theApp.pSetValue("SN",sz_SerialNo);
	if (m_strSerialNo.GetLength() == PICASSO_LENGTH)
		theApp.pSetValue("SN","");
	theApp.pSetValue("PART_NUMBER",sz_PN);
	theApp.pSetValue("TOOL_VER",TOOL_VER);

	if (m_InitialDlg.m_strProcess.Find( _T("FAPRINT"),0) != -1 || m_InitialDlg.m_strProcess.Find( _T("CSD_PRINT"),0) != -1 ) 
	{
		ZeroMemory(sz_Temp,sizeof(sz_Temp));
		WideCharToMultiByte(CP_ACP,NULL,m_str_PCBA_barcode.GetBuffer(),-1,sz_Temp,m_str_PCBA_barcode.GetLength()*3,NULL, NULL);
		theApp.pSetValue("acerPCBA",sz_Temp);
	}

	if ( (m_InitialDlg.m_strProcess.Find(_T("FALINK"),0) != -1) || (m_InitialDlg.m_strProcess.Find(_T("CSD_LINK"),0) != -1) || (m_InitialDlg.m_strProcess.Find(_T("PPIDLINK"),0) != -1) ) 
	{
		for (unsigned int i = 1 ; i <= 30 ; i ++)
		{
			ZeroMemory(sz_Temp,sizeof(sz_Temp));
			if (i <= m_InitialDlg.m_vKeyPartInfo.size())
				WideCharToMultiByte(CP_ACP,NULL,m_LC_KeyPartList.GetItemText(i-1,KEYPART_INDEX).GetBuffer(),-1,sz_Temp,m_LC_KeyPartList.GetItemText(i-1,KEYPART_INDEX).GetLength(),NULL, NULL);
			if (i >  m_InitialDlg.m_vKeyPartInfo.size())
				ZeroMemory(sz_Temp,sizeof(sz_Temp));
			if (i == 1)
				theApp.pSetValue("KeyPart1",sz_Temp);
			if (i == 2)
				theApp.pSetValue("KeyPart2",sz_Temp);
			if (i == 3)
				theApp.pSetValue("KeyPart3",sz_Temp);
			if (i == 4)
				theApp.pSetValue("KeyPart4",sz_Temp);
			if (i == 5)
				theApp.pSetValue("KeyPart5",sz_Temp);
			if (i == 6)
				theApp.pSetValue("KeyPart6",sz_Temp);
			if (i == 7)
				theApp.pSetValue("KeyPart7",sz_Temp);
			if (i == 8)
				theApp.pSetValue("KeyPart8",sz_Temp);
			if (i == 9)
				theApp.pSetValue("KeyPart9",sz_Temp);
			if (i == 10)
				theApp.pSetValue("KeyPart10",sz_Temp);
			if (i == 11)
				theApp.pSetValue("KeyPart11",sz_Temp);
			if (i == 12)
				theApp.pSetValue("KeyPart12",sz_Temp);
			if (i == 13)
				theApp.pSetValue("KeyPart13",sz_Temp);
			if (i == 14)
				theApp.pSetValue("KeyPart14",sz_Temp);
			if (i == 15)
				theApp.pSetValue("KeyPart15",sz_Temp);
			if (m_InitialDlg.m_strProcess.Find(_T("PPIDLINK"),0) != -1) 
			{
				if (i == 16)
					theApp.pSetValue("KeyPart16",sz_Temp);
				if (i == 17)
					theApp.pSetValue("KeyPart17",sz_Temp);
				if (i == 18)
					theApp.pSetValue("KeyPart18",sz_Temp);
				if (i == 19)
					theApp.pSetValue("KeyPart19",sz_Temp);
				if (i == 20)
					theApp.pSetValue("KeyPart20",sz_Temp);
				if (i == 21)
					theApp.pSetValue("KeyPart21",sz_Temp);
				if (i == 22)
					theApp.pSetValue("KeyPart22",sz_Temp);
				if (i == 23)
					theApp.pSetValue("KeyPart23",sz_Temp);
				if (i == 24)
					theApp.pSetValue("KeyPart24",sz_Temp);
				if (i == 25)
					theApp.pSetValue("KeyPart25",sz_Temp);
				if (i == 26)
					theApp.pSetValue("KeyPart26",sz_Temp);
				if (i == 27)
					theApp.pSetValue("KeyPart27",sz_Temp);
				if (i == 28)
					theApp.pSetValue("KeyPart28",sz_Temp);
				if (i == 29)
					theApp.pSetValue("KeyPart29",sz_Temp);
				if (i == 30)
					theApp.pSetValue("KeyPart30",sz_Temp);
			}
		}
	}
	if(m_InitialDlg.m_strSOCRtype  != _T("")) //"B" or "C"
	{
		// The DB pInsertCRHoldInfo is just for the first process for each segment.
		// and no need to check current station if it is the first process. 
		// Remark by Lennon 2012.9.11
		if(!theApp.pInsertCRHoldInfo(sz_Picasso,sz_IMEI,sz_szSONumber,sz_SoCRtype,sz_Model,sz_Process,sz_Segment,sz_EmployId,sz_PN,sz_Errorcode,sz_ErrMsg))
		{
			CString strErrorcode = _T("");
			CString strErrorMsg = _T("");
			theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
			theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
			AfxMessageBox(_T("pInsertPASS_data Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
			return false;
		}
	}

	if (!theApp.pInsertPASS_data(sz_Picasso,sz_szSONumber,sz_EmployId,sz_Errorcode,sz_ErrMsg))
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pInsertPASS_data Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	return true;
}

bool CKeyPartsLinkDlg::CheckRoutingCurrentStation(void)
{
	//Check Routing Start!
	if (!CheckRoutingEx(m_strPicassoNumber,m_InitialDlg.m_strModelName,m_InitialDlg.m_strProcess))
	{
		m_LShowPass.SetTextColor(RGB(255,0,0));
		m_LShowPass.SetText(_T("FAIL"));
		ResetForReDO();
		return false;
	}
	
	if ( m_InitialDlg.m_strSegment == _T("PACKAGE") )
	{
		if (!CheckProcessTestDateEx(m_strPicassoNumber,m_InitialDlg.m_strModelName,m_InitialDlg.m_strSegment,m_InitialDlg.m_strProcess))
		{
			m_LShowPass.SetTextColor(RGB(255,0,0));
			m_LShowPass.SetText(_T("FAIL"));
			ResetForReDO();
			return false;
		}
	}

	ResetForScanAcessoryBarcode();
	return true;
}

bool CKeyPartsLinkDlg::CheckRoutingEx(CString strPicassoNumber,CString strModelName,CString strProcess)
{
	//Check Routing Start!
	char sz_id[32]={0};
	char sz_Model[32]={0};
	char sz_Process[32]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};
	bool DBresult = false;

	CString strModelName_Normal;
	ConvertModelName( strModelName,strModelName_Normal );

	std::vector <CString> vect_Processes_CurSeg;
	if(!m_InitialDlg.QuerySegmentProcessEx( strModelName,m_InitialDlg.m_strSegment,vect_Processes_CurSeg))
			return false;

	if ((strModelName != strModelName_Normal) && (m_strProcessSelect == vect_Processes_CurSeg[0]))//CS SO
	{
		std::vector <CString> vect_Processes;
		CString strFAFirstProcess;
		if(!m_InitialDlg.QuerySegmentProcessEx( strModelName_Normal, _T("FA"),vect_Processes))
			return false;
		strFAFirstProcess = vect_Processes[0];

		WideCharToMultiByte(CP_ACP,NULL,strPicassoNumber.GetBuffer(),   -1,sz_id,        strPicassoNumber.GetLength()*3, NULL, NULL);
		WideCharToMultiByte(CP_ACP,NULL,strModelName_Normal.GetBuffer(),-1,sz_Model,  strModelName_Normal.GetLength()*3, NULL, NULL);
		WideCharToMultiByte(CP_ACP,NULL,strFAFirstProcess.GetBuffer(),  -1,sz_Process,  strFAFirstProcess.GetLength()*3, NULL, NULL);
		
		DBresult = theApp.pCheckRouting_New(sz_id,sz_Model,sz_Process,sz_Errorcode,sz_ErrMsg);
		if (!DBresult)
		{
			CString strErrorcode = _T("");
			CString strErrorMsg = _T("");
			theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
			theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
			AfxMessageBox(_T("pCheckRouting_new! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
			return false;
		}
		//return true;
	}

	WideCharToMultiByte(CP_ACP,NULL,strPicassoNumber.GetBuffer(), -1,sz_id,     strPicassoNumber.GetLength()*3, NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strModelName.GetBuffer(),-1,sz_Model,  strModelName.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strProcess.GetBuffer(),  -1,sz_Process,strProcess.GetLength()*3,  NULL, NULL);

	DBresult = theApp.pCheckRouting_New(sz_id,sz_Model,sz_Process,sz_Errorcode,sz_ErrMsg);
	//DBresult = true; // Just for Test
	if (!DBresult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pCheckRouting_new! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	return true;
}

void CKeyPartsLinkDlg::CheckAndDealWithKeyPartBarcode(void)
{
	m_Edit_KeyPartBarcode.EnableWindow(false);
	m_Edit_KeyPartBarcode.GetWindowTextW(m_strKeyPartBarcode);

	if(m_strKeyPartBarcode.GetLength() > KEYPARTBARCODE_MAXLENGTH || m_strKeyPartBarcode.GetLength() < KEYPARTBARCODE_MINLENGTH)
	{
		ResetForScanAcessoryBarcode();
		return;
	}

	if(m_strKeyPartBarcode.Find(_T("\'"),0) != -1)
	{
		AfxMessageBox(_T("Illegal char in barcode! -> \'"));
		ResetForScanAcessoryBarcode();
		return;
	}

	if(m_strKeyPartBarcode.Find(_T("\""),0) != -1)
	{
		AfxMessageBox(_T("Illegal char in barcode! -> \""));
		ResetForScanAcessoryBarcode();
		return;
	}

	if(m_strKeyPartBarcode.Find(_T("\("),0) != -1)
	{
		AfxMessageBox(_T("Illegal char in barcode! -> \("));
		ResetForScanAcessoryBarcode();
		return;
	}

	if(m_strKeyPartBarcode.Find(_T("\)"),0) != -1)
	{
		AfxMessageBox(_T("Illegal char in barcode! -> \)"));
		ResetForScanAcessoryBarcode();
		return;
	}

	// If the KeyPart Barcode include SN, The Barcode can't be dulplicated for each picasso ID 
	for(unsigned int i = 0 ; i< m_InitialDlg.m_vKeyPartInfo.size();i++)
	{
		if (m_strKeyPartBarcode.GetLength() < KEYPARTBARCODE_MINLENGTH_WITHSN)
			break;
		if (m_strKeyPartBarcode == m_LC_KeyPartList.GetItemText(i,KEYPART_INDEX))
		{
			AfxMessageBox(_T("The KeyPart:" + m_strKeyPartBarcode + " was scanned 2 times, please rescan!"));
			ResetForScanAcessoryBarcode();
			return;
		}
	}

	for(unsigned int i = 0 ; i< m_InitialDlg.m_vKeyPartInfo.size();i++)
	{
		CString strKeyPartPartNumber;
		if(KeyPartIDRuleCompare(m_strKeyPartBarcode,m_LC_KeyPartList.GetItemText(i,RULE_INDEX)))
			strKeyPartPartNumber = m_InitialDlg.m_vKeyPartInfo[i].strKP_Part_Number;
		else
			continue;

		if( m_LC_KeyPartList.GetItemText(i,RESULT_INDEX) == _T("PASS") || m_LC_KeyPartList.GetItemText(i,RESULT_INDEX) == _T("N/A"))
		{
			bool go_on = false;
			for(unsigned int j = i+1; j< m_InitialDlg.m_vKeyPartInfo.size();j++)
			{
				if (strKeyPartPartNumber == m_LC_KeyPartList.GetItemText(j,PN_INDEX) && m_LC_KeyPartList.GetItemText(j,RESULT_INDEX) != _T("PASS") && m_LC_KeyPartList.GetItemText(j,RESULT_INDEX) != _T("N/A"))
				{
					go_on=true;
					break;
				}
			}
			if (go_on == false)
			{
				if (m_LC_KeyPartList.GetItemText(i,RESULT_INDEX) == _T("PASS"))
					AfxMessageBox(_T("The KeyPart:" +m_strKeyPartBarcode + "(" + strKeyPartPartNumber+") is dulplicate with " + m_LC_KeyPartList.GetItemText(i,KEYPART_INDEX)));
				if (m_LC_KeyPartList.GetItemText(i,RESULT_INDEX) == _T("N/A"))
					AfxMessageBox(_T("The KeyPart:" +m_strKeyPartBarcode + "(" + strKeyPartPartNumber+") is N/A in SO BOM!" ));
				ResetForScanAcessoryBarcode();
				return;
			}
		}

		if( m_LC_KeyPartList.GetItemText(i,RESULT_INDEX) != _T("PASS") && m_LC_KeyPartList.GetItemText(i,RESULT_INDEX) != _T("N/A")) 
		{
			//Old models no need to Check Keyparts Duplicate or not
			if((m_InitialDlg.m_strModelName.Mid(1,3) != "TOU") && (m_InitialDlg.m_strModelName.Mid(1,3) != "LEO") && (m_InitialDlg.m_strModelName.Mid(1,3) != "LUN"))
			{
				if (m_LC_KeyPartList.GetItemText(i,CHKDUPLIC_INDEX) != _T("N") && 
					m_LC_KeyPartList.GetItemText(i,CHKDUPLIC_INDEX) != _T("n"))
				{
					if (!CheckKeyparts_DuplicateEx(m_strPicassoNumber, m_strKeyPartBarcode))
					{
						ResetForScanAcessoryBarcode();
						return;
					}
				}
				if (  m_LC_KeyPartList.GetItemText(i,KPNAME_INDEX).CompareNoCase(_T("MB")) == 0)
				{
					CString str_PCBA_barcode;
					
					if(m_InitialDlg.m_strProcess.Find(_T("FALINK"),0) != -1 )
					{
						if(!Query_PCBA_NOEx(m_strPicassoNumber,_T("FAPRINT"),str_PCBA_barcode))
						return;
					}
					if( m_InitialDlg.m_strProcess.Find(_T("CSD_LINK"),0) != -1)
					{
						if(!Query_PCBA_NOEx(m_strPicassoNumber,_T("CSD_PRINT"),str_PCBA_barcode))
						return;
					}
					if((str_PCBA_barcode.GetLength() != 0) && (str_PCBA_barcode != m_strKeyPartBarcode))
					{
						AfxMessageBox(_T("The KeyPart:") + strKeyPartPartNumber+ _T("(Label) error!"));
						return;
					}
				}

				if (  m_LC_KeyPartList.GetItemText(i,KPNAME_INDEX).CompareNoCase(_T("NAL")) == 0)
				{
					CString str_IMEI;

					if(!CheckNAL_ValidityEx(m_InitialDlg.m_strPartNumber,m_strKeyPartBarcode))
					{
						AfxMessageBox(_T("The KeyPart:") + strKeyPartPartNumber+ _T("(NAL) InValid!"));
						return;
					}

					if(!QueryIMEIbyNALEx( m_strKeyPartBarcode, str_IMEI))
						return;

					if( str_IMEI.GetLength() != 0 )
					{
						if (str_IMEI != m_strSerialNo)
						{
							AfxMessageBox(_T("The KeyPart:") + m_strKeyPartBarcode+ _T("(NAL) was already linked with IMEI:") + str_IMEI);
							return;
						}
					}

					if (!SSSO2_UpdateNALEx(m_strSerialNo  , m_strKeyPartBarcode))
					{
						AfxMessageBox(_T("Update:NAL") + m_strKeyPartBarcode+ _T(" to SSS Failure!"));
						return;
					}

				}

				if (  m_LC_KeyPartList.GetItemText(i,KPNAME_INDEX).CompareNoCase(_T("Badge")) == 0)
				{
					if(!CheckBadge_ValidityEx(m_strKeyPartBarcode,m_InitialDlg.m_strPartNumber))
					{
						AfxMessageBox(_T("The KeyPart:") + m_strKeyPartBarcode+ _T("(Badge) InValid!"));
						return;
					}
				}
			}

			//****For Wolf key part link	Dave.Wu 20111007
			//WOLF will     check Battery Run-In Record Marked by Lennon 2011.10.20
			//WOLF will not check Battery Run-In Record Marked by Lennon 2012.01.13
			int last_BatteryCount_Scanned_PassRunIn = m_BatteryCount_Scanned_PassRunIn;
			if(m_InitialDlg.m_strModelName.Mid(1,3) == "ORA" )
			{
				if(!this->CheckBatteryAndCharger())
				{
					ResetForScanAcessoryBarcode();
					return;
				}
			}
			//****Dave.Wu 20111007

			// Add by Lennon 2014.01.08
			if(!CheckPTKPWIP_BeforeInsertPTKPWIP(m_strPicassoNumber, m_InitialDlg.m_strProcess, m_strKeyPartBarcode,m_LC_KeyPartList.GetItemText(i,KPNAME_INDEX),m_InitialDlg.m_strEmployeeID,m_LC_KeyPartList.GetItemText(i,PN_INDEX)))
			{
				ResetForScanAcessoryBarcode();
				return;
			}
			// Add by Lennon 2014.01.08

			if(!InsertKeypartsInfoEx(m_strPicassoNumber, m_InitialDlg.m_strProcess, m_strKeyPartBarcode,m_LC_KeyPartList.GetItemText(i,KPNAME_INDEX),m_InitialDlg.m_strEmployeeID,m_LC_KeyPartList.GetItemText(i,PN_INDEX)))
			{
				ResetForScanAcessoryBarcode();
				return;
			}

			m_LC_KeyPartList.SetItemText(i,KEYPART_INDEX, m_strKeyPartBarcode);
			m_LC_KeyPartList.SetItemText(i,RESULT_INDEX, _T("PASS"));

			// Add by Lennon 2014.03.26
			for(unsigned int j = 0 ; j< m_InitialDlg.m_vKeyPartInfo.size();j++)
			{
				if ( j == i )
					continue;
				if( m_LC_KeyPartList.GetItemText(j,RESULT_INDEX) == _T("PASS") || m_LC_KeyPartList.GetItemText(j,RESULT_INDEX) == _T("N/A"))
					continue;
				if( m_LC_KeyPartList.GetItemText(j,PN_INDEX) == m_LC_KeyPartList.GetItemText(i,PN_INDEX)
					&& m_LC_KeyPartList.GetItemText(j,KPNAME_INDEX) == m_LC_KeyPartList.GetItemText(i,KPNAME_INDEX))
				{
					m_LC_KeyPartList.SetItemText(j,KEYPART_INDEX, _T(""));
					m_LC_KeyPartList.SetItemText(j,RESULT_INDEX, _T("N/A"));
					m_LC_KeyPartList.SetItemColor(j,RGB(GRAY_NA,GRAY_NA,GRAY_NA));
				}
			}
			// Add by Lennon 2014.03.26

			if (m_BatteryCount_Scanned_PassRunIn > last_BatteryCount_Scanned_PassRunIn)
				m_LC_KeyPartList.SetItemColor(i,RGB(46,139,87 ));
			else
				m_LC_KeyPartList.SetItemColor(i,RGB(0,255,0));
			//Add by Lennon for Orange
			if (m_strKeyPartBarcode.Find(_T("11S31052573ZZ0PA"),0)>= 0)
				m_BatteryCount_Scanned++;
			//ResetForScanAcessoryBarcode();
			return;
		}
	}
	AfxMessageBox(_T("The KeyPart:" + m_strKeyPartBarcode + " dose not match any KeyPart ID Rule!\nPlease check carefully!"));
	ResetForScanAcessoryBarcode();
}

bool CKeyPartsLinkDlg::CheckSNBarCodeAndGetPicasso(void)
{
	CString strIMEI;
	CString strIMEI_Status;
	CString strCartonID;

	m_Edit_SN.EnableWindow(false);
	m_Edit_SN.GetWindowTextW(m_strSerialNo);
	if(m_strSerialNo.GetLength()!=PICASSO_LENGTH&&m_strSerialNo.GetLength()!=PPID_LENGTH&&m_strSerialNo.GetLength()!=IMEI_LENGTH&&m_strSerialNo.GetLength()!=SERVICETAG_LENGTH)
	{
		ResetForReDO();
		return false;
	}
	if(m_strSerialNo.GetLength()==PICASSO_LENGTH)
		m_strPicassoNumber = m_strSerialNo;
	if(m_strSerialNo.GetLength()!=PICASSO_LENGTH)
	{
		CString str_QueriedPN;
		if(!GetPicassoEx( m_strSerialNo, m_InitialDlg.m_strModelName, m_strPicassoNumber,str_QueriedPN))
		{
			m_LShowPass.SetTextColor(RGB(255,0,0));
			m_LShowPass.SetText(_T("FAIL"));
			ResetForReDO();
			return false;
		}
		if (str_QueriedPN != m_InitialDlg.m_strPartNumber)
		{
			AfxMessageBox(_T("The Part N.O.queried by Input SN is ") + str_QueriedPN + _T(", but it is not Match queried by SO ") + m_InitialDlg.m_strPartNumber);
			m_LShowPass.SetTextColor(RGB(255,0,0));
			m_LShowPass.SetText(_T("FAIL"));
			ResetForReDO();
			return false;
		}
	}
	if (!QuerySOCountEx(m_strPicassoNumber,m_InitialDlg.m_strSONumber,m_InitialDlg.m_strProcess,m_strSOCount))
	{
		m_LShowPass.SetTextColor(RGB(255,0,0));
		m_LShowPass.SetText(_T("FAIL"));
		ResetForReDO();
		return false;
	}
	else
	{
		if(m_InitialDlg.m_strSOCRtype  != _T(""))
		{
			char sz_SOCount[256] = {0};
			char sz_SOQty[256] = {0};
			WideCharToMultiByte(CP_ACP,NULL,m_strSOCount.GetBuffer(),  -1,sz_SOCount,m_strSOCount.GetLength()*3,  NULL, NULL);
			WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strSOQty.GetBuffer(),  -1,sz_SOQty,m_InitialDlg.m_strSOQty.GetLength()*3,  NULL, NULL);
			int n_SOCount = atoi(sz_SOCount);
			int n_SOQty = atoi(sz_SOQty);
			if (n_SOCount > n_SOQty)
			{
				AfxMessageBox(_T("The Actual Input Qty of CR SO is larger then Planned Qty( ") + m_InitialDlg.m_strSOQty + _T(" )"));
				m_LShowPass.SetTextColor(RGB(255,0,0));
				m_LShowPass.SetText(_T("FAIL"));
				ResetForReDO();
				return false;
			}
		}

	}
	if(!QueryIMEI_ByPicassoEx(m_strPicassoNumber ,strIMEI))
	{
		m_LShowPass.SetTextColor(RGB(255,0,0));
		m_LShowPass.SetText(_T("FAIL"));
		ResetForReDO();
		return false;
	}

	if (strIMEI.GetLength() != 0 && m_InitialDlg.m_strProcess.Find(_T("PPIDLINK"),0) != -1)
	{
		if( !QueryIMEIStatusEx(strIMEI,strIMEI_Status,strCartonID))
		{
			m_LShowPass.SetTextColor(RGB(255,0,0));
			m_LShowPass.SetText(_T("FAIL"));
			ResetForReDO();
			return false;
		}
		else
		{
			if (strIMEI_Status != _T("I"))
			{
				AfxMessageBox(_T("IMEI Status is ") + strIMEI_Status + _T("! Must be I!"));
				m_LShowPass.SetTextColor(RGB(255,0,0));
				m_LShowPass.SetText(_T("FAIL"));
				ResetForReDO();
				return false;
			}
		}
	}

	if (!QueryholdInfoEx(m_strPicassoNumber,m_InitialDlg.m_strModelName,m_InitialDlg.m_strProcess))
	{
		m_LShowPass.SetTextColor(RGB(255,0,0));
		m_LShowPass.SetText(_T("FAIL"));
		ResetForReDO();
		return false;
	}

	if (strIMEI.GetLength() != 0 && !QueryholdInfoEx(strIMEI,m_InitialDlg.m_strModelName,m_InitialDlg.m_strProcess))
	{
		m_LShowPass.SetTextColor(RGB(255,0,0));
		m_LShowPass.SetText(_T("FAIL"));
		ResetForReDO();
		return false;
	}

	if (m_InitialDlg.m_strProcess.Find(_T("PPIDLINK"),0) != -1)
	{
		if (!QueryCRHoldInfoEx(m_strPicassoNumber))
		{
			m_LShowPass.SetTextColor(RGB(255,0,0));
			m_LShowPass.SetText(_T("FAIL"));
			AfxMessageBox(_T("QueryCRHoldInfo, Picasso Status = H"));
			ResetForReDO();
			return false;
		}
	}

	if (m_InitialDlg.m_strProcess.Find(_T("PPIDLINK"),0) == -1)
	{
		if (!CheckSOEx(m_InitialDlg.m_strSONumber,m_InitialDlg.m_strSOCRtype,m_strPicassoNumber,m_InitialDlg.m_strModelName,m_InitialDlg.m_strProcess,m_InitialDlg.m_strSegment))
		{
			m_LShowPass.SetTextColor(RGB(255,0,0));
			m_LShowPass.SetText(_T("FAIL"));
			ResetForReDO();
			return false;
		}
	}
	return true;
}

void CKeyPartsLinkDlg::OnCbnSelendokComboProcess()
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_BUTTON_LOCK))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUTTON_LOCK))->SetButtonStyle(BS_DEFPUSHBUTTON);
	((CButton*)GetDlgItem(IDC_BUTTON_LOCK))->SetFocus();
	if (m_InitialDlg.m_strProcess.Find(_T("PPIDLINK"),0) != -1 && m_InitialDlg.m_strModelName.Mid(1,3) == "ORA")
	{
		((CButton*)GetDlgItem(IDC_RADIO_ONLY_ONE))->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_RADIO_ATLEAST_ONE))->EnableWindow(true);
	}
	((CButton*)GetDlgItem(IDC_CHECK_PRINT_PCBA_LABEL))->EnableWindow(false);
	if (m_InitialDlg.m_strProcess.Find(_T("FALINK"),0) != -1 || m_InitialDlg.m_strProcess.Find(_T("CSD_LINK"),0) != -1)
	{
		((CButton*)GetDlgItem(IDC_CHECK_PRINT))->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_CHECK_REPRINTLABEL))->EnableWindow(true);
	}
}
//****For Wolf key part link	Dave.Wu 20111007
//This function will support all models if need to check run-in Records for some keyparts by Lennon 20111216
bool CKeyPartsLinkDlg::CheckBatteryAndCharger()
{
	char szErrorCode[512] = {0}, szErrorMsg[512] = {0};
	CStringA cstrModel(m_InitialDlg.m_strModelName.GetString());
	CStringA cstrBarcode(this->m_strKeyPartBarcode);
	bool ret = false;

	if(0 == cstrBarcode.Find("BT"))
	{
		bool ret = theApp.pQueryRuninID(cstrModel.GetBuffer(), cstrBarcode.GetBuffer(), "L3", szErrorCode, szErrorMsg);

		if(!ret)
		{
			CString strErrorcode = _T("");
			CString strErrorMsg = _T("");
			theApp.MultiByteToWideCharEnhanced(szErrorCode,strErrorcode);
			theApp.MultiByteToWideCharEnhanced(szErrorMsg,strErrorMsg);
			AfxMessageBox(_T("Battery didn't pass run_in! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
			ResetForScanAcessoryBarcode();
		}

		return ret;
	}

	// ***Add for Orange Battery by Lennon 20111216
	// At least one Battery must Run-In Pass
	else if(0 == cstrBarcode.Find("11S31052573ZZ0PA"))
	{
		bool ret = theApp.pQueryRuninID(cstrModel.GetBuffer(), cstrBarcode.GetBuffer(), "L3", szErrorCode, szErrorMsg);
		if(ret)
			m_BatteryCount_Scanned_PassRunIn++;

		// m_BatteryCount_InBACH;
		// m_BatteryCount_Scanned;
		if(((CButton*)GetDlgItem(IDC_RADIO_ONLY_ONE))->GetCheck() && m_BatteryCount_Scanned_PassRunIn >= 2 )
		{
			AfxMessageBox(_T("There are two Batteries pass run_in! Please take a non pass run_in Battery for ReScan!"));
			m_BatteryCount_Scanned_PassRunIn--;
			ResetForScanAcessoryBarcode();
			return false;
		}
		if(!ret && (m_BatteryCount_Scanned == m_BatteryCount_InBACH-1) &&  0 == m_BatteryCount_Scanned_PassRunIn )
		{
			CString strErrorcode = _T("");
			CString strErrorMsg = _T("");
			theApp.MultiByteToWideCharEnhanced(szErrorCode,strErrorcode);
			theApp.MultiByteToWideCharEnhanced(szErrorMsg,strErrorMsg);
			AfxMessageBox(_T("Battery didn't pass run_in! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
			ResetForScanAcessoryBarcode();
			return false;
		}
		return true;
	}
	//***Add End

	else if(0 == cstrBarcode.Find("AP0050P026"))
	{
		bool ret = theApp.pQueryRuninID(cstrModel.GetBuffer(), cstrBarcode.GetBuffer(), "L4", szErrorCode, szErrorMsg);
		
		if(!ret)
		{
			CString strErrorcode = _T("");
			CString strErrorMsg = _T("");
			theApp.MultiByteToWideCharEnhanced(szErrorCode,strErrorcode);
			theApp.MultiByteToWideCharEnhanced(szErrorMsg,strErrorMsg);
			AfxMessageBox(_T("Charger didn't pass run_in! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
			ResetForScanAcessoryBarcode();
		}

		return ret;
	}

	else if(0 == cstrBarcode.Find("5K16R"))
	{
		bool ret = theApp.pQueryRuninID(cstrModel.GetBuffer(), cstrBarcode.GetBuffer(), "L5", szErrorCode, szErrorMsg);
		
		if(!ret)
		{
			CString strErrorcode = _T("");
			CString strErrorMsg = _T("");
			theApp.MultiByteToWideCharEnhanced(szErrorCode,strErrorcode);
			theApp.MultiByteToWideCharEnhanced(szErrorMsg,strErrorMsg);
			AfxMessageBox(_T("USB charger didn't pass run_in! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
			ResetForScanAcessoryBarcode();
		}

		return ret;
	}
	
	else
		return true;
}
//****Dave.Wu 20111007

bool CKeyPartsLinkDlg::DoPrintTravelLabel(void)
{
	// Add Below is just for PrintFATravelLabel
	bool DBresult = false;
	char sz_Picasso[32]={0};
	char sz_Model[32]={0};
	char sz_HW[32]={0};
	char sz_SW[32]={0};
	char sz_szSONumber[32]={0};
	char sz_Process[32]={0};
	char sz_SOCount[32]={0};
	char sz_PN[32]={0};
	char sz_Line[32]={0};
	char sz_outInfo[512]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};
	WCHAR WTemp[32]={0};

	WideCharToMultiByte(CP_ACP,NULL,m_strPicassoNumber.GetBuffer(),-1,sz_Picasso,m_strPicassoNumber.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_strSOCount.GetBuffer(),-1,sz_SOCount,m_strSOCount.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strModelName.GetBuffer(),-1,sz_Model,m_InitialDlg.m_strModelName.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strSONumber.GetBuffer(),-1,sz_szSONumber,m_InitialDlg.m_strSONumber.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strPartNumber.GetBuffer(),-1,sz_PN,m_InitialDlg.m_strPartNumber.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strLineName.GetBuffer(),-1,sz_Line,m_InitialDlg.m_strLineName.GetLength()*3,NULL, NULL);
	
	/* Get 5E ini file pathname to print HW & SW Info */
	WCHAR szFolder[200] = {0};
	WCHAR sz_tmpBuf[100] = {0};
	::GetCurrentDirectory(200, szFolder);
	CString str_iniFileName;
	//str_iniFileName.Format(_T("%s\\Qisda\\%s\\%s.ini"), szFolder,m_InitialDlg.m_strModelName,m_InitialDlg.m_strPartNumber);
	str_iniFileName.Format(_T("C:\\PE_config\\%s\\PartNumber\\%s.ini"),m_InitialDlg.m_strModelName,m_InitialDlg.m_strPartNumber);
	/* Get HW SW Version */
	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	CString strTmp;

	::GetPrivateProfileString(_T("VERSION"), _T("HW_VERSION"), _T(""), sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
	strTmp = sz_tmpBuf;
	if (strTmp.GetLength() == 0)
	{
		AfxMessageBox(_T("No HW_VERSION Config in ") + str_iniFileName);
		return false;
	}
	WideCharToMultiByte(CP_ACP,NULL,strTmp.GetBuffer(),-1,sz_HW,strTmp.GetLength()*3,NULL, NULL);
	
	::GetPrivateProfileString(_T("VERSION"), _T("SW_VERSION"), _T(""), sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
	strTmp = sz_tmpBuf;
	if (strTmp.GetLength() == 0)
	{
		AfxMessageBox(_T("No SW_VERSION Config in ") + str_iniFileName);
		return false;
	}
	WideCharToMultiByte(CP_ACP,NULL,strTmp.GetBuffer(),-1,sz_SW,strTmp.GetLength()*3,NULL, NULL);

	CString str_labelFileName;
	str_labelFileName.Format(_T("C:\\PE_config\\Label_File\\%s travel label.lab"), m_InitialDlg.m_strModelName);
	/* Check label file if existent */
	if(_taccess(str_labelFileName, 0) != 0) {
		AfxMessageBox( _T(" Label_File not found! -> ")+ str_labelFileName);
		return false;
	}


	if(PrintFATravelLabel(sz_Picasso,sz_Model,sz_HW,sz_SW,sz_szSONumber,sz_SOCount,sz_PN,sz_Line,sz_outInfo) != TRUE)
	{
		AfxMessageBox(_T("PrintFATravelLabel Fail！Please Retry!"));
		return false;
	}
	return true;
}

bool CKeyPartsLinkDlg::GenerateAndPrintPCBALabel(void)
{
	// Add Below is just for PrintFAPCBALabel

	unsigned int year  = 0;
	unsigned int month = 0;
	unsigned int day   = 0;
	//int weekid;
	CString str_year  = m_strPicassoNumber.Mid(2,1);
	CString str_month = m_strPicassoNumber.Mid(3,1);
	CString str_day   = m_strPicassoNumber.Mid(4,1);

	CString str_acerPN = m_InitialDlg.m_vKeyPartInfo[0].strKP_ID_Rule.Mid(0,10);
	CString str_weekid;
	CString str_weekSN;
	CString str_MM_VV = m_InitialDlg.m_vKeyPartInfo[0].strKP_ID_Rule.Mid(18,4);
	//CString str_MM = _T("75");
	//CString str_VV = _T("D2");

	char sz_PCBA_barcode[32]={0};
	char sz_Model[32]={0};
	char sz_outInfo[512]={0};

	// Get Year from pciasso ID
	CTime tm=CTime::GetCurrentTime();
	year = tm.GetYear();
	year = year - year%10;
	if ( "0" == str_year )
		year = year + 0;
	else if ( "1" == str_year )
		year = year + 1;
	else if ( "2" == str_year )
		year = year + 2;
	else if ( "3" == str_year )
		year = year + 3;
	else if ( "4" == str_year )
		year = year + 4;
	else if ( "5" == str_year )
		year = year + 5;
	else if ( "6" == str_year )
		year = year + 6;
	else if ( "7" == str_year )
		year = year + 7;
	else if ( "8" == str_year )
		year = year + 8;
	else if ( "9" == str_year )
		year = year + 9;
	else
	{
		AfxMessageBox(_T("Get Year from picasso fail! Unknow Define -> ") + str_year);
		return false;
	}

	// Get Month from pciasso ID
	if ( "1" == str_month )
		month = 1;
	else if ( "2" == str_month )
		month = 2;
	else if ( "3" == str_month )
		month = 3;
	else if ( "4" == str_month )
		month = 4;
	else if ( "5" == str_month )
		month = 5;
	else if ( "6" == str_month )
		month = 6;
	else if ( "7" == str_month )
		month = 7;
	else if ( "8" == str_month )
		month = 8;
	else if ( "9" == str_month )
		month = 9;
	else if ( "A" == str_month )
		month = 10;
	else if ( "B" == str_month )
		month = 11;
	else if ( "C" == str_month )
		month = 12;
	else
	{
		AfxMessageBox(_T("Get Month from picasso fail! Unknow Define -> ") + str_month );
		return false;
	}

	// Get Day from pciasso ID
	if ( "1" == str_day )
		day = 1;
	else if ( "2" == str_day )
		day = 2;
	else if ( "3" == str_day )
		day = 3;
	else if ( "4" == str_day )
		day = 4;
	else if ( "5" == str_day )
		day = 5;
	else if ( "6" == str_day )
		day = 6;
	else if ( "7" == str_day )
		day = 7;
	else if ( "8" == str_day )
		day = 8;
	else if ( "9" == str_day )
		day = 9;
	else if ( "A" == str_day )
		day = 10;
	else if ( "B" == str_day )
		day = 11;
	else if ( "C" == str_day )
		day = 12;
	else if ( "D" == str_day )
		day = 13;
	else if ( "E" == str_day )
		day = 14;
	else if ( "F" == str_day )
		day = 15;
	else if ( "G" == str_day )
		day = 16;
	else if ( "H" == str_day )
		day = 17;
	else if ( "J" == str_day )
		day = 18;
	else if ( "K" == str_day )
		day = 19;
	else if ( "L" == str_day )
		day = 20;
	else if ( "M" == str_day )
		day = 21;
	else if ( "N" == str_day )
		day = 22;
	else if ( "P" == str_day )
		day = 23;
	else if ( "Q" == str_day )
		day = 24;
	else if ( "R" == str_day )
		day = 25;
	else if ( "S" == str_day )
		day = 26;
	else if ( "T" == str_day )
		day = 27;
	else if ( "U" == str_day )
		day = 28;
	else if ( "V" == str_day )
		day = 29;
	else if ( "W" == str_day )
		day = 30;
	else if ( "X" == str_day )
		day = 31;
	else
	{
		AfxMessageBox(_T("Get day from picasso fail! Unknow Define -> ") + str_day );
		return false;
	}

	CString str_Year_Barcode;
	CString str_WeekId_Barcode;

	int n_weekid = GetWeekID(year,month,day,str_Year_Barcode,str_WeekId_Barcode);

	//Query if it is linked by acer PCBA
	bool IsLinked = false;
	if(!Query_PCBA_NOEx(m_strPicassoNumber,m_InitialDlg.m_strProcess,m_str_PCBA_barcode))
		return false;

	if (m_str_PCBA_barcode.GetLength() != 0)
		IsLinked = true;
	if(!IsLinked)
	{
		//Query SN by acerPN + WeekID
		CString str_weekid_DB;
		CString str_Serial;

		str_weekid_DB.Format(_T("%dW%d"),year,n_weekid);
		if(!Query_PCBA_SerialNOEx(m_strPicassoNumber,str_acerPN,str_weekid_DB,m_InitialDlg.m_strEmployeeID,str_Serial))
			return false;
		m_str_PCBA_barcode = str_acerPN + str_Year_Barcode + str_WeekId_Barcode + str_Serial + str_MM_VV; //str_MM + str_VV;

		if( m_str_PCBA_barcode.GetLength() != 22 )
		{
			AfxMessageBox(_T("Generate PCBA Barcode Length Error!"));
			return false;
		}
	}

	m_Edit_KeyPartBarcode.SetWindowText(m_str_PCBA_barcode);

	WideCharToMultiByte(CP_ACP,NULL,m_str_PCBA_barcode.GetBuffer(),-1,sz_PCBA_barcode,m_str_PCBA_barcode.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strModelName.GetBuffer(),-1,sz_Model,m_InitialDlg.m_strModelName.GetLength()*3,NULL, NULL);
	
	//Insert PASS data
	for(unsigned int i = 0; i < 3;i++)
	{
		SyncTime(m_InitialDlg.m_strSyncTimeServerIP);
		if(!SetValueAndInsertPASSData())
		{
			Sleep(2000);
			if (i == 2)
			{
				m_LShowPass.SetTextColor(RGB(255,0,0));
				m_LShowPass.SetText(_T("FAIL"));
				ResetForReDO();
				return false;
			}
			continue;
		}
		else
			break;
	}

	bool IsNeedPrinted = true;
	if(IsLinked)
	{
		if (IDNO == AfxMessageBox(_T("Need to Re-Print FA PCBA Label?"),MB_YESNO))
			IsNeedPrinted = false;
	}

	if (IsNeedPrinted)
	{
		CString str_labelFileName;
		str_labelFileName.Format(_T("C:\\PE_config\\Label_File\\%s_PCBA_Label.lab"), m_InitialDlg.m_strModelName);
		/* Check label file if existent */
		if(_taccess(str_labelFileName, 0) != 0) {
			AfxMessageBox( _T(" Label_File not found! -> ")+ str_labelFileName);
			return false;
		}

		if(PrintFAPCBALabel(sz_PCBA_barcode,sz_Model,sz_outInfo) != TRUE)
		{
			AfxMessageBox(_T("Print FA PCBA Label Fail！Please Retry!"));
			return false;
		}
	}

	return true;
}
bool CKeyPartsLinkDlg::CheckKeyparts_DuplicateEx(CString strPicassoID, CString strKeyPartID/*, CString strKeyPartName, CString strModelName*/)
{
	char sz_PicassoID[512]={0};
	char sz_KeyPartID[512]={0};
	//char sz_KeyPartName[512]={0};
	//char sz_Model[32]={0};

	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};
	WCHAR WTemp[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,  strPicassoID.GetBuffer(), -1,sz_PicassoID,    strPicassoID.GetLength()*3, NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,  strKeyPartID.GetBuffer(), -1,sz_KeyPartID,    strKeyPartID.GetLength()*3, NULL, NULL);
	//WideCharToMultiByte(CP_ACP,NULL,strKeyPartName.GetBuffer(), -1,sz_KeyPartName,strKeyPartName.GetLength()*3, NULL, NULL);
	//WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strModelName.GetBuffer(),-1,sz_Model,m_InitialDlg.m_strModelName.GetLength()*3,NULL, NULL);
	
	bool DBresult = theApp.pCheckKeyparts_duplicate(sz_PicassoID,sz_KeyPartID/*,sz_KeyPartName,sz_Model*/,sz_Errorcode,sz_ErrMsg);
	if (!DBresult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pCheckKeyparts_duplicate! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}

	return true;
}


bool CKeyPartsLinkDlg::InsertKeypartsInfoEx(CString strPicasso, CString strProcess,CString strKeyPartId,CString strKeyPartName,CString strEmployId,CString strKeyPartPN)
{
	char sz_Picasso[32]={0};
	char sz_Process[32]={0};
	char sz_KeyPartId[32]={0};
	char sz_KeyPartName[32]={0};
	char sz_EmployId[32]={0};
	char sz_KeyPartPN[32]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,strPicasso.GetBuffer(),-1,sz_Picasso,strPicasso.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strProcess.GetBuffer(),-1,sz_Process,strProcess.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strEmployId.GetBuffer(),-1,sz_EmployId,strEmployId.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strKeyPartId.GetBuffer(),-1,sz_KeyPartId,strKeyPartId.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strKeyPartName.GetBuffer(),-1,sz_KeyPartName,strKeyPartName.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strKeyPartPN.GetBuffer(),-1,sz_KeyPartPN,strKeyPartPN.GetLength()*3,NULL, NULL);

	bool bResult = theApp.pInsertKeypartsInfo(sz_Picasso, sz_Process,sz_KeyPartId,sz_KeyPartName,sz_EmployId,sz_KeyPartPN,sz_Errorcode,sz_ErrMsg);
	if(!bResult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pInsertKeypartsInfo Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	return true;
}

bool CKeyPartsLinkDlg::UpdateEmployeeID_StatusEx(CString strEmployId, CString strStatus,CString strPCName)
{
	char sz_EmployId[32]={0};
	char sz_Status[32]={0};
	char sz_PCName[512]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};
	WideCharToMultiByte(CP_ACP,NULL,strEmployId.GetBuffer(),-1,sz_EmployId,strEmployId.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strStatus.GetBuffer(),-1,sz_Status,strStatus.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strPCName.GetBuffer(),-1,sz_PCName,strPCName.GetLength()*3,NULL, NULL);

	bool DBresult = theApp.pUpdateEmployeeID_Status(sz_EmployId,sz_Status,sz_PCName,sz_Errorcode,sz_ErrMsg);
	if (DBresult)
	{
		return true;
	}
	else
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pUpdateEmployeeID_Status! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
}

bool CKeyPartsLinkDlg::CheckRoutingNextStation()
{
	//Check Routing Start!
	CString strPicassoNumber = m_strPicassoNumber;
	CString strModelName = m_InitialDlg.m_strModelName;
	CString strCurrentProcess = m_InitialDlg.m_strProcess;
	CString strNextProcess;

	for(unsigned int i = 0; i< m_InitialDlg.m_vProcessSelect.size();i++)
	{
		if (m_InitialDlg.m_vProcessSelect[i] == strCurrentProcess)
		{
			if ((i + 1) <= m_InitialDlg.m_vProcessSelect.size())
				strNextProcess = m_InitialDlg.m_vProcessSelect[i+1];
			break;
		}
	}
	if (strNextProcess.GetLength() == 0)
		return false;

	if(!CheckRoutingEx( strPicassoNumber, strModelName, strNextProcess))
		return false;
	return true;
}


bool CKeyPartsLinkDlg::QuerySOCountEx(CString strId, CString strSONumber,CString strProcess,CString &strSOCount)
{
	char sz_id[32]={0};
	char sz_szSONumber[32]={0};
	char sz_Process[32]={0};
	char sz_SOCount[32]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};
	bool DBresult = false;

	WideCharToMultiByte(CP_ACP,NULL,strId.GetBuffer(),      -1,sz_id,        strId.GetLength()*3, NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strSONumber.GetBuffer(),-1,sz_szSONumber,strSONumber.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strProcess.GetBuffer(), -1,sz_Process,   strProcess.GetLength()*3,  NULL, NULL);

	DBresult =  theApp.pQuerySOCount(sz_id,sz_szSONumber,sz_Process,sz_SOCount,sz_Errorcode,sz_ErrMsg);
	if (!DBresult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("QuerySOCount! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	else
		theApp.MultiByteToWideCharEnhanced(sz_SOCount,strSOCount);
	return true;

}

bool CKeyPartsLinkDlg::QueryholdInfoEx(CString strPicassoNumber,CString strModelName,CString strProcess)
{
	//Check Routing Start!
	char sz_id[32]={0};
	char sz_Model[32]={0};
	//char sz_Process[4096]={0};
	char sz_Processes[8192]={0};
	char sz_Status[32]={0};
	char sz_HoldReason[4096]={0};
	char sz_Errorcode[4096]={0};
	char sz_ErrMsg[4096]={0};
	bool DBresult = false;

	WideCharToMultiByte(CP_ACP,NULL,strPicassoNumber.GetBuffer(), -1,sz_id,     strPicassoNumber.GetLength()*3, NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strModelName.GetBuffer(),-1,sz_Model,  strModelName.GetLength()*3,NULL, NULL);
	//WideCharToMultiByte(CP_ACP,NULL,strProcess.GetBuffer(),  -1,sz_Process,strProcess.GetLength()*3,  NULL, NULL);

	//DBresult = theApp.pQueryholdInfo(sz_id,sz_Model,sz_Status,sz_Process,sz_HoldReason,sz_Errorcode,sz_ErrMsg);
	DBresult = theApp.pQueryholdInfo(sz_id,sz_Model,sz_Status,sz_Processes,sz_HoldReason,sz_Errorcode,sz_ErrMsg);
	if (!DBresult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQueryholdInfo! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	else
	{
		CString strProcesses = _T("");
		CString strStatus = _T("");
		CString strHoldReason = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Processes,strProcesses);
		theApp.MultiByteToWideCharEnhanced(sz_Status,strStatus);
		theApp.MultiByteToWideCharEnhanced(sz_HoldReason,strHoldReason);

		if( -1 == strProcesses.Find(strProcess,0))
			return true;

		if(strStatus == _T("N") ||strStatus == _T("R"))
			return true;
		else
		{
			AfxMessageBox(_T("pQueryholdInfo! Status = ") + strStatus + _T(" ,HoldReason = ") + strHoldReason);
			return false;
		}

	}
}

bool CKeyPartsLinkDlg::QueryCRHoldInfoEx(CString strPicassoNumber)
{
	char sz_id[32]={0};
	char sz_Status[32]={0};
	char sz_Errorcode[4096]={0};
	char sz_ErrMsg[4096]={0};
	bool DBresult = false;

	WideCharToMultiByte(CP_ACP,NULL,strPicassoNumber.GetBuffer(), -1,sz_id,     strPicassoNumber.GetLength()*3, NULL, NULL);

	DBresult = theApp.pQueryCRHoldInfo(sz_id,sz_Status,sz_Errorcode,sz_ErrMsg);
	if (!DBresult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQueryCRHoldInfo! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	else
	{
		CString strStatus = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Status,strStatus);
		if(strStatus == _T("H") )
			return false;
		else
			return true;
	}
}

bool CKeyPartsLinkDlg::CheckSOEx(CString strSo,CString strSoCRtype,CString strPicasso,CString strModel,CString strProcess,CString strSegment)
{
	char sz_szSONumber[32]={0};
	char sz_soCRtype[32]={0};
	char sz_id[32]={0};
	char sz_Model[32]={0};	
	char sz_Process[32]={0};
	char sz_Segment[32]={0};
	char sz_Errorcode[4096]={0};
	char sz_ErrMsg[4096]={0};
	WCHAR WTemp[512]={0};
	bool DBresult = false;

	WideCharToMultiByte(CP_ACP,NULL,strPicasso.GetBuffer(),-1,sz_id,strPicasso.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strSo.GetBuffer(),-1,sz_szSONumber,strSo.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strSoCRtype.GetBuffer(),-1,sz_soCRtype,strSoCRtype.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strModel.GetBuffer(),-1,sz_Model,strModel.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strProcess.GetBuffer(),-1,sz_Process,strProcess.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strSegment.GetBuffer(),-1,sz_Segment,strSegment.GetLength()*3,NULL, NULL);

	DBresult = theApp.pCheckSO(sz_szSONumber,sz_soCRtype,sz_id,sz_Model,sz_Process,sz_Segment,sz_Errorcode,sz_ErrMsg);
	if (!DBresult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pCheckSO! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	return DBresult;
}	

bool CKeyPartsLinkDlg::CheckProcessTestDateEx(CString strPicassoNumber,CString strModelName,CString strSegment,CString strProcess)
{
	char sz_id[32]={0};
	char sz_Model[32]={0};
	char sz_Segment[4096]={0};
	char sz_Process[4096]={0};
	char sz_Errorcode[4096]={0};
	char sz_ErrMsg[4096]={0};
	bool DBresult = false;

	WideCharToMultiByte(CP_ACP,NULL,strPicassoNumber.GetBuffer(), -1,sz_id,     strPicassoNumber.GetLength()*3, NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strModelName.GetBuffer(),-1,sz_Model,  strModelName.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strSegment.GetBuffer(),  -1,sz_Segment,strSegment.GetLength()*3,  NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strProcess.GetBuffer(),  -1,sz_Process,strProcess.GetLength()*3,  NULL, NULL);

	DBresult = theApp.pCheckProcessTestDate(sz_id,sz_Model,sz_Segment,sz_Process,sz_Errorcode,sz_ErrMsg);
	if (!DBresult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pCheckProcessTestDate! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	else
	{
		return true;
	}
}

bool CKeyPartsLinkDlg::GetPicassoEx(CString strId, CString strModelName, CString &strPicasso,CString &strPartNO)
{
	char sz_id[32]={0};
	char sz_Model[32]={0};
	char sz_Picasso[32]={0};
	char sz_PartNO[32]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};
	WideCharToMultiByte(CP_ACP,NULL,strId.GetBuffer(), -1,sz_id,     strId.GetLength()*3, NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strModelName.GetBuffer(),-1,sz_Model,  strModelName.GetLength()*3,NULL, NULL);
	if(!theApp.pGetPicasso( sz_id, sz_Model, sz_Picasso,sz_PartNO, sz_Errorcode, sz_ErrMsg))
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pGetPicasso! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	theApp.MultiByteToWideCharEnhanced(sz_Picasso,strPicasso);
	theApp.MultiByteToWideCharEnhanced(sz_PartNO,strPartNO);
	return true;
}

bool CKeyPartsLinkDlg::QueryIMEI_ByPicassoEx(CString strPicassoid ,CString &strIMEI)
{
	char sz_picassoid[32]={0};
	char sz_IMEI[32]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,strPicassoid.GetBuffer(),-1,sz_picassoid,strPicassoid.GetLength()*3,NULL, NULL);
	bool bResult = theApp.pQueryIMEI_ByPicasso(sz_picassoid, sz_IMEI,sz_Errorcode,sz_ErrMsg);
	if(!bResult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQueryIMEI_ByPicasso Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	else
	{
		theApp.MultiByteToWideCharEnhanced(sz_IMEI,strIMEI);
	}
	return true;

}

bool CKeyPartsLinkDlg::QueryIMEIStatusEx(CString strIMEI,CString &strStatus,CString &strCartonID)
{
	char sz_IMEI[32]={0};
	char sz_Status[32]={0};
	char sz_CartonID[32]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,strIMEI.GetBuffer(),-1,sz_IMEI,strIMEI.GetLength()*3,NULL, NULL);
	bool bResult = theApp.pQueryIMEIStatus(sz_IMEI, sz_Status,sz_CartonID,sz_Errorcode,sz_ErrMsg);
	if(!bResult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQueryIMEIStatus Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	else
	{
		theApp.MultiByteToWideCharEnhanced(sz_Status,strStatus);
		theApp.MultiByteToWideCharEnhanced(sz_CartonID,strCartonID);
	}
	return true;

}

bool CKeyPartsLinkDlg::QueryKeypartsByIDEx(CString strId, CString strProcess, std::vector <CInitialDlg::KeyPartInfoArray> &vKeyPartInfo)
{
	char sz_id[32]={0};
	char sz_Process[32]={0};
	char sz_Count[32]={0};
	char sz_Info[8192]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WCHAR WTemp[8192]={0};

	CString delims = _T(";");
	CString Subdelims = _T(",");
	CString OneKeyPartInfo;

	CInitialDlg::KeyPartInfoArray tempstrArray;
	int nStart = 0;
	int nSubStart = 0;
	int nPos = 0;
	int nSubPos = 0;
	int IndexID = 0;
	int KeyPartCount = 0;

	CString strInfo;

	WideCharToMultiByte(CP_ACP,NULL,strId.GetBuffer(),      -1,sz_id,      strId.GetLength()*3,      NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strProcess.GetBuffer(), -1,sz_Process, strProcess.GetLength()*3, NULL, NULL);
	if(!theApp.pQueryKeypartsByID( sz_id, sz_Process, sz_Count,sz_Info, sz_Errorcode, sz_ErrMsg))
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQueryKeypartsByID! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	theApp.MultiByteToWideCharEnhanced(sz_Info,strInfo);
	KeyPartCount = atoi(sz_Count);

	while (strInfo.Find(delims,nStart) > 0 || nStart < strInfo.GetLength())
	{
		if (strInfo.Find(delims,nStart) > 0)
			nPos = strInfo.Find(delims,nStart);
		else
			nPos = strInfo.GetLength();
		OneKeyPartInfo = strInfo.Mid(nStart,nPos-nStart);
		while (OneKeyPartInfo.Find(Subdelims,nSubStart) > 0 || nSubStart < OneKeyPartInfo.GetLength())
		{
			if (OneKeyPartInfo.Find(Subdelims,nSubStart) > 0)
				nSubPos = OneKeyPartInfo.Find(Subdelims,nSubStart);
			else
				nSubPos = OneKeyPartInfo.GetLength();
			CString strTemp = OneKeyPartInfo.Mid(nSubStart,nSubPos-nSubStart);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			if(IndexID == 0)
				tempstrArray.strKP_ID = strTemp;
			if(IndexID == 1)
				tempstrArray.strKP_Name = strTemp;
			if(IndexID == 2)
				tempstrArray.strKP_Part_Number = strTemp;
			nSubStart = nSubPos + 1;
			IndexID++;
		}
		if(IndexID != 3)
		{
			AfxMessageBox(_T("Check KeyPart Info Error! Info = ") + OneKeyPartInfo );
			theApp.pCloseDB();
			return false;
		}

		vKeyPartInfo.push_back(tempstrArray);
		nStart = nPos + 1;
		nSubStart = 0;
		nSubPos = 0;
		IndexID = 0;

	}

	return true;
}

bool CKeyPartsLinkDlg::CheckKPsStatusInPTKPWIP()
{
	std::vector <CInitialDlg::KeyPartInfoArray> vKeyPartInfo_Queried;
	std::vector <CString>                       vKeyPartInfo_PTKPWIP;
	std::vector <CString>                       vKeyPartInfo_Scanned; 

	if(!QueryKeypartsByIDEx(m_strPicassoNumber,m_strProcessSelect,vKeyPartInfo_Queried))
		return false;
	for(unsigned int i = 0 ; i< vKeyPartInfo_Queried.size();i++)
	{
		if(vKeyPartInfo_PTKPWIP.size() == 0)
		{
			vKeyPartInfo_PTKPWIP.insert(vKeyPartInfo_PTKPWIP.begin() + 0,vKeyPartInfo_Queried[i].strKP_ID);
			continue;
		}
		for(unsigned int j = vKeyPartInfo_PTKPWIP.size()-1 ; j >= 0; j--)
		{
			if (vKeyPartInfo_Queried[i].strKP_ID >= vKeyPartInfo_PTKPWIP[j])
			{
				vKeyPartInfo_PTKPWIP.insert(vKeyPartInfo_PTKPWIP.begin() + j + 1,vKeyPartInfo_Queried[i].strKP_ID);
				break;
			}
			if (j == 0)
			{
				vKeyPartInfo_PTKPWIP.insert(vKeyPartInfo_PTKPWIP.begin() + 0,vKeyPartInfo_Queried[i].strKP_ID);
				break;
			}
		}
	}

	for(unsigned int i = 0 ; i< m_InitialDlg.m_vKeyPartInfo.size();i++)
	{
		if (m_LC_KeyPartList.GetItemText(i,RESULT_INDEX) != _T("PASS"))
			continue;

		if(vKeyPartInfo_Scanned.size() == 0)
		{
			vKeyPartInfo_Scanned.insert(vKeyPartInfo_Scanned.begin() + 0,m_LC_KeyPartList.GetItemText(i,KEYPART_INDEX));
			continue;
		}
		for(unsigned int j = vKeyPartInfo_Scanned.size()-1 ; j >= 0; j--)
		{
			if (m_LC_KeyPartList.GetItemText(i,KEYPART_INDEX) >= vKeyPartInfo_Scanned[j])
			{
				vKeyPartInfo_Scanned.insert(vKeyPartInfo_Scanned.begin() + j + 1,m_LC_KeyPartList.GetItemText(i,KEYPART_INDEX));
				break;
			}
			if (j == 0)
			{
				vKeyPartInfo_Scanned.insert(vKeyPartInfo_Scanned.begin() + 0,m_LC_KeyPartList.GetItemText(i,KEYPART_INDEX));
				break;
			}
		}
	}

	if (vKeyPartInfo_PTKPWIP.size() == 0 || vKeyPartInfo_Scanned.size() == 0 || vKeyPartInfo_PTKPWIP.size() !=vKeyPartInfo_Scanned.size())
	{
		AfxMessageBox(_T("Check KeyPart Info Count Error in PTKPWIP!") );
		return false;
	}

	for(unsigned int i = 0 ; i< vKeyPartInfo_PTKPWIP.size();i++)
		if(vKeyPartInfo_PTKPWIP[i] != vKeyPartInfo_Scanned[i])
		{
			AfxMessageBox(_T("Compare KeyPart kpid Error in PTKPWIP!") );
			return false;
		}
	return true;
}

bool CKeyPartsLinkDlg::CheckPTKPWIP_BeforeScan( )
{
	std::vector <CInitialDlg::KeyPartInfoArray> vKeyPartInfo_Queried;
	std::vector <bool> vKeyPart_Matched;

	if(!QueryKeypartsByIDEx(m_strPicassoNumber,m_strProcessSelect,vKeyPartInfo_Queried))
		return false;

	// Means this picasso is unlinked or the first time to KeyParts Link!
	if (vKeyPartInfo_Queried.size() == 0 )
		return true;

	// Intial Mapping Flag
	for(unsigned int i = 0 ; i< m_InitialDlg.m_vKeyPartInfo.size();i++)
		if ( true== m_InitialDlg.m_vKeyPartInfo[i].bExistInSOBOM)
			vKeyPart_Matched.push_back(false);

	//Compare kpid in ptKPWIP with Current SO
	for(unsigned int i = 0 ; i< vKeyPartInfo_Queried.size();i++)
	{
		bool bMatch = false;
		for(unsigned int j = 0 ; j< vKeyPart_Matched.size();j++)
		{
			if(vKeyPart_Matched[j] == true)
				continue;
			if(m_InitialDlg.m_vKeyPartInfo[j].strKP_Name == vKeyPartInfo_Queried[i].strKP_Name && m_InitialDlg.m_vKeyPartInfo[j].strKP_Part_Number == vKeyPartInfo_Queried[i].strKP_Part_Number)
			{
				if (KeyPartIDRuleCompare(vKeyPartInfo_Queried[i].strKP_ID,  m_InitialDlg.m_vKeyPartInfo[j].strKP_ID_Rule))
				{
					vKeyPart_Matched[j] = true;
					bMatch = true;
					//m_LC_KeyPartList.SetItemText(j, RULE_INDEX, vKeyPartInfo_Queried[i].strKP_ID);
					break;
				}
			}
		}
		if (  false == bMatch )
		{
			AfxMessageBox(_T("Check Old KeyPart Error in PTKPWIP! Please UnLink first then do KeyPartsLink!") );
			return false;
		}
	}
	return true;
}


bool CKeyPartsLinkDlg::CheckPTKPWIP_BeforeInsertPTKPWIP(CString strPicasso, CString strProcess,CString strKeyPartId,CString strKeyPartName,CString strEmployId,CString strKeyPartPN)
{
	std::vector <CInitialDlg::KeyPartInfoArray> vKeyPartInfo_Queried;
	if(!QueryKeypartsByIDEx(strPicasso,strProcess,vKeyPartInfo_Queried))
		return false;
	for(unsigned int i = 0 ; i< vKeyPartInfo_Queried.size();i++)
	{
		if( vKeyPartInfo_Queried[i].strKP_Name ==  strKeyPartName )
		{
			if (vKeyPartInfo_Queried[i].strKP_ID !=  strKeyPartId)
			{
				AfxMessageBox(_T("Check Old KeyPart Error in PTKPWIP!\nPicasso:") +  strPicasso 
					+ _T("\n\tOld:")   +  vKeyPartInfo_Queried[i].strKP_ID 
					+ _T("\n\tNew:")   +  strKeyPartId 
					+ _T("\nError barcode Input!"));
				//m_LShowPass.SetTextColor(RGB(255,0,0));
				//m_LShowPass.SetText(_T("FAIL"));
				//ResetForReDO();
				return false;
			}
		}
	}
	return true;
}

bool CKeyPartsLinkDlg::InitialUITitle()
{
	char sz_Temp[MAX_PATH] = {0};
	GetModuleFileNameA(NULL, sz_Temp, MAX_PATH);
	PathRemoveFileSpecA(sz_Temp);
	CString strTitlePicturePath;
	theApp.MultiByteToWideCharEnhanced(sz_Temp,strTitlePicturePath);
	strTitlePicturePath += _T("\\res\\Header.jpg");

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

	CImage image;      
	image.Load(strTitlePicturePath);
	if (image.IsNull())
	{
		return false;
	}

	CDC* pDC = pWnd->GetWindowDC();  
	pDC->SetStretchBltMode(HALFTONE);
	pDC->SetBrushOrg(0,0);//improve image quality
	CString strToolName = m_InitialDlg.m_strProcess + _T("(") + _T(TOOL_VER) + _T(")");
	image.Draw( pDC->m_hDC, rect); 
	rect.left = rect.left + 200;
	rect.right = rect.right - 280;
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
	pDC->DrawText(strToolName,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC); 
	return true;

}

bool CKeyPartsLinkDlg::SyncTime( CString strServerIP )
{

	/* Sync Net Time*/
	CString strCommand;
	strCommand = strCommand + _T("net time \\\\") + strServerIP + _T(" /set /y");
	WCHAR wsz_commandLine[2048];
	wcscpy(wsz_commandLine, CT2CW(strCommand));

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;

	BOOL b_createRes = FALSE;	
	b_createRes = CreateProcess(NULL, wsz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
	if(b_createRes == FALSE) {
		return false;
	}
	WaitForSingleObject(pi.hProcess, INFINITE); 
	return true;
}

void CKeyPartsLinkDlg::ExecuteOperationsIfCompleted ( )
 {
	if(m_InitialDlg.m_strProcess.Find(_T("FAPRINT"),0) != -1 || m_InitialDlg.m_strProcess.Find(_T("CSD_PRINT"),0) != -1)  //IE Requirement
	{
		m_LC_Result.InsertItem(0,_T(""));
		m_LC_Result.SetItemText(0, 0,m_strSerialNo );//设置行索引（行号）和列索引（列号）的值
		m_LC_Result.SetItemText(0, 1,m_str_PCBA_barcode);//设置行索引（行号）和列索引（列号）的值
		if ( m_LC_Result.GetItemCount() > 10 )
			m_LC_Result.DeleteItem(m_LC_Result.GetItemCount()-1);
		return;
	}

	bool b_KeyPartScanOK = true;
	for(unsigned int i = 0; i< m_InitialDlg.m_vKeyPartInfo.size();i++)
	{
		b_KeyPartScanOK = b_KeyPartScanOK && (m_LC_KeyPartList.GetItemText(i,RESULT_INDEX) == _T("PASS") ||( m_LC_KeyPartList.GetItemText(i,RESULT_INDEX) == _T("N/A")));
		if (!b_KeyPartScanOK)
		{
			ResetForScanAcessoryBarcode();
			break;
		}
	}
	if(b_KeyPartScanOK == true && (m_InitialDlg.m_vKeyPartInfo.size() > 0))
	{
		//Add by Lennon 2012.01.30
		//It is used to double check kp info in ptkpwip before insert pass data
		if (!CheckKPsStatusInPTKPWIP())
		{
			m_LShowPass.SetTextColor(RGB(255,0,0));
			m_LShowPass.SetText(_T("FAIL"));
			ResetForReDO();
			return;
		}
		// Add Below is just for PrintFATravelLabel
		if (((CButton*)GetDlgItem(IDC_CHECK_PRINT))->GetCheck())
		{
			for(unsigned int i = 0; i < 3;i++)
			{
				if(!DoPrintTravelLabel()) 
				{
					Sleep(2000);
					if (i == 2)
					{
						m_LShowPass.SetTextColor(RGB(255,0,0));
						m_LShowPass.SetText(_T("FAIL"));
						ResetForReDO();
						return;
					}
					continue;
				}
				else
					break;
			}
		}
		for(unsigned int i = 0; i < 3;i++)
		{
			SyncTime(m_InitialDlg.m_strSyncTimeServerIP);
			if(!SetValueAndInsertPASSData())
			{
				Sleep(2000);
				if (i == 2)
				{
					m_LShowPass.SetTextColor(RGB(255,0,0));
					m_LShowPass.SetText(_T("FAIL"));
					ResetForReDO();
					return;
				}
				continue;
			}
			else
				break;
		}

		
		if((m_InitialDlg.m_strProcess.Find(_T("FALINK"),0) != -1) || (m_InitialDlg.m_strProcess.Find(_T("CSD_LINK"),0) != -1) || (m_InitialDlg.m_strProcess.Find(_T("PPIDLINK"),0) != -1))
		{
			m_LC_Result.InsertItem(0,_T(""));
			m_LC_Result.SetItemText(0, 0,m_strSerialNo );//设置行索引（行号）和列索引（列号）的值
			m_LC_Result.SetItemText(0, 1,m_InitialDlg.m_strSegment);//设置行索引（行号）和列索引（列号）的值
			m_LC_Result.SetItemText(0, 2,m_InitialDlg.m_strProcess);//设置行索引（行号）和列索引（列号）的值
			m_LC_Result.SetItemText(0, 3,_T("OK"));//设置行索引（行号）和列索引（列号）的值
		}

		//Only Dispay the latest 10 records
		if ( m_LC_Result.GetItemCount() > 10 )
			m_LC_Result.DeleteItem(m_LC_Result.GetItemCount()-1);
		m_LShowPass.SetTextColor(RGB(0,255,0));
		m_LShowPass.SetText(_T("PASS"));
		ResetForReDO();
		return;
	}
 }

int  CKeyPartsLinkDlg::GetWeekID(unsigned int year,unsigned int month,unsigned int day,CString &str_Year,CString &str_WeekId)
{
	unsigned int year_base = year;
	unsigned int month_base = 0;
	unsigned int day_base = 0;
	unsigned int weekid = 0;
	unsigned int year_cycle;

	//Get the date belong to which year cyle
	if ( month == 12 && day >=29 )
	{
		month_base = 12;
		day_base = 29;
		for(; day_base < 32 ; day_base++ )
		{
			CTime date( year_base , month_base , day_base , 0, 0 , 0);
			if ( date.GetDayOfWeek() == 2)
				break;
		}
		
		if ( day_base < 32 )  //find Monday
			if (( month > month_base) || ( month == month_base) && ( day >= day_base ))
				year_base++;
	}

	if ( month == 1 && day <=4 )
	{
		month_base = 1;
		day_base = 1;
		for(; day_base < 5 ; day_base++ )
		{
			CTime date( year_base , month_base , day_base , 0, 0 , 0);
			if ( date.GetDayOfWeek() == 2)
				break;
		}
		
		if ( day_base < 5 )  //find Monday
			if (( month < month_base) || ( month == month_base) && ( day < day_base ))
				year_base--;
	}
	year_cycle = year_base;

	//Get Base month/day
	month_base = 1;
	day_base = 1;
	for(; day_base < 8 ; day_base++ )
	{
		CTime date( year_base , month_base , day_base , 0, 0 , 0);
		if ( date.GetDayOfWeek() == 2)
			break;
	}
	if ( day_base > 4)
	{
		year_base--;
		month_base = 12;
		day_base = 29;
		for(; day_base < 32 ; day_base++ )
		{
			CTime date( year_base , month_base , day_base , 0, 0 , 0);
			if ( date.GetDayOfWeek() == 2)
				break;
		}
	}

	CTime date_target(year,month,day,0,0,0);
	CTime date_base( year_base , month_base , day_base , 0, 0 , 0);
	CTimeSpan span = date_target - date_base; 
	int days_offset = span.GetDays();
	
	weekid = (days_offset + (7 - days_offset % 7))/7;

	year_cycle = year_cycle%10;
	str_Year.Format(_T("%1d"),year_cycle);
	str_WeekId.Format(_T("%02d"),weekid);
	return weekid;
}

bool CKeyPartsLinkDlg::Query_PCBA_SerialNOEx(CString strPicassoid ,CString str_AcerMB , CString str_YWW,CString str_Employee,CString &str_Serial)
{
	char sz_picassoid[32]={0};
	char sz_AcerMB[32]={0};
	char sz_YWW[32]={0};
	char sz_EmployId[32]={0};
	char sz_Serial[32]={0};

	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,strPicassoid.GetBuffer(),-1,sz_picassoid,strPicassoid.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,str_AcerMB.GetBuffer(),-1,sz_AcerMB,str_AcerMB.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,str_YWW.GetBuffer(),-1,sz_YWW,str_YWW.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,str_Employee.GetBuffer(),-1,sz_EmployId,str_Employee.GetLength()*3,NULL, NULL);

	bool bResult = theApp.pQuery_PCBA_SerialNO(sz_picassoid,sz_AcerMB, sz_YWW,sz_EmployId,sz_Serial,sz_Errorcode,sz_ErrMsg);
	if(!bResult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQuery_PCBA_SerialNO Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	else
	{
		//int n_serial = atoi(sz_Serial);
		//str_Serial.Format( _T("%05X"), n_serial);
		theApp.MultiByteToWideCharEnhanced(sz_Serial,str_Serial);
	}
	return true;

}

bool CKeyPartsLinkDlg::Query_PCBA_NOEx(CString strPicassoid , CString strProcess, CString &str_PCBAno )
{
	char sz_picassoid[32]={0};
	char sz_Process[32]={0};
	char sz_PCBAno[32]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,strPicassoid.GetBuffer(),-1,sz_picassoid,strPicassoid.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strProcess.GetBuffer(),-1,sz_Process,strProcess.GetLength()*3,NULL, NULL);
	bool bResult = theApp.pQuery_PCBA_NO(sz_picassoid,sz_Process, sz_PCBAno,sz_Errorcode,sz_ErrMsg);
	if(!bResult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQuery_PCBA_NO Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	else
	{
		theApp.MultiByteToWideCharEnhanced(sz_PCBAno,str_PCBAno);
	}
	return true;
}

bool CKeyPartsLinkDlg::QueryNALEx(CString str_IMEI , CString &str_NAL)
{
	char sz_IMEI[32]={0};
	char sz_NAL[32]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,str_IMEI.GetBuffer(),-1,sz_IMEI,str_IMEI.GetLength()*3,NULL, NULL);
	bool bResult = theApp.pQueryNAL(sz_IMEI, sz_NAL,sz_Errorcode,sz_ErrMsg);
	if(!bResult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQueryNALEx Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	else
	{
		theApp.MultiByteToWideCharEnhanced(sz_NAL,str_NAL);
	}
	return true;
}

bool CKeyPartsLinkDlg::QueryIMEIbyNALEx(CString str_NAL , CString &str_IMEI)
{
	char sz_NAL[32]={0};
	char sz_IMEI[32]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,str_NAL.GetBuffer(),-1,sz_NAL,str_NAL.GetLength()*3,NULL, NULL);
	bool bResult = theApp.pQueryIMEIbyNAL(sz_NAL, sz_IMEI,sz_Errorcode,sz_ErrMsg);
	if(!bResult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQueryIMEIbyNALEx Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	else
	{
		theApp.MultiByteToWideCharEnhanced(sz_IMEI,str_IMEI);
	}
	return true;
}

bool CKeyPartsLinkDlg::CheckNAL_ValidityEx ( CString str_PartNO, CString str_NAL)
{
	char sz_NAL[32]={0};
	char sz_PartNO[32]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,str_NAL.GetBuffer(),-1,sz_NAL,str_NAL.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,str_PartNO.GetBuffer(),-1,sz_PartNO,str_PartNO.GetLength()*3,NULL, NULL);

	bool bResult = theApp.pCheckNAL_Validity( sz_PartNO,sz_NAL ,sz_Errorcode,sz_ErrMsg);
	if(!bResult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pCheckNAL_Validity Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	return bResult;
}

bool CKeyPartsLinkDlg::Initial_SSSO2Dll()
{
	CoUninitialize();
	//WinExec("regsvr32 /s SSS_O2.Dll",SW_SHOWDEFAULT);
	//Sleep(2000);

	/* regsvr32 /s SSS_O2.Dll */
	CString strCommand = _T("regsvr32 /s SSS_O2.Dll");
	WCHAR wsz_commandLine[2048];
	wcscpy(wsz_commandLine, CT2CW(strCommand));

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;

	BOOL b_createRes = FALSE;	
	b_createRes = CreateProcess(NULL, wsz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
	if(b_createRes == FALSE) {
		return false;
	}
	WaitForSingleObject(pi.hProcess, INFINITE); 

	HRESULT hresult;
	CLSID clsid;

	hresult=CLSIDFromProgID(OLESTR("SSS_O2.clsO2Check"), &clsid);
	hresult=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(SSS_O2::_clsO2Check),(LPVOID *) &m_pITS_O2);

	if(FAILED(hresult))
	{
		return false;
	}
	return true;
}
bool CKeyPartsLinkDlg::SSSO2_UpdateNALEx(CString str_IMEI ,CString str_NAL)
{
	char sz_IMEI[32]={0};
	char sz_NAL[32]={0};
	//char sz_Errorcode[512]={0};
	//char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,str_IMEI.GetBuffer(),-1,sz_IMEI,str_IMEI.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,str_NAL.GetBuffer(),-1,sz_NAL,str_NAL.GetLength()*3,NULL, NULL);

	BOOL bResult;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,"");
	short int	_iRow;

	bResult = m_pITS_O2->funUpdateNAL(sz_IMEI,sz_NAL,&_iRow,&_ErrCode,&_ErrDes);

	CString strErrcode = _ErrCode.m_str;
	_ErrCode.Empty();

	CString strErrMsg = _ErrDes.m_str;
	_ErrDes.Empty();

	if ( _T("0000") == strErrcode )
		return true;

	return false;
}

bool CKeyPartsLinkDlg::CheckBadge_ValidityEx(CString str_Badge , CString str_PartNO)
{
	char sz_Badge[32]={0};
	char sz_PartNO[32]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,str_Badge.GetBuffer(),-1,sz_Badge,str_Badge.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,str_PartNO.GetBuffer(),-1,sz_PartNO,str_PartNO.GetLength()*3,NULL, NULL);

	bool bResult = theApp.pCheckBadge_Validity(sz_Badge, sz_PartNO,sz_Errorcode,sz_ErrMsg);
	if(!bResult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pCheckBadge_Validity Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	return bResult;
}

void CKeyPartsLinkDlg::OnBnClickedButtonInquiry()
{
	// TODO: Add your control notification handler code here
	CString str_ID = _T("");
	CString str_Index = _T("");
	m_UN_Link.DeleteAllItems();
	char sz_id[32]={0};
	m_Link_Text.GetWindowText(str_ID);
	WideCharToMultiByte(CP_ACP,NULL,str_ID.GetBuffer(),      -1,sz_id,      str_ID.GetLength()*3,      NULL, NULL);
	m_CBIndexSelect.GetWindowText(str_Index);
	if(str_Index==_T("PICASSO"))
	{
	
	char sz_Process[32]={0};
	char sz_IMEI[32]={0};
	char sz_Count[32]={0};
	char sz_Info[8192]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	CString delims = _T(";");
	CString Subdelims = _T(",");
	CString OneKeyPartInfo;

	CInitialDlg::KeyPartInfoArray QuerystrArray;
	int nStart = 0;
	int nSubStart = 0;

	int nPos = 0;
	int nSubPos = 0;
	int IndexID = 0;
	int KeyPartCount = 0;
    
	int nTemStart = 0;
	int nBemStart = 0;
	int nTemPos = 0;
	int nItem = 0;

	CString strInfo=_T("");
	
	bool bResult = theApp.pQueryIMEI_ByPicasso(sz_id, sz_IMEI,sz_Errorcode,sz_ErrMsg);
	if(!bResult)
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQueryIMEI_ByPicasso Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return;
	}
	CString strIMEI = _T("");
    theApp.MultiByteToWideCharEnhanced(sz_IMEI,strIMEI);
	if (strIMEI!=_T(""))
	{   
		char sz_status[30]={0};
		char sz_cartonID[30]={0};
		CString str_status=_T("");
		theApp.pQueryIMEIStatus(sz_IMEI,sz_status,sz_cartonID,sz_Errorcode,sz_ErrMsg);
        theApp.MultiByteToWideCharEnhanced(sz_status,str_status);
		if (str_status==_T("P")||str_status==_T("S"))
		{
            b_DoRemove=false;
            AfxMessageBox(_T("IMEI的状态为位")+str_status+_T("禁止进行解除操作"));
			return;
		}
	}
	
	//WideCharToMultiByte(CP_ACP,NULL,strProcess.GetBuffer(), -1,sz_Process, strProcess.GetLength()*3, NULL, NULL);
	if(theApp.pQueryKeypartsByID( sz_id, m_sz_Process, sz_Count,sz_Info, sz_Errorcode, sz_ErrMsg))
	{   
		theApp.MultiByteToWideCharEnhanced(sz_Info,strInfo);
		KeyPartCount = atoi(sz_Count);
		if (strInfo == _T(""))
		{   
			CString str_Process=_T("");
			theApp.MultiByteToWideCharEnhanced(m_sz_Process,str_Process);
			AfxMessageBox(_T("此PICASSO没有link任何")+str_Process+_T("的附件，请输入正确的已link的picasso进行查询"));
			return;
		}

		while (strInfo.Find(delims,nStart) > 0 || nStart < strInfo.GetLength())
		{

			if (strInfo.Find(delims,nSubStart) > 0)
				nSubPos = strInfo.Find(delims,nSubStart);
			else
				nSubPos = strInfo.GetLength();
			CString strTemp = strInfo.Mid(nSubStart,nSubPos-nSubStart);
			//strTemp.TrimLeft();
			//strTemp.TrimRight();
			strTemp = strTemp + _T(",");
			while (strTemp.Find(Subdelims,nTemStart) > 0&&nTemStart < strTemp.GetLength())
			{
				int abcd=strTemp.Find(Subdelims,nTemStart);
				if (strTemp.Find(Subdelims,nBemStart) > 0)
					nTemPos = strTemp.Find(Subdelims,nBemStart);
				else
					nTemPos = strTemp.GetLength();
                CString strBemp = strTemp.Mid(nTemStart,nTemPos-nTemStart);
				if(IndexID == 0)
				{
				 TCHAR itemID[32];
                 memset(itemID,0,sizeof(itemID));
				 wsprintf(itemID,_T("%d"), nItem+1);
                 m_UN_Link.InsertItem(nItem,itemID);
                 m_UN_Link.SetItemText(nItem,1,str_ID );
				 if(strIMEI==_T(""))
				 m_UN_Link.SetItemText(nItem,2,_T("NULL"));
				 else
				 m_UN_Link.SetItemText(nItem,2,strIMEI);
				 m_UN_Link.SetItemText(nItem,3,strBemp );
				 m_UN_Link.SetItemColor(nItem,RGB(255,255,255));
				}
				if(IndexID == 1)
				{
                m_UN_Link.SetItemText(nItem,4,strBemp );
				//m_UN_Link.SetItemColor(nItem,RGB(255,255,255));
				}
				if(IndexID == 2)
				{
                 m_UN_Link.SetItemText(nItem,5,strBemp );
				 //m_UN_Link.SetItemColor(nItem,RGB(255,255,255));
				}
				if (IndexID == 2)
				{
                    IndexID=0;
                    nTemStart=0;
					nBemStart=0;
					break;
				} 
				else
				{
				IndexID++;
				}
               nTemStart = nTemPos+1;
               nBemStart = nTemPos+1;
			}
            nItem++;
			nStart = nSubPos+1;
			nSubStart = nSubPos + 1;
		}

	}
	}
	else if(str_Index==_T("IMEI"))
	{
	 char sz_model[32]={0};
	 char sz_Picasso[32]={0};
	 char sz_partno[32]={0};
	 char sz_SO[32]={0};
	 char sz_sw[512]={0};
	 char sz_ErrCode[512]={0};
	 char sz_ErrMsg[512]={0};
	 char sz_Count[32]={0};
	 char sz_Info[8192]={0};

	 CString delims = _T(";");
	 CString Subdelims = _T(",");
	 //CString OneKeyPartInfo;

	// CInitialDlg::KeyPartInfoArray QuerystrArray;
	 int nStart = 0;
	 int nSubStart = 0;

	 int nPos = 0;
	 int nSubPos = 0;
	 int IndexID = 0;
	 int KeyPartCount = 0;

	 int nTemStart = 0;
	 int nBemStart = 0;
	 int nTemPos = 0;
	 int nItem = 0;

	 CString strInfo=_T("");

     
	 if (str_ID!=_T(""))
	 {   
		 char sz_status[30]={0};
		 char sz_cartonID[30]={0};
		 CString str_status=_T("");
		 theApp.pQueryIMEIStatus(sz_id,sz_status,sz_cartonID,sz_ErrCode,sz_ErrMsg);
		 theApp.MultiByteToWideCharEnhanced(sz_status,str_status);
		 if (str_status==_T("P")||str_status==_T("S"))
		 {
			 b_DoRemove=false;
			 AfxMessageBox(_T("IMEI的状态为位")+str_status+_T("禁止进行解除操作"));
			 return;
		 }
	 }

     bool result=theApp.pGetIMEI_Info(sz_id, sz_model,sz_Picasso,sz_partno,sz_SO,sz_sw,sz_ErrCode,sz_ErrMsg);
	 if (!result)
	 {
		 CString strErrorcode = _T("");
		 CString strErrorMsg = _T("");
		 theApp.MultiByteToWideCharEnhanced(sz_ErrCode,strErrorcode);
		 theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		 AfxMessageBox(_T("pQueryIMEI_ByPicasso Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		 return;
	 }
	 CString str_Picasso=_T("");
     theApp.MultiByteToWideCharEnhanced(sz_Picasso,str_Picasso);
	 if(theApp.pQueryKeypartsByID( sz_Picasso, m_sz_Process, sz_Count,sz_Info, sz_ErrCode, sz_ErrMsg))
	 {
		 theApp.MultiByteToWideCharEnhanced(sz_Info,strInfo);
		 KeyPartCount = atoi(sz_Count);
		 if (strInfo == _T(""))
		 {
			 CString str_Process=_T("");
			 theApp.MultiByteToWideCharEnhanced(m_sz_Process,str_Process);
			 AfxMessageBox(_T("此IMEI没有link任何")+str_Process+_T("的附件，请输入正确的已link的IMEI进行查询"));
			 return;
		 }

		 while (strInfo.Find(delims,nStart) > 0 || nStart < strInfo.GetLength())
		 {

			 if (strInfo.Find(delims,nSubStart) > 0)
				 nSubPos = strInfo.Find(delims,nSubStart);
			 else
				 nSubPos = strInfo.GetLength();
			 CString strTemp = strInfo.Mid(nSubStart,nSubPos-nSubStart);
			 //strTemp.TrimLeft();
			 //strTemp.TrimRight();
			 strTemp = strTemp + _T(",");
			 while (strTemp.Find(Subdelims,nTemStart) > 0&&nTemStart < strTemp.GetLength())
			 {
				 int abcd=strTemp.Find(Subdelims,nTemStart);
				 if (strTemp.Find(Subdelims,nBemStart) > 0)
					 nTemPos = strTemp.Find(Subdelims,nBemStart);
				 else
					 nTemPos = strTemp.GetLength();
				 CString strBemp = strTemp.Mid(nTemStart,nTemPos-nTemStart);
				 if(IndexID == 0)
				 {
					 TCHAR itemID[32];
					 memset(itemID,0,sizeof(itemID));
					 wsprintf(itemID,_T("%d"), nItem+1);
					 m_UN_Link.InsertItem(nItem,itemID);
					 m_UN_Link.SetItemText(nItem,1,str_Picasso);
					 m_UN_Link.SetItemText(nItem,2,str_ID);
					 m_UN_Link.SetItemText(nItem,3,strBemp );
					 m_UN_Link.SetItemColor(nItem,RGB(255,255,255));
				 }
				 if(IndexID == 1)
				 {
					 m_UN_Link.SetItemText(nItem,4,strBemp );
					 //m_UN_Link.SetItemColor(nItem,RGB(255,255,255));
				 }
				 if(IndexID == 2)
				 {
					 m_UN_Link.SetItemText(nItem,5,strBemp );
					 //m_UN_Link.SetItemColor(nItem,RGB(255,255,255));
				 }
				 if (IndexID == 2)
				 {
					 IndexID=0;
					 nTemStart=0;
					 nBemStart=0;
					 break;
				 } 
				 else
				 {
					 IndexID++;
				 }
				 nTemStart = nTemPos+1;
				 nBemStart = nTemPos+1;
			 }
			 nItem++;
			 nStart = nSubPos+1;
			 nSubStart = nSubPos + 1;
		 }

	 }
	}
	return;
}

void CKeyPartsLinkDlg::OnBnClickedButtonUnlink()
{
	// TODO: Add your control notification handler code here
	CString str_ID = _T("");
	CString str_Index = _T("");
	CString str_Model = _T("");
	int len=0;
	int i=0;
	//m_UN_Link.DeleteAllItems();
	char sz_id[32]={0};
	char sz_model[32]={0};
	char sz_Process[32]={0};

	m_Link_Text.GetWindowText(str_ID);
	GetDlgItem(IDC_STATIC_MODELNAME)->GetWindowText(str_Model);
	if(str_ID==_T(""))
	{
     AfxMessageBox(_T("请输入PICASSO或者IMEI信息"));
	 return;
	}

	WideCharToMultiByte(CP_ACP,NULL,str_ID.GetBuffer(),-1,sz_id,str_ID.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,str_Model.GetBuffer(),-1,sz_model,str_Model.GetLength()*3,NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_InitialDlg.m_strProcess.GetBuffer(),-1,sz_Process,m_InitialDlg.m_strProcess.GetLength()*3,NULL, NULL);
	m_CBIndexSelect.GetWindowText(str_Index);

	len=m_UN_Link.GetItemCount();
	if(len==0)
	{
		AfxMessageBox(_T("请先根据输入信息进行查询"));
		return;
	}


	if(str_Index==_T("PICASSO"))
	{
     bool result=false;
	 //char sz_model[32]={0};
	 char sz_ErrCode[512]={0};
	 char sz_ErrMsg[512]={0};

	 if(strstr(m_sz_Process,"PPIDLINK")!=NULL)
     result=theApp.pUnlinkKeyparts(sz_id,sz_model,sz_ErrCode,sz_ErrMsg);
	 else if(strstr(m_sz_Process,"FALINK")!=NULL)
	 result=theApp.pUnlinkKeyparts_FA(sz_id,sz_model,sz_Process,sz_ErrCode,sz_ErrMsg);
	 else if(strstr(m_sz_Process,"CSD_LINK")!=NULL)
	 result=theApp.pUnlinkKeyparts_FA(sz_id,sz_model,sz_Process,sz_ErrCode,sz_ErrMsg);

     for (i=0;i<len;i++)
     {
      m_UN_Link.SetItemText(i,6,_T("UnLink"));
	  m_UN_Link.SetItemColor(i,RGB(255,0,0));
     }

	 if (str_ID == m_strPicassoNumber)
		 ResetForReDO();

	}
	else if(str_Index==_T("IMEI"))
	{   
		char sz_model[32]={0};
		char sz_Picasso[32]={0};
		char sz_partno[32]={0};
		char sz_SO[32]={0};
		char sz_sw[32]={0};
		char sz_ErrCode[512]={0};
		char sz_ErrMsg[512]={0};

		bool result=theApp.pGetIMEI_Info(sz_id, sz_model,sz_Picasso,sz_partno,sz_SO,sz_sw,sz_ErrCode,sz_ErrMsg);
		if (!result)
		{
			CString strErrorcode = _T("");
			CString strErrorMsg = _T("");
			theApp.MultiByteToWideCharEnhanced(sz_ErrCode,strErrorcode);
			theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
			AfxMessageBox(_T("pQueryIMEI_ByPicasso Fail! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
			return;
		}
         
		if(strstr(m_sz_Process,"PPIDLINK")!=NULL)
			result=theApp.pUnlinkKeyparts(sz_Picasso,sz_model,sz_ErrCode,sz_ErrMsg);
		else if(strstr(m_sz_Process,"FALINK")!=NULL)
			result=theApp.pUnlinkKeyparts_FA(sz_Picasso,sz_model,sz_Process,sz_ErrCode,sz_ErrMsg);
		else if(strstr(m_sz_Process,"CSD_LINK")!=NULL)
			result=theApp.pUnlinkKeyparts_FA(sz_Picasso,sz_model,sz_Process,sz_ErrCode,sz_ErrMsg);
		for (i=0;i<len;i++)
		{
			m_UN_Link.SetItemText(i,6,_T("UnLink"));
			m_UN_Link.SetItemColor(i,RGB(255,0,0));
		}

		CString strPicasso;
		theApp.MultiByteToWideCharEnhanced(sz_Picasso,strPicasso);
		if (strPicasso == m_strPicassoNumber)
			ResetForReDO();
	}
	else if(str_Index==_T("KeyPart ID"))
	{
		for (i=0;i<len;i++)
		{
			m_UN_Link.SetItemText(i,6,_T("UnLink"));
			m_UN_Link.SetItemColor(i,RGB(255,0,0));
		}
	}

}

void CKeyPartsLinkDlg::OnBnClickedCheckUnLink()
{
	// TODO: Add your control notification handler code here
   if(((CButton *)GetDlgItem(IDC_CHECK_UNLINK))->GetCheck()==1)
   {
	   m_UN_Link.EnableWindow(true);
	   m_CBIndexSelect.EnableWindow(true);
	   m_Link_Text.EnableWindow(true);
	   GetDlgItem(IDC_BUTTON_INQUIRY)->EnableWindow(true);
	   GetDlgItem(IDC_BUTTON_UNLINK)->EnableWindow(true);
   }
   else
   {
	   m_UN_Link.EnableWindow(false);
	   m_CBIndexSelect.EnableWindow(false);
	   m_Link_Text.EnableWindow(false);
	   GetDlgItem(IDC_BUTTON_INQUIRY)->EnableWindow(false);
	   GetDlgItem(IDC_BUTTON_UNLINK)->EnableWindow(false);
       m_UN_Link.DeleteAllItems();
	   GetDlgItem(IDC_COMBO_Index)->SetWindowText(_T(""));
	   GetDlgItem(IDC_EDIT_UNLINK_SN)->SetWindowText(_T(""));
   }
}

void CKeyPartsLinkDlg::ConvertModelName( CString strModelName,CString &strModelName_Normal)
 {
	 CString str_cs = strModelName.Right(3);
	 str_cs = str_cs.Left(2);

	if (str_cs.CompareNoCase(_T("CS")) == 0)
	{
		str_cs.Format(_T("%sB%sA"), strModelName.Left(strModelName.GetLength() - 3) ,strModelName.Right(1));
		strModelName_Normal = str_cs;
	}
	else
	{
		strModelName_Normal = strModelName;
	}
 }

