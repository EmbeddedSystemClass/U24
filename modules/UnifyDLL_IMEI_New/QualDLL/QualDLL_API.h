#ifndef _QUALDLL_API_H
#define	_QUALDLL_API_H

// 1. Please notice that ShangHai site uses CVI 5.0 as the build environment,
//    the corresponding compiler can not recognize the syntax 'extern "C"'.
//
// 2. Please remember to add preprocessor definition '_QUALDLL_EXPORT' in project setting. (in both Debug/Release version)
//    The token means the dll project exports some variables and functions.
//
// 3. If the DLL would be used by Client APP written in C code, add 'extern "C"'
//    before every function prototypes and function definitions.
//	  Besides, if 'extern "C"' is used in the DLL, Client APP should use it as well.
/*
#ifndef _QUALDLL_EXPORT	// For importing variables
	#define QUALDLL __declspec(dllimport)
#else						// For exporting variables 
	#define QUALDLL __declspec(dllexport)
#endif // _QUALDLL_EXPORT

#ifndef _QUALDLL_EXPORT	// For importing functions
	#ifndef __cplusplus
		#define QUALDLLAPI __declspec(dllimport)
	#else
		#define QUALDLLAPI extern "C" __declspec(dllimport)
	#endif // __cplusplus
#else						// For exporting functions
	#ifndef __cplusplus
		#define QUALDLLAPI __declspec(dllexport)
	#else
		#define QUALDLLAPI extern "C" __declspec(dllexport)
	#endif // __cplusplus
#endif // _QUALDLL_EXPORT
*/
/*--------------------------------------------------------------------------*/
/* CONSTANT DECLARATION                                                     */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* TYPE/STRUCTURE DEFINITION                                                */
/*--------------------------------------------------------------------------*/
typedef enum QDLToolMessageType
{
	QMSG_ERROR   = -1,
	QMSG_NORMAL  =  0,
	QMSG_WARNING =  1,
	QMSG_OK      =  2
} MessageType;

typedef struct
{
	CRITICAL_SECTION cs;
	long count;
	long number_of_threads;
} SYNCHRONIZATION_t;

/**
 * 8650B--AUSB1AM     original
 * 8650B--SDCAALYM-02 original
 * 8650B--AUSB1AM-01  support speedup, add DT
 * 8650B--AUSB1AM-02  add RESERVED partition
 * 8650B--AUSB1AM-03  support set fastboot flag
 * 8650B--AUSB1AM-04  no change for this version
 * 8650B--AUSB1AM-05  support new way to download DT image and restore RESERVED data
 * 3180-AUSB1AM-06    big jump to R3180
 * 3180-AUSB1DM-06    support dual mode
 * 02-3180-AUSB1XM-07 not restore SIMSECURE
 * 03-3180-AUSB1XM-07 not restore SIMSECURE
 * 04-3180-AUSB1XM-07 not restore SIMSECURE
 * 05-3180-AUSB1XM-07 support restore SIMSECURE
 * XX-3200-AUSB1XM-08 R3200
 * 09-324012-AUSB1AM-08
 * 11-4060-AUSB1AM-08
 * 01-324012-TOUB1AM-08
 */
typedef enum OsblSupport
{
	OSBL_UNKNOWN  = -1,
	OSBL_NORMAL   =  0,
	OSBL_SPEEDUP  =  1,
	OSBL_FASTBOOT =  3,
	OSBL_RESERVED =  5,
	OSBL_R3180    =  6,
	OSBL_SIMSECURE = 7,
	OSBL_R3200    =  8,
	OSBL_R4077    =  9,
	OSBL_LAST     =  9,
} osbl_version_t;

enum AustinHardwareVersion
{
	AUSTIN_EVT2_1 = 3,
	AUSTIN_EVT2_2 = 4,
	AUSTIN_EVT3   = 5,
	AUSTIN_HW_BOUND = 20,
};

const char* const PTN_AUSTIN = "AUSB";
const char* const PTN_TOUCAN = "TOUB";


/*--------------------------------------------------------------------------*/
/* EXPORTED CLASS	                                                        */
/*--------------------------------------------------------------------------*/
// example: class FACTORYDLL CMyClass
//			{
//				// Declaration 
//			};


/*--------------------------------------------------------------------------*/
/* EXPORTED VARIABLES                                                       */
/*--------------------------------------------------------------------------*/
// Please use keyword "extern" if a 'constant' variable is exported.
// example of a constant: FACTORYDLL extern const COLORREF MyColor = RGB(0, 0, 0);
// example of a variable: FACTORYDLL int MyInt;

/*--------------------------------------------------------------------------*/
/* EXPORTED FUNCTIONS                                                       */
/*--------------------------------------------------------------------------*/
// example: FACTORYDLLAPI int _ func(void); 

/////////////////////////////////////////////////////////////////
// Qualcomm image signature check function
/////////////////////////////////////////////////////////////////
int CheckSignedHeaderHex(const char* const path); // NO_ERROR
int CheckSignedHeaderMbn(const char* const path); // NO_ERROR
int CheckSignedInfoHex(const char* const path); // NO_ERROR
int CheckSignedInfoMbn(const char* path); // NO_ERROR

/////////////////////////////////////////////////////////////////
/////////////////// QUALCOMM GENERAL FUNCTION ///////////////////
/////////////////////////////////////////////////////////////////
HANDLE QDL_Initial(char* szComPort);
int QDL_Close(HANDLE hResource); // NO_ERROR
int QDL_Reset(HANDLE hResource, char* pthHex); // NO_ERROR
int QDL_Probe(HANDLE hResource); // NO_ERROR
int QDL_Nop(HANDLE hResource); // NO_ERROR
int QDL_Set_CallBack_Function(HANDLE hResource, int(*CallBack)(const char*, int)); // TRUE
int QDL_Get_OSBL_Version(HANDLE hResource, char* szOSBLVersion); // NO_ERROR
int QDL_Set_Fastboot_Flag(HANDLE hResource); // NO_ERROR
int QDL_Set_Fastboot_Serial(HANDLE hResource, char* szSerial); // NO_ERROR
int QDL_Erase_EFS2(HANDLE hResource, char* pthHex, char* pthPartition); // NO_ERROR

/////////////////////////////////////////////////////////////////
/////////////////// QUALCOMM DOWNLOAD ///////////////////////////
/////////////////////////////////////////////////////////////////
int QDL_Upload_MultiImage_Emergency(HANDLE Resource, char* pthHex,
	char* pthPartition, char* pthDbl, char* pthFsbl, char* pthOsbl, bool IsRePartition); // NO_ERROR

int QDL_Upload_MultiImage(HANDLE hResource,
	char* pthHex, char* pthPartition,
	char* pthDbl, char* pthFsbl, char* pthOsbl,
	char* pthAmss,   char* pthDsp1, char* pthEfs2,
	char* pthAppsbl, char* pthApps, char* pthFlash,
	bool IsEraseEFS2, bool IsRePartition, SYNCHRONIZATION_t* sync); // NO_ERROR

int QDL_Upload_MultiImage_Speedup(HANDLE hResource,
	char* pthHex, char* pthPartition,
	char* pthDbl, char* pthFsbl, char* pthOsbl,
	char* pthAmss,   char* pthDsp1, char* pthEfs2,
	char* pthAppsbl, char* pthApps, char* pthFlash, char* pthDt,
	char* pthBkFsbl, char* pthBkOsbl, char* pthBkDt, char* pthBkAppsbl, char* pthReserved,
	bool IsEraseEFS2, bool IsRePartition, SYNCHRONIZATION_t* sync); // NO_ERROR

int QDL_Upload_MultiImage_7XXX(HANDLE hResource,
	char* pthHex, char* pthPartition,  char* pthQcsblHd, char* pthQcsbl,
	char* pthOemsblHd, char* pthOemsbl, char* pthAmssHd, char* pthAmss,
	char* pthAppsblHd, char* pthAppsbl, char* pthAppsHd, char* pthApps,
	bool IsRePartition, bool IsSetFastbootFlag, bool IsEraseEFS2); // NO_ERROR

int QDL_Upload_MultiImage_7XXX_Emergency(HANDLE hResource,
	char* pthHex, char* pthPartition,  char* pthQcsblHd, char* pthQcsbl,
	char* pthOemsblHd, char* pthOemsbl, char* pthAmssHd, char* pthAmss,
	char* pthAppsblHd, char* pthAppsbl, char* pthAppsHd, char* pthApps,
	bool IsRePartition, bool IsSetFastbootFlag, bool IsEraseEFS2); // NO_ERROR

int QDL_Upload_MultiImage_8K_Emergency(HANDLE hResource,
	char* pthHex, char* pthFastboot, bool IsSetFastbootFlag, bool IsEraseEFS2); // NO_ERROR

/////////////////////////////////////////////////////////////////
/////////////////// QUALCOMM DIAG COMMAND ///////////////////////
/////////////////////////////////////////////////////////////////
int QDL_DIAG_SWITCH_To_Dload(HANDLE hResource); // NO_ERROR
int QDL_DIAG_NV_Write(HANDLE hResource, unsigned short ItemID,
								 int nDataLength, unsigned char* pData,
								 char* szSPC, unsigned short* Status); // NO_ERROR
int QDL_DIAG_NV_Read(HANDLE hResource, unsigned short ItemID,
								int nDataLength, unsigned char* pData,
								char* szSPC, unsigned short* Status); // NO_ERROR
int QDL_DIAG_BRT_Restore(HANDLE hResource, char* pthBrt, char* szSPC); // NO_ERROR
int QDL_DIAG_BRT_Backup(HANDLE hResource, char* pthBrt, char* szSPC); // NO_ERROR

#endif // _QUALDLL_API_H

