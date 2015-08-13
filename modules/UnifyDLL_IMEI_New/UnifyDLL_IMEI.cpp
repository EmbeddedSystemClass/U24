// UnifyDLL_IMEI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "UnifyDLL_IMEI.h"
#include "MD5.h"
#include <shlwapi.h> //PathFileExists

#include "..\Common\CommonUtil\Win32SHELLUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

using namespace std;

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          Exported API
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UNIFYDLL_IMEI_API bool NewInterface(IFacTestToolInterface** ppIToolInterface)
{
	bool bRes = false;

	if (!(*ppIToolInterface))
	{
		*ppIToolInterface = new CIMEIInstance;
		bRes = true;
	}

	CIMEIInstance* pToolInstance = dynamic_cast<CIMEIInstance*>(*ppIToolInterface);

	if (!bRes && pToolInstance != NULL)
	{
		delete pToolInstance;
		*ppIToolInterface = NULL;
	}

	return bRes;
}

UNIFYDLL_IMEI_API bool DeleteInterface(IFacTestToolInterface* pIToolInterface)
{
	if (pIToolInterface)
	{
		delete (CIMEIInstance*)pIToolInterface;
		pIToolInterface = NULL;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                      Class
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CIMEIInstance::CIMEIInstance()
{
	m_str_result = _T("");
	m_str_errorCode = _T("");
	m_str_message = _T("");

	DefineNotify (UI_PROGRESS);
	DefineNotify (UI_MESSAGE);
	DefineNotify (UI_PATTERN);
	DefineNotify (UI_ITEM);
	DefineNotify (UI_RESULT);
	DefineNotify (UI_PICSDATA);
	DefineNotify (UI_JIGCONTROL);
	DefineNotify (UI_SHOWDIALOG);
	DefineNotify(UI_RELAYBOARD);
	DefineNotify (UI_POWERSUPPLY);

	m_str_message.Empty();
	m_str_errorCode.Empty();
	m_str_UIresponse.Empty();

	m_i_result = 0;
	m_i_startFlag = 1;
	m_i_waitFlag = 0;	

	/* Init google log */
	m_GLog.InitializeGLog(LOG_PATH);

	::AfxOleInit();
	::CoInitialize(NULL);
}


CIMEIInstance::~CIMEIInstance()
{
	/* Unregister */
	if (m_pITool)
	{
		m_pITool->Unregister(this, TRACE_LOG);
		m_pITool->Unregister(this, FACTORY_LOG);
		m_pITool->Unregister(this, PICS_DATA);
		m_pITool->Unregister(this, SHOW_PICTURE);
		m_pITool->Unregister(this, JIG_REQUEST);
		m_pITool->Unregister(this, SHOW_DIALOG);
	}

	/* Free google log */
	m_GLog.ShutdownGLog();
}

bool CIMEIInstance::Register (INotify_Char* pNotify, const char* strEvent)
{
	return CNotifyBase_Char::Register(pNotify, strEvent);
}

bool CIMEIInstance::Register (EventFunc_Char* pEventFunc, const char* strEvent)
{
	return CNotifyBase_Char::Register(pEventFunc, strEvent);
}

bool CIMEIInstance::Unregister (INotify_Char* pNotify, const char* strEvent)
{
	return CNotifyBase_Char::Unregister(pNotify, strEvent);
}

bool CIMEIInstance::Unregister (EventFunc_Char* pEventFunc, const char* strEvent)
{
	return CNotifyBase_Char::Unregister(pEventFunc, strEvent);
}

void CIMEIInstance::DefineNotify (const char* strNewEvent)
{
	CNotifyBase_Char::DefineNotify(strNewEvent);
}

void CIMEIInstance::Fire (const char* strEvent, long nParam)
{
	CNotifyBase_Char::Fire(strEvent, nParam);
}

void CIMEIInstance::Event(const std::string& strEvent, long nParam)
{
	CString str_log;
	if (strEvent == TRACE_LOG)
	{
		sTraceLog* psTraceLog = (sTraceLog*)nParam;
		str_log = psTraceLog->strMessage.c_str();
		TraceLog(str_log);
	}
	else if (strEvent == FACTORY_LOG)
	{
		st_UIResult st_factoryLog;

		m_pITool->GetTestResult(0, &st_factoryLog);
		Fire(UI_RESULT, (long)&st_factoryLog);

		//str_log.Format("Result: %s, ItemCode: %s, ErrCode: %s, Band: %s, Channel: %s, Upper: %s, Lower: %s, Measured: %s, Unit: %s, Msg: %s",
		//	st_factoryLog.sz_result, st_factoryLog.sz_itemCode, st_factoryLog.sz_errCode, st_factoryLog.sz_band, st_factoryLog.sz_channel,
		//	st_factoryLog.sz_upper, st_factoryLog.sz_lower, st_factoryLog.sz_measured, st_factoryLog.sz_unit, st_factoryLog.sz_msg);
		str_log.Format("[MSG]: %s", st_factoryLog.sz_msg);

		TraceLog(str_log);
	}
	else if (strEvent == PICS_DATA)
	{
		Fire(UI_PICSDATA, nParam);
	}
	else if (strEvent == SHOW_PICTURE)
	{
		Fire(UI_PATTERN, nParam);
	}
	else if (strEvent == JIG_REQUEST)
	{
		Fire(UI_JIGCONTROL, nParam);
	}
	else if (strEvent == SHOW_DIALOG)
	{
		Fire(UI_SHOWDIALOG, nParam);
	}
}


bool CIMEIInstance::SetParameterValue(char* sz_keyword, char* sz_value)
{
	CString str_temp = _T("");

	if (strcmp(sz_keyword,_T("NAL")) == 0)
	{
		m_str_NAL_FromScanner = sz_value;
	}
	else if (strcmp(sz_keyword,_T("PART_NUMBER")) == 0)
	{
		m_str_PartNumber = sz_value;
		m_pITool->SetPartNumber(sz_value);
	}
	else if (strcmp(sz_keyword,_T("IMEI")) == 0)
	{
		m_pITool->SetIMEI(sz_value);
		m_str_IMEI_FromScanner = sz_value;
		// Set google log file name again.
		if ( !m_str_IMEI_FromScanner.IsEmpty())	SetLogFileName();
		m_pITool->SetIMEI(sz_value);
	}
    else if (strcmp(sz_keyword,_T("MODEL_NAME")) == 0)
    {
		m_pITool->SetModelName(sz_value);
		str_temp.Format(_T("%s"), sz_value);
	    m_str_modelName = str_temp;
    }
	else if (strcmp(sz_keyword, _T("STATION")) == 0 )
	{
		m_pITool->SetStation(sz_value);
		str_temp.Format(_T("%s"), sz_value);
		m_str_station = str_temp;
	}
	else if (strcmp(sz_keyword, "TOOL_VERSION") == 0)
	{
		str_temp.Format(_T("%s"), sz_value);
		m_strToolVersion = str_temp;
	}
	else if (strcmp(sz_keyword, _T("PICASSO")) == 0 )
	{
		m_pITool->SetPicasso(sz_value);
		m_str_picasso.Format(_T("%s"), sz_value);
		str_temp.Format("Picasso: %s", m_str_picasso);
		TraceLog(str_temp);
	}
	else if (strcmp(sz_keyword, _T("MODEL_CODE")) == 0 )
	{
		m_str_modelID = sz_value;
	}
	else if (strcmp(sz_keyword, _T("HWID")) == 0 )
	{
		m_str_hwID = sz_value;
	}
	else if (strcmp(sz_keyword, _T("SW_VERSION")) == 0 )
	{
		m_pITool->SetSWVersion(sz_value);
		m_str_SWVer = sz_value;
	}
	else if(strcmp(sz_keyword, _T("FACTORY_VERSION")) == 0)
	{
		m_pITool->SetFactoryVersion(sz_value);
	}
	else if (strcmp(sz_keyword, _T("PCBA_VERSION")) == 0 )
	{
		m_str_pcbaVer = sz_value;
	}
	else if (strcmp(sz_keyword, _T("HW_VERSION")) == 0 )
	{
		m_pITool->SetHWVersion(sz_value);
	}
	/* IMEI STATION */
	else if (strcmp(sz_keyword, _T("PN_CHECK")) == 0 )
	{
		m_str_PPIDpn = sz_value;
	}
	else if (strcmp(sz_keyword, _T("SIM_FILE")) == 0 )
	{
		m_str_SIMFileName = sz_value;
	}
	else if(strcmp(sz_keyword, _T("lock_filename")) == 0)
	{
		m_pITool->SetSIMFileName(sz_value);
	}
	else if (strcmp(sz_keyword, _T("RC_FILE")) == 0 )
	{
		m_str_RCFile = sz_value;
	}
	else if (strcmp(sz_keyword, _T("ME_VERSION")) == 0 )
	{
		m_str_meVer = sz_value;
	}
	else if (strcmp(sz_keyword, _T("BOARD_ID")) == 0 )
	{
		m_pITool->SetBoardID(sz_value);
	}
	else if(strcmp(sz_keyword, _T("UI_LOG_PATH")) == 0)
	{
		//CString str_path;
		//str_path.Format(_T("%s\\%s"), sz_value, m_str_station.GetBuffer());
		//m_GLog.InitializeGLog(str_path);
		//m_str_station.ReleaseBuffer();
	}
	 else
	{
		map_strData[sz_keyword] = sz_value;
	}

	return true;
}
bool CIMEIInstance::GetParameterValue(char* sz_keyword, char* sz_value, int i_size)
{
	if (strcmp(sz_keyword, "TOOL_VERSION") == 0)
	{
		strcpy_s(sz_value, i_size, m_strToolVersion.c_str());
	}
	return true;
}

bool CIMEIInstance::SetFAData(int i_slot, char *sz_value)
{
	return false;
}

bool CIMEIInstance::GetFAData(int i_slot, char* sz_value, int i_size)
{
	return false;
}

bool CIMEIInstance::GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	if (!m_pITool->ReadFAData_New( i_slot, i_sectorNum, sz_sectorData, i_sectorSize))
	{
		m_pITool->GetTestResult(i_slot, &st_Result);
		Fire(UI_RESULT, (long)&st_Result);
		return false;
	}
	return true;
}
bool CIMEIInstance::SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	bool b_Res = false;
	if (!(b_Res = m_pITool->IsMobileInFTDMode()))
	{
		m_pITool->GetTestResult(i_slot, &st_Result);
		Fire(UI_RESULT, (long)&st_Result);
	}
	if (b_Res)
	{
		char out_buf[FTD_BUF_SIZE] = {0};
		if (!(b_Res = m_pITool->WriteFAData_New(i_sectorNum, sz_sectorData, i_sectorSize, out_buf)))
		{
			m_pITool->GetTestResult(i_slot, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
		}
	}
	return b_Res;
	//return true;
}

int CIMEIInstance::GetItemCount()
{
	return m_pITool->GetTestItemCount();
}

bool CIMEIInstance::GetItemByIndex(int i_index, char *sz_name, int i_size)
{
	return m_pITool->GetTestItemByIndex(i_index, sz_name, i_size);
}

bool CIMEIInstance::PreRun(int i_slot/* = 1 */)
{
	if( i_slot !=  0) return true;

	m_strIMEIflag = false;  //figure out L30 in pics is ture or flase

	bool b_Res = false;

	m_pITool->KillADBThread();

	// Set log path and file name
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	m_currTime.Format(_T("%04d%02d%02d_%02d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	SetLogFileName();

	m_pITool->TraceLog(MSG_INFO, _T("------------------------------------ Begin ----------------------------------------"));

	// Create phone object
	b_Res = m_pITool->PhoneCreateObject();
	if ( !b_Res)
	{
		m_pITool->GetTestResult(0, &st_Result);
		Fire(UI_RESULT, (long)&st_Result);
		return false;
	}

	// Initial PC port
	if (b_Res)
	{
		if (! (b_Res = m_pITool->InitialPC()))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
			return false;
		}
	}


	// Wait phone starting up and change mode to FTD if necessary
	if (b_Res)
	{
		if (! (b_Res = m_pITool->IsConnected()))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
			return false;
		}
	}

	// Check if phone is in FTD mode
/*	if (b_Res)
	{
		if (! (b_Res = m_pITool->IsMobileInFTDMode()))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
		}
	}
*/
	// Check if socket is ready
	if (b_Res)
	{
		if (! (b_Res = m_pITool->IsSocketReady()))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
		}
	}

	if (!b_Res)
	{
		if(m_pITool->IsConnected())	m_pITool->ExecAndroidLogcat(false);

		CString cstrErrorHandling = _T("CErrorHandling");
		CToolRuntimeClass* pcObjectFactory = RF_RUNTIME_CLASS(CToolBaseObject);
		CToolBaseObject* pcObject = NULL;

		CT2A szErrorHandling(cstrErrorHandling);
		std::string strErrorHandling(szErrorHandling);
		m_pIErrorHandling = NULL;
		if ((pcObject = pcObjectFactory->CreateObject(strErrorHandling)) != NULL)
		{
			m_pIErrorHandling = dynamic_cast<IErrorHandling*>(pcObject);
		}
		if(m_pIErrorHandling != NULL)
		{
			m_pIErrorHandling->SearchPortUsb(5);
			m_pIErrorHandling->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
			m_pIErrorHandling = NULL;
		}
	}

	return b_Res;
}

bool CIMEIInstance::Run(int i_slot /* = 1 */)
{
	if( i_slot !=  0) return true;

	bool b_Res = true;
	char sz_testItem[MAX_BUFFER_SIZE] = {0};

	// 1.do begin function before all test item //
	if ( !(b_Res = m_pITool->Begin()))
	{
		m_pITool->GetTestResult(0, &st_Result);
		Fire(UI_RESULT, (long)&st_Result);
	}

	// Load IMEI parameters
	if (b_Res)
	{
		if (! (b_Res = m_pITool->LoadIMEIParams()))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
			return false;
		}
	}

	// 2.run all test items //
	if (b_Res)
	{
		int i_testItemCount = m_pITool->GetTestItemCount();
		for (int i_itemIdx = 0; ((b_Res) && (i_itemIdx < i_testItemCount)); ++i_itemIdx)
		{
			b_Res = m_pITool->GetTestItemByIndex(i_itemIdx, sz_testItem, MAX_BUFFER_SIZE);

			// Notify UI now test item name //
			if (b_Res)
			{
				st_UIItem st_curTestItem ;
				st_curTestItem.i_slot = 1;
				sprintf_s(st_curTestItem.sz_item, MAX_BUFFER_SIZE, "%s", sz_testItem);

				Fire(UI_ITEM, (long)&st_curTestItem);
			}

			// run each test item sequence //
			if (b_Res)
			{
				b_Res = m_pITool->PreRun(i_itemIdx);
			}

			if (b_Res)
			{
				b_Res = m_pITool->Run(i_itemIdx);
			}
			m_pITool->PostRun(i_itemIdx);
		}
	}

	// 5.Exec adb logcat //
	m_pITool->PullDeviceLogOut(b_Res);

	// 7.write common PICS data //
	WritePICSdata();

	return b_Res;
}

bool CIMEIInstance::PostRun(int i_slot /* = 1 */)
{
	if( i_slot !=  0) return true;

	m_pITool->UpdateIMEIParams("LAST_WRITE_IMEI");

	// 2. run end function //
	if ( !m_pITool->End())
	{
		m_pITool->GetTestResult(0, &st_Result);
		Fire(UI_RESULT, (long)&st_Result);
	}

	// 3. Clear DLL data //
	m_str_picasso = _T("");
	m_str_IMEI_FromScanner = _T("");

	// 4. Upload Log to server //
	char szModulePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);
	char Path_UpLog_Bat[100];
	sprintf(Path_UpLog_Bat, _T("%s\\Qisda\\UpLog.bat"), szModulePath);

	if (::_taccess( Path_UpLog_Bat, 0 ) == 0) m_pITool->LogUploadByBAT(m_str_modelName.GetBuffer());
	else m_pITool->LogUpload(m_str_modelName.GetBuffer());
	m_str_modelName.ReleaseBuffer();
	return true;

}

bool CIMEIInstance::Stop(int i_slot /* = 1 */)
{
	return true;
}

bool CIMEIInstance::Begin(int i_slot /* = 1 */)
{
	bool b_Res = false;
	m_str_result = _T("");
	m_str_errorCode = _T("");
	m_str_message = _T("");

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	m_currTime.Format(_T("%04d%02d%02d_%02d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	SetLogFileName();

	// Check if Config.xml file exist
	b_Res = CheckConfigXMLExist();

	// Check if TestItem.xml file exist
	m_cstrTestItemFile.Format(_T("%s_%s_TestItem.xml"), m_str_modelName, m_str_station);
	if (b_Res)	b_Res = CheckTestItemXMLExist();

	// Load tool information
	if (b_Res)	b_Res = LoadToolInfo();

	if ( !b_Res)	UpdateMsg(i_slot);


	return b_Res;
}

bool CIMEIInstance::End(int i_slot /* = 1 */)
{
	return true;
}
bool CIMEIInstance::ResopnseToJig(bool b_wait)
{

	return true;
}

bool CIMEIInstance::ResopnseToShowDlg(char *sz_showReslut)
{
	bool b_response;
	std::string str_temp = sz_showReslut;
	b_response = CStr::StrToBool(str_temp);
	m_pITool->SetDialogResponse(b_response);
	return true;
/*
	if ( strcmp(sz_showReslut,"0") != 0 && strcmp(sz_showReslut,"1") != 0 )	return false;
	else	m_str_UIresponse = sz_showReslut;

	return true;
*/
}


bool CIMEIInstance::CheckConfigXMLExist()
{
	m_cstrConfigFile.Format(_T("C:\\PE_Config\\%s\\Setting\\%s_%s_Config.xml"), m_str_modelName, m_str_modelName, m_str_station);

	bool bRes = false;
	if (::_taccess(m_cstrConfigFile, 0) == 0)
	{
		if (m_ConfigXML.Load(m_cstrConfigFile) == ERROR_SUCCESS)	bRes = true;
		else														TraceLog("Fail to load config XML");
	}
	else
	{
		TraceLog("Fail to access config XML");
	}

	//if (bRes)	TraceLog("Check config XML file SUCCESS");

	return bRes;
}


bool CIMEIInstance::CheckTestItemXMLExist()
{
	bool bRes = false;
	char szModulePath[MAX_PATH] = {0};
	CString cstrTestItemXML = _T("");

	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);

	cstrTestItemXML.Format(_T("%s\\Qisda\\%s"), szModulePath, m_cstrTestItemFile);

	if (::_taccess(cstrTestItemXML, 0) == 0)
	{
		m_cstrTestItemFile = cstrTestItemXML;

		if (m_TestItemXML.Load(m_cstrTestItemFile) == ERROR_SUCCESS)
		{
			bRes = true;
		}
		else
		{
			m_str_result = _T("FAIL");
			m_str_errorCode = _T("");
			m_str_message = _T("Load test item xml fail!");
		}
	}
	else
	{
		m_str_result = _T("FAIL");
		m_str_errorCode = _T("");
		m_str_message = _T("Can not find test item xml!");
	}

	return bRes;
}


bool CIMEIInstance::LoadToolInfo()
{
	bool bRes = false;
	CString cstrTestProcessor = _T("");
	CToolRuntimeClass* pcObjectFactory = RF_RUNTIME_CLASS(CToolBaseObject);
	CToolBaseObject* pcObject = NULL;

	// 1. Load every sub-element of tag ToolInfo
	XMLNodeList toolInfoNodeList = NULL;
	toolInfoNodeList = m_TestItemXML.GetChildNodes(_T("//Configuration//ToolInfo"));
	if (toolInfoNodeList != NULL)
		bRes = true;

	for (UINT i = 0; (bRes && (i < m_TestItemXML.GetNodeListSize(toolInfoNodeList))); ++i)
	{
		XMLNode toolInfoNode = m_TestItemXML.GetNodeListItem(toolInfoNodeList, i);
		if (toolInfoNode != NULL)
			bRes = true;
		else
			bRes = false;

		CString cstrNodeName = m_TestItemXML.GetNodeName(toolInfoNode);
		CString cstrNodeText = m_TestItemXML.GetNodeText(toolInfoNode);

		if (cstrNodeName.IsEmpty() != true)
		{
			if (cstrNodeName.CompareNoCase(_T("StationID")) == 0)
			{
				CT2A szStationID(cstrNodeText);
				m_strStationID = szStationID;
				continue;
			}
			else if (cstrNodeName.CompareNoCase(_T("TOOL_VERSION")) == 0)
			{
				CT2A szToolVersion(cstrNodeText);
				m_strToolVersion = szToolVersion;

				continue;
			}

			CT2A pszNodeName(cstrNodeName);
			std::string strNodeName(pszNodeName);
			m_vToolInfo.push_back(strNodeName);

			if (cstrNodeText.IsEmpty() != true)
			{
				CT2A pszNodeText(cstrNodeText);
				std::string strNodeText(pszNodeText);
				m_mapToolInfo[strNodeName] = strNodeText;
			}
			else
				m_mapToolInfo[strNodeName] = "-";
		}
	}

	// 2. Load test processor name and new the processor object
	if (bRes)
	{
		XMLNode toolProcessor = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Name"));
		if (toolProcessor != NULL)
			cstrTestProcessor = m_TestItemXML.GetNodeText(toolProcessor);
		else
			bRes = false;

		if (bRes)
		{
			CT2A szTestProcessor(cstrTestProcessor);
			std::string strTestProcessor(szTestProcessor);

			if ((pcObject = pcObjectFactory->CreateObject(strTestProcessor)) != NULL)
			{
				m_pITool = dynamic_cast<ITestProcessor*>(pcObject);
				m_pITool->SetTestItemXML(m_cstrTestItemFile);
				m_pITool->SetConfigXML(m_cstrConfigFile);
				m_pITool->Register(this, TRACE_LOG);
				m_pITool->Register(this, FACTORY_LOG);
				m_pITool->Register(this, PICS_DATA);
				m_pITool->Register(this, SHOW_PICTURE);
				m_pITool->Register(this, JIG_REQUEST);
				m_pITool->Register(this, SHOW_DIALOG);
			}
			else
			{
				bRes = false;
			}
		}
	}

	// 3. Load test items
	if (bRes)
		bRes = m_pITool->LoadTestITems();

	return bRes;
}


bool CIMEIInstance::UpdateMsg (int i_index)
{
	st_Result.i_slot = i_index;
	sprintf_s(st_Result.sz_time, MAX_BUFFER_SIZE, "%s","" );
	sprintf_s(st_Result.sz_result, MAX_BUFFER_SIZE, "%d",m_i_result);
	sprintf_s(st_Result.sz_itemCode, MAX_BUFFER_SIZE, "%s", "");
	sprintf_s(st_Result.sz_errCode, MAX_BUFFER_SIZE, "%s", m_str_errorCode.GetString());
	sprintf_s(st_Result.sz_band, MAX_BUFFER_SIZE, "%s", "");
	sprintf_s(st_Result.sz_channel, MAX_BUFFER_SIZE, "%s", "");
	sprintf_s(st_Result.sz_upper, MAX_BUFFER_SIZE, "%s", "");
	sprintf_s(st_Result.sz_lower, MAX_BUFFER_SIZE, "%s", "");
	sprintf_s(st_Result.sz_measured, MAX_BUFFER_SIZE, "%s", "");
	sprintf_s(st_Result.sz_unit, MAX_BUFFER_SIZE, "%s","");
	sprintf_s(st_Result.sz_elapse, MAX_BUFFER_SIZE, "%s","");
	int nLen = m_str_message.GetLength();
	sprintf_s(st_Result.sz_msg, MAX_BUFFER_SIZE, "%s", m_str_message.GetString());

	Fire(UI_RESULT, (long)&st_Result);

	TraceLog(m_str_message);

	return true;
}

void CIMEIInstance::TraceLog(CString str_log, const int i_severity)
{
	m_GLog.Log(i_severity, str_log);
}

void CIMEIInstance::SetLogFileName()
{
	if (m_str_IMEI_FromScanner.IsEmpty())	m_str_IMEI_FromScanner = _T("RD");
	m_GLog.SetGLogFileName_Info(MakeStr("%s_%s.log", m_str_IMEI_FromScanner, m_currTime).c_str());
}

void CIMEIInstance::WritePICSdata(void)
{   
	st_PICSData sPICS;

	if(m_str_station == _T("IMEI"))
	{
		strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("id"));
		sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"), m_str_IMEI_FromScanner.GetBuffer());
		Fire(UI_PICSDATA,(long)&sPICS);
	}

	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("NVCheck"));
	sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"), map_strData[_T("NVCheck_value")]);
	Fire(UI_PICSDATA,(long)&sPICS);

	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("NAL"));
	sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"), m_str_NAL_FromScanner.GetBuffer());
	Fire(UI_PICSDATA,(long)&sPICS);

	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("PICASSO"));
	sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"), m_str_picasso.GetBuffer());
	Fire(UI_PICSDATA,(long)&sPICS);

	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("IMEI"));
	sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"), m_str_IMEI_FromScanner.GetBuffer());
	Fire(UI_PICSDATA,(long)&sPICS);

	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("PART_NUMBER"));
	sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"), m_str_PartNumber.GetBuffer());
	Fire(UI_PICSDATA,(long)&sPICS);

	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("HW_ID"));
	sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"), m_str_hwID.GetBuffer());
	Fire(UI_PICSDATA,(long)&sPICS);

	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("HW_VER"));
	sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"), m_str_hwID.GetBuffer());
	Fire(UI_PICSDATA,(long)&sPICS);

	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("SW_VERSION"));
	sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"), m_str_SWVer.GetBuffer());
	Fire(UI_PICSDATA,(long)&sPICS);

	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("ME_VER"));
	sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"), m_str_meVer.GetBuffer());
	Fire(UI_PICSDATA,(long)&sPICS);

	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("PCBA_VER"));
	sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"), m_str_pcbaVer.GetBuffer());
	Fire(UI_PICSDATA,(long)&sPICS);

	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("TOOL_VERSION"));
	sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"), m_strToolVersion.c_str());
	Fire(UI_PICSDATA,(long)&sPICS);

	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("SERVICETAG_TAG"));
	sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"), map_strData[_T("SERVICE_TAG")]);
	Fire(UI_PICSDATA,(long)&sPICS);

	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("SIM_LOCK"));
	CString csPicsLimitedValue;
	csPicsLimitedValue.Format(_T("%s"), map_strData[_T("lock_filename")]);
	csPicsLimitedValue = csPicsLimitedValue.Mid( 0, 50);
	sprintf_s(sPICS.sz_value, MAX_BUFFER_SIZE, _T("%s"), csPicsLimitedValue);
	Fire(UI_PICSDATA,(long)&sPICS);
	
 	strcpy_s(sPICS.sz_key,MAX_BUFFER_SIZE,_T("FGSN"));
	sprintf_s(sPICS.sz_value,MAX_BUFFER_SIZE,_T("%s"),map_strData[_T("FGSN")]);
	Fire(UI_PICSDATA,(long)&sPICS);

	return;
}
