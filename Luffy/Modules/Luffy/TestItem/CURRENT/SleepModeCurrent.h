#ifndef _C_SLEEP_MODE_LEAKQGE_CURRENT_H_
#define _C_SLEEP_MODE_LEAKQGE_CURRENT_H_

#include "BaseObject/ITestItem.h"
#include "Utils/StringUtil.h"
#include "ErrorDefine/Function_ErrorDefine.h"
#include "Processor/GPProcessor.h"
#include "Utils/InspectionUtil.h"
#include "FacTestToolInterface.h"

class CSleepModeLeakageCurrent : public ITestItem
{
GP_DECLARE_DYNCREATE(CSleepModeLeakageCurrent)


// Data members
public:
protected:
private:
	double m_dLower, m_dUpper;

	int m_iWaitTimeForShutdown;
	int m_iWaitTimeForFetch;
    int m_iFetchCount;
	int m_iInRangeCount;
	//Super4Relay* m_pRBCtrl;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strErrCode;
	std::vector<std::string> m_vLeakageCurrentRange;

	std::string m_szIicCmdTemp;

	int iiccmd;
	int iicadd;
	int iicdat;
	int iicdel;

// Member functions
public:
	//CLeakageCurrent() {}
	//~CLeakageCurrent() {}
	CSleepModeLeakageCurrent(void);
	virtual ~CSleepModeLeakageCurrent(void);

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

#endif