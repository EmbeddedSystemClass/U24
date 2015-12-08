// PassWord.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_FacTestTool.h"
#include "PassWord.h"


// CPassWord dialog

IMPLEMENT_DYNCREATE(CPassWord, CDialog)

CPassWord::CPassWord(CWnd* pParent /*=NULL*/)
	: CDialog(CPassWord::IDD, pParent)
{
	
}

CPassWord::~CPassWord()
{
}

void CPassWord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_edit_passWord);
	DDX_Control(pDX, IDC_STATIC_ERROR, m_lb_error);
}

BEGIN_MESSAGE_MAP(CPassWord, CDialog)
	ON_BN_CLICKED(IDOK, &CPassWord::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPassWord::OnBnClickedCancel)
END_MESSAGE_MAP()

void CPassWord::OnBnClickedOk()
{
	CString PassWord;
	m_edit_passWord.GetWindowText(PassWord);
	if(PassWord != _T("1829"))
	{	
		m_edit_passWord.SetWindowText(_T(""));
		m_lb_error.SetText(_T("Password Error!"));
		m_lb_error.SetFontBold(TRUE);
		m_lb_error.SetTextColor(RGB(255,0,0));
	}
	else
	{	
		CDialog::OnOK();
	}
}

void CPassWord::OnBnClickedCancel()
{
	OnCancel();
}
