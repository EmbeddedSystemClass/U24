/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CMMIBB_AudioLoopRetryTest object.
 * FileName   : PlayWavTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "MMIBB_AudioLoopRetryTest.h"


RF_IMPLEMENT_DYNCREATE(CMMIBB_AudioLoopRetryTest)


bool CMMIBB_AudioLoopRetryTest::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(Loopback_Test_BaseItemCode);
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

bool CMMIBB_AudioLoopRetryTest::PostRun()
{
	return true;
}

bool CMMIBB_AudioLoopRetryTest::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("AudtioTestItem")) != paramMap.end())
	{
		m_str_AudtioTestItem = paramMap[std::string("AudtioTestItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter AudtioTestItem for CMMIBB_AudioLoopRetryTest");
		return false;
	}

	if (paramMap.find(std::string("AudtioCMDItem")) != paramMap.end())
	{
		m_str_AudtioCMD = paramMap[std::string("AudtioCMDItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter AudtioCMDItem for CMMIBB_AudioLoopRetryTest");
		return false;
	}

	if (paramMap.find(std::string("AudtioOffCMDItem")) != paramMap.end())
	{
		m_str_AudtioOffCMD = paramMap[std::string("AudtioOffCMDItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter AudtioOffCMDItem for CMMIBB_AudioLoopRetryTest");
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
		TraceLog(MSG_INFO, "Fail to find parameter OnSleepTime for CMMIBB_AudioLoopRetryTest");
		return false;
	}

	if (paramMap.find(std::string("OffSleepTime")) != paramMap.end())
	{
		str_SleepTime = paramMap[std::string("OffSleepTime")];
		m_i_OffSleepTime = CStr::StrToInt(str_SleepTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter OffSleepTime for CMMIBB_AudioLoopRetryTest");
		return false;
	}

	if (paramMap.find(std::string("RetryTime")) != paramMap.end())
	{
		str_SleepTime = paramMap[std::string("RetryTime")];
		m_i_MexRetryTime = CStr::StrToInt(str_SleepTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter RetryTime for CMMIBB_AudioLoopRetryTest");
		return false;
	}


	if (paramMap.find(std::string("Path")) != paramMap.end())
	{
		m_str_Path = paramMap[std::string("Path")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter wav path for CMMIBB_AudioLoopRetryTest");
		return false;
	}

	return true;
}

bool CMMIBB_AudioLoopRetryTest::Run()
{
	bool passFail = false;
	CString cs_AudtioTestItem = m_str_AudtioTestItem.c_str();
	if (cs_AudtioTestItem.Compare("Loop_FrontMicToSpeakOn") == 0)
	{
		m_strErrorCode = FunErr_Loopback_Mic_To_Speaker_Fail;
		passFail = runLoop_FrontMicToSpeakOn();
	}
	else if (cs_AudtioTestItem.Compare("Loop_BackMicToReceiverOn") == 0)
	{
		m_strErrorCode = FunErr_Loopback_Mic2_To_Receiver_Fail;
		passFail = runLoop_BackMicToReceiverOn();
	}
	else if (cs_AudtioTestItem.Compare("Loop_HeadmicToHeadsetOn") == 0)
	{
		m_strErrorCode = FunErr_Loopback_Headmic_To_Headset_Fail;
		passFail = runLoop_HeadmicToHeadsetOn();
	}
	else if (cs_AudtioTestItem.Compare("Loop_FrontMicToHeadsetOn") == 0)
	{
		m_strErrorCode = FunErr_Loopback_Mic_To_Headset_Fail;
		passFail = runLoop_FrontMicToHeadsetOn();
	}
	else if (cs_AudtioTestItem.Compare("Loop_BackMicToHeadsetOn") == 0)
	{
		m_strErrorCode = FunErr_Loopback_Mic2_To_Headset_Fail;
		passFail = runLoop_BackMicToHeadsetOn();
	}
	else if (cs_AudtioTestItem.Compare("MicToSpeakTest") == 0)
	{
		m_strErrorCode = FunErr_Loopback_Mic_To_Speaker_Fail;
		passFail = runLoop_MicToSpeak();
	}
	return passFail;
}

bool CMMIBB_AudioLoopRetryTest::runLoop_FrontMicToSpeakOn()
{
	m_i_retriedTime = 0;
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
		strcpy(sz_patternPlay, m_str_AudtioCMD.c_str());
		b_res_on = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OnSleepTime);
		cs_TestTime.Format(_T("Loop_FrontMicToSpeakOn - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);
		if (b_res_on)
		{
			str_msg = "Loop_FrontMicToSpeakOn CMD success";
		}
		else
		{
			str_msg = "Loop_FrontMicToSpeakOn CMD Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try = showMsg(CW2A(L"Please blow on mic. Confirmed that there is  no noise?\n请对下麦克风吹气,确认手机无哮叫声?\n"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "Loop_FrontMicToSpeakOn pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "Loop_FrontMicToSpeakOn fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_patternPlay, 0, FTD_BUF_SIZE);
		strcpy(sz_patternPlay, m_str_AudtioOffCMD.c_str());
		b_res_off = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);
		if (b_res_off)
		{
			str_msg = "Loop_FrontMicToSpeakOff CMD pass!";
		}
		else
		{
			str_msg = "Loop_FrontMicToSpeakOff CMD fail!";
			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);

	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData("MIC_TO_SPEAKER_ON_TEST", "FAIL");
		return b_res_msg_try;
	}
	SetPICSData("MIC_TO_SPEAKER_ON_TEST", "PASS");
	return true;
}

bool CMMIBB_AudioLoopRetryTest::runLoop_BackMicToReceiverOn()
{
	m_i_retriedTime = 0;
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
		strcpy(sz_patternPlay, m_str_AudtioCMD.c_str());
		b_res_on = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OnSleepTime);
		cs_TestTime.Format(_T("Loop_BackMicToReceiverOn - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);
		if (b_res_on)
		{
			str_msg = "Loop_BackMicToReceiverOn CMD success";
		}
		else
		{
			str_msg = "Loop_BackMicToReceiverOn CMD Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);


		b_res_msg_try =  showMsg(CW2A(L"Please blow on back mic. Confirmed that there is  no noise?\n请对後麦克风吹气,确认手机无哮叫声?\n"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "Loop_BackMicToReceiverOn pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "Loop_BackMicToReceiverOn fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_patternPlay, 0, FTD_BUF_SIZE);
		strcpy(sz_patternPlay, m_str_AudtioOffCMD.c_str());
		b_res_off = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);
		if (b_res_off)
		{
			str_msg = "Loop_BackMicToReceiverOff CMD pass!";
		}
		else
		{
			str_msg = "Loop_BackMicToReceiverOff CMD fail!";
			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);

	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData("MIC2_TO_RECEIVER_ON_TEST", "FAIL");
		return b_res_msg_try;
	}
	SetPICSData("MIC2_TO_RECEIVER_ON_TEST", "PASS");
	return true;
}

bool CMMIBB_AudioLoopRetryTest:: runLoop_HeadmicToHeadsetOn()
{
	m_i_retriedTime = 0;
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
		strcpy(sz_patternPlay, m_str_AudtioCMD.c_str());
		b_res_on = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OnSleepTime);
		cs_TestTime.Format(_T("Loop_HeadmicToHeadsetOn - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);
		if (b_res_on)
		{
			str_msg = "Loop_HeadmicToHeadsetOn CMD Pass";
		}
		else
		{
			str_msg = "Loop_HeadmicToHeadsetOn CMD Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try = showMsg(CW2A(L"Please Speak to headmic. Whether there is sound on headset?\n请对耳机麦克风说话,确认耳机听筒是否有声音?\n"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "Loop_HeadmicToHeadsetOn pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "Loop_HeadmicToHeadsetOn fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_patternPlay, 0, FTD_BUF_SIZE);
		strcpy(sz_patternPlay, m_str_AudtioOffCMD.c_str());
		b_res_off = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);
		if (b_res_off)
		{
			str_msg = "Loop_HeadmicToHeadsetOff CMD pass!";
		}
		else
		{
			str_msg = "Loop_HeadmicToHeadsetOff CMD fail!";
			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);

	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData("HEADMIC_TO_HEADSET_ON_TEST", "FAIL");
		return false;
	}
	SetPICSData("HEADMIC_TO_HEADSET_ON_TEST", "PASS");
	return true;
}

bool  CMMIBB_AudioLoopRetryTest::runLoop_FrontMicToHeadsetOn()
{
	m_i_retriedTime = 0;
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
		strcpy(sz_patternPlay, m_str_AudtioCMD.c_str());
		b_res_on = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OnSleepTime);
		cs_TestTime.Format(_T("Loop_FrontMicToHeadsetOn - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);

		if (b_res_on)
		{
			str_msg = "Loop_FrontMicToHeadsetOn CMD success";
		}
		else
		{
			str_msg = "Loop_FrontMicToHeadsetOn CMD Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try =  showMsg(CW2A(L"Please Speak to mic. Whether there is sound on headset?\n请对下麦克风说话,确认耳机是否有声音?\n"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "Loop_FrontMicToHeadsetOn pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "Loop_FrontMicToHeadsetOn fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_patternPlay, 0, FTD_BUF_SIZE);
		strcpy(sz_patternPlay, m_str_AudtioOffCMD.c_str());
		b_res_off = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);
		if (b_res_off)
		{
			str_msg = "Loop_FrontMicToHeadsetOff CMD pass!";
		}
		else
		{
			str_msg = "Loop_FrontMicToHeadsetOff CMD fail!";
			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);

	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData("MIC_TO_HEADSET_ON_TEST", "FAIL");
		return false;
	}
	SetPICSData("MIC_TO_HEADSET_ON_TEST", "PASS");
	return true;
}

bool  CMMIBB_AudioLoopRetryTest::runLoop_BackMicToHeadsetOn()
{
	m_i_retriedTime = 0;
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
		strcpy(sz_patternPlay, m_str_AudtioCMD.c_str());
		b_res_on = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OnSleepTime);
		cs_TestTime.Format(_T("Loop_BackMicToHeadsetOn - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);
		if (b_res_on)
		{
			str_msg = "Loop_BackMicToHeadsetOn CMD success";
		}
		else
		{
			str_msg = "Loop_BackMicToHeadsetOn CMD Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try =  showMsg(CW2A(L"Please Speak to back mic. Whether there is sound on headset?\n请对後麦克风说话,确认耳机是否有声音?\n"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "Loop_BackMicToHeadsetOn pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "Loop_BackMicToHeadsetOn fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_patternPlay, 0, FTD_BUF_SIZE);
		strcpy(sz_patternPlay, m_str_AudtioOffCMD.c_str());
		b_res_off = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);
		if (b_res_off)
		{
			str_msg = "Loop_BackMicToHeadsetOff CMD pass!";
		}
		else
		{
			str_msg = "Loop_BackMicToHeadsetOff CMD fail!";
			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);

	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData("MIC2_TO_HEADSET_ON_TEST", "FAIL");
		return b_res_msg_try;
	}
	SetPICSData("MIC2_TO_HEADSET_ON_TEST", "PASS");
	return true;
}

bool  CMMIBB_AudioLoopRetryTest::runLoop_MicToSpeak()
{
	m_i_retriedTime = 0;
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
		strcpy(sz_patternPlay, m_str_AudtioCMD.c_str());
		b_res_on = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OnSleepTime);
		cs_TestTime.Format(_T("Loop_HeadMicToSpeak - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);
		if (b_res_on)
		{
			str_msg = "Loop_MicToSpeak CMD success";
		}
		else
		{
			str_msg = "Loop_MicToSpeak CMD Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try =  showMsg(CW2A(L" Whether there is sound on Speaker?\n麦克风是否有声音?\n"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "Loop_MicToSpeak pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "Loop_MicToSpeak fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_patternPlay, 0, FTD_BUF_SIZE);
		strcpy(sz_patternPlay, m_str_AudtioOffCMD.c_str());
		b_res_off = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternPlay, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);
		if (b_res_off)
		{
			str_msg = "Loop_MicToSpeakOff CMD pass!";
		}
		else
		{
			str_msg = "Loop_MicToSpeakOff CMD fail!";
			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);

	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData("MIC_TO_SPEAKER_ON_TEST", "FAIL");
		return b_res_msg_try;
	}
	SetPICSData("MIC_TO_SPEAKER_ON_TEST", "PASS");
	return true;
}

bool CMMIBB_AudioLoopRetryTest::showMsg(char* szMsg)
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
