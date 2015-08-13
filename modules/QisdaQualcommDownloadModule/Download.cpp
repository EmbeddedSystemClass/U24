#include "StdAfx.h"
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include "..\Common\CommonUtil\unicodeutility.h"
#include "..\Common\CommonUtil\PIMStringUtility.h"
#include "..\DownloadDLL\HexFile.h"
//#include "..\Components\MobileFinder\MobileFinderInterface.h"
#include "Download.h"
#include<iostream>
#include <fstream>
using namespace std;

CString GetDownloadType(BYTE nType)
{
	//Image_type_PRIBL		= 0x01,	/* Primary boot loader */
	//	Image_type_QCSECBLHD	= 0x02,	/* Qc Secondary boot loader header and config data */
	//	Image_type_QCSECBL		= 0x03,	/* Qc secondary boot loader */
	//	Image_type_OEMSECBL		= 0x04,	/* Oem secondary boot loader and need Oem header */
	//	Image_type_AMSSMODEL	= 0x05,	/* AMSS modem executable and need AMSS modem header */
	//	Image_type_AMSSAPP		= 0x06,	/* AMSS applications executable and need AMSS applications header */
	//	Image_type_6250OTPBL	= 0x07, /* MSM6250 OTP boot loader */
	//	Image_type_FOTAUI		= 0x08,	/* FOTA UI binary */ 
	//	Image_type_CEFS			= 0x09,	/* Compact EFS2 image */
	//	Image_type_AMSSAPPBL	= 0x0A,	/* AMSS applications boot loader and need AMSS applications boot loader header */
	//	Image_type_Flash		= 0x0C, /* Flash.Bin */
	switch(nType)
	{
	case Image_type_PRIBL:
		return _T("Image_type_PRIBL");
		break;
	case Image_type_QCSECBLHD:
		return _T("QCSECBLHD");
		break;
	case Image_type_QCSECBL:
		return _T("QCSECBL");
		break;
	case Image_type_OEMSECBL:
		return _T("OEMSECBL");
		break;
	case Image_type_AMSSMODEL:
		return _T("AMSSMODEL");
		break;
	case Image_type_AMSSAPP:
		return _T("AMSSAPP");
		break;
	case Image_type_6250OTPBL:
		return _T("6250OTPBL");
		break;
	case Image_type_FOTAUI:
		return _T("FOTAUI");
		break;
	case Image_type_CEFS:
		return _T("CEFS");
		break;
	case Image_type_AMSSAPPBL:
		return _T("AMSSAPPBL");
		break;
	case Image_type_Flash:
		return _T("Flash");
		break;
	default:
		return _T("Unknown");
		break;
	}

	return _T("Unknown");
}

CDLCommand::CDLCommand(const TSTRING& strDevice, const std::string& strPlatform)
:CInterfaceBase(strDevice, strPlatform)
,m_nProgress(0)
,CLog(strDevice)
,CLastError(strDevice)
{
	DefineNotify(EVENT_DL_PROGRESS);
    DefineNotify(EVENT_DL_PARTITION_DIF);
}

CDLCommand::CDLCommand(int nCOMPort, const std::string& strPlatform)
:CInterfaceBase(COMDeviceName(nCOMPort), strPlatform)
,m_nProgress(0)
,CLog(COMDeviceName(nCOMPort))
,CLastError(COMDeviceName(nCOMPort))
{
	DefineNotify(EVENT_DL_PROGRESS);
    DefineNotify(EVENT_DL_PARTITION_DIF);
}

CDLCommand::~CDLCommand(void)
{}

//************************************
// Method:    SetLastError
// Returns:   void
// Parameter: IN DL_ERROR nError
// Parameter: IN bool bExecuteResult: if execution fails, we need to set error code.
//                                    (bExecuteResult==false,then set ErrorCode) 
//************************************
void CDLCommand::SetLastError(DL_ERROR nError, bool bExecuteResult)
{
	if(!bExecuteResult)
	{
		SetCFCLastError(nError);
	}
}


bool CDLCommand::Register(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Register(pNotify, strEvent);
}
bool CDLCommand::Register(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Register(pEventFunc, strEvent);
}
bool CDLCommand::Unregister(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Unregister(pNotify, strEvent);
}
bool CDLCommand::Unregister(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Unregister(pEventFunc, strEvent);
}
void CDLCommand::Fire(const std::string& strEvent,long nParam)
{
	return QSYNC_LIB::CNotifyBase_ANSI::Fire(strEvent, nParam);
}
void CDLCommand::DefineNotify(const std::string& strNewEvent)
{
	return QSYNC_LIB::CNotifyBase_ANSI::DefineNotify(strNewEvent);
}


bool ByteToChar(BYTE *pucHexBcdBuf, int iHexBcdBufSize, CStringA& strBuffer)
{
	bool bRes = false;
	if (pucHexBcdBuf && (iHexBcdBufSize > 0))
	{
		for(int i=0; i< iHexBcdBufSize; i++)
		{
			CStringA strTemp;
			strTemp.Format("%02X ",pucHexBcdBuf[i]);
			strBuffer+=strTemp;
			if(i!=0 && (i+1)%18 ==0 )
				strBuffer += "\r\n";
		}
		bRes = true;
	}
	return bRes;
}

//************************************
// Method:    AddMsg
// Parameter: IN bool bResult : Command execution result
// Parameter: IN const char * szTitle : Command title
// Parameter: IN int nStep : Which step of download
//************************************
void CDLCommand::AddMsg(bool bResult, const char* szTitle, int nStep)
{
	std::string strMsg = szTitle;
	int nProgress = 0;
	strMsg += bResult ? " success" : " fail";
	nProgress = bResult ? 100 : m_nProgress;
	AddMsg(strMsg.c_str(), nStep, nProgress);

	//Add DL Error Log
#ifndef DISABLE_DL_LOG
	if(!bResult && nStep!=0)
	{
		CString cstrProgress;
		cstrProgress.Format("  %d%%", m_nProgress);
#ifndef _UNICODE
		cstrProgress = CString(szTitle) + cstrProgress;
		AddDLErrorLog((std::string)cstrProgress);

		// Print out fail packet
		CBufferPtr ptData = new CBuffer;
		CStringA cstraData;
		if (GetSendData(ptData) == false)
		{
			assert("Get sent data fail." && 0);
			AddDLErrorLog("Get sent data fail, please contact download programmer.");
		}
		else if (ByteToChar((BYTE*)ptData->GetData(), ptData->GetLength(), cstraData) == false)
		{
			assert("Translate sent data from byte to char fail." && 0);
			AddDLErrorLog("Translate received data from byte to char fail."); 
		}
		else
		{  // print SendData
			AddDLErrorLog("SendData:");
			AddDLErrorLog((std::string)cstraData);
		}
		ptData->SetEmpty();
		cstraData = _T("");

		if (GetReceiveData(ptData) == false)
			AddDLErrorLog("Get no received data."); 
		else if (ByteToChar((BYTE*)ptData->GetData(), ptData->GetLength(), cstraData) == false)
		{
			assert("Translate received data from byte to char fail." && 0);
			AddDLErrorLog("Translate received data from byte to char fail."); 
		}
		else
		{   // print ReceiveData
			AddDLErrorLog("ReceiveData:"); 
			AddDLErrorLog((std::string)cstraData);
		}

#else
		std::wstring wstrMsg = ansitowide(szTitle);
		cstrProgress = CString(wstrMsg.c_str()) + cstrProgress;
		AddDLErrorLog((std::wstring)cstrProgress);
#endif
	}
#endif
}
//************************************
// Method:    AddMsg
// Parameter: IN BYTE nType : notice which Image would be Open/Write/Close
// Parameter: IN int nStep : Which step of download
//************************************
void CDLCommand::AddMsg(BYTE nType, int nStep)
{
	std::string strMsg = "********** [Image] ";
	if(nType == OPEN_MULTI_MODE_QCSBLHDCFG)
		strMsg += "QCSBLHD";
	else if(nType == OPEN_MULTI_MODE_QCSBL)
		strMsg += "QCSBL";
	else if(nType == OPEN_MULTI_MODE_OEMSBL)
		strMsg += "OEMSBLHD/OEMSBL";
	else if(nType == OPEN_MULTI_MODE_AMSS)
		strMsg += "(AMSSHD)/AMSS";
	else if(nType == OPEN_MULTI_MODE_CEFS)
		strMsg += "CEFS";
	else if(nType == OPEN_MULTI_MODE_DBL)
		strMsg += "DBL";
	else if(nType == OPEN_MULTI_MODE_OSBL)
		strMsg += "OSBL";
	else if(nType == OPEN_MULTI_MODE_FSBL)
		strMsg += "FSBL";
	else if(nType == OPEN_MULTI_MODE_APPSBL)
		strMsg += "APPSBL";
	/* Alex Add Start 2011/03/08 */
	else if(nType == 33)
		strMsg += "msimage";   // OPEN_MULTI_MODE_EMMC_USER = 0x21  0x21=33
	/* Alex Add End 2011/03/08 */
	strMsg += " **********";
	AddMsg(strMsg.c_str(), None, 0);
}

//************************************
// Method:    AddMsg
// Parameter: IN const char * szMsg : message
// Parameter: IN int nStep : Which step of download
// Parameter: IN int nProgress : download nProgress% 
//************************************
void CDLCommand::AddMsg(const char* szMsg, int nStep, int nProgress)
{
    if(!(m_nProgress == nProgress && m_strMsg == szMsg))
	{
		m_nProgress = nProgress;
		m_strMsg = szMsg;
		DL_PROGRESS Msg = {(char*)szMsg, (DownloadStep)nStep, nProgress};
		Fire(EVENT_DL_PROGRESS, (long)&Msg);
		TRACE(szMsg);
	}
}

bool CDLCommand::DMSSReset(int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	CRequestPtr ptRequest = new CDMSSReset;
	CBufferPtr ptBuffer = new CBuffer;
	return Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
}

bool CDLCommand::DMSSNop(int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("DMSSNop", DMSSProtocol, 0);
	bool bNone = false, bDiag = false;
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		bNone = false; bDiag = false;
		CRequestPtr ptRequest = new CDMSSNop;
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		if(!bRes)
		{
			if(ptRequest->GetPotocolType() == NONE)
			{
				AddMsg(bRes, "Connection", Connection);
				bNone = true;
			}
			else if(ptRequest->GetPotocolType() == DIAG)
			{
				AddMsg(bRes, "Change Download Mode", ChangeDLMode);
				bDiag = true;
			}
		}
	}
	AddMsg(bRes, "DMSSNop", DMSSProtocol);
	AddDLLog(bRes, "Nop", _T("DownloadProcess"));
	
	if(bNone)
		SetLastError(DL_ERROR_TX_ERROR, bRes);
	else if(bDiag)
		SetLastError(DL_ERROR_ChangeDLMode, bRes);
	else
		SetLastError(DL_ERROR_DMSS_NOP, bRes);
	return bRes;
}
bool CDLCommand::DMSSParameter(int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("DMSSParameter", DMSSProtocol, 0);
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		CRequestPtr ptRequest = new CDMSSParameter;
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	}
	AddMsg(bRes, "DMSSParameter", DMSSProtocol);
	AddDLLog(bRes, "DMSSParameter", _T("DownloadProcess"));
	SetLastError(DL_ERROR_DMSS_PARAMETER, bRes);
	return bRes;
}
bool CDLCommand::DMSSWrite32Bit(BYTE* lpWData, WORD dwWLength, DWORD dwAddress, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(lpWData)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			CRequestPtr ptRequest = new CDMSSWrite32Bit(lpWData, dwWLength, dwAddress);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		}
	}
	return bRes;
}

bool CDLCommand::DMSSGo(WORD nwSegment,WORD nwoffset, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("DMSSGo", DMSSProtocol, 0);
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		CRequestPtr ptRequest = new CDMSSGo(nwSegment, nwoffset);
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	}
	AddMsg(bRes, "DMSSGo", DMSSProtocol);
	AddDLLog(bRes, "DMSSGo", _T("DownloadProcess"));
	SetLastError(DL_ERROR_DMSS_GO, bRes);
	return bRes;
}

bool CDLCommand::StreamingHello(bool bSpeedUp, int nRetryTimes, int nTimeout,DownloadProtocol nDownloadProtocol)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingHello", StreamingProtocol, 0);
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		int nVersion = 3;
		int nCVersion = 3;
		int nFeaturebit = 9;
		if(GetPlatorm() == "Qualcomm6280")
		{
			nVersion = 2;
			nCVersion = 2;
			nFeaturebit = 1;
		}
		CRequestPtr ptRequest = new CStreamingDLHello(nVersion,nCVersion,nFeaturebit,bSpeedUp,nDownloadProtocol);
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);

		::Sleep(200);
	}
	AddMsg(bRes, "StreamingHello", StreamingProtocol);
	AddDLLog(bRes, "StreamingHello", _T("DownloadProcess"));
	SetLastError(DL_ERROR_Streaming_Hello, bRes);
	return bRes;
}



bool CDLCommand::StreamingSecurity(const std::string& strPBL, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingSecurity", StreamingProtocol, 0);
	BYTE nSecurity = (strPBL == "") ? 0x01 : 0x00 ;
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		CRequestPtr ptRequest = new CStreamingDLSecurity(nSecurity, bSpeedUp);
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	}
	AddMsg(bRes, "StreamingSecurity", StreamingProtocol);
	AddDLLog(bRes, "StreamingSecurity", _T("DownloadProcess"));
	SetLastError(DL_ERROR_Streaming_Security, bRes);
	return bRes;
}





bool CDLCommand::StreamingPartition(BYTE* lpbyData, WORD wnLength/*max=512*/, bool bOverride, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	AddMsg("StreamingPartition", StreamingProtocol, 0);
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	
	bool bPartitionDiffer = false;
	if(lpbyData && wnLength < 513)
	{
		BYTE nRespStatus = 0;
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			nRespStatus = 0;
			CRequestPtr ptRequest = new CStreamingDLPartition(lpbyData, wnLength, bOverride, nRespStatus, bSpeedUp);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		}
		if(bRes)
		{
			bRes = nRespStatus == 0; /*0 = Partition table accepted*/ 
			bPartitionDiffer = nRespStatus == 0x01;
            if(bPartitionDiffer) {
				AddMsg("Partition Table is Different", StreamingProtocol, 0);
                Fire(EVENT_DL_PARTITION_DIF, 0);
            }
		}
	}
	AddMsg(bRes, "StreamingPartition", StreamingProtocol);
	AddDLLog(bRes, "StreamingPartition", _T("DownloadProcess"));
	//SetLastError(bPartitionDiffer ? DL_ERROR_DIFFERENT_PARTITION : DL_ERROR_Streaming_Partition, bRes);
	return bRes;
}

bool CDLCommand::StreamingOpenMultiImage(BYTE nType, BYTE* lpData, WORD wnLength/*max=512*/, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	if(nRetryTimes <= 0)
		nRetryTimes = 1;

	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg(nType, StreamingProtocol);
	if(wnLength < 513)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			CRequestPtr ptRequest = new CStreamingDLOpenMultiImage(nType, lpData, wnLength, bSpeedUp);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		}
	}
	AddMsg(bRes, "StreamingOpenMultiImage", StreamingProtocol);
	AddDLLog(bRes, GetDownloadType(nType) + _T(" open"), _T("DownloadProcess"));
	return bRes;
}

bool CDLCommand::StreamingOpenMultiImage7K(BYTE nType, BYTE* lpData, WORD wnLength/*max=512*/, CString Md5FromFile, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	if(nRetryTimes <= 0)
		nRetryTimes = 1;

	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg(nType, StreamingProtocol);
	if(wnLength < 513)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			CRequestPtr ptRequest;
			if (nType == Image_type_OEMSECBL)
			{
				ptRequest = new CStreamingDLOpenMultiImage7K(nType, lpData, wnLength, Md5FromFile, bSpeedUp);
			}
			else
			{
				ptRequest = new CStreamingDLOpenMultiImage(nType, lpData, wnLength, bSpeedUp);
			}
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		}
	}
	AddMsg(bRes, "StreamingOpenMultiImage", StreamingProtocol);
	AddDLLog(bRes, GetDownloadType(nType) + _T(" open"), _T("DownloadProcess"));
	return bRes;
}

bool CDLCommand::StreamingIRAMRead(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	if(nRetryTimes <= 0)
		nRetryTimes = 1;

	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingIRAMRead", StreamingProtocol, 0);
	if(lpData)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			CRequestPtr ptRequest = new CStreamingDLIRAMRead(wnLength, dwAddress, bSpeedUp);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
			if(bRes)
				memcpy(lpData, ptBuffer->GetData(), wnLength);
		}
	}
	AddMsg(bRes, "StreamingIRAMRead", StreamingProtocol);
	AddDLLog(bRes, "StreamingIRAMRead", _T("DownloadProcess"));

	return bRes;
}

bool CDLCommand::StreamingDLeMMCRead(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	if(nRetryTimes <= 0)
		nRetryTimes = 1;

	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingDLeMMCRead", StreamingProtocol, 0);
	if(lpData)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			
			CRequestPtr ptRequest = new CStreamingDLeMMCRead(wnLength, dwAddress, bSpeedUp);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
			if(bRes)
				memcpy(lpData, ptBuffer->GetData(), wnLength + 5);
		}
	}
	AddMsg(bRes, "StreamingDLeMMCRead", StreamingProtocol);
	AddDLLog(bRes, "StreamingDLeMMCRead", _T("DownloadProcess"));

	return bRes;
}

bool CDLCommand::StreamingStrRead(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	if(nRetryTimes <= 0)
		nRetryTimes = 1;

	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(lpData)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			CRequestPtr ptRequest = new CStreamingDLStrRead(wnLength, dwAddress, bSpeedUp);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
			if(bRes)
				memcpy(lpData, ptBuffer->GetData(), wnLength);
		}
	}

	return bRes;
}

bool CDLCommand::StreamingStrWrite(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	if(nRetryTimes <= 0)
		nRetryTimes = 1;

	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(lpData)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			CRequestPtr ptRequest = new CStreamingDLStrWrite(lpData, wnLength, dwAddress, bSpeedUp);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		}
	}

	return bRes;
}

bool CDLCommand::StreamingStrBuffer7K(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	if(nRetryTimes <= 0)
		nRetryTimes = 1;

	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(lpData)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			CRequestPtr ptRequest = new CStreamingDLStrBuffer7K(lpData, wnLength, dwAddress, bSpeedUp);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		}
	}

	return bRes;
}

bool CDLCommand::StreamingStrBufferWrite7K(const std::string& strImagePath, CString MD5, int nImageSize, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	if(nRetryTimes <= 0)
		nRetryTimes = 1;

	using namespace PLATFORM_TOOL;
	bool bRes = false;
	
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		CRequestPtr ptRequest = new CStreamingDLStrBufferWrite7K(nImageSize, MD5, bSpeedUp);
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	}

	return bRes;
}

bool CDLCommand::StreamingClose(bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	if(nRetryTimes <= 0)
		nRetryTimes = 1;

	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingClose", StreamingProtocol, 0);
	AddDLLog("Close Image start.", _T("DownloadProcess"));
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		CRequestPtr ptRequest = new CStreamingDLCloseImage(bSpeedUp);
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		AddDLLog(bRes, "Close Image receiver get", _T("DownloadProcess"));
		CStringA strBuffer;
		if (ByteToChar((BYTE*)ptBuffer->GetData(), ptBuffer->GetLength(), strBuffer))
			AddDLLog(strBuffer, _T("DownloadProcess"));
	}

	AddMsg(bRes, "StreamingClose", StreamingProtocol);
	AddDLLog(bRes, "Close Image end", _T("DownloadProcess"));
	return bRes;
}

bool CDLCommand::StreamingClose7K(BYTE nImageType, const std::string& strImagePath, const std::string& strMd5ListPath, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	if(nRetryTimes <= 0)
		nRetryTimes = 1;

	// get MD5 here
	CString Md5FromFile = GetMd5FromList(strImagePath ,strMd5ListPath);

	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingClose7K", StreamingProtocol, 0);
	AddDLLog("Close Image start.", _T("DownloadProcess"));
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		CRequestPtr ptRequest;
		if (nImageType == Image_type_QCSECBLHD || nImageType == Image_type_QCSECBL || nImageType == Image_type_OEMSECBL || 
			nImageType == Image_type_AMSSMODEL || nImageType == Image_type_AMSSAPPBL || nImageType == Image_type_OEMSECBL)
		{
			ptRequest = new CStreamingDLCloseImage7K(Md5FromFile, bSpeedUp);
		}
		else
		{
			ptRequest = new CStreamingDLCloseImage(bSpeedUp);
		}
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		AddDLLog(bRes, "Close Image receiver get", _T("DownloadProcess"));
		CStringA strBuffer;
		if (ByteToChar((BYTE*)ptBuffer->GetData(), ptBuffer->GetLength(), strBuffer))
			AddDLLog(strBuffer, _T("DownloadProcess"));
	}

	AddMsg(bRes, "StreamingClose7K", StreamingProtocol);
	AddDLLog(bRes, "Close Image end", _T("DownloadProcess"));
	return bRes;
}

bool CDLCommand::StreamingReset(bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingReset", StreamingProtocol, 0);
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		CRequestPtr ptRequest = new CStreamingDLReset(bSpeedUp);
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	}
	AddMsg(bRes, "StreamingReset", StreamingProtocol);
	AddDLLog(bRes, "Reset", _T("DownloadProcess"));
	return bRes;
}

CString CDLCommand::GetMd5FromList(const std::string& strImagePath, const std::string& strMd5ListPath)
{
	// get MD5 here
	char *fileName = (char*)strMd5ListPath.c_str();
	ifstream fin(fileName);
	if(!fin)
	{
		AddMsg("Read MD5 list fail", DownloadStep::None, 100);
		return false;
	}

	string sMd5FileLine;
	CString Md5FromFile;
	bool b_BreakFlag = false;
	while(getline(fin, sMd5FileLine))
	{
		CString Md5FileLine;
		CString resToken;
		CString fileName = strImagePath.c_str();
		Md5FileLine.Format("%s", sMd5FileLine.c_str());;
		int curPos = 0;
		resToken = Md5FileLine.Tokenize(" ", curPos);
		while (resToken != _T(""))
		{
			Md5FromFile = resToken.Trim();
			resToken = Md5FileLine.Tokenize(" ", curPos);
			resToken.Replace("./", "");
			if (fileName.Find(resToken) != -1)
			{
				b_BreakFlag = true;
				break;
			}
			else 
			{
				break;
			}
		}
		if (b_BreakFlag) {
			break;
		}
	}
	fin.close();

	return Md5FromFile;
}
/*************************************************************************/
CDLCompoundCommand::CDLCompoundCommand(const TSTRING& strDevice, const std::string& strPlatform)
:CDLCommand(strDevice, strPlatform)
{}
CDLCompoundCommand::CDLCompoundCommand(int nCOMPort, const std::string& strPlatform)
:CDLCommand(nCOMPort, strPlatform)
{}
CDLCompoundCommand::~CDLCompoundCommand(){}

//************************************
// Method:    DMSSOpenAndWriteHexFile
// IN Parameter: const std::string & strHexFilePath
// OUT Parameter: WORD & dwAddress : output for DMSS Command Go
// OPTIONAL Parameter: int nTimeout
//************************************
bool CDLCompoundCommand::DMSSOpenAndWriteHexFile(const std::string& strHexFilePath, WORD& wAddress, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("DMSSOpenAndWriteHexFile", DMSSProtocol, 0);
	if(::_taccess(strHexFilePath.c_str(), 0) == 0)
	{
		CHexFile HexFile;
		CFileException pError;
		if(TRUE == HexFile.Open(strHexFilePath.c_str(),CFile::modeRead | CFile::shareDenyNone,&pError))
		{
			int nTotalSize = (int) HexFile.GetLength()*16/45;  //rough WriteSize
			
			wAddress = HexFile.GetHexAddrss();//Get the first address that Hex File Setting
			DWORD dwTempAddress = wAddress;
			dwTempAddress <<= 16;
			if (wAddress == 0x8001) // update by Lennon 2013.01.15 for dubai short solution
				dwTempAddress = dwTempAddress + 0x3000;
			DWORD dwStartAddress = dwTempAddress;
			WORD dwSize = 0;
			do {
				BYTE nTemp[1024] = {0};
				dwSize = HexFile.Read((char*)nTemp, 0X03F9); // 0x03F9 = 1017 
				bRes = DMSSWrite32Bit(nTemp, dwSize, dwTempAddress, nRetryTimes, nTimeout);
				dwTempAddress += dwSize;
				Sleep(0); // for multi-download
				AddMsg("DMSSOpenAndWriteHexFile", DMSSProtocol, 100*(dwTempAddress-dwStartAddress)/nTotalSize); 
			}while( dwSize == 0X03F9 && bRes );
			HexFile.Close();
		}
	}
	AddMsg(bRes, "DMSSOpenAndWriteHexFile", DMSSProtocol);
	AddDLLog(bRes, _T("OpenAndWriteHexFile"), _T("DownloadProcess"));
	SetLastError(DL_ERROR_DMSS_Hex, bRes);
	return bRes;
}

bool CDLCompoundCommand::DMSSOpenAndWriteHexFile7K(const std::string& strHexFilePath, WORD& wAddress, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("DMSSOpenAndWriteHexFile", DMSSProtocol, 0);
	if(::_taccess(strHexFilePath.c_str(), 0) == 0)
	{
		CHexFile HexFile;
		CFileException pError;
		WORD hex_offset = strHexFilePath.find("ENPRG") != std::string::npos ? 0x3000 : 0;
		if(TRUE == HexFile.Open(strHexFilePath.c_str(),CFile::modeRead | CFile::shareDenyNone,&pError))
		{
			int nTotalSize = (int) HexFile.GetLength()*16/45;  //rough WriteSize
			
			wAddress = HexFile.GetHexAddrss();//Get the first address that Hex File Setting
			DWORD dwTempAddress = wAddress;
			dwTempAddress <<= 16;
			dwTempAddress = dwTempAddress + hex_offset;
			DWORD dwStartAddress = dwTempAddress;
			WORD dwSize = 0;
			do {
				BYTE nTemp[1024] = {0};
				dwSize = HexFile.Read((char*)nTemp, 0X03F9); // 0x03F9 = 1017 
				bRes = DMSSWrite32Bit(nTemp, dwSize, dwTempAddress, nRetryTimes, nTimeout);
				dwTempAddress += dwSize;
				Sleep(0); // for multi-download
				AddMsg("DMSSOpenAndWriteHexFile", DMSSProtocol, 100*(dwTempAddress-dwStartAddress)/nTotalSize); 
			}while( dwSize == 0X03F9 && bRes );
			HexFile.Close();
		}
	}
	AddMsg(bRes, "DMSSOpenAndWriteHexFile", DMSSProtocol);
	AddDLLog(bRes, _T("OpenAndWriteHexFile"), _T("DownloadProcess"));
	SetLastError(DL_ERROR_DMSS_Hex, bRes);
	return bRes;
}

bool CDLCompoundCommand::DMSSOpenAndWriteHexFile(const BYTE* pData, int nTotalSize, WORD& wAddress, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("DMSSOpenAndWriteHexFile", DMSSProtocol, 0);
	int nLeftDataSize = nTotalSize;
	BYTE* pHex = (BYTE*)pData;
	if(pData && nTotalSize > 15)
	{
		CHexDataParser HexParser((BYTE**)(&pHex), nLeftDataSize, wAddress);
		DWORD dwTempAddress = HexParser.GetHexAddrss();
		WORD dwReadSize = 0;
		do 
		{
			BYTE nTemp[1024] = {0};
			bool bParsed = false;
			dwReadSize = HexParser.Read((char*)nTemp, 0X03F9, bParsed); // 0x03F9 = 1017 
			bRes = bParsed ? DMSSWrite32Bit(nTemp, dwReadSize, dwTempAddress, nRetryTimes, nTimeout): false;
			dwTempAddress += dwReadSize;
			Sleep(0); // for multi-download
			AddMsg("DMSSOpenAndWriteHexFile", DMSSProtocol, 100*(dwTempAddress-HexParser.GetHexAddrss())/nTotalSize); 
		} while(dwReadSize == 0X03F9 && bRes);
	}
	AddMsg(bRes, "DMSSOpenAndWriteHexFile", DMSSProtocol);
	AddDLLog(bRes, _T("OpenAndWriteHexFile"), _T("DownloadProcess"));
	SetLastError(DL_ERROR_DMSS_Hex, bRes);
	return bRes;
}
//************************************
// Method:    StreamingOpenAndWritePartition
// IN Parameter: const std::string & strPartition
// IN Parameter: bool bOverride : Usual setting should set false. 
//    Set true only when the partition is different from the unit's.
// OPTIONAL Parameter: bool bSpeedUp
// OPTIONAL Parameter: int nTimeout
//************************************
bool CDLCompoundCommand::StreamingOpenAndWritePartition(const std::string& strPartition, bool bOverride, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(::_taccess(strPartition.c_str(), 0) == 0)
	{
		CFile file;
		CFileException pError;
		if(TRUE == file.Open(strPartition.c_str(),CFile::modeRead | CFile::shareDenyNone,&pError))
		{
			UINT unsize = (UINT)file.GetLength();
			BYTE *lpData = new BYTE[unsize+1];
			file.Read(lpData,unsize);
			bRes = StreamingPartition(lpData, unsize, bOverride, bSpeedUp, nRetryTimes, nTimeout);
			file.Close();
			delete [] lpData;
			lpData = NULL;
		}
	}
	return bRes;
}

//************************************
// Method:    StreamingOpenHeaderImage
// IN Parameter: BYTE nType 
// IN Parameter: const std::string & strHeaderImagePath
// OPTIONAL Parameter: bool bSpeedUp
// OPTIONAL Parameter: int nTimeout
// Comment: In Qualcomm's design, only OEMSBL/AMSS needs Header Image(OEMSBLHD/AMSSHD).
//          Others only use OpenMultiImage Command.
//************************************
#include <io.h>
bool CDLCompoundCommand::StreamingOpenHeaderImage(BYTE nType, const std::string& strHeaderImagePath/*MaxImageSize=512*/, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	BYTE* lpData = NULL;
	UINT wnLength = 0;
	if(nType==Image_type_OEMSECBL || nType==Image_type_AMSSMODEL || nType==Image_type_AMSSAPP || nType == Image_type_AMSSAPPBL)
	{ 
		if(_access(strHeaderImagePath.c_str(),0) != -1)
		{
			CFile cfheader;
			CFileException pError;
			if(cfheader.Open(strHeaderImagePath.c_str(),CFile::shareDenyNone,&pError) == TRUE)
			{
				wnLength = (UINT)cfheader.GetLength();
				lpData = new BYTE[wnLength+1];
				cfheader.Read(lpData,wnLength);
				cfheader.Close();
				bRes = true;
			}
		}
		else
			bRes = true;
	}
	else 
		bRes = true;
	if(bRes)
		bRes = StreamingOpenMultiImage(nType, lpData, wnLength, bSpeedUp, nRetryTimes, nTimeout);
	if(lpData)
	{
		delete [] lpData;
		lpData = NULL;
	}
	return bRes;
}

bool CDLCompoundCommand::StreamingOpenHeaderImage7K(BYTE nType, const std::string& strHeaderImagePath/*MaxImageSize=512*/, const std::string& strImagePath, const std::string& strMd5ListPath, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	BYTE* lpData = NULL;
	UINT wnLength = 0;

	// get MD5 here
	CString Md5FromFile = GetMd5FromList(strHeaderImagePath, strMd5ListPath);

	if(nType==Image_type_OEMSECBL || nType==Image_type_AMSSMODEL || nType==Image_type_AMSSAPP || nType == Image_type_AMSSAPPBL)
	{ 
		if(_access(strHeaderImagePath.c_str(),0) != -1)
		{
			CFile cfheader;
			CFileException pError;
			if(cfheader.Open(strHeaderImagePath.c_str(),CFile::shareDenyNone,&pError) == TRUE)
			{
				wnLength = (UINT)cfheader.GetLength();
				lpData = new BYTE[wnLength+1];
				cfheader.Read(lpData,wnLength);
				cfheader.Close();
				bRes = true;
			}
		}
		else
			bRes = true;
	}
	else 
		bRes = true;
	if(bRes)
		bRes = StreamingOpenMultiImage7K(nType, lpData, wnLength, Md5FromFile, bSpeedUp, nRetryTimes, nTimeout);
	if(lpData)
	{
		delete [] lpData;
		lpData = NULL;
	}
	return bRes;
}

bool CDLCompoundCommand::StreamingOpenHeaderImage(BYTE nType, const BYTE* pImageData, int nImageSize/*MaxImageSize=512*/, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	
	BYTE* pImage = (BYTE*)pImageData;
	if(nType==Image_type_OEMSECBL || nType==Image_type_AMSSMODEL || nType==Image_type_AMSSAPP || nType == Image_type_AMSSAPPBL)
	{ 
		if(pImage && nImageSize>0 && nImageSize<513)
			bRes = true;
	}
	else 
	{
		pImage = NULL;
		nImageSize = 0;
		bRes = true;
	}
	if(bRes)
		bRes = StreamingOpenMultiImage(nType, pImage, nImageSize, bSpeedUp, nRetryTimes, nTimeout);

	return bRes;
}


bool CDLCompoundCommand::StreamingWriteMultiImage(const std::string& strImagePath, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingWriteMultiImage", StreamingProtocol, 0);
	CFile cfImage;
	CFileException pError;
	if(cfImage.Open(strImagePath.c_str(),CFile::shareDenyNone | CFile::modeRead,&pError) == TRUE)
	{
		int nPageSize = bSpeedUp ? PAGE_SIZE : 1024;
		int nImageSize = (int) cfImage.GetLength();
		Struct_XOR XOR={0};
		int nProgressCount = 0;
		/* Alex Add Start 2011/03/08 */
		int i_hadSendSize = 0;
		/* Alex Add End 2011/03/08 */
		if(nImageSize > 0)
		{
			DWORD dwAddress = 0;
			WORD wReadSize = 0;
			do {
				BYTE nTemp[PAGE_SIZE+32] = {0};
				wReadSize = cfImage.Read(nTemp,nPageSize);
				if(bSpeedUp)
					//bRes = StreamingStrWrite(nTemp, wReadSize, dwAddress, XOR, bSpeedUp, nRetryTimes, nTimeout);	 // speed up download always default check image size and integrity	
					;
				else
					bRes = StreamingStrWrite(nTemp, wReadSize, dwAddress, bSpeedUp, nRetryTimes, nTimeout);
				dwAddress += wReadSize;
				Sleep(0); // for multi-download

				/* Alex Add Start 2011/03/08 */
				i_hadSendSize += wReadSize;
				/* Alex Add End 2011/03/08 */
				nProgressCount++;
				AddMsg("StreamingWriteMultiImage", StreamingProtocol, 100*nProgressCount/(nImageSize/nPageSize+1));
			/* Alex Modify Start 2011/03/08 */
			//}while( wReadSize == nPageSize && bRes );
			}while( wReadSize == nPageSize && i_hadSendSize < nImageSize && bRes );
			/* Alex Modify End 2011/03/08 */
		}
		
		//if(bRes && bSpeedUp)
			//bRes = CompareImageSize(bSpeedUp, nImageSize, XOR.nXOR, nTimeout);

		cfImage.Close();
	}
	AddMsg(bRes, "StreamingWriteMultiImage", StreamingProtocol);
	AddDLLog(bRes, "WriteMultiImage", _T("DownloadProcess"));
	SetLastError(DL_ERROR_Streaming_Image, bRes);
	return bRes;
}

bool CDLCompoundCommand::StreamingBufferMultiImage7K(const std::string& strImagePath, const std::string& strMd5ListPath, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingBufferMultiImage7K", StreamingProtocol, 0);
	CFile cfImage;
	CFileException pError;
	int nImageSize;
	if(cfImage.Open(strImagePath.c_str(),CFile::shareDenyNone | CFile::modeRead,&pError) == TRUE)
	{
		int nPageSize = bSpeedUp ? PAGE_SIZE : 1024;
		nImageSize = (int) cfImage.GetLength();
		Struct_XOR XOR={0};
		int nProgressCount = 0;
		/* Alex Add Start 2011/03/08 */
		int i_hadSendSize = 0;
		/* Alex Add End 2011/03/08 */
		if(nImageSize > 0)
		{
			DWORD dwAddress = 0;
			WORD wReadSize = 0;
			do {
				BYTE nTemp[PAGE_SIZE+32] = {0};
				wReadSize = cfImage.Read(nTemp,nPageSize);
				if(bSpeedUp)
					//bRes = StreamingStrWrite(nTemp, wReadSize, dwAddress, XOR, bSpeedUp, nRetryTimes, nTimeout);	 // speed up download always default check image size and integrity	
					;
				else
					bRes = StreamingStrBuffer7K(nTemp, wReadSize, dwAddress, bSpeedUp, nRetryTimes, nTimeout);
				dwAddress += wReadSize;
				Sleep(0); // for multi-download

				/* Alex Add Start 2011/03/08 */
				i_hadSendSize += wReadSize;
				/* Alex Add End 2011/03/08 */
				nProgressCount++;
				AddMsg("StreamingWriteMultiImage", StreamingProtocol, 100*nProgressCount/(nImageSize/nPageSize+1));
			/* Alex Modify Start 2011/03/08 */
			//}while( wReadSize == nPageSize && bRes );
			}while( wReadSize == nPageSize && i_hadSendSize < nImageSize && bRes );
			/* Alex Modify End 2011/03/08 */
		}
		
		//if(bRes && bSpeedUp)
			//bRes = CompareImageSize(bSpeedUp, nImageSize, XOR.nXOR, nTimeout);

		cfImage.Close();
	}

	// Handle_buffer_write send MD5 to target
	if (bRes) 
	{
		StreamingBufferWriteMultiImage7K(strImagePath, strMd5ListPath, nImageSize, bSpeedUp, nRetryTimes, nTimeout);
	}

	AddMsg(bRes, "StreamingWriteMultiImage", StreamingProtocol);
	AddDLLog(bRes, "WriteMultiImage", _T("DownloadProcess"));
	SetLastError(DL_ERROR_Streaming_Image, bRes);
	return bRes;
}

bool CDLCompoundCommand::StreamingBufferWriteMultiImage7K(const std::string& strImagePath, const std::string& strMd5ListPath, int nImageSize, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingBufferWriteMultiImage7K", StreamingProtocol, 0);

	// get MD5 here
	CString Md5FromFile = GetMd5FromList(strImagePath ,strMd5ListPath);
	
	bRes = StreamingStrBufferWrite7K(strImagePath, Md5FromFile, nImageSize, bSpeedUp, nRetryTimes, nTimeout);
				
	AddMsg(bRes, "StreamingBufferWriteMultiImage7K", StreamingProtocol);
	AddDLLog(bRes, "BufferWriteMultiImage7K", _T("DownloadProcess"));
	SetLastError(DL_ERROR_Streaming_Image, bRes);
	return bRes;
}

bool CDLCompoundCommand::StreamingWriteMultiImage(const BYTE* pImageData, int nImageSize, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingWriteMultiImage", StreamingProtocol, 0);
	if(pImageData && nImageSize >0)
	{
		int nPageSize = bSpeedUp ? PAGE_SIZE : 1024;
		Struct_XOR XOR={0};
		int nLoopCount = nImageSize/nPageSize +1;
		bRes = true;
		DWORD dwAddress = 0;
		for(int i=0; i < nLoopCount && bRes; i++)
		{
			WORD wReadSize = (i != nLoopCount-1) ? nPageSize : (nImageSize-(nLoopCount-1)*nPageSize);
			if(bSpeedUp)
				//bRes = StreamingStrWrite((BYTE*)pImageData+dwAddress, wReadSize, dwAddress, XOR, bSpeedUp, nRetryTimes, nTimeout);	 // speed up download always default check image size and integrity	
				;
			else
				bRes = StreamingStrWrite((BYTE*)pImageData+dwAddress, wReadSize, dwAddress, bSpeedUp, nRetryTimes, nTimeout);
			dwAddress += wReadSize;
			Sleep(0); // for multi-download
			AddMsg("StreamingWriteMultiImage", StreamingProtocol, 100*i/nLoopCount);
		}
		
		//if(bRes && bSpeedUp)
			//bRes = CompareImageSize(bSpeedUp, nImageSize, XOR.nXOR, nTimeout);
	}
	AddMsg(bRes, "StreamingWriteMultiImage", StreamingProtocol);
	AddDLLog(bRes, _T("WriteMultiImage"), _T("DownloadProcess"));
	SetLastError(DL_ERROR_Streaming_Image, bRes);
	return bRes;
}

bool CDLCompoundCommand::StreamingDLMultiImage(BYTE nImageType, const std::string& strHeaderImagePath, const std::string& strImagePath, bool bSpeedUp, int nRetryTimes, int nOpenTimeout, int nWriteTimeout, int nCloseTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(::_taccess(strImagePath.c_str(), 0) == 0)
	{
		if(StreamingOpenHeaderImage(nImageType, strHeaderImagePath, bSpeedUp, nRetryTimes, nOpenTimeout) &&
		   StreamingWriteMultiImage(strImagePath, bSpeedUp, nRetryTimes, nWriteTimeout) && 
		   StreamingClose(bSpeedUp, nRetryTimes, nCloseTimeout))
		{
			bRes = true;
		}
	}
	else
		bRes = true; // if image is empty, we don't download and pass success for the whole procedure
	return bRes;
}

bool CDLCompoundCommand::StreamingDLMultiImage7K(BYTE nImageType, const std::string& strHeaderImagePath, const std::string& strImagePath, const std::string& strMd5ListPath, bool bSpeedUp, int nRetryTimes, int nOpenTimeout, int nWriteTimeout, int nCloseTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(::_taccess(strImagePath.c_str(), 0) == 0)
	{
		for (int i = 0; i < g_nRetryTimes; i++)
		{
			if(StreamingOpenHeaderImage7K(nImageType, strHeaderImagePath, strImagePath, strMd5ListPath, bSpeedUp, nRetryTimes, nOpenTimeout) &&
				StreamingWriteMultiImage(strImagePath, bSpeedUp, nRetryTimes, nWriteTimeout) && 
				StreamingClose7K(nImageType, strImagePath, strMd5ListPath, bSpeedUp, nRetryTimes, nCloseTimeout))
			{
				bRes = true;
				break;
			}
		}
	}
	else
		bRes = true; // if image is empty, we don't download and pass success for the whole procedure

	return bRes;
}

bool CDLCompoundCommand::StreamingDLMultiImage(BYTE nImageType, const BYTE* pHeaderData, int nHeaderSize, const BYTE* pImageData, int nImageSize, bool bSpeedUp, int nRetryTimes, int nOpenTimeout, int nWriteTimeout, int nCloseTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(pImageData && nImageSize>0)
	{
		if(StreamingOpenHeaderImage(nImageType, pHeaderData, nHeaderSize, bSpeedUp, nRetryTimes, nOpenTimeout) &&
			StreamingWriteMultiImage(pImageData, nImageSize, bSpeedUp, nRetryTimes, nWriteTimeout) && 
			StreamingClose(bSpeedUp, nRetryTimes, nCloseTimeout))
		{
			bRes = true;
		}
	}
	else
		bRes = true; // if image is empty, we don't download and pass success for the whole procedure
	return bRes;
}


bool CDLCompoundCommand::StreamingDLMultiImage(BYTE nImageType, const std::string& strImagePath, bool bSpeedUp, int nRetryTimes, int nOpenTimeout, int nWriteTimeout, int nCloseTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(::_taccess(strImagePath.c_str(), 0) == 0)
	{
		if(StreamingOpenMultiImage(nImageType, NULL, 0, bSpeedUp, nRetryTimes, nOpenTimeout) &&
			StreamingWriteMultiImage(strImagePath, bSpeedUp, nRetryTimes, nWriteTimeout) && 
			StreamingClose(bSpeedUp, nRetryTimes, nCloseTimeout))
		{
			bRes = true;
		}
		else
		{
			bRes = false;
		}
	}
	else
		bRes = true; // if image is empty, we don't download and pass success for the whole procedure

	return bRes;
}

bool CDLCompoundCommand::StreamingDLMultiImage(BYTE nImageType, const BYTE* pImageData, int nImageSize, bool bSpeedUp, int nRetryTimes, int nOpenTimeout, int nWriteTimeout, int nCloseTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(pImageData && nImageSize>0)
	{
		if(StreamingOpenMultiImage(nImageType, NULL, 0, bSpeedUp, nRetryTimes, nOpenTimeout) &&
			StreamingWriteMultiImage(pImageData, nImageSize, bSpeedUp, nRetryTimes, nWriteTimeout) && 
			StreamingClose(bSpeedUp, nRetryTimes, nCloseTimeout))
		{
			bRes = true;
		}
	}
	else
		bRes = true; // if image is empty, we don't download and pass success for the whole procedure
	
	return bRes;
}





/*************************************************************************/
CDLFormatCommand::CDLFormatCommand(const TSTRING& strDevice, const std::string& strPlatform)
:CDLCompoundCommand(strDevice, strPlatform)
{
}

CDLFormatCommand::CDLFormatCommand(int nCOMPort, const std::string& strPlatform)
:CDLCompoundCommand(nCOMPort, strPlatform)
{
}

CDLFormatCommand::~CDLFormatCommand()
{
}




bool CDLFormatCommand::RAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CDLRAWRequest(szCmdInput, nCmdLength, bAddCRC);	
	bRes = Execute(ptRequest);
	return bRes;
}





bool CDLFormatCommand::CheckBadBlockWithHandShaking(bool& bStopAccepting, const char* szData, int nDataSize,int nTimeout,CallBackInfo AddMsg)
{
	using namespace PLATFORM_TOOL;
	bStopAccepting = true;
	bool bRes = false;
	CRequestPtr ptRequest = new CDLRawWithReturnRequest(szData, nDataSize, true);
	CBufferPtr ptBuffer = new CBuffer;
	bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	if(bRes && ptBuffer)
	{
		BYTE* lpData = (BYTE*)(ptBuffer->GetData());
		if(lpData)
		{
			if(lpData[0] == 0x02) // End
			{
				std::string strMsg = "End: ";
				int nTimes = 0; 
				memcpy(&nTimes, lpData+1, 4);
				TCHAR szTimes[100] = {0};
				_itot_s(nTimes, szTimes, 10);
				strMsg += szTimes;
				AddMsg(strMsg+" times");
			}
			else if(lpData[0] == 0x00) // Success
			{
				bStopAccepting = false;
				std::string strMsg = "Success: ";
				int nTimes = 0; 
				memcpy(&nTimes, lpData+1, 4);
				TCHAR szTimes[100] = {0};
				_itot_s(nTimes, szTimes, 10);
				strMsg += szTimes;

				AddMsg(strMsg+" times");
			}
			else if(lpData[0] == 0x01) // Fail
			{
				bStopAccepting = false;
				std::string strMsg = "Fail:";
				int nTimes = 0; 
				memcpy(&nTimes, lpData+1, 4);
				TCHAR szTimes[100] = {0};
				_itot_s(nTimes, szTimes, 10);
				strMsg += szTimes;
				AddMsg(strMsg+" times");
			}
			else
			{
				bStopAccepting = false;
				AddMsg(_T("Unknown Response(without 7E and CRC):"));
				CStringA strData;
				if(ByteToChar((BYTE*)ptBuffer->GetData(), ptBuffer->GetLength(), strData))
				{	
	#ifdef _UNICODE
					std::string strTemp = strData;
					AddMsg(QSYNC_LIB::GetTSTRING(strTemp));
	#else
					AddMsg((std::string)strData);
	#endif

				}

			}
		}
		
		
	}
	return bRes;
}
bool CDLFormatCommand::DumpImage(bool& bStopAccepting, const char* szData, int nDataSize,int nTimeout, CBufferPtr ptImage)
{
	using namespace PLATFORM_TOOL;
	
	bool bRes = false;
	CBufferPtr ptBuffer = new CBuffer;
	CRequestPtr ptRequest = new CDLRawWithReturnRequest(szData, nDataSize, true);
	bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	if(bRes && ptBuffer)
	{
		BYTE* lpData = (BYTE*)(ptBuffer->GetData());
		if(lpData)
		{
			if(lpData[0] == 0x02) // End
			{
				bStopAccepting = true;
			}
			else if(lpData[0] == 0x03)
			{
				bStopAccepting = false;
				CBufferPtr ptData = new CBuffer(ptBuffer->GetLength()-1,(const char*)(lpData+1));
				ptImage->Append(ptData);
			}
			else
			{
				bStopAccepting = true;
				bRes = false;
			}
		}
	}
	else
		bStopAccepting = true;
	return bRes;
}


bool CDLFormatCommand::Download(bool bSpeedup, bool bReopenAfterReset, DownloadProtocol nDLPROTOCOL) {
    return false;
}
bool CDLFormatCommand::UARTDownload(bool bSpeedup, bool bReopenAfterReset, DownloadProtocol nDLPROTOCOL) {
	return false;
}
void CDLFormatCommand::SetResetMode(bool bReset) {
    assert(0);
}
bool CDLFormatCommand::SendHello(bool bSpeedUp)
{
	bool bRes = false;
	//if(!StreamingHello_CheckBadBlock(bSpeedUp, g_n6245HelloRetryTimes_CheckBadBlock, g_n6245HelloTimeout_CheckBadBlock))
		bRes = StreamingHello(bSpeedUp, g_n6245HelloRetryTimes, g_n6245HelloTimeout);
	//else
	//	bRes = true;
	return bRes;
}
void CDLFormatCommand::SetIsOveridePartition(bool bOverridePartition) {
    assert(0);
}

bool CDLFormatCommand::StreamingDLFromPartition(bool bSpeedup, bool bReopenAfterReset) {
    assert(0);
    return false;
}

//////////////////////////////6240////////////////////////////////////////////
bool CDownload6240::SetHex(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strHEX = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6240::SetPartition(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strPartition = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6240::SetPBL(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strPBL = strFilePath;
		bRes = true;
	}
	return bRes;
}

bool CDownload6240::SetAMSS(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strAMSS = strFilePath;
		bRes = true;
	}
	return bRes;
}

bool CDownload6240::SetAMSSHD(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strAMSSHD = strFilePath;
		bRes = true;
	}
	return bRes;
}

bool CDownload6240::SetAPPS(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strAPPS = strFilePath;
		bRes = true;
	}
	return bRes;
}

bool CDownload6240::SetAPPSHD(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strAPPSHD = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6240::SetAPPSBOOT(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strAPPSBOOT = strFilePath;
		bRes = true;
	}
	return bRes;
}

bool CDownload6240::SetAPPSBOOTHD(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strAPPSBOOTHD = strFilePath;
		bRes = true;
	}
	return bRes;
}

bool CDownload6240::SetCEFS(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strCEFS = strFilePath;
		bRes = true;
	}
	return bRes;
}

//************************************
// Method:    SetResetMode
// Returns:   void
// Qualifier:
// Parameter: bool bReset : bReset == true means will reset mode if flashing fails
//************************************
void CDownload6240::SetResetMode(bool bReset)
{
	m_bResetMode = bReset;
}

void CDownload6240::SetIsOveridePartition(bool bOverridePartition)
{
	if(bOverridePartition)
		AddMsg("Set Override Partition ON", None, 100);
	m_bOverridePartition = bOverridePartition;
}

bool CDownload6240::SetDBL(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strDBL = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6240::SetFSBL(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strFSBL = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6240::SetOSBL(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strOSBL = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6240::SetDSP1(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strDSP1 = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6240::SetWIN60(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strWIN60 = strFilePath;
		bRes = true;
	}
	return bRes;
}
CDownload6240::CDownload6240(const TSTRING& strDevice, const std::string& strPlatform, bool bReopenAfterReset)
:CDLFormatCommand(strDevice, strPlatform), m_bOverridePartition(false), m_bResetMode(true), m_bReopenAfterReset(bReopenAfterReset)
{
}

CDownload6240::CDownload6240(int nCOMPort, const std::string& strPlatform, bool bReopenAfterReset)
:CDLFormatCommand(COMDeviceName(nCOMPort), strPlatform), m_bOverridePartition(false), m_bResetMode(true), m_bReopenAfterReset(bReopenAfterReset)
{
}

CDownload6240::~CDownload6240(void)
{
}

bool CDownload6240::CFCDownload(bool bSpeedup)
{
	bool bRes = false;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
		PopLastLog();
	}
	return bRes;
}

bool CDownload6240::Download(bool bSpeedup, bool bReopenAfterReset, DownloadProtocol nDLPROTOCOL)
{
	bool bRes = false;
	m_bReopenAfterReset = bReopenAfterReset;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		AddMsg("Download Success", None, 100);
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode(); 
		AddMsg("Download Fail", None, 0);
		PopLastLog();
	}
	return bRes;
}

bool CDownload6240::DMSSDownload(const std::string& strHexFilePath)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	WORD dwAddress=0;
	if( DMSSNop(g_n6240NopRetryTimes,g_n6240NopTimeout) && 
		DMSSParameter() &&
		DMSSOpenAndWriteHexFile( (strHexFilePath.empty() ? m_strHEX : strHexFilePath), dwAddress) &&
		DMSSGo(dwAddress, 0))
	{
		bRes = true;
	}
	return bRes;
}


bool CDownload6240::StreamingDL(bool bSpeedUp,DownloadProtocol nDLPROTOCOL)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CReOpenComport ReopenAfterGo(this, true);
	bool bReconnect = true;
	if(GetPlatorm() == "Qualcomm6240")
	{
		bReconnect = ReopenAfterGo.ReOepnComport(g_n6240ReOpenAfterGoSleepTime, false);
		AddMsg(bReconnect, "ReOpenAfterGo", StreamingProtocol);
	}
	else
	{
		Sleep(10000);
	}
	ASSERT(bReconnect);

	if( bReconnect && 
		StreamingHello(bSpeedUp, g_n6240HelloRetryTimes, g_n6240HelloTimeout) &&
		StreamingSecurity("", bSpeedUp) && 
		StreamingOpenAndWritePartition(m_strPartition, m_bOverridePartition, bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_DBL,  m_strDBL,  bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_FSBL, m_strFSBL, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_OSBL, m_strOSBL, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) && 
		StreamingDLMultiImage(OPEN_MULTI_MODE_AMSS, m_strAMSSHD,m_strAMSS, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_APPS, m_strAPPSHD,m_strAPPS, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_APPSBL, m_strAPPSBOOTHD,m_strAPPSBOOT, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_APPS_WM60, "",m_strWIN60, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_DSP1, "",m_strDSP1, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_CEFS, m_strCEFS, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingReset(bSpeedUp))
	{
		bRes = true;

		if(m_bReopenAfterReset)
		{
			CReOpenComport ReopenAfterReset(this);
			bReconnect = ReopenAfterReset.ReOepnComport(g_n6240ReOpenAfterResetSleepTime_Path, false);
			ASSERT(bReconnect);
			AddMsg(bReconnect, "ReOpenAfterReset", ReOpenComport);
			SetLastError(DL_ERROR_ReOpenComport, bReconnect);
		}
		else
			bReconnect = true;
	}

	return bRes && bReconnect;
}
//************************************
// Method:    StreamingDLFromPartition
// Returns:   bool : download success or fail
// Parameter: bool bSpeedUp : it must be the same as previous one
// Purpose: If previous download is failed in different partition table,
//			for overwriting partition table, 
//			you can resend partition with m_bOverridePartition==true by this function to keep downloading.
//			Meanwhile, the unit can't be unplugged and should be in the same waiting status. 
//************************************
bool CDownload6240::StreamingDLFromPartition(bool bSpeedUp, bool bReopenAfterReset)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	if(	StreamingOpenAndWritePartition(m_strPartition, m_bOverridePartition, bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_DBL,  m_strDBL,  bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_FSBL, m_strFSBL, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_OSBL, m_strOSBL, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) && 
		StreamingDLMultiImage(OPEN_MULTI_MODE_AMSS, m_strAMSS, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_CEFS, m_strCEFS, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingReset(bSpeedUp))
	{
		if(bReopenAfterReset)
		{
			CReOpenComport ReopenAfterReset(this);
			bRes = ReopenAfterReset.ReOepnComport(g_n6240ReOpenAfterResetSleepTime, true);
			AddMsg(bRes, "ReOpenAfterReset", ReOpenComport);
			SetLastError(DL_ERROR_ReOpenComport, bRes);
		}
		else
			bRes = true;
		
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
		AddMsg("Download Fail", None, 0);
	}

	return bRes;
}