#ifndef _C_EXECUTE_ADB_COMMAND_H_
#define _C_EXECUTE_ADB_COMMAND_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CExeADBCmd : public ITestItem
{
RF_DECLARE_DYNCREATE(CExeADBCmd)

// Data members
public:
protected:
private:
	int m_iRetry;
	std::string m_strTitle;
	CString m_CStrCommand;

// Member functions
public:
	CExeADBCmd() {}
	~CExeADBCmd() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
	bool ExecAdbOut(CString Command, char* output, char* ErrorCode);
};


#endif // End of #ifndef _C_EXECUTE_ADB_COMMAND_H_
