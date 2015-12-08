#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "stdafx.h"
#include "MsgDlg.h"
#include "DB_Operator.h"
#include "MyListCtrl.h"


// Input_Manage dialog

class Input_Manage : public CDialog
{
	DECLARE_DYNAMIC(Input_Manage)

public:
	Input_Manage(CWnd* pParent = NULL);   // standard constructor
	virtual ~Input_Manage();

// Dialog Data
	enum { IDD = warehouse_Input };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_listctrl_Input;
	CEdit m_EditSO;
	CEdit m_EditID;

	CStringArray m_sz_IMEIArray;
	MsgDlg msgDlg;
	DB_operator DB_Input;
	bool m_b_loadDB_Res;
	bool m_b_lock;
	bool m_check_CTNID;
	int n_pass;
	int n_fail;
	int n_total;
	int nIMEInum;

	int m_nIDLength;
	CStatic m_Fail_show;
	CStatic m_Pass_show;
	CStatic m_Total_show;
	CStatic m_Qty_show;
	CStatic m_PN_show;
	CBrush m_hwndBrush;
	CStatic m_SW_show;
	//CheckBox m_checked;


public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnKillfocusEdit1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCartonid();
	afx_msg void OnBnClickedImei();
	bool FcheckIMEI(CString str_IMEI, int nIMEI_current_number);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnNMThemeChangedCheck1(NMHDR *pNMHDR, LRESULT *pResult);
	
	CButton m_Lock_check;
};
