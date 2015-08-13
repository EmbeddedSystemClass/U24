#ifndef _C_CHECK_TOUCH_H_
#define _C_CHECK_TOUCH_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CCheckTouch : public ITestItem
{
RF_DECLARE_DYNCREATE(CCheckTouch)

// Data members
public:
protected:
private:
	int m_iRetry;

// Member functions
public:
	CCheckTouch() {}
	~CCheckTouch() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_CHECK_TOUCH_H_
