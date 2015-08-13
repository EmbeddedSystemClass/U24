#ifndef _C_WIRELESS_QMI_RF_H_
#define _C_WIRELESS_QMI_RF_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_QmiRfTxTest : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_QmiRfTxTest)

// Data members
public:
protected:
private:
	std::string m_strPICSName;

	int m_iCommandDelay;
	int m_iPowerDegree;
	double m_dFrequency;
	int m_iRetryTimes;
	int m_iInputPort;
	double m_dLower, m_dUpper;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strChannel;

	bool m_WriteCPKLog;
	std::vector <std::string> m_CPKHeaderList;

// Member functions
public:
	CWireless_QmiRfTxTest();
	~CWireless_QmiRfTxTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
	bool ParseCPKItems();
private:
	bool MainFunction();
	double GetLossByDegree(int iDegree);
};


#endif // End of #ifndef _C_WIRELESS_QMI_RF_H_
