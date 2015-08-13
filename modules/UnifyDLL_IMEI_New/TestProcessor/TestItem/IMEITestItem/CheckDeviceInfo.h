#ifndef _C_CHECK_DEVICE_INFO_H_
#define _C_CHECK_DEVICE_INFO_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CCheckDeviceInfo : public ITestItem
{
RF_DECLARE_DYNCREATE(CCheckDeviceInfo)

// Data members
public:
protected:
private:
	int m_iRetry;
	bool m_bCheckIMEI;
	bool m_bCheckBT;
	bool m_bCheckWIFI;
	bool m_bCheckMSN;
	bool m_bCheckFSN;
	bool m_bCheckSIMData;

// Member functions
public:
	CCheckDeviceInfo() {}
	~CCheckDeviceInfo() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
	bool CheckIMEI();
	bool CheckBT();
	bool CheckWIFI();
	bool CheckMSN();
	bool CheckFSN();
	bool CheckSIMData();
};


#endif // End of #ifndef _C_CHECK_DEVICE_INFO_H_
