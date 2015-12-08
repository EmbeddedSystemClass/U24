// MSG.cpp : implementation file
//

#include "stdafx.h"
#include "Entrance Tool.h"
#include "MSG.h"


// MSG dialog

IMPLEMENT_DYNAMIC(MSG, CDialog)

MSG::MSG(CWnd* pParent /*=NULL*/)
	: CDialog(MSG::IDD, pParent)
{

}

MSG::~MSG()
{
}

void MSG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MSG, CDialog)
END_MESSAGE_MAP()


// MSG message handlers
