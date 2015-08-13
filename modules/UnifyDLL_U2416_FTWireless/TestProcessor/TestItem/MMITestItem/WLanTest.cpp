/*********************************************************************************
 * Description: .h file of CWLanTest object.
 * FileName   : WLanTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Eason.YC.LEE
 * Time       : 2014/08/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2014/08/12    Eason.YC.LEE            Create for QC project BBTest test
 ********************************************************************************/

#include "stdafx.h"
#include "WLanTest.h"


RF_IMPLEMENT_DYNCREATE(CWLanTest)


bool CWLanTest::PreRun()
{
	m_strResult = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = m_str_Value;
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CWLanTest::Run()
{
	if (m_str_Name == WLAN_Self_Test)
	{
		m_strItemCode = CStr::IntToStr(WLAN_SelfTest_BaseItemCode);
		//m_strErrorCode = FunErr_WLAN_SelfTest_Fail;
		m_strErrorCode = FunErr_WLAN_Hardware_Selftest_Fail;

		return WLANSelfTestTest();
	}
	else
	{
		m_strMessage = "The Type of ID is not defined";
		TraceLog(MSG_INFO, "The Type of ID is not defined");
		return false;
	}

}

bool CWLanTest::PostRun()
{
	return true;
}

bool CWLanTest::InitData(std::map<std::string, std::string>& paramMap)
{
	/* Name */
	if (paramMap.find(std::string("Name")) != paramMap.end())
	{
		m_str_Name = paramMap[std::string("Name")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'Name' for class 'CWLanTest'");
		return false;
	}


	/* Value */
	if (paramMap.find(std::string("Value")) != paramMap.end())
	{
		m_str_Value = paramMap[std::string("Value")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'Value' for class 'CWLanTest'");
		return false;
	}

	/* CheckMatch */
	m_b_CheckMatch = true;

	std::string str_CheckMatch;
	if (paramMap.find(std::string("CheckMatch")) != paramMap.end())
	{
		str_CheckMatch = paramMap[std::string("CheckMatch")];
		m_b_CheckMatch = CStr::StrToBool(str_CheckMatch);
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'CheckMatch' for class 'CStringCheckProc'");
		return false;
	}

	/* Sleep */
	std::string str_sleep;
	if (paramMap.find(std::string("Sleep")) != paramMap.end())
	{
		str_sleep = paramMap[std::string("Sleep")];
		m_i_sleep = CStr::StrToInt(str_sleep);
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'Sleep' for class 'CStringCheckProc'");
		return false;
	}
	return true;
}

bool CWLanTest::WLANSelfTestTest()
{
	bool b_res = false;
	std::string str_msg;

	/* read WLANSelf from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_WLAN_selfTest(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read WLANSelf from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare WLANSelf with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read WLANSelf = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			if (m_strMeasured == m_str_Value)
			{
				str_msg = "Check WLANSelf pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "WLANSelfis not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
				b_res = false;
			}
		}
	}
	Sleep(m_i_sleep);

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read WLANSelf PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read WLANSelf FAIL!");
	}
	SetPICSData("WLANSelf", m_strMeasured);
	FactoryLog();
	return b_res;
}
