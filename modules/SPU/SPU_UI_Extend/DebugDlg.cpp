// DebugDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DebugDlg.h"


// CDebugDlg dialog

IMPLEMENT_DYNAMIC(CDebugDlg, CDialog)

CDebugDlg::CDebugDlg(const CString& strPrompt, CWnd* pParent /*=NULL*/)
	: CDialog(CDebugDlg::IDD, pParent)
	,m_strPrompt(strPrompt)
{

}

CDebugDlg::~CDebugDlg()
{
}

void CDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDebugDlg, CDialog)
	ON_BN_CLICKED(ID_RUN, &CDebugDlg::OnBnClickedRun)
	ON_BN_CLICKED(ID_STEP, &CDebugDlg::OnBnClickedStep)
	ON_BN_CLICKED(ID_BREAK, &CDebugDlg::OnBnClickedBreak)
	ON_BN_CLICKED(ID_STEPINTO, &CDebugDlg::OnBnClickedStepinto)
END_MESSAGE_MAP()


// CDebugDlg message handlers
BOOL CDebugDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();
	GetDlgItem(IDC_PROMPT)->SetWindowText(m_strPrompt);

	return TRUE;
}

void CDebugDlg::OnBnClickedRun()
{
	m_szInput = _T("F5");
	OnOK();
}

void CDebugDlg::OnBnClickedStep()
{
	m_szInput = _T("F10");
	OnOK();
}

void CDebugDlg::OnBnClickedBreak()
{
	m_szInput = _T("CTRL_BREAK");
	OnOK();
}

void CDebugDlg::OnBnClickedStepinto()
{
	m_szInput = _T("F11");
	OnOK();
}
