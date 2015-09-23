
#pragma once

#include "afxwin.h"
#include "label.h"
#include "CommDef.h"

#define UI_PARAM_MODEL			_T("ModelName")
#define UI_PARAM_PARTNO			_T("PartNO")
#define UI_PARAM_LOADVERSION    _T("LoadVersion")
#define UI_PARAM_STATION        _T("StationName")

class CInitDlg : public CDialog
{
	DECLARE_DYNAMIC(CInitDlg)

public:
	CInitDlg(CWnd* pParent = NULL);
	virtual ~CInitDlg();

	bool GetInitData(st_UIIDparameter& st_dataCopy);

// Dialog Data
	enum { IDD = IDD_DIALOG_INIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	/* Initialization functions */
	bool InitialProcedure();
	bool InitialWidgetProperty();
	bool InitialStationComBoBox(CString str_modelName);

	afx_msg void OnCbnSelchangeInitComboStation();
	afx_msg void OnBnClickedInitButtonOk();
	afx_msg void OnBnClickedInitButtonCancel();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetRegStringValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue);
	CString GetRegStringValue(LPCTSTR lpszKeyName, LPCTSTR lpszDefRegValue);
	void RegistryManager(void);
private:
	CLabel m_static_model;
	CLabel m_static_partNumber;
	CLabel m_static_loadVer;
	CLabel m_static_station;
	CLabel m_statci_employeeid;
	CEdit m_edit_model;
	CEdit m_edit_partNumber;
	CEdit m_edit_loadVer;
	CEdit m_edit_employeeid;
	CComboBox m_combobox_station;
	CButton m_botton_ok;
	CButton m_botton_cancel;
	CString m_str_station;

	st_UIIDparameter m_st_initData;
	HKEY m_hRegKey;
	
public:
	afx_msg void OnEnKillfocusInitEditModel();
};
