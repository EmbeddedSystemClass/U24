// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FACTORYTOOLCONTROLLER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FACTORYTOOLCONTROLLER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef FACTORYTOOLCONTROLLER_EXPORTS
#define FACTORYTOOLCONTROLLER_API extern "C" __declspec(dllexport)
#else
#define FACTORYTOOLCONTROLLER_API extern "C" __declspec(dllimport)
#endif

#include "..\UnifyInterface\Toolinterface_Notify.h"
#include "..\UnifyInterface\FacTestToolInterface.h"
#include ".\EnhanceController\EnhanceController.h"
#include ".\EnhanceController\CableLossHandler_Standard.h"
#include ".\EnhanceController\GPIBHandler_Standard.h"
#include ".\EnhanceController\PhoneHandler_Standard.h"
#include ".\EnhanceController\PowerSupplyHandler_Standard.h"
#include ".\EnhanceController\TestBoxHandler_Standard.h"
#include ".\EnhanceController\TestItemHandler_Standard.h"
#include ".\EnhanceController\ToolLoader.h"
#include ".\EnhanceController\DownLoadHandler_Standard.h"

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

class CFactoryToolController :	public IFacTestToolInterface, 
												public CEnhanceController, 
												public INotify_Char
{

public:
	CFactoryToolController(void);
	virtual ~CFactoryToolController(void);

	// TODO: add your methods here.

	/**********************************************************
										IFacTestToolInterface
	***********************************************************/
	/* Set/Get Parameter Value */
	virtual bool SetParameterValue(char* sz_keyword, char* sz_value);
	virtual bool GetParameterValue(char* sz_keyword, char* sz_value, int i_size);

	/* Set/Get FA Data */
	virtual bool SetFAData(int i_slot, char* sz_value);
	virtual bool GetFAData(int i_slot, char* sz_value, int i_size);

	/*Get/Set FA Sector Data*/
	virtual bool GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	virtual bool SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);

	/* Get test items */
	virtual int GetItemCount();
	virtual bool GetItemByIndex(int i_index, char* sz_name, int i_size);

	/* Test sequence */
	virtual bool Begin(int i_slot = 1);
	virtual bool PreRun(int i_slot = 1);
	virtual bool Run(int i_slot = 1);
	virtual bool PostRun(int i_slot = 1);
	virtual bool Stop(int i_slot = 1);
	virtual bool End(int i_slot = 1);

	/* Response to Jig control  */
	virtual bool ResopnseToJig(bool b_wait);

	/* Response to show dialog  */
	virtual bool ResopnseToShowDlg(char* sz_showReslut);

	/**********************************************************
										INotifyBase_Char
	***********************************************************/
	virtual bool Register(INotify_Char* pNotify, const char* strEvent);
	virtual bool Register(EventFunc_Char* pEventFunc, const char* strEvent);

	virtual bool Unregister(INotify_Char* pNotify, const char* strEvent);
	virtual bool Unregister(EventFunc_Char* pEventFunc, const char* strEvent);

	virtual void DefineNotify(const char* strNewEvent);

	virtual void Fire(const char* strEvent, long nParam);

	/**********************************************************
										INotify_Char
	***********************************************************/
	virtual void Event(const char* strEvent, long nParam);

	/**********************************************************
										CEnhanceController
	***********************************************************/
	virtual size_t Enhance_OnAPPLaunch();

	virtual size_t Enhance_OnRunInit(int i_slot = 1);

	virtual size_t Enhance_RunTest(int i_slot = 1);

	virtual size_t Enhance_OnRunFinish(int i_slot = 1);

	virtual size_t Enhance_OnAPPEnd();

protected:

	void DefineNotifyEvent();
	void MakeEC(CString strStation);
	void SetParameterToTool();
	void InitParameter();
	void RegistEvent();
	void SetTestResult();

protected:
	std::vector <CStringA> m_vEvent;
	CStringA						m_ErrorCode;
};

//Local function for EC call back
void ECCallBack(const TCHAR *szEvent, LPARAM lParam);

//Export functions in FactoryToolController.dll
FACTORYTOOLCONTROLLER_API bool __cdecl NewInterface(IFacTestToolInterface** ppIFacTestToolInterface);
FACTORYTOOLCONTROLLER_API bool __cdecl DeleteInterface(IFacTestToolInterface* pIFacTestToolInterface);