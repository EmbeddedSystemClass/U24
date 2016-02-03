/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CEcompassSelfTest object.
 * FileName   : TouchSelfTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12     Lion.Wu           Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_Ecompass_SELF_TEST_H_
#define _C_Ecompass_SELF_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CEcompassSelfTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CEcompassSelfTest)

// Data members
public:
protected:
private:


// Member functions
public:
	CEcompassSelfTest() {}
	~CEcompassSelfTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool EcompassSelfTest();
};


#endif // End of #ifndef _C_Ecompass_SELF_TEST_H_