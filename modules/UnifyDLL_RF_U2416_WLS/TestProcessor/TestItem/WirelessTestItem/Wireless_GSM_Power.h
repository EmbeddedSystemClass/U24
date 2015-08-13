#ifndef _C_WIRELESS_GSM_POWER_H_
#define _C_WIRELESS_GSM_POWER_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_GSM_Power : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_GSM_Power)

// Data members
public:
protected:
private:
	std::string m_strPICSName;

	int m_iCommandDelay;
	int m_iPowerDegree;
	int m_iPowerLevel;
	int m_iChannel;
	double m_dFrequency;
	int m_iRetryTimes;
	int m_iInputPort;
	double m_dLower, m_dUpper;

	int m_iGSMMode;
	short m_sGSMBand;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strFrequency;

	bool m_WriteCPKLog;
	std::vector <std::string> m_CPKHeaderList;

// Member functions
public:
	CWireless_GSM_Power();
	~CWireless_GSM_Power() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
	bool ParseCPKItems();
private:
	bool MainFunction();
	double GetLossByDegree(int iDegree);
	short GetQLibBandNum(int iBand);
};


#endif // End of #ifndef _C_WIRELESS_GSM_POWER_H_
