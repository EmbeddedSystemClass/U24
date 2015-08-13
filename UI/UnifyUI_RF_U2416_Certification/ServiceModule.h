#pragma once
#include "Utility.h"
#include "ButtonCtrl.h"

// CServiceModule dialog

class CServiceModule : public CDialog
{
	DECLARE_DYNAMIC(CServiceModule)

public:
	CServiceModule(CWnd* pParent = NULL);   // standard constructor
	virtual ~CServiceModule();

// Dialog Data
	enum { IDD = IDD_SERVICE_MAINUI_MODULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	static const int MAX_BUTTON = 8;
	void OnBnClickedButtonTool(int nIndex);
	afx_msg void OnBnClickedModuleButton0();
	afx_msg void OnBnClickedModuleButton1();
	afx_msg void OnBnClickedModuleButton2();
	afx_msg void OnBnClickedModuleButton3();
	afx_msg void OnBnClickedModuleButton4();
	afx_msg void OnBnClickedModuleButton5();
	afx_msg void OnBnClickedModuleButton6();
	afx_msg void OnBnClickedModuleButton7();

	dlltoolinfo_t m_info[MAX_BUTTON];
	toolparameter_t mParam[MAX_BUTTON];
	void SetButtons(std::vector<dlltoolinfo_t>& dlls, std::vector<toolparameter_t>& parameter);
	void SetButtons();
	CButtonCtrl m_btnMODULE_BUTTON0;
	CButtonCtrl m_btnMODULE_BUTTON1;
	CButtonCtrl m_btnMODULE_BUTTON2;
	CButtonCtrl m_btnMODULE_BUTTON3;
	CButtonCtrl m_btnMODULE_BUTTON4;
	CButtonCtrl m_btnMODULE_BUTTON5;
	CButtonCtrl m_btnMODULE_BUTTON6;
	CButtonCtrl m_btnMODULE_BUTTON7;
};
