// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyListCtrl.h"


// CMyListCtrl

IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

CMyListCtrl::CMyListCtrl()
{

}

CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
END_MESSAGE_MAP()



// CMyListCtrl message handlers

void CMyListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    *pResult = CDRF_DODEFAULT;
    NMLVCUSTOMDRAW * lplvdr=(NMLVCUSTOMDRAW*)pNMHDR;
    NMCUSTOMDRAW &nmcd = lplvdr->nmcd;
    switch(lplvdr->nmcd.dwDrawStage)//判断状态
        {
            case CDDS_PREPAINT:
           {
               *pResult = CDRF_NOTIFYITEMDRAW;
               break;
           }
               case CDDS_ITEMPREPAINT://如果为画ITEM之前就要进行颜色的改变
           {
               COLORREF ItemColor;
               if(MapItemColor.Lookup(nmcd.dwItemSpec, ItemColor))
		//根据在SetItemColor(DWORD iItem, COLORREF color) 设置的//ITEM号和COLORREF 在摸板中查找，然后进行颜色赋值。
               {
                   //lplvdr->clrText = RGB(0,0,0);//ItemColor;
                   lplvdr->clrTextBk = ItemColor;
                   *pResult = CDRF_DODEFAULT;
               }
           }
           break;
    }
}

void CMyListCtrl::SetItemColor(DWORD iItem, COLORREF color)
{
    //    m_iCurrentItem = iItem;
    //    m_CurrentColor = color;
    MapItemColor.SetAt(iItem, color);//设置某行的颜色。
    this->RedrawItems(iItem, iItem);//重新染色
    //this->SetCheck(iItem,1);
    //this->SetFocus();//设置焦点
    UpdateWindow();
}

void CMyListCtrl::SetAllItemColor(DWORD iItem,COLORREF TextBkColor)
{
    //INT_PTR ncount = this->GetItemCount();
    if(iItem > 0)
    {
            for(DWORD numItem = 0; numItem < iItem ;numItem ++)
            {
               //iItem = i;
               MapItemColor.SetAt(numItem, TextBkColor);
               this->RedrawItems(numItem, numItem);
            }
    }
    return;
}
