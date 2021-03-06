// Popup.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_FacTestTool.h"
#include "PopUpImg.h"

// CPopUpImg dialog

IMPLEMENT_DYNAMIC(CPopUpImg, CDialog)

CPopUpImg::CPopUpImg(CWnd* pParent /*=NULL*/)
	: CDialog(CPopUpImg::IDD, pParent)
{
	m_strOKButton = "YES" ;
	m_strCancelButton = "NO" ;
}

CPopUpImg::~CPopUpImg()
{
}

void CPopUpImg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TEXT,	m_label_message);
	DDX_Control(pDX, IDOK,				m_buttonIDOK);
	DDX_Control(pDX, IDCANCEL,			m_buttonIDCANCEL);
}


BEGIN_MESSAGE_MAP(CPopUpImg, CDialog)
END_MESSAGE_MAP()

void CPopUpImg::SetImgPath(CString strImgPath, CString strImgText)
{
	m_strImgPath = strImgPath ;
	m_strImgText = strImgText ;
}

void CPopUpImg::SetButtonName(CString strOK, CString strCANCEL)
{
	m_strOKButton = strOK ;
	m_strCancelButton = strCANCEL ;
}

BOOL CPopUpImg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Check Image
	if ( GetFullPath(m_strImgPath, TRUE) )
	{
		CImage imgObj ;
		imgObj.Load(m_strImgPath) ;
		((CStatic*)GetDlgItem(IDC_STATIC_SHOW_IMG))->SetBitmap( imgObj.Detach() );
	}

    // Set Text TRANSPARENT
    m_label_message.SetTextColor( RGB(255,0,0) ) ;
	m_label_message.SetFontBold(TRUE);
    m_label_message.SetFontSize(16);
    m_label_message.SetWindowText(m_strImgText) ;

    // Set Button Text
    m_buttonIDOK.SetWindowText(m_strOKButton) ;
	m_buttonIDCANCEL.SetWindowText(m_strCancelButton) ;

	return TRUE;
}


BOOL CPopUpImg::GetFullPath (CString& strPath, BOOL bIsCheckExist)
{
	if ( ::PathIsRelative(strPath) ) {
		TCHAR szCurrentPath[MAX_PATH] ;
		::ZeroMemory(szCurrentPath, MAX_PATH) ;
		::GetModuleFileName (NULL, szCurrentPath, MAX_PATH) ;
		::PathRemoveFileSpec(szCurrentPath) ;
		::PathAppend(szCurrentPath, (LPCTSTR)strPath) ;
		strPath = szCurrentPath ;
	}

	// Check Exist
	if (bIsCheckExist) {
		return ::PathFileExists(strPath) ;
	}

	return TRUE ;
}

// CPopUpImg message handlers
