/*********************************************************************************
 * COPYRIGHT Qisda WR24 2011
 *
 * Description: .h file of CSleepCtrl object.
 * FileName   : CSleepCtrl.h
 * Version    : $Revision:   1.0  $
 * Author     : Neil.Du
 * Time       : 2011/11/28
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/11/28      Neil.Du             Create.
 ********************************************************************************/

#include "stdafx.h"
#include "SleepCtrl.h"


RF_IMPLEMENT_DYNCREATE(CSleepCtrl)


bool CSleepCtrl::PreRun()
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

bool CSleepCtrl::Run()
{
	return SleepCtrl();
}

bool CSleepCtrl::PostRun()
{
	return true;
}

bool CSleepCtrl::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string str_sleep;
	if (paramMap.find(std::string("Sleep")) != paramMap.end())
	{
		str_sleep = paramMap[std::string("Sleep")];
		m_i_sleep = CStr::StrToInt(str_sleep);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'Sleep' for class 'CSleepCtrl'"));
		return false;
	}
	return true;
}

bool CSleepCtrl::SleepCtrl()
{
	Sleep(m_i_sleep);
	return true;
}