/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CDisplay object.
 * FileName   : IDTestProc.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_ID_Test_Proc_H_
#define _C_ID_Test_Proc_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

/* ID Type */
#define LSENSOR				"LSensor"
#define PSENSOR				"PSensor"
#define FRONT_CAMERA		"FrontCamera"
#define REAR_CAMERA			"RearCamera"
#define BOSTON_REAR_CAMERA	"BostonRearCamera"
#define GSENSOR				"GSensor"
#define CPUID				"CPUID"
#define E_COMPASS			"ECompass"
#define GYRO				"Gyro"
#define CAPKEY				"CapKey"
#define GAUGE				"Gauge"
#define CHARGER				"Charger"
#define BATTERY				"Battery"
#define TOUCH				"Touch"
#define DFIVERSION			"DFIVersion"
#define TOUCHMODEL			"TouchModel"
#define SCI2CID				"SmartCoverI2CId"
#define AntennaICID			"AntennaICID"


class CIDTestProc : public ITestItem
{
RF_DECLARE_DYNCREATE(CIDTestProc)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_IDType;
	std::string m_str_ID;
	
	//Only for CPU Seconde ID
	std::string m_str_SecID;

	//Only for smart cover I2C ID
	std::string m_str_SCI2CId;


// Member functions
public:
	CIDTestProc() {}
	~CIDTestProc() {}

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
	bool CheckAntennaICID();
	//ytt
	bool CheckUSBTest();

};


#endif // End of #ifndef _C_GSensor_Test_Proc_H_
