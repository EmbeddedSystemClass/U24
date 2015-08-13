#ifndef _C_SYNC_NV_TO_EFS_QMSL_H_
#define _C_SYNC_NV_TO_EFS_QMSL_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CSyncNVToEFS_QMSL : public ITestItem
{
	RF_DECLARE_DYNCREATE(CSyncNVToEFS_QMSL)


	// Data members
public:
protected:
private:
	int m_iRetryTimes;
	int m_iSyncCmdTimeout;
	std::string m_strPICSName;

	std::string m_strMsg;
	std::string m_strErrCode;


	// Member functions
public:
	CSyncNVToEFS_QMSL() {}
	~CSyncNVToEFS_QMSL() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_SYNC_NV_TO_EFS_QMSL_H_
