#ifndef _C_CHANGE_BOOT_MODE_H_
#define _C_CHANGE_BOOT_MODE_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CEnableFSG : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CEnableFSG)

// Data members
public:
protected:
private:
	int m_iRequiredMode;

	std::string m_strErrCode,m_strMsg;
	std::string m_strEnable;

// Member functions
public:
	CEnableFSG() {}
	~CEnableFSG() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool runEnableFSG();
};


#endif // End of #ifndef _C_CHANGE_BOOT_MODE_H_
