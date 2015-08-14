#ifndef _C_BATTERY_GAUGE_ID_H_
#define _C_BATTERY_GAUGE_ID_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Function_ErrorDefine.h"


class CBatteryGaugeID : public ITestItem
{
GP_DECLARE_DYNCREATE(CBatteryGaugeID)

		// Data members
public:
protected:
private:
	std::string		m_strPICSName;
	std::string		m_strBatteryID;

	short           m_nCmdCode;
	std::string     m_strErrCode;
	std::string     m_strMsg;
	std::string m_strMeasured;
	int m_ID;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

	// Member functions
public:
	CBatteryGaugeID() {}
	~CBatteryGaugeID() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) ;
	virtual bool Run(int iItem = 0) ;

protected:
private:
	bool MainFunction();
	
};


#endif // End of #ifndef _C_BATTERY_ID_H_
