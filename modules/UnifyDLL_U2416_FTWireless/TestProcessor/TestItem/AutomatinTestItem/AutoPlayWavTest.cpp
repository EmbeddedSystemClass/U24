/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of AutoPlayWav object.
 * FileName   : PlayWavTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "AutoPlayWavTest.h"


RF_IMPLEMENT_DYNCREATE(AutoPlayWav)

using namespace std;

bool AutoPlayWav::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(PlayWav_Test_BaseItemCode);
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";
	m_strErrorCode = "-";
	return true;
}

bool AutoPlayWav::Run()
{
	return PlayWavTest();
}

bool AutoPlayWav::PostRun()
{
	return true;
}

bool AutoPlayWav::InitData(std::map<std::string, std::string>& paramMap)
{
	/* PatternIndex */
	if (paramMap.find(std::string("OnOff")) != paramMap.end())
	{
		m_str_OnOff = paramMap[std::string("OnOff")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter OnOff for AutoPlayWav"));
		return false;
	}

	if (paramMap.find(std::string("Device")) != paramMap.end())
	{
		m_str_Device = paramMap[std::string("Device")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter Device for AutoPlayWav"));
		return false;
	}

	if (paramMap.find(std::string("VolumeVal")) != paramMap.end())
	{
		m_str_VolumeVal = paramMap[std::string("VolumeVal")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter VolumeVal for AutoPlayWav"));
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
		TraceLog(MSG_INFO, _T("Fail to find parameter SleepTime for AutoPlayWav"));
		return false;
	}

	if (paramMap.find(std::string("Path")) != paramMap.end())
	{
		m_str_Path = paramMap[std::string("Path")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter wav path for AutoPlayWav"));
		return false;
	}

	if (paramMap.find(std::string("PushMusic")) != paramMap.end())
	{
		m_b_PushMusic = CStr::StrToBool(paramMap[std::string("PushMusic")]);
	}
	else
	{
		m_b_PushMusic = false;
		TraceLog(MSG_INFO, _T("Default to set PushMusic to FALSE"));
	}

	if (m_b_PushMusic)
	{
		if (paramMap.find(std::string("MusicName")) != paramMap.end())
		{
			m_str_MusicName = paramMap[std::string("MusicName")];
		}
		else
		{
			TraceLog(MSG_INFO, _T("Fail to set MusicName "));
			return false;
		}
	}

	return true;
}

bool AutoPlayWav::PlayWavTest()
{
	bool b_res = true;
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	std::string str_OnOff_English;
	std::string str_OnOff_Chinese;
	std::string str_device_Chinese;
	std::string str_msg;
	/*
		if (m_str_OnOff.find(_T("on")) != string::npos &&  m_str_Device.compare("receiver")) == 0)
		{
			str_msg = "Please pick-up the mobile.\n请拿起手机.";
			strncpy_s(sz_msg, sizeof(sz_msg), str_msg.c_str(), sizeof(sz_msg));

			// Init dialog response flag to false /
			m_b_DialogResponse = false;
			m_b_waitFlag = false;

			// Notify UI to show message dialog /
			ShowDialog(sz_msg);

			// Wait UI response(Max:10s) /
			int i_waitCount = 0;
			while (m_b_waitFlag != true)
			{
				if (i_waitCount == 50)
				{
					str_msg = "Wait UI response time out.";
					TraceLog(MSG_INFO, "Wait UI response time out.");
					m_strMessage = str_msg;
					TraceLog(MSG_ERROR, m_strMessage);
					FactoryLog();
					//break;
					return false;
				}
				else
				{
					i_waitCount++;
					Sleep(200);
					continue;
				}
			}
			if(b_res)
			{
				// Get UI dialog operator response /
				if (m_b_DialogResponse == true)
				{
					str_msg = "The operator did the action.";
					TraceLog(MSG_INFO, str_msg);
				}
				else
				{
					//b_res = false;
					str_msg = "The operator didn't action.";
					TraceLog(MSG_INFO, str_msg);
					m_strMessage = str_msg;
					TraceLog(MSG_ERROR, m_strMessage);
					FactoryLog();
					return false;
				}
			}

		}
	*/

	if (m_b_PushMusic)
	{
		char szOutPullCommand[50000] = {0};
		char szErrorCodePullCommand[50000] = {0};
		char csCommandShell[50000] = {0};
		strcpy(csCommandShell, "push ");
		strcat(csCommandShell, m_str_MusicName.c_str());
		strcat(csCommandShell, " ");
		strcat(csCommandShell, m_str_Path.c_str());

		if (!m_pIPhone->ExecAdbCommand(csCommandShell, szOutPullCommand, szErrorCodePullCommand))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_Audio_Push_Wav_Fail;
			m_strMessage = _T("Push music fail.");
			TraceLog(MSG_ERROR, m_strMessage);
			FactoryLog();
			return false;
		}
		else
		{
			m_strMessage = _T("Push music as record wav pass.");
			TraceLog(MSG_INFO, m_strMessage);
			FactoryLog();
		}
	}

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
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	if (m_str_Device.compare("speaker") == 0)
	{
		str_device_Chinese = CW2A(L"免提");
	}
	else if (m_str_Device.compare("headset") == 0)
	{
		str_device_Chinese = CW2A(L"耳机");
	}
	else if (m_str_Device.compare("handset") == 0 || (m_str_Device.compare("receiver") == 0))
	{
		str_device_Chinese = CW2A(L"听筒");
	}
	else
	{
		str_msg = "required " + m_str_Device + " on/off is not defined.";
		TraceLog(MSG_INFO, str_msg);
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
		if (!(m_str_Path.compare("0") == 0))
		{
			strcat(sz_patternPlayWav, ",");
			strcat(sz_patternPlayWav, m_str_Path.c_str());
		}
		b_res = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_patternPlayWav, sz_outBuffer);
		if (!b_res)
		{
			str_msg = "Fail to turn " + m_str_Device + " on/off ";
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			Sleep(m_i_SleepTime);
			str_msg = "Mobile " + m_str_Device + " turn on/off success";
			TraceLog(MSG_INFO, str_msg);
		}
	}

	/* Query operator if display pattern is required pattern */
	if (b_res)
	{
		if (m_str_OnOff.find("on") != string::npos)
		{
			str_msg = std::string("Is ") + m_str_Device + " " + str_OnOff_English + "?\n";
			str_msg += std::string(CW2A(L"手机的")) + str_device_Chinese + std::string(CW2A(L"是")) + str_OnOff_Chinese + std::string(CW2A(L"吗？"));

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
					str_msg = _T("The ") + m_str_Device + _T(" on/off is required pattern.");
					TraceLog(MSG_INFO, str_msg);
				}
				else
				{
					b_res = false;
					str_msg = _T("The ") + m_str_Device + _T(" on/off is not required pattern.");
					TraceLog(MSG_INFO, str_msg);
				}
			}
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		str_msg = m_str_Device + " " + str_OnOff_English + " pass!";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
		if (m_str_Device.compare("receiver") == 0)
			SetPICSData("RECEIVER_PLAY", "PASS");
		if (m_str_Device.compare("speaker") == 0)
			SetPICSData("SPEAKER_PLAY", "PASS");
		if (m_str_Device.compare("headset") == 0)
			SetPICSData("HEADSET_PLAY", "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		str_msg = m_str_Device + " " + str_OnOff_English + " fail!";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
		if (m_str_Device.compare("receiver") == 0)
		{
			m_strErrorCode = FunErr_Receiver_Play_Stop_Fail;
			SetPICSData("RECEIVER_PLAY", "FAIL");
		}
		if (m_str_Device.compare("speaker") == 0)
		{
			m_strErrorCode = FunErr_Speaker_Play_Fail;
			SetPICSData("SPEAKER_PLAY", "FAIL");
		}
		if (m_str_Device.compare("headset") == 0)
		{
			m_strErrorCode = FunErr_Headset_Speaker_Play_Stop_Fail;
			SetPICSData("HEADSET_PLAY", "FAIL");
		}
	}
	FactoryLog();

	return b_res;
}
