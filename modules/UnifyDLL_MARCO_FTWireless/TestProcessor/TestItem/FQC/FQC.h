/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 * Author     : Lion.Wu
 ********************************************************************************/

#ifndef _C_Read_Battery_ID_Proc_H_
#define _C_Read_Battery_ID_Proc_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
/* ID Type */
#define BATTERY						"Battery"
#define FineWifiAP					"FineWifiAP"
#define USBCharging					"USBCharging"
#define OLEDTest					"OLEDTest"
#define LEDTest						"LEDTest"
#define BatteryTest					"BatteryTest"
#define CameraTest					"CameraTest"
#define SensorBoardEEPROM			"SensorBoardEEPROM"
#define SensorBoardFPGA_ID				"SensorBoardFPGA_ID"
#define ReadBatteryVoltage			    "ReadBatteryVoltage"
#define ReadBatteryCurrent			    "ReadBatteryCurrent"



#define ADB_CMD_TIMEOUT			25000

class CFQC : public ITestItem
{
RF_DECLARE_DYNCREATE(CFQC)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_TestItem;
	std::string m_str_Delay;
	std::string m_str_CMD;
	std::string m_str_OffCMD;
	std::string str_msg;

	std::string str_Pics ;

	int n_Delay;
	int nBatteryID_target;
	int nBatteryID_lower;
	int nBatteryID_upper;	

	int m_i_SleepTime;
	//int m_i_OffSleepTime;
	int m_i_retryTime;
	int m_i_retriedTime;
	int m_i_MexRetryTime;

	CString m_imgPath;

	bool b_res_on, b_res_off ;
	bool b_res_msg_try, b_res_msg_retry;

// Member functions
public:
	CFQC() {}
	~CFQC() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:

	bool runFindWifiAP();
	bool runUSBCharging();
	bool runOLEDTest();
	bool runLEDTest();
	bool runBatteryTest();
	bool runCameraTest();
	bool runSensorBoardEEPROMTest();
	bool runSensorBoardFpgaID();
	bool runReadBattery_Voltage();
	bool runReadBattery_Current();
	
	bool showMsg(const char* szMsg);
	bool ExecAdbOut(CString Command, char* output, char* ErrorCode);

	CString GetDirectory_Img();
	bool GetDirectory_D_LOG_IMG();
	bool DeleteOldPicture();
	//bool Detroit_CapturePic(int i_shot_number, int frontRear) ;// frontRear=0-> front pic, =1-> rear pic
	//bool Detroit_StartCam(int frontRear);  //normal
	//bool Detroit_StopCam(int frontRear);  //normal
	//bool CFQC::GetDirectory_D_LOG_IMG()

};


#endif // End of #ifndef _C_GSensor_Test_Proc_H_
