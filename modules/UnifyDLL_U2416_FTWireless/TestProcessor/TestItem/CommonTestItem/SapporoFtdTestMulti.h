/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CSapporoFtdTestMulti object.
 * FileName   : CSapporoFtdTestMulti.h
 * Version    : $Revision:   1.0  $
 * Author     : Neil.Du
 * Time       : 2013.10.01
 ********************************************************************************/

#ifndef _C_SAPPORO_FTD_TEST_MULTI_H_
#define _C_SAPPORO_FTD_TEST_MULTI_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

typedef enum e_qisda_process
{
	USB_DETECT_START,
	USB_DETECT_END,

} QISDA_PROCESS;

class CSapporoFtdTestMulti : public ITestItem
{
RF_DECLARE_DYNCREATE(CSapporoFtdTestMulti)


// Data members
public:

typedef struct tagQPhoneThreadParam
{
	CSapporoFtdTestMulti* pThisObject;
	int nQPhoneNumber;
	int nQisdaProcess;

} THREAD_PARAM, *LPTHREAD_PARAM;

protected:
private:
	std::string std_FtdFunction;
	
	HANDLE m_hQPhoneProcess[5];
	HANDLE g_MutexUsbDetectReport;
	HANDLE g_MutexUsbDetectFtd;

// Member functions
public:
	CSapporoFtdTestMulti() {}
	~CSapporoFtdTestMulti() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MultiThreadExecution(int nQisdaProcess);
	static UINT AfxQPhoneThread(LPVOID lpParam);
	void runUsbDetect(int nQPhoneNumber, int nQisdaProcess);
	bool runUsbDetectStart(int nQPhoneNumber);
	bool runUsbDetectEnd(int nQPhoneNumber);

	void Report(bool bRes, string strMessage, int nQPhoneNumber, string strMeasured = _T(""), string strErrorCode = _T(""));
};


#endif // End of #ifndef _C_SAPPORO_FTD_TEST_MULTI_H_
