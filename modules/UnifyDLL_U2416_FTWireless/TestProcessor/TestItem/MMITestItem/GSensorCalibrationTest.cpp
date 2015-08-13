/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .cpp file of CGSensorCalibration object.
 * FileName   : GSensorCalibrationTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/06/12
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2012/06/12    Evonne.Hsieh          Create.
 ********************************************************************************/

#include "stdafx.h"
#include "GSensorCalibrationTest.h"


RF_IMPLEMENT_DYNCREATE(CGSensorCalibration)

bool CGSensorCalibration::PreRun()
{
	m_strItemCode = CStr::IntToStr(GSensor_Calibration_BaseItemCode);
	m_strErrorCode = FunErr_GSensor_Calibration_Fail;
	m_strResult = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CGSensorCalibration::Run()
{
	bool bRes = false;

	CString csCommandShell(_T("shell rm -r /data/misc/sensor/profile_calib_a"));
	char szOutPullCommand[4096] = {0};
	char szErrorCodePullCommand[4096] = {0};
	ExecAdbOut(csCommandShell, szOutPullCommand, szErrorCodePullCommand);


	bRes = GSensorCalibrationTest();
	return bRes;
}

bool CGSensorCalibration::PostRun()
{
	return true;
}

bool CGSensorCalibration::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string strSleepTime;
	if (paramMap.find(std::string("SleepTime")) != paramMap.end())
	{
		strSleepTime = paramMap[std::string("SleepTime")];
		m_i_wait_for_k = CStr::StrToInt(strSleepTime);
	}
	else
	{
		TraceLog(MSG_ERROR,_T("Failed to Get SleepTime for class CGSensorCalibration"));
		return false;
	}
	return true;
}

bool CGSensorCalibration::GSensorCalibrationTest()
{
	bool b_res = false;
	CString msg;
	std::string str_msg;
	std::string str_msgGsensorData;

	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_SNSRgsensorCalib(m_nFtdPort, m_nFtdTimeOut,sz_input, sz_outData)))
	{
		str_msg = "GSensor Calibration Command fail";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		CString sOutBuffer;
		sOutBuffer = sz_outData;
		str_msgGsensorData = sz_outData;
		if (sOutBuffer.Find(_T("PASSED")) == -1)
		{
			b_res = false;
		}
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "GSensor Calibration Test pass!";
		TraceLog(MSG_INFO, "GSensor Calibration Test pass!");
		SetPICSData("GSENSOR_CALIBRATION_TEST", str_msgGsensorData );
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "GSensor Calibration Test fail: " + msg;
		TraceLog(MSG_INFO, "GSensor Calibration Test fail");
		SetPICSData("GSENSOR_CALIBRATION_TEST", "FAIL");
	}
	FactoryLog();
	return b_res;
}


bool CGSensorCalibration::ExecAdbOut(CString Command, char* output, char* ErrorCode)
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