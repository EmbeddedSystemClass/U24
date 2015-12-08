#pragma once
#include "..\..\modules\DBAccess\DBAccess.h"


// CIMEICartion dialog

class CIMEICartion : public CDialog
{
	DECLARE_DYNAMIC(CIMEICartion)

public:
	CIMEICartion(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIMEICartion();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditImei();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusEditImei();
	afx_msg void OnEnUpdateEditImei();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);


	bool bEraseCarton; 
	void Check(void);
};
