#pragma once
#include "afxwin.h"
#include "label.h"

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// CPassWord dialog

class CPassWord : public CDialog
{
	DECLARE_DYNCREATE(CPassWord)

public:
	CPassWord(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPassWord();
	enum { IDD = IDD_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()

private:
	CEdit m_edit_passWord;
	CLabel m_lb_error;
};
