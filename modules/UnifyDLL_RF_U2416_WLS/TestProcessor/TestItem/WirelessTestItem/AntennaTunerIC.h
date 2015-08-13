#ifndef _C_NFC_ANTENNA_TUNER_IC_H_
#define _C_NFC_ANTENNA_TUNER_IC_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CNFC_Antenna_Tuner_IC : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CNFC_Antenna_Tuner_IC)

// Data members
public:
protected:
private:
	std::string m_strPICSName;

	std::string m_strAntennaTunerIC;
	std::string m_strMsg;
	std::string m_strMeasured;

// Member functions
public:
	CNFC_Antenna_Tuner_IC();
	~CNFC_Antenna_Tuner_IC() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_NFC_ANTENNA_TUNER_IC_H_
