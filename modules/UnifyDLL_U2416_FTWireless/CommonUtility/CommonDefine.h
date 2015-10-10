#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_


#include "CommonFunction.h"

static const int FTD_BUF_SIZE = 4096;

// For run main function
#define CALL_MEMBER_FN(object, ptrToMember) ((object).*(ptrToMember))


// Test item event messages
const std::string TRACE_LOG = "DLLTestItemTraceLog";
const std::string FACTORY_LOG = "DLLTestItemFactoryLog";
const std::string PICS_DATA = "DLLPICSData";
const std::string SHOW_PICTURE = "DLLTestItemShowPicture";
const std::string JIG_REQUEST = "DLLTestItemJigRequest";
const std::string SHOW_DIALOG = "DLLTestItemShowDialog";


// Severity definition//110512 gilpin move to enumfile.h
//const int MSG_INFO = 0;
//const int MSG_WARNING = 1;
//const int MSG_ERROR = 2;

/*==================== Station ============================*/
/* T-Line */
#define STATION_TRIGGER			      _T("TRIGGER")
#define STATION_2GPTEST			      _T("2GPTEST")
#define STATION_3GPTEST			      _T("3GPTEST")
#define STATION_BBTEST 			      _T("BBTEST")
#define STATION_BTWLAN 			      _T("BTWLAN")

/* A-Line */
#define STATION_ONLINE_WLS		      _T("ONLINE_WLS")
#define STATION_ONLINE_WLS2		      _T("ONLINE_WLS2")
#define STATION_MMI_BB			      _T("MMI_BB")
#define STATION_PREMMI			      _T("PREMMI")
#define STATION_ALS_AUDIO			  _T("ALS_AUDIO")
#define STATION_TOUCH				  _T("TOUCH")
#define STATION_CAMERA				  _T("CAMERA")
#define STATION_FALINK			      _T("FALINK")
#define STATION_VI				      _T("VI")
#define STATION_RUN_IN				  _T("RUN_IN")

/* P-Line */
#define STATION_IMEI			      _T("IMEI")
#define STATION_OS_DL			      _T("OS_DL")
#define STATION_FCHECK			      _T("FCHECK")
#define STATION_PPIDLINK		      _T("PPIDLINK")
#define STATION_GB_PRINTING		      _T("GB_PRINTING")
#define STATION_FFU				      _T("FFU")

/* OOB */
#define STATION_OOB				      _T("OOB")
#define IICDLL								_T("IIC.dll")
#define F32SERVERDB					_T("f32server2.dll")
#define ID_SIZE								11
#define ID_SIZE_BUFFER					20
//#define PICS_S3_FACTORY TEXT("Pics_S3_Factory.dll")
//#define VMS_DBACCESS    TEXT("VMS_DBAccess.dll")
/*==================== Station ============================*/

// Test items event messages data structure
struct sTraceLog
{
	int iSeverity;
	std::string strMessage;
};

#endif // End of #ifndef _COMMON_DEFINE_H_
