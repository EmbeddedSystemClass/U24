#include "stdafx.h"
#include <cstring>
#include <sstream>
#include <fstream>
#include "GeneralDLLApi.h" // GeneralCmd
#include "io.h"
#include "../Utility.h"
#include "../QisdaAndroidCmdCtrlV2.h"

using namespace std;

extern CQisdaAndroidCmdCtrlApp theApp;


inline void CmdCtrlInit(int port)
{
	if (theApp.m_pCmdCtrl[port] == NULL)
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
}

inline void RetCodeCheck(int port, int& ret_code)
{
	if (ret_code != HS_OK)
		ret_code += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;

	TRACER_STREAM("port:" << port << "=>code:" << ret_code << "\r\n");
}

NEPDLLAPI int __stdcall SendDataToLog(int port, int timeout_ms, char* szLog)
{
	TRACER_FUNCTION_IN();
	TRACER_STREAM("port:" << port << ",timeout:" << timeout_ms << ",log:" << szLog << "\r\n");
	return HS_OK;
}

NEPDLLAPI int __stdcall InitDevice(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	CmdCtrlInit(port);
	int ret_code = theApp.m_pCmdCtrl[port]->InitDevice(port);
	RetCodeCheck(port, ret_code);

	/////////////////////////////////////////////////////////////////////
	TRACER_STREAM("port:" << port << "=>code:" << ret_code << "\r\n");
	/////////////////////////////////////////////////////////////////////
	return ret_code;
}

NEPDLLAPI int __stdcall FreeDevice(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	CmdCtrlInit(port);
	int ret_code = theApp.m_pCmdCtrl[port]->FreeDevice(port);
	RetCodeCheck(port, ret_code);

	TRACER_STREAM("port:" << port << "=>code:" << ret_code << "\r\n");
	return ret_code;
}

NEPDLLAPI int __stdcall OpenDevice(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	CmdCtrlInit(port);
	int ret_code = theApp.m_pCmdCtrl[port]->open(port, timeout_ms);
	RetCodeCheck(port, ret_code);

	TRACER_STREAM("port:" << port << "=>code:" << ret_code << "\r\n");
	return ret_code;
}

NEPDLLAPI int __stdcall CloseDevice(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	CmdCtrlInit(port);
	int ret_code = theApp.m_pCmdCtrl[port]->close();
	RetCodeCheck(port, ret_code);

	TRACER_STREAM("port:" << port << "=>code:" << ret_code << "\r\n");
	return ret_code;
}

NEPDLLAPI int __stdcall ExactOpenDevice(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	return OpenDevice(port, timeout_ms);
}

NEPDLLAPI int __stdcall ExactCloseDevice(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	return CloseDevice(port, timeout_ms);
}

NEPDLLAPI int __stdcall ExactReadDevice(int port, int timeout_ms, char* in_buf, int InSize)
{
	TRACER_FUNCTION_IN();

	CmdCtrlInit(port);
	int ret_code = theApp.m_pCmdCtrl[port]->ExactReadDevice(port, timeout_ms, in_buf, InSize) ;
	RetCodeCheck(port, ret_code);
	return ret_code;
}

NEPDLLAPI int __stdcall ExactWriteDevice(int port, int timeout_ms, char* in_buf, int InSize)
{
	TRACER_FUNCTION_IN();

	CmdCtrlInit(port);
	int ret_code = theApp.m_pCmdCtrl[port]->ExactWriteDevice(port, timeout_ms, in_buf, InSize) ;
	RetCodeCheck(port, ret_code);
	return ret_code;
}

//110420 gilpin add for receive serial command such as touch panel pixel and key value
NEPDLLAPI int __stdcall ReceiveCmdOut(int port, int timeout_ms, byte* pkt_cmd, char* out_buf)
{
	TRACER_FUNCTION_IN();

	CmdCtrlInit(port);
	int ret_code = theApp.m_pCmdCtrl[port]->ReceiveCmdOut(port, timeout_ms, pkt_cmd, out_buf);
	RetCodeCheck(port, ret_code);
	return ret_code;
}

NEPDLLAPI int __stdcall GeneralCmd(int port, int timeout_ms, byte* pkt_cmd)
{
	TRACER_FUNCTION_IN();

	CmdCtrlInit(port);
	int ret_code = theApp.m_pCmdCtrl[port]->GeneralCmd(port, timeout_ms, pkt_cmd);
	RetCodeCheck(port, ret_code);
	return ret_code;
}

NEPDLLAPI int __stdcall GeneralCmdOut(int port, int timeout_ms, byte* pkt_cmd, char* out_buf)
{
	TRACER_FUNCTION_IN();

	CmdCtrlInit(port);
	int ret_code = theApp.m_pCmdCtrl[port]->GeneralCmdOut(port, timeout_ms, pkt_cmd, out_buf);
	RetCodeCheck(port, ret_code);
	return ret_code;
}

NEPDLLAPI int __stdcall GeneralCmdIn(int port, int timeout_ms, byte* pkt_cmd, char* in_buf, size_t in_size)
{
	TRACER_FUNCTION_IN();

	CmdCtrlInit(port);
	int ret_code = theApp.m_pCmdCtrl[port]->GeneralCmdIn(port, timeout_ms, pkt_cmd, in_buf, in_size);
	RetCodeCheck(port, ret_code);
	return ret_code;
}

NEPDLLAPI int __stdcall GeneralCmdInOut(int port, int timeout_ms, byte* pkt_cmd, char* in_buf, size_t in_size, char* out_buf)
{
	TRACER_FUNCTION_IN();

	CmdCtrlInit(port);
	int ret_code = theApp.m_pCmdCtrl[port]->GeneralCmdInOut(port, timeout_ms, pkt_cmd, in_buf, in_size, out_buf);
	RetCodeCheck(port, ret_code);
	return ret_code;
}

// 2009-10-27 Mick: power on to DT UART packet mode
// send ASCII 'A' for 5 second, recieve zzz is success
NEPDLLAPI int __stdcall PowerOnToUARTPacketMode(int port, int timeout_ms, int DTRFlag)
{
	TRACER_FUNCTION_IN();

	CmdCtrlInit(port);
	int ret_code = theApp.m_pCmdCtrl[port]->NormalCmdPowerOnToUARTPacketMode(port, timeout_ms, DTRFlag);
	RetCodeCheck(port, ret_code);
	return ret_code;
}

NEPDLLAPI bool __stdcall ExecAdbOut(CString Command, char* output, char* error_msg)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	/*CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));*/
	CString path_adb = _T("adb.exe");
	if (_taccess(path_adb, 0) == -1)
	{
		strcpy(error_msg, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize))
	{
		strcpy(error_msg, "ERROR: CreatePipe fail!");
		return false;
	}

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
		Command.ReleaseBuffer();
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10 * 1000); // timeout in 10 seconds

		CloseHandle(hWrite);
		hWrite = NULL;

		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(error_msg, "ERROR: Adb timeout");
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
			strcpy(error_msg, "Adb command ok");
			delete [] message;
		}
	}
	else
	{
		isOk = false;
		strcpy(error_msg, "ERROR: Execute adb.exe fail!");

		CloseHandle(hWrite);
		hWrite = NULL;
	}

	CloseHandle(hRead);
	CloseHandle(processInfo.hProcess);
	processInfo.hProcess = NULL;

	return isOk;
}

NEPDLLAPI bool __stdcall ExecFastbootOut(CString Command, char* output, char* error_msg)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString path_Fastboot = pthToolDir + _T("\\Fastboot.exe");
	if (_taccess(path_Fastboot, 0) == -1)
	{
		strcpy(error_msg, "ERROR: No Fastboot.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize))
	{
		strcpy(error_msg, "ERROR: CreatePipe fail!");
		return false;
	}

	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	Command = _T("\"") + path_Fastboot + _T("\" ") + Command;
	TRACE(_T("Cmd: %s\n"), Command);
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		Command.ReleaseBuffer();
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10 * 1000); // timeout in 10 seconds

		CloseHandle(hWrite);
		hWrite = NULL;

		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(error_msg, "ERROR: Fastboot timeout");
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
			strcpy(error_msg, "Fastboot command ok");
			delete [] message;
		}
	}
	else
	{
		isOk = false;
		strcpy(error_msg, "ERROR: Execute Fastboot.exe fail!");

		CloseHandle(hWrite);
		hWrite = NULL;
	}

	CloseHandle(hRead);
	CloseHandle(processInfo.hProcess);
	processInfo.hProcess = NULL;

	return isOk;
}

int CheckFtdMode(char* error_msg)
{
	TRACER_FUNCTION_IN();

	int ret_code = -1 * ERROR_FUNCTION_FAILED;
	char* msg_buffer = new char[1024 * 1024 * 2];
	if (ExecAdbOut("shell ps", msg_buffer, error_msg))
	{
		if (strstr(msg_buffer, "ftd") != NULL)
		{
			ret_code = HS_OK;
		}
		else
		{
			strcpy(error_msg, "ERROR: Not in ftd mode");
			ret_code = -1 * ERROR_BAD_PROFILE;
		}
	}
	else
	{
		ret_code = -1 * ERROR_BAD_COMMAND;
	}
	delete[] msg_buffer;
	return ret_code;
}

NEPDLLAPI int __stdcall TestFtdMode(int port, int timeout_ms, char* error_msg)
{
	TRACER_FUNCTION_IN();

	int ret_code = -1 * ERROR_FUNCTION_FAILED;
	ret_code = CheckFtdMode(error_msg);
	if (ret_code != HS_OK)
	{
		strcpy(error_msg, "ERROR: Check ftd mode fail");
		ret_code = -1 * ERROR_BAD_COMMAND;
	}
	return ret_code;
}

static int EnterFtdMode(int timeout_ms, char* error_msg)
{
	TRACER_FUNCTION_IN();

	/*CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));*/

	int ret_code = -1 * ERROR_FUNCTION_FAILED;
	CString adb_path = _T("adb.exe");
	CString param = _T("shell ftd");
	if (Exec(adb_path, param, timeout_ms, true))
	{
		if (param.Find(_T("system/core/ftd")) != -1)
		{
			ret_code = HS_OK;
		}
		else
		{
			strcpy(error_msg, "ERROR: Enter ftd mode fail");
			ret_code = -1 * ERROR_BAD_PROFILE;
		}
	}
	else
	{
		ret_code = -1 * ERROR_BAD_COMMAND;
	}
	return ret_code;
}

NEPDLLAPI int __stdcall AutoFtdMode(int port, int timeout_ms, char* error_msg)
{
	TRACER_FUNCTION_IN();

	int ret_code = -1 * ERROR_FUNCTION_FAILED;
	ret_code = CheckFtdMode(error_msg);
	if (ret_code != HS_OK)
	{
		ret_code = EnterFtdMode(0, error_msg);
		if (ret_code != HS_OK)
		{
			strcpy(error_msg, "ERROR: Enter ftd mode fail");
			ret_code = -1 * ERROR_BAD_COMMAND;
		}
		else
		{
			ret_code = CheckFtdMode(error_msg);
			if (ret_code != HS_OK)
			{
				strcpy(error_msg, "ERROR: Check ftd mode fail");
				ret_code = -1 * ERROR_BAD_COMMAND;
			}
		}
	}
	if (ret_code != HS_OK)
	{
		strcpy(error_msg, "ERROR: Check ftd mode fail");
		ret_code = -1 * ERROR_BAD_COMMAND;
	}
	return ret_code;
}
