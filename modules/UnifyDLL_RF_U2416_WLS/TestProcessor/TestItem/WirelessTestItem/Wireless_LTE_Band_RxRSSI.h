#ifndef _C_Wireless_LTE_Band_RXRSSI_H_
#define _C_Wireless_LTE_Band_RXRSSI_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWireless_LTE_Band_RxRSSI : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_LTE_Band_RxRSSI)

// Data members
public:
protected:
private:

	std::string m_strPICSName;

	int m_iBand;
	int m_iCommandDelay;
	int m_iRetryTimes;
	int m_iOutputPort;
	double m_dFreq;
	int m_iRFLevel;
	int m_iChannel;
	int m_iTxBandwidth;
	int m_iRxBandwidth;

	int m_iAGCModeLNAValue; //AGC Mode
	int m_iCalibrationState; // Calibration 

	double m_dLower, m_dUpper;

	DWORD  m_dwDelayAfterCMURun;
	DWORD  m_dwCommandDelay;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strChannel;
	std::string m_strRXMode;

	std::string m_strReadValueMode; //AGC MODE

	bool m_WriteCPKLog;
	std::vector <std::string> m_CPKHeaderList;

// Member functions
public:
	CWireless_LTE_Band_RxRSSI();
	~CWireless_LTE_Band_RxRSSI() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
	bool ParseCPKItems();
private:

	std::string MakeLTE_ErrorCode( int iBand , std::string sMode );
	short GetQLibBandNum(int iBand);
	bool MainFunction();
	double GetLossByDegree(int iDegree);
};


#endif // End of #ifndef _C_Wireless_LTE_Band_RXRSSI_H_
