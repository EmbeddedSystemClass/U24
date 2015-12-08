// Popup.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_FacTestTool.h"
#include "Popup.h"


// CPopup dialog

IMPLEMENT_DYNAMIC(CPopup, CDialog)

CPopup::CPopup(CWnd* pParent /*=NULL*/)
	: CDialog(CPopup::IDD, pParent)
{

}

CPopup::~CPopup()
{
}

void CPopup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_label_message);
}


BEGIN_MESSAGE_MAP(CPopup, CDialog)
END_MESSAGE_MAP()

void CPopup::SetText( CString str_text )
{
	m_str_msg = str_text;
}

BOOL CPopup::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_label_message.SetBkColor(COLOR_WINDOW);
	m_label_message.SetTextColor(RGB(0,0,255));
	m_label_message.SetFontBold(TRUE);
	m_label_message.SetFontSize(32);
	m_label_message.SetWindowText(m_str_msg);
	//SetWindowPos(NULL,1,1,500,300,SWP_SHOWWINDOW);

	return TRUE;
}


// CPopup message handlers

