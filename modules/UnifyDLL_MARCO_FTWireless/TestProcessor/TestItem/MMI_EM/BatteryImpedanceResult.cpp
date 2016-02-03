/*********************************************************************************
 * COPYRIGHT Qisda WE02 2013
 *
 * Description: .cpp file of CBatteryImpedanceResult object.
 * FileName   : BatteryImpedanceResult.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2014/12/4
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2014/12/4       Neil.Du                 Create for Boston BB test
 ********************************************************************************/

#include "stdafx.h"
#include "BatteryImpedanceResult.h"
#include <direct.h>


RF_IMPLEMENT_DYNCREATE(CBatteryImpedanceResult)


bool CBatteryImpedanceResult::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = CommErr_Pull_File_From_Device_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CBatteryImpedanceResult::Run()
{
	return GetBIResult();
}

bool CBatteryImpedanceResult::PostRun()
{
	return true;
}

bool CBatteryImpedanceResult::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find(std::string("BIResultPath")) != paramMap.end())
	{
		m_str_BIResultPath = paramMap[std::string("BIResultPath")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to BIResultPath for class 'CBatteryImpedanceResult'");
		return false;
	}

	if (paramMap.find(std::string("LogFolderPath")) != paramMap.end())
	{
		m_str_LogFolderPath = paramMap[std::string("LogFolderPath")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to LogFolderPath for class 'CBatteryImpedanceResult'");
		return false;
	}

	return true;
}

bool CBatteryImpedanceResult::GetBIResult()
{
	std::string str_msg;

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	char sz_file_name[4096] = {0};
	sprintf(sz_file_name, "BI_%s_%04d%02d%02d_%02d%02d_%02d.txt", g_strPicasso.c_str(),
		systemTime.wYear, systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

	char in_string[4096] = {0};
	sprintf(in_string, "pull %s %s\\%s", m_str_BIResultPath.c_str(), m_str_LogFolderPath.c_str(), sz_file_name);
	str_msg = in_string;
	TraceLog(MSG_INFO, str_msg);
	
	_mkdir( m_str_LogFolderPath.c_str() );
	
	bool bRet = true;
	char out_string[4096] = {0};
	char err_string[4096] = {0};
	if (!m_pIPhone->ExecAdbCommand(in_string, out_string, err_string))
	{
		str_msg = "Adb pull fail.";
		TraceLog(MSG_INFO, str_msg);
		bRet = false;
	}
	TraceLog(MSG_INFO, out_string);
	TraceLog(MSG_INFO, err_string);

	/* test result */
	m_strMessage = str_msg;
	if(bRet)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		TraceLog(MSG_INFO, "Get BatteryImpedanceResult PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_INFO, "Get BatteryImpedanceResult FAIL!");
	}
	FactoryLog();
	return bRet;
}
