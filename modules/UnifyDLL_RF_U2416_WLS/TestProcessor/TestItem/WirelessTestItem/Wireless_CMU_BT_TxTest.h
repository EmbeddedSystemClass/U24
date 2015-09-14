#ifndef _C_WIRELESS_CMU_BT_TX_TEST_H_
#define _C_WIRELESS_CMU_BT_TX_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_CMU_BT_TxTest : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_CMU_BT_TxTest)

// Data members
public:
protected:
private:
	std::string m_strPICSName;

	int m_iCommandDelay;
	int m_i_BTStartDelay;
	double m_dFrequency;
	int m_iRetryTimes;
	int m_iInputPort;
	double m_dLower, m_dUpper;
	int m_iLoadBin;
	double m_dAdjPower;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strChannel;
	std::string m_strBTInput;


	bool m_WriteCPKLog;
	std::vector <std::string> m_CPKHeaderList;


// Member functions
public:
	CWireless_CMU_BT_TxTest();
	~CWireless_CMU_BT_TxTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run();

protected:


private:
	bool MainFunction();
	bool ParseCPKItems();

};


#endif // End of #ifndef _C_WIRELESS_BT_TX_TEST_H_