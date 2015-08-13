#ifndef _C_BTWLAN_BT_TX_TEST_H_
#define _C_BTWLAN_BT_TX_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"

class CBTWLAN_BT_TxTest : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CBTWLAN_BT_TxTest)

private:
	std::string m_strPICSName;
	std::string m_strPICSName_Value;

	double m_dFrequency;
	double m_dAdjPower, m_dRBW, m_dVBW, m_dSpan;
	double m_dLower, m_dUpper;
	double m_dChannelBWMHz;
	int m_iCommandDelay;
	//int m_iStartDelay;
	int m_iAverageTimes;
	int m_iSweepTime;
	int m_iAttenuationManual;
	int m_iRefLevel;

	std::string m_strChannel;
	std::string m_strTraceMode;
	std::string m_strDetector;
	std::string m_strDiagramFull;
	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strBTInput;
	std::string m_strChannelPower;

	bool m_WriteCPKLog;
	bool m_bKLossMode;
	std::vector <std::string> m_CPKHeaderList;

public:
	CBTWLAN_BT_TxTest(void);
	virtual ~CBTWLAN_BT_TxTest(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(void);

private:
	bool MainFunction(void);

protected:
	bool ParseCPKItems();
};

#endif // End of #ifndef _C_BTWLAN_BT_TX_TEST_H_