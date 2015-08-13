#include "stdafx.h"
#include "ITestProcessor.h"
#include <direct.h>

std::string ITestProcessor::g_strPicasso;
CString ITestProcessor::m_csArrPicasso[5] = {"", "", "", "", ""};
std::string ITestProcessor::g_str_modelName;
std::string ITestProcessor::g_str_station;
std::string ITestProcessor::g_str_SWVersion;
std::string ITestProcessor::g_str_FactoryVersion;
std::string ITestProcessor::g_str_HWVersion;
std::string ITestProcessor::g_strIMEI;//LION 2012.4.25
std::string ITestProcessor::g_strPartNumber;
std::string ITestProcessor::g_str_BoardID;

IPhone* ITestProcessor::m_pIPhone = NULL;

std::string ITestProcessor::m_strResult;
std::string ITestProcessor::m_strItemCode;
std::string ITestProcessor::m_strErrorCode;
std::string ITestProcessor::m_strBand;
std::string ITestProcessor::m_strChannel;
std::string ITestProcessor::m_strUpper;
std::string ITestProcessor::m_strLower;
std::string ITestProcessor::m_strMeasured;
std::string ITestProcessor::m_strUnit;
std::string ITestProcessor::m_strMessage;

std::map<std::string, std::string> ITestProcessor::m_mapIMEIParams;
std::map<std::string, std::string> ITestProcessor::m_mapQueryData;

/* CPhone parameters */
int ITestProcessor::m_nFtdPort = 2000;
int ITestProcessor::m_nFtdTimeOut = 15000;
bool ITestProcessor::m_b_AutoFTDMode = false;
bool ITestProcessor::m_b_SetFTDMode = false;
int ITestProcessor::m_i_WaitTimeStartingUp = 1000;	//Default: 1s
int ITestProcessor::m_i_RebootDelayTime = 1000;		//Default: 1s
bool ITestProcessor::m_b_AdbShellStart = false;
int ITestProcessor::m_i_QPhoneMultiCtrl = 0;
int ITestProcessor::m_nQPhoneOrderSlot[5] = {0, 1, 2, 3, 4};
bool ITestProcessor::m_b_LogUpload = false;
std::string ITestProcessor::m_str_UploadServerIP = _T("10.85.68.13");
int ITestProcessor::m_i_IMEILength;
bool ITestProcessor::m_b_jigRequest = false;
bool ITestProcessor::m_b_waitFlag = false;
bool ITestProcessor::m_b_UsableSlot[5] = {false, false, false, false, false}; // [0] dont use
bool ITestProcessor::m_b_isSlotPass[5] = {false, false, false, false, false}; // [0] dont use

/* Show dialog action result */
bool ITestProcessor::m_b_DialogResponse = false;


RF_IMPLEMENT_DYNAMIC(ITestProcessor)


ITestProcessor::ITestProcessor()
{
	DefineNotify(TRACE_LOG);
	DefineNotify(FACTORY_LOG);
	DefineNotify(PICS_DATA);
	DefineNotify(SHOW_PICTURE);
	DefineNotify(JIG_REQUEST);
	DefineNotify(SHOW_DIALOG);
	DefineNotify(UI_RELAYBOARD);
}


// ---------------------- Test flow functions --------------------
bool ITestProcessor::GetDeviceName(char* szName, int iSize)
{
	if (!ParsePhoneParams())
		return false;

	if (m_mapPhoneParams.find(std::string("COMPort")) == m_mapPhoneParams.end())
		return false;

	if (sprintf_s(szName, iSize - 1, "%s", m_mapPhoneParams[std::string("COMPort")].c_str()) == -1)
		return false;

	return true;
}

bool ITestProcessor::SetPicasso(const char* szPicasso)
{
	if (szPicasso != NULL)
	{
		g_strPicasso = szPicasso;
		return true;
	}

	return false;
}

bool ITestProcessor::SetPicassoMulti(const char* szPicasso, const int nSlot)
{
	if (szPicasso != NULL)
	{
		m_csArrPicasso[nSlot].Format(_T("%s"), szPicasso);
		return true;
	}

	return false;
}

bool ITestProcessor::SetSlotUsable(const char* szUsable, const int nSlot)
{
	if (szUsable != NULL)
	{
		if (strcmp(szUsable, "0") == 0)
			m_b_UsableSlot[nSlot] = false;
		return true;
	}
	return false;
}

bool ITestProcessor::SetIMEI(const char* szIMEI)
{
	if (szIMEI != NULL)
	{
		g_strIMEI = szIMEI;
		return true;
	}

	return false;
}

bool ITestProcessor::SetPartNumber(const char* szPartNumber)
{
	if (szPartNumber != NULL)
	{
		g_strPartNumber = szPartNumber;
		return true;
	}

	return false;
}

bool ITestProcessor::SetModelName(const char* p_sz_modelName)
{
	if (p_sz_modelName != NULL)
	{
		g_str_modelName = p_sz_modelName;
		return true;
	}

	return false;
}

bool ITestProcessor::SetStation(const char* p_sz_station)
{
	if (p_sz_station != NULL)
	{
		g_str_station = p_sz_station;
		return true;
	}

	return false;
}

bool ITestProcessor::SetSWVersion(const char* p_sz_SWVersion)
{
	if (p_sz_SWVersion != NULL)
	{
		g_str_SWVersion = p_sz_SWVersion;
		return true;
	}

	return false;
}

bool ITestProcessor::GetSWVersion(char* p_sz_SWVersion)
{
	strcpy(p_sz_SWVersion, g_str_SWVersion.c_str());
	return true;
}

bool ITestProcessor::SetFactoryVersion(const char* p_sz_FactoryVersion)
{
	if (p_sz_FactoryVersion != NULL)
	{
		g_str_FactoryVersion = p_sz_FactoryVersion;
		return true;
	}

	return false;
}

bool ITestProcessor::GetFactoryVersion(char* p_sz_FactoryVersion)
{
	strcpy(p_sz_FactoryVersion, g_str_FactoryVersion.c_str());
	return true;
}

bool ITestProcessor::SetHWVersion(const char* p_sz_HWVersion)
{
	if (p_sz_HWVersion != NULL)
	{
		g_str_HWVersion = p_sz_HWVersion;
		return true;
	}
	return false;
}

bool ITestProcessor::SetSIMFileName(const char* p_sz_SIMFileName)
{
	if (p_sz_SIMFileName != NULL)
	{
		m_mapQueryData["SIMFileName"] = p_sz_SIMFileName;
		return true;
	}

	return false;
}

bool ITestProcessor::SetBoardID(const char* p_sz_BoardID)
{
	if (p_sz_BoardID != NULL)
	{
		g_str_BoardID = p_sz_BoardID;
		return true;
	}

	return false;
}

bool ITestProcessor::SetDialogResponse(bool b_Resopnse)
{
	m_b_waitFlag = true;
	m_b_DialogResponse = b_Resopnse;
	return true;
}

bool ITestProcessor::PhoneCreateObject()
{
	m_strResult = "";
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strLower = "";
	m_strUpper = "";
	m_strUnit = "";
	m_strMeasured = "";
	m_strMessage = "";

	// 1. Parse phone parameters
	if (!LoadPhoneParams())
	{
		m_strMessage = "Fail to parse XML for phone parameters";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	if (!ParsePhoneParams())
	{
		m_strMessage = "Fail to parse XML for phone parameters";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	TraceLog(MSG_INFO, "Parse phone parameters success");

	// 2. Create phone object
	CT2A szPhone(m_cstrPhone);
	std::string strPhone(szPhone);
	CToolRuntimeClass* pcObjectFactory = RF_RUNTIME_CLASS(CToolBaseObject);
	CToolBaseObject* pcObject = NULL;
	if ((pcObject = pcObjectFactory->CreateObject(strPhone)) == NULL)
	{
		m_strMessage = "Fail to create phone object";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	TraceLog(MSG_INFO, "Create phone object success");

	// 3. Run phone object's InitData() API
	if (!pcObject->InitData(m_mapPhoneParams))
	{
		m_strMessage = "Fail to execute InitData() of phone object";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	m_pIPhone = dynamic_cast<IPhone*>(pcObject);
	TraceLog(MSG_INFO, "Execute InitData() of phone object success");

	return true;
}

bool ITestProcessor::InitialPC()
{
	//-- 1. Initial QMSL (find and set Diag) 
	const int MAX_CONNECT_TRY = 100;
	// Detect device
	bool bDetected = false;
	char szMsg[256] = {0};
	for (int i = 0; i < MAX_CONNECT_TRY; ++i)
	{
		if (m_pIPhone->DetectDevice(szMsg))
		{
			bDetected = true;
			break;
		}
		Sleep(500);
	}
	if (!bDetected)
	{
		TraceLog(MSG_ERROR, szMsg);
		m_strMessage = szMsg;
		return false;
	}

	// Initial QMSL
	if (!m_pIPhone->Initial_QMSL())
	{
		TraceLog(MSG_ERROR, "Fail to initial QMSL");
		m_strMessage = szMsg;
		return false;
	}

	//-- 2. Initial and setting FTD
	if (m_i_QPhoneMultiCtrl == 0)
	{
		if (!m_pIPhone->InitDevice(m_nFtdPort, m_nFtdTimeOut))
		{
			m_strMessage = "Fail to Initial PC port";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}
		if (m_pIPhone->m_devices.size() > 1)
		{
			for (size_t i = 0; i < m_pIPhone->m_devices.size(); i++)
			{
				int qphone_port = atoi(m_pIPhone->m_devices[i].c_str() + 6);//"QPHONE1"
				int port = (m_pIPhone->m_devices.size() > 1) ? m_nFtdPort + qphone_port: m_nFtdPort;
				m_pIPhone->InitDevice(port, m_nFtdTimeOut);
			}
		}
		TraceLog(MSG_INFO, "Initial PC port success.");
	}
	else
	{
		for (int i = 1; i <= m_i_QPhoneMultiCtrl; i++)
		{
			if (!m_pIPhone->InitDevice((2000 + m_nQPhoneOrderSlot[i]), m_nFtdTimeOut))
			{
				CString csTraceMsg;
				csTraceMsg.Format(_T("Fail to Initial Slot-%d PC port: %d"), i, (2000 + m_nQPhoneOrderSlot[i]));
				m_strMessage = csTraceMsg;
				TraceLog(MSG_ERROR, m_strMessage);
			}
			else
			{
				CString csTraceMsg;
				csTraceMsg.Format(_T("Success to Initial Slot-%d PC port: %d"), i, (2000 + m_nQPhoneOrderSlot[i]));
				TraceLog(MSG_INFO, std::string(csTraceMsg));
			}

		}
	}
	return true;
}

bool ITestProcessor::MobileAutoFTDMode()
{
	if (m_pIPhone->IsMobileInFTDMode(m_nFtdPort, m_nFtdTimeOut))
	{
		TraceLog(MSG_INFO, "Mobile is already in ftd mode, Do not need to change mode!");
		return true;
	}
	TraceLog(MSG_INFO, "Mobile is not in ftd mode, Auto change ftd mode......");
	if (m_pIPhone->MobileAutoFTDMode(m_nFtdPort, m_nFtdTimeOut))
	{
		TraceLog(MSG_INFO, "MobileAutoFTDMode success!");
		return true;
	}
	m_strMessage = "Fail to enter FTD mode!";
	TraceLog(MSG_INFO, "Fail to enter ftd mode!");
	return false;
}

bool ITestProcessor::MobileSetFTDMode()
{
	if (m_pIPhone->IsMobileInFTDMode(m_nFtdPort, m_nFtdTimeOut))
	{
		TraceLog(MSG_INFO, "Mobile is already in ftd mode, Do not need to set mode!");
		return true;
	}

	TraceLog(MSG_INFO, "Mobile is not in ftd mode, set ftd mode......");
	CString str_command = _T("shell set-mode 3");
	char errMsg[128] = {0};
	char cmdOut[1024] = {0};
	if (!m_pIPhone->ExecAdbCommand(str_command, cmdOut, errMsg))
	{
		m_strMessage = "Fail to set FTD mode!";
		TraceLog(MSG_INFO, errMsg);
		TraceLog(MSG_INFO, "Fail to set ftd mode!");
		return false;
	}
	TraceLog(MSG_INFO, "Execute 'set-mode 3' adb command success!");
	CString str_OutMsg;
	str_OutMsg.Format(_T("%s"), cmdOut);
	if (str_OutMsg.Find(_T("error")) != -1)
	{
		m_strMessage = "Fail to set FTD mode!";
		TraceLog(MSG_INFO, "Fail to set FTD mode!");
		return false;
	}

	/* Reboot phone */
	TraceLog(MSG_INFO, "Reboot mobile phone......");
	str_command = _T("reboot");
	if (!m_pIPhone->ExecAdbCommand(str_command, cmdOut, errMsg))
	{
		m_strMessage = "Fail to set FTD mode!";
		TraceLog(MSG_INFO, errMsg);
		TraceLog(MSG_INFO, "Fail to reboot mobile phone!");
		return false;
	}
	TraceLog(MSG_INFO, "Execute reboot mobile adb command success!");
	str_OutMsg.Format(_T("%s"), cmdOut);
	if (str_OutMsg.Find(_T("error")) != -1)
	{
		m_strMessage = "Fail to set FTD mode!";
		TraceLog(MSG_INFO, "Fail to reboot mobile phone!");
		return false;
	}
	Sleep(m_i_RebootDelayTime);//wait for phone reboot
	return true;
}

bool ITestProcessor::KillADBThread()
{
	if (!m_pIPhone) return false;

	char errMsg[128] = {0};
	char cmdOut[1024] = {0};
	return m_pIPhone->ExecAdbCommand(_T("kill-server"), cmdOut, errMsg);
}

/* FA data operate functions */
bool ITestProcessor::ReadFAData(char* pszFAdata, int i_size)
{
	return false;
}

bool ITestProcessor::WriteFAData(char* pszFAdata, size_t i_InSize, char* pszOutput)
{
	return false;
}

//QTH 2013.3.14 Neil
bool ITestProcessor::ReadFAData_New(int i_slot, int i_sectorNum, char* pszFAdata, int i_size)
{
	if (!m_pIPhone) return false;

	char sz_FAData[FTD_BUF_SIZE] = {0};
	char sz_Address[10] = {0};
	_itoa_s(i_sectorNum * 512, sz_Address, 10, 10); // int to char
	strcat(sz_Address, ",512");
	int port = m_nFtdPort;
	if (m_i_QPhoneMultiCtrl == 0)
	{
		int qphone_port = atoi(m_pIPhone->m_devices[0].c_str() + 6);//"QPHONE1"
		port = (m_pIPhone->m_devices.size() > 1) ? m_nFtdPort + qphone_port : m_nFtdPort;
	}
	else
	{
		if (!m_b_UsableSlot[i_slot])
		{
			CString csTeaceMsg;
			csTeaceMsg.Format(_T("By Pass to Read QPhone%d FA data"), i_slot);
			TraceLog(MSG_INFO, std::string(CT2A(csTeaceMsg)));
			return false;
		}
		port = 2000 + m_nQPhoneOrderSlot[i_slot];
	}
	bool b_res = m_pIPhone->FTD_FAC_FARead(port, m_nFtdTimeOut, sz_Address, sz_FAData);
	if (b_res)
	{
		memcpy(pszFAdata, sz_FAData, i_size);
		CString csTeaceMsg;
		csTeaceMsg.Format(_T("Read FA data from QPhone%d success"), i_slot);
		TraceLog(MSG_INFO, std::string(csTeaceMsg));
	}
	else
	{
		m_strErrorCode = CommErr_Read_Factory_Area_Fail;
		m_strMessage = "Fail to read FA data from mobile";
		CString csTeaceMsg;
		csTeaceMsg.Format(_T("Fail to read FA data from QPhone%d"), i_slot);
		TraceLog(MSG_ERROR, std::string(csTeaceMsg));
	}
	return b_res;
}

bool ITestProcessor::WriteFAData_New(int i_sectorNum, char* pszFAdata, size_t i_size, char* pszOutput)
{
	if (!m_pIPhone) return false;

	char szInput[FTD_BUF_SIZE] = {0} ;
	_itoa_s(i_sectorNum * 512, szInput, 10, 10); // int to char
	strcat(szInput, ",512,");
	strcat(szInput, pszFAdata);
	bool b_res = m_pIPhone->FTD_FAC_FAWrite(m_nFtdPort, m_nFtdTimeOut, szInput, pszOutput);
	if (b_res)
	{
		TraceLog(MSG_INFO, "Write FA data to mobile success");
	}
	else
	{
		m_strErrorCode = CommErr_Write_Factory_Area_Fail;
		m_strMessage = "Fail to write FA data to mobile";
		TraceLog(MSG_ERROR, m_strMessage);
	}
	return b_res;
}

bool ITestProcessor::LoadIMEIParams()
{
	// 1. Load parameters from config XML file
	if (m_ConfigXML.Load(m_cstrConfigFile) != ERROR_SUCCESS)
	{
		m_strMessage = "Fail to load config XML file, check file formation";
		m_strErrorCode = CommErr_UI_Interface_Getparameter_Fail;
		TraceLog(MSG_INFO, m_strMessage);
		return false;
	}

	XMLNodeList pIMEIList = m_ConfigXML.GetChildNodes("//Configuration//IMEI_MAL");
	if (pIMEIList == NULL)
	{
		m_strMessage = "Fail to find node IMEI_MAL in config XML file";
		m_strErrorCode = CommErr_UI_Interface_Getparameter_Fail;
		TraceLog(MSG_INFO, m_strMessage);
		return false;
	}

	m_mapIMEIParams.clear();
	for (UINT i = 0; i < m_ConfigXML.GetNodeListSize(pIMEIList); ++i)
	{
		XMLNode pIMEIParams = m_ConfigXML.GetNodeListItem(pIMEIList, i);
		if (pIMEIParams == NULL)
			continue;

		CString nodename = m_ConfigXML.GetNodeName(pIMEIParams);
		CString nodevalue = m_ConfigXML.GetNodeText(pIMEIParams);
		m_mapIMEIParams[nodename.GetString()] = nodevalue.GetString();
	}

	return true;
}

bool ITestProcessor::UpdateIMEIParams(char* szParam)
{
	// 1. Load parameters from config XML file
	if (m_ConfigXML.Load(m_cstrConfigFile) != ERROR_SUCCESS)
	{
		m_strMessage = "Fail to load config XML file, check file formation";
		m_strErrorCode = CommErr_UI_Interface_Getparameter_Fail;
		TraceLog(MSG_INFO, m_strMessage);
		return false;
	}

	XMLNodeList pIMEIList = m_ConfigXML.GetChildNodes("//Configuration//IMEI_MAL");
	if (pIMEIList == NULL)
	{
		m_strMessage = "Fail to find node IMEI_MAL in config XML file";
		m_strErrorCode = CommErr_UI_Interface_Getparameter_Fail;
		TraceLog(MSG_INFO, m_strMessage);
		return false;
	}

	for (UINT i = 0; i < m_ConfigXML.GetNodeListSize(pIMEIList); ++i)
	{
		XMLNode pIMEIParams = m_ConfigXML.GetNodeListItem(pIMEIList, i);
		if (pIMEIParams == NULL)
			continue;

		CString CStrParamName = szParam;
		if (CStrParamName.CompareNoCase(m_ConfigXML.GetNodeName(pIMEIParams)) == 0)
		{
			CString CStrParamValue = m_mapIMEIParams[szParam].c_str();
			m_ConfigXML.SetNodeText(pIMEIParams, CStrParamValue);
		}
	}
	m_ConfigXML.Save();

	return true;
}

bool ITestProcessor::LoadPhoneParams()
{
	// 1. Load parameters from config XML file
	if (m_ConfigXML.Load(m_cstrConfigFile) != ERROR_SUCCESS)
	{
		TraceLog(MSG_INFO, "Fail to load config XML file, check file formation");
		return false;
	}

	XMLNodeList phoneList = m_ConfigXML.GetChildNodes("//Configuration//Phone");
	if (phoneList == NULL)
	{
		TraceLog(MSG_INFO, "Fail to find node Phone in config XML file");
		return false;
	}

	m_mapPhoneParams.clear();
	for (UINT i = 0; i < m_ConfigXML.GetNodeListSize(phoneList); ++i)
	{
		XMLNode phoneParams = m_ConfigXML.GetNodeListItem(phoneList, i);
		if (NULL == phoneParams)
			continue;

		CString nodename = m_ConfigXML.GetNodeName(phoneParams);
		if (nodename.CompareNoCase(_T("ClassName")) == 0)
			m_cstrPhone = m_ConfigXML.GetNodeText(phoneParams);

		CString nodevalue = m_ConfigXML.GetNodeText(phoneParams);
		m_mapPhoneParams[nodename.GetString()] = nodevalue.GetString();
	}

	// 2. Load parameters from test item XML file
	if (m_ConfigXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
	{
		TraceLog(MSG_INFO, "Fail to load test item XML file, check file formation");
		return false;
	}

	phoneList = m_ConfigXML.GetChildNodes("//Configuration//Phone");
	if (phoneList == NULL)
	{
		TraceLog(MSG_INFO, "Fail to find node Phone in test item XML file");
		return false;
	}

	for (UINT i = 0; i < m_ConfigXML.GetNodeListSize(phoneList); ++i)
	{
		XMLNode phoneParams = m_ConfigXML.GetNodeListItem(phoneList, i);
		if (NULL == phoneParams)
			continue;

		CString nodename = m_ConfigXML.GetNodeName(phoneParams);
		if (nodename.CompareNoCase(_T("ClassName")) == 0)
			m_cstrPhone = m_ConfigXML.GetNodeText(phoneParams);

		CString nodevalue = m_ConfigXML.GetNodeText(phoneParams);
		m_mapPhoneParams[nodename.GetString()] = nodevalue.GetString();
	}
	return true;
}

//Test items related functions
bool ITestProcessor::LoadTestITems()
{
	bool bRes = false;
	if (m_TestItemXML.Load(m_cstrTestItemFile) == ERROR_SUCCESS)
		bRes = true;

	if (!bRes) return false;

	XMLNodeList testItemNodeList = NULL;
	if (bRes)
		testItemNodeList = m_TestItemXML.GetChildNodes(_T("//Configuration//TestItemList"));
	if (testItemNodeList != NULL)
		bRes = true;

	for (UINT i = 0; (bRes && (i < m_TestItemXML.GetNodeListSize(testItemNodeList))); ++i)
	{
		XMLNode testItemNode = m_TestItemXML.GetNodeListItem(testItemNodeList, i);
		if (testItemNode != NULL)
			bRes = true;
		else
			bRes = false;

		CString cstrEnabled = m_TestItemXML.GetNodeAttrValue(testItemNode, _T("Enabled"));
		if (cstrEnabled.CompareNoCase(_T("TRUE")) != 0)
			continue;

		CString cstrTestItemName = m_TestItemXML.GetNodeAttrValue(testItemNode, _T("Name"));
		if (cstrTestItemName.IsEmpty() != true)
		{
			CT2A pszTestItemName(cstrTestItemName);
			std::string strTestItemName(pszTestItemName);
			m_vTestItemList.push_back(strTestItemName);
		}
	}
	return bRes;
}

int ITestProcessor::GetTestItemCount()
{
	return (signed)m_vTestItemList.size();
}

bool ITestProcessor::GetTestItemByIndex(int nIndex, char* szName, int nSize)
{
	bool bRes = false;
	if (nIndex < (signed)m_vTestItemList.size())
	{
		if (sprintf_s(szName, nSize, "%s", m_vTestItemList[nIndex].c_str()) != -1)
			bRes = true;
	}
	return bRes;
}

bool ITestProcessor::GetTestResult(int nIndex, st_UIResult* psResult)
{
	if (psResult == NULL) return false;

	sprintf_s(psResult->sz_result, MAX_BUFFER_SIZE - 1, "%s", m_strResult.c_str());
	sprintf_s(psResult->sz_itemCode, MAX_BUFFER_SIZE - 1, "%s", m_strItemCode.c_str());
	sprintf_s(psResult->sz_errCode, MAX_BUFFER_SIZE - 1, "%s", m_strErrorCode.c_str());
	sprintf_s(psResult->sz_band, MAX_BUFFER_SIZE - 1, "%s", m_strBand.c_str());
	sprintf_s(psResult->sz_channel, MAX_BUFFER_SIZE - 1, "%s", m_strChannel.c_str());
	sprintf_s(psResult->sz_upper, MAX_BUFFER_SIZE - 1, "%s", m_strUpper.c_str());
	sprintf_s(psResult->sz_lower, MAX_BUFFER_SIZE - 1, "%s", m_strLower.c_str());
	sprintf_s(psResult->sz_measured, MAX_BUFFER_SIZE - 1, "%s", m_strMeasured.c_str());
	sprintf_s(psResult->sz_unit, MAX_BUFFER_SIZE - 1, "%s", m_strUnit.c_str());
	sprintf_s(psResult->sz_msg, MAX_BUFFER_SIZE - 1, "%s", m_strMessage.c_str());
	return true;
}

//Initial functions
bool ITestProcessor::ParsePhoneParams()
{
	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode phone = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone"));
	if (phone == NULL)
		return false;

	XMLNode m_FtdPort = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//FtdPort"));
	if (m_FtdPort != NULL)
	{
		CString cstrComPort = m_TestItemXML.GetNodeText(m_FtdPort);
		m_nFtdPort = atoi(cstrComPort);
	}

	XMLNode m_FtdTimeOut = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//FtdTimeOut"));
	if (m_FtdTimeOut != NULL)
	{
		CString cstrFtdTimeOut = m_TestItemXML.GetNodeText(m_FtdTimeOut);
		m_nFtdTimeOut = atoi(cstrFtdTimeOut);
	}

	XMLNode m_AutoFTDMode = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//AutoFTDMode"));
	if (m_AutoFTDMode != NULL)
	{
		CString str_AutoFTDMode = m_TestItemXML.GetNodeText(m_AutoFTDMode);
		m_b_AutoFTDMode = CStr::StrToBool(str_AutoFTDMode.GetString());
	}

	XMLNode m_SetFTDMode = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//SetFTDMode"));
	if (m_SetFTDMode != NULL)
	{
		CString str_SetFTDMode = m_TestItemXML.GetNodeText(m_SetFTDMode);
		m_b_SetFTDMode = CStr::StrToBool(str_SetFTDMode.GetString());
	}

	XMLNode m_WaitTime = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//WaitTimeStartingUp"));
	if (m_WaitTime != NULL)
	{
		CString str_WaitTime = m_TestItemXML.GetNodeText(m_WaitTime);
		m_i_WaitTimeStartingUp = atoi(str_WaitTime);
	}

	XMLNode m_RebootDelayTime = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//RebootDelayTime"));
	if (m_RebootDelayTime != NULL)
	{
		CString str_RebootDelayTime = m_TestItemXML.GetNodeText(m_RebootDelayTime);
		m_i_RebootDelayTime = atoi(str_RebootDelayTime);
	}
/*
	std::map <std::string, std::string> mapPhoneParams;
	for (UINT i = 0; i < m_TestItemXML.GetNodeListSize(phoneList); ++i)
	{
		XMLNode phoneParams = m_TestItemXML.GetNodeListItem(phoneList, i);
		if (NULL == phoneParams)
			continue;

		CString nodename = m_TestItemXML.GetNodeName(phoneParams);
		if (nodename.CompareNoCase(_T("ClassName")) == 0)
			m_cstrPhone = m_TestItemXML.GetNodeText(phoneParams);

		CString nodevalue = m_TestItemXML.GetNodeText(phoneParams);
		mapPhoneParams[nodename.GetString()] = nodevalue.GetString();
	}
*/
	XMLNode m_QPhoneMulti = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//QPhoneMulti"));
	if (m_QPhoneMulti != NULL)
	{
		CString str_QPhoneMulti = m_TestItemXML.GetNodeText(m_QPhoneMulti);
		m_i_QPhoneMultiCtrl = atoi(str_QPhoneMulti);
	}

	XMLNode m_QPhoneOrder = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//QPhoneOrder"));
	if (m_QPhoneOrder != NULL)
	{
		String parameters = m_TestItemXML.GetNodeText(m_QPhoneOrder).GetString();
		StrVtr vtrstr;
		CStr::ParseString(parameters, ",", vtrstr);
		m_nQPhoneOrderSlot[1] = CStr::StrToInt(vtrstr[0]);
		m_nQPhoneOrderSlot[2] = CStr::StrToInt(vtrstr[1]);
		m_nQPhoneOrderSlot[3] = CStr::StrToInt(vtrstr[2]);
		m_nQPhoneOrderSlot[4] = CStr::StrToInt(vtrstr[3]);
	}

	XMLNode m_LogUpload = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//LogUpload"));
	if (m_LogUpload != NULL)
	{
		CString str_LogUpload = m_TestItemXML.GetNodeText(m_LogUpload);
		m_b_LogUpload = CStr::StrToBool(str_LogUpload.GetString());
	}

	XMLNode m_UploadServerIP = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//UploadServerIP"));
	if (m_UploadServerIP != NULL)
	{
		CString str_UploadServerIP = m_TestItemXML.GetNodeText(m_UploadServerIP);
		m_str_UploadServerIP = str_UploadServerIP.GetString();
	}

	XMLNode m_IMEILength = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//IMEILength"));
	if (m_IMEILength != NULL)
	{
		CString str_IMEILength = m_TestItemXML.GetNodeText(m_IMEILength);
		m_i_IMEILength = atoi(str_IMEILength);
	}
	else
		return false;
/*
	// Search variant
	XMLNode toolinfo = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//Variant"));
	if (toolinfo == NULL)
		return false;

	mapPhoneParams["Variant"] = CT2A(m_TestItemXML.GetNodeText(toolinfo).GetString());
*/
	//m_mapPhoneParams = mapPhoneParams;
	return !m_cstrPhone.IsEmpty();
}

bool ITestProcessor::ParseTestItemParameters()
{
	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode processObjectSet = m_TestItemXML.SearchNode(_T("//Configuration//ProcessObjectSet"));
	if (processObjectSet == NULL)
		return false;

	XMLNodeList processObjectsCategoryList = processObjectSet->GetchildNodes();
	if (processObjectsCategoryList == NULL)
		return false;

	std::map<std::string, std::string> mapObjectParams;
	for (UINT j = 0; j < m_TestItemXML.GetNodeListSize(processObjectsCategoryList); ++j)
	{
		// Configuration//ProcessObjectSet//Category
		XMLNode processObjectsCategory = m_TestItemXML.GetNodeListItem(processObjectsCategoryList, j);
		if (processObjectsCategory == NULL)
			continue;

		XMLNodeList processObjectList = processObjectsCategory->GetchildNodes();
		if (processObjectList == NULL)
			continue;

		for (UINT k = 0; k < m_TestItemXML.GetNodeListSize(processObjectList); ++k)
		{
			// Configuration//ProcessObjectSet//Category//ProcessObject
			XMLNode processObject = m_TestItemXML.GetNodeListItem(processObjectList, k);
			if (processObject == NULL)
				continue;

			// Parse the process object
			mapObjectParams.clear();
			CString cstrName = m_TestItemXML.GetNodeAttrValue(processObject, _T("Name"));
			CString cstrClassName = m_TestItemXML.GetNodeAttrValue(processObject, _T("ClassName"));
			if (cstrName.IsEmpty() || cstrClassName.IsEmpty())
				continue;

			// Insert <Name, ClassName> to parameter map
			CT2A szName(cstrName);
			std::string strName(szName);
			CT2A szClassName(cstrClassName);
			std::string strClassName(szClassName);
			mapObjectParams[strName] = strClassName;

			XMLNodeList processObjectParamList = processObject->GetchildNodes();
			if (processObjectParamList == NULL)
				continue;
			for (UINT l = 0; l < m_TestItemXML.GetNodeListSize(processObjectParamList); ++l)
			{
				XMLNode processObjectParam = m_TestItemXML.GetNodeListItem(processObjectParamList, l);
				if (processObjectParam == NULL)
					continue;

				CString cstrParamName = m_TestItemXML.GetNodeName(processObjectParam);
				CString cstrParamData = m_TestItemXML.GetNodeText(processObjectParam);
				if (cstrParamName.IsEmpty() || cstrParamData.IsEmpty())
					continue;

				// Insert <ParamName, ParamData> to parameter map
				CT2A szParamName(cstrParamName);
				std::string strParamName(szParamName);
				CT2A szParamData(cstrParamData);
				std::string strParamData(szParamData);
				mapObjectParams[strParamName] = strParamData;
			}//for (UINT l = 0; l < m_TestItemXML.GetNodeListSize(processObjectParamList); ++l)
			// Insert all parameters in m_mapTestItemParams
			if (mapObjectParams.empty() != true)
				m_mapTestItemParams[strName] = mapObjectParams;
		}//for (UINT k = 0; k < m_TestItemXML.GetNodeListSize(processObjectList); ++k)
	}//for (UINT j = 0; j < m_TestItemXML.GetNodeListSize(processObjectsCategoryList); ++j)
	return true;
}

//Notification
bool ITestProcessor::Register(INotify_ANSI* pNotify, const std::string& strEvent)
{
	m_piToolNotify = pNotify;
	return CNotifyBase_ANSI::Register(pNotify, strEvent);
}

bool ITestProcessor::Register(EventFunc_ANSI* pEventFunc, const std::string& strEvent)
{
	return CNotifyBase_ANSI::Register(pEventFunc, strEvent);
}

bool ITestProcessor::Unregister(INotify_ANSI* pNotify, const std::string& strEvent)
{
	return CNotifyBase_ANSI::Unregister(pNotify, strEvent);
}

bool ITestProcessor::Unregister(EventFunc_ANSI* pEventFunc, const std::string& strEvent)
{
	return CNotifyBase_ANSI::Unregister(pEventFunc, strEvent);
}

void ITestProcessor::DefineNotify(const std::string& strNewEvent)
{
	CNotifyBase_ANSI::DefineNotify(strNewEvent);
}

void ITestProcessor::Fire(const std::string& strEvent, long nParam)
{
	CNotifyBase_ANSI::Fire(strEvent, nParam);
}

//DLL Event functions
//Logging file
void ITestProcessor::TraceLog(int iSeverity, std::string strMsg)
{
	sTraceLog traceLog;
	traceLog.iSeverity = iSeverity;
	traceLog.strMessage = strMsg;
	Fire(TRACE_LOG, (long)&traceLog);
}

void ITestProcessor::TraceLog(int iSeverity, std::string strMsg, int nQPhoneNumber)
{
	char szQphoneMsg[100] = {0};
	sprintf(szQphoneMsg, "QPhone%d ", nQPhoneNumber);

	sTraceLog traceLog;
	traceLog.iSeverity = iSeverity;
	traceLog.strMessage = szQphoneMsg;
	traceLog.strMessage += strMsg;
	Fire(TRACE_LOG, (long)&traceLog);
}

//Factory log
void ITestProcessor::FactoryLog()
{
	Fire(FACTORY_LOG, NULL);
}

//Set PICS data
void ITestProcessor::SetPICSData(std::string str_key, std::string str_value)
{
	if (str_key.empty()) return;
	st_PICSData picsData;
	picsData.i_slot = 1;
	strcpy(picsData.sz_key, str_key.c_str());
	strcpy(picsData.sz_value, str_value.c_str());

	Fire(PICS_DATA, (long)&picsData);

	TraceLog(MSG_INFO, "SetPICS:[" + str_key + "]= " + str_value);
}

void ITestProcessor::SetPICSData(std::string str_key, std::string str_value, int nQPhoneNumber)
{
	if (str_key.empty()) return;
	st_PICSData picsData;
	picsData.i_slot = (m_i_QPhoneMultiCtrl == 0) ? 1 :nQPhoneNumber;
	strcpy(picsData.sz_key, str_key.c_str());
	strcpy(picsData.sz_value, str_value.c_str());

	Fire(PICS_DATA, (long)&picsData);

	TraceLog(MSG_INFO, "SetPICS:[" + str_key + "]= " + str_value, nQPhoneNumber);
}

//Show picture
void ITestProcessor::ShowPicture(bool b_append, const char* p_sz_path)
{
	st_UIPattern picture;
	picture.i_slot = 1;
	picture.b_append = b_append;
	sprintf_s(picture.sz_pattern, "%s", p_sz_path);
	Fire(SHOW_PICTURE, (long)&picture);
}

//Jig Request
void ITestProcessor::JigRequest(const char* p_sz_switch)
{
	st_JigRequest st_switch;
	st_switch.i_slot = 1;
	sprintf_s(st_switch.sz_switch, "%s", p_sz_switch);
	Fire(JIG_REQUEST, (long)&st_switch);
	return;
}

//Show Dialog
void ITestProcessor::ShowDialog(const char* p_sz_message)
{
	st_ShowDialog dialog;
	dialog.i_slot = 1;
	sprintf_s(dialog.sz_message, "%s", p_sz_message);
	Fire(SHOW_DIALOG, (long)&dialog);
}

//Execute Android Logcat
bool ITestProcessor::ExecAndroidLogcat(bool isPass)
{
	char szLogPath[1000] = "D:\\Log";
	_mkdir(szLogPath);
	strcat(szLogPath, "\\Android");
	_mkdir(szLogPath);
	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);
	CString str_Date;
	str_Date.Format(_T("\\%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
	strcat(szLogPath, str_Date);
	_mkdir(szLogPath);

	CString str_IMEI = _T("RD");
	if (!g_strIMEI.empty())
		str_IMEI = g_strIMEI.c_str();

	CString strPassFail = isPass ? _T("PASS") : _T("FAIL");
	CString msg;
	CString command;
	if (m_i_QPhoneMultiCtrl == 0)
	{
		CString csDevice;
		if (m_pIPhone->m_devices.size() > 1)
			csDevice.Format(_T("-s %s"), m_pIPhone->m_devices[0].c_str());
		command.Format(_T("/C adb %s logcat -v time -d > %s\\%s_logcat_%s_%04d%02d%02d_%02d%02d%02d.txt"),
			csDevice, szLogPath, strPassFail, str_IMEI,
			systemTime.wYear, systemTime.wMonth, systemTime.wDay,
			systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
		SHELLEXECUTEINFO ShExecInfo = {0};
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = "open";
		ShExecInfo.lpFile = "cmd";
		ShExecInfo.lpParameters = command;
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, 3000);

		msg.Format(_T("Execute: %s"), command);
		TraceLog(MSG_INFO, std::string(CT2A(msg)));
	}
	else
	{
		for (int nQPhoneNumber = 1; nQPhoneNumber <= m_i_QPhoneMultiCtrl; nQPhoneNumber++)
		{
			if ((m_b_UsableSlot[nQPhoneNumber] == true) && (m_b_isSlotPass[nQPhoneNumber] == isPass))
			{
				command.Format(_T("/C adb -s QPHONE%d logcat -v time -d > %s\\%s_logcat_%s_%04d%02d%02d_%02d%02d%02d.txt"), m_nQPhoneOrderSlot[nQPhoneNumber],
				               szLogPath, strPassFail, m_csArrPicasso[nQPhoneNumber],
				               systemTime.wYear, systemTime.wMonth, systemTime.wDay,
				               systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
				SHELLEXECUTEINFO ShExecInfo = {0};
				ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
				ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
				ShExecInfo.hwnd = NULL;
				ShExecInfo.lpVerb = "open";
				ShExecInfo.lpFile = "cmd";
				ShExecInfo.lpParameters = command;
				ShExecInfo.lpDirectory = NULL;
				ShExecInfo.nShow = SW_HIDE;
				ShExecInfo.hInstApp = NULL;
				ShellExecuteEx(&ShExecInfo);
				WaitForSingleObject(ShExecInfo.hProcess, 3000);

				msg.Format(_T("Execute: %s"), command);
				TraceLog(MSG_INFO, std::string(CT2A(msg)));
			}
		}
	}
	return true;
}

//upload log to server by BAT
bool ITestProcessor::LogUploadByBAT(char* str_modelname)
{
	if (!m_b_LogUpload) return true;

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	char szModulePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);

	char str_ComputerName[50];
	DWORD dwRet = 50;
	GetComputerName(str_ComputerName, &dwRet);

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	char sysdate[100];
	sprintf(sysdate, _T("%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);

	char path_bat[2000];
	sprintf(path_bat, "%s\\Qisda\\Uplog.bat", szModulePath);
	ShExecInfo.lpFile = path_bat;

	char bat_param[2000];
	sprintf(bat_param, "%s %s %s %s", m_str_UploadServerIP.c_str(), str_modelname, str_ComputerName, sysdate);
	ShExecInfo.lpParameters = bat_param;

	ShellExecuteEx(&ShExecInfo);
	return true;
}


//upload log to server
bool ITestProcessor::LogUpload(char* str_modelname)
{
	if (!m_b_LogUpload) return true;

	m_strResult = "-";
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpFile = "cmd";
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	char str_RemotePath[1000];
	char str_LocalPath[1000] = "D:\\Log";
	char str_BackupPath[1000] = "D:\\LogUpload";
	char temp[2000];

	//-- 建立遠端路徑
	strcpy(str_RemotePath, _T("\\\\"));
	strcat(str_RemotePath, m_str_UploadServerIP.c_str());
	strcat(str_RemotePath, _T("\\TestLog"));

	char str_account[100] = _T("mpics\\TestLog");
	char str_pswd[100] = _T("qisda2013");
	sprintf(temp, "/C net use %s /user:%s %s", str_RemotePath, str_account, str_pswd);
	ShExecInfo.lpParameters = temp;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, 3000);

	//-- 測試建立連線是否成功
	sprintf(temp, "/C net use %s", str_RemotePath);
	ShExecInfo.lpParameters = temp;
	ShellExecuteEx(&ShExecInfo);
	if (WaitForSingleObject(ShExecInfo.hProcess, 2000) == WAIT_TIMEOUT)
	{
		m_strResult = _T("FAIL");
		m_strMessage = "Fail to connect server uploading.";
		FactoryLog();
		return false;
	}

	_mkdir(str_RemotePath);
	strcat(str_RemotePath, _T("\\"));
	strcat(str_RemotePath, str_modelname);
	_mkdir(str_RemotePath);

	char str_ComputerName[50];
	DWORD dwRet = 50;
	GetComputerName(str_ComputerName, &dwRet);
	strcat(str_RemotePath, "\\");
	strcat(str_RemotePath, str_ComputerName);
	_mkdir(str_RemotePath);

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	char systime[100];
	sprintf(systime, _T("%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
	strcat(str_RemotePath, "\\");
	strcat(str_RemotePath, systime);
	_mkdir(str_RemotePath);

	int nUploadTimeout = 20000;

	//-- 複製所有檔案至遠端
	sprintf(temp, "/C xcopy /e /y %s %s", str_LocalPath, str_RemotePath);
	ShExecInfo.lpParameters = temp;
	ShellExecuteEx(&ShExecInfo);
	if(WaitForSingleObject(ShExecInfo.hProcess, nUploadTimeout) == WAIT_TIMEOUT)
	{
		CString csMsg;
		csMsg.Format(_T("Log uploading timeout with %d sec"), nUploadTimeout / 1000);
		m_strMessage = _T(csMsg);
		FactoryLog();
	}

	//-- 確認資料完整傳送至遠端
	bool b_ok = DFSCheckFile(str_LocalPath, str_RemotePath, _T(""));

	//-- 複製log至本地指定目錄並刪除
	if (b_ok)
	{
		_mkdir(str_BackupPath);
		sprintf(temp, "/C xcopy /e /y /q %s %s", str_LocalPath, str_BackupPath);
		ShExecInfo.lpParameters = temp;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

		DFSDeleteFile(str_LocalPath, _T(""));
		m_strMessage = "Check and Upload Pass.";
		FactoryLog();
		return true;
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Copy and Check upload file Fail.";
		FactoryLog();
		return false;
	}
}

/* Check upload file */
bool ITestProcessor::DFSCheckFile(char* str_LocalPath, char* str_RemotePath, char* str_TracePath)
{
	char szDir[1000];
	sprintf(szDir, "%s\\%s\\*", str_LocalPath, str_TracePath);
	HANDLE hList;
	WIN32_FIND_DATA FileData;
	if ((hList = FindFirstFile(szDir, &FileData)) == INVALID_HANDLE_VALUE)	//此路徑沒下層路徑,有可能為檔案
	{
		sprintf(szDir, "%s\\%s", str_RemotePath, str_TracePath);			//確認遠端路徑下也有此檔案
		FindClose(hList);
		if (_taccess(szDir, 0) == -1) 										//找不到遠端路徑下檔案
		{
			std::string strMsg = str_RemotePath;
			strMsg += "\\";
			strMsg += str_TracePath;
			TraceLog(MSG_INFO, "Not to find the remote file: " + strMsg);
			return false;
		}
		else 																//找到遠端路徑下檔案
		{
			return true;
		}
	}
	char str_tempPath[1000];
	for (;;)
	{
		if (!FindNextFile(hList, &FileData))
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
			{
				FindClose(hList);
				return true;
			}
		}
		if (strcmp(FileData.cFileName, _T("..")) != 0)
		{
			strcpy(str_tempPath, str_TracePath);
			if (strcmp(str_tempPath, "") != 0)
				strcat(str_tempPath, "\\");
			strcat(str_tempPath, FileData.cFileName);
			if (!DFSCheckFile(str_LocalPath, str_RemotePath, str_tempPath))
			{
				FindClose(hList);
				return false;
			}
		}
	}
}

/* Delete local file */
bool ITestProcessor::DFSDeleteFile(char* str_LocalPath, char* str_TracePath)
{
	char szDir[1000];
	sprintf(szDir, "%s\\%s\\*", str_LocalPath, str_TracePath);
	HANDLE hList;
	WIN32_FIND_DATA FileData;
	if ((hList = FindFirstFile(szDir, &FileData)) == INVALID_HANDLE_VALUE)//此路徑沒下層路徑,有可能為檔案
	{
		bool b_DelFile = true;
		//.csv檔不刪
		if(strstr(str_TracePath, ".CSV") != NULL)	b_DelFile = false;

		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);
		char TodayKeepLog[1000];

		//DB_Log今日檔不刪
		sprintf(TodayKeepLog, "DB_Log\\%04d_%02d_%02d", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		//VMS_Log今日檔不刪
		sprintf(TodayKeepLog, "VMS_Log\\%04d_%02d_%02d", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		//UI_Log今日檔不刪
		sprintf(TodayKeepLog, _T("UILog_%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL) b_DelFile = false;

		//統計csv今日檔不刪
		sprintf(TodayKeepLog, "%04d%02d%02d.csv", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		//Android logcat今日檔不刪
		sprintf(TodayKeepLog, "Android\\%04d%02d%02d\\", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		//EntranceTool_Log今日檔不刪
		sprintf(TodayKeepLog, "EntranceTool_Log\\%04d_%02d_%02d", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		if (b_DelFile)
		{
			sprintf(szDir, "%s\\%s", str_LocalPath, str_TracePath);
			remove(szDir);
		}
		//TraceLog(MSG_INFO, szDir);
		FindClose(hList);
		return true;
	}
	char str_tempPath[1000];
	for (;;)
	{
		if (!FindNextFile(hList, &FileData))
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
			{
				FindClose(hList);
				return true;
			}
		}
		if (strcmp(FileData.cFileName, _T("..")) != 0)
		{
			strcpy(str_tempPath, str_TracePath);
			if (strcmp(str_tempPath, "") != 0)
				strcat(str_tempPath, "\\");
			strcat(str_tempPath, FileData.cFileName);
			if (!DFSDeleteFile(str_LocalPath, str_tempPath))
			{
				FindClose(hList);
				return false;
			}
		}
	}
}

bool ITestProcessor::PullDeviceLogOut(bool b_res)
{
	if ((!b_res) && (m_i_QPhoneMultiCtrl == 0))
	{
		if (IsConnected())
			ExecAndroidLogcat(false);
	}
	else if (m_i_QPhoneMultiCtrl != 0)
	{
		if (IsConnectedMulti())
			ExecAndroidLogcat(false);
	}
	return true;
}

bool ITestProcessor::ShowPassFailOnDeviceLCD()
{
	char out_buf[FTD_BUF_SIZE] = {0};
	for (int nQPhoneNumber = 1; nQPhoneNumber <= m_i_QPhoneMultiCtrl; nQPhoneNumber++)
	{
		if ((m_b_isSlotPass[nQPhoneNumber] == true) && (m_b_UsableSlot[nQPhoneNumber] == true))
		{
			m_pIPhone->Display_LCD_RGB((2000 + nQPhoneNumber), m_nFtdTimeOut, "g", out_buf);
			CString csMsg;
			csMsg.Format(_T("Set QPhone%d LCD as GREEN color"), nQPhoneNumber);
			TraceLog(MSG_INFO, std::string(csMsg));
		}
	}
	return true;
}
