#ifndef _C_WIRELESS_GPS_CN0_TEST_H_
#define _C_WIRELESS_GPS_CN0_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_GPS_CN0Test : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_GPS_CN0Test)

// Data members
public:
protected:
private:
	std::string m_strPICSName;
	std::string m_strPICSName_Value;

	int m_iRetryTimes;
	int m_iOutputPort;
	double m_dCN0Lower, m_dCN0Upper;
	int m_iFreqLower, m_iFreqUpper;
	double m_dFreq;
	int    m_iRFLevel;
	DWORD  m_dwDelayAfterCMURun;
	DWORD  m_dwCommandDelay;

	std::string m_strMsg;
	std::string m_strMeasured;

	bool m_WriteCPKLog;
	bool m_bKLossMode;
	std::vector <std::string> m_CPKHeaderList;

// Member functions
public:
	CWireless_GPS_CN0Test();
	~CWireless_GPS_CN0Test() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
	bool ParseCPKItems();

private:
	bool MainFunction();
};


#endif // End of #ifndef _C_WIRELESS_GPS_CN0_TEST_H_
