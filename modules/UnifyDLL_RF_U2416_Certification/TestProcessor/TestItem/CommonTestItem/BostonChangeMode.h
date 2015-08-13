#ifndef _C_BOSTON_CHANGE_MODE_H_
#define _C_BOSTON_CHANGE_MODE_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CBostonChangeMode : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CBostonChangeMode)

// Data members
public:
protected:
private:
	int m_iRequiredMode;

	std::string m_strErrCode,m_strMsg;


// Member functions
public:
	CBostonChangeMode() {}
	~CBostonChangeMode() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_CHECK_AND_CHANGE_MODE_H_
