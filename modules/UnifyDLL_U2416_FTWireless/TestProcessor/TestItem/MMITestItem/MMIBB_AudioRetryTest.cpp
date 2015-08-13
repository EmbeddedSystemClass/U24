/*********************************************************************************
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1   2014    Lion.Wu            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "MMIBB_AudioRetryTest.h"


RF_IMPLEMENT_DYNCREATE(CMMIBB_AudioRetryTest)


bool CMMIBB_AudioRetryTest::PreRun()
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

bool CMMIBB_AudioRetryTest::PostRun()
{
	return true;
}

bool CMMIBB_AudioRetryTest::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find(std::string("AudtioTestItem")) != paramMap.end())
	{
		m_str_AudioTestItem = paramMap[std::string("AudtioTestItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter AudtioTestItem for CMMIBB_AudioRetryTest");
		return false;
	}

	if (paramMap.find(std::string("AudtioCMDItem")) != paramMap.end())
	{
		m_str_AudioCMD = paramMap[std::string("AudtioCMDItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter AudtioCMDItem for CMMIBB_AudioRetryTest");
		return false;
	}

	if (paramMap.find(std::string("AudtioOffCMDItem")) != paramMap.end())
	{
		m_str_AudioOffCMD = paramMap[std::string("AudtioOffCMDItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter AudtioOffCMDItem for CMMIBB_AudioRetryTest");
		return false;
	}

	std::string str_SleepTime;
	if (paramMap.find(std::string("OnSleepTime")) != paramMap.end())
	{
		str_SleepTime = paramMap[std::string("OnSleepTime")];
		m_i_OnSleepTime = CStr::StrToInt(str_SleepTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter OnSleepTime for CMMIBB_AudioRetryTest");
		return false;
	}

	if (paramMap.find(std::string("OffSleepTime")) != paramMap.end())
	{
		str_SleepTime = paramMap[std::string("OffSleepTime")];
		m_i_OffSleepTime = CStr::StrToInt(str_SleepTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter OffSleepTime for CMMIBB_AudioRetryTest");
		return false;
	}

	if (paramMap.find(std::string("RetryTime")) != paramMap.end())
	{
		str_SleepTime = paramMap[std::string("RetryTime")];
		m_i_MexRetryTime = CStr::StrToInt(str_SleepTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter RetryTime for CMMIBB_AudioRetryTest");
		return false;
	}


	if (paramMap.find(std::string("Path")) != paramMap.end())
	{
		m_str_Path = paramMap[std::string("Path")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter wav path for CMMIBB_AudioRetryTest");
		return false;
	}

	return true;
}

bool CMMIBB_AudioRetryTest::Run()
{
	bool passFail = false;
	CString cs_AudtioTestItem = m_str_AudioTestItem.c_str();

	if (cs_AudtioTestItem.Compare("SpeakerOnTest") == 0)
	{
		m_strErrorCode = FunErr_Speaker_Play_Fail;
		passFail = runSpeakerOnTest();
	}
	else if (cs_AudtioTestItem.Compare("HeadsetOnTest") == 0)
	{
		m_strErrorCode = FunErr_Headset_Speaker_Test_Fail;
		passFail = runHeadsetOnTest();
	}
	else if (cs_AudtioTestItem.Compare("ReceiverOnTest") == 0)
	{
		m_strErrorCode = FunErr_Receiver_Play_Fail;
		passFail = runReceiverOnTest();
	}
	else if (cs_AudtioTestItem.Compare("FmTest") == 0)
	{
		m_strErrorCode = FunErr_FM_Test_Fail;
		passFail = runFmTest();
	}
	else if (cs_AudtioTestItem.Compare("HeadsetDetectTest") == 0)
	{
		m_strErrorCode = FunErr_Detect_Headset_Fail;
		passFail = runHeadsetDetectTest();
	}
	return passFail;
}

bool CMMIBB_AudioRetryTest::runHeadsetDetectTest()
{
	m_i_retriedTime = 0;
	CString cs_TestTime;
	char sz_JackType[FTD_BUF_SIZE] = {0};
	do
	{
		b_res_on = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		cs_TestTime.Format(_T("runHeadsetDetectTest - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);

		memset(sz_JackType, 0, FTD_BUF_SIZE);
		b_res_on = m_pIPhone->FTD_AUdetectHeadset(m_nFtdPort, m_nFtdTimeOut, sz_JackType);
		if (b_res_on) {
			str_msg = "FTD_AUdetectHeadset CMD Pass";
		}
		else
		{
			str_msg = "FTD_AUdetectHeadset CMD Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		m_strMeasured = sz_JackType;
		if (CStr::StrToInt(m_strMeasured) ==  CStr::StrToInt(m_str_AudioCMD))
		{
			str_msg = "Jack type pass! type = " + m_strMeasured;
			b_res_msg_try = true;
		}
		else
		{
			str_msg = "Jack type is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
			b_res_msg_try = false;
		}
		TraceLog(MSG_INFO, str_msg);

		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check Headset cable\n 请确认耳机有插到位?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "HeadsetDetectTest fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if (!b_res_msg_try)  /*fail*/
	{
		SetPICSData("JACK_DETECTION_TEST", "FAIL");
		return false;
	}
	SetPICSData("JACK_DETECTION_TEST", "PASS");
	return true;
}

bool CMMIBB_AudioRetryTest::runFmTest()
{
	m_i_retriedTime = 0;
	CString cs_TestTime;
	char sz_patternPlayFM[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	do
	{
		b_res_on = false;
		b_res_off = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_patternPlayFM, 0, FTD_BUF_SIZE);
		strcpy(sz_patternPlayFM, m_str_AudioCMD.c_str());
		b_res_on = m_pIPhone->FTD_FMRModuleOn(m_nFtdPort, m_nFtdTimeOut, sz_patternPlayFM, sz_outBuffer);//turn on
		Sleep(m_i_OnSleepTime);

		cs_TestTime.Format(_T("runFmTest - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);

		if (b_res_on) {
		str_msg = "FTD_FMRModuleOn CMD Pass";
		}
		else
		{
			str_msg = "FTD_FMRModuleOn CMD Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try = showMsg(CW2A(L"[FM] Is Speaker on ? \n 手机的FM有播音乐吗? "));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "FMRModuleOn pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check Headset cable\n 请确认耳机有插到位?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			m_strErrorCode = FunErr_FM_Test_Fail;
			str_msg = "FMRModuleOn fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		strcpy(sz_patternPlayFM, m_str_AudioOffCMD.c_str());
		b_res_off = m_pIPhone->FTD_FMRModuleOn(m_nFtdPort, m_nFtdTimeOut, sz_patternPlayFM, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);

		if (b_res_off){str_msg = "FMRModuleOff CMD pass!";}
		else
		{
			str_msg = "FMRModuleOff CMD fail!";
			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData("FMRModuleOn", "FAIL");
		return false;
	}
	SetPICSData("FMRModuleOn", "PASS");
	return true;
}

bool CMMIBB_AudioRetryTest::runSpeakerOnTest()
{
	m_i_retriedTime = 0;
	runPushAudioFile();

	char sz_patternPlay[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	CString cs_TestTime;
	do
	{
		b_res_on = false;
		b_res_off = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_patternPlay, 0, FTD_BUF_SIZE);
		strcpy(sz_patternPlay, m_str_AudioCMD.c_str());
		b_res_on = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);
		Sleep(m_i_OnSleepTime);

		cs_TestTime.Format(_T("runSpeakerOnTest -TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);

		if (b_res_on) {str_msg = "SpeakerOn CMD Pass";}
		else
		{
			str_msg = "SpeakerOn CMD Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try = showMsg(CW2A(L"Is Speaker on ? \n 手机的免提是开启的吗 ?"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "SpeakerOn pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "SpeakerOn fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		strcpy(sz_patternPlay, m_str_AudioOffCMD.c_str());
		b_res_off = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);

		if (b_res_off){str_msg = "SPEAKER_PLAY Off  CMD pass!";}
		else
		{
			str_msg = "SPEAKER_PLAY Off CMD fail!";
			m_strErrorCode = FunErr_Speaker_Play_Stop_Fail;
			break;
		}
		TraceLog(MSG_INFO, str_msg);

	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData("SPEAKER_PLAY", "FAIL");
		return false;
	}
	SetPICSData("SPEAKER_PLAY", "PASS");
	return true;
}

bool CMMIBB_AudioRetryTest::runHeadsetOnTest()
{
	m_i_retriedTime = 0;
	runPushAudioFile();

	CString cs_TestTime;
	char sz_patternPlay[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	do
	{
		b_res_on = false;
		b_res_off = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_patternPlay, 0, FTD_BUF_SIZE);
		strcpy(sz_patternPlay, m_str_AudioCMD.c_str());
		b_res_on = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);
		Sleep(m_i_OnSleepTime);

		cs_TestTime.Format(_T("runHeadsetOnTest - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);

		if (b_res_on) { str_msg =  "turn headsetOn CMD PASS"; }
		else
		{
			str_msg = "Fail to turn headsetOn CMD";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try = showMsg(CW2A(L"Is headset on　? \n 手机的耳机是开启的吗？"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "headsetOn pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "headsetOn fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		strcpy(sz_patternPlay, m_str_AudioOffCMD.c_str());
		b_res_off = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);

		if (b_res_off){str_msg = "headsetOn Off CMD pass!";}
		else
		{
			str_msg = "headsetOn Off CMD fail!";
			m_strErrorCode = FunErr_Headset_Speaker_Test_Fail;
			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData("HEADSET_PLAY", "FAIL");
		return false;
	}
	SetPICSData("HEADSET_PLAY", "PASS");
	return true;
}

bool CMMIBB_AudioRetryTest::runReceiverOnTest()
{
	m_i_retriedTime = 0;
	runPushAudioFile();

	CString cs_TestTime;
	char sz_patternPlay[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	do
	{
		b_res_on = false;
		b_res_off = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_patternPlay, 0, FTD_BUF_SIZE);
		strcpy(sz_patternPlay, m_str_AudioCMD.c_str());
		b_res_on = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);
		Sleep(m_i_OnSleepTime);

		cs_TestTime.Format(_T("runReceiverOnTest - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);

		if (b_res_on){str_msg =  "turn ReceiverOn CMD PASS";}
		else
		{
			str_msg = "Fail to turn ReceiverOn CMD";
			break;
		}
		TraceLog(MSG_INFO, str_msg);


		b_res_msg_try =	showMsg(CW2A(L"Is receiver on　?　\n 手机的听筒是开启的吗?"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "ReceiverOn pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "ReceiverOn fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		strcpy(sz_patternPlay, m_str_AudioOffCMD.c_str());
		b_res_off = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);

		if (b_res_off){str_msg = "ReceiverOn Off CMD pass!";}
		else
		{
			str_msg = "ReceiverOn Off CMD fail!";
			m_strErrorCode = FunErr_Receiver_Play_Stop_Fail;
			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData("RECEIVER_PLAY", "FAIL");
		return false;
	}
	SetPICSData("RECEIVER_PLAY", "PASS");
	return true;
}

bool CMMIBB_AudioRetryTest::runPushAudioFile()
{
	char szOutPullCommand[50000] = {0};
	char szErrorCodePullCommand[50000] = {0};
	char csCommandShell[50000] = {0};
	strcpy(csCommandShell, "push ");
	strcat(csCommandShell, m_str_Path.c_str());
	if (!m_pIPhone->ExecAdbCommand(csCommandShell, szOutPullCommand, szErrorCodePullCommand))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Audio_Push_Wav_Fail;
		m_strMessage = "Push music fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	else
	{
		m_strMessage = "Push music as record wav pass.";
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
	}
	return true;
}
bool CMMIBB_AudioRetryTest::showMsg(char* szMsg)
{
	/* Init dialog response flag to false */
	m_b_DialogResponse = false;
	m_b_waitFlag = false;

	/* Notify UI to show message dialog */
	ShowDialog(szMsg);

	/* Wait UI response(Max:10s) */
	int i_waitCount = 0;
	while (m_b_waitFlag != true)
	{
		if (i_waitCount == 50)
		{
			str_msg = "Wait UI response time out.";
			TraceLog(MSG_INFO, "Wait UI response time out.");
			break;
		}
		else
		{
			i_waitCount++;
			Sleep(200);
			continue;
		}
	}

	/* Get UI dialog operator response */
	if (m_b_DialogResponse == true)
	{
		str_msg = "Pressed PASS";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		str_msg = "Pressed FAIL";
		TraceLog(MSG_INFO, str_msg);
	}
	return m_b_DialogResponse;
}
