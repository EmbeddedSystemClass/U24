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
	int m_iRefLevel;
	long m_lTone;
	unsigned long m_lAmpl;
	double m_dPeakExcursion;
	int m_iTrace;
	double m_iFreqCounterRes;

	

	std::string m_strChannel;
	std::string m_strTraceMode;
	std::string m_strDetector;
	std::string m_strDiagramFull;
	std::string m_strMsg;
	std::string m_strMeasured;

	bool m_WriteCPKLog;
	std::vector <std::string> m_CPKHeaderList;

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