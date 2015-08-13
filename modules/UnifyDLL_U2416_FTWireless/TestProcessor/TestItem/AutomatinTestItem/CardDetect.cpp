/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CardDetect object.
 * FileName   : CardDetect.h
 * Version    : $Revision:   1.1  $
 * Author     : Lion.Wu
 * Time       : 2012.3.14
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/14    Lion.Wu             Create.

 /*
301 flag
2 pass
0 NONE_TEST
1 don't
255 Fail
*/
 ********************************************************************************/

#include "stdafx.h"
#include "CardDetect.h"


RF_IMPLEMENT_DYNCREATE(CardDetectProc)


bool CardDetectProc::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = m_str_Value;
	//m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CardDetectProc::Run()
{
	/* run different API function based on name */

	if (m_str_Name == SIMCardDetect)
	{
		m_strItemCode = CStr::IntToStr(SIMTest_BaseItemCode);
		if(m_strErrorCode.length() < 2) m_strErrorCode = FunErr_Detect_SIM_Card_Fail;
		return runSIMCardDetect();
	}

	if (m_str_Name == SDCardDetect)
	{
		m_strItemCode = CStr::IntToStr(SDCard_Test_BaseItemCode);
		if(m_strErrorCode.length() < 2) m_strErrorCode = FunErr_Detect_SD_Fail;
		return runSDCardDetect();
	}

	if (m_str_Name == SIMCardDataDetect)
	{
		m_strItemCode = CStr::IntToStr(SDCard_Test_BaseItemCode);
		if(m_strErrorCode.length() < 2) m_strErrorCode = FunErr_Detect_SD_Fail;
		return runSIMCardDataDetect();
	}

	if (m_str_Name == SDCardDataDetect)
	{
		m_strItemCode = CStr::IntToStr(SDCard_Test_BaseItemCode);
		if(m_strErrorCode.length() < 2) m_strErrorCode = FunErr_SD_ReadWrite_Data_Fail;
		return runSDCardDataDetect();
	}

	TraceLog(MSG_INFO, _T("TestItem can't fine"));
	return false;
}

bool CardDetectProc::PostRun()
{
	return true;
}

bool CardDetectProc::InitData(std::map<std::string, std::string>& paramMap)
{
	/* Name */
	if (paramMap.find(std::string("Name")) != paramMap.end())
	{
		m_str_Name = paramMap[std::string("Name")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'Name' for class 'CardDetectProc'"));
		return false;
	}

	/* Value */
	if (paramMap.find(std::string("Value")) != paramMap.end())
	{
		m_str_Value = paramMap[std::string("Value")];
	}
	else
	{
		//TraceLog(MSG_INFO, _T("Failed to Get filed 'CheckMatch' for class 'CardDetectProc'"));
		TraceLog(MSG_INFO, _T("Failed to Get filed 'Value' for class 'CardDetectProc'"));
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
		TraceLog(MSG_INFO, _T("Failed to Get filed 'CheckMatch' for class 'CardDetectProc'"));
		return false;
	}

	return true;
}

bool CardDetectProc::runSIMCardDetect()
{
	//-- SIM card test --//
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_SIMdetect(m_nFtdPort, m_nFtdTimeOut, sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Detect_SIM_Card_Fail;
		m_strMessage = _T("SIM_Card_Detect");
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData(_T("SIM_Card_Detect"), m_strResult);
		FactoryLog();
		return false;
	}

	CString sOutBuffer = sz_outBuffer;
	char sz_str_Value[256] = {0};
	strcpy(sz_str_Value, m_str_Value.c_str());
	m_strMeasured = sOutBuffer;
	
	bool b_res = true;
	if (sOutBuffer.Find(sz_str_Value) == -1)//0=no SIMCard, 1 = exist SIMCard, MMI_BB = 0 = no SIMCard
	{
		m_strResult = "FAIL";
		m_strMessage = "SIM_Card_Detect fail: ";
		TraceLog(MSG_INFO, "SIM_Card_Detect fail");
		SetPICSData("SIM_Card_Detect", "FAIL");
		b_res = false;
	}
	else
	{
		m_strResult = "PASS";
		m_strMessage = "SIM_Card_Detect Pass.";

		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("SIM_Card_Detect", m_strResult);
		m_strErrorCode = "-";
	}

	FactoryLog();
	return b_res;
}

bool CardDetectProc::runSDCardDetect()
{
	//-- SIM card test --//
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_SDdetect(m_nFtdPort, m_nFtdTimeOut, sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Detect_SIM_Card_Fail;
		m_strMessage = "SD_Card_Detect Fail.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("SD_Card_Detect", m_strResult);
		FactoryLog();
		return false;
	}

	bool b_res = true;
	CString sOutBuffer = sz_outBuffer;
	char sz_str_Value[256] = {0};
	strcpy(sz_str_Value, m_str_Value.c_str());
	m_strMeasured = sOutBuffer;
	if (sOutBuffer.Find(sz_str_Value) == -1)	//0=no SDCard, 1 = exist SDCard, MMI_BB = 0 = no SDCard
	{
		m_strResult = "FAIL";
		m_strMessage = "SD_Card_Detect fail: ";
		TraceLog(MSG_INFO, "SD_Card_Detect fail");
		/* Set PICS Data */
		SetPICSData("SD_Card_Detect", "FAIL");
		b_res = false;
	}
	else
	{
		m_strResult = "PASS";
		m_strMessage = "SD_Card_Detect Pass.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("SD_Card_Detect", m_strResult);
		m_strErrorCode = "-";
	}
	FactoryLog();
	return b_res;
}



bool CardDetectProc::runSIMCardDataDetect()
{
	bool b_res_msg = false;
	m_strErrorCode = FunErr_Detect_SIM_Card_Fail;
	b_res_msg = showMsg(CW2A(L"Please Insert SIM Card?\nÇë²åÈë SIM ¿¨?\n"));
	if (!b_res_msg)
	{
		m_strResult = "FAIL";
		m_strMessage = "runSIMCardDataDetect Fail 1.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("SIM_Card_Data_Detect", m_strResult);
		FactoryLog();
		return false;
	}

	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_SIMtest(m_nFtdPort, m_nFtdTimeOut, sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Detect_SIM_Card_Fail;
		m_strMessage = _T("FTD_SIMDatadetect");
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData(_T("SIM_Card_Data_Detect"), m_strResult);
		FactoryLog();
		return false;
	}

	m_strResult = "PASS";
	m_strMessage = "FTD_SIMDatadetect Pass.";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("SIM_Card_Data_Detect", m_strResult);
	m_strErrorCode = "-";

	FactoryLog();
	return true;
}



bool CardDetectProc::runSDCardDataDetect()
{
	bool b_res_msg = false;
	m_strErrorCode = FunErr_SD_ReadWrite_Data_Fail;
	b_res_msg = showMsg(CW2A(L"Please Insert SD Card?\nÇë²åÈë SD ¿¨?\n"));
	if (!b_res_msg)
	{
		m_strResult = "FAIL";
		m_strMessage = "runSDCardDataDetect Fail 1.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("SDCard_Data_Detect", m_strResult);
		FactoryLog();
		return false;
	}

	//-- SDCardDataDetect --//
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_SDdetectPin(m_nFtdPort, m_nFtdTimeOut, sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strMessage = "SDCard_Data_Detect Fail 2.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("SDCard_Data_Detect", m_strResult);
		FactoryLog();
		return false;
	}

	m_strResult = "PASS";
	m_strMessage = "SD_Card_Data_Detect Pass.";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("SDCard_Data_Detect", m_strResult);
	m_strErrorCode = "-";

	FactoryLog();
	return true;
}

bool CardDetectProc::ExecAdbOut(CString Command, char* output, char* ErrorCode)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));

	CString path_adb = pthToolDir + _T("\\adb.exe");
	if (_taccess(path_adb, 0) == -1) {
		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize)) {
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
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo)) {
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10*1000); // timeout in 10 seconds
		CloseHandle(hWrite);
		hWrite=NULL;
		//if timeout then exit the process
		if(TimeOutSignal == WAIT_TIMEOUT) {
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: Adb timeout");
		}
		else {
			isOk = true;
			DWORD bytesRead;
			char *message = new char[nPipeSize];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
			message[bytesRead] = '\0';

			strcpy(output, message);
			strcpy(ErrorCode, "Adb command ok");
			delete [] message;
		}
	}
	else {
		isOk = false;
		strcpy(ErrorCode, "ERROR: Execute adb.exe fail!");
	}
	Command.ReleaseBuffer();
	CloseHandle(hRead);
	if (hWrite) CloseHandle(hWrite);
	//CloseHandle(hProcess);
	//hProcess = NULL;

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	return isOk;
}

bool CardDetectProc::showMsg(char* szMsg)
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
