#ifndef _C_WIRELESS_WIFI_CONTINOUS_5G_TX_TEST_H_
#define _C_WIRELESS_WIFI_CONTINOUS_5G_TX_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_WiFi_Continuous5GTxTest : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_WiFi_Continuous5GTxTest)

// Data members
public:
protected:
private:
	int m_iCommandDelay;
	//int m_iStartDelay;
	int m_iChannel;
	int m_iPower;
	int m_iDataRate;
	int m_iPreamble;
	int m_iPayloadSize;
	int m_iSpacing;
	int m_iBond;
	int m_iChain;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strChannel;


// Member functions
public:
	CWireless_WiFi_Continuous5GTxTest() {}
	~CWireless_WiFi_Continuous5GTxTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_WIRELESS_WIFI_CONTINOUS_5G_TX_TEST_H_
