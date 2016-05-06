// E330_Service_Adapter.h : main header file for the E330_Service_Adapter DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "..\..\Modules\UnifyInterface\Toolinterface_Notify.h"
#include "..\..\Modules\UnifyInterface\SerTestToolInterface.h"
#include "..\..\Modules\UnifyInterface\FacTestToolInterface.h"
#include "..\..\Modules\FactoryToolController_Boston\EnhanceController\EnhanceController.h"
#include "..\..\Modules\FactoryToolController_Boston\EnhanceController\CableLossHandler_Standard.h"
#include "..\..\Modules\FactoryToolController_Boston\EnhanceController\GPIBHandler_Standard.h"
#include "..\..\Modules\FactoryToolController_Boston\EnhanceController\PhoneHandler_Standard.h"
#include "..\..\Modules\FactoryToolController_Boston\EnhanceController\PowerSupplyHandler_Standard.h"
#include "..\..\Modules\FactoryToolController_Boston\EnhanceController\TestBoxHandler_Standard.h"
#include "..\..\Modules\FactoryToolController_Boston\EnhanceController\TestItemHandler_Standard.h"
#include "..\..\Modules\FactoryToolController_Boston\EnhanceController\ToolLoader.h"
#include "..\..\Modules\FactoryToolController_Boston\EnhanceController\DownLoadHandler_Standard.h"

#define ISRV_EXPORT_EXPORTS

#ifdef ISRV_EXPORT_EXPORTS
#define ISRV_API extern "C" __declspec(dllexport)
#define ISRV_CLASS_API __declspec(dllexport)
#else
#define ISRV_API extern "C" __declspec(dllimport)
#define ISRV_CLASS_API __declspec(dllimport)
#endif

//#define _BBTEST_nonNFC
#define _CSDWIFI_1
#define UNIFYMODELNAME			      _T("GBROB1A")

#ifdef _BBTEST_NFC
	#define QISDA_MODULE_NAME "BBTEST_NFC"
	#define QISDA_MODULE_TITLE "BBTEST_NFC"
	#define QISDA_MODULE_INFO "reserved"
	#define QISDA_MODULE_TYPE "FACTORYINFOLIST"
	static const bool QISDA_MODULE_CHECK_DONGLE = true;
#elif defined _BBTEST_nonNFC
	#define QISDA_MODULE_NAME "BBTEST_nonNFC"
	#define QISDA_MODULE_TITLE "BBTEST_nonNFC"
	#define QISDA_MODULE_INFO "reserved"
	#define QISDA_MODULE_TYPE "FACTORYINFOLIST"
	static const bool QISDA_MODULE_CHECK_DONGLE = true;
#elif defined _2G3GTEST
	#define QISDA_MODULE_NAME "2G3GTEST"
	#define QISDA_MODULE_TITLE "2G3GTEST"
	#define QISDA_MODULE_INFO "reserved"
	#define QISDA_MODULE_TYPE "FACTORYINFOLIST"
	static const bool QISDA_MODULE_CHECK_DONGLE = true;
#elif defined _LTE
	#define QISDA_MODULE_NAME "LTE"
	#define QISDA_MODULE_TITLE "LTE"
	#define QISDA_MODULE_INFO "reserved"
	#define QISDA_MODULE_TYPE "FACTORYINFOLIST"
	static const bool QISDA_MODULE_CHECK_DONGLE = true;
#elif defined _BTWLAN
	#define QISDA_MODULE_NAME "BTWLAN"
	#define QISDA_MODULE_TITLE "BTWLAN"
	#define QISDA_MODULE_INFO "reserved"
	#define QISDA_MODULE_TYPE "FACTORYINFOLIST"
	static const bool QISDA_MODULE_CHECK_DONGLE = true;
#elif defined _CSDEM
	#define QISDA_MODULE_NAME "CSDEM"
	#define QISDA_MODULE_TITLE "CSDEM"
	#define QISDA_MODULE_INFO "reserved"
	#define QISDA_MODULE_TYPE "FACTORYINFOLIST"
	static const bool QISDA_MODULE_CHECK_DONGLE = false;
#elif defined _CSDWRITE
	#define QISDA_MODULE_NAME "CSDWRITE"
	#define QISDA_MODULE_TITLE "CSDWRITE"
	#define QISDA_MODULE_INFO "reserved"
	#define QISDA_MODULE_TYPE "CSDWRITE"
	static const bool QISDA_MODULE_CHECK_DONGLE = false;
#elif defined _CSDWRITE_MARCO
	#define QISDA_MODULE_NAME "CSDWRITE"
	#define QISDA_MODULE_TITLE "CSDWRITE"
	#define QISDA_MODULE_INFO "reserved"
	#define QISDA_MODULE_TYPE "CSDWRITE_MARCO"
	static const bool QISDA_MODULE_CHECK_DONGLE = false;
#elif defined _CSDWIFI_1
	#define QISDA_MODULE_NAME "CSDWIFI_1"
	#define QISDA_MODULE_TITLE "CSDWIFI_1"
	#define QISDA_MODULE_INFO "reserved"
	#define QISDA_MODULE_TYPE "CSDWIFI_1"
	static const bool QISDA_MODULE_CHECK_DONGLE = false;
#elif defined _CSDWIFI_2
	#define QISDA_MODULE_NAME "CSDWIFI_2"
	#define QISDA_MODULE_TITLE "CSDWIFI_2"
	#define QISDA_MODULE_INFO "reserved"
	#define QISDA_MODULE_TYPE "CSDWIFI_2"
	static const bool QISDA_MODULE_CHECK_DONGLE = false;
/*
#elif defined _OSDL
	#define QISDA_MODULE_NAME "OS_DL"
	#define QISDA_MODULE_TITLE "OS_DL"
	#define QISDA_MODULE_INFO "reserved"
	#define QISDA_MODULE_TYPE "DOWNLOAD"
	static const bool QISDA_MODULE_CHECK_DONGLE = false;
*/
#endif

typedef int (*MsgCallBack)(const char* szMsg, const char* szTag);
typedef int (*PortCallBack)(const char* szPort);
typedef int (*ResultCallBack)(const int nIndex, const char* szUnit, const char* szLower, const char* szUpper, const char* szMeasured, const char* szResult, const char* szErrCode, const char* szMsg);
typedef int (*ShowPopupMsgCallBack)(const char* szMsg);

/******************************************************************************
 * export  extern C API ,
 *   1. New interface
 *   2. Del interface
******************************************************************************/

//ISRV_API bool NewSrvInterface(IServiceInterface** pp_ISRV);
//ISRV_API bool DelSrvInterface(IServiceInterface* p_ISRV);

ISRV_API void GetQisdaModuleInfo(char* szName, char* szTitle, char* szType, char* szInfo, bool* isDongleLock);

ISRV_API HANDLE QisdaDllNew();
ISRV_API void QisdaDllDelete(HANDLE hResource);

ISRV_API int SetCallBackMsg(HANDLE hResource, int(*CallBack)(const char*, const char*)); // NO_ERROR
ISRV_API int SetCallBackPort(HANDLE hResource, int(*CallBack)(const char*)); // NO_ERROR
ISRV_API int SetCallBackResult(HANDLE hResource, int(*CallBack)(const int, const char*, const char*, const char*, const char*, const char*, const char*)); // NO_ERROR

ISRV_API int SetParameter(HANDLE hResource, char* szKey, char* szValue); // NO_ERROR
ISRV_API int GetParameter(HANDLE hResource, char* szKey, char* szValue); // NO_ERROR

ISRV_API int GetPortCount(HANDLE hResource);
ISRV_API int GetPortByIndex(HANDLE hResource, int nIndex, char* szPort);

ISRV_API int GetInfoCount(HANDLE hResource);
ISRV_API int GetInfoByIndex(HANDLE hResource, int nIndex, char* szName, char* szInfo);

ISRV_API int Begin(HANDLE hResource);
ISRV_API int PreRun(HANDLE hResource);
ISRV_API int Run(HANDLE hResource);
ISRV_API int PostRun(HANDLE hResource);

ISRV_API int ShowPopupMsg(HANDLE hResource, int(*CallBack)(const char*)); // NO_ERROR

/******************************************************************************
 *  declare class
 *
******************************************************************************/
using namespace QSYNC_LIB;

class CNotifyBase_Char_Public : CNotifyBase_Char
{
public:
	virtual bool Register(INotify_Char* pNotify, CStringA strEvent);
	virtual bool Register(EventFunc_Char* pEventFunc, CStringA strEvent);

	virtual bool Unregister(INotify_Char* pNotify, CStringA strEvent);
	virtual bool Unregister(EventFunc_Char* pEventFunc, CStringA strEvent);

	virtual void DefineNotify(CStringA strNewEvent);

	virtual void Fire(CStringA strEvent, long nParam);
};

CNotifyBase_Char_Public		NotifyBase_Char_Public;

/******************************************************************************
 *  declare class
 *
******************************************************************************/

class CServiceToolController : public IServiceInterface, public CEnhanceController, public INotify_Char
{
//IServiceInterface
public:
	CServiceToolController();
	virtual ~CServiceToolController();

	size_t SetParameterValue(char* wsz_keyword, char* wsz_value);
	size_t GetParameterValue(char* wsz_keyword, char* wsz_value);
	void SetCallSendMsg(LPVOID pfunc);
	void SetCallSetPort(LPVOID pfunc);
	void SetCallSendResult(LPVOID pfunc);
	void SetShowPopupMsg(LPVOID pfunc);

	int GetInfoCount();
	int GetInfoByIndex(int nIndex, char* szName, char*szInfo);

	/* Send Msg to DLL */
	//size_t MsgHandler(const wchar_t* target, const wchar_t* msg_key , const wchar_t* msg_value );

	/**
     *   To Reg the callback function.
	 *   @param  p_fn_Callback is a pinter to a callback function
     *   @return return 0 is OK. not 0 is other error code.
     */
	//size_t RegCallback( p_fn_Callback );

	//INotify_Char
public:
	void Event(const char* str_event, long nParam);

	//Enhance Controller Interface
	size_t Enhance_OnAPPLaunch();

	size_t Enhance_OnRunInit(int i_slot = 1);

	size_t Enhance_RunTest(int i_slot = 1);

	size_t Enhance_OnRunFinish(int i_slot = 1);

	size_t Enhance_OnAPPEnd();

protected:
	size_t	SetParameterToDLL();
	size_t	MakeEnhanceController();
	void	DefineNotifyEvent();
	void	RegistEvent();

protected:
	std::vector <CStringA> m_vEvent;
	CString	m_strCurrentStation;
	size_t	nTestItemIndex;
	bool	bProcessResult;

private:
	MsgCallBack m_fpSendMsg;
	void SendMsg(const char* szMsg, const char* szTag);
	PortCallBack m_fpSetPort;
	void SetPort(const char* szPort);
	ResultCallBack m_fpSendResult;
	void SendResult(const int nIndex, const char* szUnit, const char* szLower, const char* szUpper, const char* szMeasured, const char* szResult, const char* szErrCode, const char* szMsg);
	ShowPopupMsgCallBack m_fpSetShowPopupMsg;
	int SendShowPopupMsg(const char* szMsg);

	IFacTestToolInterface*			m_p_facTestToolInterface;  // point to IFacTestToolInterface
};
