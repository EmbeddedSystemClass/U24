/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CLoopback object.
 * FileName   : LoopbackTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_LOOPBACK_TEST_H_
#define _C_LOOPBACK_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CLoopback : public ITestItem
{
RF_DECLARE_DYNCREATE(CLoopback)

// Data members
public:
protected:
private:
	std::string m_str_OnOff;
	std::string m_str_DeviceFrom;
	std::string m_str_VolumeValFrom;
	std::string m_str_DeviceTo;
	std::string m_str_VolumeValTo;
	std::string m_str_ErrorCode;
	std::string m_str_CLoopback_Sleep; 
	int n_CLoopback_Sleep;
// Member functions
public:
	CLoopback() {}
	~CLoopback() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool LoopbackTest();
};


#endif // End of #ifndef _C_LOOPBACK_TEST_H_
