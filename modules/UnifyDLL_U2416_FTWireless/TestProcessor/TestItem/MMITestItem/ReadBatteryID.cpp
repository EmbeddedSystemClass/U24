/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 * Author     : Lion.Wu
 ********************************************************************************/

#include "stdafx.h"
#include "ReadBatteryID.h"
#include <string>

template <class T> 
std::string ConvertToString(T);

RF_IMPLEMENT_DYNCREATE(CreadBatteryID)

template <class T> 
std::string ConvertToString(T value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

bool CreadBatteryID::PreRun()
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

bool CreadBatteryID::Run()
{
	/* judge sensor type, then run different API function based on type */
	if (m_str_IDType == BATTERY)
	{
		m_strItemCode = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_Check_Battery_ID_Fail;
		return BatteryCheckID();
	}
	else
	{
		m_strMessage = "The Type of ID is not defined";
		TraceLog(MSG_INFO, "The Type of ID is not defined");
		return false;
	}
	return true;
}

bool CreadBatteryID::PostRun()
{
	return true;
}

bool CreadBatteryID::InitData(std::map<std::string, std::string>& paramMap)
{
	/* SensorType */
	if (paramMap.find(std::string("IDType")) != paramMap.end())
	{
		m_str_IDType = paramMap[std::string("IDType")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'IDType' for class 'CreadBatteryID'");
		return false;
	}

	if (paramMap.find(std::string("Battery_IDRange")) != paramMap.end())
	{
		std_BatteryID = paramMap[std::string("Battery_IDRange")];
		sscanf(std_BatteryID.c_str(),"%d,%d", &nBatteryID_lower, &nBatteryID_upper);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Battery_IDRange for CreadBatteryID");
	}

	return true;
}

bool CreadBatteryID::BatteryCheckID()
{
	bool b_res = true;
	std::string str_msg;

	/* read ID from mobile */
	char szId[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->ReadBatteryID(m_nFtdPort, m_nFtdTimeOut, szId))
	{
		str_msg = "Read Battery ID fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}
	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = szId;
		int nCheck = atoi(m_strMeasured.c_str());
		if ( (nCheck > nBatteryID_lower) && ( nCheck < nBatteryID_upper))
		{
			str_msg = "Check Battery ID pass! ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "Battery ID is not in range. Moble:" + m_strMeasured + ",Config:" 
				+  ConvertToString(nBatteryID_lower) + "," + ConvertToString(nBatteryID_upper);
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Battery ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Battery ID FAIL!");
	}
	SetPICSData("BatteryID", m_strMeasured);
	FactoryLog();
	return b_res;
}
