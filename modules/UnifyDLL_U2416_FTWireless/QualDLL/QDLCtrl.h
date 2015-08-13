#pragma once
#include "QualDLL_API.h"
#include "SerCOMLibrary.h"
#include "HexFile.h"

#define RD_HEX_2_FASTBOOT	0x7766aa0f 
#define NONE_MODE			0x77665501 //nothing, so reboot normal

typedef int (*MsgCallBack)(const char* szInfo, int nStatus);

class CQDLCtrl
{
public:
	CQDLCtrl();
	~CQDLCtrl();

private:
	MsgCallBack m_fpSendMsg;
	void SendMsg(const char* szInfo, int nStatus);
public:
	void SetSendMsg(LPVOID pfunc);

private:
	CSerCOMLibrary m_SerCom;
	CString m_ComPort;
	int ComOpen(bool isEmdl = false);
	int ComClose();
public:
	int ComSet(char* szPort);

private:
	char m_szOsblVersion[64];
	int CmdDiagReadVersion(char* OsblVersion);
	int CmdDlSetFlag(DWORD nAddress, char* flag, int nLength);
	int CmdDiagReset();
	int CmdDlHexReset();
public:
	int ProbeEmdl();
	int Nop();
	int GetVersion(char* OsblVersion);
	int SetFastbootSerial(char* szSerial);
	int	SetFastbootFlag();
	int	SetFastbootFlag7k(bool IsSetFastbootFlag);
	int	SetFastbootFlag8k(bool IsSetFastbootFlag);
	int	Reset(char* pthHex);

private:
	int WaitForMultiThread(long* lCount, long lWaitAllCount, long dwMilliseconds);
public:
	int	SwitchDiag2Dload();

	int UploadMultiImageEmergency(char* pthHex, char* pthPartition,
			char* pthDbl, char* pthFsbl, char* pthOsbl, bool IsRePartition);

	int UploadMultiImage(char* pthHex, char* pthPartition,
			char* pthDbl,  char* pthFsbl, char* pthOsbl,
			char* pthAmss, char* pthDsp1, char* pthEfs2,
			char* pthAppsbl, char* pthApps, char* pthFlash,
			bool IsEraseEFS2, bool IsRePartition, SYNCHRONIZATION_t* sync);

	int	UploadMultiImageSpeedup(char* pthHex, char* pthPartition,
			char* pthDbl, char* pthFsbl, char* pthOsbl,
			char* pthAmss,   char* pthDsp1, char* pthEfs2,
			char* pthAppsbl, char* pthApps, char* pthFlash, char* pthDt,
			char* pthBkFsbl, char* pthBkOsbl, char* pthBkDt, char* pthBkAppsbl, char* pthReserved,
			bool IsEraseEFS2, bool IsRePartition, SYNCHRONIZATION_t* sync);

	int UploadMultiImage7k(char* pthHex, char* pthPartition,
			char* pthQcsblHd, char* pthQcsbl, char* pthOemsblHd, char* pthOemsbl,
			char* pthAmssHd,  char* pthAmss,  char* pthAppsblHd, char* pthAppsbl,
			char* pthAppsHd,  char* pthApps,  bool IsRePartition, 
			bool IsSetFastbootFlag, bool IsEraseEFS2);

	int UploadMultiImage7kEmergency(char* pthHex, char* pthPartition,
			char* pthQcsblHd, char* pthQcsbl, char* pthOemsblHd, char* pthOemsbl,
			char* pthAmssHd,  char* pthAmss,  char* pthAppsblHd, char* pthAppsbl,
			char* pthAppsHd,  char* pthApps,  bool IsRePartition, 
			bool IsSetFastbootFlag, bool IsEraseEFS2);

	int UploadMultiImage8kEmergency(char* pthHex, char* pthFastboot, bool IsSetFastbootFlag, bool IsEraseEFS2);
	int UploadMultiImage8kEmergencyAndIRAMRead(char* pthHex, char* pthFastboot,	bool IsSetFastbootFlag, bool IsEraseEFS2);

private:
	int DloadOpen();
	int DloadOpen8K();
	int DloadOpenRetry();
	int SwitchToDload();
	int RunHex(const char* path);
	int RunHexEmergency(const char* path);
	int UploadPartition(char* path, unsigned char bOverride);
	int UploadALLImage(bool IsRePartition, // IsRePartition to check restore
			char* pthDbl, char* pthFsbl, char* pthOsbl,
			char* pthAmss, char* pthDsp1, char* pthEfs2,
			char* pthAppsbl, char* pthApps, char* pthFlash);
	int UploadImage(unsigned char ImageType, char* pthImage, char* Part);
	int UploadFile(char* pthImage);
	int IsFileExist(const char * path);

	//// 20081015 Nek.Wang for DT DL & Speedup
	int UploadALLImageToDDR(char* pthDbl, char* pthFsbl, char* pthOsbl,
			char* pthAmss, char* pthDsp1, char* pthEfs2,
			char* pthAppsbl, char* pthApps, char* pthFlash, char* pthDt,
			char* pthBkFsbl, char* pthBkOsbl, char* pthBkDt, char* pthBkAppsbl, char* pthReserved);
	int UploadImageToDDR(unsigned char ImageType, char* pthImage, char* Part);
	int CmdUploadImageToDDR(unsigned char ImageType, char* pthImage);
	int CmdDlImageDdrChecksum(unsigned char ImageType, char* pthImage);
	int UploadALLImageSpeedup(char* pthDbl, char* pthFsbl, char* pthOsbl,
		char* pthAmss, char* pthDsp1, char* pthEfs2,
		char* pthAppsbl, char* pthApps, char* pthFlash, char* pthDt,
		char* pthBkFsbl, char* pthBkOsbl, char* pthBkDt, char* pthBkAppsbl, char* pthReserved);
	int UploadImageSpeedup(unsigned char ImageType, char* pthImage, char* Part);
	int CmdDlHexWriteImageToNAND(unsigned char ImageType, char* pthImage);

	// 2010-07-16 Mick: RESERVED change to 14 blocks
	// Austin: 14 + n(spare) = 22
	// Toucan: 14 + 2(spare) = 20
	enum RestoreSize {
		RESTORE_SIZE_RESERVED = 14*128*1024, // 14 blocks = 14 * 128k
		RESTORE_SIZE_SIMSECURE = 4*128*1024, // 4 blocks = 4 * 128k, all is 6 blocks (spare is 2)
	};

	int UploadALLImage7k(char* pthQcsblHd, char* pthQcsbL,
			char* pthOemsblHd, char* pthOemsbl, char* pthAmssHd, char* pthAmss,
			char* pthAppsblHd, char* pthAppsbl, char* pthAppsHd, char* pthApps);
	int UploadImage7k(unsigned char Type, char* pthImage, char* pthHeader, char* Part);

	// command packet
	// diag packet
	bool m_isSecureBoot;
	int CmdDiagSwitchToDlad();
	int CmdDiagSetSPC(char* szSPC);
	int CmdDiagProbe();
	int CmdDiagProbeEmdl();
	// dload packet
	int CmdDlNop();
	int CmdDlParameter();
	int CmdDlUploadHex(byte* pData, WORD nLength, DWORD wAddress);
	int CmdDlGo(WORD nSegment, WORD nOffset);
	int CmdDlHexHello();
	int CmdDlHexSecurity(byte nMode);
	int CmdDlHexUploadPartition(byte* pData, WORD nLength, unsigned char bOverride);
	int CmdSetFastbootFlag(byte* pData, WORD nLength, unsigned char nPair);
	int CmdDlHexOpenImage(unsigned char ImageType);
	int CmdDlHexOpenImage7k(unsigned char ImageType, unsigned char* szHdData, int nHdSize);
	int CmdDlHexUploadFile(byte* pData, DWORD nLength, DWORD wAddress);
	int CmdDlHexCloseImage();
	int CmdDlStreamingStrRead(BYTE* lpData);
	int CmdDlStreamingIRAMRead(BYTE* lpData, WORD wnLength, DWORD dwAddress);

private:
	int EraseEFS2();
public:
	int ErasePartitionEFS2(char* pthHex, char* pthPartition);

private:
	//int UploadBRT(char* path);
	//int DownloadBRT(char* path);
public:
	//int BRTRestore(char* pthBrt, char* szSPC);
	//int BRTBackup(char* pthBrt, char* szSPC);

private:
	int DiagCmdNvWrite(unsigned short nItemID, int nLen, unsigned char* pData,
			unsigned short* Status);
	int DiagCmdNvRead(unsigned short nItemID, int nLen, unsigned char* pData,
			unsigned short* Status);
public:
	int NvWrite(unsigned short ItemID, int nDataLength, byte* pData,
			char* szSPC, unsigned short* Status);
	int NvRead(unsigned short ItemID, int nDataLength, byte* pData,
			char* szSPC, unsigned short* Status);
};
