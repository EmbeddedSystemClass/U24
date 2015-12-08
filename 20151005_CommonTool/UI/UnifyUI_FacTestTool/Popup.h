#pragma once

#include "Label.h"

// CPopup dialog

class CPopup : public CDialog
{
	DECLARE_DYNAMIC(CPopup)

public:
	CPopup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPopup();

	void SetText(CString str_text);

// Dialog Data
	enum { IDD = IDD_POPUP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
private:
	CLabel      m_label_message;

	CString     m_str_msg;

	DECLARE_MESSAGE_MAP()
};

