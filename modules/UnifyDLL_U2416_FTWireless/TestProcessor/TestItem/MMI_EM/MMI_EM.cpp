/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .h file of CMMIBB object.
 * FileName   : CMMIBB.h
 * Version    : $Revision:   1.1  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/04/15
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2013/04/15     Evonne.Hsieh             Create.

/*
301 flag
2	Pass
0	Non test
1	Don't
255 Fail
*/
******************************************************************************** /

#include "stdafx.h"
#include "MMI_EM.h"


RF_IMPLEMENT_DYNCREATE(MMIBBProc)

//get mudule
CString GetDirectoryPath()
{
	CString strDesPath = "D:\\Log\\SensorLog";
	CFileFind cfilefind;
	if (!cfilefind.FindFile(strDesPath))
	{
		if (!CreateDirectory(strDesPath, NULL))
			AfxMessageBox(_T("Create SensorLog folder Img Failed!"));
	}
	return strDesPath;
}

bool MMIBBProc::InitData(std::map<std::string, std::string>& paramMap)
{
	/* Offset */
	std::string str_offset;
	if (paramMap.find(std::string("Offset")) != paramMap.end())
	{
		str_offset = paramMap[std::string("Offset")];
		m_i_offset = CStr::StrToInt(str_offset);
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get 'Offset' for class 'CMMIBBProc'");
		return false;
	}

	/* Message */
	if (paramMap.find(std::string("Message")) != paramMap.end())
	{
		m_str_Message = paramMap[std::string("Message")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get 'Message' for class 'MMIBBProc'");
		return false;
	}

	/* Name */
	if (paramMap.find(std::string("Name")) != paramMap.end())
	{
		m_str_Name = paramMap[std::string("Name")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get 'Name' for class 'MMIBBProc'");
		return false;
	}

	/* ErrorCode */
	if (paramMap.find(std::string("ErrorCode")) != paramMap.end())
	{
		m_str_ErrorCode = paramMap[std::string("ErrorCode")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get 'ErrorCode' for class 'MMIBBProc'");
		return false;
	}

	return true;
}

bool MMIBBProc::PreRun()
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

	char* szAddress = "504,50";// 504~511 = errCode, 512~540 = test item
	memset(m_szFAData, 0, sizeof(m_szFAData));
	if (!m_pIPhone->FTD_FAC_CFGRead(m_nFtdPort, m_nFtdTimeOut, szAddress, m_szFAData))
		return false;

	/* read Error Code, 504~511 = errCode */
	memset(m_szErrCode, 0, sizeof(m_szErrCode));
	strncpy(m_szErrCode, m_szFAData, 8);
	return true;
}

bool MMIBBProc::Run()
{
	/* run different API function based on name */
	if (m_str_Name == "GetSensorLog")
	{
		m_strItemCode = CStr::IntToStr(SensorLog_BaseItemCode);
		return 	GetSensorLog();
	}
	else if (m_str_Name == "NFCSWPTest")
	{
		m_strItemCode = CStr::IntToStr(NFCMODEL_Test_BaseItemCode);
		return Detroit_NFCSWPTest();
	}
	else
	{
		m_strItemCode = CStr::IntToStr(NFCMODEL_Test_BaseItemCode);
		return MMI_PASS_FAIL_CRITERIA();
	}
	return true;
}

bool MMIBBProc::PostRun()
{
	if (m_hNfcFuncDLL != NULL)
	{
		FreeLibrary(m_hNfcFuncDLL);
		m_hNfcFuncDLL = NULL;
	}
	return true;
}

bool MMIBBProc::MMI_PASS_FAIL_CRITERIA()
{
	bool b_res = true;

	if (m_szFAData[m_i_offset - 504] == 2)
	{
		m_strResult = "PASS";
		m_strMessage = m_str_Message + "_PASS";
		SetPICSData("MMIEM_RESULT", m_str_Message + "_PASS");
	}
	else if (m_szFAData[m_i_offset - 504] == -1)
	{
		m_strResult = "FAIL";
		m_strMessage = m_str_Message + "_FAIL";
		m_strErrorCode = m_szErrCode;
		SetPICSData("MMIEM_ErrCode", m_strErrorCode);
		b_res = false;
	}
	else if (m_szFAData[m_i_offset - 504] == 1)
	{
		m_strResult = "Don't_Care";
		m_strMessage = m_str_Message + "_Don't_Care";
		SetPICSData("MMIEM_RESULT", m_str_Message + "_DONT_CARE");
	}
	else
	{
		m_strResult = "Non_test";
		m_strMessage = m_str_Message + "_Non_Test";
		SetPICSData("MMIEM_RESULT", m_str_Message + "_NON_TEST");
		b_res = false;
	}
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return b_res;
}

bool MMIBBProc::Detroit_NFCSWPTest()
{
	bool b_res = false;
	std::string str_msg;
	int nRetry = 2;

	CString Command = _T("shell getprop");
	char output[50000] = {0};
	char ErrorCode[50000] = {0};
	if (!(b_res = ExecAdbOut(Command, output, ErrorCode)))
	{
		str_msg = "ExecAdbOut Command fail";
		TraceLog(MSG_INFO, str_msg);
		m_strMessage = "ExecAdbOut Command fail.  ";
	}
	else
	{
		CString NfcStatus = output;
		if (NfcStatus.Find(_T("[runtime.mmimode.nfc]: [off]")) != -1)
		{
			const int nRetry = 2;
			for (int nTry = 0; nTry < nRetry; nTry++)
			{
				char sz_input[FTD_BUF_SIZE] = {0};
				char sz_outData[FTD_BUF_SIZE] = {0};
				if (!(b_res = m_pIPhone->FTD_NFCSwpSelftest(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_outData)))
				{
					str_msg = "NFC SWP Self Command fail";
					TraceLog(MSG_INFO, str_msg);
					break;
				}
				else
				{
					m_strMeasured = sz_outData;

					if (strncmp(m_strMeasured.c_str(), _T("0,1"), 3) == 0)
					{
						b_res = true;
						break;
					}
					else
					{
						b_res = false;
					}
				}
				Sleep(1000);
			}
		}
		else if (NfcStatus.Find(_T("[runtime.mmimode.nfc]: [on]")) != -1)
		{
			b_res = false;
			TraceLog(MSG_INFO, "nfc status: on");
			m_strMessage = "nfc status: on.  ";
		}
		else if (NfcStatus.Find(_T("[runtime.mmimode.nfc]: [timeout]")) != -1)
		{
			b_res = false;
			m_strMessage = "nfc status: timeout.  ";
			TraceLog(MSG_INFO, "nfc status: timeout");
		}
	}

	memset(output, 0, sizeof(ErrorCode));
	memset(ErrorCode, 0, sizeof(ErrorCode));
	Command = _T("shell am broadcast -a android.intent.action.nfc.power --ez state true");
	if (!(b_res = ExecAdbOut(Command, output, ErrorCode)))
	{
		str_msg = "ExecAdbOut Command fail";
		TraceLog(MSG_INFO, str_msg);
		m_strMessage = "ExecAdbOut Command fail.";
	}
	Sleep(3000);

	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "NFC SWP Test pass!";
		TraceLog(MSG_INFO, "NFC SWP Test pass!");
		SetPICSData("MMIEM_RESULT", "NFCSWPTest_PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage += "NFC SWP Test fail";
		TraceLog(MSG_INFO, "NFC SWP Test fail");
		m_strErrorCode = m_str_ErrorCode;
		SetPICSData("MMIEM_RESULT", m_strErrorCode);
	}
	FactoryLog();
	return b_res;
}

bool MMIBBProc::NFCOffline0CmTestAuto()
{
	bool b_res = true;
	/* Init dialog response flag to false */
	m_b_DialogResponse = false;
	m_b_waitFlag = false;
	/* Notify UI to show message dialog */
	ShowDialog(CW2A(L"请按音量下键并拔数据线关机。关机後将手机放到NFC读卡机上测试"));

	// connect to nfc card reader
	b_res = Test_NFC_IT100MUR();

	/* Wait UI response(Max:10s) */
	int i_waitCount = 0;
	while (m_b_waitFlag)
	{
		if (i_waitCount == 50)
		{
			TraceLog(MSG_INFO, "Wait UI response time out.");
			b_res = false;
			break;
		}
		i_waitCount++;
		Sleep(200);
	}

	if (b_res)
	{
		m_strResult = "PASS";
		TraceLog(MSG_INFO, "NFC Offline 0 cm test pass.");
		SetPICSData("MMIEM_RESULT", "NFC_Offline_0cm_Pass");
	}
	else
	{
		b_res = false;
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_NFC_Card_Mode_0cm_PowerOff_Fail;
		TraceLog(MSG_INFO, "NFC Offline 0 cm test fail.");
		SetPICSData("MMIEM_RESULT", m_strErrorCode);
	}
	FactoryLog();
	return b_res;
}

bool MMIBBProc::GetSensorLog()
{
	bool isPass = true;
	char szOutPut[4096] = {0};
	char szErrorCode[4096] = {0};
	CString csDirectory = GetDirectoryPath();
	csDirectory += "\\";
	CString csPicasso = g_strPicasso.c_str();
	CString csAdbCommand;
	csAdbCommand.Format(_T("%s %s%s%s"), _T("pull /data/data/com.oem.engineeringmode.gsensormanualtest/GsensorLog.txt"), csDirectory, csPicasso, _T("GsensorLog.txt"));
	if (!ExecAdbOut(csAdbCommand, szOutPut, szErrorCode))
	{
		TraceLog(MSG_ERROR, "pull GsensorLog301 fail ");
		isPass = false;
	}

	if (!csPicasso.IsEmpty())
	{
		CString csFileName = csPicasso + _T(".log");
		csAdbCommand.Format(_T("%s%s %s%s"), _T("pull /data/data/"), csFileName, csDirectory, csFileName);
	}
	else
	{
		csAdbCommand.Format(_T("%s  %s%s"), _T("pull /data/data/UnknownPhoneId.log"), csDirectory, _T("UnknownPhoneId.log"));
	}
	if (!ExecAdbOut(csAdbCommand, szOutPut, szErrorCode))
	{
		TraceLog(MSG_ERROR, "pull 301 Log fail ");
		isPass = false;
	}

	if (isPass)
	{
		SetPICSData("MMIEM_RESULT", "Get_GSensor_Pass");
	}
	else
	{
		m_strErrorCode = m_str_ErrorCode;
		SetPICSData("MMIEM_RESULT", m_strErrorCode);
	}
	return isPass;
}

bool MMIBBProc::ExecAdbOut(CString Command, char* output, char* ErrorCode)
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

bool MMIBBProc::Test_NFC_IT100MUR()
{
	CString cstrPthToolDir;
	::GetModuleFileName(NULL, cstrPthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	cstrPthToolDir.ReleaseBuffer();
	cstrPthToolDir = cstrPthToolDir.Left(cstrPthToolDir.ReverseFind('\\'));
	CString cstrPthDll = cstrPthToolDir + _T("\\WinNfc.dll");
	m_hNfcFuncDLL = ::LoadLibrary(cstrPthDll);
	if (m_hNfcFuncDLL == NULL)
	{
		TraceLog(MSG_INFO, "load nfc.dll fail");
		return false;
	}

	//initialize NFC functions
	PNfcEstablishContext p_NfcEstablishContext = (PNfcEstablishContext)::GetProcAddress(m_hNfcFuncDLL, _T("NfcEstablishContext"));
	NFCCONTEXT phCtx = NULL;
	LONG rc = 0;
	rc = p_NfcEstablishContext(0, 0, 0, &phCtx);
	if (rc)
	{
		ShowNfcCardReaderError(phCtx, rc);
		return false;
	}
	TraceLog(MSG_INFO, "initialize NFC functions successfully");

	//connect to the reader
	PNfcConnect p_NfcConnect = (PNfcConnect)::GetProcAddress(m_hNfcFuncDLL, _T("NfcConnect"));
	rc = p_NfcConnect(phCtx, NULL, 0);
	if (rc)
	{
		ShowNfcCardReaderError(phCtx, rc);
		return false;
	}
	TraceLog(MSG_INFO, "connect to reader successfully");

	//connect card
	int i_Count = 0;
	while (1)
	{
		if (i_Count == 20)
		{
			break;
		}
		else
		{
			PNFC_ConnectCard p_NFC_ConnectCard = (PNFC_ConnectCard)::GetProcAddress(m_hNfcFuncDLL, _T("NFC_ConnectCard"));
			rc = p_NFC_ConnectCard(phCtx, 0, 0);
		}

		if (!rc)
		{
			break;
		}
		Sleep(500);
		i_Count++;
	}
	if (rc)
	{
		ShowNfcCardReaderError(phCtx, rc);
		return false;
	}
	TraceLog(MSG_INFO, "connect NFC card successfully");
	PNfcBuzzerControl p_NfcBuzzerControl = (PNfcBuzzerControl)::GetProcAddress(m_hNfcFuncDLL, _T("NfcBuzzerControl"));
	p_NfcBuzzerControl(phCtx, BUZZER_ON);
	Sleep(50);
	p_NfcBuzzerControl(phCtx, BUZZER_OFF);

	i_Count = 0;
	BYTE byATR[256] = {0};
	DWORD dwLen;
	DWORD dwActiveProtocol;
	PNFC_GetATR p_NFC_GetATR = (PNFC_GetATR)::GetProcAddress(m_hNfcFuncDLL, _T("NFC_GetATR"));
	while (1)
	{
		if (i_Count == 2) break;

		rc = p_NFC_GetATR(phCtx, byATR, &dwLen, &dwActiveProtocol);//get ATR
		if (!rc)
			break;
		Sleep(500);
		i_Count++;
	}

	if (rc)
	{
		ShowNfcCardReaderError(phCtx, rc);
		return false;
	}
	TraceLog(MSG_INFO, "get ATR successfully");
	PrintData(byATR, dwLen);

	//1.select AID APDU
	BYTE EMV_T1[] = {0x00, 0xA4, 0x04, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x04, 0x10, 0x10, 0x00};
	BYTE Rec[262] = {0};
	dwLen = 262;
	i_Count = 0;
	PNFC_SendAPDU p_NFC_SendAPDU = (PNFC_SendAPDU)::GetProcAddress(m_hNfcFuncDLL, _T("NFC_SendAPDU"));
	while (1)
	{
		if (i_Count == 2) break;

		//transmit APDU command
		rc = p_NFC_SendAPDU(phCtx, (LPCBYTE)EMV_T1, 13, Rec, &dwLen);
		if (!rc)
			break;
		Sleep(500);
		i_Count++;
	}
	if (rc)
	{
		ShowNfcCardReaderError(phCtx, rc);
		return false;
	}
	TraceLog(MSG_INFO, "select AID");
	PrintData(Rec, dwLen);

	//2.EMV_ReadRecord
	BYTE ReadRecord[] = {0x00, 0xB2, 0x01, 0x0C, 0x00};
	memset(Rec, 0x00, 262);
	dwLen = 262;
	i_Count = 0;
	while (1)
	{
		if (i_Count == 2) break;

		//transmit APDU command
		rc = p_NFC_SendAPDU(phCtx, (LPCBYTE)ReadRecord, 5, Rec, &dwLen);
		if (!rc)
			break;
		Sleep(500);
		i_Count++;
	}
	if (rc)
	{
		ShowNfcCardReaderError(phCtx, rc);
		return false;
	}

	TraceLog(MSG_INFO, "read Record");
	PrintData(Rec, dwLen);

	//disconnect card
	PNFC_DisconnectCard p_NFC_DisconnectCard = (PNFC_DisconnectCard)::GetProcAddress(m_hNfcFuncDLL, _T("NFC_DisconnectCard"));
	rc = p_NFC_DisconnectCard(phCtx, 0);
	if (rc)
	{
		ShowNfcCardReaderError(phCtx, rc);
		return false;
	}
	TraceLog(MSG_INFO, "disconnect NFC card successfully");

	//disconnect device
	PNfcDisconnect p_NfcDisconnect = (PNfcDisconnect)::GetProcAddress(m_hNfcFuncDLL, _T("NfcDisconnect"));
	rc = p_NfcDisconnect(phCtx, 0);
	if (rc)
	{
		ShowNfcCardReaderError(phCtx, rc);
		return false;
	}
	TraceLog(MSG_INFO, "disconnect from reader successfully");

	//release NFC functions
	PNfcReleaseContext p_NfcReleaseContext = (PNfcReleaseContext)::GetProcAddress(m_hNfcFuncDLL, _T("NfcReleaseContext"));
	p_NfcReleaseContext(phCtx);
	TraceLog(MSG_INFO, "release NFC functions successfully");

	return true;
}

void MMIBBProc::ShowNfcCardReaderError(NFCCONTEXT ctx, LONG rc)
{
	char msg[50] = {0};
	PNfcDisconnect p_NfcDisconnect = (PNfcDisconnect)::GetProcAddress(m_hNfcFuncDLL, _T("NfcDisconnect"));
	PNfcReleaseContext p_NfcReleaseContext = (PNfcReleaseContext)::GetProcAddress(m_hNfcFuncDLL, _T("NfcReleaseContext"));
	p_NfcDisconnect(ctx, 0);
	p_NfcReleaseContext(ctx);
	sprintf(msg, "error = %X\n", rc);
	TraceLog(MSG_INFO, msg);
}

void MMIBBProc::PrintData(PBYTE pbData, DWORD dwLen)
{
	char msg[1024] = {0};
	for (int i = 0; i < (int)dwLen; i++)
		sprintf(&msg[i * 2], "%02X ", *(pbData + i));

	TraceLog(MSG_INFO, msg);
}
