#ifndef _C_READ_IMEI_H_
#define _C_READ_IMEI_H_

#include "BaseObject/ITestItem.h"
#include "Utils/StringUtil.h"


class CReadIMEI : public ITestItem
{
GP_DECLARE_DYNCREATE(CReadIMEI)


// Data members
public:
	bool m_bTestResult;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

private:
	std::string m_strErrCode;
	std::string m_strMsg;
	std::string m_strIMEI;

// Member functions
public:
	CReadIMEI();
	~CReadIMEI() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(int i_slot = 0);

private:
	bool MainFunction();
};


#endif // End of #ifndef _C_READ_IMEI_H_
