// SODlg.cpp : implementation file
//

#include "stdafx.h"
#include "SODlg.h"

#define EMPLOYEEID_LEN		7

// CSODlg dialog

IMPLEMENT_DYNAMIC(CSODlg, CDialog)

CSODlg::CSODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSODlg::IDD, pParent)
	, EmployeeID(_T(""))
	, SONumber(_T(""))
{

}

CSODlg::~CSODlg()
{
}

void CSODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EMPLOYEE_EDIT_DIALOGS_DLL, EmployeeID);
	DDX_Text(pDX, IDC_SO_EDIT_DIALOGS_DLL, SONumber);
}

BEGIN_MESSAGE_MAP(CSODlg, CDialog)
	ON_BN_CLICKED(IDOK_DIALOGS_DLL, &CSODlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL_DIALOGS_DLL, &CSODlg::OnBnClickedCancel)
END_MESSAGE_MAP()

void CSODlg::OnBnClickedOk()
{
	this->UpdateData (TRUE);

	if(this->SONumber.IsEmpty())
	{
		::AfxMessageBox(_T("SO number can't be empty"));
		this->GetDlgItem(IDC_SO_EDIT_DIALOGS_DLL)->SetFocus();
	}

	else if (this->EmployeeID.IsEmpty())
	{
		::AfxMessageBox(_T("Employee can't be empty"));
		this->GetDlgItem(IDC_EMPLOYEE_EDIT_DIALOGS_DLL)->SetFocus();
	}

	else
		OnOK();	
}

void CSODlg::OnBnClickedCancel()
{
	OnCancel();	
}

const CString CSODlg::GetEmployeeID() const
{
	return this->EmployeeID;
}

const CString CSODlg::GetSONumber() const
{
	return this->SONumber;
}