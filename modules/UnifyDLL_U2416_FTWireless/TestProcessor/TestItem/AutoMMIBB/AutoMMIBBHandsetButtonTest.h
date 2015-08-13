/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CAutoMMIBBHandsetButtonTest object.
 * FileName   : BostonKeyTest.h
 * Version    : $Revision:   1.0  $
 * Author     : LION.WU
 * Time       : 2013.02.26
 ********************************************************************************/

#ifndef _C_KEY_TEST_H_
#define _C_KEY_TEST_H_

#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "AutoMMIBBCommunicate.h"


class CAutoMMIBBHandsetButtonTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CAutoMMIBBHandsetButtonTest)

private:
	int TimeOut;
	int nRetry;
	bool b_StopkKeyTest;

// Member functions
public:
	CAutoMMIBBHandsetButtonTest();
	~CAutoMMIBBHandsetButtonTest();

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool HandsetKeyTest();


	bool AutoMMIBBSetComport();
	bool JigGO(char* sz_number);
	bool JigBACK(char* sz_number);

	CCommunicate   m_obj_Communicate;
	std::string m_str_comport;
	std::string m_str_command;
	std::string m_str_buttonID;
	std::string m_str_SleepJIG;
	std::string m_str_Check;

	int         m_i_sleep_for_Jig;
	int         m_i_sleep_for_Data;
	int         m_i_sleep_for_Check;
	DWORD			d_readBufferSize;
	DWORD			d_writeBufferSize;
};


#endif