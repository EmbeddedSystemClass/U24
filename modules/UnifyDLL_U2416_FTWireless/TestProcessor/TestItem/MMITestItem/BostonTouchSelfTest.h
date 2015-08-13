/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CBostonTouchSelfTest object.
 * FileName   : TouchSelfTest.h
 * Version    : $Revision:   1.0  $
 * Author     :Lion.Wu
 * Time       : 2012/12/18
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/12/18   Lion.Wu            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_TOUCH_SELF_TEST_H_
#define _C_TOUCH_SELF_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CBostonTouchSelfTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CBostonTouchSelfTest)

// Data members
public:
protected:
private:


// Member functions
public:
	CBostonTouchSelfTest() {}
	~CBostonTouchSelfTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool TouchSelfTest();
};


#endif // End of #ifndef _C_TOUCH_SELF_TEST_H_