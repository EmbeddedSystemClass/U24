#include "stdafx.h"
#include "ITestProcessor.h"

#include "Utils/CommonDefine.h"
#include "..\..\UnifyInterface\FacTestToolInterface.h"
using namespace StringUtility;

#define MSGBUFF 1024



ITestProcessor::ITestProcessor()
{
	StartTotalDurationCounter();
	StartTestItemDurationCounter();

	m_p_event_callback=NULL;

	m_iSlot = -1;

	m_strErrCode = "";
	m_strErrMsg = "";

	clock_t m_totalTestDuration_Start=0;
	clock_t m_testItemDuration_Start=0;

	//CLog
	m_LogName = "";
	m_pLog = new CLogger();
}

ITestProcessor::~ITestProcessor()
{
	delete m_pLog;
}



// ---------------- Set/Get Parameter Value ----------------

bool ITestProcessor::SetParameterValue(char* sz_keyword, char* sz_value)
{
	string strKeyWord = sz_keyword;
	string strValue = sz_value;

	g_mapToolInfo[strKeyWord] = std::string(strValue);

	if ( "PICASSO" == strKeyWord )
	{
		char szLogName[MAX_PATH];
		string strTimestamp = CLogger::GetTimeStamp(); 
		if (strValue.empty())
		{
			//init name , timestamp + slot
			sprintf_s(szLogName, MAX_PATH, "%s_[%s]",
				strTimestamp.c_str(), g_mapToolInfo["CURRENT_SLOT"].c_str());
		}
		else
		{
			//init name , timestamp + picasso
			sprintf_s(szLogName, MAX_PATH, "%s_%s",
				strTimestamp.c_str(), strValue.c_str());
		}
		
		//New log and set log name
		m_pLog->SetLogName(szLogName);
		m_LogName=szLogName;
	}

	return true;
}

bool ITestProcessor::GetParameterValue(char* sz_keyword, char* sz_value, int i_size)
{
	if (g_mapToolInfo.find(std::string(sz_keyword)) == g_mapToolInfo.end())
	{
		char szMsg[256];
		memset(szMsg, 0, 256);
		sprintf_s(szMsg, 256, "Fail to find value with keyword %s", sz_keyword);

		FactoryLog(false, "GetParameterValue", "", "", "", "", "", "", "", szMsg);
		return false;
	}

	sprintf_s(sz_value, (i_size - 1), "%s", g_mapToolInfo[std::string(sz_keyword)].c_str());
	return true;
}

// ------------------ UI control functions -----------------

void ITestProcessor::SetUIPattern(std::string strPattern)
{
	sPattern firedData;

	firedData.strPattern = strPattern;

	Fire(DLL_PATTERN, (long)&firedData);
}

void ITestProcessor::SetTestingItem(std::string strTestItemName)
{
	sTestingItem firedData;

	firedData.strTestingItem = strTestItemName;

	Fire(DLL_TESTING_ITEM, (long)&firedData);
}

void ITestProcessor::SetPICSData(std::string strKey, std::string strValue)
{
	if (strKey.empty())
		return;

	sPICSData firedData;

	firedData.strKey = strKey;
	firedData.strValue = strValue;

	Fire(DLL_PICSDATA, (long)&firedData);
}

void ITestProcessor::SetTestResult(bool bPass)
{
	sTestResult firedData;
	memset(&firedData, 0, sizeof(sTestResult));

	SYSTEMTIME currTime;
	char szCurrTime[256];

	::GetLocalTime(&currTime);
	memset(szCurrTime, 0, 256);
	sprintf_s(szCurrTime, 255, "%02d:%02d:%02d.%03d",
		currTime.wHour, currTime.wMinute, currTime.wSecond, currTime.wMilliseconds);

	firedData.strTime = szCurrTime;

	firedData.strErrCode = this->m_strErrCode;

	firedData.strElapse = GetTestDuration();

	firedData.strMsg = this->m_strErrMsg;

	bPass ? firedData.strResult = "PASS" : firedData.strResult = "FAIL";

	Fire(DLL_TEST_RESULT, (long)&firedData);
}

void ITestProcessor::SetUIMessage(std::string strMsg)
{
	/* Check input */
	if (strMsg.length() <= 0)
	{
		return;
	}

	sMessage firedData;
	firedData.strMessage = strMsg;

	/* Write Log */
	TraceLog(strMsg);

	Fire(DLL_MESSAGE, (long)&firedData);

	return;
}

void ITestProcessor::SetUIProgress(int iMainProgress, int iSubProgress)
{
	sProgress firedData;
	firedData.iMainProgress = iMainProgress;
	firedData.iSubProgress = iSubProgress;

	Fire(DLL_PROGRESS, (long)&firedData);
}

/**********************************************************
    Duration 
***********************************************************/

void ITestProcessor::StartTotalDurationCounter()
{
	m_totalTestDuration_Start = clock();
}

void ITestProcessor::StartTestItemDurationCounter()
{
	m_testItemDuration_Start = clock();
}

std::string ITestProcessor::GetTestDuration()
{
	clock_t currentClock = clock();
	clock_t durationClock = currentClock - m_testItemDuration_Start;

	double dDuration = durationClock / (double)CLOCKS_PER_SEC;

	char szDuration[256];
	memset(szDuration, 0, 256);
	sprintf_s(szDuration, 255, "%.3lf", dDuration);

	return std::string(szDuration);
}

std::string ITestProcessor::GetTotalTestDuration()
{
	clock_t currentClock = clock();
	clock_t durationClock = currentClock - m_totalTestDuration_Start;

	double dDuration = durationClock / (double)CLOCKS_PER_SEC;

	char szDuration[256];
	memset(szDuration, 0, 256);
	sprintf_s(szDuration, 255, "%.3lf", dDuration);

	return std::string(szDuration);
}

/**********************************************************
    Event to UI 
***********************************************************/
void ITestProcessor::FireToUI(const std::string& strEvent, long nParam)
{
	char szEvent[32] = {0};

	sprintf_s(szEvent, 32, "%s", strEvent.c_str());
	m_p_event_callback(szEvent , nParam);
}

void ITestProcessor::Fire(const std::string& strEvent, long nParam)
{
	if (strEvent == DLL_PATTERN)
	{
		sPattern* psPattern = (sPattern*)nParam;
		st_UIPattern sFired;
		memset(&sFired, 0, sizeof(st_UIPattern));  

		sFired.i_slot = m_iSlot;
		sFired.b_append = false;
		sprintf_s(sFired.sz_pattern, MAX_BUFFER_SIZE, "%s", psPattern->strPattern.c_str());

		FireToUI(UI_PATTERN, (long)&sFired);
	}
	else if (strEvent == DLL_TESTING_ITEM)
	{
		sTestingItem* psTestingItem = (sTestingItem*)nParam;
		st_UIItem sFired;
		memset(&sFired, 0, sizeof(st_UIItem));  

		sFired.i_slot = m_iSlot;
		sprintf_s(sFired.sz_item, MAX_BUFFER_SIZE, "%s", psTestingItem->strTestingItem.c_str());

		FireToUI(UI_ITEM, (long)&sFired);
	}
	else if (strEvent == DLL_MESSAGE)
	{
		sMessage* psMessage = (sMessage*)nParam;
		st_UIMessage sFired;
		memset(&sFired, 0, sizeof(st_UIMessage));  

		sFired.i_slot = m_iSlot;
		sprintf_s(sFired.sz_message, MAX_BUFFER_SIZE, "%s", psMessage->strMessage.c_str());

		FireToUI(UI_MESSAGE, (long)&sFired);
	}
	else if (strEvent == DLL_TEST_RESULT)
	{
		sTestResult* psTestResult = (sTestResult*)nParam;
		st_UIResult sFired;
		memset(&sFired, 0, sizeof(st_UIResult));  

		sFired.i_slot = m_iSlot;
		sprintf_s(sFired.sz_time, MAX_BUFFER_SIZE, "%s", psTestResult->strTime.c_str());
		sprintf_s(sFired.sz_result, MAX_BUFFER_SIZE, "%s", psTestResult->strResult.c_str());
		sprintf_s(sFired.sz_itemCode, MAX_BUFFER_SIZE, "%s", psTestResult->strItemCode.c_str());
		sprintf_s(sFired.sz_errCode, MAX_BUFFER_SIZE, "%s", psTestResult->strErrCode.c_str());
		sprintf_s(sFired.sz_band, MAX_BUFFER_SIZE, "%s", psTestResult->strBand.c_str());
		sprintf_s(sFired.sz_channel, MAX_BUFFER_SIZE, "%s", psTestResult->strChannel.c_str());
		sprintf_s(sFired.sz_lower, MAX_BUFFER_SIZE, "%s", psTestResult->strLower.c_str());
		sprintf_s(sFired.sz_upper, MAX_BUFFER_SIZE, "%s", psTestResult->strUpper.c_str());
		sprintf_s(sFired.sz_measured, MAX_BUFFER_SIZE, "%s", psTestResult->strMeasured.c_str());
		sprintf_s(sFired.sz_unit, MAX_BUFFER_SIZE, "%s", psTestResult->strUnit.c_str());
		sprintf_s(sFired.sz_elapse, MAX_BUFFER_SIZE, "%s", psTestResult->strElapse.c_str());
		sprintf_s(sFired.sz_msg, MAX_BUFFER_SIZE, "%s", psTestResult->strMsg.c_str());

		FireToUI(UI_RESULT, (long)&sFired);
	}
	else if (strEvent == DLL_PICSDATA)
	{
		sPICSData* psPICSData = (sPICSData*)nParam;
		st_PICSData sFired;
		memset(&sFired, 0, sizeof(st_PICSData));  

		sFired.i_slot = m_iSlot;
		sprintf_s(sFired.sz_key, MAX_BUFFER_SIZE, "%s", psPICSData->strKey.c_str());
		sprintf_s(sFired.sz_value, MAX_BUFFER_SIZE, "%s", psPICSData->strValue.c_str());

		char szTmp[512];
		memset(szTmp, 0, 512);
		sprintf_s(szTmp, 512, "############### PICS data : %s , %s ###############", sFired.sz_key, sFired.sz_value);
		TraceLog(szTmp);

		FireToUI(UI_PICSDATA, (long)&sFired);
	}
	else if (strEvent == DLL_PROGRESS)
	{
		sProgress* psProgress = (sProgress*)nParam;
		st_UIProgress sFired;
		memset(&sFired, 0, sizeof(st_UIProgress));  

		sFired.i_slot = m_iSlot;
		sFired.i_mainProgress = psProgress->iMainProgress;
		sFired.i_subProgress = psProgress->iSubProgress;

		FireToUI(UI_PROGRESS, (long)&sFired);
	}
}



/**********************************************************
   Log callback to controller
***********************************************************/

void ITestProcessor::TraceLog(std::string strMsg)
{
	if( NULL != m_pLog)
	{
		m_pLog->TraceLog(strMsg.c_str() , m_iSlot);
	}
}

/**********************************************************
   extend log functions 
***********************************************************/

void ITestProcessor::FactoryLog(bool bPass, std::string strMsg)
{
	FactoryLog (bPass, "", "", "", "", "", "", "", "", strMsg);
}

void ITestProcessor::FactoryLog(
								bool bPass,
								std::string strItemCode,
								std::string strErrCode,
								std::string strBand,
								std::string strChannel,
								std::string strLower,
								std::string strUpper,
								std::string strMeasured,
								std::string strUnit,
								std::string strMsg
								)
{
	sTestResult firedData;
	SYSTEMTIME currTime;
	char szCurrTime[256];

	::GetLocalTime(&currTime);
	memset(szCurrTime, 0, 256);
	sprintf_s(szCurrTime, 255, "%02d:%02d:%02d.%03d",
		currTime.wHour, currTime.wMinute, currTime.wSecond, currTime.wMilliseconds);

	firedData.strTime = szCurrTime;

	firedData.strItemCode = strItemCode;
	firedData.strErrCode = strErrCode;
	firedData.strBand = strBand;
	firedData.strChannel = strChannel;
	firedData.strLower = strLower;
	firedData.strUpper = strUpper;
	firedData.strMeasured = strMeasured;
	firedData.strUnit = strUnit;

	firedData.strElapse = GetTestDuration();

	firedData.strMsg = strMsg;

	if (bPass)
	{
		firedData.strResult = "PASS";
		firedData.strErrCode = "PASS";
	}
	else
	{
		firedData.strResult = "FAIL";

		m_strErrCode = strErrCode;
		m_strErrMsg = strMsg;
	}


	TraceLog("================================= Factory Log ==================================");
	char szLog[MSGBUFF];
	memset(szLog, 0, MSGBUFF);
	if (bPass)
	{
		sprintf_s(szLog, MSGBUFF, "PASS,%s,%s,%s,%s,%s,%s,%s,%s,%s",
			strItemCode.c_str(), strErrCode.c_str(), strBand.c_str(), strChannel.c_str(), 
			strLower.c_str(), strUpper.c_str(), strMeasured.c_str(), strUnit.c_str(), strMsg.c_str());
	}
	else
	{
		sprintf_s(szLog, MSGBUFF, "FAIL,%s,%s,%s,%s,%s,%s,%s,%s,%s",
			strItemCode.c_str(), strErrCode.c_str(), strBand.c_str(), strChannel.c_str(), 
			strLower.c_str(), strUpper.c_str(), strMeasured.c_str(), strUnit.c_str(), strMsg.c_str());
	}

	TraceLog(szLog);
	TraceLog("================================================================================");
	



//	Fire(DLL_TEST_RESULT, (long)&firedData);
}

// Reg callback function for Fire() ,  set to GPProcessor
bool ITestProcessor::reg_event_callback( p_fn_event_callback pcallback )
{	
	m_p_event_callback = pcallback;

	return true;
}


// Reg callback function for SendEventToProcessor() ,  set to DLL
bool ITestProcessor::reg_processor_callback( p_fn_processor_callback pcallback )
{	
	m_p_processor_callback = pcallback;

	return true;
}

void ITestProcessor::CPKLog(
	const char* szTech,
	const char* szItemName,
	const char* szBandClass,
	const char* szChannel,
	const char* szIdentifier1,
	const char* szIdentifier2,
    const char* szLowerSpec,
    const char* szUpperSpec,
	const char* szMeasValue,
	bool bPass,
	int iSlot
	)
{
	//SetTraceLogDestination();

	char szLog[MSGBUFF];
	memset(szLog, 0, MSGBUFF);

    // ,CPKData,{TECH},{TestItem},{BAND},{Channel},{Identifier1},{Identifier2},{LowerSpec},{UpperSpec},{MeasValue},{PASS/FAIL}

	if (bPass)
	{
		sprintf_s(szLog, MSGBUFF, ",CPKData,%s,%s,%s,%s,%s,%s,%s,%s,%s,PASS",
			szTech, szItemName, szBandClass, szChannel, szIdentifier1, szIdentifier2, szLowerSpec, szUpperSpec, szMeasValue);
	}
	else
	{
		sprintf_s(szLog, MSGBUFF, ",CPKData,%s,%s,%s,%s,%s,%s,%s,%s,%s,FAIL",
			szTech, szItemName, szBandClass, szChannel, szIdentifier1, szIdentifier2, szLowerSpec, szUpperSpec, szMeasValue);
	}

	TraceLog(szLog);
}