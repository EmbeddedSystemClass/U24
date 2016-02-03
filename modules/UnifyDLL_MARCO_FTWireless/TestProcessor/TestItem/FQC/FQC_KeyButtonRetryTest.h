/*********************************************************************************
 * COPYRIGHT Qisda WR24 2014
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 ********************************************************************************/


#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "FQC_KeyButtonRetryTestWindow.h"
class CFQC_KeyButtonRetryTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CFQC_KeyButtonRetryTest)

// Data members
public:

protected:
private:
	std::string m_str_TestItem;
	std::string m_str_XMLStation;

	std::string str_msg;

	int m_i_OnSleepTime;
	int m_i_OffSleepTime;
	int m_i_retryTime;
	int m_i_retriedTime;
	int m_i_MexRetryTime;

	bool b_res_msg_try, b_res_msg_retry;


// Member functions
public:
	CFQC_KeyButtonRetryTest() {}
	~CFQC_KeyButtonRetryTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:

	bool runLoop_HeadsetKeyTest();
	bool runLoop_SideKeyTest();
	bool getButtonCount();
	bool getSideKey();

	bool showMsg(char* szMsg);

	CFQC_KeyButtonRetryTestWindow *pButtonWindow; 


	CWinThread *KeyTestThreadHandle;
	static UINT AfxBounceWindowThread(LPVOID pParam);

	int TimeOut;
	bool b_StopkKeyTest;
	bool InitialBounceWindow(int nStyle = 0);/* 0=headset key, 1=iF side key */
	int  getButtonCountProcess();
	bool getSideKeyProcess();
	bool EndButtonTestDlg();

};

