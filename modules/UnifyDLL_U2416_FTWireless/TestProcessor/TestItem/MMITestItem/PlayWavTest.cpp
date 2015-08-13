/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CPlayWav object.
 * FileName   : PlayWavTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "PlayWavTest.h"


RF_IMPLEMENT_DYNCREATE(CPlayWav)


bool CPlayWav::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(PlayWav_Test_BaseItemCode);
	m_strErrorCode = "";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CPlayWav::Run()
{
	if (m_str_Device.find("speaker") != string::npos)
	{
		m_strErrorCode = FunErr_Speaker_Play_Stop_Fail;
	}
	else if (m_str_Device.find("handset") != string::npos || (m_str_Device.find("receiver") != string::npos))
	{
		m_strErrorCode = FunErr_Headset_Speaker_Play_Stop_Fail;
	}
	else if (m_str_Device.find(_T("headset")) != string::npos)
	{
		m_strErrorCode = FunErr_Headset_Speaker_Play_Stop_Fail;
	}

	return PlayWavTest();
}

bool CPlayWav::PostRun()
{
	return true;
}

bool CPlayWav::InitData(std::map<std::string, std::string>& paramMap)
{
	/* PatternIndex */
	if (paramMap.find(std::string("OnOff")) != paramMap.end())
	{
		m_str_OnOff = paramMap[std::string("OnOff")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter OnOff for CPlayWav"));
		return false;
	}

	if (paramMap.find(std::string("Device")) != paramMap.end())
	{
		m_str_Device = paramMap[std::string("Device")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter Device for CPlayWav"));
		return false;
	}

	if (paramMap.find(std::string("VolumeVal")) != paramMap.end())
	{
		m_str_VolumeVal = paramMap[std::string("VolumeVal")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter VolumeVal for CPlayWav"));
		return false;
	}

	std::string str_SleepTime;
	if (paramMap.find(std::string("SleepTime")) != paramMap.end())
	{
		str_SleepTime = paramMap[std::string("SleepTime")];
		m_i_SleepTime = CStr::StrToInt(str_SleepTime);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter SleepTime for CPlayWav"));
		return false;
	}

	return true;
}

bool CPlayWav::PlayWavTest()
{
	bool b_res = true;
	std::string str_OnOff_English;
	std::string str_OnOff_Chinese;
	std::string str_device_Chinese;
	std::string str_Pics;
	std::string str_msg;

	/* Get Play Wav pattern */
	if (m_str_OnOff.compare("on") == 0)
	{
		str_OnOff_English = "On";
		str_OnOff_Chinese = CW2A(L"开启");
	}
	else if (m_str_OnOff.compare("off") == 0)
	{
		str_OnOff_English = "Off";
		str_OnOff_Chinese = CW2A(L"关闭");
	}
	else
	{
		str_msg = "required " + m_str_Device + " on/off is not defined.";
		TraceLog(MSG_INFO,str_msg);
		b_res = false;
	}

	if (m_str_Device.compare("speaker") == 0)
	{
		str_device_Chinese = CW2A(L"免提");
		str_Pics = "SPEAKER_ON_TEST";
	}
	else if (m_str_Device.compare("headset") == 0)
	{
		str_device_Chinese = CW2A(L"耳机");
		str_Pics = "HEADSET_ON_TEST";
	}
	else if (m_str_Device.compare("handset") == 0 || (m_str_Device.compare("receiver") == 0))
	{
		str_device_Chinese = CW2A(L"听筒");
		str_Pics = "RECEIVER_ON_TEST";
	}
	else
	{
		str_msg = "required " + m_str_Device + " on/off is not defined.";
		TraceLog(MSG_INFO,str_msg);
		b_res = false;
	}

	if (b_res)
	{
		char sz_patternPlayWav[FTD_BUF_SIZE] = {0};
		strcpy(sz_patternPlayWav, m_str_OnOff.c_str());
		strcat(sz_patternPlayWav, ",");
		strcat(sz_patternPlayWav, m_str_Device.c_str());
		strcat(sz_patternPlayWav, ",");
		strcat(sz_patternPlayWav, m_str_VolumeVal.c_str());
		char sz_outBuffer[FTD_BUF_SIZE] = {0};
		b_res = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_patternPlayWav, sz_outBuffer);
		if (!b_res)
		{
			str_msg = "Fail to turn " + m_str_Device + " on/off ";
			TraceLog(MSG_INFO,str_msg);
		}
		else
		{
			Sleep(m_i_SleepTime);
			str_msg = "Mobile " + m_str_Device + " turn on/off success";
			TraceLog(MSG_INFO,str_msg);
		}
	}

	/* Query operator if display pattern is required pattern */
	if (b_res)
	{
		if (m_str_OnOff.find(_T("on")) != string::npos)
		{
			str_msg = std::string("Is ") + m_str_Device + " " + str_OnOff_English + "?\n" +
				std::string(CW2A(L"手机的")) + str_device_Chinese + std::string(CW2A(L"是")) + str_OnOff_Chinese + std::string(CW2A(L"吗？"));

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
					str_msg = "Wait UI response time out.";
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
					str_msg = "The " + m_str_Device + " on/off is required pattern.";
					TraceLog(MSG_INFO, str_msg);
				}
				else
				{
					b_res = false;
					str_msg = "The " + m_str_Device + " on/off is not required pattern.";
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
		str_msg = m_str_Device + " " + str_OnOff_English + " pass!";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		str_msg = m_str_Device + " " + str_OnOff_English + " fail!";
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "FAIL");
	}
	FactoryLog();
	return b_res;
}
