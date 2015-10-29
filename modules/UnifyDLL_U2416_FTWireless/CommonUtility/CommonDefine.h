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

/*==================== Station ============================*/
/* T-Line */
#define STATION_TRIGGER			      _T("TRIGGER")
#define STATION_BTWLAN 			      _T("BTWLAN")

/* A-Line */
#define STATION_RUN_IN				  _T("RUN_IN")
#define STATION_OS_DL			      _T("OS_DL")

#define IICDLL								_T("IIC.dll")
#define F32SERVERDB					_T("f32server2.dll")
#define ID_SIZE								11
#define ID_SIZE_BUFFER					20

/*==================== Station ============================*/

// Test items event messages data structure
struct sTraceLog
{
	int iSeverity;
	std::string strMessage;
};

#endif // End of #ifndef _COMMON_DEFINE_H_
