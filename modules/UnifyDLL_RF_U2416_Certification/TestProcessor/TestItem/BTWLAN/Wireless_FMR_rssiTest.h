#ifndef _C_WIRELESS_FMR_RSSI_TEST_H_
#define _C_WIRELESS_FMR_RSSI_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"

class CWireless_FMR_rssiTest : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_FMR_rssiTest)

private:
	std::string m_strPICSName;
	std::string m_strPICSName_Value;

	int m_iRetryTimes;
	int m_iOutputPort;
	int m_iLower, m_iUpper;
	double m_dFreq;
	int    m_iRFLevel;
	DWORD  m_dwDelayAfterCMURun;
	DWORD  m_dwCommandDelay;

	std::string m_strMsg;
	std::string m_strMeasured;

	bool m_WriteCPKLog;
	std::vector <std::string> m_CPKHeaderList;

public:
	CWireless_FMR_rssiTest(void);
	~CWireless_FMR_rssiTest(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	bool Run(void);

protected:
	bool ParseCPKItems();

private:
	bool MainFunction(void);
};
#endif // End of #ifndef _C_WIRELESS_FMR_RSSI_TEST_H_