#pragma once

#include <vector>
#include <string>
#include "..\CommonUtil\Buffer.h"

namespace QSYNC_LIB
{
	using namespace std;
	
class CSerialPort
{	
public:
	enum CSerialPortMode
	{
		CSPFreeMode,//Without check anything
		CSPATCommandMode,//Lastest response Must be "0x0d 0x0a O K 0x0d 0x0d"
		CSPDIAGCommandMode,//lastest response Must be "0x7e"
		CSPOBEXCommandMode//check length
	};
// Construction
public:

	CSerialPort(CSerialPortMode nMode = CSPATCommandMode)
		:m_nCSPMode(nMode)
		,m_hComm(NULL)
		,m_hThread(NULL)
		,m_ptBuffer(new CBuffer)
		,m_hExitThread(NULL)
	{
		m_opWrite.hEvent = NULL;
		m_opRead.hEvent = NULL;
	}
	CSerialPort(const TSTRING& portname,CSerialPortMode nMode = CSPATCommandMode)
		:m_nCSPMode(nMode)
		,m_portname(portname)
		,m_hComm(NULL)
		,m_hThread(NULL)
		,m_ptBuffer(new CBuffer)
	{
		m_opWrite.hEvent = NULL;
		m_opRead.hEvent = NULL;
		OpenPort(m_portname);
	}
	virtual ~CSerialPort(){ClosePort();}

public:

	const bool IsOBEXMode() const {return m_nCSPMode == CSPOBEXCommandMode;}

	const bool SwitchToOBEX(const bool bSwitchToOBEX)
	{	
		bool bRes = false;
		CBufferPtr ptNewBuffer = new CBuffer;
		
		if(bSwitchToOBEX)
		{				
			if(WriteAndRead("AT+CGMI\r",ptNewBuffer))
			{
				bRes = WriteAndRead("AT^SQWE=3\r",ptNewBuffer);
				if(bRes) 
				{
					Sleep(1000);
					m_nCSPMode = CSPOBEXCommandMode;
				}
			}
		}
		else
		{
			BYTE szBuf[3] = {0x2B,0x2B,0x2B};
			WriteAndRead(szBuf,3,ptNewBuffer,1000);
			Sleep(1000);
			m_nCSPMode = CSPATCommandMode;
			BYTE szEndBuf[4] = {'a','t','z',0x0d};
			while(WriteAndRead(szEndBuf,4,ptNewBuffer,500) == false);
			bRes = WriteAndRead("AT+CGMI\r",ptNewBuffer,1000);
			assert(bRes);//switch to AT common mode fail
		}
		return bRes;
	}

	const bool WriteAndRead(const string& strBuffer,CBufferPtr& ptNewBuffer, DWORD dwDelay = 0)
	{
		return WriteAndRead((const BYTE*)strBuffer.c_str(),static_cast<long>(strBuffer.size()),ptNewBuffer,dwDelay);
	}

	const bool WriteAndRead(const BYTE* pByteBuffer,const DWORD nSize,CBufferPtr& ptNewBuffer, DWORD dwDelay = 0)
	{
		bool bRes = false;
		BYTE *pbyteBuffer = NULL;
		DWORD nTempSize = 0;
		if(WriteAndRead(pByteBuffer,nSize,&pbyteBuffer,nTempSize,dwDelay))
		{
			ptNewBuffer = new CBuffer(nTempSize,(char*)pbyteBuffer);
			delete [] pbyteBuffer;
			bRes = true;
		}
		return bRes;
	}

	const bool ConfigurePort(DWORD BaudRate, BYTE ByteSize, DWORD fParity, BYTE Parity, BYTE StopBits)
	{
		bool bRes = false;
		if(NULL != m_hComm)
		{
			if((m_bPortReady = GetCommState(m_hComm, &m_dcb))==0)
			{
				CloseHandle(m_hComm);
			}
			else
			{
				m_dcb.BaudRate =BaudRate;
				m_dcb.ByteSize = ByteSize;
				m_dcb.Parity =Parity ;
				m_dcb.StopBits =StopBits;
				m_dcb.fBinary=TRUE;
				m_dcb.fDsrSensitivity=false;
				m_dcb.fParity=fParity;
				m_dcb.fOutX=false;
				m_dcb.fInX=false;
				m_dcb.fNull=false;
				m_dcb.fAbortOnError=TRUE;
				m_dcb.fOutxCtsFlow=FALSE;
				m_dcb.fOutxDsrFlow=false;
				m_dcb.fDtrControl=DTR_CONTROL_DISABLE;
				m_dcb.fDsrSensitivity=false;
				m_dcb.fRtsControl=RTS_CONTROL_DISABLE;
				m_dcb.fOutxCtsFlow=false;
				m_dcb.fOutxCtsFlow=false;

				m_bPortReady = SetCommState(m_hComm, &m_dcb);
				if(m_bPortReady ==0)
					CloseHandle(m_hComm);
				else 
					bRes = true;
			}
		}
		return bRes;
	}

	const bool SetCommunicationTimeouts(DWORD ReadIntervalTimeout, DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant, DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant)
	{
		bool bRes = false;
		if(NULL != m_hComm)
		{
			if((m_bPortReady = GetCommTimeouts (m_hComm, &m_CommTimeouts))!=0)
			{
				m_CommTimeouts.ReadIntervalTimeout =ReadIntervalTimeout;
				m_CommTimeouts.ReadTotalTimeoutConstant =ReadTotalTimeoutConstant;
				m_CommTimeouts.ReadTotalTimeoutMultiplier =ReadTotalTimeoutMultiplier;
				m_CommTimeouts.WriteTotalTimeoutConstant = WriteTotalTimeoutConstant;
				m_CommTimeouts.WriteTotalTimeoutMultiplier =WriteTotalTimeoutMultiplier;
				m_bPortReady = SetCommTimeouts (m_hComm, &m_CommTimeouts);
				if(m_bPortReady == 0)
					CloseHandle(m_hComm);
				else
					bRes = true;
			}
		}
		return bRes;
	}

	const bool OpenUartPort(const TSTRING& portname,CSerialPortMode nMode = CSPATCommandMode)
	{
		m_nCSPMode = nMode;
		DCB					tempDCB;

		if(NULL == m_hComm)
		{
			TSTRING strPort;
			strPort= _T("\\\\.\\") +portname;
			m_hComm = CreateFile(strPort.c_str(),
				GENERIC_READ | GENERIC_WRITE,
				0,
				0,
				OPEN_EXISTING,
				FILE_FLAG_OVERLAPPED,
				0);
			assert(m_hComm != NULL);
			if (m_hComm == INVALID_HANDLE_VALUE)
			{
				return false;
			}


			if ( !::SetCommMask(m_hComm, EV_CTS | EV_DSR | EV_RLSD | EV_BREAK) ){
				ClosePort();
				return false;
			}

			// Set Input/Ouput Queue Size
			if (!::SetupComm(m_hComm, 4096, 4096)){
				ClosePort();
				return false;
			}

			// Purge COM port buffer
			if(!::PurgeComm(m_hComm, PURGE_TXCLEAR |
				PURGE_RXCLEAR |
				PURGE_TXABORT |
				PURGE_RXABORT) )
			{
				ClosePort();
				return false;
			}

			if (!::GetCommState(m_hComm,&tempDCB))
			{
				ClosePort();
				return false;
			}

			tempDCB.DCBlength = sizeof(DCB);
			tempDCB.BaudRate = 115200;
			tempDCB.fParity = FALSE;
			tempDCB.fOutxDsrFlow = FALSE;
			tempDCB.fDtrControl = DTR_CONTROL_ENABLE;
			tempDCB.fRtsControl = RTS_CONTROL_ENABLE;
			tempDCB.fDsrSensitivity = FALSE;
			tempDCB.ByteSize = 8;
			tempDCB.Parity = NOPARITY;
			tempDCB.StopBits = ONESTOPBIT;
			tempDCB.XoffChar = 0x13;
			tempDCB.XonChar = 0x11;

			tempDCB.fTXContinueOnXoff = FALSE;
			tempDCB.fInX = FALSE;
			tempDCB.fOutX = FALSE;

			tempDCB.fOutxCtsFlow = FALSE;


			if (!::SetCommState(m_hComm,&tempDCB))
			{
				ClosePort();
				return false;
			}

			BOOL bRes =::SetCommMask(m_hComm, EV_RXCHAR);
			::EscapeCommFunction (m_hComm, SETDTR|SETRTS);

			Sleep(100);
			::PurgeComm( m_hComm, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR );	


			if(bRes)
			{
				memset(&m_opRead,0,sizeof(OVERLAPPED));
				memset(&m_opWrite,0,sizeof(OVERLAPPED));
				m_opRead.hEvent	= ::CreateEvent(NULL,TRUE,FALSE,NULL);	
				m_opWrite.hEvent= ::CreateEvent(NULL,TRUE,FALSE,NULL);

				m_hExitThread = CreateEvent(NULL,TRUE,FALSE,_TC("SerialThreadExitEvent"));
				ResetEvent(m_hExitThread);
				m_ReadToken = CreateEvent(NULL,TRUE,FALSE,_TC("SerialPortRead"));
				ResetEvent(m_ReadToken);
				m_WriteToken = CreateEvent(NULL,TRUE,TRUE,_TC("SerialPortWrite"));
				SetEvent(m_WriteToken);

				DWORD dwThreadID = 0;
				m_hThread = CreateThread(NULL,0,ThreadProcA,this,0,&dwThreadID);
			}
			else
			{
				CloseHandle(m_hComm);
				m_hComm = NULL;
			}
		}
		return m_hComm != NULL;
	}
	const bool OpenPort(const TSTRING& portname,CSerialPortMode nMode = CSPATCommandMode)
	{
		m_nCSPMode = nMode;

		if(NULL == m_hComm)
		{
			TSTRING strPort;
			strPort= _T("\\\\.\\") +portname;
			m_hComm = CreateFile(strPort.c_str(),
								  GENERIC_READ | GENERIC_WRITE,
								  0,
								  0,
								  OPEN_EXISTING,
								  FILE_FLAG_OVERLAPPED,
								  0);
			assert(m_hComm != NULL);

			BOOL bRes = ::SetCommMask(m_hComm, EV_RXCHAR);


			if(bRes)
			{
				memset(&m_opRead,0,sizeof(OVERLAPPED));
				memset(&m_opWrite,0,sizeof(OVERLAPPED));
				m_opRead.hEvent	= ::CreateEvent(NULL,TRUE,FALSE,NULL);	
				m_opWrite.hEvent= ::CreateEvent(NULL,TRUE,FALSE,NULL);

				m_hExitThread = CreateEvent(NULL,TRUE,FALSE,_TC("SerialThreadExitEvent"));
				ResetEvent(m_hExitThread);
				m_ReadToken = CreateEvent(NULL,TRUE,FALSE,_TC("SerialPortRead"));
				ResetEvent(m_ReadToken);
				m_WriteToken = CreateEvent(NULL,TRUE,TRUE,_TC("SerialPortWrite"));
				SetEvent(m_WriteToken);

				DWORD dwThreadID = 0;
				m_hThread = CreateThread(NULL,0,ThreadProcA,this,0,&dwThreadID);
			}
			else
			{
				CloseHandle(m_hComm);
				m_hComm = NULL;
			}
		}
		return m_hComm != NULL;
	}

	void ClosePort()
	{
		if(m_hComm)
		{
			::SetCommMask(m_hComm, 0);
			::PurgeComm(m_hComm, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
			SetEvent(m_hExitThread);
			WaitForSingleObject(m_hThread,INFINITE);
			if(m_hThread)
				TerminateProcess(m_hThread,0);
			if(m_hExitThread)
			{
				CloseHandle(m_hExitThread);
				m_hExitThread = NULL;
			}
			if(m_opRead.hEvent)
			{
				CloseHandle(m_opRead.hEvent);
				m_opRead.hEvent = NULL;
			}
			if(m_opWrite.hEvent)
			{
				CloseHandle(m_opWrite.hEvent);
				m_opWrite.hEvent = NULL;
			}
			if(m_hComm)
			{
				CloseHandle(m_hComm);
				m_hComm = NULL;
			}
			m_hComm = NULL;
		}
	}

private:

	static VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode,DWORD dwNumberOfBytesTransfered,LPOVERLAPPED lpOverlapped)
	{
	}	
	static DWORD WINAPI ThreadProcA(LPVOID lpParameter)
	{
		CSerialPort* pThis = (CSerialPort*)lpParameter;
		HANDLE hEvents[2];
		hEvents[0] = pThis->m_hExitThread;
		hEvents[1] = pThis->m_ReadToken;		

		#define READ_TIMEOUT      500      // milliseconds
		DWORD nReadSize = 0;
		int nTimeOutTimes = 0;
		while(1)
		{	
			DWORD dResWait = WaitForMultipleObjects(2,hEvents,FALSE,INFINITE);
			if(dResWait == WAIT_OBJECT_0)
			{
				SetEvent(pThis->m_WriteToken);
				return 0;
			}
			ResetEvent(pThis->m_ReadToken);
			
			///////////////////////////////////////////////////////////////////////////////////////
			DWORD			dwCommEvent;
			DWORD			dwErr;
			COMSTAT			comStat;
			if(!::WaitCommEvent(pThis->m_hComm, &dwCommEvent, &pThis->m_opRead))
			{
				DWORD dwError = GetLastError();
				if(dwError != ERROR_IO_PENDING)
				{
					ClearCommError(pThis->m_hComm, &dwErr, &comStat);
					SetCommMask(pThis->m_hComm, EV_RXCHAR);//ClearCommError will reset the mask
					//assert(0);
					return 0;
				}
				else
				{
					DWORD dwRes = WaitForSingleObject(pThis->m_opRead.hEvent, READ_TIMEOUT);
					if(dwRes != WAIT_OBJECT_0)
					{
						ClearCommError(pThis->m_hComm, &dwErr, &comStat);
						SetCommMask(pThis->m_hComm, EV_RXCHAR);//ClearCommError will reset the mask
						SetEvent(pThis->m_ReadToken);
						continue;
					}
				}
			}
			else
			{
				if((dwCommEvent & EV_RXCHAR) != EV_RXCHAR)
				{
					SetEvent(pThis->m_ReadToken);
					continue;
				}
			}
	 
			ClearCommError(pThis->m_hComm, &dwErr, &comStat);
			SetCommMask(pThis->m_hComm, EV_RXCHAR);//ClearCommError will reset the mask

			if(nTimeOutTimes > 10)
			{
				nTimeOutTimes = 0;
			}
			else
			{
				if(comStat.cbInQue <= 0)
				{
					Sleep(200);
					nTimeOutTimes++;
					SetEvent(pThis->m_ReadToken);
					continue;
				}
				nTimeOutTimes = 0;
				//prepare buffer by comStat.cbInQue
				char* szTemp = new char[comStat.cbInQue];
				CBufferPtr ptTempBuffer = new CBuffer(comStat.cbInQue, szTemp);
				delete [] szTemp;
				ReadFile(pThis->m_hComm, (BYTE*)ptTempBuffer->GetData(), comStat.cbInQue, &nReadSize, &pThis->m_opRead);
				//assert(comStat.cbInQue == nReadSize);
				pThis->m_ptBuffer->Append(ptTempBuffer);			
				switch(pThis->m_nCSPMode)
				{
				case CSPFreeMode:
					{
						SetEvent(pThis->m_WriteToken);
						SetEvent(pThis->m_ReadToken);
						continue;
					}
					break;
				case CSPATCommandMode:					
					if(!IsCompleteATPackage(pThis->m_ptBuffer))
					{
						SetEvent(pThis->m_WriteToken);
						SetEvent(pThis->m_ReadToken);
						continue;
					}
					break;
				case CSPDIAGCommandMode:
					if(!IsCompleteDIAGPackage(pThis->m_ptBuffer))
					{
						SetEvent(pThis->m_WriteToken);
						SetEvent(pThis->m_ReadToken);
						continue;
					}
					break;
				case CSPOBEXCommandMode:
					if(!pThis->IsCompleteOBEXPackage(pThis->m_ptBuffer->GetLength(),(BYTE*)pThis->m_ptBuffer->GetData()))
					{
						SetEvent(pThis->m_WriteToken);
						SetEvent(pThis->m_ReadToken);
						continue;
					}
					break;
				default:
					break;
				};
			}
			SetEvent(pThis->m_WriteToken);
		};

		return 0;
	}

	const bool WriteAndRead(const BYTE* pByteBuffer,const DWORD nSize,BYTE **ppbyteBuffer,DWORD& nRetSize, DWORD dwDelay = 0)
	{		
		nRetSize = 0;
		*ppbyteBuffer = NULL;

		WaitForSingleObject(m_WriteToken,INFINITE);
		ResetEvent(m_WriteToken);

		bool bRes = false;
		if(NULL != m_hComm)
		{
			bRes = (TRUE == WriteFileEx(m_hComm,pByteBuffer,nSize,&m_opWrite,FileIOCompletionRoutine));
			if(bRes == false)
			{
				::WaitForSingleObject(m_opWrite.hEvent,INFINITE);
				DWORD dwBytesTransferred = 0;
				bRes = (TRUE == GetOverlappedResult(m_hComm, &m_opWrite, &dwBytesTransferred, TRUE));
			}
		}

		Sleep(500);
		if(bRes)
		{
			DWORD dwMilliseconds = (dwDelay > 0) ? dwDelay : INFINITE;
			SetEvent(m_ReadToken);
			if(WAIT_OBJECT_0 == WaitForSingleObject(m_WriteToken,dwMilliseconds))
			{
				ResetEvent(m_WriteToken);

				nRetSize = m_ptBuffer->GetLength();
				if(nRetSize > 0)
				{
					*ppbyteBuffer = new BYTE[nRetSize];
					memcpy(*ppbyteBuffer,m_ptBuffer->GetData(),nRetSize);
				}				
				m_ptBuffer = new CBuffer;//reset share buffer
			}
		}

		SetEvent(m_WriteToken);		
		assert((dwDelay == 0 && nRetSize > 0) || (dwDelay > 0));

		return nRetSize > 0;
	}
public:
	static const bool DecodeATPackage(const CBufferPtr& ptBuffer,CBufferPtr& ptReceiverBuffer)
	{
		bool bRes = false;
		if(IsCompleteATPackage(ptBuffer))
		{			
			BYTE* pTemp = (BYTE*)ptBuffer->GetData();
			string strBuffer;
			int nSteps = 0;
			int nTempCount=0;
			BYTE bytePrev = 0;
			for(ULONGLONG i = 0;i<(ptBuffer->GetLength()-6);i++)
			{
				nTempCount++;
				if(pTemp[i] == 0x0a && bytePrev == 0x0d)
				{
					if(nSteps == 1)
					{
						assert((i-nTempCount+1) >= 0 && (i-nTempCount+1) < ptBuffer->GetLength());
						if((i-nTempCount+1) >= 0 && (i-nTempCount+1) < ptBuffer->GetLength())
							ptReceiverBuffer = new CBuffer(nTempCount-2,(const char*)&(pTemp[i-nTempCount+1]));
						bRes = true;
						break;
					}
					nSteps++;
					nTempCount = 0;
				}				
				bytePrev = pTemp[i];
			}
		}
		return bRes;
	}

	static const bool IsCompleteDIAGPackage(const CBufferPtr& ptBuffer)
	{
		bool bRes = false;
		const int nSize = 1;
		BYTE szEnd[nSize] = {0x7E};
		if(ptBuffer->GetLength() > nSize)
		{
			BYTE* pTemp = (BYTE*)ptBuffer->GetData();
			BYTE* pEnd = pTemp + (ptBuffer->GetLength()-nSize);
			bRes = memcmp(pEnd,szEnd,nSize) == 0;

			//Filter garbage message
			if(bRes)
			{
				BYTE* pCommand = (BYTE*)ptBuffer->GetData();
				bRes = pCommand[0] != 0x79;
			}
		}
		return bRes;
	}

	static const bool IsCompleteATPackage(const CBufferPtr& ptBuffer)
	{
		bool bRes = false;
		const int nSize = 6;
		BYTE szEnd[nSize] = {0x0d,0x0a,'O','K',0x0d,0x0a};
		if(ptBuffer->GetLength() >= nSize)
		{
			BYTE* pTemp = (BYTE*)ptBuffer->GetData();
			BYTE* pEnd = pTemp + (ptBuffer->GetLength()-nSize);
			bRes = memcmp(pEnd,szEnd,nSize) == 0;
		}
		return bRes;
	}

	static const bool IsCompleteOBEXPackage( DWORD dwDataSize,BYTE* pData )
	{
		bool bRes = false;
		if( dwDataSize > 3 )
		{
			DWORD dwPackageSize = MAKEWORD(pData[2],pData[1]);
			bRes = dwDataSize == dwPackageSize;
		}
		return bRes;
	}
private:
	HANDLE					m_hComm;
	DCB						m_dcb;
	COMMTIMEOUTS			m_CommTimeouts;
	BOOL					m_bPortReady;	
	OVERLAPPED				m_opRead;
	OVERLAPPED				m_opWrite;
	TSTRING					m_portname;
	CSerialPortMode m_nCSPMode;
	CBufferPtr				m_ptBuffer;
	HANDLE					m_ReadToken;
	HANDLE					m_WriteToken;
	HANDLE					m_hThread;
	HANDLE					m_hExitThread;
};

};