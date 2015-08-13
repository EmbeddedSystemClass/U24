#pragma once
#include "Utility.h"
#include "DllCtrl.h"
#include "GridCtrl/GridCtrl.h"


// CFuncInfoWriter dialog

class CFuncInfoWriter : public CDialog
{
	DECLARE_DYNAMIC(CFuncInfoWriter)

public:
	CFuncInfoWriter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFuncInfoWriter();

// Dialog Data
	enum { IDD = IDD_SERVICE_SUBUI_INFOWRITER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRun();

protected:
	CGridCtrl m_uiInfoEdit;
	dlltoolinfo_t m_data;
	CString m_pthToolDir;
	CString m_csFsn;
	CString m_csNetworkCode;

public:
	void SetInit(dlltoolinfo_t& data);
	static int MsgCall(const char* szMsg, const char* szTag);
	void PrintMsg(CString message, CString tag, bool isLog = true);
	static int UpdatePortCall(const char* szPort);
	void SetPortUpdate(CString port);
protected:
	CDllCtrl m_dllCtrl;

private:
	void UIInit();
	void UIWONLYInit();
	void UIRWInit();
	void UIRW_FILEInit();
	void UIHide(const int& item);
	void UIWarning(CString message);
	void UIControl(bool isEnable, bool isPass);
	void UITotalTime(double secTime);

private:
	// timer
	HANDLE m_eventTimer;
	CWinThread* m_thrdTimer;
	void Timer(bool isEnable);
	static UINT WorkerThreadTimerProc(LPVOID param);
	void WorkerThreadTimerRun();

	// runner
	CWinThread* m_thrdMainProgress;
	static UINT WorkerThreadFuncProc(LPVOID param);
	void WorkerThreadFuncRun();

	// read button
	CWinThread* m_thrdReadProgress;
	static UINT ReadThreadFuncProc(LPVOID param);
	void ReadThreadFuncRun();
	void ReadList();

	// flag
	bool m_isBrowseFile;
	bool m_isBrowseFileRsa;
	bool m_isBrowseFileSimlock;
	bool m_isRead;
public:
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonFolder();
	afx_msg void OnBnClickedButtonFolderRsa();
	afx_msg void OnBnClickedButtonFolderSimlock();
};
