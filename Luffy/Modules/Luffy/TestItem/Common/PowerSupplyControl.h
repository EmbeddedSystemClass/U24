#ifndef _C_POWER_SUPPLY_CONTROL_H_
#define _C_POWER_SUPPLY_CONTROL_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Common_ErrorDefine.h"
#include "FacTestToolInterface.h"

class CPowerSupplyControl : public ITestItem
{
GP_DECLARE_DYNCREATE(CPowerSupplyControl)


// Data members
public:
protected:
private:
	ITestBox		*m_pITestBox;
	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

	//st_UIMessage m_st_UIMessage;

	int     m_iPS_Index;
	//double  m_dStepdVoltage;
	//double  m_dFinaldVoltage;
	//double  m_dCurrent;
	bool    m_bPS_OnOff;
	int     m_iDelay;
	std::string m_strPICSName;
	std::string m_strMsg;

	friend class CEraseOs;
	// Member functions
public:
	CPowerSupplyControl() {}
	~CPowerSupplyControl() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();
};



#endif // End of #ifndef _C_POWER_SUPPLY_CONTROL_H_
