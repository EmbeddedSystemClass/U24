#ifndef _C_WLAN_TEST_H_
#define _C_WLAN_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


#define WLAN_Self_Test	_T("WLAN_Self_Test")

class CWLanTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CWLanTest)

// Data members
public:
protected:
private:

	std::string m_str_Name;
	std::string m_str_Value;
	bool m_b_CheckMatch;
	int m_i_sleep;


// Member functions
public:
	CWLanTest() {}
	~CWLanTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:

	bool WLANSelfTestTest();
};


#endif // End of #ifndef _C_TOUCH_TEST_H_