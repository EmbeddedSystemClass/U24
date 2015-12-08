/******************************************************************************
* Copyright (C) QCS/WGC18                                      
* Module name	: Entrance Tool   
* File name     : MsgDlg.h            
* Summary       : Display message information                                                              
* Version      Author            Date                 Abstract                      
* 1.0          Tom Li         2011/06/28           First version                        
*****************************************************************************/

#pragma once

#include "afxwin.h"
#include "Label.h"


// MsgDlg dialog

class MsgDlg : public CDialog
{
	DECLARE_DYNAMIC(MsgDlg)

public:
	MsgDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MsgDlg();

	enum { IDD = IDD_MSG };




private:
	CString m_cstrMsg;
	CString m_cstrMsg_code;
	COLORREF m_TextColor;
	int m_iTextSize;
	CLabel m_errmsg;
	CLabel m_errcode;
	CBrush m_brush;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()






public:
	void SetTextColor(COLORREF color);
	void SetTextSize(int iSize);
	void SetError(CString& cstrMsg);
	void SetErrorCode(CString& cstrMsg);
	HBRUSH CreateHBRUSH();
	HBRUSH DeleteHBRUSH();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
};
