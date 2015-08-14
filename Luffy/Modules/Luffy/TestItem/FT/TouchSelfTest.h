#ifndef _C_TOUCH_SELFTEST_CHARGING_H_
#define _C_TOUCH_SELFTEST_CHARGING_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Function_ErrorDefine.h"


class CTouchSelfTest : public ITestItem
{
GP_DECLARE_DYNCREATE(CTouchSelfTest)

		// Data members
public:
protected:
private:
	std::string		m_strPICSName;

	short           m_nCmdCode;
	std::string     m_strErrCode;
	std::string     m_strMsg;

	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

	// Member functions
public:
	CTouchSelfTest() {}
	~CTouchSelfTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();
	
};


#endif // End of #ifndef _C_TOUCH_SELFTEST_CHARGING_H_
