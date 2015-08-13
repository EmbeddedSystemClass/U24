// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "InputDlg.h"

// CInputDlg dialog

IMPLEMENT_DYNAMIC(CInputDlg, CDialog)

CInputDlg::CInputDlg(const CString& strPrompt,const CString& strDefaultPath,CWnd* pParent /*=NULL*/)
: CDialog(CInputDlg::IDD, pParent)
,m_strPrompt(strPrompt)
,m_szInput(strDefaultPath)
{

}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT_EDIT, m_szInput);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInputDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_BROWN, &CInputDlg::OnBnClickedButtonBrown)
END_MESSAGE_MAP()

// CInputDlg message handlers
BOOL CInputDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();
	GetDlgItem(IDC_PROMPT)->SetWindowText(m_strPrompt);
	this->UpdateData(FALSE);

	return TRUE;
}

void CInputDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	OnOK();
}

// CInputDlg message handlers

void CInputDlg::OnBnClickedButtonBrown()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	TCHAR szDir[MAX_PATH] = {0};

	BROWSEINFO bi;

	ITEMIDLIST *pidl;

	bi.hwndOwner = this->m_hWnd;

	bi.pidlRoot = NULL;

	bi.pszDisplayName = m_szInput.GetBuffer();

	bi.lpszTitle = _T("Select the folder");

	bi.ulFlags = BIF_STATUSTEXT | BIF_USENEWUI | BIF_RETURNONLYFSDIRS;

	bi.lpfn = NULL;

	bi.lParam = 0;

	bi.iImage = 0;

	pidl = SHBrowseForFolder(&bi);

	if(pidl == NULL)  return;

	if(!SHGetPathFromIDList(pidl, szDir))   return;

	else  m_szInput = szDir;

	UpdateData(FALSE);    


}
