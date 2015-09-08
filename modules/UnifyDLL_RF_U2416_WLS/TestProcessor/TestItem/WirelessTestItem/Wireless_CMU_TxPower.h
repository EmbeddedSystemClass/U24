#ifndef _C_Wireless_CMU_TxPower_H_
#define _C_Wireless_CMU_TxPower_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_CMU_TxPower : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_CMU_TxPower)

// Data members
public:
protected:
private:
	std::string m_strPICSName;
	std::string m_strLineName;

	int m_iBand;
	int m_iCommandDelay;
	int m_iStartDelay;
	int m_iChannel;
	int m_iTxBandwidth;
	int m_iTxGainIndex;
	double m_dFrequency;
	int m_iRetryTimes;
	int m_iInputPort;
	double m_dAdjPower;

	double m_dLower, m_dUpper;
	int m_iTestMethod;
	std::string m_strLevelMode;
	int m_iLevelMax;

	//int m_iWlanMode;
	int m_iChain;
	int m_iPower;
	int m_iRateBitIndex;
	int m_iWlanMode;
	int m_iLoadBin;
	int m_iTxChain;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strChannel;

	bool m_WriteCPKLog;
	std::vector <std::string> m_CPKHeaderList;


// Member functions
public:
	CWireless_CMU_TxPower();
	~CWireless_CMU_TxPower() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
	

private:

	bool MainFunction();
	bool ParseCPKItems();
	
};


#endif // End of #ifndef _C_Wireless_CMU_TxPower_H_
