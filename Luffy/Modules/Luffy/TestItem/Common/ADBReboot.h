#ifndef _C_CADBReboot_
#define _C_CADBReboot_

#include "BaseObject/ITestItem.h"
#include "Phone/AndroidPhone.h"
#include "Processor/GPProcessor.h"

#include "ErrorDefine/Common_ErrorDefine.h"
#include "Utils/StringUtil.h"

class CADBReboot : public ITestItem
{
GP_DECLARE_DYNCREATE(CADBReboot)

// Data members
public:
protected:
private:
	int m_iWaitTimeForBoot;

	std::string m_strErrCode;
	std::string m_strMsg;

	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

// Member functions
public:
	CADBReboot() {}
	~CADBReboot() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_PHONE_STATE_ITEM_H_
