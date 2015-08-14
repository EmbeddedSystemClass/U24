#ifndef _C_GET_CURRENT_H_
#define _C_GET_CURRENT_H_

#include "BaseObject/ITestItem.h"
#include "Utils/StringUtil.h"
#include "ErrorDefine/Function_ErrorDefine.h"
#include "Processor/GPProcessor.h"
#include "Utils/InspectionUtil.h"

class CGetCurrent : public ITestItem
{
GP_DECLARE_DYNCREATE(CGetCurrent)


// Data members
public:
protected:
private:
	double m_dLower, m_dUpper;

	int m_iPowerSupplyNo;
	int m_iWaitTimeForFetch;
	int m_iAverageTimes;
	bool m_bNoNegative;

	//Super4Relay* m_pRBCtrl;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strErrCode;
	std::string m_strPICSName;
	std::string m_strUnit;


// Member functions
public:
	CGetCurrent(void);
	virtual ~CGetCurrent(void);

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();

	//cast
	CGPProcessor    *m_pCGProc;
	IPowerSupply    *m_pIPS;

};


#endif // End of #ifndef _C_GET_CURRENT_H_
