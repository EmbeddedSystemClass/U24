#ifndef _C_WIRELESS_LTE_Band_TXPOWER_H_
#define _C_WIRELESS_LTE_Band_TXPOWER_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_LTE_Band_TxPower : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_LTE_Band_TxPower)

// Data members
public:
protected:
private:
	std::string m_strPICSName;

	int m_iBand;
	int m_iCommandDelay;
	int m_iStartDelay;
	int m_iChannel;
	int m_iTxBandwidth;
	int m_iRxBandwidth;
	int m_iTxGainIndex;
	int m_iPowerDegree;
	int m_iPowerLevel;
	double m_dFrequency;
	int m_iRetryTimes;
	int m_iInputPort;
	double m_dLower, m_dUpper;
	int m_iTestMethod;
	std::string m_strLevelMode;
	int m_iLevelMax;


	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strChannel;

	bool m_WriteCPKLog;
	std::vector <std::string> m_CPKHeaderList;


// Member functions
public:
	CWireless_LTE_Band_TxPower();
	~CWireless_LTE_Band_TxPower() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
	bool ParseCPKItems();

private:

	std::string MakeLTE_ErrorCode( int iBand );
	short GetQLibBandNum(int iBand);
	bool MainFunction();
	bool GainIndexTest();
	bool PowerDbmTest();
	double GetLossByDegree(int iDegree);
};


#endif // End of #ifndef _C_WIRELESS_LTE_Band_TXPOWER_H_
