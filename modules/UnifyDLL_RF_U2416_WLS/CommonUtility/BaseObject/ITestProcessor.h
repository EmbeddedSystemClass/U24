#ifndef _I_TOOL_PROCESSOR_H_
#define _I_TOOL_PROCESSOR_H_


#include "CToolBaseObject.h"
#include "../../../UnifyInterface/NotifyBase.h"
#include "../XML/MyMSXML.h"
#include "IPhone.h"
#include "ITesterDevice.h"
#include "IPowerSupply.h"
#include "../CommonDefine.h"
#include "../../../Common/ErrorDefine/Function_ErrorDefine.h"
#include "../../../Common/ErrorDefine/RF_ErrorDefine.h"
#include "../../../Common/ErrorDefine/Common_ErrorDefine.h"
#include "../../../Common/ErrorDefine/ErrorDefine.h"
#include "../../../Super4Relay/Super4Relay.h"
#include "../CPKLog/CPKLogItem.h"
#include "../CommonUtil.h"


using namespace QSYNC_LIB;


interface ITestItem;

#define COMMONPATH     "C:\\PE_config"


interface ITestProcessor : public CToolBaseObject, public CNotifyBase_ANSI
{
RF_DECLARE_DYNAMIC(ITestProcessor)


// Data member
public:
	bool m_bUsingJig;
	std::string m_strDeviceName;
	int m_iBaudRate;

	static std::string g_strErrMsg;

	static std::map<std::string, std::string> g_mapToolInfo;
protected:
	static std::string g_str_modelName;
	static std::string g_str_station;
	static int m_nFtdPort;
	static int m_nFtdTimeOut;

	std::string     m_strBand ;
	std::string     m_strChannel ;

	static IPowerSupply* m_pIPS;
	static IPowerSupply* m_pIPS2;
	static IPowerSupply* m_pIPS3;
	static ITesterDevice* m_pITesterDevice;
	static ITesterDevice* m_pITesterDeviceFSP;
	static IPhone* m_pIPhone;

	static std::map<std::string, std::map<std::string, std::string>> g_mapTxCableLossData;
	static std::map<std::string, std::map<std::string, std::string>> g_mapRxCableLossData;

	static std::map<std::string, std::map<std::string, std::string>> g_mapTxRefResultData;
	static std::map<std::string, std::map<std::string, std::string>> g_mapRxRefResultData;

	//static std::map<std::string, std::string> g_mapCableLossMatix;

	static clock_t g_totalTestDuration_Start;
	static clock_t g_testItemDuration_Start;

	static std::string g_strErrCode;
	//static std::string g_strErrMsg;
	std::string g_strFactoryVersion, g_strSWVersion;

	static CString m_cstrCableLossPath;

	CString m_cstrCableLossFile;
	static CString m_cstrTestItemFile;
	CMyMSXML m_CableLossXML, m_TestItemXML;
	CString m_cstrPhone;
	bool m_bForceToPass;
	bool m_bUsingGPIB;
	int m_iGPIBInterfaceID;
	int m_iGPIBCardAddress;
	int m_iPSCount;
	std::vector<int> m_vPowerSupplyAddr;
	std::map<std::string, std::string> m_mapTesterDeviceParams, m_mapPowerSupplyParams, m_mapPhoneParams;
	std::vector<std::string> m_vTestItemList;
	std::vector<std::string> m_vTestItemTypeList;
	std::vector<ITestProcessor*> m_vpiTestItem;
	std::map<std::string, std::map<std::string, std::string>> m_mapTestItemParams;

	static std::string g_strPicasso;

	INotify_ANSI* m_piToolNotify;
	static CCPKLogItem gCpkRecord;

	CString m_cstrStationName, m_cstrModelName;
	CString m_cstrKCableLossFile, m_cstrGSRefResultFile;
	CMyMSXML m_KCableLossXML, m_GSRefResultXML;
	bool m_bCalibrationMode;

	static bool m_b_LogUpload;
	static std::string m_str_UploadServerIP;

// public:
// 	static CString m_strPicassoNum;
// 	static CString m_strSWVersion;
// 	static CString m_strToolMode;
// 	static CString m_strModeName;
// 	static CString m_stationName;
// 	static CString m_strToolVersion;


// Member functions
public:
	ITestProcessor();
	virtual ~ITestProcessor() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) = 0;

	// Set cable loss & test item XML
	void SetCableLossXML(CString cstrCableLossXML) { m_cstrCableLossFile = cstrCableLossXML; m_cstrCableLossPath = m_cstrCableLossFile;}
	void SetTestItemXML(CString cstrTestItemXML) { m_cstrTestItemFile = cstrTestItemXML; }
	void SetStationModelName(CString m_cstr_StationName, CString m_cstr_ModelName) {m_cstrStationName = m_cstr_StationName; m_cstrModelName = m_cstr_ModelName;}

	// Information functions
	bool LoadToolInfo();
	bool LoadTestItem();

	// Create phone object
	bool CreatePhoneObject();

	// Get parameter functions
	bool GetFactoryVersion(char* sz_value);
	bool GetSWVersion(char* sz_value);
	bool SetModelName(const char* p_sz_modelName);
	bool SetStation(const char* p_sz_station);

	///////////////////////////////////////////////////////////////////////
	//                          UnifyInterface
	///////////////////////////////////////////////////////////////////////

	/* Set/Get Parameter Value */
	virtual bool SetParameterValue(char* sz_keyword, char* sz_value);
	virtual bool GetParameterValue(char* sz_keyword, char* sz_value, int i_size);

	/* Set/Get FA Data */
	virtual bool SetFAData(int i_slot, char* sz_value) {return true;};
	virtual bool GetFAData(int i_slot, char* sz_value, int i_size) {return true;};

	/*Get/Set FA Sector Data*/
	virtual bool GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize) {return true;};
	virtual bool SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize) {return true;};

	/* Get test items */
	virtual int GetItemCount();
	virtual bool GetItemByIndex(int i_index, char* sz_name, int i_size);

	/* Response to Jig control  */
	virtual bool ResopnseToJig(bool b_wait);

	/* Response to show dialog  */
	virtual bool ResopnseToShowDlg(char* sz_showReslut);

	/* Execute Android Logcat */
	bool ExecAndroidLogcat(bool b_IsPass);

	/* upload log to server */
	bool ITestProcessor::LogUploadByBAT(char *str_modelname);
	bool ITestProcessor::LogUpload(char *str_modelname);

	/* Check upload file */
	bool DFSCheckFile(char *str_LocalPath, char *str_RemotePath, char *str_TracePath);
	
	/* Delete local file */
	bool DFSDeleteFile(char *str_LocalPath, char *str_TracePath);
	
	// Test sequence
	//virtual bool Initial(){return true;};
	//virtual bool IsConnected(){return true;};
	//virtual bool GetFA(char* szValue, int iSize) = 0;
	virtual bool Begin(){return true;};
	virtual bool Run() = 0;
	virtual bool Stop(){return true;};
	//virtual bool Disconnect(){return true;};
	virtual bool End(){return true;};
	virtual bool PreRun() {return true;};
	virtual bool PostRun() {return true;};

	// Log and UI control functions
	void TraceLog(int iSeverity, std::string strMsg);
	virtual void FactoryLog(bool bPass, std::string strMessage);
	virtual void FactoryLog(bool bPass,
		std::string strItemCode,
		std::string strErrCode,
		std::string strBand,
		std::string strChannel,
		std::string strLower,
		std::string strUpper,
		std::string strMeasured,
		std::string strUnit,
		std::string strMsg
		);
	virtual void CPKLog(int iSeverity, 
		std::string strTech,
		std::string strItemName,
		std::string strBand,
		std::string strChannel,
		std::string strIdentify1,
		std::string strIdentify2,
		std::string strIdentify3,
		std::string strValue,
		bool result
		);
	void SetUIPattern(std::string strPattern);
	void SetTestingItem(std::string strTestItemName);
	void SetPICSData(std::string strKey, std::string strValue);
	void JigControl(std::string strSwitch);
	void ShowMsgDlg(std::string strMsg);

	// Test duration functions
	void StartTotalDurationCounter();
	void StartTestItemDurationCounter();
	std::string GetTestDuration();
	std::string GetTotalTestDuration();

	// Notification
	virtual bool Register(INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Register(EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual bool Unregister(INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Unregister(EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual void DefineNotify(const std::string& strNewEvent);
	virtual void Fire(const std::string& strEvent, long nParam);

protected:
	// Setup functions
	bool CheckCalibrationMode();
	bool LoadRefResult();
	bool LoadCableLoss();
	bool LoadTestProcessorParams();
	bool LoadGPIBDeviceParams();
	bool DetectGPIBDevice();
	bool LoadPhoneParams();
	bool LoadTestItemParameters();

	// Test result function
	void SetTestResult(bool bPass);

	//move functions from itestitem to itestprocessor
	double GetRefResult(const std::string strRfType, const std::string strRfBand, std::string strRfRange);
	double GetCableLoss(const std::string strRfType, const std::string strRfBand, std::string strRfRange);
	bool UpdateCableLoss(const std::string strRfType, const std::string strRfBand, std::string strRfRange, std::string strNewLoss);
	bool InRange(int iLowerBound, int iUpperBound, int iMeasured);
	bool InRange(double dLowerBound, double dUpperBound, double dMeasured);

private:
};


#endif // End of #ifndef _I_TOOL_PROCESSOR_H_
