#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MsgDlg.h"
#include "DB_Operator.h"
#include "MyListCtrl.h"


// OutPut_Manage dialog

class OutPut_Manage : public CDialog
{
	DECLARE_DYNAMIC(OutPut_Manage)

public:
	OutPut_Manage(CWnd* pParent = NULL);   // standard constructor
	virtual ~OutPut_Manage();

// Dialog Data
	enum { IDD = warehouse_Out };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CMyListCtrl m_listctrl_OutPut;
	CEdit m_Edit_SO_OutPut;
	MsgDlg msgDlg;
	DB_operator DB_Output;
	bool m_b_loadDB_Res;
	bool m_check_CTNID;
	bool m_b_lock;
	CStringArray m_szIMEIArray;
	int m_n_Fail;
	int m_n_Pass;
	int nIMEInum;
	CStatic m_Fail_show;
	CStatic m_Pass_show;
	CStatic m_Total_show;
	CBrush m_hwndBrush;

	int m_nIDLength;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	bool FcheckIMEI(CString str_IMEI, int nIMEI_current_number);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CEdit m_Edit_ReworkReason;
	CEdit m_Edit_DOA_RMA_NO;
	CButton m_Button_Lock;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	CButton m_Button_DOA_RMA_Option;
	afx_msg void OnEnChangeEdit4();
};
