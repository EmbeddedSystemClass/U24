/***************************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name	: 8260 Download   
* File name     : Download8260.h             
* Summary       : 8260 Download (Hex DL(Qualcomm images) + Fastboot DL(Android images))                                                             
* Version     Author            Date                 Abstract                      
* 1.0       Alex.Chen        2011/02/22           First version  
* 1.1       Alex.Chen        2011/03/03           Add: Hex only download msimage
* 1.2       Alex.Chen        2011/07/18           Add: ReDL function
****************************************************************************************/

#pragma once

#include "Download.h"
#include "ADBDevice.h"
#include "../Common/ErrorDefine/Common_ErrorDefine.h"
#include "../Common/ErrorDefine/DL_ErrorDefine.h"
#include "../Common/ErrorDefine/ErrorDefine.h"

#pragma warning(disable:4996)
#pragma warning(disable:4251)
#pragma warning(disable:4482)


/*============================ Define =================================*/
/* Error Code */
/*
#define DLErr_DL_Partition_Table_Fail                    "DL_Partition_Table_Fail"
#define DLErr_DL_Hex_Fail                         "DL_Hex_Fail"
#define DLErr_DL_MsImage_Fail                     "DL_MsImage_Fail"
#define DLErr_DL_SBL1_Fail                        "DL_SBL1_Fail"
#define DLErr_DL_SBL2_Fail                        "DL_SBL2_Fail"
#define DLErr_DL_SBL3_Fail                        "DL_SBL3_Fail"
#define DLErr_DL_RPM_Fail                         "DL_RPM_Fail"
#define DLErr_DL_TZ_Fail                          "DL_TZ_Fail"
#define DLErr_DL_Emmc_Appboot_Fail                     "DL_Emmc_Appboot_Fail"
#define DLErr_DL_Boot_Fail                        "DL_Boot_Fail"
#define DLErr_DL_System_Fail                      "DL_System_Fail"
#define DLErr_DL_Userdata_Fail                    "DL_Userdata_Fail"
#define DLErr_DL_Persist_Fail                     "DL_Persist_Fail"
#define DLErr_DL_Recovery_Fail                    "DL_Recovery_Fail"
#define DLErr_DL_Nonhlos_Fail                     "DL_Nonhlos_Fail"
#define DLErr_DL_Dmss_Nop_Fail                        "DL_Dmss_Nop_Fail"
#define DLErr_Set_Fastboot_Flag_Fail                "Set_Fastboot_Flag_Fail"
#define DLErr_Setting_Open_Fail                  "DL2034"
#define DLErr_DL_Streaming_Hello_Fail                          "DL_Streaming_Hello_Fail"
#define DLErr_DL_Streaming_Security_Fail                       "DL_Streaming_Security_Fail"
#define DLErr_DL_Streaming_Reset_Fail                  "DL_Streaming_Reset_Fail"
#define DLERR_REBOOTSETMODE                  "DL2038"
#define DLErr_DL_RC_Fail                          "DL_RC_Fail"
#define CommErr_Get_ADB_Fail                "DL2040"
#define DLErr_DL_Cache_Fail                       "DL_Cache_Fail"
#define DLErr_DL_Read_Checksum_File_Fail           "DL_Read_Checksum_File_Fail"
#define DLERR_DDR_CHECK_FAILURE              "DL2043"
#define DLERR_ERASE_USER_FAT                 "DL2044"
#define DLERR_DL_TOMBSTONES					 "DL2045"

#define DLERR_UNKNOWN                      "DL2099"
*/
/*============================ Define =================================*/

/*============================ Const ==================================*/
/* Timeout */
const int g_i_8260NopRetryTimes		   		 = 100;
const int g_i_8260NopTimeout			   	 = 1000;
const int g_i_8260HelloRetryTimes	   		 = 100;
const int g_i_8260HelloTimeout		   		 = 1000;
const int g_i_8260ReOpenComPortAfterGo		 = 1000;
const int g_i_8260ReOpenComPortAfterReset	 = 4000;
const int g_i_8260CloseImageTimeout			 = 300000;

#define CHECKSUM_MODEM_FILE_NAME        _T("Modem_Image_Cksum.txt")
#define CHECKSUM_ANDROID_FILE_NAME      _T("Android_Image_Cksum.txt")
#define DOWNLOAD_MODE_PREDL             _T("PreDL")
#define DOWNLOAD_MODE_REDL              _T("ReDL")
#define FALENGTH                        512
#define NVPARTITION_1_LENGTH            1152 / 2 * 1024
#define NVPARTITION_2_LENGTH            128 /2 * 1024
/*============================ Const ==================================*/

/*============================ Structrue ==============================*/
/* Image file partition type */
#pragma pack (1)
typedef enum IMAGE_TYPE
{
	HEX          = 1,          // MPRG8660.hex
	MSIMAGE      = 2,          // 8660_msimage.mbn
	PARTITIONBIN = 3,          // partition.bin
	SBL1         = 4,          // sbl1.mbn
	SBL2         = 5,          // sbl2.mbn
	SBL3         = 6,          // sbl3.mbn
	RPM          = 7,          // rpm.mbn
	TZ           = 8,          // tz.mbn
	APPBOOT      = 9,          // emmc_appsboot.mbn
	BOOT         = 10,         // boot.img
	SYSTEM       = 11,         // system.img.ext4
	USERDATA     = 12,         // userdata.img.ext4
	PERSIST      = 13,         // persist.img.ext4
	RECOVERY     = 14,         // recovery.img
	NONHLOS      = 15,         // NON-HLOS.bin
	CACHE        = 16,         // cache.ime.ext4
	RC           = 17,         // RC.img
	TOMBSTONES	 = 18		  //tombstones.img.ext4 
} IMAGE_TYPE_8260;
#pragma pack()

/* Emmc download partition type */
#pragma pack (1)
typedef enum HEX_COMMAND_TYPE
{
	OPEN_MULTI_MODE_EMMC_USER = 0x21
} HEX_COMMAND_TYPE_8260;
#pragma pack()
/*============================ Structrue ==============================*/

class QISDA_DOWNLOAD_CLASS_API CDownload8260 : public CDLFormatCommand, public CRefObject
{
public:
	CDownload8260(const TSTRING& str_deviceName, const std::string& str_platform = "Qualcomm8260");
	CDownload8260(int i_COMPort, const std::string& str_platform = "Qualcomm8260");
	virtual ~CDownload8260(void);

public:
	/* 8260 Only support b_speedUp(false) and QTT_FASTDOWNLOAD, b_speedUp relate to Hex(QTH Modify Hex to Qualcomm) */
	bool DMSSDL_ReDL(void);
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
	void SetCheckHWVersion(int i_CheckHWVersion)   {m_i_CheckHWVersion = i_CheckHWVersion;};
	
	void SetHexDLMode(CString str_DLMode) {m_str_HexDLMode = str_DLMode;};
	void SetHexImagePath(CString str_filePath) {m_str_HexImagePath = str_filePath;};

	bool CheckRounting() {return true;}
	bool FastbootDL_ReDL() {return true;}


private:
	CString  m_str_errorCode;
	CString  m_str_DLMode;             // DL Mode
	int      m_i_rework;               // Rework flag
	int      m_i_reboot;               // Reboot flag
	int      m_i_DDRCheck;             // DDR check flag
	int      m_i_checkSum;             // CheckSum flag
	int      m_i_backupNV;             // Backup NV flag
	int      m_i_SupportQDownload;     // Support QDownload
	int      m_i_eraseUserFat;         // Erase User Fat
	CString  m_str_imagePath;          // Image file path
	std::map<int, CString> m_map_COMPicasso;

	int		m_i_CheckHWVersion;			//Check HW Version
	CString  m_str_HexDLMode;             //Hex DL Mode
	CString  m_str_HexImagePath;          // Hex Image file path

/*======================== Qualcomm Download ================================================================================*/
private:
	bool Download(bool b_speedUp = false, bool b_reOpenAfterReset = false, DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD);
	int  GetComport(void);
	void SetIsOveridePartition(bool b_overridePartition);
	void SetResetMode(bool b_reset);

private:
	
	bool DMSSDL(void);
	bool StreamingDL(bool b_speedUp = false, DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD);
	bool SetFastBootFlag(void);
	bool SendOEM_CMD(void);
	bool SendReset_CMD(void);
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
	bool CheckHWVariant(void);


private:
	std::string  m_str_partitionBin;
	std::string  m_str_sbl1;
	std::string  m_str_sbl2;
	std::string  m_str_sbl3;
	std::string  m_str_rpm;
	std::string  m_str_tz;
	std::string  m_str_appboot;
	std::string  m_str_boot;
	std::string  m_str_system;
	std::string  m_str_userdata;
	std::string  m_str_persist;
	std::string  m_str_recovery;
	std::string  m_str_nonhlos;
	std::string  m_str_cache;
	std::string  m_str_rc;
	std::string  m_str_tombstones;

private:
	ADBDevice*        m_p_adbDevice;                     // Get ADB device
	CCriticalSection  m_obj_loadADBFileCritSection;      // Thread sync
	CString           m_str_imageFilePath;               // Load path
	std::map<CString, CString> m_map_imageNameCheckSum;
	static CCriticalSection  m_obj_loadcheckSumCritSection;      // Thread sync
	CString			  m_str_ImageHWVariant;
/*======================== Android Download =================================================================================*/
};

