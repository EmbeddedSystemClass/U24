#ifndef _C_CONNECT_PHONE_H_
#define _C_CONNECT_PHONE_H_


#include "BaseObject/ITestItem.h"
#include "Processor/GPProcessor.h"


class CConnectPhone : public ITestItem
{
GP_DECLARE_DYNCREATE(CConnectPhone)


// Data members
public:
protected:
private:
	int m_iWaitTimeForBoot;
	int m_iRetryConnectTimes;
	int m_iWaitTimeAfterConnect;
	std::string m_strPICSName;

	std::string m_strMsg;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;


// Member functions
public:
	CConnectPhone() {}
	~CConnectPhone() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_CONNECT_PHONE_H_
