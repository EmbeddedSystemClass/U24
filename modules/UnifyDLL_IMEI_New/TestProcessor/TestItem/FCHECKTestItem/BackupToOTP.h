#ifndef _C_CHECK_MOBILE_IMEI_H_
#define _C_CHECK_MOBILE_IMEI_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CBackupToOTP : public ITestItem
{
RF_DECLARE_DYNCREATE(CBackupToOTP)

// Data members
public:
protected:
private:
	int m_iRetry;
	bool m_bBackupFA;
	bool m_bBackupCFG;
	bool m_bBackupModemCFG;
// Member functions
public:
	CBackupToOTP() {}
	~CBackupToOTP() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_CHECK_MOBILE_IMEI_H_
