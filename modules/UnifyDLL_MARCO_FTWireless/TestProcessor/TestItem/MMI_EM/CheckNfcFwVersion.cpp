/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .cpp file of CCheckNfcFwVersion object.
 * FileName   : CheckNfcFwVersion.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/03/15
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/03/15    Evonne.Hsieh            Create for Boston BB test
 ********************************************************************************/

#include "stdafx.h"
#include "CheckNfcFwVersion.h"


RF_IMPLEMENT_DYNCREATE(CCheckNfcFwVersion)


static string ltrim(const string& str)
{
	size_t at = str.find_first_not_of(" \t\r\n\0\a\b\f\v");
	return at == string::npos ? str : str.substr(at);
}

static string rtrim(const string& str)
{
	size_t at = str.find_last_not_of(" \t\r\n\0\a\b\f\v");
	return at == string::npos ? str : str.substr(0, at + 1);
}

static string trim(const string& str0)
{
	string str = str0;
	size_t at2 = str.find_last_not_of(" \t\r\n\0\a\b\f\v");
	size_t at1 = str.find_first_not_of(" \t\r\n\0\a\b\f\v");
	if (at2 != string::npos) str.erase(at2 + 1);
	if (at1 != string::npos) str.erase(0, at1);
	return str;
}

bool CCheckNfcFwVersion::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = FunErr_NFC_FW_Version_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CCheckNfcFwVersion::Run()
{
	return CheckNfcFwVersion();
}

bool CCheckNfcFwVersion::PostRun()
{
	return true;
}

bool CCheckNfcFwVersion::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find(std::string("FWVersion")) != paramMap.end())
	{
		m_str_FwVersion = paramMap[std::string("FWVersion")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get FW version for class 'CCheckNfcFwVersion'");
		return false;
	}

	return true;
}

bool CCheckNfcFwVersion::CheckNfcFwVersion()
{
	bool b_res = true;
	std::string str_msg;

	/* read LSensor ID from mobile */
	char sz_inData[FTD_BUF_SIZE] = {0};
	char sz_FwVersion[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_NFCFWVersion(m_nFtdPort, m_nFtdTimeOut, sz_inData, sz_FwVersion))
	{
		str_msg = "Read FW version from mobile fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare FW version with XML file */
	if (b_res)
	{
		//char* delim = ",";
		//char* pch;
		//std::string token;
		m_strMeasured = sz_FwVersion;
		//pch = strtok(sz_FwVersion, delim);
		//while (pch != NULL)
		//{
			//token = pch;
			//if (m_str_FwVersion.compare(trim(token)) == 0)
			if (m_str_FwVersion.compare(trim(sz_FwVersion)) == 0)
			{
				str_msg = "FW version matched. Version = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
				b_res = true;
				//break;
			}
			else
			{
				str_msg = "FW version is not matched. Moble:" + m_strMeasured + "-- XML File:" + m_str_FwVersion;
				TraceLog(MSG_INFO, str_msg);
				b_res = false;
			}
			//pch = strtok(NULL, delim);
		//}
	}

	/* No Need in T47 */
	//char Output[50000] = {0};
	//char ErrorCode[50000] = {0};
	//CString Command = _T("shell am broadcast -a android.intent.action.nfc.power --ez state true");
	//if (!m_pIPhone->ExecAdbCommand(Command, Output, ErrorCode))
	//{
	//	str_msg = "ExecAdbOut Command fail";
	//	TraceLog(MSG_INFO, str_msg);
	//	m_strMessage = "ExecAdbOut Command fail.  ";
	//}
	//Sleep(3000);

	/* test result */
	m_strMessage = str_msg;
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		TraceLog(MSG_INFO, "Check FW version PASS!");
		SetPICSData("MMIEM_RESULT", "CheckFWVersion_PASS");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_INFO, "Check FW version FAIL!");
		SetPICSData("MMIEM_RESULT", "CheckFWVersion_FAIL");
	}
	FactoryLog();
	return b_res;
}
