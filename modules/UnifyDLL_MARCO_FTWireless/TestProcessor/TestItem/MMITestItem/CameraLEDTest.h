/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CCameraLEDTest object.
 * FileName   : CameraLEDTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2012/1210
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1     2012/1210    Lion.Wu            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_CameraLEDTest_H_
#define _C_CameraLEDTest_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CCameraLEDTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CCameraLEDTest)

// Data members
public:
protected:
private:
	std::string m_str_OnOff;
	std::string m_str_ErrorCode;

// Member functions
public:
	CCameraLEDTest() {}
	~CCameraLEDTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool CameraLEDTest();
};


#endif // End of #ifndef _C_LOOPBACK_TEST_H_
