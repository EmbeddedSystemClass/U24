/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CLoopback object.
 * FileName   : CameraLEDTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2012/1210
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1     2012/1210    Lion.Wu            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_SmartCoverTest_H_
#define _C_SmartCoverTest_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CSapporoSmartCover : public ITestItem
{
RF_DECLARE_DYNCREATE(CSapporoSmartCover)

// Data members
public:
protected:
private:
	std::string m_str_SmartCoverTestItem;
	std::string m_str_SCoverCheckI2C_ID;
	std::string m_str_SCover_CMD;
	std::string m_str_SCover_Sleep;
	std::string m_str_ErrorCode;
	std::string m_str_Pics;
	int n_SCover_Sleep;

// Member functions
public:
	CSapporoSmartCover() {}
	~CSapporoSmartCover() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:

	bool SCoverDetect(char* szFlag);
	bool SCoverInterrupt(char* szFlag);
	bool SCoverEnable3V();
	bool SCoverCheckI2C();
	bool SCoverSwitch5V();
};


#endif // End of #ifndef _C_LOOPBACK_TEST_H_
