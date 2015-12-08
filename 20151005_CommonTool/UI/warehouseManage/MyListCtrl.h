#pragma once

// CMyListCtrl

class CMyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)

public:
	CMyListCtrl();
	virtual ~CMyListCtrl();
	void SetAllItemColor(DWORD iItem,COLORREF TextBkColor);
	void CMyListCtrl::SetItemColor(DWORD iItem, COLORREF color);
	void CMyListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	DECLARE_MESSAGE_MAP()
	CMap<DWORD , DWORD& , COLORREF , COLORREF&> MapItemColor;
};


