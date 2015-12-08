
#include "stdafx.h"
#include <vector>
#include "Run_in.h"
#include "Run_inDlg.h"
#include "IniAccess.h"

#define INI_FILENAME			"Run_In_Start.ini"

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

CRun_inDlg::CRun_inDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRun_inDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_brush.CreateSolidBrush(RGB(215,240,198));
}

CRun_inDlg::~CRun_inDlg()
{
	/* Delete Brush */
	m_brush.DeleteObject();
}

void CRun_inDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PICASSO_STATIC, m_static_picasso);
	DDX_Control(pDX, IDC_BATTERY_STATIC, m_static_battery);
	DDX_Control(pDX, IDC_CHARGER_STATIC, m_static_charger);
	DDX_Control(pDX, IDC_USBCABLE_STATIC, m_static_usbcable);

	DDX_Text(pDX, IDC_PICASSO_EDIT, m_str_picasso);
	DDX_Text(pDX, IDC_BATTERY_EDIT, m_str_battery);
	DDX_Text(pDX, IDC_CHARGER_EDIT, m_str_charger);
	DDX_Text(pDX, IDC_USBCABLE_EDIT, m_str_usbcable);
}

BEGIN_MESSAGE_MAP(CRun_inDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_PASS_BUTTON, &CRun_inDlg::OnBnClickedPassButton)
END_MESSAGE_MAP()

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

void CRun_inDlg::OnPaint()
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

bool CRun_inDlg::SetUIIDParameter(st_UIIDparameter st_idParameter)
{
	//this->m_cstrSO = st_idParameter.str_so;
	//this->m_cstrEmployee = st_idParameter.str_employee;

	//if(this->m_cstrSO.IsEmpty())
	//{
	//	CString str_message = _T("");
	//	str_message = _T("Entrance tool 传递的参数SO 为空!\nThe parameters SO Transferred from Entrance tool is empty!");
	//	AfxMessageBox(str_message, MB_OK);
	//	return FALSE;
	//}

	//if(this->m_cstrEmployee.IsEmpty())
	//{
	//	CString str_message = _T("");
	//	str_message = _T("Entrance tool 传递的参数EmployeeID 为空!\nThe parameters EmployeeID Transferred from Entrance tool is empty!");
	//	AfxMessageBox(str_message, MB_OK);
	//	return FALSE;
	//}

	return true;
}

HCURSOR CRun_inDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CRun_inDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG) 
	{
		return m_brush;
	}

	return hbr;
}

BOOL CRun_inDlg::OnInitDialog()
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

	if (ReadIniFile() != true)
	{
		CDialog::OnCancel();
		return FALSE;
	}

	InitUIShow();

	return TRUE;
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
     CString str_toolname = _T("PreRunIN V2.001");
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

void CRun_inDlg::InitUIShow(void)
{
	if (m_i_picassoFlag == 1)
	{
		m_static_picasso.SetTextColor(RGB(0,0,0));
		m_static_picasso.SetBkColor(RGB(215,240,198));
		GetDlgItem(IDC_PICASSO_EDIT)->ShowWindow(TRUE);
	}else
	{
		m_static_picasso.ShowWindow(FALSE);
		GetDlgItem(IDC_PICASSO_EDIT)->ShowWindow(FALSE);
	}

	if (m_i_batteryFlag == 1)
	{
		m_static_battery.SetTextColor(RGB(0,0,0));
		m_static_battery.SetBkColor(RGB(215,240,198));
		GetDlgItem(IDC_BATTERY_EDIT)->ShowWindow(TRUE);
	}else
	{
		m_static_battery.ShowWindow(FALSE);
		GetDlgItem(IDC_BATTERY_EDIT)->ShowWindow(FALSE);
	}

	if (m_i_chargerFlag == 1)
	{
		m_static_charger.SetTextColor(RGB(0,0,0));
		m_static_charger.SetBkColor(RGB(215,240,198));
		GetDlgItem(IDC_CHARGER_EDIT)->ShowWindow(TRUE);
	}else
	{
		m_static_charger.ShowWindow(FALSE);
		GetDlgItem(IDC_CHARGER_EDIT)->ShowWindow(FALSE);
	}

	if (m_i_USBCableFlag == 1)
	{
		m_static_usbcable.SetTextColor(RGB(0,0,0));
		m_static_usbcable.SetBkColor(RGB(215,240,198));
		GetDlgItem(IDC_USBCABLE_EDIT)->ShowWindow(TRUE);
	}else
	{
		m_static_usbcable.ShowWindow(FALSE);
		GetDlgItem(IDC_USBCABLE_EDIT)->ShowWindow(FALSE);
	}

	return;
}

bool CRun_inDlg::ReadIniFile(void)
{
	/* Get current path */
	TCHAR sz_currentPath[MAX_PATH];
	memset(sz_currentPath, 0, sizeof(sz_currentPath));
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
	PathRemoveFileSpec(sz_currentPath);

	/* Check ini file exist */
	CString str_path;
	str_path.Format("%s\\%s", sz_currentPath, INI_FILE_NAME);
	if (_taccess(str_path, 0) != 0) 
	{
		AfxMessageBox(_T("Ini config file do not exist."));
		return false;
	}

	/* Read ini file */
	m_str_savePath = _T("");
	m_i_retryCount = 1;
	m_i_picassoFlag = 0;
	m_i_batteryFlag = 0;
	m_i_chargerFlag = 0;
	m_i_USBCableFlag = 0;

	char sz_buffer[128];
	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("Setup"), _T("SavePath"), _T(""), sz_buffer, sizeof(sz_buffer), str_path);
	m_str_savePath.Format(_T("%s"), sz_buffer);

	CString str_tempFilePath;
	str_tempFilePath = m_str_savePath;
	PathRemoveFileSpec(str_tempFilePath.GetBuffer(str_tempFilePath.GetLength()));
	if (_taccess(str_tempFilePath, 0) != 0) 
	{
		AfxMessageBox(_T("SavePath file path is not exist."));
		return false;
	}
	str_tempFilePath.ReleaseBuffer();

	m_i_retryCount = ::GetPrivateProfileInt("Setup", "RetryCount", 0, str_path);
	m_i_picassoFlag = ::GetPrivateProfileInt("UISetting", "Picasso", 0, str_path);
	m_i_batteryFlag = ::GetPrivateProfileInt("UISetting", "Battery", 0, str_path);
	m_i_chargerFlag = ::GetPrivateProfileInt("UISetting", "Charger", 0, str_path);
	m_i_USBCableFlag = ::GetPrivateProfileInt("UISetting", "USBCable", 0, str_path);

	if (m_i_picassoFlag != 1)
	{
		AfxMessageBox("Picasso must be scan, Please check ini file.");
		return false;
	}

	return true;
}

void CRun_inDlg::OnBnClickedPassButton()
{
	//AfxMessageBox(_T("OK"), MB_OK);
	UpdateData (TRUE);

	if (m_i_picassoFlag == 1)
	{
		if (m_str_picasso.IsEmpty())
		{
			AfxMessageBox(_T("Please scan Picasso!"));
			return;
		}
	}

	if (m_i_batteryFlag == 1)
	{
		if (m_str_battery.IsEmpty())
		{
			AfxMessageBox(_T("Please scan Battery!"));
			return;
		}
	}

	if (m_i_chargerFlag == 1)
	{
		if (m_str_charger.IsEmpty())
		{
			AfxMessageBox(_T("Please scan Charger!"));
			return;
		}
	}

	if (m_i_USBCableFlag == 1)
	{
		if (m_str_usbcable.IsEmpty())
		{
			AfxMessageBox(_T("Please scan USB Cable!"));
			return;
		}
	}

	/* Get time */
	CString csTime = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
	if (!WriteStartTimeToFile(csTime))
	{
		AfxMessageBox("FAIL");
	}
	else
	{
		AfxMessageBox("OK");
	}

	if (m_i_picassoFlag == 1)
	{
		GetDlgItem(IDC_PICASSO_EDIT)->SetWindowText("");
	}

	if (m_i_batteryFlag == 1)
	{
		GetDlgItem(IDC_BATTERY_EDIT)->SetWindowText("");
	}

	if (m_i_chargerFlag == 1)
	{
		GetDlgItem(IDC_CHARGER_EDIT)->SetWindowText("");
	}

	if (m_i_USBCableFlag == 1)
	{
		GetDlgItem(IDC_USBCABLE_EDIT)->SetWindowText("");
	}

	if (m_i_picassoFlag == 1)
	{
		GetDlgItem(IDC_PICASSO_EDIT)->SetFocus();
	}

	return;
}

bool CRun_inDlg::WriteStartTimeToFile(CString csTime)
{
	if (csTime.IsEmpty())
	{
		return false;
	}

	CStdioFile csvFile;
	CString cstrRead;
	std::vector<CString> vFileContent;
	std::vector<CString>::iterator Iter;
	bool bSuccess = false;

	if (m_i_retryCount < 1)
	{
		m_i_retryCount = 1;
	}
	bSuccess = false;
	CFileException e;
	for (int i = 0; i< m_i_retryCount; ++i)
	{
		if (csvFile.Open(m_str_savePath, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate, &e))
		{
			while(csvFile.ReadString(cstrRead))
			{
				vFileContent.push_back(cstrRead);
			}

			csvFile.Close();
			bSuccess = true;
			break;
		}

		Sleep(1000);
	}

	if (!bSuccess)
	{
		return false;
	}

	// search if the same picasso exist.
	CString tempPicasso;
	int nStart = 0;
	Iter = vFileContent.begin();
	while(Iter != vFileContent.end())
	{
		nStart=0;
		tempPicasso = Iter->Tokenize(",", nStart);
		if (tempPicasso.CompareNoCase(m_str_picasso) == 0)
		{
			Iter = vFileContent.erase(Iter);
			continue;
		}
		Iter++;
	}

	// append to the end
	// write "Picasso,battery,charger,20101102235959" to csv
	CString cstrLog;
	if (m_i_picassoFlag == 1)
	{
		cstrLog = m_str_picasso + ",";
	}

	if (m_i_batteryFlag == 1)
	{
		cstrLog += m_str_battery + ",";
	}

	if (m_i_chargerFlag == 1)
	{
		cstrLog += m_str_charger + ",";
	}

	if (m_i_USBCableFlag == 1)
	{
		cstrLog += m_str_usbcable + ",";
	}

	cstrLog += csTime;

	vFileContent.push_back(cstrLog);

	// write back to file
	bSuccess = false;
	for (int i = 0; i< m_i_retryCount; ++i)
	{
		if (csvFile.Open (m_str_savePath, CFile::modeReadWrite|CFile::modeCreate, NULL))
		{
			csvFile.Seek(0, CFile::begin);
			for (unsigned int i=0; i<vFileContent.size(); ++i)
			{
				csvFile.WriteString (vFileContent[i] + "\n");
			}

			csvFile.Close();
			bSuccess = true;
			break;
		}

		Sleep (1000);
	}

	return bSuccess;
}

BOOL CRun_inDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_RETURN:
			{
				CWnd *pFocus = GetFocus();
				UINT unID = pFocus->GetDlgCtrlID();
				UINT unNextID;

				if (unID >= IDC_PICASSO_EDIT && unID <= IDC_PASS_BUTTON)
				{
					this->GetNextDlgTabItem(pFocus)->SetFocus();

					unNextID = GetFocus()->GetDlgCtrlID();
					if (unNextID == IDC_PASS_BUTTON)
					{
						OnBnClickedPassButton();
					}
					else
					{
						CEdit *pCurrentEdit = (CEdit *)this->GetFocus();
						pCurrentEdit->SetSel(0, -1);
					}
				}

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

	return CDialog::PreTranslateMessage(pMsg); 
}


