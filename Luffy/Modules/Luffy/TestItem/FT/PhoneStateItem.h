#ifndef _C_PHONE_STATE_ITEM_H_
#define _C_PHONE_STATE_ITEM_H_


#include "BaseObject/ITestItem.h"
#include "Phone/AndroidPhone.h"
#include "Processor/GPProcessor.h"

#include "ErrorDefine/Common_ErrorDefine.h"
#include "Utils/StringUtil.h"


class CPhoneStateItem : public ITestItem
{
GP_DECLARE_DYNCREATE(CPhoneStateItem)

// Data members
public:
protected:
private:
	int m_iWaitTimeForBoot;
	int m_iRetryConnectTimes;
	int m_iWaitTimeAfterCommand;
	int m_iWaitTimeAfterConnect;

	bool m_bEnableFTD;

	std::string m_strMsg;

	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

// Member functions
public:
	CPhoneStateItem() {}
	~CPhoneStateItem() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_PHONE_STATE_ITEM_H_
