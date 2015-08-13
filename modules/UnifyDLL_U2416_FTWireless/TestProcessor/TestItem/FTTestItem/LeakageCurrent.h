

#ifndef _C_Charge_Average_Current_H_
#define _C_Charge_Average_Current_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

#define dLeakageCurrent  _T("LeakageCurrent")


class CLeakageCurrent : public ITestItem
{
RF_DECLARE_DYNCREATE(CLeakageCurrent)


// Data members

private:

	std::string m_str_Name;
	std::string m_str_Value;

	CRangePair m_rp_Value;

	bool m_b_CheckMatch;
	int m_i_sleep;
	int m_iWaitTimeForShutdown;
	int m_iWaitTimeForFetch;

// Member functions
public:

	CLeakageCurrent() {}
	~CLeakageCurrent() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool CheckLeakageCurrent();
};


#endif // End of #ifndef _C_Charge_Average_Current_H_
