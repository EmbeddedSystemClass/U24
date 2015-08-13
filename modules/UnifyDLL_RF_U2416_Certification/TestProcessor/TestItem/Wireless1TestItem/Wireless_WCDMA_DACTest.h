#ifndef _C_WIRELESS_WCDMA_DAC_TEST_H_
#define _C_WIRELESS_WCDMA_DAC_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_WCDMA_DACTest : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_WCDMA_DACTest)

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
	CWireless_WCDMA_DACTest() {}
	~CWireless_WCDMA_DACTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();

	int GetPDM(int iBand, double dDesiredPower);
	short GetPhoneMode(int iBand);
	double GetLossByBand(int iBand);
};


#endif // End of #ifndef _C_WIRELESS_WCDMA_DAC_TEST_H_
