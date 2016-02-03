
#include "stdafx.h"
#include "HallSensorTest.h"

RF_IMPLEMENT_DYNCREATE(CHallSensor)

bool CHallSensor::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string pstrRange;

	if (paramMap.find(std::string("XMLTestItem")) != paramMap.end())
	{
		m_str_TestItem = paramMap[std::string("XMLTestItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'TestItem' for class 'CHallSensor'");
		return false;
	}


	/* Init to 5s */
	m_i_sleep = 5;

	std::string str;
	if (paramMap.find(std::string("SleepTime")) != paramMap.end())
	{
		str = paramMap[std::string("SleepTime")];
		m_i_sleep = CStr::StrToInt(str);
	}
	else
	{
		TraceLog(MSG_INFO, "Get SleepTime Fail.");
		return false;
	}

	// Init Retry 1 time
	m_i_retry = 1;
	std::string strRetry;
	if (paramMap.find(std::string("Retry")) != paramMap.end())
	{
		strRetry = paramMap[std::string("Retry")];
		m_i_retry = CStr::StrToInt(strRetry);
	}
	else
	{
		TraceLog(MSG_INFO, "Set Retry for 1 time by default.");
	}


	//Enable JIG or NO
	m_i_EnableJIG = 1;
	std::string strEnableJIG;
	if (paramMap.find(std::string("EnableJIG")) != paramMap.end())
	{
		strEnableJIG = paramMap[std::string("EnableJIG")];
		m_i_EnableJIG = CStr::StrToInt(strEnableJIG);
	}
	else
	{
		TraceLog(MSG_INFO, "Set EnableJIG for 1 by default.");
	}

	return true;
}

bool CHallSensor::PreRun()
{
	m_strResult    = "";
	m_strItemCode  = "";
	m_strErrorCode = "";
	m_strBand      = "-";
	m_strChannel   = "-";
	m_strUpper     = "";
	m_strLower     = "";
	m_strMeasured  = "";
	m_strUnit      = "-";
	m_strMessage   = "";

	return true;
}

bool CHallSensor::Run()
{

	bool passFail = false;

	if (m_str_TestItem == HallSensorOpenTest )
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_HallSensor_Test_Fail;
		str_Pics = CW2A(L"Hall_Sensor_Open_Test");
		passFail = runHallSensorOpenTest();
	}
	else if (m_str_TestItem == HallSensorCloseTest)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_HallSensor_Test_Fail;
		str_Pics = CW2A(L"Hall_Sensor_Close_Test");
		passFail = runHallSensorCloseTest();
	}

	return passFail;
}

bool CHallSensor::PostRun()
{
	return true;
}
bool CHallSensor::runHallSensorCloseTest(void)
{
	int nHallSensorValue = 0;

	if (SendJigRequest("ON") != true)// k near
	{
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Jig_Move_Fail;
		m_strMessage = "Jig Request(ON) Fail!";
		m_strUpper = "";
		m_strLower = "";
		m_strMeasured = "";
		TraceLog(MSG_INFO, "Jig Request(ON) Fail!");
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, "Jig Request(ON) Pass!");
	m_strItemCode  = CStr::IntToStr(HallSensor_Calibration_BaseItemCode);

	char sz_inData[FTD_BUF_SIZE] = "117";
	char sz_outData[FTD_BUF_SIZE] = {0};
	if (! (m_pIPhone->FTD_Get_GPIO_Value(m_nFtdPort, m_nFtdTimeOut, sz_inData, sz_outData)) )
	{
		m_strResult = "FAIL";
		
		m_strMessage = "runHallSensorCloseTest Command Fail.";
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		return false;
	}
	if(strcmp(sz_outData, "") == 0)
	{
		m_strResult = "FAIL";
		m_strMessage = "runHallSensorCloseTest Command return empty.";
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		return false;
	}

	sscanf(sz_outData,"%d", &nHallSensorValue);

	if ( nHallSensorValue != 0 ){ //  0 ==> pass
		m_strResult = "FAIL";
		m_strMessage = "runHallSensorCloseTest Fail";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData(str_Pics, "FAIL");
		FactoryLog();
		return false;

	}

	m_strResult = "PASS";
	m_strMessage = "runHallSensorCloseTest Pass.";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData(str_Pics, "PASS");
	FactoryLog();
	return true;
}

bool CHallSensor::runHallSensorOpenTest(void)
{
	int nHallSensorValue = 0;

	if (SendJigRequest("OFF") != true)
	{
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Jig_Move_Fail;
		m_strMessage = "Jig Request(OFF) Fail!";
		m_strUpper = "";
		m_strLower = "";
		m_strMeasured = "";
		TraceLog(MSG_INFO, "Jig Request(OFF) Fail!");
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, "Jig Request(OFF) Pass!");

	m_strItemCode  = CStr::IntToStr(HallSensor_Calibration_BaseItemCode);

	char sz_inData[FTD_BUF_SIZE] = "117";
	char sz_outData[FTD_BUF_SIZE] = {0};
	if (! (m_pIPhone->FTD_Get_GPIO_Value(m_nFtdPort, m_nFtdTimeOut, sz_inData, sz_outData)) )
	{
		m_strResult = "FAIL";
		
		m_strMessage = "runHallSensorOpenTest Command Fail.";
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		return false;
	}
	if(strcmp(sz_outData, "") == 0)
	{
		m_strResult = "FAIL";
		m_strMessage = "runHallSensorOpenTest Command return empty.";
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		return false;
	}

	sscanf(sz_outData,"%d", &nHallSensorValue);

	if ( nHallSensorValue != 1 ){ //  1 == >> pass
		m_strResult = "FAIL";
		m_strMessage = "runHallSensorOpenTest Fail";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData(str_Pics, "FAIL");
		FactoryLog();
		return false;

	}

	m_strResult = "PASS";
	m_strMessage = "runHallSensorOpenTest Pass.";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData(str_Pics, "PASS");
	FactoryLog();
	return true;
}

bool CHallSensor::SendJigRequest(CString str_command)
{
	/*don't enable JIG for RD test, SKIP the section*/
	if (m_i_EnableJIG == 0)
	{
		return true;
	}

	/* Init wait flag to false */
	m_b_waitFlag = false;

	/* Send Jig request */
	JigRequest(str_command);

	/* Wait response(Max:10s) */
	int i_waitCount = 0;
	while (m_b_waitFlag != true)
	{
		if (i_waitCount == 50)
		{
			TraceLog(MSG_INFO, "Jig Request Response timeout!");
			return false;
		}
		else
		{
			i_waitCount++;
			Sleep(200);
			continue;
		}
	}

	/* Get Jig result */
	if (m_b_jigRequest == false)
	{
		TraceLog(MSG_INFO, "Jig ON/OFF Fail!");
		return false;
	}

	/* Sleep for Jig */
	int dw_sleepTime = 0;
	dw_sleepTime = m_i_sleep * 1000;
	Sleep(dw_sleepTime);

	TraceLog(MSG_INFO, "Jig Request Pass!");
	return true;
}

