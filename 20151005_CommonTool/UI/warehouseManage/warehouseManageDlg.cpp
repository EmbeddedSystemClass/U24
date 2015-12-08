
// warehouseManageDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "CTabel.h"
#include "warehouseManage.h"
#include "warehouseManageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const COLORREF CLOUDBLUE = RGB(128, 184, 223);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF BLACK = RGB(1, 1, 1);
const COLORREF DKGRAY = RGB(128, 128, 128);
const COLORREF LLTGRAY = RGB(220, 220, 220);
const COLORREF LTGRAY = RGB(192, 192, 192);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF DKYELLOW = RGB(128, 128, 0);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF DKRED = RGB(128, 0, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF LBLUE = RGB(192, 192, 255);
const COLORREF DKBLUE = RGB(0, 0, 128);
const COLORREF CYAN = RGB(0, 255, 255);
const COLORREF DKCYAN = RGB(0, 128, 128);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF DKGREEN = RGB(0, 128, 0);
const COLORREF MAGENTA = RGB(255, 0, 255);
const COLORREF DKMAGENTA = RGB(128, 0, 128);
const COLORREF STANDARD = RGB(215,240,198);

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


// CAboutTestDlg dialog

// CwarehouseManageDlg dialog


CwarehouseManageDlg::CwarehouseManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CwarehouseManageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_n_test = 0;
 //   m_b_loadDB_Res = false;
	//m_hDLL = NULL;
}

void CwarehouseManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab_warehouse);
	DDX_Control(pDX, IDCANCEL, m_btn_cancel);
	DDX_Control(pDX, IDC_Convirm, m_btn_Convirm);
	DDX_Control(pDX, IDOK, m_btn_ok);
}

BEGIN_MESSAGE_MAP(CwarehouseManageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, &CwarehouseManageDlg::OnTcnSelchangingTab1)
	ON_BN_CLICKED(IDOK, &CwarehouseManageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CwarehouseManageDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_Convirm, &CwarehouseManageDlg::OnBnClickedConvirm)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CwarehouseManageDlg::OnTcnSelchangeTab1)
	ON_COMMAND(ID_HELP_ABOUT, OnAbout)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CwarehouseManageDlg message handlers

void CwarehouseManageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	//if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	//{
	//	CAboutDlg dlgAbout;
	//	dlgAbout.DoModal();
	//}
	//else
	//{
	//	CDialog::OnSysCommand(nID, lParam);
	//}
}

BOOL CwarehouseManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_brush.CreateSolidBrush(RGB(215,240,198));//生成一绿色刷子

	/* Create menu */
	m_Menu.LoadMenu(IDR_MENU1); 
	SetMenu(&m_Menu);

	/* Change menu color */
	::MENUINFO  lpcmi;
	memset(&lpcmi, 0, sizeof(::LPCMENUINFO));
	lpcmi.cbSize  = sizeof(MENUINFO);
	lpcmi.fMask   = MIM_BACKGROUND;   
	lpcmi.hbrBack = (HBRUSH)m_brush.operator HBRUSH();
	::SetMenuInfo(GetMenu()->m_hMenu, &lpcmi);

	//初始化TAB
	TCITEM TC_InPut,TC_OutPut,TC_OutCsd;
	TC_InPut.mask = TCIF_TEXT;
	TC_InPut.pszText = _T("入库检查工具");
	TC_OutPut.mask = TCIF_TEXT;
	//TC_OutPut.pszText = _T("出库检查工具");
	TC_OutPut.pszText = _T("成品返工工具");
	TC_OutCsd.mask = TCIF_TEXT;
	TC_OutCsd.pszText = _T("CSD出货检查工具");


	m_tab_warehouse.InsertItem(0, &TC_InPut);
	m_tab_warehouse.InsertItem(1, &TC_OutPut); //到这里只是添加了两个tab页
	m_tab_warehouse.InsertItem(2, &TC_OutCsd);
	

	CRect rec;
	m_tab_warehouse.GetClientRect(&rec);//获得TAB控件的坐标

	//定位选项卡页的位置，这里可以根据情况自己调节偏移量
	rec.bottom -= 1;
	rec.left += 1;
	rec.top += 21;
	rec.right -= 2;

	//创建子页面
	m_InputDlg.Create(warehouse_Input, GetDlgItem(IDC_TAB1));
	m_OutputDlg.Create(warehouse_Out, GetDlgItem(IDC_TAB1));
	m_OutCsdDlg.Create(warehouse_Csd, GetDlgItem(IDC_TAB1));
	//将子页面移动到指定的位置
	m_InputDlg.MoveWindow(&rec);
	m_OutputDlg.MoveWindow(&rec);
	m_OutCsdDlg.MoveWindow(&rec);
	//显示子页面
	m_InputDlg.ShowWindow(SW_SHOW);
	m_OutputDlg.ShowWindow(SW_HIDE);
    m_OutCsdDlg.ShowWindow(SW_HIDE);
		
	m_tab_warehouse.SetBkgndColor(RGB(215,240,198));
	m_tab_warehouse.SetAttribute();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/*****************************************************************************
* Function name: OnUserManual     
* Summary      : Open user manual --- Help menu
* Parameters   : void
*                
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Tom.Li     2012/02/13             First version             
*****************************************************************************/
void CwarehouseManageDlg::OnAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
	return;
}

bool CwarehouseManageDlg::InitialUITitle()
{
	 //Draw title
	 char sz_Temp[MAX_PATH] = {0};

	 GetModuleFileNameA(NULL, sz_Temp, MAX_PATH);
	 PathRemoveFileSpecA(sz_Temp);
	 m_str_titlePicturePath = sz_Temp;
	 m_str_titlePicturePath += _T("\\res\\Header.jpg");


	 CRect rect;
     CStatic *pWnd = (CStatic*)GetDlgItem(IDC_STATIC_Title);
	 if (pWnd == NULL)
	 {
	  return false;
	 } 
	 else
	 {
	  pWnd->GetClientRect(&rect);
	 }

	 CImage image;      
	 image.Load(m_str_titlePicturePath);
	 if (image.IsNull())
	 {
	  return false;
	 }
	 CDC* pDC = pWnd->GetWindowDC();  
     pDC->SetStretchBltMode(HALFTONE);
	 image.Draw( pDC->m_hDC, rect);
	 pDC->SetBrushOrg(0,0);//improve image quality
     CString str_toolname = _T("WareHouse Manage V2.011");
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

	 //pOldFont_Button = GetDlgItem(IDC_EDIT1)-> GetFont();
	 m_btn_Convirm.SetColor(BLACK, STANDARD);
	 m_btn_Convirm.SetFont(&font);
	 m_btn_cancel.SetColor(BLACK, STANDARD);
	 m_btn_cancel.SetFont(&font);
	 m_btn_ok.SetColor(BLACK, STANDARD);
	 m_btn_ok.SetFont(&font);

	 return true;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CwarehouseManageDlg::OnPaint()
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
		//CRect rect;
  //      CPaintDC dc(this);
  //      GetClientRect(rect);
  //      dc.FillSolidRect(rect,RGB(215,240,198)); //设置为绿色背景
		CDialog::OnPaint();
		InitialUITitle();
		//m_Static_Title.DrawTitle();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.


HCURSOR CwarehouseManageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CwarehouseManageDlg::OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	if(m_InputDlg.m_b_lock)
	{
		bool b_res = m_tab_warehouse.SetCurSel(0);
		return;
	}
	else if(m_OutputDlg.m_b_lock)
	{
		bool b_res = m_tab_warehouse.SetCurSel(1);
		return;
	}
	else if(m_OutCsdDlg.m_b_lock)
	{
		bool b_res = m_tab_warehouse.SetCurSel(2);
		return;
	}
	//switch (m_tab_warehouse.GetCurSel())
 //   {
 //     case 0:
 //           m_InputDlg.ShowWindow(SW_HIDE);
 //           m_OutputDlg.ShowWindow(SW_SHOW);
	//		if(m_n_test != 0)
	//		{
	//			m_OutputDlg.m_Edit_SO_OutPut.SetWindowTextA(_T(""));
	//		}
	//		m_n_test++;
	//		m_OutputDlg.m_listctrl_OutPut.DeleteAllItems();
 //           break;
 //      case 1:
 //           m_InputDlg.ShowWindow(SW_SHOW);
 //           m_OutputDlg.ShowWindow(SW_HIDE);
	//		if(m_n_test != 0)
	//		{
	//		   m_InputDlg.m_EditSO.SetWindowTextA(_T(""));
	//		   m_InputDlg.m_EditID.SetWindowTextA(_T(""));
	//		   m_InputDlg.m_PN_show.SetWindowTextA(_T(""));
	//		   m_InputDlg.m_SW_show.SetWindowTextA(_T(""));
	//		   m_InputDlg.m_Qty_show.SetWindowTextA(_T(""));
	//		   m_InputDlg.m_Pass_show.SetWindowTextA(_T(""));
	//		   m_InputDlg.m_Total_show.SetWindowTextA(_T(""));
	//		   m_InputDlg.m_Fail_show.SetWindowTextA(_T(""));
	//		}
	//		m_n_test++;
	//		m_InputDlg.m_listctrl_Input.DeleteAllItems();
 //           break;
 //     default:
 //           break;
 //   }

	*pResult = 0;
}

void CwarehouseManageDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	int n_check = m_tab_warehouse.GetCurSel();
	
	if(n_check == 0)
	{
		//m_InputDlg.m_listctrl_Input.DeleteAllItems();
		//m_EditSO.GetWindowText(str_SO);
	    //m_EditID.GetWindowText(str_InputID);
		m_InputDlg.OnBnClickedOk();
	}
	else if(n_check == 1)
	{
		//m_OutputDlg.m_listctrl_OutPut.DeleteAllItems();
		m_OutputDlg.OnBnClickedOk();
	}
	else if(n_check == 2)
	{
        m_OutCsdDlg.OnBnClickedOk();
	}	
}

void CwarehouseManageDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}


void CwarehouseManageDlg::OnBnClickedConvirm()
{
	// TODO: Add your control notification handler code here
	int n_check = m_tab_warehouse.GetCurSel();
	if(n_check == 0)
	{
		m_InputDlg.m_listctrl_Input.DeleteAllItems();
		m_InputDlg.m_EditSO.SetWindowTextA(_T(""));
		m_InputDlg.m_EditID.SetWindowTextA(_T(""));
		m_InputDlg.m_PN_show.SetWindowTextA(_T(""));
		m_InputDlg.m_SW_show.SetWindowTextA(_T(""));
		m_InputDlg.m_Qty_show.SetWindowTextA(_T(""));
		m_InputDlg.m_Fail_show.SetWindowTextA(_T(""));
		m_InputDlg.m_Pass_show.SetWindowTextA(_T(""));
		m_InputDlg.m_Total_show.SetWindowTextA(_T(""));
		m_InputDlg.n_fail = 0;
		m_InputDlg.n_pass = 0;
		m_InputDlg.nIMEInum = 0;
		if(m_InputDlg.m_b_lock)
		{
		   m_InputDlg.OnBnClickedCheck1();
		}
		m_InputDlg.m_Lock_check.SetCheck(0);
		//if(m_InputDlg.n_IMEI_num_array > m_InputDlg.n_total)
		//{
		//	CString str_errcode = _T("");
		//	CString str_errMsg = _T("");
		//	str_errcode.Format(_T("%s"),_T("WM0102"));
		//	str_errMsg.Format(_T("%s"),_T("扫描的IMEI数量大于手机数量"));
		//	msgDlg.SetErrorCode(str_errcode);
		//	msgDlg.SetError(str_errMsg);
		//	msgDlg.DoModal();
		//	return;
		//}
	}
	else if(n_check == 2)
	{   
		m_OutCsdDlg.m_listctrl_OutCSD.DeleteAllItems();
		m_OutCsdDlg.n_fail = 0;
		m_OutCsdDlg.n_pass = 0;
		m_OutCsdDlg.n_total = 0;
		if(m_OutCsdDlg.m_Edit_CHECK.GetCheck()==1)
		{
		m_OutCsdDlg.m_b_lock = false;
        m_OutCsdDlg.m_Edit_CHECK.SetCheck(false);
        m_OutCsdDlg.GetDlgItem(IDC_EDIT1)->EnableWindow(true);
		}
		m_OutCsdDlg.GetDlgItem(IDC_EDIT1)->SetWindowText(_T(""));
		m_OutCsdDlg.GetDlgItem(IDC_EDIT2)->SetWindowText(_T(""));
		m_OutCsdDlg.GetDlgItem(IDC_EDIT3)->SetWindowText(_T(""));

		m_OutCsdDlg.GetDlgItem(IDC_PN_show)->SetWindowText(_T(""));
		m_OutCsdDlg.GetDlgItem(IDC_Model_show)->SetWindowText(_T(""));
		m_OutCsdDlg.GetDlgItem(IDC_SW_Show)->SetWindowText(_T(""));

		m_OutCsdDlg.GetDlgItem(IDC_Total_show)->SetWindowText(_T(""));
		m_OutCsdDlg.GetDlgItem(IDC_Pass_Show)->SetWindowText(_T(""));
		m_OutCsdDlg.GetDlgItem(IDC_Fail_show)->SetWindowText(_T(""));
	}
	else
	{
		m_OutputDlg.m_listctrl_OutPut.DeleteAllItems();
		m_OutputDlg.m_n_Fail = 0;
	    m_OutputDlg.m_n_Pass = 0;
		m_OutputDlg.m_Pass_show.SetWindowTextA(_T(""));
		m_OutputDlg.m_Total_show.SetWindowTextA(_T(""));
		m_OutputDlg.m_Fail_show.SetWindowTextA(_T(""));
		m_OutputDlg.nIMEInum = 0;
		m_OutputDlg.m_Edit_SO_OutPut.SetWindowText(_T(""));

		m_OutputDlg.m_Edit_ReworkReason.SetWindowText(_T(""));
		m_OutputDlg.m_Edit_ReworkReason.SetReadOnly(false);
		m_OutputDlg.m_Button_DOA_RMA_Option.SetCheck(false);
		m_OutputDlg.m_Button_DOA_RMA_Option.EnableWindow(true);
		m_OutputDlg.m_Edit_DOA_RMA_NO.SetWindowText(_T(""));
		m_OutputDlg.m_Edit_DOA_RMA_NO.SetReadOnly(false);
		m_OutputDlg.m_Button_Lock.SetCheck(false);
		m_OutputDlg.m_b_lock=false;
	}

	return;
}

void CwarehouseManageDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	if(m_InputDlg.m_b_lock)
	{
		bool b_res = m_tab_warehouse.SetCurSel(0);
		m_OutputDlg.EnableWindow(false);
		m_OutCsdDlg.EnableWindow(false);
		return;
	}
	else if(m_OutputDlg.m_b_lock)
	{
		bool b_res = m_tab_warehouse.SetCurSel(1);
		m_InputDlg.EnableWindow(false);
		m_OutputDlg.EnableWindow(true);
		m_OutCsdDlg.EnableWindow(false);
		return;
	}
	else if(m_OutCsdDlg.m_b_lock)
	{
		bool b_res = m_tab_warehouse.SetCurSel(2);
		m_InputDlg.EnableWindow(false);
		m_OutputDlg.EnableWindow(false);
		m_OutCsdDlg.EnableWindow(true);
		return;
	}

	m_InputDlg.EnableWindow(true);
	m_OutputDlg.EnableWindow(true);
	m_OutCsdDlg.EnableWindow(true);
	switch (m_tab_warehouse.GetCurSel())
    {
      case 0:
            m_InputDlg.ShowWindow(SW_SHOW);
            m_OutputDlg.ShowWindow(SW_HIDE);
			m_OutCsdDlg.ShowWindow(SW_HIDE);
			if(m_n_test != 0)
			{
				m_OutputDlg.m_Edit_SO_OutPut.SetWindowTextA(_T(""));
			}
			m_n_test++;
			m_OutputDlg.m_listctrl_OutPut.DeleteAllItems();
            break;
       case 1:
            m_InputDlg.ShowWindow(SW_HIDE);
            m_OutputDlg.ShowWindow(SW_SHOW);
			m_OutCsdDlg.ShowWindow(SW_HIDE);
			if(m_n_test != 0)
			{
			   m_InputDlg.m_EditSO.SetWindowTextA(_T(""));
			   m_InputDlg.m_EditID.SetWindowTextA(_T(""));
			   m_InputDlg.m_PN_show.SetWindowTextA(_T(""));
			   m_InputDlg.m_SW_show.SetWindowTextA(_T(""));
			   m_InputDlg.m_Qty_show.SetWindowTextA(_T(""));
			   m_InputDlg.m_Pass_show.SetWindowTextA(_T(""));
			   m_InputDlg.m_Total_show.SetWindowTextA(_T(""));
			   m_InputDlg.m_Fail_show.SetWindowTextA(_T(""));
			}
			m_n_test++;
			m_InputDlg.m_listctrl_Input.DeleteAllItems();
            break;
	   case 2:
		   m_InputDlg.ShowWindow(SW_HIDE);
		   m_OutputDlg.ShowWindow(SW_HIDE);
		   m_OutCsdDlg.ShowWindow(SW_SHOW);
      default:
            break;
    }
}

HBRUSH CwarehouseManageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired

	//switch(pWnd->GetDlgCtrlID())
	//{
	////针对ID为IDC_CTL1、IDC_CTL2和IDC_CTL3的控件进行同样的设置
	//	case IDOK:
	//	case IDC_Convirm:
	//	case IDCANCEL:
	//	{
	//	   pDC->SetBkMode(TRANSPARENT);
	//	   pDC->SetTextColor(RGB(255,0, 0));
	//	   return m_brush;
	//	}
	//	default:
	//		{
				pDC->SetBkMode(TRANSPARENT);
	            return m_brush;
	//		}
	//}
}
