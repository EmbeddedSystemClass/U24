// ButtonCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_ServiceTool.h"
#include "ButtonCtrl.h"


// CButtonCtrl

IMPLEMENT_DYNAMIC(CButtonCtrl, CButton)

CButtonCtrl::CButtonCtrl()
:m_crColor(0)
{

}

CButtonCtrl::~CButtonCtrl()
{
}


BEGIN_MESSAGE_MAP(CButtonCtrl, CButton)
END_MESSAGE_MAP()



// CButtonCtrl message handlers



void CButtonCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CButtonCtrl::ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void CButtonCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  Add your code to draw the specified item
	::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, DFC_BUTTON, DFCS_BUTTONPUSH);
	
	CString strText;
	GetWindowText(strText);
	
	COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, m_crColor);
	if(strText.GetLength() < 25)
	{
		::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), &lpDrawItemStruct->rcItem, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else
	{
		lpDrawItemStruct->rcItem.top = 5;
		lpDrawItemStruct->rcItem.left = 5;
		::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), &lpDrawItemStruct->rcItem, DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	
	::SetTextColor(lpDrawItemStruct->hDC, crOldColor);
}

int CButtonCtrl::SetTextColor(COLORREF crColor)
{
	m_crColor = crColor;
	Invalidate();
	return 0;
}
