#pragma once

#include "Label.h"
#include "afxwin.h"
#include "atlimage.h"


// CPopUpImg dialog


class CPopUpImg : public CDialog
{
	DECLARE_DYNAMIC(CPopUpImg)

public:
	CPopUpImg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPopUpImg();

	void SetImgPath(CString strImgPath, CString strImgText) ;
	void SetButtonName(CString strOK, CString strCANCEL) ;

// Dialog Data
	enum { IDD = IDD_POPUP_IMG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

private:
	CImage		m_image ;
	CLabel      m_label_message;
	CString     m_strImgPath ;
	CString     m_strImgText ;
	CString     m_strOKButton ;
	CString     m_strCancelButton ;
	CButton		m_buttonIDOK ;
	CButton		m_buttonIDCANCEL ;
	BOOL GetFullPath (CString& strPath, BOOL bIsCheckExist) ;
	DECLARE_MESSAGE_MAP()
};

