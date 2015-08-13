#ifndef _C_EXEC_ADB_COMMAND_H_
#define _C_EXEC_ADB_COMMAND_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CExecAdbCommand : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CExecAdbCommand)

// Data members
public:
protected:
private:

	std::string m_strErrCode,m_strMsg;
	std::string m_strCommand;
	int m_nCommandWait;


// Member functions
public:
	CExecAdbCommand() {}
	~CExecAdbCommand() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_EXEC_ADB_COMMAND_H_
