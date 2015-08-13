/*********************************************************************************
 * COPYRIGHT Qisda WR24 2014
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 ********************************************************************************/

#ifndef _C_PLAY_WAV_TEST_H_
#define _C_PLAY_WAV_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CMMIBB_AudioLoopRetryTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CMMIBB_AudioLoopRetryTest)

// Data members
public:

protected:
private:
	std::string m_str_AudtioTestItem;
	std::string m_str_AudtioCMD;
	std::string m_str_AudtioOffCMD;
	std::string m_str_Path;
	std::string str_msg;
	int m_i_OnSleepTime;
	int m_i_OffSleepTime;
	int m_i_retryTime;
	int m_i_retriedTime;
	int m_i_MexRetryTime;

	bool b_res_on, b_res_off ;
	bool b_res_msg_try, b_res_msg_retry;



// Member functions
public:
	CMMIBB_AudioLoopRetryTest() {}
	~CMMIBB_AudioLoopRetryTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:

	bool runLoop_FrontMicToSpeakOn();
	bool runLoop_BackMicToReceiverOn();
	bool runLoop_HeadmicToHeadsetOn();
	bool runLoop_FrontMicToHeadsetOn();
	bool runLoop_BackMicToHeadsetOn();
	bool runLoop_MicToSpeak();

	bool showMsg(char* szMsg);

};


#endif 
