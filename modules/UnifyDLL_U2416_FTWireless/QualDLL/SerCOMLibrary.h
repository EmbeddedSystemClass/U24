#if !defined(_SERCOMLIBRARY_H_)
#define _SERCOMLIBRARY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "stdafx.h"
#include "SerialIO.h "
#include "MemBuffer.h "
//#include "Logger.h"

#define DEFAULT_CMD_TIME_OUT 5000	//default time out
#define READ_BUFFER_SIZE     8192

#define RUN_SUCCESSFULLY	1
#define RUN_FAILED			(-1)
#define ERROR_MASK			0xF0000000

class CSerCOMLibrary : public CSerialIO
{
public:
	CSerCOMLibrary();
	~CSerCOMLibrary();
private:
	CRCChecker m_CRCChecker;
	HANDLE m_hNotifyEvent;		// Event for OnRev
	int volatile m_nRxLen;
	int volatile m_nReadCount;
	BYTE * m_pbBuffer;
public:
	int Open(LPCTSTR lpszComport, bool isEmdl);
	int Close();
	virtual void OnEvent();
	int TXRXDiag(CHAR* lpTxData, DWORD dwTxLen, CHAR* lpRxData, DWORD dwRxLen, UINT uiTimeout = DEFAULT_CMD_TIME_OUT);
	int TXRXDl(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout = DEFAULT_CMD_TIME_OUT);
	int TxRx(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout = DEFAULT_CMD_TIME_OUT);
	int TxRxEmdl(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout = DEFAULT_CMD_TIME_OUT);
	int TxRxSpeedUp(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, char *szIMAGEPATH, UINT uiTimeout);	

	enum SercomErrorCode
	{
		ERROR_CSERCOMLIBRARY_INVALID_PARAMETER				= ERROR_MASK | 0x00000301,	// Invalid parameter.
		ERROR_CSERCOMLIBRARY_COM_OPEN_FAIL					= ERROR_MASK | 0x00000302,	// Open COM port fail
		ERROR_CSERCOMLIBRARY_COM_SERVER_FAIL				= ERROR_MASK | 0x00000303,	// Init COM server fail.
		ERROR_CSERCOMLIBRARY_COM_SERVER_VERSION_OUTOFDATE	= ERROR_MASK | 0x00000304,	// COM server is out-of-date.
		ERROR_CSERCOMLIBRARY_DL_CHANGE_INTERFACE_FAIL		= ERROR_MASK | 0x00000305,	// Change interface failed
		ERROR_CSERCOMLIBRARY_NULL_INTERFACE					= ERROR_MASK | 0x00000306,	// NULL Interface
		ERROR_CSERCOMLIBRARY_COM_NOT_FOUND					= ERROR_MASK | 0x00000307,	// Can not find COM port
		ERROR_CSERCOMLIBRARY_CHANGE_OFFLINE_FAIL			= ERROR_MASK | 0x00000308,	// Change offline failed
		ERROR_CSERCOMLIBRARY_MODE_CHANGE_FAIL				= ERROR_MASK | 0x00000309,	// Mode change failed
		ERROR_CSERCOMLIBRARY_TX_FAIL						= ERROR_MASK | 0x00000310,	// Mode change failed
		ERROR_CSERCOMLIBRARY_RX_FAIL						= ERROR_MASK | 0x00000311,	// Mode change failed
		ERROR_CSERCOMLIBRARY_DS_CHANGE_INTERFACE_FAIL		= ERROR_MASK | 0x00000312,	// Mode change failed
	};
};

#endif // !defined(_SIOCOMLIBRARY_H_)
