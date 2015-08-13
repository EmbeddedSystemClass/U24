#pragma once


// CInputDlg dialog

class CInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(const CString& strPrompt,const CString& strDefaultPath, CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputDlg();
	virtual BOOL OnInitDialog();

	// Dialog Data
	enum { IDD = IDD_INPUT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_szInput;
	CString m_strPrompt;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonBrown();
};
