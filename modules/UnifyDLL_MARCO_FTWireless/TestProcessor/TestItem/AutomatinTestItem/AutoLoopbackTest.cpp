/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of AutoLoopback object.
 * FileName   : LoopbackTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "AutoLoopbackTest.h"


RF_IMPLEMENT_DYNCREATE(AutoLoopback)

using namespace std;

bool AutoLoopback::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(Loopback_Test_BaseItemCode);
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool AutoLoopback::Run()
{
	return LoopbackTest();
}

bool AutoLoopback::PostRun()
{
	return true;
}

bool AutoLoopback::InitData(std::map<std::string, std::string>& paramMap)
{
	/* PatternIndex */
	if (paramMap.find(std::string("OnOff")) != paramMap.end())
	{
		m_str_OnOff = paramMap[std::string("OnOff")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter OnOff for AutoLoopback");
		return false;
	}

	if (paramMap.find(std::string("DeviceFrom")) != paramMap.end())
	{
		m_str_DeviceFrom = paramMap[std::string("DeviceFrom")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter DeviceFrom for AutoLoopback");
		return false;
	}

	if (paramMap.find(std::string("VolumeValFrom")) != paramMap.end())
	{
		m_str_VolumeValFrom = paramMap[std::string("VolumeValFrom")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter VolumeValFrom for AutoLoopback");
		return false;
	}

	if (paramMap.find(std::string("DeviceTo")) != paramMap.end())
	{
		m_str_DeviceTo = paramMap[std::string("DeviceTo")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter DeviceTo for AutoLoopback");
		return false;
	}

	if (paramMap.find(std::string("VolumeValTo")) != paramMap.end())
	{
		m_str_VolumeValTo = paramMap[std::string("VolumeValTo")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter VolumeValTo for AutoLoopback");
		return false;
	}
	return true;
}

bool AutoLoopback::LoopbackTest()
{
	std::string str_msg;
	bool b_res = true;
	if (b_res)
	{
		char sz_patternLoopback[256] = {0};
		strcpy(sz_patternLoopback, m_str_OnOff.c_str());
		strcat(sz_patternLoopback, ",");
		strcat(sz_patternLoopback, m_str_DeviceFrom.c_str());
		strcat(sz_patternLoopback, ",");
		strcat(sz_patternLoopback, m_str_VolumeValFrom.c_str());
		strcat(sz_patternLoopback, ",");
		strcat(sz_patternLoopback, m_str_DeviceTo.c_str());
		strcat(sz_patternLoopback, ",");
		strcat(sz_patternLoopback, m_str_VolumeValTo.c_str());
		char szOutput[FTD_BUF_SIZE] = {0};
		b_res = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternLoopback, szOutput);
		if (!b_res)
		{
			str_msg = "FTD_AUloopbackTest command fail";
			TraceLog(MSG_INFO,str_msg);
		}
		else
		{
			str_msg = "FTD_AUloopbackTest command success";
			TraceLog(MSG_INFO,str_msg);
		}
	}

	/* Query operator if display pattern is required pattern */
	if (b_res)
	{
		if (m_str_OnOff.find(_T("on")) != string::npos)
		{
			if (m_str_DeviceFrom.find("headmic") != string::npos && m_str_DeviceTo.find("headset") != string::npos)
			{
				str_msg = CW2A(L"Please blow on headmic. Whether there is sound on headset?\n请对耳机麦克风吹气,确认耳机听筒是否有声音?\n");
			}
			else if(m_str_DeviceFrom.find("front") != string::npos && m_str_DeviceTo.find("headset") != string::npos)
			{
				str_msg = CW2A(L"Please blow on mic. Whether there is sound on headset?\n请对麦克风吹气,确认耳机听筒是否有声音?\n");
			}
			else if(m_str_DeviceFrom.find("back") != string::npos && m_str_DeviceTo.find("headset") != string::npos)
			{
				str_msg = CW2A(L"Please blow on mic. Whether there is sound on headset?\n请对背面麦克风吹气,确认耳机听筒是否有声音?\n");
			}

			/* Init dialog response flag to false */
			m_b_DialogResponse = false;
			m_b_waitFlag = false;

			/* Notify UI to show message dialog */
			ShowDialog(str_msg.c_str());

			/* Wait UI response(Max:10s) */
			int i_waitCount = 0;
			while (m_b_waitFlag != true)
			{
				if (i_waitCount == 50)
				{
					str_msg = _T("Wait UI response time out.");
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
					str_msg = "Loopback Test success.";
					TraceLog(MSG_INFO, str_msg);
				}
				else
				{
					b_res = false;
					str_msg = "Loopback Test fail.";
					TraceLog(MSG_INFO, str_msg);
				}
			}
		}
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		str_msg = "Loopback " + m_str_OnOff + " Test success!";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
		if (m_str_DeviceFrom.compare("headmic") == 0)
			SetPICSData("HEADMIC_TEST", "PASS");
		if (m_str_DeviceFrom.compare("front") == 0)
			SetPICSData("FRONTMIC_TEST", "PASS");
		if (m_str_DeviceFrom.compare("back") == 0)
			SetPICSData("BACKMIC_TEST", "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		str_msg = "Loopback " + m_str_OnOff + " Test fail!";
		TraceLog(MSG_INFO, str_msg);
		if (m_str_DeviceFrom.compare("headmic") == 0)
		{
			m_strMessage = "Head MIC Loopback Test fail.";
			m_strErrorCode = FunErr_Loopback_Headmic_To_Headset_Fail;
			SetPICSData("HEADMIC_TEST", "FAIL");
		}
		if (m_str_DeviceFrom.compare("front") == 0)
		{
			m_strMessage = "Front MIC Loopback Test fail.";
			m_strErrorCode = FunErr_Loopback_Mic_To_Headset_Fail;
			SetPICSData("FRONTMIC_TEST", "FAIL");
		}
		if (m_str_DeviceFrom.compare("back") == 0)
		{
			m_strMessage = "Back MIC Loopback Test fail.";
			m_strErrorCode = FunErr_Loopback_Mic2_To_Headset_Fail;
			SetPICSData("BACKMIC_TEST", "FAIL");
		}
	}
	FactoryLog();
	return b_res;
}
