/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CCapkeyLEDTest object.
 * FileName   : CapkeyLEDTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2012/1210
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1     2012/1210    Lion.Wu            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_CapkeyLEDTest_H_
#define _C_CapkeyLEDTest_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CCapkeyLEDTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CCapkeyLEDTest)

// Data members
public:
protected:
private:
	std::string m_str_OnOff;
	std::string m_str_ErrorCode;

// Member functions
public:
	CCapkeyLEDTest() {}
	~CCapkeyLEDTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool CapkeyLEDTest();
	bool CapkeyLEDTestTogether();
	bool RedPattern_N_CapkeyLED_Test();
};


#endif // End of #ifndef _C_LOOPBACK_TEST_H_
