// UnifyDLL_Detroit_WLS.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "UnifyDLL_Detroit_WLS.h"
//#include "./CommonUtility/INI/IniAccess.h"
#include <Shlwapi.h>
#include <io.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#if (_WIN32_WINNT >= 0x0600)  
#import "msxml6.dll"
#else  
#import "msxml2.dll"  
#endif
// ----------------- The extern functions ------------------
#ifdef _ToolInterface
IDL_API bool NewToolInterfaceNotify(IToolInterface_Notify** ppIToolInterface)
#else
UnifyDLL_Detroit_WLS_API bool NewInterface(IFacTestToolInterface** ppIToolInterface)
#endif
{
	bool bRes = false;

	if (! (*ppIToolInterface))
	{
		*ppIToolInterface = new CUnifyDLL_Detroit_WLS;
		bRes = true;
	}

	//CUnifyDLL_Detroit_WLS* pToolInstance = dynamic_cast<CUnifyDLL_Detroit_WLS*>(*ppIToolInterface);	

	return bRes;
}

#ifdef _ToolInterface
IDL_API bool __cdecl DeleteToolInterfaceNotify(IToolInterface_Notify* pIToolInterface)
#else
UnifyDLL_Detroit_WLS_API bool __cdecl DeleteInterface(IFacTestToolInterface* pIToolInterface)
#endif
{
	if (pIToolInterface)
	{
		delete (CUnifyDLL_Detroit_WLS*)pIToolInterface;
		pIToolInterface = NULL;
	}

	return true;
}


// ---------------- Constructor & Destructor ---------------

CUnifyDLL_Detroit_WLS::CUnifyDLL_Detroit_WLS()
{

	//DefineNotifyEvent
	DefineUINotify();

	::AfxOleInit();
	::CoInitialize(NULL);

	m_pITool = NULL;
	m_mapKeyWord.clear();

	SetLogFilePath();
	SetLogFileName();
}

CUnifyDLL_Detroit_WLS::~CUnifyDLL_Detroit_WLS()
{}


// ------------ Setting files related functions ------------

bool CUnifyDLL_Detroit_WLS::LoadSettingFile()
{

	if(m_cstr_ModelName.IsEmpty() || m_cstr_StationName.IsEmpty())
	{
		return false;
	}

//	m_cstrCableLossFile = m_cstr_StationName + _T("_CableLoss.xml");
	m_cstrTestItemFile = m_cstr_ModelName + _T("_") + m_cstr_StationName;


	return true;
}

//bool CUnifyDLL_Detroit_WLS::CheckCableLossXMLExist()
//{
//	bool bRes = false;
//	char szModulePath[MAX_PATH] = {0};
//	CString cstrConfigXML = _T("");
//
//// 	GetModuleFileName(NULL, szModulePath, MAX_PATH);
//// 	PathRemoveFileSpec(szModulePath);
//	cstrConfigXML.Format(_T("%s\\%s\\Setting\\%s"), COMMONPATH,m_cstr_ModelName, m_cstrCableLossFile);
//	if (::_taccess(cstrConfigXML, 0) == 0)
//	{
//		m_cstrCableLossFile = cstrConfigXML;
//
//		if (m_CableLossXML.Load(m_cstrCableLossFile) == ERROR_SUCCESS)
//		{
//			return true;
//		}
//	}
//	
//	return false;
//}

bool CUnifyDLL_Detroit_WLS::CheckTestItemXMLExist()
{
	bool bRes = false;
	char szModulePath[MAX_PATH] = {0};
	CString cstrRFXML = _T("");

	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);
#ifndef _ToolInterface
	if (m_cstr_buildmode == _T("FACTORY"))
	{
		cstrRFXML.Format(_T("%s\\Qisda\\%s_TestItem.xml"), szModulePath, m_cstrTestItemFile);
	}
	else
	{
		cstrRFXML.Format(_T("%s\\Qisda\\%s_TestItem_RD.xml"), szModulePath, m_cstrTestItemFile);
	}
#else
	if (m_cstr_buildmode == _T("FACTORY"))
	{
		cstrRFXML.Format(_T("%sQisda\\%s_TestItem.xml"), m_str_ToolWorkDirectory, m_cstrTestItemFile);
	}
	else
	{
		cstrRFXML.Format(_T("%sQisda\\%s_TestItem_RD.xml"), m_str_ToolWorkDirectory, m_cstrTestItemFile);
	}
#endif
	
	if (::_taccess(cstrRFXML, 0) == 0)
	{
		m_cstrTestItemFile = cstrRFXML;

		if (m_TestItemXML.Load(m_cstrTestItemFile) == ERROR_SUCCESS)
		{
			return true;
		}	
	}
	return false;
}

bool CUnifyDLL_Detroit_WLS::CreateTestProcessor()
{
	bool bRes = true;

	CString cstrTestProcessor = _T("");
	CToolRuntimeClass* pcObjectFactory = RF_RUNTIME_CLASS(CToolBaseObject);
	CToolBaseObject* pcObject = NULL;

	// Load test processor name
	XMLNode toolProcessor = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Name"));
	if (toolProcessor != NULL)
	{
		cstrTestProcessor = m_TestItemXML.GetNodeText(toolProcessor);
	}
	else
	{
		bRes = false;
	}

	// Create test processor and initialize it
	if (bRes)
	{
		CT2A szTestProcessor(cstrTestProcessor);
		std::string strTestProcessor(szTestProcessor);

		if ((pcObject = pcObjectFactory->CreateObject(strTestProcessor)) != NULL)
		{
			m_pITool = dynamic_cast<ITestProcessor*>(pcObject);

			m_pITool->Register(this, DLL_PATTERN);
			m_pITool->Register(this, DLL_TESTING_ITEM);
			m_pITool->Register(this, DLL_MESSAGE);
			m_pITool->Register(this, DLL_TEST_RESULT);
			m_pITool->Register(this, DLL_PICSDATA);
			m_pITool->Register(this, DLL_JIG_CONTROL);
			m_pITool->Register(this, DLL_SHOW_MSG_DLG);
			m_pITool->Register(this, DLL_LOG);

			//m_pITool->SetCableLossXML(m_cstrCableLossFile);
			m_pITool->SetTestItemXML(m_cstrTestItemFile);
			m_pITool->SetStationModelName(m_cstr_StationName, m_cstr_ModelName);
		}
		else
		{
			bRes = false;
		}
	}

	return bRes;
}

bool CUnifyDLL_Detroit_WLS::LoadToolInfo()
{
	if (m_pITool)
	{
		return m_pITool->LoadToolInfo();
	}
	return false;
}

bool CUnifyDLL_Detroit_WLS::LoadTestItem()
{
	if (m_pITool)
	{
		return m_pITool->LoadTestItem();
	}
	return false;
}


//Add Begin Function for Init TestItem by Jackiesen.wan 2012/02/02
#ifdef _ToolInterface
bool CUnifyDLL_Detroit_WLS::OnAPPLaunch(int i_slot)
#else
bool CUnifyDLL_Detroit_WLS::Begin(int i_slot)
#endif
{
	// 1. Get ModelName and StationName
	if (!LoadSettingFile())
	{
		FireResult(_T(""),_T("Load Setting File Fail, please check it!"));
		return false;
	}

	//// 2.Check if CableLoss.xml file exist
	//if (!CheckCableLossXMLExist())
	//{
	//	FireResult(_T(""),_T("Check CableLoss XML File fail, please check it"));
	//	return false;
	//}

	// 3. Check if TestItem.xml file exist
	if (!CheckTestItemXMLExist())
	{
		FireResult(_T(""),_T("Check TestItem XML File fail, please check it!"));
		return false;
	}

	// 4.Check MD5 for Test XML
	//if (m_cstr_buildmode == _T("FACTORY"))
	//{
	//	CString strOutMsg = _T("");
	//	if (!CheckTestItemXMLMD5(strOutMsg))
	//	{
	//		FireResult(_T(""),strOutMsg);
	//		return false;
	//	}
	//}

	// 5. Create tool processor
	if (!CreateTestProcessor())
	{
		FireResult(_T(""),_T("Create Test Processor Fail, please check it!"));
		return false;
	}

	//6. set parameter map to lower
	if (m_pITool)
	{
		m_pITool->g_mapToolInfo = m_mapKeyWord;
	}

	// 7. Load tool information,tool version will set in g_maptoolinfo
	if (!LoadToolInfo())
	{
		FireResult(_T(""),_T("Load Tool Info fail, please check it!"));
		return false;
	}

	// 8. Load test items
	if (!LoadTestItem())
	{
		FireResult(_T(""),_T("Load Test Item fail, please check it!"));
		return false;
	}

	// 9. run begin
	if (m_pITool)
	{
		return m_pITool->Begin();
	}
	
	return false;
}

#ifdef _ToolInterface
bool CUnifyDLL_Detroit_WLS::OnAPPEnd(int i_slot)
#else
bool CUnifyDLL_Detroit_WLS::End( int i_slot )
#endif
{
	if (m_pITool)
	{
		return m_pITool->End();
	}
	return false;
}

// ---------------- Set/Get Parameter Value ----------------
#ifndef _ToolInterface
bool CUnifyDLL_Detroit_WLS::SetParameterValue(char* sz_keyword, char* sz_value)
{
	CString cstrLogName,str_key;

	if (sz_keyword == NULL)
	{
		return true;
	}

    m_mapKeyWord[std::string(sz_keyword)] = std::string(sz_value);


	str_key.Format(_T("%s"), sz_keyword);	

	/* Read ParameterValue to use */
	if(str_key == _T("STATION"))
	{
		m_cstr_StationName = CString(sz_value);
	}
	else if(str_key == _T("MODEL_NAME"))
	{
		m_cstr_ModelName = CString(sz_value);
	}
	else if(str_key == _T("TOOL_MODE"))
	{
		m_cstr_buildmode = CString(sz_value);
	}
	else if(str_key == _T("PICASSO"))
	{
		m_str_picasso.Format(_T("%s"),sz_value);

		SetLogFileName();

		if(m_pITool)
		{
			m_pITool->g_mapToolInfo["PICASSO"] = std::string(sz_value);
		}
		m_pITool->SetParameterValue(_T("PICASSO"), sz_value);
	}

	return true;
}

bool CUnifyDLL_Detroit_WLS::GetParameterValue(char* sz_keyword, char* sz_value, int i_size)
{
	if (m_pITool)
	{
		return m_pITool->GetParameterValue(sz_keyword,sz_value,i_size);
	}

	return false;
}
#else
bool CUnifyDLL_Detroit_WLS::SetParameterValue(const wchar_t* sz_keyword, const wchar_t* sz_value)
{
	CString cstrLogName;
	CString str_key = CW2CT(sz_keyword);
	CString str_value = CW2CT(sz_value);

	if (sz_keyword == NULL)
	{
		return true;
	}

    m_mapKeyWord[std::string(str_key)] = std::string(str_value);

	/* Read ParameterValue to use */
	if(str_key == _T("STATION"))
	{
		m_cstr_StationName = CString(sz_value);
	}
	else if(str_key == _T("MODEL_NAME"))
	{
		m_cstr_ModelName = CString(sz_value);
	}
	else if(str_key == _T("TOOL_MODE"))
	{
		m_cstr_buildmode = CString(sz_value);
	}
	else if(str_key == _T("PICASSO"))
	{
		m_str_picasso.Format(_T("%s"),sz_value);

		SetLogFileName();

		if(m_pITool)
		{
			m_pITool->g_mapToolInfo["PICASSO"] = std::string(str_value);
		}
		m_pITool->SetParameterValue(_T("PICASSO"), CT2A(str_value.GetString()));
	}
	else if (str_key == _T("WORKING_DIR"))
	{
		m_str_ToolWorkDirectory = CString(sz_value);
	}

	return true;
}

bool CUnifyDLL_Detroit_WLS::GetParameterValue(const wchar_t* sz_keyword, wchar_t* sz_value, int i_size)
{
	CString str_key = CW2CT(sz_keyword);
	CString str_value = CW2CT(sz_value);

	if (m_pITool)
	{
		return m_pITool->GetParameterValue(CT2A(str_key.GetString()), CT2A(str_value.GetString()), i_size);
	}

	return false;
}
#endif

// -------------------- Set/Get FA Data --------------------

bool CUnifyDLL_Detroit_WLS::SetFAData(int i_slot, char* sz_value)
{
	return m_pITool->SetFAData(i_slot, sz_value);
}

bool CUnifyDLL_Detroit_WLS::GetFAData(int i_slot, char* sz_value, int i_size)
{
	return m_pITool->GetFAData(i_slot, sz_value, i_size);
}

bool CUnifyDLL_Detroit_WLS::GetFASector( int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize )
{
	return m_pITool->GetFASector(i_slot,i_sectorNum,sz_sectorData,i_sectorSize);
}

bool CUnifyDLL_Detroit_WLS::SetFASector( int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize )
{
	return m_pITool->SetFASector(i_slot,i_sectorNum,sz_sectorData,i_sectorSize);
}

// --------------------- Get test items --------------------

int CUnifyDLL_Detroit_WLS::GetItemCount()
{
	return m_pITool->GetItemCount();
}

bool CUnifyDLL_Detroit_WLS::GetItemByIndex(int iIndex, char* szName, int iSize)
{
	return m_pITool->GetItemByIndex(iIndex, szName, iSize);
}

// --------------------- Test sequence ---------------------

#ifdef _ToolInterface
bool CUnifyDLL_Detroit_WLS::SetCableloss(const wchar_t *szType, const wchar_t *szBand, const wchar_t *szChannel, const wchar_t *szLoss)
{
	return true; //this->m_pITool->SetCableLoss(CW2A(szType), CW2A(szBand), CW2A(szChannel), CW2A(szLoss));
}

bool CUnifyDLL_Detroit_WLS::SetTestPart(IToolInterface::ToolTestParts PartID, const wchar_t *szName, const wchar_t *szItem, const wchar_t *szContent, const wchar_t *szAttribute)
{
	bool bRet = true;

	switch(PartID)
	{
	case IToolInterface::PHONE:
		//bRet = this->m_pITool->SetPhone(CW2A(szName), CW2A(szItem), CW2A(szContent));
		break;

	case IToolInterface::POWERSUPPLY:
		//bRet = this->m_pITool->SetPS(CW2A(szName), CW2A(szItem), CW2A(szContent), CW2A(szAttribute));
		break;

	case IToolInterface::TEST_BOX:
		//bRet = this->m_pITool->SetTestBox(CW2A(szName), CW2A(szItem), CW2A(szContent), CW2A(szAttribute));
		break;

	case IToolInterface::COMMON:
		//bRet = this->m_pITool->SetCommonInstrument(CW2A(szName), CW2A(szItem), CW2A(szContent), CW2A(szAttribute));
		break;

	case IToolInterface::JIG://currently no corresponding method
		bRet = false;
		break;

	default:
		bRet = false;
		//WriteLog("Fail to SetTestPart, PartID wrong");
		break;
	}

	//TraceLog(MakeSTDstring("SetTestPart %d SUCCESS  (10 for TEST_BOX, 20 for PS, 30 for PHONE, 40 for JIG)", PartID).c_str());

	return bRet;
}

bool CUnifyDLL_Detroit_WLS::SetTestItem(const wchar_t *szItem)
{
	return true; //this->m_pITool->SetTestItem(CW2A(szItem));
}

bool CUnifyDLL_Detroit_WLS::InitTestItem(const wchar_t *szItem)
{
	return true;
}
#endif

#ifdef _ToolInterface
bool CUnifyDLL_Detroit_WLS::OnRunInit(int i_slot)
#else
bool CUnifyDLL_Detroit_WLS::PreRun(int i_slot)
#endif
{
	return m_pITool->PreRun();
}

#ifdef _ToolInterface
bool CUnifyDLL_Detroit_WLS::RunTestItem(int iItem, int i_slot)
#else
bool CUnifyDLL_Detroit_WLS::Run(int i_slot)
#endif
{
	return  m_pITool->Run();
}

#ifdef _ToolInterface
bool CUnifyDLL_Detroit_WLS::OnRunFinish(int i_slot)
#else
bool CUnifyDLL_Detroit_WLS::PostRun(int i_slot)
#endif
{
	/* Upload Log to server */
	string sz_modelName = m_cstr_ModelName.GetBuffer();
	char str_modelName[50];
	strcpy(str_modelName, sz_modelName.c_str());
	m_pITool->LogUpload(str_modelName);

	return m_pITool->PostRun();
}

#ifndef _ToolInterface
bool CUnifyDLL_Detroit_WLS::Stop(int i_slot)
{
	return m_pITool->Stop();
}
#endif
// ---------------- Response to Jig control ----------------

bool CUnifyDLL_Detroit_WLS::ResopnseToJig(bool b_wait)
{
	return m_pITool->ResopnseToJig(b_wait);
}


// ---------------- Response to show dialog ----------------

bool CUnifyDLL_Detroit_WLS::ResopnseToShowDlg(char* sz_showReslut)
{
	return m_pITool->ResopnseToShowDlg(sz_showReslut);
}


// --------------------- Notification ----------------------

void CUnifyDLL_Detroit_WLS::Event(const std::string& strEvent, long nParam)
{
	if (strEvent == DLL_PATTERN)
	{
		sPattern* psPattern = (sPattern*)nParam;
		st_UIPattern sFired;

		sFired.i_slot = 1;
		sprintf_s(sFired.sz_pattern, MAX_BUFFER_SIZE - 1, "%s", psPattern->strPattern.c_str());

		Fire(UI_PATTERN, (long)&sFired);
	}
	else if (strEvent == DLL_TESTING_ITEM)
	{
		sTestingItem* psTestingItem = (sTestingItem*)nParam;
		st_UIItem sFired;

		sFired.i_slot = 1;
		sprintf_s(sFired.sz_item, MAX_BUFFER_SIZE - 1, "%s", psTestingItem->strTestingItem.c_str());

		Fire(UI_ITEM, (long)&sFired);
	}
	else if (strEvent == DLL_MESSAGE)
	{
		sMessage* psMessage = (sMessage*)nParam;
		st_UIMessage sFired;

		sFired.i_slot = 1;
		sprintf_s(sFired.sz_message, MAX_BUFFER_SIZE - 1, "%s", psMessage->strMessage.c_str());

		WriteLog(psMessage->strMessage.c_str());

		Fire(UI_MESSAGE, (long)&sFired);
	}
	else if (strEvent == DLL_TEST_RESULT)
	{
		sTestResult* psTestResult = (sTestResult*)nParam;
		st_UIResult sFired;

		sFired.i_slot = 1;
		sprintf_s(sFired.sz_time, MAX_BUFFER_SIZE - 1, "%s", psTestResult->strTime.c_str());
		sprintf_s(sFired.sz_result, MAX_BUFFER_SIZE - 1, "%s", psTestResult->strResult.c_str());
		sprintf_s(sFired.sz_itemCode, MAX_BUFFER_SIZE - 1, "%s", psTestResult->strItemCode.c_str());
		sprintf_s(sFired.sz_errCode, MAX_BUFFER_SIZE - 1, "%s", psTestResult->strErrCode.c_str());
		sprintf_s(sFired.sz_band, MAX_BUFFER_SIZE - 1, "%s", psTestResult->strBand.c_str());
		sprintf_s(sFired.sz_channel, MAX_BUFFER_SIZE - 1, "%s", psTestResult->strChannel.c_str());
		sprintf_s(sFired.sz_lower, MAX_BUFFER_SIZE - 1, "%s", psTestResult->strLower.c_str());
		sprintf_s(sFired.sz_upper, MAX_BUFFER_SIZE - 1, "%s", psTestResult->strUpper.c_str());
		sprintf_s(sFired.sz_measured, MAX_BUFFER_SIZE - 1, "%s", psTestResult->strMeasured.c_str());
		sprintf_s(sFired.sz_unit, MAX_BUFFER_SIZE - 1, "%s", psTestResult->strUnit.c_str());
		sprintf_s(sFired.sz_elapse, MAX_BUFFER_SIZE - 1, "%s", psTestResult->strElapse.c_str());
		sprintf_s(sFired.sz_msg, MAX_BUFFER_SIZE - 1, "%s", psTestResult->strMsg.c_str());

		char szMsg[512];
		memset(szMsg, 0, 512);
		sprintf_s(szMsg, 511, "%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s",
			psTestResult->strTime.c_str(),
			psTestResult->strResult.c_str(),
			psTestResult->strItemCode.c_str(),
			psTestResult->strErrCode.c_str(),
			psTestResult->strBand.c_str(),
			psTestResult->strChannel.c_str(),
			psTestResult->strLower.c_str(),
			psTestResult->strUpper.c_str(),
			psTestResult->strMeasured.c_str(),
			psTestResult->strUnit.c_str(),
			psTestResult->strElapse.c_str(),
			psTestResult->strMsg.c_str());

		WriteLog("========================= test results =========================");
		WriteLog(szMsg);
		WriteLog("================================================================");

		Fire(UI_RESULT, (long)&sFired);
	}
	else if (strEvent == DLL_PICSDATA)
	{
		sPICSData* psPICSData = (sPICSData*)nParam;
		st_PICSData sFired;

		sFired.i_slot = 1;
		sprintf_s(sFired.sz_key, MAX_BUFFER_SIZE - 1, "%s", psPICSData->strKey.c_str());
		sprintf_s(sFired.sz_value, MAX_BUFFER_SIZE - 1, "%s", psPICSData->strValue.c_str());

		char szTmp[512];
		memset(szTmp, 0, 512);
		sprintf_s(szTmp, 511, "##### PICS data : %s , %s #####", sFired.sz_key, sFired.sz_value);
		WriteLog(szTmp);

		Fire(UI_PICSDATA, (long)&sFired);
	}
	else if (strEvent == DLL_JIG_CONTROL)
	{
		sJigControl* psJigControl = (sJigControl*)nParam;
		st_JigRequest sFired;

		sFired.i_slot = 1;
		sprintf_s(sFired.sz_switch, MAX_BUFFER_SIZE - 1, "%s", psJigControl->strSwitch.c_str());

		Fire(UI_JIGCONTROL, (long)&sFired);
	}
	else if (strEvent == DLL_SHOW_MSG_DLG)
	{
		sShowMsgDlg* psShowMsgDlg = (sShowMsgDlg*)nParam;
		st_ShowDialog sFired;

		sFired.i_slot = 1;
		sprintf_s(sFired.sz_message, MAX_BUFFER_SIZE - 1, "%s", psShowMsgDlg->strMessage.c_str());

		Fire(UI_SHOWDIALOG, (long)&sFired);
	}
	else if (strEvent == DLL_LOG)
	{
		sTraceLog* psTraceLog = (sTraceLog*)nParam;

		WriteLog(psTraceLog->strMessage.c_str(), psTraceLog->iSeverity);
	}
	else if (strEvent == UI_PICSDATA)
	{
		Fire(UI_PICSDATA, nParam);
	}
}

void CUnifyDLL_Detroit_WLS::FireResult( CString str_errorcode, CString str_errrormsg )
{
	st_UIResult sFired;
	sFired.i_slot = 1;
	sprintf_s(sFired.sz_errCode, MAX_BUFFER_SIZE - 1, "%s", str_errorcode.GetBuffer());
	str_errorcode.ReleaseBuffer();
	sprintf_s(sFired.sz_msg, MAX_BUFFER_SIZE - 1, "%s", str_errrormsg.GetBuffer());
	WriteLog(sFired.sz_msg);
	str_errrormsg.ReleaseBuffer();
	Fire(UI_RESULT, (long)&sFired);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          INotifyBase_Char
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnifyDLL_Detroit_WLS::Register(INotify_Char* pNotify,const char* strEvent)
{
	return CNotifyBase_Char::Register(pNotify, strEvent);
}
bool CUnifyDLL_Detroit_WLS::Register(EventFunc_Char* pEventFunc,const char* strEvent)
{
	return CNotifyBase_Char::Register(pEventFunc, strEvent);
}

bool CUnifyDLL_Detroit_WLS::Unregister(INotify_Char* pNotify,const char* strEvent)
{
	return CNotifyBase_Char::Unregister(pNotify, strEvent);
}
bool CUnifyDLL_Detroit_WLS::Unregister(EventFunc_Char* pEventFunc,const char* strEvent)
{
	return CNotifyBase_Char::Unregister(pEventFunc, strEvent);
}

void CUnifyDLL_Detroit_WLS::DefineNotify(const char* strNewEvent)
{
	return CNotifyBase_Char::DefineNotify(strNewEvent);
}
void CUnifyDLL_Detroit_WLS::Fire(const char* strEvent,long nParam)
{
	return CNotifyBase_Char::Fire(strEvent, nParam);
}

void CUnifyDLL_Detroit_WLS::DefineUINotify()
{
	this->DefineNotify(UI_PROGRESS);
	this->DefineNotify(UI_MESSAGE);
	this->DefineNotify(UI_PATTERN);
	this->DefineNotify(UI_ITEM);
	this->DefineNotify(UI_RESULT);
	this->DefineNotify(UI_PICSDATA);
	this->DefineNotify(UI_JIGCONTROL);
	this->DefineNotify(UI_SHOWDIALOG);
	this->DefineNotify(UI_RELAYBOARD);
	this->DefineNotify(UI_POWERSUPPLY);
}

//bool CUnifyDLL_Detroit_WLS::CheckMD5(CString& strOutMsg)
//{
//	char sz_currentPath[MAX_PATH] = {0};
//	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
//	::PathRemoveFileSpec(sz_currentPath);
//
//	CString str_MD5LocalPath,strDir;
//	strDir.Format(_T("%s"),sz_currentPath);
//
//	str_MD5LocalPath.Format("%s\\MD5\\%s_%s_TestItem_MD5.xml", sz_currentPath, m_cstr_ModelName, m_cstr_StationName);
//
//	if (_taccess(str_MD5LocalPath, 0) != 0) 
//	{
//		strOutMsg = _T("MD5 xml file of test item is not exist!");
//		return false;
//	}
//
//	CMD5Checksum md5check;
//
//	bool bRet = md5check.CheckDirMD5(str_MD5LocalPath,strDir,strOutMsg);
//
//	return bRet;
//}


void CUnifyDLL_Detroit_WLS::WriteLog(CString str_log, const int i_severity)
{
	m_GLog.Log(i_severity, str_log);
}

void CUnifyDLL_Detroit_WLS::SetLogFileName()
{
	CString str_curTime = _T("");
	CString str_fileName = _T("");

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	str_curTime.Format(_T("%04d%02d%02d_%02d%02d_%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

	if (m_str_picasso.IsEmpty())
	{
		str_fileName.Format(_T("RD_%s.log"), str_curTime);
	} 
	else
	{
		str_fileName.Format(_T("%s_%s.log"), m_str_picasso, str_curTime);
	}

	m_GLog.SetGLogFileName_Info(str_fileName);

}

void CUnifyDLL_Detroit_WLS::SetLogFilePath()
{
	/* Initial log path */
	CString str_path = _T("");
	CString str_curDate = _T("");

	/* Get current time */
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	str_curDate.Format(_T("%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);

	str_path.Format(_T("D:\\LOG\\DLL"));
	m_GLog.InitializeGLog(str_path);
}

// ------------ MD5 verify functions ------------

//bool CUnifyDLL_Detroit_WLS::CheckTestItemXMLMD5(CString& strOutMsg)
//{
//	char sz_currentPath[MAX_PATH] = {0};
//	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
//	::PathRemoveFileSpec(sz_currentPath);
//
//	CString str_MD5LocalPath;
//	str_MD5LocalPath.Format("%s\\MD5\\%s_%s_TestItem_MD5.xml", sz_currentPath, m_cstr_ModelName, m_cstr_StationName);
//	if (_taccess(str_MD5LocalPath, 0) != 0)
//	{
//		strOutMsg = _T("MD5 xml file of test item is not exist!");
//		return false;
//	}
//
//	/* Replace tool path of MD5 xml */
//	//if (ConvertXML(str_MD5LocalPath, m_cstrTestItemFile) != true)
//	//{
//	//	strOutMsg = _T("Replace tool path of MD5 xml fail!");
//	//	return false;
//	//}
//
//	//CString str_logName;
//	//str_logName = "D:\\LOG\\MD5.log";
//	//if (MD5Verify(m_cstrTestItemFile, str_MD5LocalPath, str_logName) != true)
//	//{
//	//	strOutMsg = _T("Verify MD5 fail!");
//	//	return false;
//	//}
//
//	//if (CheckVerifyResult(str_logName) != true)
//	//{
//	//	strOutMsg = _T("Check test item xml MD5 fail.\nTest item xml may be changed, Please check it!");
//	//	return false;
//	//}
//
//	return true;
//}

bool CUnifyDLL_Detroit_WLS::ConvertXML(CString str_XMLFilePathName, CString str_newName)
{
	/* Check input */
	if(str_XMLFilePathName == _T("") || str_newName == _T(""))
	{
		return false;
	}

	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr;
	hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument26));
	if(FAILED(hr)) {
		return FALSE;
	}

	if(_taccess(str_XMLFilePathName, 0) != 0)
	{
		return false;
	}
	pDoc->load((LPCTSTR)str_XMLFilePathName);
	if(pDoc == NULL)
	{
		return false;
	}

	MSXML2::IXMLDOMNodePtr pNode;
	pNode = pDoc->selectSingleNode("//FCIV");
	if(pNode == NULL) {
		return false;
	}

	MSXML2::IXMLDOMNodePtr pItem;
	pItem = pNode->GetfirstChild();
	while(NULL != pItem)
	{
		MSXML2::IXMLDOMNodePtr pNode;
		pNode = pItem->selectSingleNode("name");
		if(pNode == NULL)
		{
			return false;
		}
		CString str_nameValue;
		str_nameValue = (char *)pNode->text;
		str_nameValue.Replace(str_nameValue, str_newName);
		pNode->Puttext((_bstr_t)(const char *)str_nameValue);

		pItem = pItem->GetnextSibling();
	}

	pDoc->save((LPCTSTR)str_XMLFilePathName);

	return true;
}

//bool CUnifyDLL_Detroit_WLS::MD5Verify(CString str_dirPath, CString str_XMLFilePathName, CString str_logFile)
//{
//	/* Check Input */
//	if(str_dirPath == _T("") || str_XMLFilePathName == _T("")  || str_logFile == _T(""))
//	{
//		return false;
//	}
//
//	/* Check fciv.exe exist */
//	char sz_currentPath[MAX_PATH] = { 0 };
//	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
//	::PathRemoveFileSpec(sz_currentPath);
//	CString str_fcivPathName;
//	str_fcivPathName.Format(_T("%s\\MD5\\fciv.exe"), sz_currentPath);
//	if (_taccess(str_fcivPathName, 0) != 0)
//	{
//		return false;
//	}
//
//	/* Command */
//	CString str_command;
//	CString str_commandOne;
//	CString str_commandTwo;
//	CString str_commandThree;
//	str_commandOne = _T("cmd.exe /c ");
//	str_commandTwo = str_fcivPathName + _T(" -r -v ") + str_dirPath + _T(" -xml ") + str_XMLFilePathName;
//	str_commandThree = str_commandThree + _T(" > ") + str_logFile;
//	str_command = str_commandOne + str_commandTwo + str_commandThree;
//
//	/* Run */
//	TCHAR sz_commandLine[1024];
//	memset(sz_commandLine, 0, sizeof(sz_commandLine));
//	for (int i = 0; i < str_command.GetLength(); i ++)
//	{
//		sz_commandLine[i] = str_command[i];
//	}
//	PROCESS_INFORMATION pi;
//	STARTUPINFO si;
//	memset(&si, 0, sizeof(si));
//	si.cb          = sizeof(si);
//	si.wShowWindow = SW_HIDE;
//	si.dwFlags     = STARTF_USESHOWWINDOW;
//
//	BOOL b_createRes = FALSE;
//	b_createRes = CreateProcess(NULL, sz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
//	if(b_createRes == FALSE)
//	{
//		return false;
//	}
//	DWORD dw_waitResult = 0;
//	DWORD dw_timeout;
//	dw_timeout = 10000;
//	dw_waitResult = ::WaitForSingleObject(pi.hProcess, dw_timeout);
//	switch(dw_waitResult)
//	{
//	case WAIT_FAILED:
//		::CloseHandle(pi.hProcess);
//		pi.hProcess = NULL;
//		return false;
//
//	case WAIT_TIMEOUT:
//		::CloseHandle(pi.hProcess);
//		pi.hProcess = NULL;
//		return false;
//
//	case WAIT_OBJECT_0:
//		break;
//	}
//	::CloseHandle(pi.hProcess);
//	pi.hProcess = NULL;
//
//	return true;
//}

bool CUnifyDLL_Detroit_WLS::CheckVerifyResult(CString str_logFile)
{
	/* Check Input */
	if(str_logFile == _T(""))
	{
		return false;
	}

	/* Check file exist */
	int i_existentResult = 0;
	int i_retry = 10;
	while(1)
	{
		i_existentResult = _taccess(str_logFile, 0);
		if(i_existentResult != 0)
		{
			if(i_retry <= 0)
			{
				break;
			}
			else
			{
				Sleep(1000);
				i_retry--;
				continue;
			}
		}
		else
		{
			break;
		}
	}
	if(i_existentResult != 0)
	{
		return false;
	}

	/* Check file Size */
	DWORD dw_fileSize = 0;
	i_retry = 10;
	while(1)
	{
		HANDLE h_handle = NULL;
		h_handle = CreateFile(str_logFile, FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if(h_handle != INVALID_HANDLE_VALUE)
		{
			dw_fileSize = GetFileSize(h_handle, NULL);
			CloseHandle(h_handle);
			if(dw_fileSize == 0)
			{
				if(i_retry <= 0)
				{
					break;
				}
				else
				{
					Sleep(1000);
					i_retry--;
					continue;
				}
			}
			else
			{
				Sleep(1000);
				break;
			}
		}
		else {
			break;
		}
	}
	if(dw_fileSize == 0)
	{
		return false;
	}

	/* Read */
	int i_succeedFlag = 0;
	CStdioFile obj_stdioFile;
	if (!obj_stdioFile.Open(str_logFile, CFile::modeRead|CFile::modeNoTruncate|CFile::shareDenyNone, NULL))
	{
		return false;
	}
	try
	{
		CString str_string;
		while(obj_stdioFile.ReadString(str_string))
		{
			if(str_string.Find(_T("All files verified successfully")) != -1)
			{
				i_succeedFlag = 1;
				break;
			}
			str_string = _T("");
		}
	}
	catch(CFileException *e)
	{
		e->Delete();
	}

	obj_stdioFile.Close();

	if (i_succeedFlag == 1)
	{
		return true;
	}
	else
	{
		return false;
	}

	return true;
}