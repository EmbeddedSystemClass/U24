// CSerCOMLibrary : Serial COM port library. Just for end user download.
// 2005/07/19 Embert Tsai.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerCOMLibrary.h"
#include "SerialIO.h"
#include "BQPacketLib.h"
#include <string>
#include "..\Common\CommonUtil\LogPolicy.h"

#define CONNECT_TRY_TIMES				6
#define	CONNECT_TRY_FAIL_DELAY_TIME		1000
#define READ_BUFFER_SIZE				8192
#define PHONE_MODE_DETECTION_TIMES		120
#define MODE_DETECTION_RETY_TIMES		40
#define MODE_DETECTION_DELAY			500
#define DELAY_FOR_COM_START				60000
#define DEFAULT_TIME_OUT				3000

CSerCOMLibrary::CSerCOMLibrary()
:m_bIsUsingRequest(false)
,m_bIsSetProbeRequestMode(false)
,m_nPortID(0)
{	
	m_hNotifyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// Create event for Rx notification.
	::ResetEvent(m_hNotifyEvent);
	m_hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// Create event for read completition.
	::ResetEvent(m_hReadEvent);
	_ASSERTE(m_hNotifyEvent !=NULL);
	_ASSERTE(m_hReadEvent != NULL);
	m_pbBuffer = new BYTE[READ_BUFFER_SIZE];
	ZeroMemory(m_pbBuffer, READ_BUFFER_SIZE);
	m_nReadCount = 0;
}

CSerCOMLibrary::~CSerCOMLibrary()
{	
	CloseHandle(m_hNotifyEvent);
	m_hNotifyEvent = NULL;
	CloseHandle(m_hReadEvent);
	m_hReadEvent = NULL;
	CSerialIO::Close();
	delete[] m_pbBuffer;
	m_pbBuffer = NULL;
	
}

int CSerCOMLibrary::GetComPort()
{
	return m_nPortID;
}

void CSerCOMLibrary::AddLog(LPCTSTR szMsg)
{
#ifdef DETIAL_LOG
	LogPolicy::Log(QSYNC_LIB::GetAnsi(szMsg).c_str(),m_strLogFile.c_str());
#endif//#ifndef DISABLE_LOG
}

int CSerCOMLibrary::OpenComport(LPCTSTR lpszModelName, LPCTSTR lpszDevice,bool bRequestFunction)
{
	int nRes = ERROR_CSERCOMLIBRARY_INVALID_PARAMETER;
	if ( lpszModelName == NULL || lpszDevice == NULL )
	{
		nRes = ERROR_CSERCOMLIBRARY_INVALID_PARAMETER;
	}
	else
	{
		//assume the device is COM port device
		CString strCOMNum;
		strCOMNum = lpszDevice;
		strCOMNum.MakeUpper();
		//int nIndex = strCOMNum.ReverseFind('COM');
		int nIndex = strCOMNum.Find ("COM");
		if(nIndex >= 0 )
		{
			strCOMNum = strCOMNum.Mid(nIndex+1);
			m_nPortID = _ttoi(strCOMNum);
			CString strFormat;
			strFormat.Format(_T("Test_COM%02d.txt"),GetComPort());
			m_strLogFile = (LPCTSTR)strFormat;
		}

		if ( CSerialIO::Open(lpszDevice,EBaud115200,bRequestFunction) != ERROR_SUCCESS )
		{
			nRes = CSerCOMLibrary::ERROR_CSERCOMLIBRARY_COM_OPEN_FAIL;
		}
		else
		{
			nRes = RUN_SUCCESSFULLY;
		}
	}
	return nRes;
}

void CSerCOMLibrary::CloseComport()
{
	CSerialIO::Close();
}
bool CSerCOMLibrary::ResetMode()
{
	bool bRes = true;
	m_eState = EStart;
	bRes = ResetEvent(m_hModeEvent) == TRUE;
	return bRes;
}

void CSerCOMLibrary::OnEvent(EEvent eEvent)
{
	//AddLog(_T("CSerCOMLibrary::OnEvent(EEvent eEvent)"));

	CHAR	chStart = PACKET_NULL_CHAR;
	CHAR	chEnd	= PACKET_NULL_CHAR;
	int		nRet	= 0;
	DWORD	dwRead	= 0;
	EState	eState	= EStart;
	ResetEvent(m_hModeEvent);

	ZeroMemory(m_pbBuffer, sizeof(m_pbBuffer));

	if ( CSerialIO::EOnRecv == eEvent){
		if( Read(m_pbBuffer, DIAG_MAX_RESPONSE_LEN, &dwRead) != ERROR_SUCCESS ||	//+ Read Failed
			dwRead == 0)										//+ No more received
		{
			AddLog(_T("READ_BUFFER_SIZE"));
			_ASSERTE( dwRead <= READ_BUFFER_SIZE);
			return;
		}
		
		if(m_bIsUsingRequest)
		{
			//AddLog(_T("m_bIsUsingRequest Mode"));

			CBufferPtr ptReceiver = new CBuffer(dwRead, (const char*)m_pbBuffer);
			push(false,ptReceiver);
			if(m_ptRequest->IsCompletePacket(ptReceiver))
			{	
				if(m_ptRequest->WaitForOtherResponse() == false)
					ResetRequestMode();
				m_ptRequest->OnDataReceiver(ptReceiver);			
			}
			else
			{
				AddLog(_T("m_ptRequest->IsCompletePacket(ptReceiver) fail"));
			}
		}
		else if(m_bIsSetProbeRequestMode)
		{
			AddLog(_T("m_bIsSetProbeRequestMode Mode"));

			CBufferPtr ptReceiver = new CBuffer(dwRead, (const char*)m_pbBuffer);			
			push(false,ptReceiver);
			if(m_ptRequest->IsCompletePacket(ptReceiver))
			{				
				m_ptRequest->OnDataReceiver(ptReceiver);				
				m_bIsSetProbeRequestMode = false;
			}
			else
			{
				AddLog(_T("m_ptRequest->IsCompletePacket(ptReceiver) fail"));
			}
		}
		else if(m_bIsSetProbeMode)
		{
			AddLog(_T("m_bIsSetProbeMode Mode"));
			m_bIsSetProbeMode = false;
		}
		else
		{
		}
	}

}

BOOL CSerCOMLibrary::IsOutBufEmpty(){
	return (m_nRxLen >0 );
}

void CSerCOMLibrary::ResetReadParameter(){
	
}

void CSerCOMLibrary::ResetRequestMode()
{
	//AddLog(_T("m_bIsUsingRequest = false;"));
	m_bIsUsingRequest = false;
}

bool CSerCOMLibrary::ExecuateProbe(PLATFORM_TOOL::CRequestPtr& ptRequest)
{
	AddLog(_T("CSerCOMLibrary::ExecuateProbe ()"));

	bool bRes = false;

	m_bIsSetProbeRequestMode = true;

	m_ptRequest = ptRequest;

	CBufferPtr ptBufferSend;
	ptRequest->GetSendData(ptBufferSend);
	push(true,ptBufferSend);

	DWORD dwWritten = 0;
	long lRet = Write(const_cast<CHAR *>(ptBufferSend->GetData()),ptBufferSend->GetLength(),&dwWritten);
	bRes = lRet == 0;
	if(bRes == false)
	{
		AddLog(_T("CSerCOMLibrary::ExecuateProbe ==>Write fail"));
		ResetRequestMode();
	}

	return bRes;
}

bool CSerCOMLibrary::ExecuateRequest(PLATFORM_TOOL::CRequestPtr& ptRequest)
{
	AddLog(_T("CSerCOMLibrary::ExecuateRequest ()"));

	bool bRes = false;

	if(m_bIsUsingRequest == false)
	{
		AddLog(_T("m_bIsUsingRequest = true;"));
		m_bIsUsingRequest = true;
		m_ptRequest = ptRequest;

		CBufferPtr ptBufferSend;
		ptRequest->GetSendData(ptBufferSend);
		push(true,ptBufferSend);

		DWORD dwWritten = 0;
		long lRet = Write(const_cast<CHAR *>(ptBufferSend->GetData()),ptBufferSend->GetLength(),&dwWritten);
		bRes = lRet == 0;
		if(bRes == false)
		{
			AddLog(_T("CSerCOMLibrary::ExecuateRequest ==>Write fail"));
			ResetRequestMode();
		}

		/*
		DWORD dwReturn;
		if(WaitForWrite(ptRequest->GetTimeout(),dwReturn))
		{
			int nBufferLen = ptRequest->GetReceiveBufferSize();
			CBufferPtr ptReceiver = new CBuffer(nBufferLen, new char[nBufferLen]);
			DWORD nReceiver = ptReceiver->GetLength();
			if(GetReceiverData(const_cast<CHAR *>(ptReceiver->GetData()),nReceiver))
			{
				ptReceiver->ReleaseBuffer(nReceiver);
				ptRequest->OnDataReceiver(ptReceiver);
				bRes = true;
			}
		}
		*/
	}
	else
	{
		AddLog(_T(" CSerCOMLibrary::ExecuateRequest() fail"));
	}

	return bRes;
}

void CSerCOMLibrary::push(bool bIsSendRequest,CBufferPtr& ptBufferSend)
{
	m_RequestLogQueue.push(new CRequestLog(bIsSendRequest,ptBufferSend));
	while(m_RequestLogQueue.size() > LOG_MAXIMUM)
		m_RequestLogQueue.pop();
}

void CSerCOMLibrary::pop()
{
	while(m_RequestLogQueue.empty() == false)
	{
		CRequestLogPtr LogPtr = m_RequestLogQueue.front();
		
		SYSTEMTIME LocalTime;
		SystemTimeToTzSpecificLocalTime(NULL,&LogPtr->m_UTCTime,&LocalTime);

		CString strTemp;
		if(LogPtr->m_bIsSendRequest)
			strTemp.Format(_T("Send %04d\\%02d\\%02d %02d:%02d:%02d\r\n"),LocalTime.wYear,LocalTime.wMonth,LocalTime.wDay,LocalTime.wHour,LocalTime.wMinute,LocalTime.wSecond);
		else
			strTemp.Format(_T("Read %04d\\%02d\\%02d %02d:%02d:%02d\r\n"),LocalTime.wYear,LocalTime.wMonth,LocalTime.wDay,LocalTime.wHour,LocalTime.wMinute,LocalTime.wSecond);
		
		LogBinaryToFile(strTemp,LogPtr->m_ptRequest->GetData(),LogPtr->m_ptRequest->GetLength());		
		m_RequestLogQueue.pop();
	}
}

void CSerCOMLibrary::LogBinaryToFile(const char* szDescriptiion,const char* szTemp,DWORD nBufferSize)
{
	const int nLenMax = 32;
	char szCharTemp[nLenMax+1] = {0};
	int nLineCount = 0;

	CStringA strLog;

	for(DWORD i=0;i<nBufferSize;i++)
	{		
		if(i > 0 && i % nLenMax == 0)
		{
			strLog += "   ";
			for(int j=0;j<nLenMax;j++)
			{
				CStringA strTemp;
				strTemp.Format("%c",szCharTemp[j]);
				strLog += strTemp;
			}
			strLog += "\n";
			nLineCount = 0;
		}
		BYTE nTest = szTemp[i];
		CStringA strTemp;
		strTemp.Format("%02X ",nTest);
		strLog += strTemp;
		szCharTemp[i%nLenMax] = (nTest <= 0x1F || nTest >= 0xFF) ? '.' : szTemp[i];
		nLineCount++;
	}

	for(int nSpaceCount = 0;nSpaceCount < (nLenMax - nLineCount);nSpaceCount++)	
		strLog += "   ";

	strLog += "   ";
	for(int j=0;j<nLineCount;j++)
	{		
		CStringA strTemp;
		strTemp.Format("%c",szCharTemp[j]);
		strLog += strTemp;
	}

	strLog = szDescriptiion + strLog;
	LogPolicy::Log(QSYNC_LIB::GetAnsi((LPCTSTR)strLog).c_str(),m_strLogFile.c_str());
}

void CSerCOMLibrary::PopLastLog()
{
	pop();
}