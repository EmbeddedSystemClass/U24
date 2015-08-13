/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CAutoMMIBB_Test object.
 * FileName   : VibratorVibrate.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2013.03.13
 ********************************************************************************/

#pragma once

#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "AutoMMIBBCommunicate.h"
#include <cstdlib>

class CAutoMMIBB_Test : public ITestItem
{
RF_DECLARE_DYNCREATE(CAutoMMIBB_Test)

public:
	CAutoMMIBB_Test() {}
	~CAutoMMIBB_Test() {}

public:
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool JigGO(char* sz_number);
	bool JigBACK(char* sz_number);
	bool runAutoMMIBB();

private:
	//bool AutoMMIBBVibrateTest(void);

	CCommunicate   m_obj_Communicate;
	std::string m_str_comport;
	std::string m_str_command;
	std::string m_str_SleepJIG;

	int         m_i_sleep_for_Jig;
	int         m_i_sleep_for_Data;
	DWORD			d_readBufferSize;
	DWORD			d_writeBufferSize;


};
