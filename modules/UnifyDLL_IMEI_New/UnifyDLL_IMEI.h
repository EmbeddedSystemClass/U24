/***************************************************************************************
* Copyright (C) : QTH/WR22                                      
* Module name	: UnifyDLL_IMEI Interface  
* File name     : UnifyDLL_IMEI.h             
* Summary       : Interface for IMEI station tools.                                                  
* Version		Author             Date                Abstract                      
* 0.1           CY.Lee             2015/02/11          First version  
****************************************************************************************/

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UNIFYDLL_IMEI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UNIFYDLL_IMEI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef UNIFYDLL_IMEI_EXPORTS
#define UNIFYDLL_IMEI_API extern "C" __declspec(dllexport)
#else
#define UNIFYDLL_IMEI_API __declspec(dllimport)
#endif

#include "../UnifyInterface/Toolinterface_Notify.h"
#include "../UnifyInterface/FacTestToolInterface.h"
#include "./CommonUtility/BaseObject/ITestProcessor.h"
#include "./CommonUtility/CommonDefine.h"
#include "./CommonUtility/XML/MyMSXML.h"
#include "./GLog.h"
#include "./CommonUtility/BaseObject/IErrorHandling.h"

/***************Const **********************/
#define   LOG_PATH                   _T("D:\\Log\\DLL")
            
/*
typedef struct tagInitData
{
	CString str_IMEI_FromScanner;
	CString str_NAL_FromScanner;

	CString str_modelName;
	CString str_partNumber;
	CString str_picasso;
	CString str_station;
	CString str_swVer;
	CString str_fswVer;
	CString str_hwID;
	CString str_meVer;
	CString str_hwVer;
	CString str_pcbaVer;
	CString str_SDVer;
	CString str_RCFile;
	CString str_modelID;
	CString str_SIMFileName;
	CString str_PPIDpn;
	CString str_SDSizeMax;
	CString str_SDSizeMin;
	CString str_toolVer;
    
}InitData;
*/

using namespace QSYNC_LIB;

UNIFYDLL_IMEI_API bool __cdecl NewInterface (IFacTestToolInterface** ppIToolInterface);
UNIFYDLL_IMEI_API bool __cdecl DeleteInterface(IFacTestToolInterface* pIToolInterface);

class CIMEIInstance : public IFacTestToolInterface, public CNotifyBase_Char, public INotify_ANSI
{
public:
	CIMEIInstance();
	virtual ~CIMEIInstance(); 
	
	// Set/Get Parameter , such as MODEL, PROCESS, SO, Picasso etc //
	virtual bool SetParameterValue(char* sz_keyword, char* sz_value);
	virtual bool GetParameterValue(char* sz_keyword, char* sz_value, int i_size) ;

	// Set/Get FA information  //
	virtual bool SetFAData(int i_slot, char* sz_value);
	virtual bool GetFAData(int i_slot, char* sz_value, int i_size);

	// Get/Set FA Sector Data //
	virtual bool GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	virtual bool SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);

	//  Get test items  //
	virtual int GetItemCount();
	virtual bool GetItemByIndex(int i_index, char* sz_name, int i_size);

	// Test sequence  //
	virtual bool Begin(int i_slot = 1);
	virtual bool PreRun(int i_slot = 1);
	virtual bool Run(int i_slot = 1);
	virtual bool PostRun(int i_slot = 1);
	virtual bool Stop(int i_slot = 1);
	virtual bool End(int i_slot = 1);

	// Response to Jig control  //
	virtual bool ResopnseToJig(bool b_wait);

	// Response to show dialog  //
	virtual bool ResopnseToShowDlg(char* sz_showReslut);

	// INotifyBase_ANSI //
	virtual bool Register (INotify_Char* pNotify, const char* strEvent);
	virtual bool Register (EventFunc_Char* pEventFunc, const char* strEvent);
	virtual bool Unregister (INotify_Char* pNotify, const char* strEvent);
	virtual bool Unregister (EventFunc_Char* pEventFunc, const char* strEvent);
	virtual void DefineNotify (const char* strNewEvent);
	virtual void Fire (const char* strEvent, long nParam);

	virtual void Event(const std::string& strEvent, long nParam);


private:
	CString               m_str_UIresponse;
	int                   m_i_result;
	int                   m_i_startFlag;
	int                   m_i_waitFlag;
	bool				  m_strIMEIflag;

	std::map<CString,CString>  map_strData;

	st_UIResult st_Result;

	CGLog m_GLog;
	ITestProcessor*	m_pITool;
	IErrorHandling* m_pIErrorHandling;

	CString	m_cstrTestItemFile;
	CString m_cstrConfigFile;
	CMyMSXML m_TestItemXML;
	CMyMSXML m_ConfigXML;

	CString m_str_result;
	CString m_str_errorCode;
	CString m_str_message;

	CString m_str_picasso;
	CString m_str_modelName;
	CString m_str_station;
	CString m_str_NAL_FromScanner;
	CString m_str_PartNumber;
	CString m_str_IMEI_FromScanner;
	CString m_str_modelID;
	CString m_str_hwID;
	CString m_str_pcbaVer;
	CString m_str_PPIDpn;
	CString m_str_SIMFileName;
	CString m_str_RCFile;
	CString m_str_meVer;
	CString m_str_SWVer;
	CString m_currTime;

	std::vector<std::string> m_vToolInfo;
	std::map<std::string, std::string> m_mapToolInfo;
	std::string m_strStationID;
	std::string m_strToolVersion;

private:

	void GetComputerName(void);
	void WritePICSdata(void);

	/* google log */
	void TraceLog(CString str_log, const int i_severity = __INFO);

	bool CheckTestItemXMLExist();
	bool CheckConfigXMLExist();
	bool LoadToolInfo();
	bool UpdateMsg(int i_index);
	void SetLogFileName();

};
