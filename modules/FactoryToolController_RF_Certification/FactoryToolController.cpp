// FactoryToolController.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FactoryToolController.h"
#include <Shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;
using namespace QSYNC_LIB;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	return nRetCode;
}

//Export functions in FactoryToolController.dll
FACTORYTOOLCONTROLLER_API bool NewInterface(IFacTestToolInterface** ppIFacTestToolInterface)
{
	bool bRes = true;

	*ppIFacTestToolInterface = new CFactoryToolController;

	CFactoryToolController* pToolInstance = dynamic_cast<CFactoryToolController*>(*ppIFacTestToolInterface);

	if (!pToolInstance)
	{
		delete pToolInstance;
		*ppIFacTestToolInterface = NULL;
		bRes = false;
	}

	return bRes;
}

FACTORYTOOLCONTROLLER_API bool DeleteInterface(IFacTestToolInterface* pIFacTestToolInterface)
{
	if (pIFacTestToolInterface)
	{
		delete dynamic_cast<CFactoryToolController*>(pIFacTestToolInterface);
		pIFacTestToolInterface = NULL;
	}

	return true;
}

/**********************************************************
CNotifyBase_Char_Public
***********************************************************/

bool CNotifyBase_Char_Public::Register(INotify_Char* pNotify, CStringA strEvent)
{
	return CNotifyBase_Char::Register(pNotify, strEvent.GetString());
}

bool CNotifyBase_Char_Public::Register(EventFunc_Char* pEventFunc, CStringA strEvent)
{
	return CNotifyBase_Char::Register(pEventFunc, strEvent.GetString());
}

bool CNotifyBase_Char_Public::Unregister(INotify_Char* pNotify, CStringA strEvent)
{
	return CNotifyBase_Char::Unregister(pNotify, strEvent.GetString());
}

bool CNotifyBase_Char_Public::Unregister(EventFunc_Char* pEventFunc, CStringA strEvent)
{
	return CNotifyBase_Char::Unregister(pEventFunc, strEvent.GetString());
}

void CNotifyBase_Char_Public::DefineNotify(CStringA strNewEvent)
{
	CNotifyBase_Char::DefineNotify(strNewEvent.GetString());
}

void CNotifyBase_Char_Public::Fire(CStringA strEvent, long nParam)
{
	CNotifyBase_Char::Fire(strEvent.GetString(), nParam);
}

/**********************************************************
CFactoryToolController
***********************************************************/

CFactoryToolController::CFactoryToolController(void)
{
	::CoInitialize(NULL);

	this->DefineNotifyEvent();

	this->InitParameter();
}

CFactoryToolController::~CFactoryToolController(void)
{
	::CoUninitialize();
}

/**********************************************************
IFacTestToolInterface
***********************************************************/

/* Set/Get Parameter Value */
bool CFactoryToolController::SetParameterValue(char* sz_keyword, char* sz_value)
{
	CString strKey(sz_keyword), strValue(sz_value);

	//make a copy
	m_Parametermap[strKey] = strValue;

	//Make enhance controller list if get station name
	if(strKey == ParameterKeyWord::STATIONNAME)
		this->MakeEC(strValue);

	//For DL station, UI set DLMODE to DLL
	if(strKey == _T("DLMODE"))
		this->MakeEC(strValue);

	//Make config dir if get model name
	else if(strKey == ParameterKeyWord::MODELNAME)
	{
		//Factory tool config dir is C:\\PE_Config\\ModelName\\Setting
		this->m_Parametermap[ParameterKeyWord::CONFIGDIR] = _T("C:\\PE_Config\\") + this->m_Parametermap[ParameterKeyWord::MODELNAME] + _T("\\Setting\\");
	}
	
	return true;
}

bool CFactoryToolController::GetParameterValue(char* sz_keyword, char* sz_value, int i_size)
{
	CString strKey(sz_keyword);
	bool ret = false;
	wchar_t *wsz_value = NULL;
	
	if(this->m_pITI)
	{
		wsz_value = new wchar_t [i_size];

		ret = this->m_pITI->GetParameterValue(strKey.GetString(), wsz_value, i_size);
	}

	if(ret)
	{
		strcpy_s(sz_value, i_size, CW2A(wsz_value));
	}

	if(wsz_value)
	{
		delete [] wsz_value;
		wsz_value = NULL;
	}

	return ret;
}

/* Set/Get FA Data */
bool CFactoryToolController::SetFAData(int i_slot, char* sz_value)
{
	if(this->m_pITI)
		return this->m_pITI->SetFAData(i_slot, sz_value);

	return false;
}

bool CFactoryToolController::GetFAData(int i_slot, char* sz_value, int i_size)
{
	if(this->m_pITI)
		return this->m_pITI->GetFAData(i_slot, sz_value, i_size);

	return false;
}

/*Get/Set FA Sector Data*/
bool CFactoryToolController::GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	if(this->m_pITI)
		return this->m_pITI->GetFASector(i_slot, i_sectorNum, sz_sectorData, i_sectorSize);

	return false;
}

bool CFactoryToolController::SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	if(this->m_pITI)
		return this->m_pITI->SetFASector(i_slot, i_sectorNum, sz_sectorData, i_sectorSize);

	return false;
}

/* Get test items */
int CFactoryToolController::GetItemCount()
{
	return this->m_vTestItemList.size();
}

bool CFactoryToolController::GetItemByIndex(int i_index, char* sz_name, int i_size)
{
	//Request index larger than vector size
	if(i_index >= (int)this->m_vTestItemList.size())
		return false;

	strcpy_s(sz_name, i_size, CW2A(this->m_vTestItemList[i_index]));
	return true;
}

/* Test sequence */
bool CFactoryToolController::Begin(int i_slot)
{
	size_t ret = this->Enhance_OnAPPLaunch();
	
	this->RegistEvent();

	if(NOERROR == ret)
		this->SetParameterToTool();

	if(!this->m_pITI->OnAPPLaunch())
		ret = 2;

	return ret == NOERROR;
}

bool CFactoryToolController::PreRun(int i_slot)
{
	this->m_ErrorCode = "";

	return NOERROR == this->Enhance_OnRunInit(i_slot);
}

bool CFactoryToolController::Run(int i_slot)
{
	size_t ret = this->Enhance_RunTest(i_slot);
	
	this->SetTestResult();

	return NOERROR == ret;
}

bool CFactoryToolController::PostRun(int i_slot)
{
	return NOERROR == this->Enhance_OnRunFinish(i_slot);
}

bool CFactoryToolController::Stop(int i_slot)
{
	return true;
}

bool CFactoryToolController::End(int i_slot)
{
	if(!this->m_pITI->OnAPPEnd())
		return 1;

	return NOERROR == this->Enhance_OnAPPEnd();
}

/* Response to Jig control  */
bool CFactoryToolController::ResopnseToJig(bool b_wait)
{
	return true;
}

/* Response to show dialog  */
bool CFactoryToolController::ResopnseToShowDlg(char* sz_showReslut)
{
	return true;
}

/**********************************************************
INotifyBase_Char
***********************************************************/

bool CFactoryToolController::Register(INotify_Char* pNotify, const char* strEvent)
{
	return NotifyBase_Char_Public.Register(pNotify, CStringA(strEvent));
}

bool CFactoryToolController::Register(EventFunc_Char* pEventFunc, const char* strEvent)
{
	return NotifyBase_Char_Public.Register(pEventFunc, CStringA(strEvent));
}

bool CFactoryToolController::Unregister(INotify_Char* pNotify, const char* strEvent)
{
	return NotifyBase_Char_Public.Unregister(pNotify, CStringA(strEvent));
}

bool CFactoryToolController::Unregister(EventFunc_Char* pEventFunc, const char* strEvent)
{
	return NotifyBase_Char_Public.Unregister(pEventFunc, CStringA(strEvent));
}

void CFactoryToolController::DefineNotify(const char* strNewEvent)
{
	return NotifyBase_Char_Public.DefineNotify(CStringA(strNewEvent));
}

void CFactoryToolController::Fire(const char* strEvent, long nParam)
{
	return NotifyBase_Char_Public.Fire(CStringA(strEvent), nParam);
}

/**********************************************************
INotify_Char
***********************************************************/

void CFactoryToolController::Event(const char* strEvent, long nParam)
{
	CStringA cstrEvent(strEvent);

	if(cstrEvent == UI_RESULT)
	{
		st_UIResult *pst_UIResult = (st_UIResult*)nParam;

		CStringA cstrResult(pst_UIResult->sz_result);

		if(0 != cstrResult.CompareNoCase("PASS"))
			this->m_ErrorCode = pst_UIResult->sz_errCode;
	}

	this->Fire(strEvent, nParam);
}

/**********************************************************
CEnhanceController
***********************************************************/

size_t CFactoryToolController::Enhance_OnAPPLaunch()
{
	return CEnhanceController::Enhance_OnAPPLaunch();
}

size_t CFactoryToolController::Enhance_OnRunInit(int i_slot)
{
	return CEnhanceController::Enhance_OnRunInit(i_slot);
}

size_t CFactoryToolController::Enhance_RunTest(int i_slot)
{
	return CEnhanceController::Enhance_RunTest(i_slot);
}

size_t CFactoryToolController::Enhance_OnRunFinish(int i_slot)
{
	return CEnhanceController::Enhance_OnRunFinish(i_slot);
}

size_t CFactoryToolController::Enhance_OnAPPEnd()
{
	return CEnhanceController::Enhance_OnAPPEnd();
}

/**********************************************************
Protected member functions
***********************************************************/

void CFactoryToolController::DefineNotifyEvent()
{
	this->m_vEvent.clear();

	this->m_vEvent.push_back(UI_PROGRESS);
	this->m_vEvent.push_back(UI_MESSAGE);
	this->m_vEvent.push_back(UI_PATTERN);
	this->m_vEvent.push_back(UI_ITEM);
	this->m_vEvent.push_back(UI_RESULT);
	this->m_vEvent.push_back(UI_PICSDATA);
	this->m_vEvent.push_back(UI_JIGCONTROL);
	this->m_vEvent.push_back(UI_SHOWDIALOG);
	this->m_vEvent.push_back(UI_RELAYBOARD);
	this->m_vEvent.push_back(UI_POWERSUPPLY);

	for(	std::vector <CStringA>::iterator iter = this->m_vEvent.begin();
			iter != this->m_vEvent.end();
			iter++)
	{
		NotifyBase_Char_Public.DefineNotify(iter->GetString());
	}
}

void CFactoryToolController::MakeEC(CString strStation)
{
	this->FreeNextEC();

	CEnhanceController *pToolLoader = new CToolLoader(NULL);

	CEnhanceController *pPhoneHdrStd = new CPhoneHdrStd(pToolLoader);

	if(	strStation == STATION_2GPTEST ||
		strStation == STATION_3GPTEST ||
		strStation == STATION_BBTEST ||
		strStation == STATION_BTWLAN ||
		strStation == STATION_CURRENT ||
		strStation == STATION_ONLINE_WLS ||
		strStation == STATION_ONLINE_WLS2 ||
		strStation == "SPRD" )
	{
		CEnhanceController *pTestBoxHdrStd = new CTestBoxHdrStd(pPhoneHdrStd);

		CEnhanceController *pPowerSupplyHdrStd = new CPowerSupplyHdrStd(pTestBoxHdrStd);

		CEnhanceController *pGPIBHdrStd = new CGPIBHdrStd(pPowerSupplyHdrStd);

		CEnhanceController *pCablelossHdrStd = new CCablelossHdrStd(pGPIBHdrStd);

		CEnhanceController *pTestItemHdrStd = new CTestItemHdrStd(pCablelossHdrStd);

		this->SetNextEC(pTestItemHdrStd);
	}

	else if( strStation == STATION_CAMERA || strStation == STATION_ALS_AUDIO || 
		     strStation == STATION_AUDIO || strStation == STATION_ALS || 
		     strStation == STATION_IMEI || strStation == STATION_FCHECK
			 || strStation == STATION_MMI_BB )
	{
		CEnhanceController *pTestItemHdrStd = new CTestItemHdrStd(pPhoneHdrStd);

		this->SetNextEC(pTestItemHdrStd);
	}

	else if(strStation == STATION_OS_DL || strStation == STATION_REDL || strStation == _T("ReDL"))
	{
		CEnhanceController *pDownLoadHdrStd = new CDownLoadHdrStd(pPhoneHdrStd);
	
		this->SetNextEC(pDownLoadHdrStd);
	}

	this->SetECICallback(ECCallBack);
}

void CFactoryToolController::SetParameterToTool()
{
	//Set parameters which CFactoryToolController gets to tool again, prevent loss
	for(	std::map<CString, CString>::iterator iter = this->m_Parametermap.begin();
			iter != this->m_Parametermap.end();
			iter++)
	{
		this->m_pITI->SetParameterValue(iter->first.GetString(), iter->second.GetString());
	}
}

void CFactoryToolController::InitParameter()
{
	//Get module path as working dir
	TCHAR szModulePath[MAX_PATH] = {0};

	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);
	this->m_Parametermap[ParameterKeyWord::WORKINGDIR] = szModulePath;
	this->m_Parametermap[ParameterKeyWord::WORKINGDIR] += _T("\\");

	//Log dir
	this->m_Parametermap[ParameterKeyWord::LOGDIR] = _T("D:\\Log\\DLL\\");
}

void CFactoryToolController::RegistEvent()
{
	for(	std::vector <CStringA>::iterator iter = this->m_vEvent.begin();
			iter != this->m_vEvent.end();
			iter++)
	{
		this->m_pITI->Register(this, iter->GetString());
	}	
}

void CFactoryToolController::SetTestResult()
{
	st_UIResult Result;

	ZeroMemory(&Result, sizeof st_UIResult);
	Result.i_slot = 1;
	
	sprintf_s(Result.sz_result, MAX_BUFFER_SIZE, "%s", this->m_ErrorCode.IsEmpty() ? "PASS" : "FAIL");

	sprintf_s(Result.sz_errCode, MAX_BUFFER_SIZE, "%s", this->m_ErrorCode.IsEmpty() ? "PASS" : this->m_ErrorCode.GetString());

	sprintf_s(Result.sz_msg, MAX_BUFFER_SIZE, "%s", "Test finish");

	this->Fire(UI_RESULT, (long)&Result);
}

//Local function for EC call back
void ECCallBack(const TCHAR *szEvent, LPARAM lParam)
{
	CString strEvent(szEvent);

	//Fire testing item to factory UI
	if(strEvent == ECIMsg::TestingItem)
	{
		char* szTestItem = (char*)lParam;

		st_UIItem st_UIItem_Inst;

		st_UIItem_Inst.i_slot = 1;

		strcpy_s(st_UIItem_Inst.sz_item, MAX_BUFFER_SIZE, szTestItem);

		NotifyBase_Char_Public.Fire(UI_ITEM, (long)&st_UIItem_Inst);
	}
}