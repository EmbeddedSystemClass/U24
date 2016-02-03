/*********************************************************************************
 * COPYRIGHT Qisda WR24 2014
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 ********************************************************************************/
#ifndef _C_PLAY_WAV_TEST_H_
#define _C_PLAY_WAV_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CMMIBB_AudioTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CMMIBB_AudioTest)

// Data members
public:

protected:
private:
	std::string m_str_AudtioTestItem;
	std::string m_str_AudtioCMD;
	std::string m_str_Path;
	std::string str_msg;
	int m_i_SleepTime;

// Member functions
public:
	CMMIBB_AudioTest() {}
	~CMMIBB_AudioTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool PlayWavTest();
	bool runPushAudioFile();
	bool runFmTest();
	bool runOffFmTest();
	bool runSpeakerOnTest();
	bool runHeadsetOnTest();
	bool runReceiverOnTest();
	bool runAudioOffTest(char* szMsg);
	bool showMsg(char* szMsg);

};


#endif // End of #ifndef _C_PLAY_WAV_TEST_H_
