/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CTouchTest object.
 * FileName   : TouchTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_TOUCH_TEST_H_
#define _C_TOUCH_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CTouchTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CTouchTest)

// Data members
public:
protected:
private:
	int TimeOut;


// Member functions
public:
	CTouchTest() {}
	~CTouchTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool TouchTest();
};


#endif // End of #ifndef _C_TOUCH_TEST_H_