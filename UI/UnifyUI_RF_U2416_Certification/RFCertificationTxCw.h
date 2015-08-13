#pragma once
#include "afxwin.h"
#include "Utility.h"
#include "DllCtrl.h"
#include <setupapi.h>
#pragma comment(lib, "setupapi")

// RFCertificationTxCw dialog

class CRFCertificationTxCw : public CDialog
{
	DECLARE_DYNAMIC(CRFCertificationTxCw)

public:
	CRFCertificationTxCw(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRFCertificationTxCw();

// Dialog Data
	enum { IDD = IDD_SERVICE_SUBUI_TX_CW };

	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnCbnDropdownComboPort();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

	CComboBox m_combChannel;
	CComboBox m_combChain;

	DECLARE_MESSAGE_MAP()

	dlltoolinfo_t m_data;
	CString m_pthToolDir;
	CDllCtrl m_dllCtrl;
public:
	void SetInit(dlltoolinfo_t& data);
	static int MsgCall(const char* szMsg, const char* szTag);
	void PrintMsg(CString message, CString tag, bool isLog = true);
	static int UpdatePortCall(const char* szPort);
	void SetPortUpdate(CString port);
	static int ResultCall(const int nIndex, const char* szUnit, const char* szLower, const char* szUpper, const char* szMeasured,
						  const char* szResult, const char* szErrCode, const char* szMsg);
	void SetResult(int nIndex, char* szUnit, char* szLower, char* szUpper, char* szMeasured, char* szResult, char* szErrCode, char* szMsg);
	static int RxResultCall(const char* szMeasured);
	void SetRxResult(char* szMeasured);

private:
	CComboBox m_combComport;
	CString m_sRfGain;

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

	struct ComInfo {
		CString port;
		CString desc;
	};
	struct ComInfo m_com;

	bool SearchPortUsbAdb(const UINT secTimeout);
	bool SearchPortUsb(const UINT secTimeout);
	bool GetAdbDevice();
	bool FindUsbDevice();
	bool FindDeviceId(CString DeviceId);
	CString GetDeviceName(HDEVINFO& hDevInfo, SP_DEVINFO_DATA& spDevInfoData);
	bool GetComPort();
public:
	afx_msg void OnBnClickedButtonStop();
};

