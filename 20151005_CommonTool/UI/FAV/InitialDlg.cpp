// InitialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FAV.h"
#include "InitialDlg.h"

// CInitialDlg dialog

IMPLEMENT_DYNAMIC(CInitialDlg, CDialog)

CInitialDlg::CInitialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInitialDlg::IDD, pParent)
{
}

CInitialDlg::~CInitialDlg()
{
}

void CInitialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInitialDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInitialDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CInitialDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CInitialDlg message handlers

void CInitialDlg::OnBnClickedOk()
{
	GetDlgItem(IDC_EDIT_LOT)->GetWindowText(m_strLotNumber);
	GetDlgItem(IDC_EDIT_LOTCOUNT)->GetWindowText(m_strLotCount);
	
	if(m_strLotNumber == _T("") || m_strLotCount == _T(""))
	{
		AfxMessageBox(_T("Please enter all items first!"));
		return;
	}

	OnOK();
}

void CInitialDlg::OnBnClickedCancel()
{
	OnCancel();
}

BOOL CInitialDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE; 
}
