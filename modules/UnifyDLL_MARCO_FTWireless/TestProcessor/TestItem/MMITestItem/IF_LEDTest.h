/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CLoopback object.
 * FileName   : CapkeyLEDTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 ********************************************************************************/

#ifndef _C_IF_LEDTest_H_
#define _C_IF_LEDTest_H_

#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CIF_LEDTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CIF_LEDTest)

// Data members
public:
protected:
private:
	std::string m_str_OnOff;
	std::string m_str_Func;
	std::string m_str_ErrorCode;

// Member functions
public:
	CIF_LEDTest() {}
	~CIF_LEDTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool RGBLED_Test();

};


#endif // End of #ifndef _C_LOOPBACK_TEST_H_
