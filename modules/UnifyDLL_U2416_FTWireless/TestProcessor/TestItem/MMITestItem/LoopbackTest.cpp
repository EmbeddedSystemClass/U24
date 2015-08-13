/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CLoopback object.
 * FileName   : LoopbackTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "LoopbackTest.h"


RF_IMPLEMENT_DYNCREATE(CLoopback)


bool CLoopback::PreRun()
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

bool CLoopback::Run()
{
	return LoopbackTest();
}

bool CLoopback::PostRun()
{
	return true;
}

bool CLoopback::InitData(std::map<std::string, std::string>& paramMap)
{
	/* PatternIndex */
	if (paramMap.find(std::string("OnOff")) != paramMap.end())
	{
		m_str_OnOff = paramMap[std::string("OnOff")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter OnOff for CLoopback");
		return false;
	}

	if (paramMap.find(std::string("DeviceFrom")) != paramMap.end())
	{
		m_str_DeviceFrom = paramMap[std::string("DeviceFrom")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter DeviceFrom for CLoopback");
		return false;
	}

	if (paramMap.find(std::string("VolumeValFrom")) != paramMap.end())
	{
		m_str_VolumeValFrom = paramMap[std::string("VolumeValFrom")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter VolumeValFrom for CLoopback");
		return false;
	}

	if (paramMap.find(std::string("DeviceTo")) != paramMap.end())
	{
		m_str_DeviceTo = paramMap[std::string("DeviceTo")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter DeviceTo for CLoopback");
		return false;
	}

	if (paramMap.find(std::string("VolumeValTo")) != paramMap.end())
	{
		m_str_VolumeValTo = paramMap[std::string("VolumeValTo")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter VolumeValTo for CLoopback");
		return false;
	}

	if (paramMap.find(std::string("ErrorCode")) != paramMap.end())
	{
		m_str_ErrorCode = paramMap[std::string("ErrorCode")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter ErrorCode for CLoopback");
		return false;
	}

	n_CLoopback_Sleep = 0 ;
	if (paramMap.find(std::string("CLoopback_Sleep")) != paramMap.end())
	{
		m_str_CLoopback_Sleep = paramMap[std::string("CLoopback_Sleep")];
		n_CLoopback_Sleep = atoi (m_str_CLoopback_Sleep.c_str());
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter ErrorCode for CLoopback");
	}

	return true;
}

bool CLoopback::LoopbackTest()
{
	bool b_res = true;
	std::string str_msg;
	std::string str_Pics;
	if (b_res)
	{
		char sz_patternLoopback[FTD_BUF_SIZE] = {0};
		strcpy(sz_patternLoopback, m_str_OnOff.c_str());
		strcat(sz_patternLoopback, ",");
		strcat(sz_patternLoopback, m_str_DeviceFrom.c_str());
		strcat(sz_patternLoopback, ",");
		strcat(sz_patternLoopback, m_str_VolumeValFrom.c_str());
		strcat(sz_patternLoopback, ",");
		strcat(sz_patternLoopback, m_str_DeviceTo.c_str());
		strcat(sz_patternLoopback, ",");
		strcat(sz_patternLoopback, m_str_VolumeValTo.c_str());
		char sz_outBuffer[FTD_BUF_SIZE] = {0};
		b_res = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, sz_patternLoopback, sz_outBuffer);
		if (!b_res)
		{
			str_msg = "FTD_AUloopbackTest command fail";
			TraceLog(MSG_INFO,str_msg);
		}
		else
		{
			str_msg = "FTD_AUloopbackTest command success";
			TraceLog(MSG_INFO,str_msg);
			Sleep( n_CLoopback_Sleep );
		}
	}

	/* Query operator if display pattern is required pattern */
	if (b_res)
	{
		if (m_str_OnOff.find("on") != string::npos)
		{
			if (m_str_DeviceFrom.find("headmic") != string::npos && m_str_DeviceTo.find("headset") != string::npos)
			{
				m_strErrorCode = FunErr_Loopback_Headmic_To_Headset_Fail;
				str_msg = CW2A(L"Please blow on headmic. Whether there is sound on headset?\n请对耳机麦克风吹气,确认耳机听筒是否有声音?\n");
				str_Pics = "HEADMIC_TO_HEADSET_ON_TEST";
			}
			else if(m_str_DeviceFrom.find("front") != string::npos && m_str_DeviceTo.find("speaker") != string::npos)
			{
				m_strErrorCode = FunErr_Loopback_Mic_To_Speaker_Fail;
				str_msg = CW2A(L"Please blow on mic. Confirmed that there is  no noise?\n请对下麦克风吹气,确认手机无哮叫声?\n");
				str_Pics = "MIC_TO_SPEAKER_ON_TEST";
			}
			else if(m_str_DeviceFrom.find("front") != string::npos && m_str_DeviceTo.find("headset") != string::npos)
			{
				m_strErrorCode = FunErr_Loopback_Mic_To_Headset_Fail;
				str_msg = CW2A(L"Please blow on mic. Whether there is sound on headset?\n请对下麦克风吹气,确认耳机是否有声音?\n");
				str_Pics = "MIC_TO_HEADSET_ON_TEST";
			}
			else if(m_str_DeviceFrom.find("back") != string::npos && m_str_DeviceTo.find("receiver") != string::npos)
			{
				m_strErrorCode = FunErr_Loopback_Mic2_To_Receiver_Fail;
				str_msg = CW2A(L"Please blow on back mic. Confirmed that there is  no noise?\n请对後麦克风吹气,确认手机无哮叫声?\n");
				str_Pics = "MIC2_TO_RECEIVER_ON_TEST";
			}
			else if(m_str_DeviceFrom.find("back") != string::npos && m_str_DeviceTo.find("headset") != string::npos)
			{
				m_strErrorCode = FunErr_Loopback_Mic2_To_Headset_Fail;
				str_msg = CW2A(L"Please blow on back mic. Whether there is sound on headset?\n请对後麦克风吹气,确认耳机是否有声音?\n");
				str_Pics = "MIC2_TO_HEADSET_ON_TEST";
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
					str_msg = "Wait UI response time out.";
					TraceLog(MSG_INFO, "Wait UI response time out.");
					b_res = false;
					break;
				}
				i_waitCount++;
				Sleep(200);
			}
			if (b_res)
			{
				/* Get UI dialog operator response */
				if (m_b_DialogResponse)
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
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		str_msg = "Loopback " + m_str_OnOff + " Test fail!";
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "FAIL");
	}
	FactoryLog();
	return b_res;
}
