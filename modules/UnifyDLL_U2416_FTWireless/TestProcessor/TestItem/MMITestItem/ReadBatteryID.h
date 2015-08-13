/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 * Author     : Lion.Wu
 ********************************************************************************/

#ifndef _C_Read_Battery_ID_Proc_H_
#define _C_Read_Battery_ID_Proc_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
/* ID Type */
#define BATTERY				"Battery"

class CreadBatteryID : public ITestItem
{
RF_DECLARE_DYNCREATE(CreadBatteryID)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_IDType;
	std::string m_str_ID;
	
	std::string std_BatteryID;
	int nBatteryID_target;
	int nBatteryID_lower;
	int nBatteryID_upper;		

// Member functions
public:
	CreadBatteryID() {}
	~CreadBatteryID() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool GaugeCheckID();
	bool ChargerCheckID();
	bool BatteryCheckID();
	bool CPUCheckID();
	bool GSensorCheckID();
	bool ECompassCheckID();
	bool LSensorCheckID();
	bool PSensorCheckID();
	bool FrontCameraCheckID();
	bool RearCameraCheckID();
	bool BostonRearCameraCheckID();
	bool GyroCheckID();
	bool CapKeyCheckID();
	bool TouchCheckID();
	bool TouchCheckModel();
	bool DFIcheckVersion();
	bool SmartCoverCheckI2CID();
};


#endif // End of #ifndef _C_GSensor_Test_Proc_H_
