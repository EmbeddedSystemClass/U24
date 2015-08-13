#ifndef _C_PHONE_STATE_ITEM_H_
#define _C_PHONE_STATE_ITEM_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CPhoneStateItem : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CPhoneStateItem)

// Data members
public:
protected:
private:
	int m_iWaitTimeForBoot;
	int m_iRetryConnectTimes;
	int m_iWaitTimeAfterConnect;

	std::string m_strMsg;


// Member functions
public:
	CPhoneStateItem() {}
	~CPhoneStateItem() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_PHONE_STATE_ITEM_H_
