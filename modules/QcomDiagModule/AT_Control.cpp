#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\ATRequest.h"


QCOM_DIAG_API bool ATControl_SPU(const char* szPort, char* szCommand, char* szResponse)
{
	HANDLE hCOMPort;
	DCB dcb;
	COMMTIMEOUTS commTimeouts;
	DWORD iBytesWritten;
	DWORD iBytesRead;

	char szFileName[32];
	memset(szFileName, 32, 0);
	sprintf_s(szFileName, 31, "\\\\.\\COM%s", szPort);

	hCOMPort = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE , 0, NULL, OPEN_EXISTING , 0, NULL);

	if (hCOMPort == INVALID_HANDLE_VALUE)
		return false;

	if (SetupComm(hCOMPort, 128, 128) != TRUE)
	{
		CloseHandle(hCOMPort);
		return false;
	}

	if (GetCommState(hCOMPort, &dcb) != TRUE)
	{
		CloseHandle(hCOMPort);
		return false;
	}

	dcb.BaudRate = 9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fAbortOnError = TRUE;

	if (SetCommState(hCOMPort, &dcb) != TRUE)
	{
		CloseHandle(hCOMPort);
		return false;
	}

	if (GetCommTimeouts(hCOMPort, &commTimeouts) != TRUE)
	{
		CloseHandle(hCOMPort);
		return false;
	}

	commTimeouts.ReadIntervalTimeout = 50;
	commTimeouts.ReadTotalTimeoutConstant = 50;
	commTimeouts.ReadTotalTimeoutMultiplier = 10;
	commTimeouts.WriteTotalTimeoutConstant = 50;
	commTimeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hCOMPort, &commTimeouts) != TRUE)
	{
		CloseHandle(hCOMPort);
		return false;
	}

	// COM port is now ready for AT command

	int iLength = strlen(szCommand);
	char* szCmd = new char[iLength + 2];
	memset(szCmd, 0, iLength + 2);
	sprintf_s(szCmd, iLength + 1, szCommand);
	szCmd[iLength] = '\r';

	WriteFile(hCOMPort, szCmd, iLength + 1, &iBytesWritten, NULL);
	Sleep(500);
	ReadFile(hCOMPort, (void*)szResponse, 255, &iBytesRead, NULL);

	delete [] szCmd;

	CloseHandle(hCOMPort);
	return true;
}
