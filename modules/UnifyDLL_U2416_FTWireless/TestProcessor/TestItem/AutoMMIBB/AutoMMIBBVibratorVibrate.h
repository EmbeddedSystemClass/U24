/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CAutoMMIBBVibratorVibrate object.
 * FileName   : VibratorVibrate.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2013.03.13
 ********************************************************************************/

#pragma once

#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "AutoMMIBBCommunicate.h"

class CAutoMMIBBVibratorVibrate : public ITestItem
{
RF_DECLARE_DYNCREATE(CAutoMMIBBVibratorVibrate)

typedef struct
{
	int  i_slot;                         // slot: [1, 8]  Default: 1
	char sz_switch[MAX_BUFFER_SIZE];     // Jig Request: ON / OFF   only the two values
}st_JigRequest;

public:
	CAutoMMIBBVibratorVibrate() {}
	~CAutoMMIBBVibratorVibrate() {}

public:
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool JigGO(char* sz_number);
	bool JigBACK(char* sz_number);

private:
	bool AutoMMIBBVibrateTest(void);

	CCommunicate   m_obj_Communicate;
	std::string m_str_comport;
	std::string m_str_command;
	std::string m_str_SleepJIG;
	std::string m_str_SleepDATA;
	std::string m_str_Retry;
	std::string m_str_Check;

	int         m_i_for_Retry;
	int         m_i_sleep_for_Jig;
	int         m_i_sleep_for_Data;
	int         m_i_sleep_for_Check;

	DWORD			d_readBufferSize;
	DWORD			d_writeBufferSize;


};
