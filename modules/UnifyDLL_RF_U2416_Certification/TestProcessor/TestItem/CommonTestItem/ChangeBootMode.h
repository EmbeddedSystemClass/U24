#ifndef _C_CHANGE_BOOT_MODE_H_
#define _C_CHANGE_BOOT_MODE_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CChangeBootMode : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CChangeBootMode)

// Data members
public:
protected:
private:
	int m_iRequiredMode;

	std::string m_strErrCode,m_strMsg;


// Member functions
public:
	CChangeBootMode() {}
	~CChangeBootMode() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_CHANGE_BOOT_MODE_H_
