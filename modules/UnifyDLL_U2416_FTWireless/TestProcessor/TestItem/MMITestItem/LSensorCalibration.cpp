
#include "stdafx.h"
#include "LSensorCalibration.h"

RF_IMPLEMENT_DYNCREATE(CLSensorCalibration)

bool CLSensorCalibration::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string pstrRange;

	if (paramMap.find(std::string("LSensorCalibrationGainSpec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("LSensorCalibrationGainSpec")];
		m_obj_LSensorGainSpec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, "Get LSensorCalibrationGainSpec Fail.");
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

	// Init Standard lux is 500 lux
	m_i_StandardLux = 500;
	std::string strStandardLux;
	if (paramMap.find(std::string("StandardLux")) != paramMap.end())
	{
		strStandardLux = paramMap[std::string("StandardLux")];
		m_i_StandardLux = CStr::StrToInt(strStandardLux);
	}
	else
	{
		TraceLog(MSG_INFO, "Set StandardLux 500 lux by default.");
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

bool CLSensorCalibration::PreRun()
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

bool CLSensorCalibration::Run()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone = NULL");
		return false;
	}
	return LSensorCalibrationProc();
}

bool CLSensorCalibration::PostRun()
{
	return true;
}

bool CLSensorCalibration::LSensorCalibrationProc(void)
{
	char *delim =",";
	char *str_data[3];

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
	m_strItemCode  = CStr::IntToStr(LSensor_Calibration_BaseItemCode);

	/*===============================================================================*/
	/*================================= Calibration =================================*/
	/*===============================================================================*/
	char sz_inData[FTD_BUF_SIZE] = {0};
	itoa(m_i_StandardLux, sz_inData, 10);
	char sz_outData[FTD_BUF_SIZE] = {0};
	if (! (m_pIPhone->FTD_SNSRlsensorCalib(m_nFtdPort, m_nFtdTimeOut, sz_inData, sz_outData)) )
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_LSensor_Calibration_Fail;
		m_strMessage = "LSensor Calibration Command Fail.";
		TraceLog(MSG_INFO, "LSensor Calibration Command Fail.");
		FactoryLog();
		return false;
	}
	if(strcmp(sz_outData, "") == 0)
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_LSensor_Calibration_Fail;
		m_strMessage = "LSensor Calibration Command return empty.";
		TraceLog(MSG_INFO, "LSensor Calibration Command return empty.");
		FactoryLog();
		return false;
	}

	//Write log for calibration
	CString cstrLogCal;
	cstrLogCal.Format(_T("K: %s"), sz_outData);
	Log_Value(cstrLogCal);
	m_strMeasured = sz_outData;

	std::string strLogResult;
	strLogResult = sz_outData;
	TraceLog(MSG_INFO, "Lsensor K: " + strLogResult);
	str_data[0] = strtok(sz_outData, delim);	//PASSED or FAILED
	str_data[1] = strtok(NULL, delim);			//Para x
	str_data[2] = strtok(NULL, delim);			//Para y
	if(str_data[0] == NULL)
		str_data[0] = "FAILED";

	std::string str_Gain_para_x;
	std::string str_Gain_para_y;
	double d_Gain_para_x;
	double d_Gain_para_y;
	if(str_data[1] == NULL)
	{
		d_Gain_para_x = 9999;
	}
	else
	{
		str_Gain_para_x = str_data[1];
		d_Gain_para_x = CStr::StrToDouble(str_Gain_para_x);
	}

	if(str_data[2] == NULL)
	{
		d_Gain_para_y = 1;
	}
	else
	{
		str_Gain_para_y = str_data[2];
		d_Gain_para_y = CStr::StrToDouble(str_Gain_para_y);
	}
	if(!(strcmp(str_data[0], "PASSED") == 0))
	{
		m_strErrorCode = FunErr_LSensor_Calibration_Fail;
		m_strMeasured = "-";
		m_strMessage = "LSensor Calibration Fail.";
		TraceLog(MSG_INFO, "LSensor Calibration Fail.");
		SetPICSData("LSensor_K", "FAIL");
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, "LSensor Calibration Pass!");
	SetPICSData("LSensor_K", strLogResult);
	FactoryLog();
	Sleep(1000);

	/*===============================================================================*/
	/*================================= Read  Value =================================*/
	/*===============================================================================*/
	memset(sz_outData, 0, sizeof(sz_outData));
	if(!m_pIPhone->FTD_SNSRlsensorValue(m_nFtdPort, m_nFtdTimeOut, sz_inData, sz_outData))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_LSensor_Verification_Fail;
		m_strMessage = "LSensor Verification Fail.";
		TraceLog(MSG_INFO, "LSensor Verification Fail.");
		FactoryLog();
		return false;
	}
	m_strMeasured = sz_outData;
	std::string str_value;
	str_value = _T("LSensor Read Value:");
	TraceLog(MSG_INFO, str_value + m_strMeasured);
	SetPICSData("LSensor_VERIFY", m_strMeasured);

	/*===============================================================================*/
	/*================================ Compare Spec =================================*/
	/*===============================================================================*/
	//Now is eat uncover range
	double dw_value = 0;
	dw_value = d_Gain_para_x / d_Gain_para_y;
	char c_Measured[10] = {0};
	sprintf(c_Measured, "%f", dw_value);
	m_strMeasured = c_Measured;
	m_strUpper = CStr::FloatToStr(m_obj_LSensorGainSpec.GetMax());
	m_strLower = CStr::FloatToStr(m_obj_LSensorGainSpec.GetMin());
	if (!m_obj_LSensorGainSpec.InRange(dw_value))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_LSensor_Range_Check_Fail;
		m_strMessage = "LSensor GA is not in the spec.";
		TraceLog(MSG_INFO, "LSensor GA is not in the spec.");
		FactoryLog();
		return false;
	}
	m_strResult = "PASS";
	m_strMessage = "LSensor GA in the spec.";
	TraceLog(MSG_INFO, "LSensor GA in the spec.");
	FactoryLog();
	return true;
}

bool CLSensorCalibration::SendJigRequest(CString str_command)
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

void CLSensorCalibration::Log_Value(CString message)
{
	SYSTEMTIME tLocal;
	::GetLocalTime(&tLocal);
	CString Logtime;
	Logtime.Format(_T("%04d%02d%02d-%02d:%02d:%02d"),
		           tLocal.wYear, tLocal.wMonth, tLocal.wDay,
				   tLocal.wHour, tLocal.wMinute, tLocal.wSecond);

	CString pthLog(_T("D:\\Log\\UI_RD"));
	if (::GetFileAttributes(pthLog) != INVALID_FILE_ATTRIBUTES)
		CreateDirectory(pthLog, NULL);

	pthLog = _T("D:\\Log\\UI_RD\\LsensorValue.CSV");
	CStdioFile fLog;
	UINT nOpenFlags =
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyWrite;
	message = Logtime + _T("\t") + message + _T("\n");
	if (fLog.Open(pthLog, nOpenFlags))
	{
		fLog.SeekToEnd();
		fLog.WriteString(message);
		fLog.Close();
	}
}
