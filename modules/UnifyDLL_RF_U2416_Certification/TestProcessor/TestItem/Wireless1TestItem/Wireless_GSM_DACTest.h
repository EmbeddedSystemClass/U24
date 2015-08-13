#ifndef _C_WIRELESS_GSM_DAC_TEST_H_
#define _C_WIRELESS_GSM_DAC_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_GSM_DACTest : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_GSM_DACTest)

// Data members
public:
protected:
private:
	std::string m_strPICSName;
	std::string m_strPICSName_Value;

	int m_iBand;
	int m_iChannel;

	int m_iRetryTimes;
	double m_dDesirePower;
	double m_dLower, m_dUpper;


	std::string m_strMsg;
	std::string m_strMeasured;


// Member functions
public:
	CWireless_GSM_DACTest() {}
	~CWireless_GSM_DACTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();

	int GetDAC(int iBand, double dDesiredPower);
	short GetPhoneMode(int iBand);
	double GetLossByBand(int iBand);
};


#endif // End of #ifndef _C_WIRELESS_GSM_DAC_TEST_H_
