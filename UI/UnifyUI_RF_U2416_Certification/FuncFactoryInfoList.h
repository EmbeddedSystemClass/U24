#pragma once
#include "Utility.h"
#include "DllCtrl.h"


// CFuncFactoryInfoList dialog

class CFuncFactoryInfoList : public CDialog
{
	DECLARE_DYNAMIC(CFuncFactoryInfoList)

public:
	CFuncFactoryInfoList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFuncFactoryInfoList();

// Dialog Data
	enum { IDD = IDD_SERVICE_SUBUI_FACTORY_INFOLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRun();

protected:
	dlltoolinfo_t m_data;
	CString m_pthToolDir;
public:
	void SetInit(dlltoolinfo_t& data);
	static int MsgCall(const char* szMsg, const char* szTag);
	void PrintMsg(CString message, CString tag, bool isLog = true);
	static int UpdatePortCall(const char* szPort);
	void SetPortUpdate(CString port);
	static int ResultCall(const int nIndex, const char* szUnit, const char* szLower, const char* szUpper, const char* szMeasured,
						  const char* szResult, const char* szErrCode, const char* szMsg);
	void SetResult(int nIndex, char* szUnit, char* szLower, char* szUpper, char* szMeasured, char* szResult, char* szErrCode, char* szMsg);
protected:
	CDllCtrl m_dllCtrl;

private:
	void UIInit();
	void UIWarning(CString message);
	void UIControl(bool isEnable);
	void UITotalTime(double secTime);

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
};
