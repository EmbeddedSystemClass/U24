#ifndef _C_WIRELESS_WIFI_TX_TEST_H_
#define _C_WIRELESS_WIFI_TX_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_WiFi_TxTest : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_WiFi_TxTest)

// Data members
public:
protected:
private:
	std::string m_strPICSName;
	std::string m_strPICSName_Value;

	int m_iCommandDelay;
	//int m_iStartDelay;
	int m_iChannel;
	int m_iPower;
	int m_iDataRate;
	int m_iRetryTimes;
	int m_iInputPort;
	double m_dLower, m_dUpper;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strChannel;


// Member functions
public:
	CWireless_WiFi_TxTest() {}
	~CWireless_WiFi_TxTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_WIRELESS_WIFI_TX_TEST_H_
