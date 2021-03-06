/***************************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name	: 8x26 Download   
* File name     : Download8x26_QC1.h             
* Summary       : 8x26 Download (Hex DL(Qualcomm images) + Fastboot DL(Android images))                                                             
* Version     Author            Date                 Abstract                      
* 1.0       Alex.Chen        2011/02/22           First version  
* 1.1       Alex.Chen        2011/03/03           Add: Hex only download msimage
* 1.2       Alex.Chen        2011/07/18           Add: ReDL function
****************************************************************************************/

#pragma once

//#include "../UnifyDLL_U2416_Download/Project/BaseProject.h" 
//#include "../UnifyInterface/FacTestToolInterface.h"
//#include "../../20151005_CommonTool/UI/UnifyUI_FacTestTool/FacTestToolInterface.h"
#include "../../20151005_CommonTool/Modules/UnifyInterface/FacTestToolInterface.h"
#include "Download.h"
//#include "ADBDevice.h"
#include "../Common/ErrorDefine/ErrorDefine.h"
#include "../../Lib/Qualcomm/QDART4823/inc/inc/QMSL_Core.h"
#include "../../Lib/Qualcomm/QDART4823/inc/inc/QMSL_SWDownload/QMSL_QPHONEMS_SWDL.h"
#include <ATLSTR.H>
#include <stdio.h>
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

#define FIREHOSE_DL_MODE_FASTBOOT_ONLY 1

#define DNULL                                "NULL"
#define DREAD	                             "READ"

#define QMSL_MSVC10R				_T("QMSL_MSVC10R.dll")
#define IICDLL								_T("IIC.dll")
#define F32SERVERDB					_T("f32server2.dll")
#define ID_SIZE								11
#define ID_SIZE_BUFFER					20

/*============================ Define =================================*/

/*============================ Const ==================================*/
#define DOWNLOAD_MODE_PREDL             _T("PREDL")
#define DOWNLOAD_MODE_REDL              _T("ReDL")
#define DOWNLOAD_MODE_OSDL1            _T("OS_DL1")
#define DOWNLOAD_MODE_REDL_OSDL         _T("ReDL_OSDL")
#define FALENGTH                        512
#define NVPARTITION_1_LENGTH            1152 / 2 * 1024
#define NVPARTITION_2_LENGTH            128 /2 * 1024
#define NUM_OF_SPARSE_DLL               9
#define LOG_ALL         0xFFFF  //!<' everything
/*============================ Const ==================================*/

/*============================ Structrue ==============================*/
///* Image file partition type */
//#pragma pack (1)
//typedef enum IMAGE_TYPE_8916
//{
//	Q8K_HEX          = 1,          // MPRG8960.hex
//	Q8K_MSIMAGE      = 2,          // 8960_msimage.mbn
//	Q8K_PARTITIONBIN = 3,          // partition.bin
//	Q8K_SBL1         = 4,          // sbl1.mbn
//	Q8K_RPM          = 5,          // rpm.mbn
//	Q8K_TZ           = 6,          // tz.mbn
//	Q8K_HYP          = 7,          // HYP.mbn
//	Q8K_HYPBAK       = 8,          // HYP.mbn
//	Q8K_APPBOOT      = 9,          // emmc_appsboot.mbn
//	Q8K_SBL1BAK      = 10,          // sbl1.mbn
//	Q8K_RPMBAK       = 11,         // rpm.mbn
//	Q8K_TZBAK        = 12,         // tz.mbn
//	Q8K_ABOOTBAK     = 13,         // emmc_appsboot.mbn
//	Q8K_BOOT         = 14,         // boot.img
//	Q8K_RECOVERY     = 15,         // recovery.img
//	Q8K_SYSTEM       = 16,         // system.img
//	Q8K_PERSIST      = 17,         // persist.img
//	Q8K_QGLOGFILTER  = 18,         // Qlogfilter
//	Q8K_NONHLOS      = 19,         // NON-HLOS.bin
//	Q8K_SPLASH       = 20,         // splash.bin
//	Q8K_OEM  	     = 21,         // flex_default.img
//	Q8K_USERDATA     = 22,         // userdata.img
//	Q8K_CACHE        = 23,         // cache.ime
//	Q8K_QGLOG	     = 24,         // Qglog
//	Q8K_DRM          = 25,         // drm.img
//	Q8K_PASSPORT     = 26          // passport_FactoryDLTool
//} IMAGE_TYPE_8916;
//#pragma pack()
//
///* Emmc download partition type */
//#pragma pack (1)
////typedef enum HEX_COMMAND_TYPE_8K
////{
////	Q8K_OPEN_MULTI_MODE_EMMC_USER = 0x21
////} HEX_COMMAND_TYPE_8960;
//#pragma pack()


#define USB_BUF_SIZE 512
#define BUFFER_SIZE 4096

typedef struct  {
	std::string	m_firehose_8994_lite;
	std::string  m_patch0_fastbootimage;
	std::string m_rawprogram0_fastbootimage;
	std::string m_emmc_appsboot;
} QFIL;

class QISDA_DOWNLOAD_CLASS_API CDownload8994 :public CDLCommand
{
public:
	CDownload8994(int i_COMPort, CString str_multiDLFlag);
	virtual ~CDownload8994(void);

public:
	/* 8260 Only support b_speedUp(false) and QTT_FASTDOWNLOAD, b_speedUp relate to Hex(QTH Modify Hex to Qualcomm) */
	bool MultiDownload(bool b_speedUp = false, bool b_reOpenAfterReset = false, DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD, int i_idtype = 2);
	//bool SetImageFilePath(const int i_imageType, const std::string& str_filePath);
	CString GetErrorCode(void);
	void SetDLMode(CString str_DLMode) {m_str_DLMode = str_DLMode;};
	void SetFactoryVersion(CString str_FactoryVersion) {m_str_FactoryVersion = str_FactoryVersion;};
	//void SetRebootFlag(int i_reboot)   {m_i_reboot = i_reboot;};
	//void SetCheckSumFlag(int i_checkSum)   {m_i_checkSum = i_checkSum;};
	void SetImagePath(CString str_filePath) {m_str_imagePath = str_filePath;};
//	void SetCOMPicasso(int i_COMPort, CString str_picasso) {m_map_COMPicasso[i_COMPort] = str_picasso;};
	//ADBDevice*        m_p_adbDevice;                     // Get ADB device
	void RecordQMSLCallBack(char *szMsg);

	bool FindADBdevice(int nADBPort);
	bool FindQualcomDevice(int nComport);

private:
	CString  m_str_multiDLFlag;		   // Multi-download flag	
	CString  m_str_errorCode;
	CString  m_str_DLMode;             // DL Mode
	CString  m_str_FactoryVersion;
	CString  m_str_imageFilePath;
	//int      m_i_reboot;               // Reboot flag
	//int      m_i_RebootMode;
	//int      m_i_checkSum;             // CheckSum flag
	//int      m_i_SupportQDownload;     // Support QDownload
	int      m_i_COMPort;			   // COM port
	CString  m_str_imagePath;          // Image file path
	std::map<int, CString> m_map_COMPicasso;

	int		qualcommComPort;
	char*   sz_id;
	USBDEVICE_STRUCT devStruct;
/*======================== Qualcomm Download ================================================================================*/
private:
	bool Download(bool b_speedUp = false, bool b_reOpenAfterReset = false, DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD);
	bool FastbootEXE_Download(int nPort);
	bool FastbootEXE_Download_SINGLE();
	bool FastbootEXE_OS_DL(void);
    bool ReadId(void);
	std::string GetId(void);
	std::string m_szId;
	std::string	m_szErrMsg;

private:
	//bool DMSSDL(void);
	bool SAHARADL(void);
	bool FireHose(void);
	bool FlashProgrammer(void);

	int m_iFileIndex;
	int m_iFileCount;
	int m_iFirehoseDLMode;

	bool DetectDiagPort(void);
	bool GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice);	

private:
	std::string  m_str_hex;
	std::string  m_str_msimage;

    std::string m_strDeviceName;
	std::string m_strKeyword;
	std::string m_strPROLIFICcomPort;
private:
	bool m_b_reOpenAfterReset;      // false: After reset, handset enter fastboot mode(not exist COM port)
	bool m_b_resetMode;             // true: Init to NONE mode
	bool m_b_overridePartition;     // false: Emmc partition(.bin)
/*======================== Qualcomm Download ================================================================================*/

/*======================== Android Download =================================================================================*/
public:
	bool ReadFA(char* sz_FAData);
	bool WriteFA(char* sz_FAData);
	bool ReadFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize, int i_idtype);
	bool WriteFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	bool runWriteFASector(char *sz_sectorData);

	int GetQualcommport(){return m_i_COMPort;};
	bool bFastbootDL(CString folderPath);
	bool bGetADB(int nPhone);
	bool bGetADB_SINGLE();
	bool bADB_to_Fastboot(int nPhone);
	bool bADB_to_Fastboot_SINGLE();
	bool bCallAdbFastbootCMD(CString csAdbFastboot, CString Command, char* output, char* ErrorCode, CString cs_FindData);
	bool postCmd();
	//bool bAdbCMD(CString Command, char* output, char* ErrorCode, int nPhoneIndex);

	//bool bGetFastboot(CString Command, char* output, char* ErrorCode, int nPhoneIndex);
	//bool bFastbootDL_New(CString Command, char* output, char* ErrorCode);
	//bool bAdbCMD(CString csCMD);
	
	QFIL st_QFILE;
	bool ReadIMG_qfil(CString imagePath);

	bool setIMGPath(CString imagePath);
	bool ReadIMG(CString imagePath);
	std::map < int, CString> mapDL_PASSPORT_IMGS;
	std::map < int, CString> mapDL_ALL_CHECK1_IMGS;
	std::map < int, CString> mapFORMAT_IMGS;
	std::map < int, CString> mapERASE_ALL_IMGS;

	std::map < int, CString> mapDL_PASSPORT_IMGS_TYPE;
	std::map < int, CString> mapDL_ALL_CHECK1_IMGS_TYPE;
	std::map < int, CString> mapFORMAT_IMGS_TYPE;
	std::map < int, CString> mapERASE_ALL_IMGS_TYPE;
private:
	bool SpiltString(CString str_sourceString, CString str_delimiter, CStringArray& stra_strArg);


//	CCriticalSection  m_obj_loadADBFileCritSection;      // Thread sync

//	std::map<CString, CString> m_map_imageNameCheckSum;
	static CCriticalSection  m_obj_loadcheckSumCritSection;      // Thread sync

	HMODULE     m_hQMSL_MSVC10R; 
	HANDLE g_hResourceContext;

	typedef  void (*pQLIB_SetLibraryMode)(unsigned char bUseQPST);
	typedef  void (*pQLIB_SetTargetType)(unsigned char iTargetType);
	typedef  HANDLE (*pQLIB_QPHONEMS_ConnectServer_Sahara)(unsigned int iComPort, unsigned long* version, unsigned long* msmid,  unsigned char bGetMsmId, int mode, unsigned long timeout, swdlQPHONEMSCB pSWDL_QPHONEMS_CB);
	typedef  void (*pQLIB_QPHONEMS_SaharaConfigureCallback)(HANDLE hResourceContext, swdlQPHONEMSCB pSWDL_QPHONEMS_CB);
	typedef  unsigned char (*pQLIB_QPHONEMS_Sahara_FlashProgrammer)(HANDLE hResourceContext, char* sARMPRG_FileName);
	typedef  void (*pQLIB_DisconnectServer_Sahara)(HANDLE hResourceContext);

/*============================ Structrue ==============================*/

//typedef QLIB_API HANDLE (*pQLIB_QPHONEMS_ConnectServer_Sahara)(unsigned int iComPort, unsigned long* version, swdlQPHONEMSCB pSWDL_QPHONEMS_CB);
//typedef HANDLE (*pQLIB_QPHONEMS_ConnectServer_Sahara)( unsigned int iComPort, unsigned long* version, unsigned long* msmid,  unsigned char bGetMsmId, int mode, unsigned long timeout, swdlQPHONEMSCB pSWDL_QPHONEMS_CB);

//typedef unsigned char (*pQLIB_QPHONEMS_Sahara_FlashProgrammer)(HANDLE hResourceContext, char* sARMPRG_FileName);
typedef void (*pQLIB_DisconnectServer)(HANDLE hResourceContext);

typedef HANDLE (*pQLIB_QPHONEMS_ConnectServer_FireHose)( unsigned int iComPort, swdlQPHONEMSCB pSWDL_QPHONEMS_CB);
//typedef void (*pQLIB_QPHONEMS_FireHoseConfigureCallback)(HANDLE hResourceContext, swdlQPHONEMSCB pSWDL_QPHONEMS_CB);
typedef unsigned char (*pQLIB_QPHONEMS_FireHoseNOP)(HANDLE hResourceContext);
typedef unsigned char (*pQLIB_QPHONEMS_FireHoseConfigure)(  HANDLE hResourceContext,
																   char* MaxPayloadSizeToTargetInBytes,
																   char* memory_name,
																   char* target_name,
																   char* AckPacketNumber,
																   unsigned char bUseAckRawDataAttr,
																   unsigned char bUseSkipWriteAttr,
																   unsigned char bUseAlwaysValidateAttr,
																   unsigned char bUseVerboseAttr,
																   unsigned char bUseAckRawDataEveryNumPackets
																   );
typedef unsigned char (*pQLIB_QPHONEMS_UploadEmmcImage_FireHose)(HANDLE hResourceContext, char* rawprogramfile, char* patchfile,  float* imageSizeInMB, float* throughput);
typedef unsigned char (*pQLIB_QPHONEMS_FireHosePower)(HANDLE hResourceContext, char* Action);
typedef void (*pQLIB_DisconnectServer_FireHose)(HANDLE hResourceContext );

//typedef void (*pQLIB_QPHONEMS_FireHosePower_WithDelay)(HANDLE hResourceContext,
//														char* Action,
//														unsigned int DelayInSeconds);

//typedef HANDLE (*pQLIB_ConnectServer) ( unsigned int iComPort );


//typedef unsigned char (*pQLIB_QPHONEMS_QLIB_SetLogFlags)( HANDLE hResourceContext, unsigned int uiLogFlags );
//typedef unsigned char (*pQLIB_QPHONEMS_QLIB_StartLogging)( HANDLE hResourceContext, char* sLogFile );
/*======================== Android Download =================================================================================*/
};

