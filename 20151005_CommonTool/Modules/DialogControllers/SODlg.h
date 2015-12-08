#ifndef __SO_DLG__H__
#define __SO_DLG__H__

#pragma once

#include <string>
#include "afxwin.h"
#include "..\DialogResources\resource.h"
// CSODlg dialog
/*
#define IDD_SO_DIALOG_DIALOGS_DLL       101
#define IDD_LARGE_MSG_DIALOG_DIALOGS_DLL 102
#define IDS_APP_TITLE                   103
#define IDC_SO_STATIC_DIALOGS_DLL       1000
#define IDC_FACTORYINFO_STATIC_DIALOGS_DLL 1001
#define IDC_EMPLOYEE_STATIC_DIALOGS_DLL 1002
#define IDC_SO_EDIT_DIALOGS_DLL         1003
#define IDC_EMPLOYEE_EDIT_DIALOGS_DLL   1004
#define IDOK_DIALOGS_DLL                1005
#define IDCANCEL_DIALOGS_DLL            1006
#define IDC_CAPTION_STATIC_DIALOGS_DLL  1007
#define IDC_MSG_STATIC_DIALOGS_DLL		1008
*/
class CSODlg : public CDialog
{
	DECLARE_DYNAMIC(CSODlg)

public:
	CSODlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSODlg();

// Dialog Data
	enum { IDD = IDD_SO_DIALOG_DIALOGS_DLL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	const CString GetEmployeeID() const;
	const CString GetSONumber() const;
private:
	CString EmployeeID;
	CString SONumber;
};

#endif //__SO_DLG__H__