#ifndef _C_WIRELESS_GSM_RXLEVEL_H_
#define _C_WIRELESS_GSM_RXLEVEL_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_GSM_RxLevel : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_GSM_RxLevel)

// Data members
public:
protected:
private:
	std::string m_strPICSName;

	int m_iExpected_RX_Power_dbm16;
	int m_iNumber_of_Averages;
	int m_iDelayAfterCMURun;
	int m_iAFFrequency;
	int m_iModIndex;
	int m_iChannel;
	int m_iOutputPort;
	int m_iRFLevel;

	std::string m_strMsg;
	std::string m_strMeasured;

	double m_dFreq;
	double m_dLower, m_dUpper;

	int m_iGSMMode;
	short m_sGSMBand;

	bool m_WriteCPKLog;
	std::vector <std::string> m_CPKHeaderList;

// Member functions
public:
	CWireless_GSM_RxLevel();
	~CWireless_GSM_RxLevel() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
	
	bool ParseCPKItems();
private:
	bool MainFunction();
	double GetLossByDegree(int iDegree);
	short GetQLibBandNum(int iBand);
};


#endif // End of #ifndef _C_WIRELESS_GSM_RXLEVEL_H_
