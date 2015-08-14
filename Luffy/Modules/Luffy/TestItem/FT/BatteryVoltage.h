#ifndef _C_BATTERY_VOLTAGE_H_
#define _C_BATTERY_VOLTAGE_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Function_ErrorDefine.h"
#include "Utils/InspectionUtil.h"


typedef struct
{
	double dLower;
	double dUpper;

	std::string strLower;
	std::string strUpper;
} TESTING_RANGE;

class CBatteryVoltage : public ITestItem
{
GP_DECLARE_DYNCREATE(CBatteryVoltage)

		// Data members
public:
protected:
private:
	std::string		m_strPICSName;

	short           m_nCmdCode;
	std::string     m_strErrCode;
	std::string     m_strMsg;
	std::string		m_strMeasured;

	// RTC Voltage criteria
	TESTING_RANGE   m_Range_RTCVoltage;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

	// Member functions
public:
	CBatteryVoltage() {}
	~CBatteryVoltage() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) ;
	virtual bool Run(int iItem = 0) ;

protected:
private:
	bool MainFunction();
	
};


#endif // End of #ifndef _C_BATTERY_VOLTAGE_H_
