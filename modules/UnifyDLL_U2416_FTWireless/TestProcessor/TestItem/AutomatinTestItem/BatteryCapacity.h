/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CBatteryCapacity object.
 * FileName   : BatteryCapacityDetect.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Lion.Wu            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_Battery_Capacity_TEST_H_
#define _C_Battery_Capacity_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CBatteryCapacity : public ITestItem
{
RF_DECLARE_DYNCREATE(CBatteryCapacity)

// Data members
public:
protected:
private:

	std::string m_str_BatteryName;
	std::string m_str_BatteryLimit;
// Member functions
public:
	CBatteryCapacity() {}
	~CBatteryCapacity() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool BatteryCapacityDetectEmpty();
	bool BatteryCapacityDetectFull();
	bool showWarningMsgFull();
	bool showWarningMsgEmpty();
	

	
};


#endif // End of #ifndef _C_GYRO_SELF_TEST_H_