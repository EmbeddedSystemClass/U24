#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_


// Test item event messages
const std::string DLL_PATTERN = "DLLPattern";
const std::string DLL_TESTING_ITEM = "DLLTestingItem";
const std::string DLL_MESSAGE = "DLLMessage";
const std::string DLL_TEST_RESULT = "DLLTestResult";
const std::string DLL_PICSDATA = "DLLPICSData";
const std::string DLL_PROGRESS = "DLLProgress";


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



// Event messages data structure for event 'DLL_SHOW_MSG_DLG'
struct sShowMsgDlg
{
	std::string strMessage;
};

// Event messages data structure for event 'DLL_PROGRESS'
struct sProgress
{
	int iMainProgress;
	int iSubProgress;
};


#endif // End of #ifndef _COMMON_DEFINE_H_
