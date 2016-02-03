/*********************************************************************************
 * COPYRIGHT Qisda WR24 2014
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 ********************************************************************************/


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CMMIBB_RGBVibratorRetryTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CMMIBB_RGBVibratorRetryTest)

// Data members
public:

protected:
private:
	std::string m_str_TestItem;
	std::string m_str_CMD;
	std::string m_str_OffCMD;
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
	CMMIBB_RGBVibratorRetryTest() {}
	~CMMIBB_RGBVibratorRetryTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool runLoop_CameraLed();
	bool runLoop_RGBLED_Test(int n_Case_RGB);
	bool runLoop_RGBPattern_Test(int n_Case_RGB);
	bool runLoop_BackLight_Test();
	bool runLoop_Vibrator_Test();

	bool showMsg(const char* szMsg);
};
