#pragma once


// CDebugDlg dialog

class CDebugDlg : public CDialog
{
	DECLARE_DYNAMIC(CDebugDlg)

public:
	CDebugDlg(const CString& strPrompt, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDebugDlg();
	virtual BOOL OnInitDialog();

	CString m_szInput;
	CString m_strPrompt;

// Dialog Data
	enum { IDD = IDD_DEBUG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedStep();
	afx_msg void OnBnClickedBreak();
	afx_msg void OnBnClickedStepinto();
};
