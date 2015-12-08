
// warehouseManageDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "atlimage.h"
#include "Input_Manage.h"
#include "OutPut_Manage.h"
#include "Label.h"
#include "MsgDlg.h"
#include "afxwin.h"
#include "EnTabCtrl.h"
#include "ClrButton.h"
#include "OutPut_CSD.h"

// CwarehouseManageDlg dialog
class CwarehouseManageDlg : public CDialog
{
// Construction
public:
	CwarehouseManageDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WAREHOUSEMANAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAbout();
	DECLARE_MESSAGE_MAP()
private:
	CEnTabCtrl m_tab_warehouse;
	CImage m_image;
	CBrush m_brush;
	CMenu m_Menu;              // Help menu
	CString m_str_titlePicturePath;
	int m_CurSelTab;
	int m_n_test;
	

//public:
//	bool LoadDB();
//	bool GetFunadress();
//	bool FconnectDB();
//	bool FcloseDB();


private:
	Input_Manage m_InputDlg;
	OutPut_Manage m_OutputDlg;
	OutPut_CSD m_OutCsdDlg;
	MsgDlg msgDlg;


public:
	afx_msg void OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedConvirm();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	bool InitialUITitle();
	//CLabel m_Static_Title;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CClrButton m_btn_cancel;
	CClrButton m_btn_Convirm;
	CClrButton m_btn_ok;
};
