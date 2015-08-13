#ifndef _C_CONNECT_PHONE_QMSL_H_
#define _C_CONNECT_PHONE_QMSL_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "../../../CommonUtility/BaseObject/IErrorHandling.h"

class CConnectPhone_QMSL : public ITestItem
{
	RF_DECLARE_DYNCREATE(CConnectPhone_QMSL)


	// Data members
public:
protected:
private:
	int m_iWaitTimeForBoot;
	int m_iRetryConnectTimes;
	int m_iWaitTimeAfterConnect;
	std::string m_strPICSName;

//	std::string m_strErrCode;
//	std::string m_strMsg;

	IErrorHandling* m_pIErrorHandling;

	// Member functions
public:
	CConnectPhone_QMSL() {}
	~CConnectPhone_QMSL() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_CONNECT_PHONE_QMSL_H_
