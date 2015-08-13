#ifndef _C_AT_COMMAND_H_
#define _C_AT_COMMAND_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CShieldBoxControl : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CShieldBoxControl)


// Data members
public:
protected:
private:
	std::string m_strDeviceName;
	std::string m_strCommand;
	std::string m_strMsg;
	int m_iBaudRate;


// Member functions
public:
	CShieldBoxControl() {}
	~CShieldBoxControl() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_AT_COMMAND_H_
