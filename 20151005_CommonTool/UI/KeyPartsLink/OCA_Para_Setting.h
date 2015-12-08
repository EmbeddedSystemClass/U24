#pragma once


// COCA_Para_Setting dialog

class COCA_Para_Setting : public CDialog
{
	DECLARE_DYNAMIC(COCA_Para_Setting)

public:
	COCA_Para_Setting(CWnd* pParent = NULL);   // standard constructor
	virtual ~COCA_Para_Setting();

	CString m_strRollerPressure;
	CString m_strZ_AxisHeight;
	CString m_strMoundLayerHeight;

// Dialog Data
	enum { IDD = IDD_OCA_PARA_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
