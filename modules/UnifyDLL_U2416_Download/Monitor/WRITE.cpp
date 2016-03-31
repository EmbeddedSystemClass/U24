/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 * Author     : Lion.Wu
 ********************************************************************************/

#include "stdafx.h"
#include "WRITE.h"
#include <string> 

template <class T> 
std::string ConvertToString(T);

RF_IMPLEMENT_DYNCREATE(CWRITE)

template <class T> 
std::string ConvertToString(T value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

bool CWRITE::PreRun()
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

bool CWRITE::Run()
{
	bool passFail = false;

	if (m_str_TestItem == FineWifiAP)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_WLAN_Connect_Fail;
		str_Pics = CW2A(L"Fine_Wifi_AP");
		passFail = runFindWifiAP();
	}
	else if (m_str_TestItem == DELL_SER_TAG)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_USB_Charging_Current_Test_Fail;
		str_Pics = CW2A(L"DELL_SER_TAG");
		passFail = run_DELL_SER_TAG();
	}


	return passFail;

}

bool CWRITE::PostRun()
{
	return true;
}

bool CWRITE::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("XMLTestItem")) != paramMap.end())
	{
		m_str_TestItem = paramMap[std::string("XMLTestItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'TestItem' for class 'CWRITE'");
		return false;
	}

	std::string str_SleepTime;
	if (paramMap.find(std::string("XMLSleepTime")) != paramMap.end())
	{
		str_SleepTime = paramMap[std::string("XMLSleepTime")];
		m_i_SleepTime = CStr::StrToInt(str_SleepTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter OnSleepTime for CWRITE");
		return false;
	}

	if (paramMap.find(std::string("XMLCMDItem")) != paramMap.end())
	{
		m_str_CMD = paramMap[std::string("XMLCMDItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter XMLCMDItem for CWRITE");
		return false;
	}

	if (paramMap.find(std::string("XMLOffCMDItem")) != paramMap.end())
	{
		m_str_OffCMD = paramMap[std::string("XMLOffCMDItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter XMLOffCMDItem for CWRITE");
		return false;
	}

	std::string str_MexRetryTime;
	if (paramMap.find(std::string("XMLRetryTime")) != paramMap.end())
	{
		str_MexRetryTime = paramMap[std::string("XMLRetryTime")];
		m_i_MexRetryTime = CStr::StrToInt(str_MexRetryTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter RetryTime for CWRITE");
		return false;
	}

	return true;
}



bool CWRITE::run_DELL_SER_TAG()
{
	m_i_retriedTime = 0;
	std::string msg;
	CString cs_TestTime;
	
	std::string str_CMD_Msg_Pass, str_CMD_Msg_Fail, str_Result_Msg_Pass, str_Result_Msg_Fail;
	m_strItemCode = CStr::IntToStr(Flash_LED_BaseItemCode);

	msg = CW2A(L"Is the LED lighting ?\n LED有亮吗?\n");

	char szInput[FTD_BUF_SIZE] = {0};
	char szOutnput[FTD_BUF_SIZE] = {0};
	do
	{
		b_res_on = false;
		b_res_off = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		memset(szOutnput, 0, FTD_BUF_SIZE);
		memset(szInput, 0, FTD_BUF_SIZE);
		strcpy(szInput, m_str_CMD.c_str());
		b_res_on = m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput);//turn on
	//	Sleep(m_i_SleepTime);
		cs_TestTime.Format(CW2A(L"runLEDTest - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);
		if (b_res_on)
		{
			str_msg = str_CMD_Msg_Pass;
		}
		else
		{
			str_msg = str_CMD_Msg_Fail;
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try =  showMsg(msg.c_str());
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = str_Result_Msg_Pass;
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = str_Result_Msg_Fail;
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		strcpy(szInput, m_str_OffCMD.c_str());
		b_res_off = m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput);//turn on
		//Sleep(m_i_OffSleepTime);
		if (b_res_off)
		{
			str_msg = str_CMD_Msg_Fail;
		}
		else
		{
			str_msg = str_CMD_Msg_Pass;
			break;
		}
		TraceLog(MSG_INFO, str_msg);

	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData(str_Pics, "FAIL");
		return false;
	}

	SetPICSData(str_Pics , "PASS");
	return true;
}


bool CWRITE::runOLEDTest()
{
	m_i_retriedTime = 0;
	m_strItemCode = CStr::IntToStr(Backlight_Test_BaseItemCode);
	CString cs_TestTime;
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutnput[FTD_BUF_SIZE] = {0};
	
	do
	{
		b_res_on = false;
		b_res_off = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		m_str_CMD = CW2A(L"1");// turn on 
		memset(szOutnput, 0, FTD_BUF_SIZE);
		memset(szInput, 0, FTD_BUF_SIZE);
		strcpy(szInput, m_str_CMD.c_str());
		b_res_on = m_pIPhone->FTD_OLEDTest(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput );//turn on
	//	Sleep(m_i_SleepTime);

		cs_TestTime.Format(CW2A(L"runOLEDTest - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);


		if (b_res_on)
		{
			str_msg = "runOLEDTest Pass";
		}
		else
		{
			str_msg = "runOLEDTest Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try =  showMsg(CW2A(L"Is OLED on?\n OLED有亮吗?\n"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "runOLEDTest pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "runOLEDTest fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		m_str_CMD = CW2A(L"0");// turn off
		strcpy(szInput, m_str_CMD.c_str());
		b_res_off = m_pIPhone->FTD_OLEDTest(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput );//turn on

		//Sleep(m_i_OffSleepTime);

		if (b_res_off)
		{
			str_msg = "runOLEDTest Cmd pass!";
		}
		else
		{
			str_msg = "runOLEDTest Cmd fail!";

			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData(str_Pics, "FAIL");
		return false;
	}

	SetPICSData(str_Pics, "PASS");
	return true;
}


bool CWRITE::runFindWifiAP()
{
	bool b_res = true;

	/* read ID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutnput[FTD_BUF_SIZE] = {0};
	strcpy(szInput, m_str_CMD.c_str());	
	if (!m_pIPhone->FTD_WLAN_ScanAP(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput ))
	{
		b_res = false;
	}

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"FindWifiAP PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "FindWifiAP PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		str_msg = CW2A(L"FindWifiAP ");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "FindWifiAP FAIL!");
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}


bool CWRITE::showMsg(const char* szMsg)
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
		i_waitCount++;
		Sleep(200);
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


bool CWRITE::ExecAdbOut(CString Command, char* output, char* ErrorCode)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString path_adb = pthToolDir + _T("\\adb.exe");
	if (_taccess(path_adb, 0) == -1)
	{
		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize))
	{
		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
		return false;
	}

	//HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	Command = _T("\"") + path_adb + _T("\" ") + Command;
	TRACE(_T("Cmd: %s\n"), Command);
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10 * 1000); // timeout in 10 seconds

		CloseHandle(hWrite);
		hWrite = NULL;
		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: Adb timeout");
		}
		else
		{

			isOk = true;
			DWORD bytesRead;
			char* message = new char[nPipeSize];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
			message[bytesRead] = '\0';

			strcpy(output, message);
			strcpy(ErrorCode, "Adb command ok");
			delete [] message;
		}
	}
	else
	{
		isOk = false;
		strcpy(ErrorCode, "ERROR: Execute adb.exe fail!");
	}
	Command.ReleaseBuffer();
	CloseHandle(hRead);
	if (hWrite)CloseHandle(hWrite);
	//CloseHandle(hProcess);

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	//hProcess = NULL;

	return isOk;
}