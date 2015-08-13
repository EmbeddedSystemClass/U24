#ifndef _C_CHECK_SW_VERSION_H_
#define _C_CHECK_SW_VERSION_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CCheckSWVersion : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CCheckSWVersion)

// Data members
public:
protected:
private:
	std::string m_strSWversion;
	std::string m_strSWVerList;
	int m_nType;
	std::string m_strMsg;


// Member functions
public:
	CCheckSWVersion() {}
	~CCheckSWVersion() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_CHECK_SW_VERSION_H_
