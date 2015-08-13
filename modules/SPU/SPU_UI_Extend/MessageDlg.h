#if !defined(AFX_MESSAGE_H__AC5068A9_1F8E_451D_AFB1_FFDEA02CF34F__INCLUDED_)
#define AFX_MESSAGE_H__AC5068A9_1F8E_451D_AFB1_FFDEA02CF34F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Message.h : header file
//
#include "Label.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CMessage dialog

class CMessageDlg : public CDialog
{
// Construction
public:
	CMessageDlg(const CString& m_strErrorCode,const CString& strMessage,CWnd* pParent = NULL);   // standard constructor
	CBrush m_brush;

// Dialog Data
	//{{AFX_DATA(CMessage)
	enum { IDD = IDD_DIALOG_MESSAGE };
	CLabel	m_ERROR;
	CButton	m_OK;
	CLabel	m_MESSAGE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strErrorCode;
	CString m_strMessage;

	// Generated message map functions
	//{{AFX_MSG(CMessage)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGE_H__AC5068A9_1F8E_451D_AFB1_FFDEA02CF34F__INCLUDED_)
