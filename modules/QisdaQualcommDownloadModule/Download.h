#ifndef __DL__H__
#define __DL__H__
#pragma once

#include "..\Lib\RequestUtility\InterfaceBase.h"
#include "..\Lib\RequestUtility\DownloadCommand.h"
//#include "..\RequestManager\Request\DownloadRequest.h"
#include "..\Common\CommonUtil\RefObject.h"
#include <vector>
#include "..\Common\CommonUtil\NotifyBase.h"
#include "..\Common\CommonUtil\ErrorCode.h"
#include "..\Common\ErrorDefine\ErrorDefine.h"

#pragma warning(push)
#pragma warning(disable:4251)


#ifdef QISDA_DOWNLOAD_MODULE_EXPORTS
#define QISDA_DOWNLOAD_API extern "C" __declspec(dllexport)
#define QISDA_DOWNLOAD_CLASS_API __declspec(dllexport)
#else
#define QISDA_DOWNLOAD_API extern "C" __declspec(dllimport)
#define QISDA_DOWNLOAD_CLASS_API __declspec(dllimport)
#endif

const LPCSTR EVENT_DL_PROGRESS		= "TNotify_DL_Progress";
const LPCSTR EVENT_DL_PARTITION_DIF = "TNotify_DL_Partition_Different";
const LPCSTR EVENT_DL_CHECKRONTING  = "TNotify_DL_Check_Routing";

const int g_nTimeout = 20000;
const int g_nRetryTimes = 3;
/************************************************************************/
/* 6240 fine tune data                                                  */
/************************************************************************/
const int g_n6240NopRetryTimes		   		= 30;
const int g_n6240NopTimeout			   		= 1000;
const int g_n6240HelloRetryTimes	   		= 100;
const int g_n6240HelloTimeout		   		= 700;
const int g_n6240ReOpenAfterGoSleepTime		= 35000;
const int g_n6240ReOpenAfterResetSleepTime	= 40000;
const int g_n6240ReOpenAfterResetSleepTime_Path = 30000;
const int g_n6240CloseImageTimeout			= 300000;
/************************************************************************/
/* 6245 fine tune data                                                  */
/************************************************************************/
const int g_n6245NopRetryTimes				= 20;
const int g_n6245NopTimeout					= 1000;
const int g_n6245HelloRetryTimes			= 60;
const int g_n6245HelloTimeout				= 3000;
const int g_n6245HelloRetryTimes_CheckBadBlock			= 200;
const int g_n6245HelloTimeout_CheckBadBlock				= 1000;
const int g_n6245ReOpenAfterGoSleepTime		= 35000;
const int g_n6245ReOpenAfterResetSleepTime  = 35000;
const int g_n6245ReOpenAfterResetSleepTime_Path = 35000;
const int g_n6245CloseImageTimeout			= 120000;

/* Alex Modify Start 2011/03/08 */

enum DownloadStep
{
	None = 0,  // For notice
	Connection = 1,
	OpenImage = 2,
	ChangeDLMode = 3,
	DMSSProtocol = 4,
	StreamingProtocol = 5,
	ReOpenComport = 6,
	ADBProtocol = 7,
	SaharaProtocol = 8,
	FireHoseProtocol = 9
};
/* Alex Modify End 2011/03/08 */

typedef struct
{
	char* szMsg;
	DownloadStep nStep;
	int nProgress;
}DL_PROGRESS;


typedef struct
{
	char   chHandleName  [MAX_PATH];
	char   chInstanceName[MAX_PATH];
	char   chFriendlyName[MAX_PATH];
	char   chDeviceDescr [MAX_PATH];
	char   chParentName  [MAX_PATH];
	char   chSerialNumber[MAX_PATH];
	int    nRealPort;
	DWORD  dwDeviceNumber;

} USBDEVICE_STRUCT;

class IMAGE
{
public:
	IMAGE():pData(NULL),nSize(0)
	{
	}
	~IMAGE()
	{
		clean();
	}
	bool copyFrom(const BYTE* pSrcData, const int& nSrcSize)
	{
		bool bRes = false;
		if(nSrcSize > 0)
		{
			clean();
			pData = new BYTE[nSrcSize];
			::memcpy((void*)pData,pSrcData,nSrcSize);
			nSize = nSrcSize;
			bRes = true;
		}
		return bRes;
	}
	IMAGE& operator=(const IMAGE& Value)
	{
		copyFrom(Value.pData,Value.nSize);
		return (*this);
	}
	BYTE* pData;
	int nSize;
private:
	void clean()
	{
		if(pData)
		{
            delete pData;
			pData = NULL;
			nSize = 0;
		}
	}
};

class QISDA_DOWNLOAD_CLASS_API CDLCommand : public QSYNC_LIB::CNotifyBase_ANSI
{
public:
	//CDLFormatCommand(const TSTRING& strDevice, const std::string& strPlatform = "Qualcomm6245");
	CDLCommand();
	//CDLFormatCommand(int nCOMPort, const std::string& strPlatform = "Qualcomm6245");
	~CDLCommand(){};

	int m_nProgress; // Keep Original Progress data
	std::string m_strMsg; // Keep Original Message

	void AddMsg(const char* szMsg, int nStep, int nProgress); //Fire function
	void AddMsg(bool bResult, const char* szTitle, int nStep); // Notify successful or failed in the end of command 
public:// CNotifyBase
	virtual bool Register(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Register(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual bool Unregister(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Unregister(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual void Fire(const std::string& strEvent,long nParam);
	virtual void DefineNotify(const std::string& strNewEvent);

	/************************************************************************/
	/*                   ErrorCode related function                         */
	/************************************************************************/
public:
	void SetLastError(DL_ERROR nError, bool bExecuteResult = true);
};


//
//class QISDA_DOWNLOAD_CLASS_API CDLCommand : public CInterfaceBase, public QSYNC_LIB::CNotifyBase_ANSI, public CLog, public CLastError
//{
//public:
//	CDLCommand(const TSTRING& strDevice, const std::string& strPlatform = "Qualcomm6245");
//	CDLCommand(int nCOMPort, const std::string& strPlatform = "Qualcomm6245");
//	virtual ~CDLCommand();
//public: // DMSS DL
//	//Command
//	virtual bool DMSSReset(int nRetryTimes = 10, int nTimeout=g_nTimeout);
//	virtual bool DMSSNop(int nRetryTimes = 10, int nTimeout=g_nTimeout);
//	virtual bool DMSSParameter(int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//	virtual bool DMSSWrite32Bit(BYTE* lpWData, WORD dwWLength, DWORD dwAddress, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//	virtual bool DMSSGo(WORD nwSegment,WORD nwoffset, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//public: // Streaming DL
//	//Command
//	virtual bool StreamingHello(bool bSpeedUp = false, int nRetryTimes = 50, int nTimeout= 500,DownloadProtocol nDownloadProtocol = QTT_FASTDOWNLOAD);
//	//virtual bool StreamingHello_CheckBadBlock(bool bSpeedUp = false, int nRetryTimes = 50, int nTimeout= 500); // only support in Speedup DL
//	virtual bool StreamingSecurity(const std::string& strPBL, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//	virtual bool StreamingPartition(BYTE* lpbyData, WORD wnlength/*max=512*/, bool bOverride, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//	virtual bool StreamingOpenMultiImage(BYTE nType, BYTE* lpbyData, WORD wnlength/*max=512*/, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//	virtual bool StreamingOpenMultiImage7K(BYTE nType, BYTE* lpbyData, WORD wnlength/*max=512*/, CString Md5FromFile, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//	virtual bool StreamingIRAMRead(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp, int nRetryTimes, int nTimeout);
//	virtual bool StreamingDLeMMCRead(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp, int nRetryTimes, int nTimeout);
//	virtual bool StreamingStrRead(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp, int nRetryTimes, int nTimeout);
//	virtual bool StreamingStrWrite(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout); // stream write
//	virtual bool StreamingStrBuffer7K(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout); // stream write
//	virtual bool StreamingStrBufferWrite7K(const std::string& strImagePath, CString MD5, int nImageSize, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout); // stream write
//	//virtual bool StreamingStrWrite(BYTE* lpData, WORD wnLength, DWORD dwAddress, Struct_XOR& nXOR, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout); // stream write with return XOR[4]
//	virtual bool StreamingClose(bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//	virtual bool StreamingClose7K(BYTE nImageType, const std::string& strImagePath, const std::string& strMd5ListPath, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//	virtual bool StreamingReset(bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//	//virtual bool QueryImageSize(int& nSize, BYTE lpXOR[4],bool& bSupported, int nTimeout=g_nTimeout); // only support in Speedup DL
//	virtual CString GetMd5FromList(const std::string& strImagePath, const std::string& strMd5ListPath);
//
//
///************************************************************************/
///*                    Callback related function                         */
///************************************************************************/
//public:
//	void AddMsg(const char* szMsg, int nStep, int nProgress); //Fire function
//	int m_nProgress; // Keep Original Progress data
//	std::string m_strMsg; // Keep Original Message
//
//	void AddMsg(bool bResult, const char* szTitle, int nStep); // Notify successful or failed in the end of command 
//	void AddMsg(BYTE nType/*ImageType*/, int nStep); // only to title what Image before Open/Write/Close Image
//public:// CNotifyBase
//	virtual bool Register(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent);
//	virtual bool Register(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent);
//	virtual bool Unregister(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent);
//	virtual bool Unregister(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent);
//	virtual void Fire(const std::string& strEvent,long nParam);
//	virtual void DefineNotify(const std::string& strNewEvent);
//
//	/************************************************************************/
//	/*                   ErrorCode related function                         */
//	/************************************************************************/
//public:
//	void SetLastError(DL_ERROR nError, bool bExecuteResult = true);
//};
//
///************************************************************************
//Class: CDLCompoundCommand
//Purpose: Combination of DLCommand
//************************************************************************/
//class QISDA_DOWNLOAD_CLASS_API CDLCompoundCommand : public CDLCommand
//{
//public:
//	CDLCompoundCommand(const TSTRING& strDevice, const std::string& strPlatform = "Qualcomm6245");
//	CDLCompoundCommand(int nCOMPort, const std::string& strPlatform = "Qualcomm6245");
//	virtual ~CDLCompoundCommand();
//
//
//
//public: // Streaming DL
//	virtual bool StreamingOpenAndWritePartition(const std::string& strPartition, bool bOverride, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//
//	virtual bool StreamingOpenHeaderImage(BYTE nType, const std::string& strHeaderImagePath/*MaxImageSize=512*/, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//	virtual bool StreamingOpenHeaderImage7K(BYTE nType, const std::string& strHeaderImagePath/*MaxImageSize=512*/, const std::string& strImagePath, const std::string& strMd5ListPath, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//	virtual bool StreamingOpenHeaderImage(BYTE nType, const BYTE* pImageData, int nImageSize/*MaxImageSize=512*/, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
//	virtual bool StreamingWriteMultiImage(const std::string& strImagePath, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout = g_nTimeout); // use StreamWrite with return XOR[4]
//	virtual bool StreamingWriteMultiImage(const BYTE* pImageData, int nImageSize, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout = g_nTimeout);
//
//	//virtual bool StreamingBufferMultiImage7K(const std::string& strImagePath, const std::string& strMd5ListPath, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout = g_nTimeout); // use StreamWrite with return XOR[4]
//	//virtual bool StreamingBufferWriteMultiImage7K(const std::string& strImagePath, const std::string& strMd5ListPath, int nImageSize, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout = g_nTimeout); // use StreamWrite with return XOR[4]
//
//	virtual bool StreamingDLMultiImage(BYTE nImageType, const std::string& strHeaderImagePath, const std::string& strImagePath, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nOpenTimeout=g_nTimeout, int nWriteTimeout=g_nTimeout, int nCloseTimeout=g_nTimeout); //Open(Header)+Write(Image)+Close
//	virtual bool StreamingDLMultiImage(BYTE nImageType, const BYTE* pHeaderData, int nHeaderSize, const BYTE* pImageData, int nImageSize, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nOpenTimeout=g_nTimeout, int nWriteTimeout=g_nTimeout, int nCloseTimeout=g_nTimeout); //Open(Header)+Write(Image)+Close
//	virtual bool StreamingDLMultiImage(BYTE nImageType, const std::string& strImagePath, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nOpenTimeout=g_nTimeout, int nWriteTimeout=g_nTimeout, int nCloseTimeout=g_nTimeout); //Open+Write(Image)+Close
//	virtual bool StreamingDLMultiImage(BYTE nImageType, const BYTE* pImageData, int nImageSize, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nOpenTimeout=g_nTimeout, int nWriteTimeout=g_nTimeout, int nCloseTimeout=g_nTimeout); //Open+Write(Image)+Close
//
//	//virtual bool StreamingDLMultiImage7K(BYTE nImageType, const std::string& strHeaderImagePath, const std::string& strImagePath, const std::string& strMd5ListPath, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nOpenTimeout=g_nTimeout, int nWriteTimeout=g_nTimeout, int nCloseTimeout=g_nTimeout); //Open(Header)+Write(Image)+Close
//protected:
//	bool CompareImageSize(bool bSpeedUp, int nSize, BYTE lpXOR[4], int nTimeout=g_nTimeout); //use QueryImageSize
//};
//
//
//
///************************************************************************
// Class: CDLFormatCommand
// Purpose: independent of DMSS/Streaming DLCommand/DLCompoundCommand
//************************************************************************/
//class QISDA_DOWNLOAD_CLASS_API CDLFormatCommand: public CDLCompoundCommand
//{
//public:
//	CDLFormatCommand(const TSTRING& strDevice, const std::string& strPlatform = "Qualcomm6245");
//	CDLFormatCommand(int nCOMPort, const std::string& strPlatform = "Qualcomm6245");
//	virtual ~CDLFormatCommand();
//public:
//	//bool CheckBadBlock(std::string& strBadBlockInfo, int nOption=0, int nTimeout=60000); // for Driver Test
//	typedef void (*CallBackInfo)(const std::string&);
//	//bool CheckBadBlock(const char* szCmdInput, int nCmdLength, int nTimeout, CallBackInfo CallBack); // for Driver Test
//	//bool CheckBadBlockWithHandShaking(bool& bStopAccepting, const char* szData, int nDataSize,int nTimeout,CallBackInfo Callback); // for Driver Test
//	//bool DumpImage(bool& bStopAccepting, const char* szData, int nDataSize,int nTimeout, CBufferPtr ptImage);  // for Driver Test
//
//	//bool RAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC);
//	bool GetDLBootLoaderRevocation(int& nStatus,int& nVersion);
//	//bool GetDLPhoneCodeRevocation(int& nStatus,int& nVersion);
//	bool DLGetHWVariant(int& nHWID, int& nVariantID, int nTimeout=g_nTimeout); // ONLY support Lavernock
//    
//	//////////////////////////////////////////////////////////////////////////
//	virtual bool Download(bool bSpeedup=true, bool bReopenAfterReset=false/*false=not to do this step*/, DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD);
//	virtual bool UARTDownload(bool bSpeedup, bool bReopenAfterReset=false, DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD);
//	virtual void SetResetMode(bool bReset);
//	virtual bool SendHello(bool bSpeedUp = true);
//   // virtual void SetIsOveridePartition(bool bOverridePartition);
//   // virtual bool StreamingDLFromPartition(bool bSpeedup = true, bool bReopenAfterReset=false/*false=not to do this step*/); //bSpeedup must be the same as previous one
//protected:
//    bool SetImage(IMAGE& image, const BYTE* pData, const int& nSize) 
//	{
//		bool bRes = false;
//        if(pData)
//			bRes = image.copyFrom(pData,nSize);
//        return bRes;
//    };
//};
//


//

#pragma warning(pop)
#endif // __DL__H__