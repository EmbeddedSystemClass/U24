
#include "stdafx.h"
#include "PSensorCalibration.h"

RF_IMPLEMENT_DYNCREATE(CBostonPSensorCalibration)

bool CBostonPSensorCalibration::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string pstrRange;
	if (paramMap.find(std::string("PSensorCalibrationNearSpec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("PSensorCalibrationNearSpec")];
		m_obj_PSensorNearSpec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, "Get PSensorCalibrationNearSpec Fail.");
		return false;
	}

	if (paramMap.find(std::string("PSensorCalibrationFarSpec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("PSensorCalibrationFarSpec")];
		m_obj_PSensorFarSpec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, "Get PSensorCalibrationFarSpec Fail.");
		return false;
	}

	/* Init to 5s for wait Jig*/
	m_i_sleep_for_Jig = 5;

	std::string str;
	if (paramMap.find(std::string("SleepTime")) != paramMap.end())
	{
		str = paramMap[std::string("SleepTime")];
		m_i_sleep_for_Jig = CStr::StrToInt(str);
	}
	else
	{
		TraceLog(MSG_INFO, "Set SleepTime for 5s by default.");
	}

	/* Init to 6s */
	m_i_wait_for_k = 6000;

	std::string strWaitForK;
	if (paramMap.find(std::string("WaitForK")) != paramMap.end())
	{
		strWaitForK = paramMap[std::string("WaitForK")];
		m_i_wait_for_k = CStr::StrToInt(strWaitForK);
	}
	else
	{
		TraceLog(MSG_INFO, "Set Calibration delay for 6000ms by default.");
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

bool CBostonPSensorCalibration::PreRun()
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

bool CBostonPSensorCalibration::Run()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone = NULL");
		return false;
	}

	return PSensorCalibrationProc();
}

bool CBostonPSensorCalibration::PostRun()
{
	return true;
}


bool CBostonPSensorCalibration::PSensorCalibrationProc(void)
{
	char* delim = ",";
	char* str_data[3];

	m_strMeasured = "";
	std::string str_K_far_value;
	std::string str_K_near_value;

	m_strItemCode  = CStr::IntToStr(PSensor_Calibration_BaseItemCode);
	/*===============================================================================*/
	/*================================= Calibration =================================*/
	/*===============================================================================*/
	bool K_success = false;
	for (int i = 0; i < m_i_retry; i++)
	{
		if (SendJigRequest("OFF") != true) // k far
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

		if (i > 0)
		{
			Sleep(m_i_wait_for_k);
		}

		/*for RD test , show messagebox  */
		if (m_i_EnableJIG == 0)
		{
			AfxMessageBox(_T("K FAR"), MB_OK | MB_ICONEXCLAMATION);
		}

		/*=============================== K FAR ===============================*/
		char sz_outData[FTD_BUF_SIZE] = {0};
		if (!m_pIPhone->FTD_SNSRpsensorCalib(m_nFtdPort, m_nFtdTimeOut, "f", sz_outData))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_PSensor_Calibration_Fail;
			m_strMessage = "PSensor Far Calibration Fail!";
			TraceLog(MSG_INFO, "PSensor Far Calibration Fail!");
			FactoryLog();
			return false;
		}
		SetPICSData("PSensor_K_FAR", sz_outData);
		CString cs_PSensorFar;
		cs_PSensorFar.Format("%s, %s", _T("PSensor_K_FAR"), sz_outData);
		TraceLog(MSG_INFO, std::string(cs_PSensorFar));
		m_strMeasured = cs_PSensorFar;
		FactoryLog();

		/*k far, offset, non k far*/
		str_data[0] = strtok(sz_outData, delim);	//PASSED or FAILED
		str_data[1] = strtok(NULL, delim);			//Far
		str_data[2] = strtok(NULL, delim);			//Value
		if (!(strcmp(str_data[0], "PASSED") == 0))
		{
			m_strMeasured = "-";
			m_strResult = "FAR";
			m_strMessage = "PSensor far calibration Retrying...";
			FactoryLog();
			K_success = false;
			continue;
		}

		// show hint and wait
		str_K_far_value = str_data[1];
		m_strMeasured = str_data[1];
		m_strResult = "FAR";
		m_strMessage = "PSensor far calibration pass";
		//FactoryLog();
		Sleep(m_i_wait_for_k);

		/*for RD test , show messagebox  */
		if (m_i_EnableJIG == 0)
		{
			AfxMessageBox(_T("K NEAR"), MB_OK | MB_ICONEXCLAMATION);
		}

		/*=============================== K NEAR ===============================*/
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

		memset(sz_outData, 0, sizeof(sz_outData));
		if (!m_pIPhone->FTD_SNSRpsensorCalib(m_nFtdPort, m_nFtdTimeOut, "n", sz_outData))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_PSensor_Calibration_Fail;
			m_strMessage = "PSensor Near Calibration Fail!";
			TraceLog(MSG_ERROR, "PSensor Near Calibration Fail!");
			FactoryLog();
			return false;
		}
		SetPICSData("PSensor_K_NEAR", sz_outData);
		CString cs_PSensorNEAR;
		cs_PSensorNEAR.Format(_T("%s, %s"), _T("PSensor_K_NEAR"), sz_outData);
		TraceLog(MSG_INFO, cs_PSensorNEAR.GetString());
		m_strMeasured = cs_PSensorNEAR;
		FactoryLog();

		/*k far, offset, non k far*/
		str_data[0] = strtok(sz_outData, delim);	//PASSED or FAILED
		str_data[1] = strtok(NULL, delim);			//Near
		str_data[2] = strtok(NULL, delim);			//Value
		if (strcmp(str_data[0], "PASSED") != 0)
		{
			m_strMeasured = "-";
			m_strResult = "NEAR";
			m_strMessage = "PSensor near calibration Retrying...";
			FactoryLog();
			K_success = false;
			continue;
		}
		K_success = true;
		break;
	}

	//If retry N times all fail
	if (!K_success)
	{
		m_strMeasured = "-";
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_PSensor_Calibration_Fail;
		m_strMessage = "PSensor Calibration Fail!";
		TraceLog(MSG_ERROR, "PSensor Calibration Fail!");
		FactoryLog();
		return false;
	}
	str_K_near_value = str_data[1];
	m_strMeasured = str_data[1];
	m_strResult = "NEAR";
	m_strMessage = "PSensor NEAR calibration pass";
	//FactoryLog();

	if (checkPSensorValue(str_K_far_value, str_K_near_value))
	{
		SetPICSData("PSensor_NEAR_VERIFY", "PASS");
	}
	else
	{
		SetPICSData("PSensor_NEAR_VERIFY", "FAIL");
		return false;
	}
	return true;
}

bool CBostonPSensorCalibration::checkPSensorValue(std::string cs_farValue, std::string cs_nearValue)
{
	bool b_Res = true;
	double dw_value = 0;
	//----------------------------Compare the spec with FAR K----------------------------
	m_strMeasured = cs_farValue + _T(",") + cs_nearValue;
	m_strUpper = CStr::IntToStr((int)m_obj_PSensorFarSpec.GetMax());
	m_strLower = CStr::IntToStr((int)m_obj_PSensorFarSpec.GetMin());
	dw_value = CStr::StrToDouble(cs_farValue);
	if (!m_obj_PSensorFarSpec.InRange(dw_value))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_PSensor_Range_Check_Fail;
		m_strMessage = "PSensor Calibration FAR Fail! FAR: " + cs_farValue;
		TraceLog(MSG_INFO, "PSensor Calibration FAR is not in the spec. FAR: " + cs_farValue);
		FactoryLog();
		return false;
	}
	m_strResult = "PASS";
	m_strMessage = "Check PSensor Calibration FAR Pass!";
	TraceLog(MSG_INFO, "Check PSensorCalibration FAR in the spec.");
	FactoryLog();

	//----------------------------Compare the spec with NEAR K----------------------------
	m_strUpper = CStr::IntToStr((int)m_obj_PSensorNearSpec.GetMax());
	m_strLower = CStr::IntToStr((int)m_obj_PSensorNearSpec.GetMin());
	dw_value = CStr::StrToDouble(cs_nearValue);
	if (!m_obj_PSensorNearSpec.InRange(dw_value))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_PSensor_Range_Check_Fail;
		m_strMessage = "PSensor Calibration NEAR Fail! NEAR: " + cs_nearValue;
		TraceLog(MSG_INFO, "PSensor Calibration NEAR is not in the spec. NEAR: " + cs_nearValue);
		FactoryLog();
		return false;
	}
	m_strResult = "PASS";
	m_strMessage = "Check PSensor Calibration NEAR Pass!";
	TraceLog(MSG_INFO, "Check PSensorCalibration NEAR in the spec.");
	FactoryLog();
	TraceLog(MSG_INFO, "Psensor K: " + m_strMeasured);

	//Write log for calibration
	CString cstrLogCal;
	cstrLogCal.Format(_T("K: %s"), m_strMeasured.c_str());
	Log_Value(cstrLogCal);
	return b_Res;
}

bool CBostonPSensorCalibration::SendJigRequest(CString str_command)
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
	dw_sleepTime = m_i_sleep_for_Jig * 1000;
	Sleep(dw_sleepTime);

	TraceLog(MSG_INFO, "Jig Request Pass!");
	return true;
}

void CBostonPSensorCalibration::Log_Value(CString message)
{
	SYSTEMTIME tLocal;
	::GetLocalTime(&tLocal);
	CString Logtime;
	Logtime.Format(_T("%04d%02d%02d-%02d:%02d:%02d"),
	               tLocal.wYear, tLocal.wMonth, tLocal.wDay,
	               tLocal.wHour, tLocal.wMinute, tLocal.wSecond);

	CString pthLog(_T("D:\\Log\\UI_RD"));
	if (::GetFileAttributes(pthLog) == INVALID_FILE_ATTRIBUTES)
		CreateDirectory(pthLog, NULL);

	pthLog = _T("D:\\Log\\UI_RD\\PsensorValue.CSV");
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
