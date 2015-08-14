#ifndef _C_CHARGE_CURRENT_H_
#define _C_CHARGE_CURRENT_H_

#include "BaseObject/ITestItem.h"
#include "Utils/StringUtil.h"
#include "ErrorDefine/Function_ErrorDefine.h"
#include "Processor/GPProcessor.h"
#include "Utils/InspectionUtil.h"



class CChargeCurrent : public ITestItem
{
GP_DECLARE_DYNCREATE(CChargeCurrent)


// Data members
public:
protected:
private:
	double m_dChargeVoltage, m_dNormalVoltage;
	int m_iWaitTimeForFetch;
	double m_dLower, m_dUpper;

	std::string m_strMsg;
	std::string m_strMeasured;

	//cast
	CGPProcessor    *m_pCGProc;
	IPowerSupply    *m_pIPS;
	IPowerSupply    *m_pIPS2;


// Member functions
public:
	//CChargeCurrent() {}
	//~CChargeCurrent() {}
	CChargeCurrent(void);
	virtual ~CChargeCurrent(void);

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_CHARGE_CURRENT_H_
