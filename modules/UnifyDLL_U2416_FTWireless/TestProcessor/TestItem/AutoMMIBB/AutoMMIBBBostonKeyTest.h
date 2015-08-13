/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CAutoMMIBBBostonKeyTest object.
 * FileName   : BostonKeyTest.h
 * Version    : $Revision:   1.0  $
 * Author     : LION.WU
 * Time       : 2013.02.26
 ********************************************************************************/

#ifndef _C_KEY_TEST_H_
#define _C_KEY_TEST_H_

#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "AutoMMIBBCommunicate.h"


class CAutoMMIBBBostonKeyTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CAutoMMIBBBostonKeyTest)
// Data members
private:
	int TimeOut;
	int nRetry;
	bool b_StopkKeyTest;

// Member functions
public:
	CAutoMMIBBBostonKeyTest();
	~CAutoMMIBBBostonKeyTest();

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();
	bool RunAnalysisThread();
	bool StartKeyTest();
	CWinThread *thKeyTest_Thread;
	CWinThread *thJIG_Thread;

private:
	bool KeyTest();
	bool RunKeyTestProcess(int nXMLButtonID);

	bool AutoMMIBBSetComport();
	bool JigGO(char* sz_number);
	bool JigBACK(char* sz_number);
	static UINT KeyTest_Thread(LPVOID pParam);
	static UINT JIG_Thread(LPVOID pParam);

	CCommunicate   m_obj_Communicate;
	std::string m_str_comport;
	std::string m_str_command;
	std::string m_str_buttonID;
	std::string m_str_SleepJIG;

	int         m_i_sleep_for_Jig;
	int         m_i_sleep_for_Data;
	DWORD			d_readBufferSize;
	DWORD			d_writeBufferSize;
};


#endif // End of #ifndef _C_KEY_TEST_H_