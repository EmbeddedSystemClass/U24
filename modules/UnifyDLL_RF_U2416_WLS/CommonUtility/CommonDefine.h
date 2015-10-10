#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_


#include "CommonFunction.h"
#include <limits.h>

static const int FTD_BUF_SIZE = 4096;

#define STATION_OOB				      _T("OOB")
#define IICDLL								_T("IIC.dll")
#define F32SERVERDB					_T("f32server2.dll")
#define ID_SIZE								11
#define ID_SIZE_BUFFER					20

//WCDMA
#define WCDMA_850_BAND_ID	850
#define WCDMA_900_BAND_ID	900
#define WCDMA_AWS_BAND_ID	1700
#define WCDMA_PCS_BAND_ID	1900
#define WCDMA_IMT_BAND_ID	2100

#define NONE_VALUE			(double)_I64_MAX

// For run main function
#define CALL_MEMBER_FN(object, ptrToMember) ((object).*(ptrToMember))


// Test item event messages
const std::string DLL_PATTERN = "DLLPattern";
const std::string DLL_TESTING_ITEM = "DLLTestingItem";
const std::string DLL_MESSAGE = "DLLMessage";
const std::string DLL_TEST_RESULT = "DLLTestResult";
const std::string DLL_PICSDATA = "DLLPICSData";
const std::string DLL_JIG_CONTROL = "DLLJigControl";
const std::string DLL_SHOW_MSG_DLG = "DLLShowMsgDlg";
const std::string DLL_LOG = "DLLLog";
const std::string SHOW_DIALOG = "DLLTestItemShowDialog";

//// Severity definition
const int MSG_INFO = 0;
const int MSG_WARNING = 1;
const int MSG_ERROR = 2;


// Event messages data structure for event 'DLL_PATTERN'
struct sPattern
{
	std::string strPattern;
};


// Event messages data structure for event 'DLL_TESTING_ITEM'
struct sTestingItem
{
	std::string strTestingItem;
};


// Event messages data structure for event 'DLL_MESSAGE'
struct sMessage
{
	std::string strMessage;
};


// Event messages data structure for event 'DLL_TEST_RESULT'
struct sTestResult
{
	std::string strTime;
	std::string strResult;
	std::string strItemCode;
	std::string strErrCode;
	std::string strBand;
	std::string strChannel;
	std::string strLower;
	std::string strUpper;
	std::string strMeasured;
	std::string strUnit;
	std::string strElapse;
	std::string strMsg;
};


// Event messages data structure for event 'DLL_PICSDATA'
struct sPICSData
{
	std::string strKey;
	std::string strValue;
};


// Event messages data structure for event 'DLL_JIG_CONTROL'
struct sJigControl
{
	std::string strSwitch;
};


// Event messages data structure for event 'DLL_SHOW_MSG_DLG'
struct sShowMsgDlg
{
	std::string strMessage;
};


// Event messages data structure for event 'DLL_LOG'
struct sTraceLog
{
	int iSeverity;
	std::string strMessage;
};


#endif // End of #ifndef _COMMON_DEFINE_H_
