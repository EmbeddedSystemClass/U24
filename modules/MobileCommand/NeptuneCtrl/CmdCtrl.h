// CmdCtrl.h: interface for the CmdCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CmdCtrl_H__7C100BB3_1FAE_4BE3_A1C2_531317A625A5__INCLUDED_)
#define AFX_CmdCtrl_H__7C100BB3_1FAE_4BE3_A1C2_531317A625A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cstddef> // size_t
#include "NeptuneGeneralController.h" // public CNeptuneGeneralController
#include "comdriver.h"
#include "usbdriver.h"
#include "tcpdriver.h"
#include "serialdriver.h"
#include "dl_filehandler.h"


#define READ_BIT(A,B)              ((A & (1 << B)) ? 1 : 0)
#define CLEAR_BIT(A,B)             (A &= (~(1 << B)))
#define SET_BIT(A,B)               (A |= (1 << B))

static const int PAYLOAD_SIZE = 4096;

//CALLBACK FUNCTION FOR PROGRESS
typedef struct CameraImageTag {
	CString		csFormat;
	long		lResolutionX;
	long		lResolutionY;
	long		lSize;
} S_Image;

typedef int (*P21_CALLBACK) (int nPort , int Progress);
typedef int (*P21_CALLBACKCHAR) (int nPort , char* Data);

class CmdCtrl : public CNeptuneGeneralController
{
public:
	CmdCtrl();
	virtual ~CmdCtrl();

	char m_UIInfo[128];
	bool m_bIfUSBCmd;
	bool m_isGeneralCmd;
	bool m_isOpen;
	BaseDrv* m_pCommDrv;
	int open(int nPort, int Timeout);
	int close(int nPort, int Timeout);
	int ExactReadDevice(int nPort, int Timeout, char* InBuffer, int InSize);
	int ExactWriteDevice(int nPort, int Timeout, char* InBuffer, int InSize);
	
	virtual int InitDevice(int nPort);
	virtual int FreeDevice(int nPort);

	/* 111020 Gilpin add parameter of in buffer size transfer to solve the problem of data lose */
	virtual int GeneralCmd(int nPort, int Timeout, char* CtrlBuffer);
	virtual int GeneralCmdOut(int nPort, int Timeout, char* CtrlBuffer, char* OutBuffer) ;
	virtual int GeneralCmdIn(int nPort, int Timeout, char* CtrlBuffer, char* InBuffer, size_t nInLength);
	virtual int GeneralCmdInOut(int nPort, int Timeout, char* CtrlBuffer, char* InBuffer, size_t i_InLength, char* OutBuffer);
	
	//110420 gilpin add for receive serial command such as touch panel pixel and key value 
    virtual int ReceiveCmdOut(int nPort, int Timeout, char* CtrlBuffer, char* OutBuffer);

	int ExactBDTLSectorUploadCmd(int nPort, int Timeout, char* CmdNumber, char* FilePath, char* parameter);
	int BDTLSectorUpload(int nPort, int Timeout, char* CmdNumber, char* FilePath, char* parameter);
	int ExactBDTLSectorDownloadCmd(int nPort, int Timeout, char* CmdNumber, char* FilePath, char* parameter);
	int BDTLSectorDownload(int nPort, int Timeout, char* CmdNumber, char* FilePath, char* parameter);

	int ConvertDTImageData(int iRow, int iColum, unsigned char* source, unsigned char* output);
	BOOL SaveBMP(int iRow, int iColum, unsigned char* SourceData, char* FileName);
	long ConvertImageToBmpFile(S_Image udtImage, byte *pbytSource, byte *pbytOutput, int iRow, int iColum);

	bool m_bP21LPR2PhaseOne;
	int ExactGeneralCmdInOut(int nPort, int Timeout, char* CtrlBuffer, char* InBuffer, size_t i_InLength, char* OutBuffer);
	int NormalCmdPowerOnToUARTPacketMode(int nPort, int TimeOut, int DTRFlag);

	int EnableDTR(int nPort, int TimeOut);
	int DisableDTR(int nPort, int TimeOut);

	int ComDtrCtrl(int nPort, int TimeOut, bool isEnable);
	int ComRtsCtrl(int nPort, int TimeOut, bool isEnable);
	int ComTxCtrl(int nPort, int TimeOut, byte* pBuffer, size_t nLength);

	virtual int UnregisterCallBack(void* farCallBack);
	virtual int RegisterCallBack(void* farCallBack);
	int UnregisterCallBackChar(void* farCallBack);
	int RegisterCallBackChar(void* farCallBack);

	int UARTTILoadFile(int nPort, int Timeout, char* DDRFileName , unsigned int StartAddr);
	int UARTLoad2nd(int nPort, int Timeout, char* ndFilePath);

	//The work buffer would be a data buffer or a file path. It just depends on implementation.
	virtual int GeneralDownload(int nPort, int Timeout, char* DLType, char* WorkBuffer) ;
	//The work buffer would be a data buffer or a file path. It just depends on implementation.
	virtual int GeneralUpload(int nPort, int Timeout, char* CtrlBuffer, char* WorkBuffer) ;

	int ExactUSBTIJumpAddr(unsigned int StartAddr);
	int ExactGeneralCmd(int nPort, int Timeout, char* CtrlCmd);
	int ExactGeneralCmdOut(int nPort, int Timeout, char* CtrlCmd, char* OutBuffer) ;
	int ExactGeneralCmdIn(int nPort, int Timeout, char* CtrlCmd, char* InBuffer, size_t nInLength);
	//110420 gilpin add for receive serial command such as touch panel pixel and key value 
	int ExactReceiveCmdOut(int nPort, int Timeout, char* CtrlCmd, char* OutBuffer) ;

	int ExactUSBGenUploadCmd(int nPort, int Timeout, char* CtrlBuffer, char* WorkBuffer);
	int ExactUSBGenDownloadCmd(int nPort, int TimeOut, char* DLFileType, char* FileName);
	// 2006-11-15 YTT for sending FB 2nd command
	int USB2ndSendCommand(int nPort, int Timeout, char* MenuCommand);
	int USBTILoadFile(int nPort, int Timeout, char* DDRFileName, unsigned int StartAddr);
	int USBLoad2nd(int nPort, int Timeout, char* ndFilePath);
	int ExactUSBTIJumpAddrEx(int nPort, int Timeout, unsigned int StartAddr, char* workPath);
	// 2007-05-14 YTT for download info
	int SetUIInfo(int nPort, char* UIInfo);
	int HexToAscii(char* Input, int Size);
	int AsciiToHex(char* InOut, int Size);
	// 2008-02-18 Mick
	int GetFile2Buffer(const char* filepath, char* buffer, int length);
	int GetExternalOSVer(char* Inputfilepath, char* cOSVer, int iOSVerlen);
	//// 2007-12-29 Nek
	CString GetExternalVersion(char* WorkBuffer, CString cstrTempFileName, int flag);
	int DownloadInfoFileSize(char* FileName, char* szFileSize);
	//// 2007-12-29 Nek
	int DDRDump(int nPort, int Timeout);
	int WriteDownloadInfo(int nPort, int Timeout, char* DLMethod, char* DLType, char* DLFilename, char* DLFileSize, int DLFlag);
	int PacketResultCheck(int iPacketResult, unsigned char * pPackData);
	int FlashDumpBDTL( int nPort, int Timeout, char* InBuffer, int BDTL);
	int MTKGeneralCmdInOut(int nPort, int nTimeout,int nBodySize, unsigned char* ReqBody,int nPeerBufferSize,unsigned char* PeerBuffer,unsigned char* OutConfirm, unsigned char * OutPeerData);
private:
	P21_CALLBACK m_farCallBack;
	// 2006-11-15 YTT
	P21_CALLBACKCHAR m_farCallBackChar;
	//I wrote JumpAddress code for convenience  , but it may confuse , need more evaluate
	int UARTGenPacketSend(BaseDrv *pCom, int Function, unsigned char* FileSize, unsigned char* pPackData, unsigned char* OChecksum, int DataSize);
	int UARTGenPacketRecv(BaseDrv* pCom, int Function, unsigned char* SectorNumber, unsigned char* pReceiveData, unsigned char* NChecksum, int DataSize);
	int UARTPutHugeData(BaseDrv *pCom, int PortNum, unsigned char* pPackData, int Length, int TimeOut);

	int QthGenPacketSend(int Type, int PayLoadSize, unsigned char* pPackData, unsigned char* OChecksum, int nPort);
	int QthGenPacketRecv(int type, unsigned char* PayloadCommand, unsigned char* pReceiveData, unsigned char* NChecksum);
	int QthGenCommand(int PayloadSize, unsigned char* Command, unsigned char* PayloadData, unsigned char* OutputData, int nPort);
	//110420 gilpin add for receive serial command such as touch panel pixel and key value 
	int RecvCommand(int PayloadSize, unsigned char* Command, unsigned char* PayloadData, unsigned char* OutputData, int nPort);

	int MTKGenCommand(int nBodySize, unsigned char* ReqBody,int nPeerBufferSize,unsigned char* PeerBuffer,unsigned char* OutConfirm, unsigned char * OutPeerData);
	int MTKGenPacketSend(int Type,int nBodySize, unsigned char* ReqBody,int nPeerBufferSize,unsigned char* PeerBuffer);
	int MTKGenPacketRecv(unsigned char * PayloadCommand, unsigned char* pConfirmData, unsigned char * pPeerData, int nTimeout);
};

#endif // !defined(AFX_CmdCtrl_H__7C100BB3_1FAE_4BE3_A1C2_531317A625A5__INCLUDED_)
