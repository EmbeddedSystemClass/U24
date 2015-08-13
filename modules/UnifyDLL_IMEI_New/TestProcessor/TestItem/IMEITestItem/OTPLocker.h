#ifndef _C_OTP_LOCKER_H_
#define _C_OTP_LOCKER_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class COTPLocker : public ITestItem
{
RF_DECLARE_DYNCREATE(COTPLocker)

// Data members
public:
protected:
private:
	int m_iRetry;
	bool m_bOTPLock;

// Member functions
public:
	COTPLocker() {}
	~COTPLocker() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_OTP_LOCKER_H_
