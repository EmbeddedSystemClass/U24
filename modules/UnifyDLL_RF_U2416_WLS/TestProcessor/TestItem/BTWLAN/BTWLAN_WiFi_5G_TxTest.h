#ifndef _C_BTWLAN_WIFI_5G_TX_TEST_H_
#define _C_BTWLAN_WIFI_5G_TX_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"

class CBTWLAN_WiFi_5G_TxTest : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CBTWLAN_WiFi_5G_TxTest)

private:
	std::string m_strPICSName;
	std::string m_strPICSName_Value;

	double m_dFrequency;
	double /*m_dLoss,*/ m_dRBW, m_dVBW, m_dSpan;
	double m_dLower, m_dUpper;
	double m_dChannelBWMHz;
	int m_iCommandDelay;
	double m_dAdjPower;
	//int m_iStartDelay;
	int m_iRetryTimes;
	int m_iChannel;
	int m_iPower;
	int m_iRateBitIndex;
	int m_iSweepTime;
	int m_iAttenuationManual;
	int m_iRefLevel;
	int m_Regulatory_Fixed;
	int m_Power_control_mode;
	int m_iWlanMode;
	int m_iTxChain;
	int m_iLoadBin;

	std::string m_strChannel;
	std::string m_strTraceMode;
	std::string m_strDetector;
	std::string m_strChannelPower;
	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strLineName, m_strDomain;

	bool m_WriteCPKLog;
	bool m_bKLossMode;
	std::vector <std::string> m_CPKHeaderList;

public:
	CBTWLAN_WiFi_5G_TxTest(void);
	virtual ~CBTWLAN_WiFi_5G_TxTest(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(void);

private:
	bool MainFunction(void);
protected:
	bool ParseCPKItems();
};

#endif // End of #ifndef _C_BTWLAN_WIFI_5G_TX_TEST_H_