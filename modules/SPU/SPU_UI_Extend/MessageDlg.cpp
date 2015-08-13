// Message.cpp : implementation file
//

#include "stdafx.h"
#include "MessageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessage dialog


CMessageDlg::CMessageDlg(const CString& strErrorCode,const CString& strMessage,CWnd* pParent /*=NULL*/)
	: CDialog(CMessageDlg::IDD, pParent)
	,m_strErrorCode(strErrorCode)
	,m_strMessage(strMessage)
{
	//{{AFX_DATA_INIT(CMessage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessage)
	DDX_Control(pDX, IDC_STATIC_ERROR, m_ERROR);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_MESSAGE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CMessage)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessage message handlers

HBRUSH CMessageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor ==CTLCOLOR_DLG)
         return m_brush;
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CMessageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_brush.CreateSolidBrush(RGB(200,200,100));
	m_OK.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_OK)));
	//*******************************
	if(m_strErrorCode=="PASS")
	{
		m_ERROR.SetBkColor(RGB(0, 255, 0));
		m_ERROR.SetTextColor(RGB(0, 0, 255));
		m_ERROR.SetFontSize(50);
		m_ERROR.SetWindowText(m_strErrorCode);
		m_ERROR.SetFontBold(TRUE);

		m_MESSAGE.SetBkColor(RGB(0, 255, 0));
		m_MESSAGE.SetTextColor(RGB(0, 0, 255));
		m_MESSAGE.SetFontSize(25);
		m_MESSAGE.SetWindowText(m_strMessage);
		m_MESSAGE.SetFontBold(TRUE);
	}
	else//error
	{
		m_ERROR.SetBkColor(RGB(255, 0, 0));
		m_ERROR.SetTextColor(RGB(0, 0, 255));
		m_ERROR.SetFontSize(50);
		m_ERROR.SetWindowText(m_strErrorCode);
		m_ERROR.SetFontBold(TRUE);

		m_MESSAGE.SetBkColor(RGB(255, 0, 0));
		m_MESSAGE.SetTextColor(RGB(0, 0, 255));
		m_MESSAGE.SetFontSize(25);
		m_MESSAGE.SetWindowText(m_strMessage);
		m_MESSAGE.SetFontBold(TRUE);
	}
	return true;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


