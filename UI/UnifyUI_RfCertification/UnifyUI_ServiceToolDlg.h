// UnifyUI_ServiceToolDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include <vector>

#include "ServiceModule.h"
#include "Utility.h"
#include "NotifyBase.h"
#include "DLInterface.h"

// CUnifyUI_ServiceToolDlg dialog
class CUnifyUI_ServiceToolDlg : public CDialog
{
// Construction
public:
	CUnifyUI_ServiceToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SERVICE_MAINUI };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	HANDLE m_mutexOneInstance;
	CString m_pthToolDir;
	CString m_strTitle;
protected:
	std::vector<toolproj_t> m_projects;
	void UITabSetup();
	size_t GetToolInfo(CString scan_path, std::vector<toolproj_t>& projects);
	size_t GetTabs(CString scan_path, std::vector<tooltablist_t>& tabs);
	size_t GetDlls(CString& path, std::vector<dlltoolinfo_t>& dlls, std::vector<toolparameter_t>& parameter);
	void UIModuleSetup();
	void UITitleSetup();
	bool CheckQisdaDll(CString& path_dll, dlltoolinfo_t& data, toolparameter_t& param);
public:
	int m_nProjectSelected;
	std::vector<CServiceModule*> m_uiTabs;
	CTabCtrl m_uiTabModule;
	int m_nTabSelected;
	afx_msg void OnTcnSelchangeTabModule(NMHDR *pNMHDR, LRESULT *pResult);

	IDLInterface* m_iDL;
	afx_msg void OnCbnSelchangeComboModule();
	toolparameter_t parameter[8];
	afx_msg void OnClose();
};
