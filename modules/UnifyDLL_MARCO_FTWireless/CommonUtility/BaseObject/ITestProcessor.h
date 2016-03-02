#ifndef _I_TOOL_PROCESSOR_H_
#define _I_TOOL_PROCESSOR_H_


#include "CToolBaseObject.h"
#include "../XML/MyMSXML.h"
#include "IPhone.h"
#include "ITesterDevice.h"
#include "IPowerSupply.h"
#include "../../../UnifyInterface/FacTestToolInterface.h"
#include "../CommonDefine.h"
#include "../ErrorCode.h"
#include "../../../Common/ErrorDefine/ErrorDefine.h"
#include "IFunc_Dev.h"
#include "../CommonUtil.h"

using namespace QSYNC_LIB;


interface ITestItem;


interface ITestProcessor : public CToolBaseObject, public CNotifyBase_ANSI
{
RF_DECLARE_DYNAMIC(ITestProcessor)


// Data member
public:
protected:
	static std::map<std::string, std::map<std::string, std::string>> g_mapTxCableLossData;
	static std::map<std::string, std::map<std::string, std::string>> g_mapRxCableLossData;
	static std::string g_strPicasso;
	static std::string g_strTag;
	static std::string g_strSn;
	static std::string g_strLine;
	static std::string g_strDayNight;
	static std::string g_strScalarID;
	static std::string g_strIMEI;//LION 2012.4.25
	static std::string g_str_modelName;
	static std::string g_str_station;
	static std::string g_str_SWVersion;
	static std::string g_str_FactoryVersion;
	static std::string g_str_HWVersion;
	//Eason Check from Server S-----------------------------------------------
	static std::map<std::string, std::string> m_map_parameterListSeverCheck; 
	//Eason Check from Server E-----------------------------------------------

	static IPowerSupply* m_pIPS;
	static IPowerSupply* m_pIPS2;
	static IPowerSupply* m_pIPS3;
	static ITesterDevice* m_pITesterDevice;
	static IPhone* m_pIPhone;

	static std::string m_strResult;
	static std::string m_strItemCode;
	static std::string m_strErrorCode;
	static std::string m_strBand;
	static std::string m_strChannel;
	static std::string m_strUpper;
	static std::string m_strLower;
	static std::string m_strMeasured;
	static std::string m_strUnit;
	static std::string m_strMessage;

	CString m_cstrCableLossFile, m_cstrDeviceItemFile, m_cstrTestItemFile;
	CMyMSXML m_CableLossXML, m_DeviceItemXML, m_TestItemXML;
	CString m_cstrPhone;
	int m_iGPIBInterfaceID;
	int m_iGPIBCardAddress;//GPIB Card Address for Tester Device
	int m_iPSCount;
	bool m_b_PS1_OnOff;
	bool m_b_PS2_OnOff;
	double m_d_PSVoltage;
	double m_d_PSVoltage2;
	double m_d_PSVoltage3;
	double m_d_PSCurrent;
	double m_d_PSCurrent2;
	double m_d_PSCurrent3;
	std::vector<int> m_vPowerSupplyAddr;
	std::map<std::string, std::string> m_mapTesterDeviceParams, m_mapPowerSupplyParams, m_mapPhoneParams;
	std::vector<std::string> m_vTestItemList;
	std::vector<ITestItem*> m_vpiTestItem;
	std::map<std::string, std::map<std::string, std::string>> m_mapTestItemParams;

	INotify_ANSI* m_piToolNotify;

	/* CPhone parameters */
	static int m_nCOMPort;
	static int m_nFtdPort;
	static int m_nFtdTimeOut;
	static bool m_b_AutoFTDMode;
	static bool m_b_SetFTDMode;
	static int m_i_WaitTimeStartingUp;
	static int m_i_RebootDelayTime;
	static int m_i_diagCmdFlag;
	static bool m_b_IsNeedQXDMLog;
	static bool m_b_AdbShellStart;
	static int m_i_QPhoneMultiCtrl;
	static int m_nQPhoneOrderSlot[5];
	static bool m_b_LogUpload;
	static std::string m_str_UploadServerIP;

	/* RelayBoard Parameters */
	bool m_b_IsUsed;
	int m_i_board;
	int m_i_slot[4];

	/* USB4702 Parameters */
	static bool m_b_IsUsed4702;

	/* Show dialog action result */
	static bool m_b_DialogResponse;

	static CString m_csArrPicasso[5];
	static bool m_b_UsableSlot[5];
	static bool m_b_isSlotPass[5];

	//Eason
	bool m_b_IsUsePowerOnCheck;
	int  m_i_PowerCheckOnTime;
	int  m_i_PS1Check;
	int  m_i_PS2Check;
	std::string m_str_PS1Value;
	std::string m_str_PS2Value;


private:

// Member functions
public:
	ITestProcessor();
	virtual ~ITestProcessor(){}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) { return true; }

	// Set cable loss & test item XML
	void SetCableLossXML(CString& cstrCableLossXML) { m_cstrCableLossFile = cstrCableLossXML; }
	void SetDeviceItemXML(CString& cstrDeviceItemXML) { m_cstrDeviceItemFile = cstrDeviceItemXML; }
	void SetTestItemXML(CString& cstrTestItemXML) { m_cstrTestItemFile = cstrTestItemXML; }

	// Test items related functions
	bool LoadTestITems();
	int GetTestItemCount();
	bool GetTestItemByIndex(int nIndex, char* szName, int nSize);
	bool GetTestResult(int nIndex, st_UIResult* psResult);

	/* Test flow functions */
	/* Create GPIB device object function */
	bool GPIBDeviceCreateObject();

	/* Create phone object function */
	bool PhoneCreateObject();

	/* Initial PC port function */
	bool InitialPC();

	/* Initial Relay Board function */
//	bool InitialRelayBoard();

	/* Initial USB4702 function */  //120517 for detroit by cy.lee
//	bool InitialUSB4702();

	/* Close USB4702 function */  //120521 for detroit by cy.lee
//	bool CloseUSB4702();

	/* Initial PowerSupply function */
	bool InitialPowerSupply();

	/* Mobile auto FTD mode function */
	bool MobileAutoFTDMode();

	/* Mobile set FTD mode function */
	bool MobileSetFTDMode();

	/* Kill adb thread in Task Manager function */
	bool KillADBThread();

	/* Copy mobile QXDM log to PC function */
//	bool CopyQXDMMobileLogToPC();

	/* Execute Android Logcat */
	bool ExecAndroidLogcat(bool isPass);

	/* upload log to server */
	bool ITestProcessor::LogUploadByBAT(char *str_modelname);
	bool ITestProcessor::LogUpload(char *str_modelname);

	/* Check upload file */
	bool DFSCheckFile(char *str_LocalPath, char *str_RemotePath, char *str_TracePath);

	/* Delete local file */
	bool DFSDeleteFile(char *str_LocalPath, char *str_TracePath);

	/* Pull Device Logcat Out */
	bool PullDeviceLogOut(bool b_res);

	/* Show pass or fail on device LCD screen*/
	bool ShowPassFailOnDeviceLCD();

	/* Get/Set parameter functions */
	bool GetDeviceName(char* szName, int iSize);
	bool GetSWVersion(char* p_sz_SWVersion);
	bool GetSWVersionFromDB();
	bool GetFactoryVersion(char* p_sz_FactoryVersion);
	bool SetPicasso(const char* szPicasso);
	bool SetTag(const char* szTag);
	bool SetSn(const char* szSn);
	bool SetLine(const char* szLine);
	bool SetDayNight(const char* szDayNight);
	bool SetPicassoMulti(const char* szPicasso, const int nSlot);
	bool SetSlotUsable(const char* szUsable, const int nSlot);
	bool SetIMEI(const char* szIMEI);//Lion 2012.4.25
	bool SetModelName(const char* p_sz_modelName);
	bool SetStation(const char* p_sz_station);
	bool SetSWVersion(const char* p_sz_SWVersion);
	bool SetFactoryVersion(const char* p_sz_FactoryVersion);
	bool SetHWVersion(const char* p_sz_HWVersion);
	bool SetDialogResponse(bool b_Resopnse);

	//Eason Check from Server S-----------------------------------------------
	void RecordServerPara( std::string Station , std::string Value );
	std::string GetServerPara( std::string Station );
	//Eason Check from Server E-----------------------------------------------

	/* FA data operate functions */
	bool ReadFAData(char* pszFAdata, int i_size);
	bool WriteFAData(char* pszFAdata, size_t i_InSize, char* pszOutput);
	bool ReadFAData_New(int i_slot, int i_sectorNum, char* pszFAdata, int i_size);
	bool WriteFAData_New(int i_sectorNum, char* pszFAdata, size_t i_size, char* pszOutput);

	/* Test sequence functions */
	virtual bool Begin() = 0;
	virtual bool IsConnected() = 0;
	virtual bool IsConnectedMulti() = 0;
	virtual bool IsMobileInFTDMode() = 0;
	virtual bool IsSocketReady() = 0;
	virtual bool PreRun(int iIndex) = 0;
	virtual bool Run(int iIndex) = 0;
	virtual bool PostRun(int iIndex) = 0;
	virtual bool Disconnect() = 0;
	virtual bool End() = 0;

	// DLL Event functions
	/* Logging file */
	void TraceLog(int iSeverity, std::string strMsg);
	void TraceLog(int iSeverity, std::string strMsg, int nQPhoneNumber);

	/* Factory log */
	void FactoryLog();

	/* Set PICS data */
	void SetPICSData(std::string str_key, std::string str_value);
	void SetPICSData(std::string str_key, std::string str_value, int nQPhoneNumber);

	/* Show picture */
	void ShowPicture(bool b_append, const char *p_sz_path);

	/* Jig Request */
	void JigRequest(const char *p_sz_switch);
	void SetJigRequestInfo(bool b_requestRes)
	{
		m_b_waitFlag = true;
		m_b_jigRequest = b_requestRes;
	}
	static bool m_b_jigRequest;
	static bool m_b_waitFlag;

	/* Show Dialog */
	void ShowDialog(const char *p_sz_message);

	// Notification
	virtual bool Register(INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Register(EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual bool Unregister(INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Unregister(EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual void DefineNotify(const std::string& strNewEvent);
	virtual void Fire(const std::string& strEvent, long nParam);

protected:
	// Initial functions
	bool LoadCableLossXML();
	bool ParseGPIBDeviceParams();
	bool DetectGPIBDevice();
	bool ParsePhoneParams();
	//bool ParseRelayBoardParameters();
	//bool ParseUSB4702Parameters();
	bool ParseTestItemParameters();
	bool ParseLeakageCurrentPowerOnCheckParameters(); 


private:
public:
	int m_iProcessCode;
	int m_iItemCode;
	int m_iErrorCode;
};


#endif // End of #ifndef _I_TOOL_PROCESSOR_H_
