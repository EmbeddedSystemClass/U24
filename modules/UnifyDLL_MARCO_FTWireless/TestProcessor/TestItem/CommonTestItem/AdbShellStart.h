#ifndef _C_ADB_SHELL_START_H_
#define _C_ADB_SHELL_START_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CAdbShellStart : public ITestItem
{
RF_DECLARE_DYNCREATE(CAdbShellStart)

// Data members
public:
protected:
private:
	int m_msWaitTime;

// Member functions
public:
	CAdbShellStart() {}
	~CAdbShellStart() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_ADB_SHELL_START_H_
