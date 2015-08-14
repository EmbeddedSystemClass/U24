#ifndef _C_LEAKAGE_CURRENT_H_
#define _C_LEAKAGE_CURRENT_H_

#include "BaseObject/ITestItem.h"
#include "Utils/StringUtil.h"
#include "ErrorDefine/Function_ErrorDefine.h"
#include "Processor/GPProcessor.h"
#include "Utils/InspectionUtil.h"
#include "FacTestToolInterface.h"

class CLeakageCurrent : public ITestItem
{
GP_DECLARE_DYNCREATE(CLeakageCurrent)


// Data members
public:
protected:
private:
	double m_dLower, m_dUpper;

	int m_iWaitTimeForShutdown;
	int m_iWaitTimeForFetch;
    int m_iFetchCount;

	//Super4Relay* m_pRBCtrl;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strErrCode;
	std::vector<std::string> m_vLeakageCurrentRange;


// Member functions
public:
	//CLeakageCurrent() {}
	//~CLeakageCurrent() {}
	CLeakageCurrent(void);
	virtual ~CLeakageCurrent(void);

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();


	//cast
	CGPProcessor    *m_pCGProc;
	IPowerSupply    *m_pIPS;
	IPowerSupply    *m_pIPS2;

	st_UIMessage m_st_UIMessage;

};


#endif // End of #ifndef _C_LEAKAGE_CURRENT_H_
