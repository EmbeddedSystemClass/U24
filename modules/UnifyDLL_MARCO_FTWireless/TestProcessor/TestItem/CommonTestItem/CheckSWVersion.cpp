#include "stdafx.h"
#include "CheckSWVersion.h"


RF_IMPLEMENT_DYNCREATE(CCheckSWVersion)

bool CCheckSWVersion::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CCheckSWVersion::Run()
{
	bool bRes = MainFunction();
	m_strItemCode = CStr::IntToStr(CheckSWVersionMT_BaseItemCode);
	return bRes;
}

bool CCheckSWVersion::PostRun()
{
	return true;
}

bool CCheckSWVersion::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string str;
	/* WaitTime */
	if (paramMap.find(std::string("WaitTime")) != paramMap.end())
	{
		str = paramMap[std::string("WaitTime")];
		m_msWaitTime = CStr::StrToInt(str);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'WaitTime' for class 'CCheckSWVersion'"));
		::AfxMessageBox(_T("AdbShellStart get WaitTime fail"));
		return false;
	}

	// VersionType //
	if (paramMap.find(std::string("VersionType")) != paramMap.end())
	{
		m_VersionType = paramMap[std::string("VersionType")].c_str();
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'VersionType' for class 'CCheckSWVersion'"));
		return false;
	}

	return true;
}

bool CCheckSWVersion::MainFunction()
{
	if (!m_pIPhone) return false;

	if (!m_pIPhone->Initial())
	{
		m_strMessage = "Fail to Initial IPhone,Loading Qcomdiag fail!";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	if (!m_pIPhone->IsConnected())
	{
		m_strMessage = "Fail to connect phone with Qcomdiag dll" ;
		TraceLog(MSG_ERROR, m_strMessage) ;
		return false;
	}

	char output[100] = {0};
	char error[100] = {0};
	if (!m_pIPhone->ExecAdbCommand("shell getprop ro.build.oemversion.main", output, error))
	{
		m_strMessage = "Get Software Version fail";
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Get_Software_Version_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}

	for (int i = 0; i < 100; i++)
	{
		if ((output[i] == 0x0A) || (output[i] == 0x0D))
			output[i] = 0x00;
	}

	//-- Check SW Version --//
	char SWver[1024];
	if (strcmp(m_VersionType.c_str(), "Factory") == 0)
	{
		GetFactoryVersion(SWver);
	}
	else if (strcmp(m_VersionType.c_str(), "Ship") == 0)
	{
		GetSWVersion(SWver);
	}
	else
	{
		m_strMessage = "Get XML Version Type fail";
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Get_Software_Version_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}

	StrVtr vToken;
	CStr::ParseString(SWver, _T(","), vToken);
	bool b_match = false;
	for (size_t i = 0; i < vToken.size(); i++)
	{
		if (strstr(output, vToken[i].c_str()) != NULL)
		{
			b_match = true;
			break;
		}
	}

	char msg[1024];
	if (!b_match)
	{
		strcpy(msg, "SWVer are diff. Server=");
		strcat(msg, SWver);
		m_strMessage = msg;
		m_strResult = "FAIL";
		m_strMeasured = output;
		m_strErrorCode = CommErr_Check_Software_Version_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}

	strcpy(msg, "Server=");
	strcat(msg, SWver);
	m_strMessage = msg;
	m_strResult = "PASS";
	m_strErrorCode = "-";
	m_strMeasured = output;
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return true;
}
