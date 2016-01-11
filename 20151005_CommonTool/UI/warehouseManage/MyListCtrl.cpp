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
    switch(lplvdr->nmcd.dwDrawStage)//�ж�״̬
        {
            case CDDS_PREPAINT:
           {
               *pResult = CDRF_NOTIFYITEMDRAW;
               break;
           }
               case CDDS_ITEMPREPAINT://���Ϊ��ITEM֮ǰ��Ҫ������ɫ�ĸı�
           {
               COLORREF ItemColor;
               if(MapItemColor.Lookup(nmcd.dwItemSpec, ItemColor))
		//������SetItemColor(DWORD iItem, COLORREF color) ���õ�//ITEM�ź�COLORREF �������в��ң�Ȼ�������ɫ��ֵ��
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
    MapItemColor.SetAt(iItem, color);//����ĳ�е���ɫ��
    this->RedrawItems(iItem, iItem);//����Ⱦɫ
    //this->SetCheck(iItem,1);
    //this->SetFocus();//���ý���
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
