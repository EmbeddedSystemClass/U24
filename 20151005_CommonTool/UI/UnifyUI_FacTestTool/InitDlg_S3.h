#pragma once

#include "afxwin.h"
#include "label.h"
#include "CommDef.h"

// CInitDlg_S3 dialog

class CInitDlg_S3 : public CDialog
{
	DECLARE_DYNAMIC(CInitDlg_S3)

public:
	CInitDlg_S3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInitDlg_S3();
	bool GetInitData(st_UIIDparameter& st_dataCopy);

// Dialog Data
	enum { IDD = IDD_DIALOG_INIT_S3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP();
private:
	/* Initialization functions */
	//bool InitialProcedure();
	//bool InitialWidgetProperty();
	bool InitialTypeComBoBox();

	afx_msg void OnCbnSelchangeInitComboType();
	afx_msg void OnBnClickedInitButtonOk();
	afx_msg void OnBnClickedInitButtonCancel();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//void SetRegStringValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue);
	//CString GetRegStringValue(LPCTSTR lpszKeyName, LPCTSTR lpszDefRegValue);
//	void RegistryManager(void);
private:

	CLabel m_static_type;
	CLabel m_statci_employeeid;
	CEdit m_edit_line;

	CComboBox m_combobox_daynight;
	CButton m_botton_ok;
	CButton m_botton_cancel;
	CString str_type;

	st_UIIDparameter m_st_initData;
	HKEY m_hRegKey;
	
public:
	afx_msg void OnEnKillfocusInitEditModel();
};
