#ifndef _C_SLEEP_DELAY_H_
#define _C_SLEEP_DELAY_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CSleepDelay : public ITestItem
{
	RF_DECLARE_DYNCREATE(CSleepDelay)


	// Data members
public:
protected:
private:
	int m_iWaitTime;
	std::string m_strPICSName;


	// Member functions
public:
	CSleepDelay() {}
	~CSleepDelay() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
};


#endif // End of #ifndef _C_SLEEP_DELAY_H_
