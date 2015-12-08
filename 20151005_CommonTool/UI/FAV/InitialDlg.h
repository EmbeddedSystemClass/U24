/**********************************************************
*   Copyright (C) 2011 Qisda Corporation
*   All Rights Reserved.
*   Class name : CPdaRec                                                     
*   Summary    : Record management for FA station and QA team
*   Version     Author            Date                Abstract  
*   1.0         Nancy.Yang        2011/05/12          First version
*   1.1         Ken.Deng
*   1.2         Sunlight          2012/02/13          UI change
************************************************************
*/
#pragma once

// CInitialDlg dialog

class CInitialDlg : public CDialog
{
	DECLARE_DYNAMIC(CInitialDlg)

public:
	CInitialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInitialDlg();

// Dialog Data
	enum { IDD = IDD_INITIAL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	virtual BOOL OnInitDialog();
	
	CString m_strLotNumber;
	CString m_strLotCount;

};
