/******************************************************************************
* Copyright (C) QCS/WGC18                                      
* Module name	: Entrance Tool   
* File name     : Entrance ToolDlg.h            
* Summary       : Check user input information                                                              
* Version      Author            Date                 Abstract                      
* 1.0          Tom Li         2011/07/01           First version                        
*****************************************************************************/

#pragma once
#include "afxwin.h"
#include "Clog.h"
#include "MsgDlg.h"
#include "Funccheck.h"
#include "Filename_resolution.h"
#include "MyListCtrl.h"

#include <windows.h>//get exe version
#include "afxcmn.h"
#pragma comment(lib,"Version.lib")

// CEntranceToolDlg 对话框
class CEntranceToolDlg : public CDialog
{
// 构造
public:
	CEntranceToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ENTRANCETOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	virtual BOOL PreTranslateMessage(MSG* pMsg);


// 实现
protected:
	HICON m_hIcon;
	Clog m_Clog;
	CBrush     m_brush;

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnEnKillfocusEditEmployeeid();
	afx_msg void OnEnKillfocusEditSo();
	afx_msg void OnEnSetfocusEditSo();
	afx_msg void OnEnSetfocusEditEmployeeid();
	afx_msg void OnEnChangeEditEmployeeid();
	afx_msg void OnEnChangeEditSo();
	afx_msg void OnCbnSelchangeComboProject();

	bool Fchecknumber(CString str_number);
	HBRUSH CreateHBRUSH();
	HBRUSH DeleteHBRUSH();

public:
	CString m_str_SONumber; 
	CString m_str_ImployeeID; 
	CString m_str_Model;
	CString m_str_Process;
	CString m_str_AccessoryList;

	CString m_str_PartNumber;

	CString m_str_TSFlag;

	CString m_str_ToolVersion;
	CString m_str_ImageVersion;
	CString m_str_ProjectName;
	CString m_str_EntranceToolVersion;
	CString m_str_state;
	CString m_str_ModelProject;

	

	bool b_check_SO;
	bool b_check_ImployeeID;
	bool b_syc;  //警告框显示同步
	bool m_b_state;//用户选择的状态
public:
	CComboBox m_cb_Project;
	CComboBox m_cb_Process;
	CComboBox m_cb_Accessory;
	CMyListCtrl m_cl_Accessory;
	int		m_nAccessoryCount;
	Funccheck m_Funccheck;
	MsgDlg msgDlg;
	afx_msg void OnCbnSelchangeComboProcess();
	CComboBox m_combox_State;
	afx_msg void OnNMThemeChangedCombo1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCombo1();

	BOOL GetFileVersion(LPCTSTR szFN, LPTSTR szVersion, DWORD nSize);
	afx_msg void OnCbnSelchangeComboAccessory();
};
