#ifndef _C_ADB_SHELL_START_H_
#define _C_ADB_SHELL_START_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CExeAdbCommand : public ITestItem
{
RF_DECLARE_DYNCREATE(CExeAdbCommand)

// Data members
public:
protected:
private:
	int m_msWaitTime;
	std::string m_Adbcommand;

// Member functions
public:
	CExeAdbCommand() {}
	~CExeAdbCommand() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_ADB_SHELL_START_H_
