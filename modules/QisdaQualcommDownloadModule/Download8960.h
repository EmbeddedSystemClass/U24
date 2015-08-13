/***************************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name	: 8260 Download   
* File name     : Download8960.h             
* Summary       : 8260 Download (Hex DL(Qualcomm images) + Fastboot DL(Android images))                                                             
* Version     Author            Date                 Abstract                      
* 1.0       Alex.Chen        2011/02/22           First version  
* 1.1       Alex.Chen        2011/03/03           Add: Hex only download msimage
* 1.2       Alex.Chen        2011/07/18           Add: ReDL function
****************************************************************************************/

#pragma once

#include "Download.h"
#include "ADBDevice.h"
#include "../Common/ErrorDefine/ErrorDefine.h"
#pragma warning(disable:4996)
#pragma warning(disable:4251)
#pragma warning(disable:4482)

/*============================ Define =================================*/
/* Error Code */
#define DLERR_DL_PATITION                    "DL2002"
#define DLERR_DL_HEX                         "DL2018"
#define DLERR_DL_MSIMAGE                     "DL2019"
#define DLERR_DL_SBL1                        "DL2020"
#define DLERR_DL_SBL2                        "DL2021"
#define DLERR_DL_SBL3                        "DL2022"
#define DLERR_DL_RPM                         "DL2023"
#define DLERR_DL_TZ                          "DL2024"
#define DLERR_DL_APPBOOT                     "DL2025"
#define DLERR_DL_BOOT                        "DL2026"
#define DLERR_DL_SYSTEM                      "DL2027"
#define DLERR_DL_USERDATA                    "DL2028"
#define DLERR_DL_PERSIST                     "DL2029"
#define DLERR_DL_RECOVERY                    "DL2030"
#define DLERR_DL_NONHLOS                     "DL2031"
#define DLERR_DMSSNOP                        "DL2032"
#define DLERR_SETFASTBOOTFLAG                "DL2033"
#define DLERR_REOPENAFTERGO                  "DL2034"
#define DLERR_HELLO                          "DL2035"
#define DLERR_SECURITY                       "DL2036"
#define DLERR_STEAMINGRESET                  "DL2037"
#define DLERR_REBOOTSETMODE                  "DL2038"
#define DLERR_DL_RC                          "DL2039"
#define DLERR_GET_ADB_FAILURE                "DL2040"
#define DLERR_DL_CACHE                       "DL2041"
#define DLERR_GET_CHECKSUM_FAILURE           "DL2042"
#define DLERR_DDR_CHECK_FAILURE              "DL2043"
#define DLERR_ERASE_USER_FAT                 "DL2044"
#define DLERR_OEM_ALL_QON					 "DL2045"
#define DLERR_REBOOT						 "DL2046"

#define DLERR_DL_UNKWON                      "DL2099"
/*============================ Define =================================*/

/*============================ Const ==================================*/
/* Timeout */
const int g_i_8960NopRetryTimes		   		 = 100;
const int g_i_8960NopTimeout			   	 = 1000;
const int g_i_8960HelloRetryTimes	   		 = 100;
const int g_i_8960HelloTimeout		   		 = 1000;
const int g_i_8960ReOpenComPortAfterGo		 = 1000;
const int g_i_8960ReOpenComPortAfterReset	 = 4000;
const int g_i_8960CloseImageTimeout			 = 300000;

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
typedef enum IMAGE_TYPE_8960
{
	Q8K_HEX          = 1,          // MPRG8960.hex
	Q8K_MSIMAGE      = 2,          // 8960_msimage.mbn
	Q8K_PARTITIONBIN = 3,          // partition.bin
	Q8K_SBL1         = 4,          // sbl1.mbn
	Q8K_SBL2         = 5,          // sbl2.mbn
	Q8K_SBL3         = 6,          // sbl3.mbn
	Q8K_RPM          = 7,          // rpm.mbn
	Q8K_TZ           = 8,          // tz.mbn
	Q8K_APPBOOT      = 9,          // emmc_appsboot.mbn
	Q8K_BOOT         = 10,         // boot.img
	Q8K_SYSTEM       = 11,         // system.img.ext4
	Q8K_USERDATA     = 12,         // userdata.img.ext4
	Q8K_PERSIST      = 13,         // persist.img.ext4
	Q8K_RECOVERY     = 14,         // recovery.img
	Q8K_NONHLOS      = 15,         // NON-HLOS.bin
	Q8K_CACHE        = 16,         // cache.ime.ext4
	Q8K_RC           = 17,         // RC.img
	Q8K_PASSPORT     = 18,         // passport_FactoryDLTool
	Q8K_QGLOGFILTER  = 19,         // Qlogfilter
	Q8K_QGLOG	     = 20          // Qglog
} IMAGE_TYPE_8960;
#pragma pack()

/* Emmc download partition type */
#pragma pack (1)
typedef enum HEX_COMMAND_TYPE_8K
{
	Q8K_OPEN_MULTI_MODE_EMMC_USER = 0x21
} HEX_COMMAND_TYPE_8960;
#pragma pack()
/*============================ Structrue ==============================*/

class QISDA_DOWNLOAD_CLASS_API CDownload8960 : public CDLFormatCommand, public CRefObject
{
public:
	CDownload8960(const TSTRING& str_deviceName, const std::string& str_platform = "Qualcomm8260");
	CDownload8960(int i_COMPort, CString str_multiDLFlag, const std::string& str_platform = "Qualcomm8260");
	virtual ~CDownload8960(void);

public:
	/* 8260 Only support b_speedUp(false) and QTT_FASTDOWNLOAD, b_speedUp relate to Hex(QTH Modify Hex to Qualcomm) */
	bool MultiDownload(bool b_speedUp = false, bool b_reOpenAfterReset = false, DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD);
	bool SetImageFilePath(const int i_imageType, const std::string& str_filePath);
	CString GetErrorCode(void);
	void SetDLMode(CString str_DLMode) {m_str_DLMode = str_DLMode;};
	void SetFactoryVersion(CString str_FactoryVersion) {m_str_FactoryVersion = str_FactoryVersion;};
	void SetReworkFlag(int i_rework)   {m_i_rework = i_rework;};
	void SetRebootFlag(int i_reboot)   {m_i_reboot = i_reboot;};
	void SetDDRCheckFlag(int i_DDRCheck)   {m_i_DDRCheck = i_DDRCheck;};
	void SetCheckSumFlag(int i_checkSum)   {m_i_checkSum = i_checkSum;};
	void SetBackupNVFlag(int i_backupNV)   {m_i_backupNV = i_backupNV;};
	void SetSupportQDownloadFlag(int i_SupportQDownload)   {m_i_SupportQDownload = i_SupportQDownload;};
	void SetEraseUserFatFlag(int i_EraseUserFatFlag)   {m_i_eraseUserFat = i_EraseUserFatFlag;};
	void SetImagePath(CString str_filePath) {m_str_imagePath = str_filePath;};
	void SetCOMPicasso(int i_COMPort, CString str_picasso) {m_map_COMPicasso[i_COMPort] = str_picasso;};
	void SetProjectName(CString str_ProjectName) {m_str_projectName = str_ProjectName;};

private:
	CString  m_str_multiDLFlag;		   // Multi-download flag	
	CString  m_str_errorCode;
	CString  m_str_DLMode;             // DL Mode
	CString  m_str_FactoryVersion;
	CString  m_str_projectName;
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
	bool ExecuteFastboot(char* sz_command);
	bool ExecuteFastbootOut(char* sz_command, CString& out);
	bool GetOemMainVersion(CString& str_parameter);

private:
	bool FastbootDownload(void);
	bool FastbootFlashCmd(const std::string& str_parameter);
	bool FastbootFormatCmd(const std::string& str_parameter);	
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
	bool IsPathExist(const CString& path);

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
	std::string  m_str_passport;
	std::string  m_str_qlogfilter;
	std::string  m_str_qglog;

private:
	ADBDevice*        m_p_adbDevice;                     // Get ADB device
	CCriticalSection  m_obj_loadADBFileCritSection;      // Thread sync
	CString           m_str_imageFilePath;               // Load path
	std::map<CString, CString> m_map_imageNameCheckSum;
	static CCriticalSection  m_obj_loadcheckSumCritSection;      // Thread sync
/*======================== Android Download =================================================================================*/
};

