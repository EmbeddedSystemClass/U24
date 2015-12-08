// LargeMsgBox.cpp : implementation file
//

#include "stdafx.h"
#include "LargeMsgBox.h"


// CLargeMsgBox dialog

IMPLEMENT_DYNAMIC(CLargeMsgBox, CDialog)

CLargeMsgBox::CLargeMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CLargeMsgBox::IDD, pParent)
	, m_MessageString(_T(""))
	, m_CaptionString(_T(""))
	, m_MessageColor(RGB(0, 38, 255))
	, m_CaptionColor(RGB(255, 0, 0))
	, m_MessageSize(50)
	, m_CaptionSize(100)
	, m_CaptionBold(TRUE)
	, m_MessageBold(TRUE)
{
}

CLargeMsgBox::~CLargeMsgBox()
{
}

void CLargeMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_STATIC_DIALOGS_DLL, m_MessageLabel);
	DDX_Control(pDX, IDC_CAPTION_STATIC_DIALOGS_DLL, m_CaptionLabel);
}


BEGIN_MESSAGE_MAP(CLargeMsgBox, CDialog)
END_MESSAGE_MAP()


void CLargeMsgBox::SetCaptionColor (COLORREF color)
{
	this->m_CaptionColor = color;
}

void CLargeMsgBox::SetMessageColor (COLORREF color)
{
	this->m_MessageColor = color;
}

void CLargeMsgBox::SetCaptionSize (int nSize)
{
	this->m_CaptionSize = nSize;
}

void CLargeMsgBox::SetMessageSize (int nSize)
{
	this->m_MessageSize = nSize;
}

void CLargeMsgBox::SetCaptionBold (BOOL bBold)
{
	this->m_CaptionBold = bBold;
}

void CLargeMsgBox::SetMessageBold (BOOL bBold)
{
	this->m_MessageBold = bBold;
}

void CLargeMsgBox::SetCaption (CString& inMsg)
{
	this->m_CaptionString = inMsg;
}

void CLargeMsgBox::SetMessage (CString& inMsg)
{
	this->m_MessageString = inMsg;
}

BOOL CLargeMsgBox::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	this->m_CaptionLabel.SetTextColor(this->m_CaptionColor);
	this->m_CaptionLabel.SetFontBold (this->m_CaptionBold);
	this->m_CaptionLabel.SetFontSize (this->m_CaptionSize);
	this->m_CaptionLabel.SetWindowText (this->m_CaptionString);

	this->m_MessageLabel.SetTextColor(this->m_MessageColor);
	this->m_MessageLabel.SetFontBold (this->m_MessageBold);
	this->m_MessageLabel.SetFontSize (this->m_MessageSize);
	this->m_MessageLabel.SetWindowText (this->m_MessageString);

	return TRUE;
}