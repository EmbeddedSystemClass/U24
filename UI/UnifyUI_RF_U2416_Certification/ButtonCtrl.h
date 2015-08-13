#pragma once


// CButtonCtrl

class CButtonCtrl : public CButton
{
	DECLARE_DYNAMIC(CButtonCtrl)

public:
	CButtonCtrl();
	virtual ~CButtonCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	int SetTextColor(COLORREF crColor);
	COLORREF m_crColor;
};


