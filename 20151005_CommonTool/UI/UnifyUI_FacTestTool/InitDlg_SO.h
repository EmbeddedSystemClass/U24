#pragma once

#include "afxwin.h"
#include "label.h"
#include "CommDef.h"

// CInitDlg_SO dialog

class CInitDlg_SO : public CDialog
{
	DECLARE_DYNAMIC(CInitDlg_SO)

public:
	CInitDlg_SO(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInitDlg_SO();
	bool GetInitData(st_UIIDparameter& st_dataCopy);

// Dialog Data
	enum { IDD = IDD_DIALOG_INIT_SO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP();
private:
	/* Initialization functions */
	//bool InitialProcedure();
	//bool InitialWidgetProperty();
//	bool InitialTypeComBoBox();

//	afx_msg void OnCbnSelchangeInitComboType();
	afx_msg void OnBnClickedInitButtonOk();
	afx_msg void OnBnClickedInitButtonCancel();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//void SetRegStringValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue);
	//CString GetRegStringValue(LPCTSTR lpszKeyName, LPCTSTR lpszDefRegValue);
//	void RegistryManager(void);
private:

	//CLabel m_static_type;
//	CLabel m_statci_employeeid;
	//CEdit m_edit_so;

	//CComboBox m_combobox_daynight;
	CButton m_botton_ok;
	CButton m_botton_cancel;
//	CString str_type;

	st_UIIDparameter m_st_initData;
	HKEY m_hRegKey;
	
public:
	afx_msg void OnEnKillfocusInitEditModel();
};
