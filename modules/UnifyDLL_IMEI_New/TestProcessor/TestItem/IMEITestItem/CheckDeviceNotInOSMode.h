#ifndef _C_CHECK_DEVICE_NOT_IN_OS_MODE_H_
#define _C_CHECK_DEVICE_NOT_IN_OS_MODE_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CCheckDeviceNotInOSMode : public ITestItem
{
RF_DECLARE_DYNCREATE(CCheckDeviceNotInOSMode)

// Data members
public:
protected:
private:
	int m_iRetry;
// Member functions
public:
	CCheckDeviceNotInOSMode() {}
	~CCheckDeviceNotInOSMode() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_CHECK_DEVICE_NOT_IN_OS_MODE_H_
