// Unify_RTR6285A_FTWireless.h : main header file for the Unify_RTR6285A_FTWireless DLL
//

#ifndef _UnifyDLL_WLS_H_
#define _UnifyDLL_WLS_H_


#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif


#include "resource.h"		// main symbols

#include "../UnifyInterface/Toolinterface_Notify.h"
#include "../UnifyInterface/FacTestToolInterface.h"
//#include "../UnifyInterface/UnifyAdapter.h"
#include "./CommonUtility/BaseObject/ITestProcessor.h"
#include "./CommonUtility/XML/MyMSXML.h"
#include "./CommonUtility/MD5/MD5Checksum.h"
#include "./GLog.h"


#ifdef UnifyDLL_RF_U2416_WLS_EXPORTS
#define IDL_API extern "C" __declspec(dllexport)
#define UnifyDLL_WLS_API extern "C" __declspec(dllexport)
#else
#define IDL_API extern "C" __declspec(dllimport)
#define UnifyDLL_WLS_API extern "C" __declspec(dllimport)
#endif

#ifdef _ToolInterface
IDL_API bool NewToolInterfaceNotify(IToolInterface_Notify** ppIToolInterface);
IDL_API bool DeleteToolInterfaceNotify(IToolInterface_Notify* pIToolInterface);
#else
UnifyDLL_WLS_API bool __cdecl NewInterface(IFacTestToolInterface** ppIToolInterface);
UnifyDLL_WLS_API bool __cdecl DeleteInterface(IFacTestToolInterface* pIToolInterface);
#endif

#define COMMONPATH     "C:\\PE_config"

using namespace QSYNC_LIB;


#ifdef _ToolInterface
class CUnifyDLL_WLS : public IToolInterface_Notify, public CNotifyBase_Char, public INotify_ANSI
#else
class CUnifyDLL_WLS :  public IFacTestToolInterface, public CNotifyBase_Char, public INotify_ANSI
#endif
{
// Data members
public:

	std::map<std::string, std::string> m_mapKeyWord;

protected:
private:
	ITestProcessor* m_pITool;

	CString m_cstrCableLossFile, m_cstrTestItemFile;
	CString m_cstrLogName,m_str_picasso;

	CMyMSXML m_CableLossXML, m_TestItemXML;

	CString m_cstr_StationName,m_cstr_ModelName;//,m_cstr_Picasso,m_cstr_SWVersion,ToolMode,m_cstr_ToolVersion;
	CString m_cstr_buildmode;

	CGLog   m_GLog;
	CString m_str_ToolWorkDirectory;
// Member functions
public:
	// Constructor & Destructor
	CUnifyDLL_WLS();
	virtual ~CUnifyDLL_WLS();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                          INotifyBase_Char
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool Register(INotify_Char* pNotify,const char* strEvent);
	virtual bool Register(EventFunc_Char* pEventFunc,const char* strEvent);

	virtual bool Unregister(INotify_Char* pNotify,const char* strEvent);
	virtual bool Unregister(EventFunc_Char* pEventFunc,const char* strEvent);

	virtual void DefineNotify(const char* strNewEvent);
	virtual void Fire(const char* strEvent,long nParam);
	virtual bool SetTag(int i_slot, char *sz_sectorData, int i_sectorSize);

	void DefineUINotify();

	// Setting files related functions
	bool CheckMD5(CString& strOutMsg);
	bool LoadSettingFile();
	bool CheckCableLossXMLExist();
	bool CheckTestItemXMLExist();
	bool CreateTestProcessor();
	bool LoadToolInfo();
	bool LoadTestItem();
	void FireResult(CString str_errorcode, CString str_errrormsg);

	/* MD5 verify functions */
	bool CheckTestItemXMLMD5(CString& strOutMsg);
	bool ConvertXML(CString str_XMLFilePathName, CString str_newName);
	bool MD5Verify(CString str_dirPath, CString str_XMLFilePathName, CString str_logFile);
	bool CheckVerifyResult(CString str_logFile);

	///////////////////////////////////////////////////////////////////////
	//                          UnifyInterface
	///////////////////////////////////////////////////////////////////////

	/* Set/Get FA Data */
	virtual bool SetFAData(int i_slot, char* sz_value);
	virtual bool GetFAData(int i_slot, char* sz_value, int i_size);

	/*Get/Set FA Sector Data*/
	virtual bool GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize, int i_idType);
	virtual bool SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);

	/* Get test items */
	virtual int GetItemCount();
	virtual bool GetItemByIndex(int i_index, char* sz_name, int i_size);

#ifdef _ToolInterface
	//Oninitial dialog
	virtual bool OnAPPLaunch();

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

	virtual bool OnAPPEnd();
	// End of Tool interface
#else
	/* Set/Get Parameter Value */
	virtual bool SetParameterValue(char* sz_keyword, char* sz_value);
	virtual bool GetParameterValue(char* sz_keyword, char* sz_value, int i_size);

	bool Begin(int i_slot);
	/* Test sequence */
	virtual bool PreRun   (int i_slot = 1);
	virtual bool Run      (int i_slot = 1);
	virtual bool PostRun  (int i_slot = 1);
	virtual bool Stop     (int i_slot = 1);
	bool End(int i_slot);
#endif

	/* Response to Jig control  */
	virtual bool ResopnseToJig(bool b_wait);

	/* Response to show dialog  */
	virtual bool ResopnseToShowDlg(char* sz_showReslut);

	// Notification
	virtual void Event(const std::string& strEvent, long nParam);
	


protected:
private:
	/* google Log */
	void WriteLog(CString str_log, const int i_severity = __INFO);

	void SetLogFileName();
	void SetLogFilePath();
};


#endif // End of #ifndef _UnifyDLL_WLS_H_
