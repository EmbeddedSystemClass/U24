/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .cpp file of CNFCPowerOffCardMode object.
 * FileName   : NFCPowerOffCardMode.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/03/15
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/03/15    Evonne.Hsieh            Create for Boston BB test
 ********************************************************************************/

#include "stdafx.h"
#include "NFCPowerOffCardMode.h"


RF_IMPLEMENT_DYNCREATE(CNFCPowerOffCardMode)

bool CNFCPowerOffCardMode::PreRun()
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

bool CNFCPowerOffCardMode::Run()
{
	return NFCPowerOffCardModeTest();
}

bool CNFCPowerOffCardMode::PostRun()
{
	return true;
}

bool CNFCPowerOffCardMode::InitData(std::map<std::string, std::string>& paramMap)
{
	/* Offset */
	if (paramMap.find(std::string("Distance")) != paramMap.end())
	{
		m_str_Distance = paramMap[std::string("Distance")];
		m_i_Distance = CStr::StrToFloat(m_str_Distance);
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get 'Distance' for class 'CNFCPowerOffCardMode'");
		return false;
	}

	if (paramMap.find(std::string("Message")) != paramMap.end())
	{
		m_str_Message = paramMap[std::string("Message")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get 'Message' for class 'CNFCPowerOffCardMode'");
		return false;
	}

	if (paramMap.find(std::string("ErrorCode")) != paramMap.end())
	{
		m_str_ErrorCode = paramMap[std::string("ErrorCode")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get 'ErrorCode' for class 'CNFCPowerOffCardMode'");
		return false;
	}

	return true;
}

bool CNFCPowerOffCardMode::NFCPowerOffCardModeTest()
{
	bool b_res = true;
	/* Init dialog response flag to false */
	m_b_DialogResponse = false;
	m_b_waitFlag = false;
	/* Notify UI to show message dialog */
	if(m_i_Distance == 0)
		ShowDialog(CW2A(L"请移除USB线并按手机画面关机键或是按电源键正常关机"));

	char message[512] = {0};
	sprintf(message, CW2A(L"请将手机放到NFC %.2f公分读卡机上,请问读卡机是否显示绿灯pass?"), m_i_Distance);
	ShowDialog(message);
	
	/* Wait UI response(Max:10s) */
	int i_waitCount = 0;
	while (m_b_waitFlag != true)
	{
		if (i_waitCount == 50)
		{
			TraceLog(MSG_INFO, _T("Wait UI response time out."));
			b_res = false;
			break;
		}
		else
		{
			i_waitCount++;
			Sleep(200);
			continue;
		}
	}

	if (b_res)
	{
		/* Get UI dialog operator response */
		if (m_b_DialogResponse == true)
		{
			m_strResult = "PASS";
			TraceLog(MSG_INFO, m_str_Message + "_PASS");
			SetPICSData(_T("MMIEM_RESULT"), m_str_Message + "_PASS");
		}
		else
		{
			m_strResult = "FAIL";
			b_res = false;
			m_strErrorCode = m_str_ErrorCode;
			TraceLog(MSG_INFO, m_str_Message + "_FAIL");
			SetPICSData(_T("MMIEM_RESULT"), m_str_ErrorCode);
		}
	}

	FactoryLog();
	return b_res;
}
