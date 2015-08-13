#ifndef _C_CHECK_AND_CHANGE_MODE_H_
#define _C_CHECK_AND_CHANGE_MODE_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CCheckAndChangeMode : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CCheckAndChangeMode)

// Data members
public:
protected:
private:
	int m_iRequiredMode;

	std::string m_strErrCode,m_strMsg;


// Member functions
public:
	CCheckAndChangeMode() {}
	~CCheckAndChangeMode() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_CHECK_AND_CHANGE_MODE_H_
