/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CMMIBB_AudioTest object.
 * FileName   : PlayWavTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "MMIBB_AudioTest.h"


RF_IMPLEMENT_DYNCREATE(CMMIBB_AudioTest)


bool CMMIBB_AudioTest::PreRun()
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

bool CMMIBB_AudioTest::PostRun()
{
	return true;
}

bool CMMIBB_AudioTest::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find(std::string("AudtioTestItem")) != paramMap.end())
	{
		m_str_AudtioTestItem = paramMap[std::string("AudtioTestItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter AudtioTestItem for CMMIBB_AudioTest");
		return false;
	}

	if (paramMap.find(std::string("AudtioCMDItem")) != paramMap.end())
	{
		m_str_AudtioCMD = paramMap[std::string("AudtioCMDItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter AudtioCMDItem for CMMIBB_AudioTest");
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
		TraceLog(MSG_INFO, "Fail to find parameter SleepTime for CMMIBB_AudioTest");
		return false;
	}

	if (paramMap.find(std::string("Path")) != paramMap.end())
	{
		m_str_Path = paramMap[std::string("Path")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter wav path for CMMIBB_AudioTest");
		return false;
	}

	return true;
}

bool CMMIBB_AudioTest::Run()
{
	bool passFail = false;
	CString cs_AudtioTestItem = m_str_AudtioTestItem.c_str();

	if (cs_AudtioTestItem.Compare("SpeakerOnTest") == 0)
	{
		m_strErrorCode = FunErr_Speaker_Play_Fail;
		passFail = runSpeakerOnTest();
	}
	else if (cs_AudtioTestItem.Compare("SpeakerOffTest") == 0)
	{
		m_strErrorCode = FunErr_Speaker_Play_Stop_Fail;
		passFail = runAudioOffTest("SpeakerOffTest");
	}
	else if (cs_AudtioTestItem.Compare("HeadsetOnTest") == 0)
	{
		m_strErrorCode = FunErr_Headset_Speaker_Test_Fail;
		passFail = runHeadsetOnTest();
	}
	else if (cs_AudtioTestItem.Compare("HeadsetOffTest") == 0)
	{
		m_strErrorCode = FunErr_Headset_Speaker_Play_Stop_Fail;
		passFail = runAudioOffTest("HeadsetOffTest");
	}
	else if (cs_AudtioTestItem.Compare("ReceiverOnTest") == 0)
	{
		m_strErrorCode = FunErr_Receiver_Play_Fail;
		passFail = runReceiverOnTest();
	}
	else if (cs_AudtioTestItem.Compare("ReceiverOffTest") == 0)
	{
		m_strErrorCode = FunErr_Receiver_Play_Stop_Fail;
		passFail = runAudioOffTest("ReceiverOffTest");
	}
	else if (cs_AudtioTestItem.Compare("FmTest") == 0)
	{
		m_strErrorCode = FunErr_FM_Test_Fail;
		passFail = runFmTest();
	}
	else if (cs_AudtioTestItem.Compare("CloseFmTest") == 0)
	{
		m_strErrorCode = FunErr_FM_Test_Fail;
		passFail = runOffFmTest();
	}
	return passFail;
}

bool CMMIBB_AudioTest::runFmTest()
{
	bool b_res = false;
	char sz_patternPlayFM[FTD_BUF_SIZE] = {0};
	strcpy(sz_patternPlayFM, m_str_AudtioCMD.c_str());
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	b_res = m_pIPhone->FTD_FMRModuleOn(m_nFtdPort, m_nFtdTimeOut, sz_patternPlayFM, sz_outBuffer);//turn on
	if (!b_res)
	{
		str_msg = "FTD_FMRModuleOn Fail";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		str_msg = "FTD_FMRModuleOn success";
		TraceLog(MSG_INFO, str_msg);
	}
	Sleep(m_i_SleepTime);

	if (b_res)
	{
		b_res = showMsg(CW2A(L"[FM] Is Speaker on ? \n 手机的FM有播音乐吗? ?"));
		if (b_res)
		{
			m_strErrorCode = "-";
			SetPICSData("FMRModuleOn", "PASS");
			str_msg = "FMRModuleOn pass!";
		}
		else
		{
			SetPICSData("FMRModuleOn", "FAIL");
			str_msg = "FMRModuleOn fail!";
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
	}
	FactoryLog();
	return b_res;
}

bool CMMIBB_AudioTest::runOffFmTest()
{
	bool b_res = false;
	char sz_patternPlayFM[FTD_BUF_SIZE] = {0};
	strcpy(sz_patternPlayFM, m_str_AudtioCMD.c_str());
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	b_res = m_pIPhone->FTD_FMRModuleOn(m_nFtdPort, m_nFtdTimeOut, sz_patternPlayFM, sz_outBuffer);//turn on
	Sleep(m_i_SleepTime);
	if (b_res)
	{
		m_strErrorCode = "-";
		str_msg = "FMRModuleOff pass!";

	}
	else
	{
		str_msg = "FMRModuleOff fail!";
	}
	m_strMessage = str_msg;
	TraceLog(MSG_INFO, str_msg);
	FactoryLog();
	return b_res;
}

bool CMMIBB_AudioTest::runSpeakerOnTest()
{
	bool b_res = false;
	runPushAudioFile();
	char sz_patternPlayWav[FTD_BUF_SIZE] = {0};
	strcpy(sz_patternPlayWav, m_str_AudtioCMD.c_str());
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	b_res = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_patternPlayWav, sz_outBuffer);
	if (!b_res)
	{
		str_msg = "Fail to turn SpeakerOn";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		Sleep(m_i_SleepTime);
		str_msg = "SpeakerOn success";
		TraceLog(MSG_INFO, str_msg);
	}

	if (b_res)
	{
		b_res = showMsg(CW2A(L"Is Speaker on ? \n 手机的免提是开启的吗 ?"));
		if (b_res)
		{
			SetPICSData("SPEAKER_PLAY", "PASS");
			str_msg = "SpeakerOn pass!";
		}
		else
		{
			SetPICSData("SPEAKER_PLAY", "FAIL");
			str_msg = "SpeakerOn fail!";
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
	}
	FactoryLog();
	return b_res;
}

bool CMMIBB_AudioTest::runHeadsetOnTest()
{
	bool b_res = false;
	runPushAudioFile();
	char sz_patternPlayWav[FTD_BUF_SIZE] = {0};
	strcpy(sz_patternPlayWav, m_str_AudtioCMD.c_str());
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	b_res = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_patternPlayWav, sz_outBuffer);
	if (!b_res)
	{
		str_msg = "Fail to turn headsetOn";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		Sleep(m_i_SleepTime);
		str_msg = "turn headsetOn success";
		TraceLog(MSG_INFO, str_msg);
	}

	if (b_res)
	{
		b_res = showMsg(CW2A(L"Is headset on　? \n 手机的耳机是开启的吗？"));
		if (b_res)
		{
			SetPICSData("HEADSET_PLAY", "PASS");
			str_msg = "ReceiverOn pass!";
		}
		else
		{
			SetPICSData("HEADSET_PLAY", "FAIL");
			str_msg = "ReceiverOn fail!";
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
	}
	FactoryLog();
	return b_res;
}

bool CMMIBB_AudioTest::runReceiverOnTest()
{
	bool b_res = false;
	runPushAudioFile();
	char sz_patternPlayWav[FTD_BUF_SIZE] = {0};
	strcpy(sz_patternPlayWav, m_str_AudtioCMD.c_str());
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	b_res = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_patternPlayWav, sz_outBuffer);
	if (!b_res)
	{
		str_msg = "Fail to turn ReceiverOn";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		Sleep(m_i_SleepTime);
		str_msg = "turn ReceiverOn success";
		TraceLog(MSG_INFO, str_msg);
	}
	if (b_res)
	{
		b_res =	showMsg(CW2A(L"Is receiver on　?　\n 手机的听筒是开启的吗?"));
		if (b_res)
		{
			SetPICSData("RECEIVER_PLAY", "PASS");
			str_msg = "ReceiverOn pass!";
		}
		else
		{
			SetPICSData("RECEIVER_PLAY", "FAIL");
			str_msg = "ReceiverOn fail!";
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
	}
	FactoryLog();
	return b_res;
}

bool CMMIBB_AudioTest::runAudioOffTest(char* szMsg)
{
	bool b_res = false;
	char sz_patternPlayWav[FTD_BUF_SIZE];
	strcpy(sz_patternPlayWav, m_str_AudtioCMD.c_str());
	char sz_outBuffer[FTD_BUF_SIZE];
	b_res = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_patternPlayWav, sz_outBuffer);
	str_msg = szMsg ;
	if (!b_res)
	{
		m_strResult = "FAIL";
		str_msg = str_msg + " FAIL ";
	}
	else
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		str_msg = str_msg + " PASS";
	}
	m_strMessage = str_msg;
	TraceLog(MSG_INFO, str_msg);
	FactoryLog();
	return b_res;
}

bool CMMIBB_AudioTest::runPushAudioFile()
{
	char csCommandShell[50000] = {0};
	strcpy(csCommandShell, "push ");
	strcat(csCommandShell, m_str_Path.c_str());
	char cmdOut[50000] = {0};
	char errMsg[50000] = {0};
	if (!m_pIPhone->ExecAdbCommand(csCommandShell, cmdOut, errMsg))
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

bool CMMIBB_AudioTest::showMsg(char* szMsg)
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
	if (m_b_DialogResponse)
	{
		TraceLog(MSG_INFO, "PASS");
	}
	else
	{
		TraceLog(MSG_INFO, "FAIL");
	}
	return m_b_DialogResponse;
}

bool CMMIBB_AudioTest::PlayWavTest()
{
	return false;
}
