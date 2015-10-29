/***************************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name	: DLL  
* File name     : UnifyDLL_Download.h             
* Summary       : Unify DL dll.                                                  
* Version		Author             Date                Abstract                      
* 1.0           Alex.Chen          2011/07/04          First version  
****************************************************************************************/

#pragma once

#include <map>
#include "Shlwapi.h"

#include "../../Modules/UnifyInterface/FacTestToolInterface.h"
#include "../../Modules/QisdaEmmcDownloadModule/QisdaEmmcDownloadModule.h"
#include "IniAccess.h"
#include "GLog.h"
#include "Project/U2416.h"
#include "global.h"
#include "io.h"
#include "Monitor/MonitorId.h"
//#include "Monitor/CommonDefine.h"
#include <direct.h>

#define ID_SIZE								11
#define i_Station_Count					10
#define IICDLL								_T("IIC.dll")
#define F32SERVERDB					_T("f32server2.dll")
#define VMS_DBACCESS				_T("VMS_DBACCESS.dll")
#define ID_SIZE								11
#define ID_SIZE_BUFFER					20

#define IDL_EXPORT_EXPORTS

#ifdef IDL_EXPORT_EXPORTS
#define IDL_API extern "C" __declspec(dllexport)
#define IDL_CLASS_API __declspec(dllexport)
#else
#define IDL_API extern "C" __declspec(dllimport)
#define IDL_CLASS_API __declspec(dllimport)
#endif

/*================================= Const ==================================*/
#define   MAX_SLOT			     8
#define   FA_SIZE			     512
#define   ARRAY_SIZE             128
#define   LOG_PATH               _T("LOG\\LOG_DL")
#define   UI_LOG_PATH            _T("UI_LOG_PATH")
#define   DLMODE                 _T("DLMODE")
#define   TOOL_MODE              _T("TOOL_MODE")
#define   MODEL_NAME             _T("MODEL_NAME")
#define   CUSTOMER_SW_VER        _T("Customer_SW_ver")

#define   PARAMETER_ONE          _T("IMAGEPATH")
#define   PARAMETER_TWO          _T("COM")
#define   PARAMETER_THREE        _T("PICASSO")
#define   PARAMETER_FOUR         _T("SW_VERSION")
#define   PARAMETER_FIVE         _T("HW_VERSION")
#define   PARAMETER_SIX          _T("DOWNLOAD_MODE")
#define   PARAMETER_SEVEN        _T("TOOL_VERSION")
#define   PARAMETER_EIGHT        _T("FACTORY_VERSION")
#define   PARAMETER_NINE         _T("SUPPORTED_VIDPID")
/*================================= Const ==================================*/

/*================================= Structure ==============================*/
#pragma pack (1)
typedef struct
{
	int  i_COMPort;                     // COM Port
	char szEMMCDeviceName[MAX_PATH];    // eMMC 
	std::string str_errorCode;          // ErrorCode
	CString str_multiDLFlag;			// multi-download flag
}DeviceStruct;
#pragma pack()
/*================================= Structure ==============================*/

using namespace QSYNC_LIB;
class CDLInstance : public IFacTestToolInterface, public CNotifyBase_Char, public INotify_ANSI
{
public:
	CDLInstance();
	virtual ~CDLInstance();
	CMonitorId Id;
public:
	/* CNotifyBase_Char */
	virtual bool Register(INotify_Char* pNotify, const char* strEvent);
	virtual bool Register(EventFunc_Char* pEventFunc, const char* strEvent);
	virtual bool Unregister(INotify_Char* pNotify, const char* strEvent);
	virtual bool Unregister(EventFunc_Char* pEventFunc, const char* strEvent);
	virtual void DefineNotify(const char* strNewEvent);
	virtual void Fire(const char* strEvent, long nParam);

	/* INotify_ANSI */
	virtual void Event (const std::string& strEvent, long nParam);

public:
	/* IFacTestToolInterface */
	virtual bool Begin(int i_slot = 1);
	virtual bool PreRun(int i_slot = 1);
	virtual bool Run(int i_slot = 1);
	virtual bool PostRun(int i_slot = 1);
	virtual bool Stop(int i_slot = 1);
	virtual bool End(int i_slot = 1);
	virtual bool SetParameterValue(char* sz_keyword, char* sz_value);
	virtual bool GetParameterValue(char* sz_keyword, char* sz_value, int i_size);
	virtual bool SetFAData(int i_slot, char* sz_value);
	virtual bool GetFAData(int i_slot, char* sz_value, int i_size);
	virtual bool GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	virtual bool SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	virtual bool SetTag(int i_slot, char *sz_sectorData, int i_sectorSize);
	virtual int  GetItemCount(void);
	virtual bool GetItemByIndex(int i_index, char* sz_name, int i_size);
	virtual bool ResopnseToJig(bool b_wait);
	virtual bool ResopnseToShowDlg(char* sz_showReslut);
	CString GetProjectName(){ return m_str_projectName;}
private:
	std::string st_Test_Flow[i_Station_Count];
	bool ReadDLLIniFile(void);
	bool GetDLLIniFile(void);
	//bool GetSupportQDownload(void);
	bool GetCOMPort(void);
	bool GetMultiDLFlag(void);
	bool GetDLMode(void);
	bool Get_PID_VID(void);
	bool SetImagePath(void);
	bool ReadSetupIniFile(void);
	bool GetToolVersion(void);
	bool GetCheckFlowInsertData(void);
	bool GetSWVersion();
	bool GetPartNo();
	//bool runReadScalarID( char *szvalue, int iSize );
	
	void AddMsg(const char* szMsg, int nStep, int nProgress);
	void SetErrorCode(const char* sz_errorCode, const int i_slot = 1);
	void SetErrorMessage(const char* sz_errorMessage, const int i_slot = 1);
	bool IsConnected(const int i_slot);
	bool IsCOMPortExist(const int i_slot);

	/* MD5 verify */
	bool CheckMD5(void);
	bool ConvertXML(CString str_XMLFilePathName, CString str_newName);
	bool MD5Verify(CString str_dirPath, CString str_XMLFilePathName, CString str_logFile);
	bool CheckVerifyResult(CString str_logFile);

	/* google log */
	void SetLogFileName(void);
	void SetLogFilePath();
	void SetInfoLogFileName(CString str_fileName);
	void WriteLog(CString str_log, const int i_severity = __INFO);
	bool SpiltString(CString str_sourceString, CString str_delimiter, CStringArray& stra_strArg);

private:
	CString                      m_str_projectName;         // Project name
	CString                      m_str_imageFilePath;       // Image file path
	CString                      m_str_iniFileName;         // Dll ini file name
	CString                      m_str_comPortList;         // All COM port, For example: 10,20,30,40,50,60,70,80
	CString						 m_str_multiDLFlagList;		// All COM port, For example: 10,20,30,40,50,60,70,80
	bool                         m_b_readIniFile;           // Read dll ini file result
	bool                         m_b_SupportQDownload;      // Support QDownload Tool
	bool                         m_b_checkFlow;     
	bool                         m_b_insertData;  
	bool                         m_b_checkSWVersion;     
	CString						 str_prestation;
	CString						 str_SWVersion;
	int                          m_i_imageCount;            // Image file count
	char                         m_sz_DLMode[ARRAY_SIZE];   // Current DL mode
	char                         m_sz_PID_VID[ARRAY_SIZE];   // Current DL mode
	std::vector<st_Image>        m_vector_image;            // Image
	std::map<int, DeviceStruct>	 m_map_deviceStruct;        // Slot <-> COM port
	std::map<int, DeviceStruct>	 m_map_deviceStructMultiDLFlag;  // Slot <-> Multi-download flag
	std::map<int, CString>	     m_map_picassoList;         // Slot <-> Picasso
	CString                      m_str_SWVersion;           // SW version
	CString                      m_str_HWVersion;           // HW version
	CString                      m_str_ToolVersion;         // Tool version
	std::map<CString, CString>	 m_map_parameterList;      // keyword <-> value;
	CGLog                        obj_gLog;                  // Google log
	CString                      m_str_logFileName;         // DLL set log file name by oneself
	std::map<int, CString>	     m_map_COMPicasso;          // COM -> Picasso
	CString                      m_str_fatoryVersion;
	CString                      m_str_UI_LogPath;
	CString                      m_str_toolMode;
	CString                      m_str_modelName;
	CString                      m_str_DLMode;
	CString                      m_str_CustomerSwVersion;
	CString						 csStation;

	int m_nProgress; // Keep Original Progress data
	std::string m_strMsg; // Keep Original Message

	std::string std_key_path;
	std::string std_Key_Id;
	std::string std_Key_Name;
	std::string std_ScalarId ;
	std::string szPcabaId;
	std::string checkStation;
	std::string sz_Hdcp_key_path;

	bool runInsertData();
	bool runCheckFlow( int i_type);// i_type 1 = pcbaid,  2 = scalarId
	bool runCheckFlowAllStation( int i_type);// i_type 1 = pcbaid,  2 = scalarId
	bool runReadScalarID( char *szvalue, int iSize );

	bool getWeek(void);
	int GetWeek	(struct tm* date	);
	int GetCurrentWeek();
	std::string ErrMsg;
	//std::string g_strScalarID;
	//std::string g_str_modelName;
	std::string m_szSWver;
	std::string m_szPartNo;

};



IDL_API bool NewInterface(IFacTestToolInterface**);
IDL_API bool DeleteInterface(IFacTestToolInterface*);
