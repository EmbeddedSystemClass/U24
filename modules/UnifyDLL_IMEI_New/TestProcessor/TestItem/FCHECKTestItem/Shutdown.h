#ifndef _C_SHUTDOWN_H_
#define _C_SHUTDOWN_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CShutdown : public ITestItem
{
RF_DECLARE_DYNCREATE(CShutdown)

// Data members
public:
protected:
private:
	int m_iFTDRetry;
	int m_iShutdownRetry;

// Member functions
public:
	CShutdown() {}
	~CShutdown() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_SHUTDOWN_H_
