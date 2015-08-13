#ifndef _C_WIRELESS_WCDMA_RSSI_H_
#define _C_WIRELESS_WCDMA_RSSI_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_WCDMA_Rssi : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_WCDMA_Rssi)

// Data members
public:
protected:
private:
	std::string m_strPICSName;

	int m_iCommandDelay;
	int m_iRetryTimes;
	int m_iOutputPort;
	double m_dLower, m_dUpper;
	double m_dFreq;
	int    m_iRFLevel;
	DWORD  m_dwDelayAfterCMURun;
	DWORD  m_dwCommandDelay;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strChannel;

	bool m_WriteCPKLog;
	std::vector <std::string> m_CPKHeaderList;

// Member functions
public:
	CWireless_WCDMA_Rssi();
	~CWireless_WCDMA_Rssi() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
	bool ParseCPKItems();
private:
	bool MainFunction();
	double GetLossByDegree(int iDegree);
};


#endif // End of #ifndef _C_WIRELESS_WCDMA_RSSI_H_
