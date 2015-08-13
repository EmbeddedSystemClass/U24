#ifndef _C_ANTENNA_TUNER_CAL_H_
#define _C_ANTENNA_TUNER_CAL_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CAntenna_Tuner_Cal : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CAntenna_Tuner_Cal)

// Data members
public:
protected:
private:
	std::string m_strPICSName;

	std::string m_strMsg;
	std::string m_strMeasured;

// Member functions
public:
	CAntenna_Tuner_Cal();
	~CAntenna_Tuner_Cal() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_ANTENNA_TUNER_CAL_H_
