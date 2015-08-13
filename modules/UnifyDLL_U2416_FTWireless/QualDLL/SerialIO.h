// SerialIO.h: interface for the CSerialIO class.
//
//////////////////////////////////////////////////////////////////////
#ifndef SERIALIO_H
#define SERIALIO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SimpleCriticalSection.h"

#define	DEF_READ_INTERVAL_TIMEOUT			MAXDWORD
#define	DEF_READ_TOTAL_TIMEOUT_MULTIPLIER	MAXDWORD
#define	DEF_READ_TOTAL_TIMEOUT_CONSTANT		500
#define	DEF_WRITE_TOTAL_TIMEOUT_MULTIPLIER	1
#define	DEF_WRITE_TOTAL_CONSTANT			50

#define RECV_BUFFER_SIZE		8192
#define SEND_BUFFER_SIZE		4096
#define COM_PORT_NAME_LENGTH	16
#define LISTENER_TIME_OUT		6000
#define SPEED_ARRAY_SIZE		3
#define DIAG_MAX_RESPONSE_LEN	8192
#define ESCAPE_CHAR				0x7D
#define END_CHAR				0x7E
#define COMPLEMENT_CHAR			0x20
#define START_CHAR				0x7E

class CSerialIO
{
public:
	// Baudrate
	typedef enum {
		EBaudUnknown = -1,			// Unknown
		EBaud110     = CBR_110,		// 110 bits/sec
		EBaud300     = CBR_300,		// 300 bits/sec
		EBaud600     = CBR_600,		// 600 bits/sec
		EBaud1200    = CBR_1200,	// 1200 bits/sec
		EBaud2400    = CBR_2400,	// 2400 bits/sec
		EBaud4800    = CBR_4800,	// 4800 bits/sec
		EBaud9600    = CBR_9600,	// 9600 bits/sec
		EBaud14400   = CBR_14400,	// 14400 bits/sec
		EBaud19200   = CBR_19200,	// 19200 bits/sec (default)
		EBaud38400   = CBR_38400,	// 38400 bits/sec
		EBaud56000   = CBR_56000,	// 56000 bits/sec
		EBaud57600   = CBR_57600,	// 57600 bits/sec
		EBaud115200  = CBR_115200,	// 115200 bits/sec
		EBaud128000  = CBR_128000,	// 128000 bits/sec
		EBaud230400	 = 230400,
		EBaud256000  = CBR_256000,	// 256000 bits/sec
		EBaud460800	 = 460800,
	} EBaudrate;

	CSerialIO();
	virtual ~CSerialIO();
	LONG GetLastError () const { return m_lLastError; }

	virtual void OnEvent () = 0;
	LONG Open(LPCTSTR lpszDevice, bool isEmdl, EBaudrate eBaudrate = EBaud115200);
	LONG Close();
	LONG Write(const void* pData, size_t iLen, DWORD* pdwWritten = 0);
	LONG Read(void* pData, size_t iLen, DWORD* pdwRead = 0);

	typedef enum 
	{
		EDIAG	= 5,
		EDL		= 6
	} EState;
	EState m_eState;
	EState GetState() { return m_eState; }
	void SetState(EState eState) { m_eState = eState; }

protected:
	CSimpleCriticalSection m_csWrite;
	CSimpleCriticalSection m_csRead;

	EBaudrate m_eBaudrate;

	LONG	m_lLastError;
	HANDLE	m_hFile;
	HANDLE	m_hReadEvent;
	BOOL	m_fStopping;

	LONG	m_dwCurWriteCount;

	DWORD m_dwCOMNum;
	TCHAR m_szDevice[COM_PORT_NAME_LENGTH];	//device full name, for example COM10 --> \\.\COM10
	TCHAR m_szCOM[COM_PORT_NAME_LENGTH]; //device short name, not including prefix string.
	LPCTSTR GetComport() { return m_szCOM; }
	int  GetCOMNum(LPCTSTR szDevice);

	bool m_isEmdl;
	LONG InternalOpen();
	LONG InternalClose();
	int  InternalResetCOM();
	long InternalWrite(const void* pData, size_t iLen, DWORD* pdwWritten = 0);

	HANDLE	m_hThread;
	LONG StartListener();
	LONG StopListener(DWORD dwTimeout = LISTENER_TIME_OUT);
	static DWORD WINAPI ThreadProc(LPVOID lpArg);
	DWORD ThreadProc();

	DWORD volatile m_dwBufStart, m_dwBufEnd;
	DWORD volatile m_dwBufSize;
	BYTE* m_pbRecvBuf;
	int AppendBuffer(const void *pvSrc, int nLen);
	int ReadBuffer(void *pvDest, int nLen);
};

#endif //SERIALIO_H
