#ifndef _C_WIRELESS_WIFI_CONTINOUS_TX_CWWAVE_H_
#define _C_WIRELESS_WIFI_CONTINOUS_TX_CWWAVE_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_WiFi_ContinuousTxCwWave : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_WiFi_ContinuousTxCwWave)

// Data members
public:
protected:
private:
	int m_iCommandDelay;
	//int m_iChannel;
	//int m_iRfGain;
	//int m_iChain;
	//int m_iWlandMode;

	int m_iChannel;
	int m_iPower;
	int m_iRateBitIndex;
	int m_iWlandMode;
	//int m_iPayloadSize;
	//int m_iSpacing;
	int m_iChain;


	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strChannel;


// Member functions
public:
	CWireless_WiFi_ContinuousTxCwWave() {}
	~CWireless_WiFi_ContinuousTxCwWave() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_WIRELESS_WIFI_CONTINOUS_TX_CWWAVE_H_
