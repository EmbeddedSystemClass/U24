/***************************************************************************************
* Copyright (C) : QTH/WR24                                      
* Module name	: 7227A Download   
* File name     : Download7227A.h             
* Summary       : 7227A Download (Hex DL(Qualcomm images) + Fastboot DL(Android images))                                                             
* Version     Author            Date                 Abstract                      
* 1.0       Evonne.Hsieh      2012/02/10           First version  
* 2.0       Alex.Chen         2012/04/22           1.Import new ErrorCode.
                                                   2.Modify code rule.
****************************************************************************************/

#pragma once

#include "Download.h"
#include "ADBDevice.h"
#include "../../Modules/QisdaNVModule/NVItem.h"
#include "../../Modules/Common/ErrorDefine/ErrorDefine.h"

#pragma warning(disable:4996)
#pragma warning(disable:4251)
#pragma warning(disable:4482)

/*============================ Const ==================================*/
/* Timeout */
const int g_i_7227ANopRetryTimes		   	 = 100;
const int g_i_7227ANopTimeout			   	 = 1000;
const int g_i_7227AHelloRetryTimes	   		 = 200;
const int g_i_7227AUploadPtnRetryTimes	   	 = 100;
const int g_i_7227AUploadPtnTimeout		   	 = 1000;
const int g_i_7227AHelloTimeout		   		 = 2000;
const int g_i_7227AReOpenComPortAfterGo		 = 1000;
const int g_i_7227AReOpenComPortAfterReset	 = 4000;
const int g_i_7227ACloseImageTimeout		 = 300000;

/* Const */
#define CHECKSUM_MODEM_FILE_NAME        _T("Modem_Image_Cksum.txt")
#define CHECKSUM_ANDROID_FILE_NAME      _T("Android_Image_Cksum.txt")
#define DOWNLOAD_MODE_PREDL             _T("PreDL")
#define DOWNLOAD_MODE_REDL              _T("ReDL")
#define FALENGTH                        512
#define FAREADLENGTH                    2*1024*1024
#define NVPARTITION_1_LENGTH            1152 / 2 * 1024
#define NVPARTITION_2_LENGTH            128 /2 * 1024
/*============================ Const ==================================*/

/*============================ Structrue ==============================*/
/* Image file partition type */
#pragma pack (1)
typedef enum IMAGE_TYPE_7K
{
	Q7K_HEX				 = 1,	// ENPRG7627A.hex / NPRG7627A.hex
	Q7K_PARTITIONBIN	 = 2,	// PARTITIONBIN		
	Q7K_QCSBLHD			 = 3,	// QCSBLHD
	Q7K_QCSBL			 = 4,	// QCSBL
	Q7K_OEMSBLHD         = 5,	// OEMSBLHD
	Q7K_OEMSBL			 = 6,	// OEMSBL
	Q7K_AMSSHD			 = 7,	// AMSSHD
	Q7K_AMSS			 = 8,	// AMSS
	Q7K_APPSBOOTHD       = 9,	// APPSBOOTHD
	Q7K_APPSBOOT		 = 10,	// APPSBOOT
	Q7K_BOOT			 = 11,	// boot.img
	Q7K_SYSTEM			 = 12,	// system.img
	Q7K_USERDATA		 = 13,	// userdata.img
	Q7K_FLEX			 = 14,	// flex.img
	Q7K_RECOVERY		 = 15,	// recovery.img
	Q7K_LOGFILTER		 = 16,	// logfilter.img
	Q7K_PASSPORT		 = 17,	// passport.txt
	Q7K_WIFIBIN			 = 18,	// wifi bin
	Q7K_MD5LIST			 = 19	// md5 list
} IMAGE_TYPE_7227A;
#pragma pack()

/* Emmc download partition type */
#pragma pack (1)
typedef enum HEX_COMMAND_TYPE_7K
{
	OPEN_MODE_NONE       = 0x00, // Not opened yet
	OPEN_MODE_PBL        = 0x01, // Primary Boot Loader
	OPEN_MODE_QCSBLHDCFG = 0x02, // QC 2ndary Boot Loader Header and
	OPEN_MODE_QCSBL      = 0x03, // QC 2ndary Boot Loader
	OPEN_MODE_OEMSBL     = 0x04, // OEM 2ndary Boot Loader
	OPEN_MODE_AMSS       = 0x05, // AMSS modem executable
	OPEN_MODE_APPS       = 0x06, // APPS executable
	OPEN_MODE_OBL        = 0x07, // OTP Boot Loader
	OPEN_MODE_FOTAUI     = 0x08, // FOTA UI binarh
	OPEN_MODE_CEFS       = 0x09, // Modem CEFS image
	OPEN_MODE_APPSBL     = 0x0A, // APPS Boot Loader
	OPEN_MODE_APPS_CEFS  = 0x0B, // APPS CEFS image
	OPEN_MODE_FLASH_BIN  = 0x0C, // Flash.bin image for Windows mobile
	OPEN_MODE_DSP1       = 0x0D, // DSP1 runtime image
	OPEN_MODE_CUSTOM     = 0x0E, // Image for user defined partition
	OPEN_MODE_DBL        = 0x0F, // DBL Image for SB Architecture 2.0
	OPEN_MODE_OSBL       = 0x10, // OSBL Image for SB Architecture 2.0
	OPEN_MODE_FSBL       = 0x11, // FSBL Image for SB Architecture 2.0
	OPEN_MODE_DSP2       = 0x12, // DSP3 executable
	OPEN_MODE_DT		 = 0x15, // QTH DT
	OPEN_MODE_RESERVED   = 0x16, // Austin RESERVED for backup NV
	OPEN_MODE_SIMSECURE  = 0x17, // Austin backup for SIMSECURE
	OPEN_MODE_BAK_DT     = 0x18, // backup DT image
	OPEN_MODE_BAK_FSBL   = 0x19, // backup FSBL image
	OPEN_MODE_BAK_OSBL   = 0x1A, // backup OSBL image
	OPEN_MODE_BAK_APPSBL = 0x1B  // backup APPSBL image
} HEX_COMMAND_TYPE_7227A;
#pragma pack()
/*============================ Structrue ==============================*/

class QISDA_DOWNLOAD_CLASS_API CDownload7227A : public CDLFormatCommand, public CRefObject
{
public:
	CDownload7227A(const TSTRING& str_deviceName, const std::string& str_platform = "Qualcomm7227A");
	CDownload7227A(int i_COMPort, CString str_multiDLFlag,const std::string& str_platform = "Qualcomm7227A");
	virtual ~CDownload7227A(void);

public:
	/* 7227A Only support b_speedUp(false) and QTT_FASTDOWNLOAD, b_speedUp relate to Hex(QTH Modify Hex to Qualcomm) */
	bool MultiDownload(bool b_speedUp = false, bool b_reOpenAfterReset = false, DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD);
	bool SetImageFilePath(const int i_imageType, const std::string& str_filePath);
	CString GetErrorCode(void);
	void SetDLMode(CString str_DLMode) {m_str_DLMode = str_DLMode;};
	void SetReworkFlag(int i_rework)   {m_i_rework = i_rework;};
	void SetRebootFlag(int i_reboot)   {m_i_reboot = i_reboot;};
	void SetDDRCheckFlag(int i_DDRCheck)   {m_i_DDRCheck = i_DDRCheck;};
	void SetCheckSumFlag(int i_checkSum)   {m_i_checkSum = i_checkSum;};
	void SetBackupNVFlag(int i_backupNV)   {m_i_backupNV = i_backupNV;};
	void SetSupportQDownloadFlag(int i_SupportQDownload)   {m_i_SupportQDownload = i_SupportQDownload;};
	void SetEraseUserFatFlag(int i_EraseUserFatFlag)   {m_i_eraseUserFat = i_EraseUserFatFlag;};
	void SetImagePath(CString str_filePath) {m_str_imagePath = str_filePath;};
	void SetCOMPicasso(int i_COMPort, CString str_picasso) {m_map_COMPicasso[i_COMPort] = str_picasso;};

private:
	CString  m_str_multiDLFlag;		   // Multi-download flag	
	CString  m_str_errorCode;
	CString  m_str_DLMode;             // DL Mode
	CString  m_str_COMPort;
	int      m_i_rework;               // Rework flag
	int      m_i_reboot;               // Reboot flag
	int      m_i_DDRCheck;             // DDR check flag
	int      m_i_checkSum;             // CheckSum flag
	int      m_i_backupNV;             // Backup NV flag
	int      m_i_SupportQDownload;     // Support QDownload
	int      m_i_eraseUserFat;         // Erase User Fat
	CString  m_str_imagePath;          // Image file path
	std::map<int, CString> m_map_COMPicasso;

/*======================== Qualcomm Download ================================================================================*/
private:
	bool Download(bool b_speedUp = false, bool b_reOpenAfterReset = false, DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD);
	int  GetComport(void);
	void SetIsOveridePartition(bool b_overridePartition);
	void SetResetMode(bool b_reset);

private:
	bool DMSSDL(void);
	bool DMSSDLReDL(void);
	bool StreamingDL(bool b_speedUp = false, DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD);
	bool SetFastBootFlag(void);
	bool DDRTest(void);
	bool ReadDDRTestReslult(void);

private:
	std::string  m_str_hex;
	std::string  m_str_msimage;

private:
	bool m_b_reOpenAfterReset;      // false: After reset, handset enter fastboot mode(not exist COM port)
	bool m_b_resetMode;             // true: Init to NONE mode
	bool m_b_overridePartition;     // false: Emmc partition(.bin)
/*======================== Qualcomm Download ================================================================================*/

/*======================== Android Download =================================================================================*/
public:
	bool ReadFA(char* sz_FAData);
	bool WriteFA(char* sz_FAData);
	bool ReadFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	bool WriteFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	bool Reboot(void);
	bool EraseSBL1(void);
	CString m_toolPth;
	CString GetFactoryVersion(void) {return m_str_factoryLoadVersion;};

private:
	bool FastbootDownload(void);
	bool FastbootFlashCmd(const std::string& str_parameter);
	bool ADBFlash(const char* sz_parmeter, const void* p_data, unsigned long long l_fileLen);
	void* LoadFile(const char* sz_fileName, unsigned long long* i_fileSize);
	ADBDevice* GetADBDevice(const char* sz_comPort);
	bool GetADB(void);
	bool WriteCheckSum(CString str_imageFilePathName);
	bool SetFTDMode(void);
	bool ReadCheckSumFile(void);
	bool BackupNVPartition(void);
	bool SpiltString(CString str_sourceString, CString str_delimiter, CStringArray& stra_strArg);
	bool ADBFlashAll(const char* sz_parmeter, const char* sz_fileName);
	bool EraseUserFat(void);
	bool ExecuteFastboot(char* sz_command);
	bool ExecuteFastbootErase(char* sz_command);
	bool FastbootGetUnlock(void);
	bool FastbootCheckEcc(void);
	bool FastbootWriteWifibin(void);
	bool ExecFastboot(CString& Command, bool hasResponse);
	bool Exec(CString& path, CString& param, DWORD msTimeout, bool hasResponse);
	bool IsPathExist(const CString& path);

	bool GetFactoryLoadVersion(void);
	CString m_str_factoryLoadVersion;

private:
	std::string  m_str_partitionBin;
	std::string  m_str_qcsblhd;
	std::string  m_str_qcsbl;
	std::string  m_str_oemsblhd;
	std::string  m_str_oemsbl;
	std::string  m_str_amsshd;
	std::string  m_str_amss;
	std::string  m_str_appsboothd;
	std::string  m_str_appsboot;
	std::string  m_str_boot;
	std::string  m_str_system;
	std::string  m_str_userdata;
	std::string  m_str_flex;
	std::string  m_str_recovery;
	std::string  m_str_logfilter;
	std::string  m_str_passport;
	std::string  m_str_wifi_bin;
	std::string  m_str_md5_list;

private:
	ADBDevice*        m_p_adbDevice;                     // Get ADB device
	CCriticalSection  m_obj_loadADBFileCritSection;      // Thread sync
	CString           m_str_imageFilePath;               // Load path
	std::map<CString, CString> m_map_imageNameCheckSum;
	static CCriticalSection  m_obj_loadcheckSumCritSection;      // Thread sync
/*======================== Android Download =================================================================================*/
};

