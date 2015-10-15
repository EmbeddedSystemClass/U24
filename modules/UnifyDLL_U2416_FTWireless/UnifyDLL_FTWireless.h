// UnifyDLL_FTWireless.h : main header file for the UnifyDLL_FTWireless DLL
//

#ifndef _UNIFY_DLL_FTD_FT_WIRELESS_H_
#define _UNIFY_DLL_FTD_FT_WIRELESS_H_
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif


#include "resource.h"		// main symbols

#include "../UnifyInterface/Toolinterface_Notify.h"
#include "../UnifyInterface/FacTestToolInterface.h"
#include "./CommonUtility/BaseObject/ITestProcessor.h"
#include "./CommonUtility/CommonDefine.h"
#include "./CommonUtility/INI/IniAccess.h"
#include "./CommonUtility/XML/MyMSXML.h"
#include "./GLog.h"
#include "./CommonUtility/BaseObject/IErrorHandling.h"

#ifdef FT_WIRELESS_INSTANCE_EXPORTS
#define IDL_API extern "C" __declspec(dllexport)
#define FT_WIRELESS_INSTANCE_API extern "C" __declspec(dllexport)
#else
#define IDL_API extern "C" __declspec(dllimport)
#define FT_WIRELESS_INSTANCE_API extern "C" __declspec(dllimport)
#endif

#ifdef _ToolInterface
IDL_API bool NewToolInterfaceNotify(IToolInterface_Notify** ppIToolInterface);
IDL_API bool DeleteToolInterfaceNotify(IToolInterface_Notify* pIToolInterface);
#else
FT_WIRELESS_INSTANCE_API bool __cdecl NewInterface(IFacTestToolInterface** ppIToolInterface);
FT_WIRELESS_INSTANCE_API bool __cdecl DeleteInterface(IFacTestToolInterface* pIToolInterface);
#endif


using namespace QSYNC_LIB;


#ifdef _ToolInterface
class CUnifyDLL_FTWireless : public IToolInterface_Notify, public CNotifyBase_Char, public INotify_ANSI
#else
class CUnifyDLL_FTWireless : public IFacTestToolInterface, public CNotifyBase_Char, public INotify_ANSI
#endif
{
// Data members
public:
protected:
private:

	st_UIResult st_Result;

	CString m_str_ToolMode;
	CString m_str_picasso;
	CString m_str_tag;
	CString m_str_modelName;
	CString m_str_station;
	CString m_str_logFileName;
	CString m_str_ToolWorkDirectory;
	CString m_str_picasso_pool[4];

	CString m_str_result;
	CString m_str_errorCode;
	CString m_str_message;

	char m_flagMessage;

	ITestProcessor* m_pITool;
	IErrorHandling* m_pIErrorHandling;

	CString m_cstrCableLossFile, m_cstrDeviceItemFile, m_cstrTestItemFile;

	CMyMSXML m_CableLossXML, m_DeviceItemXML, m_TestItemXML;

	std::vector<std::string> m_vToolInfo;
	std::map<std::string, std::string> m_mapToolInfo;
	std::string m_strStationID;
	std::string m_strToolVersion;

	CGLog m_GLog;

	std::map<CString, CString>	 m_map_parameterList;      // keyword <-> value;

// Member functions
public:
	// Constructor & Destructor
	CUnifyDLL_FTWireless();
	virtual ~CUnifyDLL_FTWireless();

	/* Setting files related functions */
	bool LoadSettingFile();
	bool GetXMLFileName();
	bool CheckCableLossXMLExist();
	bool CheckDeviceItemXMLExist();
	bool CheckTestItemXMLExist();
	bool LoadToolInfo();

	/* MD5 verify functions */
	bool CheckTestItemXMLMD5();
	bool ConvertXML(CString str_XMLFilePathName, CString str_newName);
	bool MD5Verify(CString str_dirPath, CString str_XMLFilePathName, CString str_logFile);
	bool CheckVerifyResult(CString str_logFile);

	/* FT DLL Interface functions */
	/* Get test items */
	virtual int GetItemCount();
	virtual bool GetItemByIndex(int i_Index, char* sz_Name, int i_Size);

	/* Set/Get FA Data */
	virtual bool SetFAData(int i_slot, char* sz_value) ;
	virtual bool GetFAData(int i_slot, char* sz_value, int i_size);

	/* Set/Get FA Sector Data */
	virtual bool GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	virtual bool SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	virtual bool SetTag(int i_slot, char *sz_sectorData, int i_sectorSize);

	/* Response to Jig control  */
	virtual bool ResopnseToJig(bool b_wait);

	/* Response to show dialog  */
	virtual bool ResopnseToShowDlg(char* sz_showReslut);

	/* Notification */
	virtual bool Register(INotify_Char* pNotify,const char* strEvent);
	virtual bool Register(EventFunc_Char* pEventFunc,const char* strEvent);
	virtual bool Unregister(INotify_Char* pNotify,const char* strEvent);
	virtual bool Unregister(EventFunc_Char* pEventFunc,const char* strEvent);
	virtual void DefineNotify(const char* strNewEvent);
	virtual void Fire(const char* strEvent,long nParam);

	virtual void Event(const std::string& strEvent, long nParam);

#ifdef _ToolInterface
	//Oninitial dialog
	virtual bool OnAPPLaunch(int i_slot = 1);

	//Each run
	virtual bool SetCableloss(const wchar_t *szType, const wchar_t *szBand, const wchar_t *szChannel, const wchar_t *szLoss);

	virtual bool SetTestPart(IToolInterface::ToolTestParts PartID, const wchar_t *szName, const wchar_t *szItem, const wchar_t *szContent, const wchar_t *szAttribute = L"");

	virtual bool SetTestItem(const wchar_t *szItem);//Must keep the same for factory
	virtual bool InitTestItem(const wchar_t *szItem);

	virtual bool SetParameterValue(const wchar_t* sz_keyword, const wchar_t* sz_value);
	virtual bool GetParameterValue(const wchar_t* sz_keyword, wchar_t* sz_value, int i_size);

	virtual bool OnRunInit(int i_slot = 1);

	virtual bool RunTestItem(int iItem, int i_slot = 1);

	virtual bool OnRunFinish(int i_slot = 1);

	virtual bool OnAPPEnd(int i_slot = 1);
	// End of Tool interface
#else
	/* Set/Get Parameter Value */
	virtual bool SetParameterValue(char* sz_keyword, char* sz_value);
	virtual bool GetParameterValue(char* sz_keyword, char* sz_value, int i_size);

	/* Test sequence */
	virtual bool Begin(int i_slot = 1);
	virtual bool PreRun(int i_slot = 1);
	virtual bool Run(int i_slot = 1);
	virtual bool PostRun(int i_slot = 1);
	virtual bool Stop(int i_slot = 1);
	virtual bool End(int i_slot = 1);
#endif

protected:
private:
	/* Get test item result */
	bool GetTestResult(int i_Index, st_UIResult* psResult);

	/* PICS data */
	void WritePICSdata(void);

	/* google Log */
	void WriteLog(CString str_log, const int i_severity = __INFO);
	void SetLogFileName();
	void SetLogFilePath();
};


#endif // End of #ifndef _UNIFY_DLL_FTD_FT_WIRELESS_H_
