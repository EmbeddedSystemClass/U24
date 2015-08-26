#ifndef _C_WIRELESS_WIFI_CONTINOUS_TX_CWWAVE_H_
#define _C_WIRELESS_WIFI_CONTINOUS_TX_CWWAVE_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_WiFi_Continuous5GTxCwWave : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_WiFi_Continuous5GTxCwWave)

// Data members
public:
protected:
private:
	int m_iCommandDelay;
	int m_iChannel;
	int m_iRfGain;
	int m_iBond;
	int m_iChain;
	int m_iWlandMode;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strChannel;


// Member functions
public:
	CWireless_WiFi_Continuous5GTxCwWave() {}
	~CWireless_WiFi_Continuous5GTxCwWave() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_WIRELESS_WIFI_CONTINOUS_TX_CWWAVE_H_
