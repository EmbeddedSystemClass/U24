// UnifyDLL_FTWireless.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "UnifyDLL_FTWireless.h"
#include "./CommonUtility/INI/IniAccess.h"
#include "../Common/CommonUtil/Win32SHELLUtility.h"
#include <Shlwapi.h>
#include <io.h>

#if (_WIN32_WINNT >= 0x0600)
#import "msxml6.dll"
#else
#import "msxml2.dll"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------- The extern functions ------------------

#ifdef _ToolInterface
IDL_API bool NewToolInterfaceNotify(IToolInterface_Notify** ppIToolInterface)
#else
FT_WIRELESS_INSTANCE_API bool NewInterface(IFacTestToolInterface** ppIToolInterface)
#endif
{
	bool bRes = false;

	if (!(*ppIToolInterface))
	{
		*ppIToolInterface = new CUnifyDLL_FTWireless;
		bRes = true;
	}

	CUnifyDLL_FTWireless* pToolInstance = dynamic_cast<CUnifyDLL_FTWireless*>(*ppIToolInterface);

	if (!bRes && pToolInstance != NULL)
	{
		delete pToolInstance;
		*ppIToolInterface = NULL;
	}

	return bRes;
}

#ifdef _ToolInterface
IDL_API bool __cdecl DeleteToolInterfaceNotify(IToolInterface_Notify* pIToolInterface)
#else
FT_WIRELESS_INSTANCE_API bool DeleteInterface(IFacTestToolInterface* pIToolInterface)
#endif
{
	if (pIToolInterface)
	{
		delete (CUnifyDLL_FTWireless*)pIToolInterface;
		pIToolInterface = NULL;
	}

	return true;
}


// ------------ Setting files related functions ------------

bool CUnifyDLL_FTWireless::LoadSettingFile()
{
	std::string strTmp;
	CIniAccess SetupIni("Setup");

	if (! SetupIni.IsIniExist())
		return false;

	if (! SetupIni.GetValue("CableLoss", strTmp))
		return false;

	m_cstrCableLossFile.Format(_T("%s"), strTmp.c_str());

	if (! SetupIni.GetValue("TestItem", strTmp))
		return false;

	m_cstrTestItemFile.Format(_T("%s"), strTmp.c_str());

	return true;
}

bool CUnifyDLL_FTWireless::GetXMLFileName()
{
	m_cstrCableLossFile = _T("CableLoss.xml");

	m_cstrDeviceItemFile.Format(_T("%s_%s_DeviceItem.xml"), m_str_modelName, m_str_station);

	if (m_str_ToolMode.CompareNoCase(_T("RD")) != 0)
	{
		m_cstrTestItemFile.Format(_T("%s_%s_TestItem.xml"), m_str_modelName, m_str_station);
	}
	else
	{
		m_cstrTestItemFile.Format(_T("%s_%s_TestItem_RD.xml"), m_str_modelName, m_str_station);
	}

	return true;
}

//bool CUnifyDLL_FTWireless::CheckCableLossXMLExist()
//{
//	bool bRes = false;
//	char szModulePath[MAX_PATH] = {0};
//	CString cstrConfigXML = _T("");
//
//	GetModuleFileName(NULL, szModulePath, MAX_PATH);
//	PathRemoveFileSpec(szModulePath);
//#ifndef _ToolInterface
//	cstrConfigXML.Format(_T("%s\\Qisda\\%s"), szModulePath, m_cstrCableLossFile);
//#else
//	cstrConfigXML.Format(_T("%sQisda\\%s"), m_str_ToolWorkDirectory, m_cstrCableLossFile);
//#endif
//	if (::_taccess(cstrConfigXML, 0) == 0)
//	{
//		m_cstrCableLossFile = cstrConfigXML;
//
//		if (m_CableLossXML.Load(m_cstrCableLossFile) == ERROR_SUCCESS)
//		{
//			bRes = true;
//		}
//		else
//		{
//			bRes = false;
//			m_str_result = _T("FAIL");
//			m_str_errorCode = _T("");
//			m_str_message = _T("Load cable loss xml fail!");
//		}
//	}
//	else
//	{
//		bRes = false;
//		m_str_result = _T("FAIL");
//		m_str_errorCode = _T("");
//		m_str_message = _T("Can not find cable loss xml!");
//	}
//
//	return bRes;
//}

bool CUnifyDLL_FTWireless::CheckDeviceItemXMLExist()
{
	bool bRes = false;
	char szModulePath[MAX_PATH] = {0};
	CString cstrDeviceXML = _T("");

	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);
#ifndef _ToolInterface
	cstrDeviceXML.Format(_T("%s\\Qisda\\%s"), szModulePath, m_cstrDeviceItemFile);
#else
	cstrDeviceXML.Format(_T("%s\\Qisda\\%s"), m_str_ToolWorkDirectory, m_cstrDeviceItemFile);
#endif
	if (::_taccess(cstrDeviceXML, 0) == 0)
	{
		m_cstrDeviceItemFile = cstrDeviceXML;

		if (m_DeviceItemXML.Load(m_cstrDeviceItemFile) == ERROR_SUCCESS)
		{
			bRes = true;
		}
		else
		{
			bRes = false;
			m_str_result = _T("FAIL");
			m_str_errorCode = _T("");
			m_str_message = _T("Load device item xml fail!");
		}
	}
	else
	{
		bRes = false;
		m_str_result = _T("FAIL");
		m_str_errorCode = _T("");
		m_str_message = _T("Can not find device item xml!");
	}

	return bRes;
}

bool CUnifyDLL_FTWireless::CheckTestItemXMLExist()
{
	bool bRes = false;
	char szModulePath[MAX_PATH] = {0};
	CString cstrRFXML = _T("");

	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);
#ifndef _ToolInterface
	cstrRFXML.Format(_T("%s\\Qisda\\%s"), szModulePath, m_cstrTestItemFile);
#else
	cstrRFXML.Format(_T("%s\\Qisda\\%s"), m_str_ToolWorkDirectory, m_cstrTestItemFile);
#endif
	if (::_taccess(cstrRFXML, 0) == 0)
	{
		m_cstrTestItemFile = cstrRFXML;

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

// ------------ MD5 verify functions ------------

bool CUnifyDLL_FTWireless::CheckTestItemXMLMD5()
{
	char sz_currentPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
	::PathRemoveFileSpec(sz_currentPath);

	CString str_MD5LocalPath;
	str_MD5LocalPath.Format("%s\\MD5\\%s_%s_TestItem_MD5.xml", sz_currentPath, m_str_modelName, m_str_station);
	if (_taccess(str_MD5LocalPath, 0) != 0)
	{
		m_str_result = _T("FAIL");
		m_str_errorCode = _T("");
		m_str_message = _T("MD5 xml file of test item is not exist!");
		return false;
	}

	/* Replace tool path of MD5 xml */
	if (ConvertXML(str_MD5LocalPath, m_cstrTestItemFile) != true)
	{
		m_str_result = _T("FAIL");
		m_str_errorCode = _T("");
		m_str_message = _T("Replace tool path of MD5 xml fail!");
		return false;
	}

	CString str_logName;
	str_logName = "C:\\MD5.log";
	if (MD5Verify(m_cstrTestItemFile, str_MD5LocalPath, str_logName) != true)
	{
		m_str_result = _T("FAIL");
		m_str_errorCode = _T("");
		m_str_message = _T("Verify MD5 fail!");
		return false;
	}

	if (CheckVerifyResult(str_logName) != true)
	{
		m_str_result = _T("FAIL");
		m_str_errorCode = _T("");
		m_str_message = _T("Check test item xml MD5 fail.\nTest item xml may be changed, Please check it!");
		return false;
	}

	return true;
}

bool CUnifyDLL_FTWireless::ConvertXML(CString str_XMLFilePathName, CString str_newName)
{
	/* Check input */
	if(str_XMLFilePathName == _T("") || str_newName == _T(""))
	{
		return false;
	}

	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr;
	hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
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

bool CUnifyDLL_FTWireless::MD5Verify(CString str_dirPath, CString str_XMLFilePathName, CString str_logFile)
{
	/* Check Input */
	if(str_dirPath == _T("") || str_XMLFilePathName == _T("")  || str_logFile == _T(""))
	{
		return false;
	}

	/* Check fciv.exe exist */
	char sz_currentPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
	::PathRemoveFileSpec(sz_currentPath);
	CString str_fcivPathName;
	str_fcivPathName.Format(_T("%s\\MD5\\fciv.exe"), sz_currentPath);
	if (_taccess(str_fcivPathName, 0) != 0)
	{
		return false;
	}

	/* Command */
	CString str_command;
	CString str_commandOne;
	CString str_commandTwo;
	CString str_commandThree;
	str_commandOne = _T("cmd.exe /c ");
	str_commandTwo = str_fcivPathName + _T(" -r -v ") + str_dirPath + _T(" -xml ") + str_XMLFilePathName;
	str_commandThree = str_commandThree + _T(" > ") + str_logFile;
	str_command = str_commandOne + str_commandTwo + str_commandThree;

	/* Run */
	TCHAR sz_commandLine[1024];
	memset(sz_commandLine, 0, sizeof(sz_commandLine));
	for (int i = 0; i < str_command.GetLength(); i ++)
	{
		sz_commandLine[i] = str_command[i];
	}
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;

	BOOL b_createRes = FALSE;
	b_createRes = CreateProcess(NULL, sz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
	if(b_createRes == FALSE)
	{
		return false;
	}
	DWORD dw_waitResult = 0;
	DWORD dw_timeout;
	dw_timeout = 10000;
	dw_waitResult = ::WaitForSingleObject(pi.hProcess, dw_timeout);
	switch(dw_waitResult)
	{
	case WAIT_FAILED:
		::CloseHandle(pi.hProcess);
		pi.hProcess = NULL;
		return false;

	case WAIT_TIMEOUT:
		::CloseHandle(pi.hProcess);
		pi.hProcess = NULL;
		return false;

	case WAIT_OBJECT_0:
		break;
	}
	::CloseHandle(pi.hProcess);
	pi.hProcess = NULL;

	return true;
}

bool CUnifyDLL_FTWireless::CheckVerifyResult(CString str_logFile)
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

bool CUnifyDLL_FTWireless::LoadToolInfo()
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
			//else if (cstrNodeName.CompareNoCase(_T("ToolVersion")) == 0)
			else if (cstrNodeName.CompareNoCase(_T("TOOL_VERSION")) == 0) //  AutoBuild need the same name , Eason 
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
				m_pITool->SetCableLossXML(m_cstrCableLossFile);
				m_pITool->SetDeviceItemXML(m_cstrDeviceItemFile);
				m_pITool->SetTestItemXML(m_cstrTestItemFile);
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


// ---------------- Constructor & Destructor ---------------

CUnifyDLL_FTWireless::CUnifyDLL_FTWireless()
{
	m_str_result = _T("");
	m_str_errorCode = _T("");
	m_str_message = _T("");

	DefineNotify(UI_PROGRESS);
	DefineNotify(UI_MESSAGE);
	DefineNotify(UI_PATTERN);
	DefineNotify(UI_ITEM);
	DefineNotify(UI_RESULT);
	DefineNotify(UI_PICSDATA);
	DefineNotify(UI_JIGCONTROL);
	DefineNotify(UI_SHOWDIALOG);
	DefineNotify(UI_RELAYBOARD);
	DefineNotify(UI_POWERSUPPLY);
	DefineNotify(UI_SHOWIMGDIALOG);

	::AfxOleInit();
	::CoInitialize(NULL);
}

CUnifyDLL_FTWireless::~CUnifyDLL_FTWireless()
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


/******************* FT DLL Interface functions ***************************/

// --------------------- Get test items --------------------

int CUnifyDLL_FTWireless::GetItemCount()
{
	return m_pITool->GetTestItemCount();
}

bool CUnifyDLL_FTWireless::GetItemByIndex(int iIndex, char* szName, int iSize)
{
	return m_pITool->GetTestItemByIndex(iIndex, szName, iSize);
}


// --------------------- Test sequence ---------------------
#ifdef _ToolInterface
bool CUnifyDLL_FTWireless::OnAPPLaunch(int i_slot)

#else
bool CUnifyDLL_FTWireless::Begin(int i_slot)
#endif
{
#ifndef _ToolInterface
	if( i_slot !=  0) return true;
#endif

	bool b_Res = false;
	m_str_result = _T("");
	m_str_errorCode = _T("");
	m_str_message = _T("");

	/* Set log path and file name */
	SetLogFilePath();
	SetLogFileName();

	/* 1. Load FT_Wireless_Instance.ini */
	if (!(b_Res = GetXMLFileName() ) ){
		AfxMessageBox("GetXMLFileName Fail");
		goto Exit_ShowResult;
	}

	if (!(b_Res = CheckDeviceItemXMLExist() ) ){
		AfxMessageBox("CheckDeviceItemXMLExist Fail");
		goto Exit_ShowResult;
	}

	if (!(b_Res = CheckTestItemXMLExist() ) ){
		AfxMessageBox("CheckTestItemXMLExist Fail");
		goto Exit_ShowResult;
	}



	/* 2. Check if CableLoss.xml file exist */
	//if (b_Res)
	//{
	//	b_Res = CheckCableLossXMLExist();
	//}
	//else
	//{
	//	AfxMessageBox("GetXMLFileName Fail");
	//}

	/* 3. Check if DeviceItem.xml file exist */
	//if (b_Res)
	//{
	//	b_Res = CheckDeviceItemXMLExist();
	//}
	//else
	//{
	//	AfxMessageBox("GetXMLFileName Fail");
	//}

	/* 4. Check if TestItem.xml file exist */
	//if (b_Res)
	//{
	//	b_Res = CheckTestItemXMLExist();
	//}else
	//{
	//	AfxMessageBox("CheckDeviceItemXMLExist Fail");
	//}

	/* 5. TestItem.xml file MD5 check if load factory test item file */
	if (b_Res)
	{
		if (m_str_ToolMode.CompareNoCase(_T("RD")) != 0)
		{
			//b_Res = CheckTestItemXMLMD5();
			//if (!(b_Res = CheckTestItemXMLMD5() ) ){
			//	AfxMessageBox("CheckTestItemXMLMD5 Fail");
			//	goto Exit_ShowResult;
			//}

		}
	}else
	{
		AfxMessageBox("CheckTestItemXMLExist Fail");
	}

	if (!(b_Res = LoadToolInfo() ) ){
		AfxMessageBox("LoadToolInfo Fail");
		goto Exit_ShowResult;
	}

	/* 6. Load tool information */
	//if (b_Res)
	//{
	//	b_Res = LoadToolInfo();
	//
	//}
	//else
	//{
	//		AfxMessageBox("CheckTestItemXMLMD5 Fail");
	//}

	/* 7. Detect GPIB device and create related device object */
	if (b_Res)
	{
		b_Res = m_pITool->GPIBDeviceCreateObject();
	
	}else{
			AfxMessageBox("LoadToolInfo FAil");
	}

	if ( !b_Res)
	{
		AfxMessageBox("GPIBDeviceCreateObject FAIL");
		sprintf_s(st_Result.sz_result, MAX_BUFFER_SIZE - 1, "%s", m_str_result);
		sprintf_s(st_Result.sz_msg, MAX_BUFFER_SIZE - 1, "%s", m_str_message);

		Fire(UI_RESULT, (long)&st_Result);
	}

Exit_ShowResult:
	/* Delete log files */
	//char str_LocalPath[1000] = "D:\\Log";
	//m_pITool->DFSDeleteFile(str_LocalPath, _T(""));

	return b_Res;
}

#ifdef _ToolInterface
bool CUnifyDLL_FTWireless::SetCableloss(const wchar_t *szType, const wchar_t *szBand, const wchar_t *szChannel, const wchar_t *szLoss)
{
	return true; //this->m_pITool->SetCableLoss(CW2A(szType), CW2A(szBand), CW2A(szChannel), CW2A(szLoss));
}

bool CUnifyDLL_FTWireless::SetTestPart(IToolInterface::ToolTestParts PartID, const wchar_t *szName, const wchar_t *szItem, const wchar_t *szContent, const wchar_t *szAttribute)
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

bool CUnifyDLL_FTWireless::SetTestItem(const wchar_t *szItem)
{
	return true; //this->m_pITool->SetTestItem(CW2A(szItem));
}

bool CUnifyDLL_FTWireless::InitTestItem(const wchar_t *szItem)
{
	return true;
}
#endif

#ifdef _ToolInterface
bool CUnifyDLL_FTWireless::OnRunInit(int i_slot)
#else
bool CUnifyDLL_FTWireless::PreRun(int i_slot)
#endif
{

#ifndef _ToolInterface
	if( i_slot !=  0) return true;
#endif

	bool b_Res = false;

	m_pITool->KillADBThread();

	///* Set log file name again in every run */
	//SetLogFileName();

	/* 2. Initial Relay Board */
	//if ( !(b_Res = m_pITool->InitialRelayBoard()))
	//{
	//	m_pITool->GetTestResult(0, &st_Result);
	//	Fire(UI_RESULT, (long)&st_Result);
	//	return false;
	//}

	/* 3. Create phone object */
	//if (b_Res)
	//{
		if ( !(b_Res = m_pITool->PhoneCreateObject()))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
			return false;
		}
//	}

	/* 4. Initial PC port */
	if (b_Res)
	{
		if (! (b_Res = m_pITool->InitialPC()))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
			return false;
		}
	}

	/* 5. Set PowerSupply voltage and current if exist */

	if (b_Res)
	{
		if ( !(b_Res = m_pITool->InitialPowerSupply()))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
			return false;
		}
	}

	// 20141202 Eason Remove USB4702
	/* 6. Initial USB4702 */  //120517 for detroit by cy.lee; 120822 Fix by cy
	//if ( !(b_Res = m_pITool->InitialUSB4702()))
	//{
	//	m_pITool->GetTestResult(0, &st_Result);
	//	Fire(UI_RESULT, (long)&st_Result);
	//	return false;
	//}

	/* 7. Wait phone starting up and change mode to FTD if necessary */
	if (b_Res)
	{
		if (! (b_Res = m_pITool->IsConnected()))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
			return false;
		}
	}

#ifdef _ToolInterface
	if (b_Res)
	{
		char sz_sectorData[512] = {0};
		std::string st_sectorData;
		TRACE("msg=%s",m_str_station);

		if (! (b_Res = m_pITool->CsdOpenFtd()))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
			return false;
		}
		Sleep(1000);


		if (!m_pITool->ReadFAData_New( 0, 0, sz_sectorData, 200))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
			return false;
		}

		st_sectorData = sz_sectorData;
		StrVtr vToken;
		CStr::ParseString(st_sectorData.c_str(), _T(","), vToken);
		m_pITool->SetPicasso( vToken[2].c_str());
		m_str_picasso = vToken[2].c_str();
		/* Set google log file name again if Picasso is not empty*/
		if ( !m_str_picasso.IsEmpty())
		{
			SetLogFileName();
		}

	}

#endif
	/* 8. Check if phone is in FTD mode */
/*	if (b_Res)
	{
		if (! (b_Res = m_pITool->IsMobileInFTDMode()))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
		}
	}
*/
	/* 9. Check if socket is ready */
	if (b_Res)
	{
		if (! (b_Res = m_pITool->IsSocketReady()))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
		}
	}

	if (!b_Res) {
		if(m_pITool->IsConnected()){
			m_pITool->ExecAndroidLogcat(false);
		}

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

#ifdef _ToolInterface
bool CUnifyDLL_FTWireless::RunTestItem(int iItem, int i_slot)
#else
bool CUnifyDLL_FTWireless::Run(int i_slot)
#endif
{

#ifndef _ToolInterface
	if( i_slot !=  0) return true;
#endif

	bool b_Res = true;
	char sz_testItem[MAX_BUFFER_SIZE];
	::memset(sz_testItem, 0, MAX_BUFFER_SIZE);

	/* 1.do begin function before all test item */
	if ( !(b_Res = m_pITool->Begin()))
	{
		m_pITool->GetTestResult(0, &st_Result);
		Fire(UI_RESULT, (long)&st_Result);
	}

	/* 2.run all test items */
	if (b_Res)
	{
		int i_testItemCount = m_pITool->GetTestItemCount();
		for (int i_itemIdx = 0; ((b_Res) && (i_itemIdx < i_testItemCount)); ++i_itemIdx)
		{
			b_Res = m_pITool->GetTestItemByIndex(i_itemIdx, sz_testItem, MAX_BUFFER_SIZE);

			/* Notify UI now test item name */
			if (b_Res)
			{
				st_UIItem st_curTestItem ;
				st_curTestItem.i_slot = 1;
				sprintf_s(st_curTestItem.sz_item, MAX_BUFFER_SIZE, "%s", sz_testItem);

				Fire(UI_ITEM, (long)&st_curTestItem);
			}

			/* run each test item sequence */
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

	/* 3.if test fail, save QXDM log to PC */
	//if (! b_Res)
	//{
	//	if (m_pITool->IsMobileInFTDMode())
	//	{
	//		m_pITool->CopyQXDMMobileLogToPC();
	//	}
	//}


	/* 4.if all items test pass, notify UI finally result 'PASS' */
	//if(b_Res)
	//{
	//	st_Result.i_slot = 1;  //Default: 1
	//	sprintf_s(st_Result.sz_result, MAX_BUFFER_SIZE - 1, "%s", _T("PASS"));
	//	sprintf_s(st_Result.sz_itemCode, MAX_BUFFER_SIZE - 1, "%s", _T("-"));
	//	sprintf_s(st_Result.sz_errCode, MAX_BUFFER_SIZE - 1, "%s", _T("PASS"));
	//	sprintf_s(st_Result.sz_band, MAX_BUFFER_SIZE - 1, "%s", _T("-"));
	//	sprintf_s(st_Result.sz_channel, MAX_BUFFER_SIZE - 1, "%s", _T("-"));
	//	sprintf_s(st_Result.sz_upper, MAX_BUFFER_SIZE - 1, "%s", _T("-"));
	//	sprintf_s(st_Result.sz_lower, MAX_BUFFER_SIZE - 1, "%s", _T("-"));
	//	sprintf_s(st_Result.sz_measured, MAX_BUFFER_SIZE - 1, "%s", _T("-"));
	//	sprintf_s(st_Result.sz_unit, MAX_BUFFER_SIZE - 1, "%s", _T("-"));
	//	sprintf_s(st_Result.sz_msg, MAX_BUFFER_SIZE - 1, "%s", _T("All test Item PASS"));
	//
	//	Fire(UI_RESULT, (long)&st_Result);
	//}

	/* 5.Exec adb logcat */
	m_pITool->PullDeviceLogOut(b_Res);

	/* 6.device lcd show pass or fail */
	//m_pITool->ShowPassFailOnDeviceLCD();

	/* 7.write common PICS data */
	WritePICSdata();

	return b_Res;
}

#ifdef _ToolInterface
bool CUnifyDLL_FTWireless::OnRunFinish(int i_slot)
#else
bool CUnifyDLL_FTWireless::PostRun(int i_slot)
#endif
{
#ifndef _ToolInterface
	if( i_slot !=  0) return true;
#endif
	/* 1. Close USB4702 */
	//if ( !m_pITool->CloseUSB4702())
	//{
	//	m_pITool->GetTestResult(0, &st_Result);
	//	Fire(UI_RESULT, (long)&st_Result);
	//}

	/* 2. run end function */
	if ( !m_pITool->End())
	{
		m_pITool->GetTestResult(0, &st_Result);
		Fire(UI_RESULT, (long)&st_Result);
	}

	/* 3. Clear DLL data */
	m_str_picasso = _T("");

	/* 4. Upload Log to server */
	//char szModulePath[MAX_PATH] = {0};
	//GetModuleFileName(NULL, szModulePath, MAX_PATH);
	//PathRemoveFileSpec(szModulePath);
	//char Path_UpLog_Bat[MAX_PATH];
	//sprintf(Path_UpLog_Bat, _T("%s\\Qisda\\UpLog.bat"), szModulePath);

	//string sz_modelName = m_str_modelName.GetBuffer();
	//char str_modelName[50];
	//strcpy(str_modelName, sz_modelName.c_str());

//	if (::_taccess( Path_UpLog_Bat, 0 ) == 0) m_pITool->LogUploadByBAT(str_modelName);
//	else m_pITool->LogUpload(str_modelName);

	//m_str_modelName.ReleaseBuffer();

	return true;
}

#ifndef _ToolInterface
bool CUnifyDLL_FTWireless::Stop(int i_slot)
{
	// Currently, IToolProcesser does NOT have 'stop' API
	return true;
}
#endif

#ifdef _ToolInterface
bool CUnifyDLL_FTWireless::OnAPPEnd(int i_slot)
#else
bool CUnifyDLL_FTWireless::End(int i_slot)
#endif
{
	return true;
}

// -------------------- Get test result --------------------

bool CUnifyDLL_FTWireless::GetTestResult(int iIndex, st_UIResult* psResult)
{
	return m_pITool->GetTestResult(iIndex, psResult);
}

/************************** Transmit parameter functions ***************************/

#ifndef _ToolInterface
bool CUnifyDLL_FTWireless::SetParameterValue(char* sz_keyword, char* sz_value)
{
	CString str_temp = _T("");

	if (strcmp(sz_keyword, "TOOL_MODE") == 0)
	{
		str_temp.Format(_T("%s"), sz_value);
		m_str_ToolMode = str_temp;
	}
	else if (strcmp(sz_keyword, "PICASSO") == 0)
	{
		m_pITool->SetPicasso(sz_value);
		str_temp.Format(_T("%s"), sz_value);
		m_str_picasso = str_temp;

		char* p_ctoken;
		p_ctoken = strtok(sz_value, "&");
		int nlist = 1;
		while (p_ctoken != NULL)
		{
			m_pITool->SetPicassoMulti(p_ctoken, nlist);
			m_str_picasso_pool[nlist-1].Format(_T("%s"), p_ctoken);
			p_ctoken = strtok(NULL, "&");
			nlist++;
		}

		/* Set google log file name again if Picasso is not empty*/
		if ( !m_str_picasso.IsEmpty())
		{
			SetLogFileName();
		}
	}
	else if (strcmp(sz_keyword, "TAG") == 0)
	{
		m_pITool->SetTag(sz_value);
		str_temp.Format(_T("%s"), sz_value);
		m_str_tag = str_temp;

	}
	else if (strcmp(sz_keyword, "SN") == 0)
	{
		m_pITool->SetSn(sz_value);
		str_temp.Format(_T("%s"), sz_value);
		m_str_sn = str_temp;
	}
	else if (strcmp(sz_keyword, "FACTORY_SO") == 0)
	{
		m_pITool->SetSo(sz_value);
		str_temp.Format(_T("%s"), sz_value);
		m_str_so = str_temp;
	//	AfxMessageBox(m_str_so);
	}
	else if (strcmp(sz_keyword, "FACTORY_LINE") == 0)
	{
		m_pITool->SetLine(sz_value);
		str_temp.Format(_T("%s"), sz_value);
		m_str_line = str_temp;
	}
	else if (strcmp(sz_keyword, "FACTORY_DAYNIGHT") == 0)
	{
		m_pITool->SetDayNight(sz_value);
		str_temp.Format(_T("%s"), sz_value);
		m_str_daynight = str_temp;
	}
	else if (strcmp(sz_keyword, "MODEL_NAME") == 0)
	{
		m_pITool->SetModelName(sz_value);
		str_temp.Format(_T("%s"), sz_value);
		m_str_modelName = str_temp;
	}
	else if (strcmp(sz_keyword, "STATION") == 0)
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
	else if (strcmp(sz_keyword, "SW_VERSION") == 0)
	{
		m_pITool->SetSWVersion(sz_value);
	}
	else if (strcmp(sz_keyword, "FACTORY_VERSION") == 0)
	{
		m_pITool->SetFactoryVersion(sz_value);
	}
	else if (strcmp(sz_keyword, "HW_VERSION") == 0)
	{
		m_pITool->SetHWVersion(sz_value);
	}
	else if (strcmp(sz_keyword, "UI_LOG_PATH") == 0)
	{
		CString str_path;
		str_path.Format(_T("%s"), sz_value);
		//m_GLog.InitializeGLog(str_path);
	}
	else if (strcmp(sz_keyword, "SLOT_AVAILABLE_LIST") == 0)
	{
		char* p_ctoken;
		p_ctoken = strtok(sz_value, ",");
		int nlist = 1;
		while (p_ctoken != NULL)
		{
			m_pITool->SetSlotUsable(p_ctoken, nlist);
			p_ctoken = strtok(NULL, ",");
			nlist++;
		}
	}
	else if (strcmp(sz_keyword, "IMEI") == 0)
	{
		m_pITool->SetIMEI(sz_value);
	}


	//Eason Check from Server S-----------------------------------------------
	//else
	//{
	//	return true;
	//}

	CString str_otherKey,str_otherKeyValue;
	str_otherKey.Format(_T("%s"), sz_keyword);
	str_otherKeyValue.Format(_T("%s"), sz_value);

	if ( str_otherKey.GetLength() < 64 && str_otherKeyValue.GetLength() < 128 )
	{
		if ( !str_otherKey.IsEmpty() && !str_otherKeyValue.IsEmpty() )
		{
			m_pITool->RecordServerPara( std::string(str_otherKey) , std::string(str_otherKeyValue) );
		}
	}
	//Eason Check from Server E-----------------------------------------------

	return true;
}

bool CUnifyDLL_FTWireless::GetParameterValue(char* sz_keyword, char* sz_value, int i_size)
{
	if (strcmp(sz_keyword, "TOOL_VERSION") == 0)
	{
		strcpy_s(sz_value, i_size, m_strToolVersion.c_str());
	}
	return true;
}
#else
bool CUnifyDLL_FTWireless::SetParameterValue(const wchar_t* sz_keyword, const wchar_t* sz_value)
{
	CString str_temp = _T("");

	CString str_key = CW2CT(sz_keyword);
	CString str_value = CW2CT(sz_value);

	if (strcmp(str_key, "TOOL_MODE") == 0)
	{
		str_temp.Format(_T("%s"), str_value);
		m_str_ToolMode = str_temp;
	}
	else if (strcmp(str_key, "PICASSO") == 0)
	{
		m_pITool->SetPicasso(str_value);
		str_temp.Format(_T("%s"), str_value);
		m_str_picasso = str_temp;

		/* Set google log file name again if Picasso is not empty*/
		if ( !m_str_picasso.IsEmpty())
		{
			SetLogFileName();
		}
	}
	else if (strcmp(str_key, "MODEL_NAME") == 0)
	{
		m_pITool->SetModelName(str_value);
		str_temp.Format(_T("%s"), str_value);
		m_str_modelName = str_temp;
	}
	else if (strcmp(str_key, "STATION") == 0)
	{
		m_pITool->SetStation(str_value);
		str_temp.Format(_T("%s"), str_value);
		m_str_station = str_temp;
	}
	else if (strcmp(str_key, "TOOL_VERSION") == 0)
	{
		str_temp.Format(_T("%s"), str_value);
		m_strToolVersion = str_temp;
	}
	else if (strcmp(str_key, "SW_VERSION") == 0)
	{
		m_pITool->SetSWVersion(str_value);
	}
	else if (strcmp(str_key, "FACTORY_VERSION") == 0)
	{
		m_pITool->SetFactoryVersion(str_value);
	}
	else if (strcmp(str_key, "HW_VERSION") == 0)
	{
		m_pITool->SetHWVersion(str_value);
	}
	else if (strcmp(str_key, "UI_LOG_PATH") == 0)
	{
		CString str_path;
		str_path.Format(_T("%s"), str_value);
		m_GLog.InitializeGLog(str_path);
	}
	else if (strcmp(str_key, "IMEI") == 0)
	{
		m_pITool->SetIMEI(str_value);
	}
	/* Tool WORKING_DIR */
	else if (str_key == _T("WORKING_DIR"))
	{
		m_str_ToolWorkDirectory.Format(_T("%s"), str_value);
	}
	else
	{
		return true;
	}

	return true;
}

//bool CUnifyDLL_FTWireless::GetParameterValue(const wchar_t* sz_keyword, wchar_t* sz_value, int i_size)
bool CUnifyDLL_FTWireless::GetParameterValue(const wchar_t* sz_keyword, char* sz_value, int i_size)
{
	CString str_key = CW2CT(sz_keyword);

	if (strcmp(str_key, "TOOL_VERSION") == 0)
	{
		//wcscpy_s(sz_value, i_size, CT2W(m_strToolVersion.c_str()));
		sprintf_s( sz_value , i_size, "%s", m_strToolVersion.c_str());
	}
	if (strcmp(str_key, "SN") == 0)
	{
		return GetSn(sz_value);
	}
	return true;
}
#endif

//FA functions
bool CUnifyDLL_FTWireless::GetFAData(int i_slot, char* sz_value, int i_size)
{
	return false;
}

bool CUnifyDLL_FTWireless::SetFAData(int i_slot, char* sz_value)
{
	return false;
}
 
bool CUnifyDLL_FTWireless::GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize, int i_idType)
{
	TRACE("msg=%s",m_str_station);
	if (!m_pITool->ReadFAData_New( i_slot, i_sectorNum, sz_sectorData, i_sectorSize))
	{
		m_pITool->GetTestResult(0, &st_Result);
		Fire(UI_RESULT, (long)&st_Result);
		return false;
	}

	//if (  m_str_station.Compare("RUN_IN")  == 0 ||
	//	  m_str_station.Compare("WRITE") == 0 ||
	//	  m_str_station.Compare("MMI") == 0 )
   if ( i_idType == 2 )// 1 picasso , 2 dell id 
	{	
		if ( Id.ReadId() ){/*get scalar id ok*/
			m_szId = Id.GetId();
			if(m_szId.empty() || m_szId.length() != ID_SIZE)
			{	
				m_szErrMsg = "get scalar id Fail  = ";//  + std_ScalarId.c_str();
				m_szErrMsg = m_szErrMsg + m_szId.c_str();
				AfxMessageBox( m_szErrMsg.c_str());
			}
			else
			{
				//	m_szId = "a1234567899";
				m_szId = m_szId + ",";
				std::string str_faData = sz_sectorData;
				str_faData.replace(14, 12, m_szId);
				sprintf_s( sz_sectorData , 512, "%s", str_faData.c_str());
			}
		}
	}

	return true;
}

bool CUnifyDLL_FTWireless::SetTag(int i_slot, char *sz_sectorData, int i_sectorSize)
{
	bool b_Res = false;
	char szInput[FTD_BUF_SIZE] = {0} ;

	strcat(szInput, sz_sectorData);

	m_pITool->SetTag(sz_sectorData);
	return b_Res;
}

//bool GetSn( char *sz_Data);
bool CUnifyDLL_FTWireless::GetSn( char *sz_Data)
{
	if (m_pITool->GetSn(sz_Data))	return true;
	return false;
}


bool CUnifyDLL_FTWireless::SetSn(int i_slot, char *sz_sectorData, int i_sectorSize)
{
	bool b_Res = false;
	char szInput[FTD_BUF_SIZE] = {0} ;

	strcat(szInput, sz_sectorData);

	m_pITool->SetSn(sz_sectorData);
	return b_Res;
}


bool CUnifyDLL_FTWireless::SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	bool b_Res = false;
	if (!(b_Res = m_pITool->IsMobileInFTDMode()))
	{
		m_pITool->GetTestResult(0, &st_Result);
		Fire(UI_RESULT, (long)&st_Result);
	}
	if (b_Res)
	{
		char out_buf[FTD_BUF_SIZE] = {0};
		if (!(b_Res = m_pITool->WriteFAData_New(i_sectorNum, sz_sectorData, i_sectorSize, out_buf)))
		{
			m_pITool->GetTestResult(0, &st_Result);
			Fire(UI_RESULT, (long)&st_Result);
		}
	}
	return b_Res;
}

/*************************************** Jig operate functions *****************************/
bool CUnifyDLL_FTWireless::ResopnseToJig(bool b_wait)
{
	if (m_pITool == NULL) return false;
	m_pITool->SetJigRequestInfo(b_wait);
	return true;
}

bool CUnifyDLL_FTWireless::ResopnseToShowDlg(char* sz_showReslut)
{
	bool b_response;
	std::string str_temp = sz_showReslut;
	b_response = CStr::StrToBool(str_temp);
	m_pITool->SetDialogResponse(b_response);
	return true;
}

// --------------------- Notification ----------------------
bool CUnifyDLL_FTWireless::Register(INotify_Char* pNotify, const char* strEvent)
{
	return QSYNC_LIB::CNotifyBase_Char::Register(pNotify, strEvent);
}

bool CUnifyDLL_FTWireless::Register(EventFunc_Char* pEventFunc, const char* strEvent)
{
	return QSYNC_LIB::CNotifyBase_Char::Register(pEventFunc, strEvent);
}

bool CUnifyDLL_FTWireless::Unregister(INotify_Char* pNotify, const char* strEvent)
{
	return QSYNC_LIB::CNotifyBase_Char::Unregister(pNotify, strEvent);
}

bool CUnifyDLL_FTWireless::Unregister(EventFunc_Char* pEventFunc, const char* strEvent)
{
	return QSYNC_LIB::CNotifyBase_Char::Unregister(pEventFunc, strEvent);
}

void CUnifyDLL_FTWireless::DefineNotify(const char* strNewEvent)
{
	QSYNC_LIB::CNotifyBase_Char::DefineNotify(strNewEvent);
}

void CUnifyDLL_FTWireless::Fire(const char* strEvent, long nParam)
{
	return QSYNC_LIB::CNotifyBase_Char::Fire(strEvent, nParam);
}

void CUnifyDLL_FTWireless::Event(const std::string& strEvent, long nParam)
{
	CString str_log;
	if (strEvent == TRACE_LOG)
	{
		sTraceLog* psTraceLog = (sTraceLog*)nParam;
		str_log = psTraceLog->strMessage.c_str();
		WriteLog(str_log);
	}
	else if (strEvent == FACTORY_LOG)
	{
		st_UIResult st_factoryLog;

		m_pITool->GetTestResult(0, &st_factoryLog);
		Fire(UI_RESULT, (long)&st_factoryLog);

		str_log.Format("Result: %s, ItemCode: %s, ErrCode: %s, Band: %s, Channel: %s, Upper: %s, Lower: %s, Measured: %s, Unit: %s, Msg: %s",
			st_factoryLog.sz_result, st_factoryLog.sz_itemCode, st_factoryLog.sz_errCode, st_factoryLog.sz_band, st_factoryLog.sz_channel,
			st_factoryLog.sz_upper, st_factoryLog.sz_lower, st_factoryLog.sz_measured, st_factoryLog.sz_unit, st_factoryLog.sz_msg);

		WriteLog(str_log);
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

/**************************** Log functions ********************************/
void CUnifyDLL_FTWireless::WriteLog(CString str_log, const int i_severity)
{
	m_GLog.Log(i_severity, str_log);
}

void CUnifyDLL_FTWireless::SetLogFileName()
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
    CString currTime;
	currTime.Format(_T("%04d%02d%02d_%02d%02d_%02d"),
		systemTime.wYear, systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	if (m_str_picasso.IsEmpty())
		m_str_picasso = _T("RD");
	CString filename;
	filename.Format(_T("%s_%s.log"), m_str_picasso, currTime);
	m_GLog.SetGLogFileName_Info(filename);
}

void CUnifyDLL_FTWireless::SetLogFilePath()
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	CString currDate;
	currDate.Format(_T("%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
	CString logPath;
	logPath.Format(_T("D:\\LOG\\DLL\\%s\\%s"), m_str_station, currDate);
	m_GLog.InitializeGLog(logPath);
}

void CUnifyDLL_FTWireless::WritePICSdata(void)
{
	st_PICSData sPICS;

	if( m_str_picasso.Find("&") == -1 )
	{
		/* PICASSO */
		sPICS.i_slot = 1; //default value: 1
		strcpy_s(sPICS.sz_key, MAX_BUFFER_SIZE, _T("PICASSO"));
		sprintf_s(sPICS.sz_value, MAX_BUFFER_SIZE,_T("%s"), m_str_picasso);

		Fire(UI_PICSDATA,(long)&sPICS);

		/* TOOL_VERSION */
		sPICS.i_slot = 1; //default value: 1
		strcpy_s(sPICS.sz_key, MAX_BUFFER_SIZE, _T("TOOL_VERSION"));
		sprintf_s(sPICS.sz_value, MAX_BUFFER_SIZE,_T("%s"), m_strToolVersion.c_str());

		Fire(UI_PICSDATA,(long)&sPICS);
	}
	else
	{
		for( int nSlot=1; nSlot<=4; nSlot++ )
		{
			/* PICASSO */
			sPICS.i_slot = nSlot;
			strcpy_s(sPICS.sz_key, MAX_BUFFER_SIZE, _T("PICASSO"));
			sprintf_s(sPICS.sz_value, MAX_BUFFER_SIZE,_T("%s"), m_str_picasso_pool[nSlot-1]);

			Fire(UI_PICSDATA,(long)&sPICS);

			/* TOOL_VERSION */
			sPICS.i_slot = nSlot;
			strcpy_s(sPICS.sz_key, MAX_BUFFER_SIZE, _T("TOOL_VERSION"));
			sprintf_s(sPICS.sz_value, MAX_BUFFER_SIZE,_T("%s"), m_strToolVersion.c_str());

			Fire(UI_PICSDATA,(long)&sPICS);
		}
	}
}
