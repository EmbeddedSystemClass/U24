#ifndef _C_WIRELESS_WIFI_CONTINOUS_RX_TEST_H_
#define _C_WIRELESS_WIFI_CONTINOUS_RX_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_WiFi_ContinuousRxTest : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_WiFi_ContinuousRxTest)

// Data members
public:
protected:
private:
	int m_iCommandDelay;
	//int m_iStartDelay;
	int m_iChannel;
	int m_iChain;
	int m_iPreamble;
//	ifiPowerOnRxGetPacket (int iChannel,int iPreamble, unsigned long* rxFrameCounter,int iChain);

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strChannel;


// Member functions
public:
	CWireless_WiFi_ContinuousRxTest() {}
	~CWireless_WiFi_ContinuousRxTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_WIRELESS_WIFI_CONTINOUS_RX_TEST_H_
