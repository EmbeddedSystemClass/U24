
// Run_inDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Run_in.h"
#include "Run_inDlg.h"
#include "..\..\Modules\DialogControllers\SODlg.h"
#include "..\..\Modules\DialogControllers\LargeMsgBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INI_FILENAME _T("Run_In.ini")

CCriticalSection m_obj_FileLog;

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


// CRun_inDlg dialog




CRun_inDlg::CRun_inDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRun_inDlg::IDD, pParent)
	, m_cstrSO(_T(""))
	, m_cstrEmployee(_T(""))
	, m_PICSItemNameStatic(NULL)
	, m_PICSItemEdit(NULL)
	, m_Toolver(_T("2.007"))
	, m_curColor(GetSysColor(COLOR_BTNFACE))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	::AfxOleInit();
	::CoInitializeEx (NULL, COINIT_MULTITHREADED);

	GetModuleFileName (NULL, this->m_ModulePath.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec (this->m_ModulePath.GetBuffer(MAX_PATH));
	this->m_ModulePath.ReleaseBuffer();
}

void CRun_inDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PASS_BUTTON, m_Pass_Button);
	DDX_Control(pDX, IDC_FAIL_BUTTON, m_Fail_Button);
}

BEGIN_MESSAGE_MAP(CRun_inDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PASS_BUTTON, &CRun_inDlg::OnBnClickedPassButton)
	ON_BN_CLICKED(IDC_FAIL_BUTTON, &CRun_inDlg::OnBnClickedFailButton)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CRun_inDlg message handlers

BOOL CRun_inDlg::OnInitDialog()
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

	m_brush.CreateSolidBrush(RGB(215,240,198));//生成一绿色刷子

	 CFont font,*pOldFont;
	 font.CreateFont(65, //font hight 
	  40, // font width 
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
	

	 m_Pass_Button.SetFont(&font);
	 m_Fail_Button.SetFont(&font);

	// pOldFont=pDC->SelectObject(&font); 


	// TODO: Add extra initialization here
	#ifdef FACTORYMODE
	if(!this->CallSODialog())
	{
		CDialog::OnCancel();
		return TRUE;
	}
	#endif

	if(!this->InitCIM())
	{
		UpdateEmloyee();
		CDialog::OnCancel();
		return TRUE;
	}

	if(!this->InitUI())
	{
		UpdateEmloyee();
		CDialog::OnCancel();
		return TRUE;
	}

	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CRun_inDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRun_inDlg::OnPaint()
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
HCURSOR CRun_inDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRun_inDlg::OnBnClickedPassButton()
{
	// TODO: Add your control notification handler code here
	this->runCIM(true);	
}

void CRun_inDlg::OnBnClickedFailButton()
{
	// TODO: Add your control notification handler code here
	this->runCIM(false);
}

BOOL CRun_inDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_RETURN:
			{
				UINT unID = this->GetFocus()->GetDlgCtrlID();

				//Focus on DeviceInput CEdit currently?
				if(IDC_DYNAMIC_CREATE_EDIT <= unID && IDC_DYNAMIC_CREATE_EDIT + this->m_PICSItemNameArray.GetSize() > unID)
				{
					//Circular set focus to next cedit
					IDC_DYNAMIC_CREATE_EDIT + this->m_PICSItemNameArray.GetSize() - 1 == unID ? this->GetDlgItem(IDC_DYNAMIC_CREATE_EDIT)->SetFocus(): 
																								this->GetDlgItem(unID + 1)->SetFocus();			
				}

				//Get current cedit handle
				CEdit *pCurrentEdit = (CEdit *)this->GetFocus();
				//For select all
				pCurrentEdit->SetSel(0, -1);	

				pMsg->wParam = NULL;

				break;
			}

			case VK_ESCAPE:
				pMsg->wParam = NULL;

				break;

			default:
				break;
		}
	}

	else if(pMsg->message == WM_LBUTTONUP)
	{
		UINT unID = this->GetFocus()->GetDlgCtrlID();
		
		CButton *PassButton = (CButton *)this->GetDlgItem(IDC_PASS_BUTTON);
		CButton *FailButton = (CButton *)this->GetDlgItem(IDC_FAIL_BUTTON);

		//it means click one checkbox
		if(unID >= IDC_DYNAMIC_CREATE_CHECKBOX && unID < IDC_DYNAMIC_CREATE_CHECKBOX + this->m_PICSItemNameArray.GetSize())
		{
			CButton *ThisCheckBox = (CButton *)this->GetDlgItem(unID);

			if(ThisCheckBox->GetCheck())
			{			
				PassButton->EnableWindow(TRUE);
				FailButton->EnableWindow(FALSE);
			}
			
			else
			{
				PassButton->EnableWindow(FALSE);
				FailButton->EnableWindow(TRUE);
			}

			for (int i = 0;i < this->m_PICSItemNameArray.GetSize();i++)
			{
				if(unID != IDC_DYNAMIC_CREATE_CHECKBOX + i)
				{
					this->m_PICSItemCheck[i].SetCheck(BST_UNCHECKED);
				}
			}

			
		}
	}

	return CDialog::PreTranslateMessage(pMsg); 
}


void CRun_inDlg::runCIM(bool res)
{
	char szErrcode[512] = {0}, szErrmsg[512] = {0};

	//AfxMessageBox("RunCIM  begin", MB_OK);

	if(!this->m_CIM.Connect())
	{
		this->ShowLargeMsg(_T("Connect DB fail"));
		return;
	}
  

	//AfxMessageBox("RunCIM  Connect", MB_OK);

	CString Item;
	char sz_Picaso[50];
	memset(sz_Picaso,0,50);
	//make sure nothing empty
	for(int i = 0;i < this->m_PICSItemNameArray.GetSize();i++)
	{
		this->GetDlgItemText(IDC_DYNAMIC_CREATE_EDIT + i, Item);
		
		if(Item.IsEmpty())
		{
			this->ShowLargeMsg(_T("Please scan all item barcode"));
			return;
		}
	}
	
	CString Picasso;
	this->GetDlgItemText(IDC_DYNAMIC_CREATE_EDIT, Picasso);
	Picasso.MakeUpper();

//=====================================================================================
    CString str_Model;
	this->GetDlgItemText(IDC_MODEL_EDIT,str_Model);
	str_Model.MakeUpper();

	char sz_status[30]={0};
	char sz_holdreas[30]={0};
    


    if(!this->m_CIM.GetHoldInformation(Picasso.GetBuffer(),str_Model.GetBuffer(),"RUN_IN",sz_status,sz_holdreas,szErrcode,szErrmsg))
	{
		return;
	}
    Picasso.ReleaseBuffer();
	str_Model.ReleaseBuffer();
//===========================================================================================

	//AfxMessageBox("Query DB  begin", MB_OK);
	if(!this->m_CIM.QueryDB(this->m_cstrSO.GetString(), this->m_cstrEmployee.GetString(), szErrcode, szErrmsg) || 
	   !this->m_CIM.CheckRoute(Picasso.GetBuffer(), szErrcode, szErrmsg))
	{
		CString temp(szErrcode);
		temp += szErrmsg;
		this->ShowLargeMsg(temp.GetString());
		//AfxMessageBox("CheckRoute Fail", MB_OK);
	}
	else
	{
		//bool ret;

		//AfxMessageBox("CheckRoute OK", MB_OK);

		//set picasso
		char szErrCode[512];
		char szErrMsg[512];
		memset(szErrCode,0,512);
		memset(szErrMsg,0,512);
		//AfxMessageBox("Check SO begin", MB_OK);
		bool b_res = Check_SO(Picasso.GetBuffer(),szErrCode, szErrMsg);
		Picasso.ReleaseBuffer();
		if(!b_res)
		{
			CString temp(szErrCode);
		    temp += szErrMsg;
		    this->ShowLargeMsg(temp.GetString());
			//AfxMessageBox("Check SO Fail", MB_OK);
			return;
		}
		this->m_CIM.SetPICS(Picasso.GetString());
		this->SetDlgItemText(IDC_DYNAMIC_CREATE_EDIT, _T(""));

		this->m_CIM.SetPICS(this->m_Toolver.GetString());

		for(int i = 1;i < this->m_PICSItemNameArray.GetSize();i++)
		{
			this->GetDlgItemText(IDC_DYNAMIC_CREATE_EDIT + i, Item);
			Item.MakeUpper();
			this->m_CIM.SetPICS(Item.GetString());
			this->SetDlgItemText(IDC_DYNAMIC_CREATE_EDIT + i, _T(""));
		}

		this->UpdateCSV(Picasso, res);
	}
    

	this->m_PICSItemEdit[0].SetFocus();

	//uncheck all radio button
	for(int i = 0;i < this->m_PICSItemNameArray.GetSize();i++)
	{
		this->m_PICSItemCheck[i].SetCheck(BST_UNCHECKED);
	}

	this->m_CIM.Disconnect();
}

bool CRun_inDlg::Check_SO(char* sz_picaso, char *szErrCode, char *szErrMsg)
{
	bool bRet = true;

	bRet = this->m_CIM.CheckSO(sz_picaso,szErrCode,szErrMsg);

	return bRet;
}

bool CRun_inDlg::SetUIIDParameter(st_UIIDparameter st_idParameter)
{
	this->m_cstrSO = st_idParameter.str_so;
	this->m_cstrEmployee = st_idParameter.str_employee;

	if(this->m_cstrSO.IsEmpty())
	{
		CString str_message = _T("");
		str_message = _T("Entrance tool 传递的参数SO 为空!\nThe parameters SO Transferred from Entrance tool is empty!");
		AfxMessageBox(str_message, MB_OK);
		return FALSE;
	}

	if(this->m_cstrEmployee.IsEmpty())
	{
		CString str_message = _T("");
		str_message = _T("Entrance tool 传递的参数EmployeeID 为空!\nThe parameters EmployeeID Transferred from Entrance tool is empty!");
		AfxMessageBox(str_message, MB_OK);
		return FALSE;
	}

	return true;
}

bool CRun_inDlg::CallSODialog()
{
	HINSTANCE hDialogResDll = NULL, hOldResDll = NULL;
	hDialogResDll = LoadLibrary(_T("DialogResources.dll"));

	if(NULL == hDialogResDll)
	{
		::AfxMessageBox(_T("Load DialogResources.dll fail"));
		return false;
	}
	
	//Save current resource handle
	hOldResDll = AfxGetResourceHandle(); 
	
	//Set current resource to Dialogs.dll, this exe use the dll resource now
	AfxSetResourceHandle(hDialogResDll);

	CSODlg SODlg(this);

	bool SORet = IDOK == SODlg.DoModal();
	
	this->m_cstrSO = SODlg.GetSONumber();
	this->m_cstrEmployee = SODlg.GetEmployeeID();

	//Rollback to origin resource
	AfxSetResourceHandle(hOldResDll); 

	return SORet;
}

void CRun_inDlg::ShowLargeMsg(CStringA Msg, CStringA Caption, COLORREF CaptionColor)
{
	HINSTANCE hDialogResDll = NULL, hOldResDll = NULL;
	hDialogResDll = LoadLibrary(_T("DialogResources.dll"));
	
	if(NULL == hDialogResDll)
	{
		::AfxMessageBox(_T("Load DialogResources.dll fail"));
		return;
	}

	//Save current resource handle
	hOldResDll = AfxGetResourceHandle(); 

	//Set current resource to Dialogs.dll, this exe use the dll resource now
	AfxSetResourceHandle(hDialogResDll);

	CLargeMsgBox LargeMsgBox(this);

	LargeMsgBox.SetCaption(Caption);
	LargeMsgBox.SetCaptionColor(CaptionColor);
	LargeMsgBox.SetMessage(Msg);

	LargeMsgBox.DoModal();

	//Rollback to origin resource
	AfxSetResourceHandle(hOldResDll); 
}

bool CRun_inDlg::InitCIM()
{
	char szErrorMsg[DefaultStringLength] = {""}, szErrorCode[DefaultStringLength] = {""};

	if(!this->m_CIM.LoadDBAccess(szErrorMsg))
	{
		this->ShowLargeMsg(szErrorMsg);
		return false;
	}

	if(!this->m_CIM.Connect())
	{
		this->ShowLargeMsg("Connect CIM fail");
		return false;
	}

	if(!this->m_CIM.QueryDB(this->m_cstrSO.GetString(), this->m_cstrEmployee.GetString(), szErrorCode, szErrorMsg))
	{
		this->ShowLargeMsg(szErrorMsg);
		return false;
	}

	this->m_CIM.Disconnect();

	//Get PICS item
	this->m_PICSItemCount = this->m_CIM.GetPicsItemCount();
	
	if(0 == this->m_PICSItemCount)
	{
		this->ShowLargeMsg("Get no PICS item");
		return false;
	}

	this->m_PICSItemNameArray.RemoveAll();

	for(int i = 0;i < this->m_PICSItemCount;i++)
	{
		CStringA Item;
		this->m_CIM.GetPicsItem(i, Item.GetBuffer(DefaultStringLength));
		Item.ReleaseBuffer();

		this->m_PICSItemNameArray.Add(Item);
	}

	//Remove toolver
	this->m_PICSItemNameArray.RemoveAt(1);

	return true;
}

bool CRun_inDlg::InitUI()
{
	//Set info
	CStringA DBInfo;
	char szErrorMsg[DefaultStringLength] = {""}, szErrorCode[DefaultStringLength] = {""};

	//SO static
	this->SetDlgItemText(IDC_SO_EDIT, this->m_cstrSO);
	this->SetDlgItemText(IDC_EMPLOYEE_EDIT, this->m_cstrEmployee);
	
	//SO type static
	this->m_CIM.GetDBInfo(DBI_CRTYPE, DBInfo.GetBuffer(DefaultStringLength), szErrorCode, szErrorMsg);
	DBInfo.ReleaseBuffer();
	if(DBInfo==_T(""))
	{
	this->m_CIM.GetDBInfo(DBI_SOTYPE, DBInfo.GetBuffer(DefaultStringLength), szErrorCode, szErrorMsg);
	DBInfo.ReleaseBuffer();
	}
	else
	{
		this->SetDlgItemText(IDC_SOTYPE_STATIC,_T("CR type"));
	}
	this->SetDlgItemText(IDC_SOTYPE_EDIT, DBInfo);

	//Model static
	this->m_CIM.GetDBInfo(DBI_MODEL, DBInfo.GetBuffer(DefaultStringLength), szErrorCode, szErrorMsg);
	DBInfo.ReleaseBuffer();
	this->SetDlgItemText(IDC_MODEL_EDIT, DBInfo);

	//PC name static
	this->m_CIM.GetDBInfo(DBI_PCNAME, DBInfo.GetBuffer(DefaultStringLength), szErrorCode, szErrorMsg);
	DBInfo.ReleaseBuffer();
	this->SetDlgItemText(IDC_PCNAME_EDIT, DBInfo);

	//Employee lv static
	this->m_CIM.GetDBInfo(DBI_EMPLOYEELV, DBInfo.GetBuffer(DefaultStringLength), szErrorCode, szErrorMsg);
	DBInfo.ReleaseBuffer();
	this->SetDlgItemText(IDC_EMPLOYEELV_EDIT, DBInfo);

	//RD level
	if(0 == DBInfo.Compare(_T("10")))
	{
		this->m_curColor = RDPermissionColor;
		this->Invalidate();
		this->UpdateWindow();
	}

	this->SetDlgItemText(IDC_TOOL_VER_EDIT, this->m_Toolver);

	//Create PICS static and edit
	this->m_PICSItemNameStatic = new CStatic [this->m_PICSItemCount];
	this->m_PICSItemEdit = new CEdit [this->m_PICSItemCount];
	this->m_PICSItemCheck = new CButton [this->m_PICSItemCount];

	CRect SORec, InfoGBRec, ToolverRec;

	//Get SO static rect, alignment to its left
	CStatic *SOStatic = (CStatic *)this->GetDlgItem(IDC_SO_STATIC);
	SOStatic->GetWindowRect(&SORec);
	ScreenToClient(&SORec);
	
	//Get info group box height, take position under info group box as start top position
	CStatic *InfoStatic = (CStatic *)this->GetDlgItem(IDC_INFO_STATIC);
	InfoStatic->GetClientRect(&InfoGBRec);
	int Top = InfoGBRec.bottom + 200;

	//Items alignment to its right
	CEdit *ToolverEdit = (CEdit *)this->GetDlgItem(IDC_TOOL_VER_EDIT);
	ToolverEdit->GetWindowRect(&ToolverRec);
	ScreenToClient(&ToolverRec);

	//create pics item
	for(int i = 0;i < this->m_PICSItemNameArray.GetSize();i++)
	{
		this->m_PICSItemCheck[i].Create(_T(""), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, CRect(SORec.left, Top + i * 40 + 20, SORec.left + 20, Top + 45 + i * 40), this, IDC_DYNAMIC_CREATE_CHECKBOX + i);
		this->m_PICSItemNameStatic[i].Create(this->m_PICSItemNameArray[i], WS_CHILD | WS_VISIBLE, CRect(SORec.left + 30,  Top + i * 40 + 20, SORec.left + 130, Top + 45 + i * 40), this, IDC_DYNAMIC_CREATE_STATIC + i);
		this->m_PICSItemEdit[i].Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, CRect(SORec.left + 160, Top + i * 40 + 20, ToolverRec.right, Top + 45 + i * 40), this, IDC_DYNAMIC_CREATE_EDIT + i);
	}

	CRect PassButtonRec, FailButtonRec;
	CButton *PassButton = (CButton *)this->GetDlgItem(IDC_PASS_BUTTON);
	CButton *FailButton = (CButton *)this->GetDlgItem(IDC_FAIL_BUTTON);

	PassButton->GetWindowRect(&PassButtonRec);
	FailButton->GetWindowRect(&FailButtonRec);

	ScreenToClient(&PassButtonRec);
	ScreenToClient(&FailButtonRec);

	//Resize dialog
	CRect ResizeDialogRec;
	this->GetWindowRect(&ResizeDialogRec);
	ResizeDialogRec.bottom = InfoGBRec.bottom + 30 + //Info group box height
							 (this->m_PICSItemNameArray.GetSize() - 1) * 40	+ //Items height
							 PassButtonRec.Height() + //Button height
							 300; //more space
		//ResizeDialogRec.bottom = InfoGBRec.bottom + 30 + //Info group box height
		//					 (4 - 1) * 40	+ //Items height
		//					 PassButtonRec.Height() + //Button height
		//					 120; //more space

	this->MoveWindow(&ResizeDialogRec);
	
	//Move pass and fail button
	int ButtonH = PassButtonRec.Height();

	//Make button position under pics item 40 pixels
	PassButtonRec.top = InfoGBRec.bottom + 30 + (this->m_PICSItemNameArray.GetSize() - 1) * 40 + 230;
	FailButtonRec.top = InfoGBRec.bottom + 30 + (this->m_PICSItemNameArray.GetSize() - 1) * 40 + 230;
	//FailButtonRec.top = InfoGBRec.bottom + 30 + (4 - 1) * 40 + 70;

	PassButtonRec.bottom = PassButtonRec.top + ButtonH;
	FailButtonRec.bottom = FailButtonRec.top + ButtonH;

	PassButton->MoveWindow(PassButtonRec);
	FailButton->MoveWindow(FailButtonRec);

	this->m_PICSItemEdit[0].SetFocus();
	
	return true;
}

bool CRun_inDlg::InitialUITitle()
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
     CString str_toolname = _T(" Run_In V2.007");
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


HBRUSH CRun_inDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
	//if(CTLCOLOR_DLG == nCtlColor)
	//{
	//	return ::CreateSolidBrush(this->m_curColor);	
	//}

	//return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode(TRANSPARENT);
    return m_brush;
}

void CRun_inDlg::OnDestroy()
{
	if (NULL != this->m_PICSItemNameStatic)
	{
		delete [] this->m_PICSItemNameStatic;
	}

	if (NULL != this->m_PICSItemEdit)
	{
		delete [] this->m_PICSItemEdit;
	}

	__super::OnDestroy();
}

bool CRun_inDlg::UpdateCSV(CString Picasso, bool Result)
{
	//Read all content
	CString CSVPath;
	UINT Duration, Retry;

	if(!this->ReadIni(CSVPath, Duration, Retry))
		return false;

	//Open csv file
	CStdioFile CSVfile;
	if(!this->OpenCSVFile(CSVfile, CSVPath, Retry))
		return false;

	//Read content
	std::vector<CString> vFileContent;
	if(!this->ReadCSV(CSVfile, vFileContent))
	{
		CSVfile.Close();
		return false;
	}

	//find start time if picasso match
	CString RunIn_StartTime;
	if(!this->FindRecord(vFileContent, RunIn_StartTime, Picasso))
	{
		CSVfile.Close();
		return false;
	}
	
	//Check time stamp > duration?
	if(!this->CheckTimeStamp(RunIn_StartTime, Duration))
	{
		CSVfile.Close();
		return false;
	}

	//Update CIM
	if(!this->UpdateCIM(Result))
	{
		CSVfile.Close();
		return false;
	}

	//update record
	CSVfile.Seek(0, CFile::begin);
	CSVfile.SetLength(0);
	for (unsigned int i=0; i < vFileContent.size(); ++i)
	{
		CSVfile.WriteString (vFileContent[i] + "\n");
	}

	CSVfile.Close();

	return true;
}

bool CRun_inDlg::ReadIni(CString &CSVPath, UINT &Duration, UINT &Retry)
{
	//ini full path
	CString inifile(this->m_ModulePath);
	PathAppend(inifile.GetBuffer(MAX_PATH), INI_FILENAME);
	inifile.ReleaseBuffer();

	//CSV path
	::GetPrivateProfileString(_T("Setup"), _T("SavePath"), _T(""), CSVPath.GetBuffer(MAX_PATH), MAX_PATH, inifile.GetString());
	CSVPath.ReleaseBuffer();

	//Test time duration
	Duration = ::GetPrivateProfileInt(_T("Setup"), _T("Duration"), 0, inifile.GetString());

	Retry = ::GetPrivateProfileInt(_T("Setup"), _T("Retry"), 0, inifile.GetString());

	if(CSVPath.IsEmpty() || 0 == Duration || 0 == Retry)
	{
		this->ShowLargeMsg(_T("Ini setting error"));
		return false;
	}

	return true;
}

bool CRun_inDlg::OpenCSVFile(CStdioFile &File, CString CSVPath, UINT Retry)
{
	bool FileOpenSuccess = false;

	//retry until success
	while(Retry-- > 0)
	{
		if(File.Open(CSVPath.GetString(), CFile::modeReadWrite|CFile::modeNoTruncate, NULL))
		{
			FileOpenSuccess = true;
			break;
		}

		Sleep(1000);
	}

	if(!FileOpenSuccess)
	{
		this->ShowLargeMsg("Open CSV file fail, please retry");
		return false;
	}

	return true;
}

bool CRun_inDlg::ReadCSV(CStdioFile &File, std::vector<CString> &vFileContent)
{
	CString cstrRead;
	while (File.ReadString (cstrRead))
	{
		vFileContent.push_back (cstrRead);
	}

	if(vFileContent.empty())
	{
		this->ShowLargeMsg(_T("CSV file has no record!"));
		return false;
	}

	return true;
}

bool CRun_inDlg::FindRecord(std::vector<CString> &vFileContent, CString &RunIn_StartTime, CString Picasso)
{
	std::vector<CString>::iterator Iter;
	CString tempPicasso;
	Iter = vFileContent.begin();

	while (Iter != vFileContent.end())
	{
		//first token is picasso
		int Pos = 0;
		tempPicasso = Iter->Tokenize (",", Pos);

		if (tempPicasso.CompareNoCase (Picasso) == 0)
		{
			//erase it for update
			int Last = Iter->ReverseFind(',');

			RunIn_StartTime = Iter->Right(Iter->GetLength() - Last - 1);

			Iter = vFileContent.erase(Iter);
			continue;
		}

		Iter++;
	}

	//not found record
	if(RunIn_StartTime.IsEmpty())
	{
		this->ShowLargeMsg(_T("Not found record in CSV record"));
		return false;
	}

	return true;
}

bool CRun_inDlg::CheckTimeStamp(CString RunIn_StartTime, UINT Duration)
{
	CString Year, Month, Day, Hour, Minute, Second;
	
	//Timestamp format would be %4dYEAR %2dMONTH %2dDAY %2dHOUR %2dMININUTE %2dSECOND like 20111012104123

	//first 4 char->year
	Year = RunIn_StartTime.Left(4);
	RunIn_StartTime = RunIn_StartTime.Right(10);

	Month = RunIn_StartTime.Left(2);
	RunIn_StartTime = RunIn_StartTime.Right(8);

	Day = RunIn_StartTime.Left(2);
	RunIn_StartTime = RunIn_StartTime.Right(6);

	Hour = RunIn_StartTime.Left(2);
	RunIn_StartTime = RunIn_StartTime.Right(4);

	Minute = RunIn_StartTime.Left(2);
	RunIn_StartTime = RunIn_StartTime.Right(2);

	Second = RunIn_StartTime;

	//Get current time
	time_t osTime;
	time(&osTime) ;

	CTime CurrentTime(osTime);
	CTime StartTime(atoi(Year.GetString()),
					atoi(Month.GetString()),
					atoi(Day.GetString()),
					atoi(Hour.GetString()),
					atoi(Minute.GetString()),
					atoi(Second.GetString()));

	CTimeSpan TestDuration = CurrentTime - StartTime;

	if(TestDuration < Duration)
	{
		this->ShowLargeMsg(_T("Test time not exceed duration setting in ini file"));
		return false;
	}

	return true;
}

bool CRun_inDlg::UpdateCIM(bool Result)
{
	bool ret;

	char szErrcode[512] = {0}, szErrmsg[512] = {0};
	CStringA thisErrorCode;

	for(int i = 0;i < this->m_PICSItemNameArray.GetSize();i++)
	{
		if(this->m_PICSItemCheck[i].GetCheck())
		{
			thisErrorCode.Format("RI%4d", i);
			thisErrorCode.Replace(' ', '0');
			break;
		}
	}

	Result ? ret = this->m_CIM.Write(Result, "", szErrcode, szErrmsg) : 
			 ret = this->m_CIM.Write(Result, thisErrorCode.GetBuffer(), szErrcode, szErrmsg);

	if(ret)
	{
		this->ShowLargeMsg("Update CIM success", "PASS", PassCaptionColor);
	}

	else
	{
		CString temp(szErrcode);
		temp += szErrmsg;
		this->ShowLargeMsg(temp.GetString());
	}

	return ret;
}


void CRun_inDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
    UpdateEmloyee();

	CDialog::OnClose();
}

void CRun_inDlg::UpdateEmloyee()
{
	char szErrcode[512] = {0}, szErrmsg[512] = {0};

	if(!this->m_CIM.Connect())
	{
		this->ShowLargeMsg(_T("Connect DB fail"));
		return;
	}


	if(!this->m_CIM.UpdateEmployeeIDStatus(szErrcode, szErrmsg))
	{
		CString temp(szErrcode);
		temp += szErrmsg;
		this->ShowLargeMsg(temp.GetString());
	}


	this->m_CIM.Disconnect();

}