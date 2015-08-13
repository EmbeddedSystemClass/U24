#ifndef _C_CHECK_NVS_H_
#define _C_CHECK_NVS_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CCheckNVs : public ITestItem
{
RF_DECLARE_DYNCREATE(CCheckNVs)

// Data members
public:
protected:
private:
	int m_iRetry;
	int m_iNVs[3];

// Member functions
public:
	CCheckNVs() {}
	~CCheckNVs() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();

};


#endif // End of #ifndef _C_CHECK_NVS_H_
