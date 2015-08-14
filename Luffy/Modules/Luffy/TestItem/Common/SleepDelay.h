#ifndef _C_SLEEP_DELAY_H_
#define _C_SLEEP_DELAY_H_

#include "BaseObject/ITestItem.h"
#include "Processor/GPProcessor.h"
#include "FacTestToolInterface.h"

class CSleepDelay : public ITestItem
{
GP_DECLARE_DYNCREATE(CSleepDelay)


// Data members
public:
protected:
private:
	int m_iWaitTime;
	bool m_blnShowMsgbox;
	std::string m_strPICSName;

	CGPProcessor    *m_pCGProc;
	bool m_bTestResult;

	st_UIMessage m_st_UIMessage;

// Member functions
public:
	CSleepDelay();
	~CSleepDelay() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(int iItem = 0);
};


#endif // End of #ifndef _C_SLEEP_DELAY_H_
