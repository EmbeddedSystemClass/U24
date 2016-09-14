#pragma once
#include "afxwin.h"
#include "Utility.h"
#include "DllCtrl.h"
#include <setupapi.h>
#pragma comment(lib, "setupapi")

#define TAG_LENGTH		7

// CCSDREAD dialog

class CCSDREAD : public CDialog
{
	DECLARE_DYNAMIC(CCSDREAD)

public:
	CCSDREAD(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCSDREAD();

// Dialog Data
	enum { IDD = IDD_SERVICE_SUBUI_CSDWRITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

	DECLARE_MESSAGE_MAP()

	dlltoolinfo_t m_data;
	CString m_pthToolDir;
	CDllCtrl m_dllCtrl;

public:
	void SetInit(dlltoolinfo_t& data);
	static int MsgCall(const char* szMsg, const char* szTag);
	void PrintMsg(CString message, CString tag, bool isLog = true);

	static int ResultCall(const int nIndex, const char* szUnit, const char* szLower, const char* szUpper, const char* szMeasured,
						  const char* szResult, const char* szErrCode, const char* szMsg);
	void SetResult(int nIndex, char* szUnit, char* szLower, char* szUpper, char* szMeasured, char* szResult, char* szErrCode, char* szMsg);

private:
	CComboBox m_combWlanMode;
	CComboBox m_combComport;
	CComboBox m_combChain;


	CString csTag;

	void UIInit();
	//void UIControl(bool isEnable);
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
	void UIControl(bool isEnable);
	int WriteResultCall(const char* szMeasured);
	void SetWriteResult(char* szMeasured);

	struct ComInfo {
		CString port;
		CString desc;
	};
	struct ComInfo m_com;


public:
	afx_msg void OnBnClickedButtonRun();

protected:
	CString m_sTag;


public:
	afx_msg void OnBnClickedButtonStop();

};
