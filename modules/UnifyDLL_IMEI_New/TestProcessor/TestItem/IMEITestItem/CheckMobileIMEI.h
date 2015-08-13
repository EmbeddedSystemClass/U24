#ifndef _C_CHECK_MOBILE_IMEI_H_
#define _C_CHECK_MOBILE_IMEI_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CCheckMobileIMEI : public ITestItem
{
RF_DECLARE_DYNCREATE(CCheckMobileIMEI)

// Data members
public:
protected:
private:
	int m_iRetry;
	int m_iIMEILength;

// Member functions
public:
	CCheckMobileIMEI() {}
	~CCheckMobileIMEI() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_CHECK_MOBILE_IMEI_H_
