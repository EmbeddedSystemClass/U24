#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MsgDlg.h"
#include "DB_Operator.h"
#include "MyListCtrl.h"



// OutPut_CSD dialog

class OutPut_CSD : public CDialog
{
	DECLARE_DYNAMIC(OutPut_CSD)

public:
	OutPut_CSD(CWnd* pParent = NULL);   // standard constructor
	virtual ~OutPut_CSD();

// Dialog Data
	enum { IDD = warehouse_Csd };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CBrush m_hwndBrush;
	CEdit m_Edit_SO;
	CMyListCtrl m_listctrl_OutCSD;
	CStringArray m_szIMEIArray;
	CStringArray m_szPicassoArray;
	CString m_str_so;
	CString m_str_SO_Qty;
	int PicassoNum;
	int n_fail;
	int n_pass;
	int n_total;
	bool m_b_lock;
	DB_operator DB_OutCSD;
	MsgDlg msgDlg;
	CButton m_Edit_CHECK;

	int m_nIDLength;

public:
	afx_msg void OnEnKillfocusEdit1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	bool FcheckIMEI(CString str_IMEI, int nIMEI_current_number);
	bool FcheckPicasso(CString str_Picasso, int nPicasso_current_number);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheck3();
};
