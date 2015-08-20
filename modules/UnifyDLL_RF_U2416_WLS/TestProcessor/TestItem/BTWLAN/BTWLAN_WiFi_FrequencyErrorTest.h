#ifndef _C_BTWLAN_WIFI_FREQUENCYERROR_TEST_H_
#define _C_BTWLAN_WIFI_FREQUENCYERROR_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"

class CBTWLAN_WiFi_FrequencyErrorTest : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CBTWLAN_WiFi_FrequencyErrorTest)

private:
	std::string m_strPICSName;
	std::string m_strPICSName_Value;

	double m_dFrequency;
	double m_dRBW, m_dVBW, m_dSpan;
	double m_dLower, m_dUpper;
	int m_iCommandDelay;
	//int m_iStartDelay;
	int m_iRetryTimes;
	int m_iChannel;
	int m_iGain;
	int m_iAttenuationManual;
	//int m_iRefLevel;
	long m_lTone;
	unsigned long m_lAmpl;
	double m_dPeakExcursion;
	int m_iTrace;
	double m_iFreqCounterRes;

	std::string m_strChannel;
	std::string m_strTraceMode;
	std::string m_strDetector;
	std::string m_strMsg;
	std::string m_strMeasured;

	bool m_WriteCPKLog;
	std::vector <std::string> m_CPKHeaderList;

	double m_dChannelBWMHz, m_dAdjPower;
	int m_Regulatory_Fixed;
	int m_Power_control_mode;

	int m_iAverageTimes;
	int m_iPower;
	int m_iRateBitIndex;
	int m_iSweepTime;
	int m_iRefLevel;
	int m_iWlanMode;
	int m_iTxChain;
	int m_iLoadBin;

	int m_iPreamble;
	int m_iPayloadSize;
	int m_iSpacing;
	int m_iChain;

	std::string m_strLineName, m_strDomain;
	std::string m_strXscalingMode, m_strYscalingMode;
	std::string m_strUnit, m_strSpacing;
	std::string m_strContData, m_strUppData;
	std::string m_strChannelPower;
	bool m_bKLossMode;

public:
	CBTWLAN_WiFi_FrequencyErrorTest(void);
	virtual ~CBTWLAN_WiFi_FrequencyErrorTest(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(void);

private:
	bool MainFunction(void);

protected:
	bool ParseCPKItems();
};

#endif // End of #ifndef _C_BTWLAN_WIFI_FREQUENCYERROR_TEST_H_