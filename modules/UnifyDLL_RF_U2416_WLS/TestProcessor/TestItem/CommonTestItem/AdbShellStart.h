#ifndef _C_ADB_SHELL_START_H_
#define _C_ADB_SHELL_START_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CAdbShellStart : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CAdbShellStart)

// Data members
public:
protected:
private:

	std::string m_strErrCode,m_strMsg;
	int m_nWiteStart;


// Member functions
public:
	CAdbShellStart() {}
	~CAdbShellStart() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_ADB_SHELL_START_H_
