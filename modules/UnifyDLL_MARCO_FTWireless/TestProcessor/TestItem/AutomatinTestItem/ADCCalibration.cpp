/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .cpp file of ADCCalibration object.
 * FileName   : ADCCalibration.cpp
 * Version    : $Revision:   1.0  $
 * Author     : CY.Lee
 * Time       : 2012/09/10
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/09/10       CY.Lee                          Create.
 ********************************************************************************/

#include "stdafx.h"
#include "ADCCalibration.h"


RF_IMPLEMENT_DYNCREATE(ADCCalibration)


bool ADCCalibration::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool ADCCalibration::Run()
{
	return Check_SWversion();
}

bool ADCCalibration::PostRun()
{
	return true;
}

bool ADCCalibration::InitData(std::map<std::string, std::string>& paramMap)
{
	//timeout
	if (paramMap.find(std::string("Timeout")) != paramMap.end())
		m_str_Timeout = paramMap[std::string("Timeout")];
	else
	{
		m_str_Timeout = "10";
		TraceLog(MSG_INFO, _T("Failed to Get Timeout for class 'ADCCalibration', default set 10 seconds"));;
	}

	//cycle
	if (paramMap.find(std::string("Cycle")) != paramMap.end())
		m_str_Cycle = paramMap[std::string("Cycle")];
	else
	{
		m_str_Cycle = "15";
		TraceLog(MSG_INFO, _T("Failed to Get Timeout for class 'ADCCalibration', default set 15 cycles"));
	}

	//filter
	if (paramMap.find(std::string("Filter")) != paramMap.end())
		m_str_Filter = paramMap[std::string("Filter")];
	else
	{
		m_str_Filter = "20";
		TraceLog(MSG_INFO, _T("Failed to Get Timeout for class 'ADCCalibration', default set 20"));
	}

	//delta
	//std::string str_Delta;
	if (paramMap.find(std::string("Delta")) != paramMap.end())
	{
		m_str_Delta = paramMap[std::string("Delta")];
		//m_i_Delta = CStr::StrToInt(str_Delta);
	}
	else
	{
		m_str_Delta = "50";
		TraceLog(MSG_INFO, _T("Failed to Get Timeout for class 'ADCCalibration', default set 50"));
	}

	return true;
}
bool ADCCalibration::Check_SWversion()
{
	std::string str_msg;

	//Check SWversion. if version=v51.005.03.g_03 or v51.006.03.g_03, skip this test item.
	char output[100] = {0};
	char error[100] = {0};

	if (!m_pIPhone->ExecAdbCommand("shell getprop ro.build.oemversion.main", output, error))
	{
		m_strMessage = "Get Software Version fail";
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Get_Software_Version_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}

	for(int i = 0; i < 100; i++)
	{
		if((output[i] == 0x0A) || (output[i] == 0x0D))
			output[i] = 0x00;
	}

	//if NOT "v51.005.03.g_03", "v51.006.03.g_03" do.
	if((strstr(output, "v51.005.03.g_03") == NULL) && (strstr(output, "v51.006.03.g_03") == NULL))
	{
		if(!ADC_Calibration())
			return false;
	}
	else
	{
		str_msg = _T("No ADC Calibration. ");
		str_msg = str_msg + output;
		TraceLog(MSG_INFO, output);
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		FactoryLog();
	}

	return true;
}

bool ADCCalibration::ADC_Calibration()
{
	char sz_inBuffer[512] = {0};
	char sz_outBuffer[512] = {0};

	// Initial ADC Calibration data //
	if (!m_pIPhone->FTD_AUinitADC(m_nFtdPort, m_nFtdTimeOut, sz_inBuffer, sz_outBuffer))
	{
		m_strResult = "FAIL";
		if(strcmp(sz_outBuffer, "CAL_OPEN_FAIL") == 0)
		{
			m_strErrorCode = FunErr_Headset_Test_Fail;
			m_strMessage = _T("Initial ADC open fail");
		}
		else if(strcmp(sz_outBuffer, "CAL_READ_FAIL") == 0)
		{
			m_strErrorCode = FunErr_Headset_Test_Fail;
			m_strMessage = _T("Initial ADC read fail");
		}
		else if(strcmp(sz_outBuffer, "CAL_WRITE_FAIL") == 0)
		{
			m_strErrorCode = FunErr_Headset_Test_Fail;
			m_strMessage = _T("Initial ADC write fail");
		}
		else if(strcmp(sz_outBuffer, "CAL_FA_WRITE_FAIL") == 0)
		{
			m_strErrorCode = CommErr_Write_Factory_Area_Fail;
			m_strMessage = _T("Initial ADC FA write fail");
		}
		else
		{
			m_strErrorCode = FunErr_No_Headset_But_Detect_It;
			m_strMessage = _T("Initial ADC unexpect fail");
		}
		m_strMeasured = sz_outBuffer;
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		return false;
	}
	else
	{
		m_strResult = "PASS";
		m_strMessage = _T("Initial ADC Calibration pass");
		m_strMeasured = _T("CAL_SUCCESS");
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
	}

	// Set PICS Data //
	SetPICSData(_T("ADC_INIT"), m_strMeasured);


	// Init dialog response flag to false //
	m_b_DialogResponse = false;
	m_b_waitFlag = false;
	// Notify UI to show message dialog //
	std::string msg = "请按下YES后再插上耳机";
	ShowDialog(msg.c_str());


	// ADC Calibration //
	strcpy(sz_inBuffer, m_str_Timeout.c_str());
	strcat(sz_inBuffer, _T(","));
	strcat(sz_inBuffer, m_str_Cycle.c_str());
	strcat(sz_inBuffer, _T(","));
	strcat(sz_inBuffer, m_str_Filter.c_str());
	strcat(sz_inBuffer, _T(","));
	strcat(sz_inBuffer, m_str_Delta.c_str());

	if (!m_pIPhone->FTD_AUADCCali(m_nFtdPort, m_nFtdTimeOut, sz_inBuffer, sz_outBuffer))
	{
		m_strResult = "FAIL";
		if(strcmp(sz_outBuffer, "CAL_OPEN_FAIL") == 0)
		{
			m_strErrorCode = FunErr_Headset_Test_Fail;
			m_strMessage = _T("ADC Calibration open fail");
		}
		else if(strcmp(sz_outBuffer, "CAL_READ_FAIL") == 0)
		{
			m_strErrorCode = FunErr_Headset_Test_Fail;
			m_strMessage = _T("ADC Calibration read fail");
		}
		else if(strcmp(sz_outBuffer, "CAL_WRITE_FAIL") == 0)
		{
			m_strErrorCode = FunErr_Headset_Test_Fail;
			m_strMessage = _T("ADC Calibration write fail");
		}
		else if(strcmp(sz_outBuffer, "CAL_PARAM_FAIL") == 0)
		{
			m_strErrorCode = CommErr_XML_Testitem_Parameter_Missed;
			m_strMessage = _T("ADC Calibration parameter fail");
		}
		else if(strcmp(sz_outBuffer, "CAL_HS_INSERT_FAIL") == 0)
		{
			m_strErrorCode = FunErr_Detect_Headset_Fail;
			m_strMessage = _T("ADC Calibration insert fail");
		}
		else if(strcmp(sz_outBuffer, "CAL_TIMEOUT_FAIL") == 0)
		{
			m_strErrorCode = FunErr_Headset_Timeout_Fail;
			m_strMessage = _T("ADC Calibration timeout fail");
		}
		else if(strcmp(sz_outBuffer, "CAL_CALIBRATE_FAIL") == 0)
		{
			m_strErrorCode = FunErr_Headset_Calibration_Fail;
			m_strMessage = _T("ADC Calibration K fail");
		}
		else if(strcmp(sz_outBuffer, "CAL_FA_WRITE_FAIL") == 0)
		{
			m_strErrorCode = CommErr_Write_Factory_Area_Fail;
			m_strMessage = _T("ADC Calibration FA write fail");
		}
		else
		{
			m_strErrorCode = FunErr_No_Headset_But_Detect_It;
			m_strMessage = _T("ADC Calibration unexpect fail");
		}
		m_strMeasured = sz_outBuffer;
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		return false;
	}
	else
	{
		m_strResult = "PASS";
		m_strMessage = _T("ADC Calibration pass");
		m_strMeasured = _T("CAL_SUCCESS");
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();

		//  Control Run logcat ADC Calibration  //
		CString str_command = _T("");
		CString str_msg = _T("");
		CString str_LogPath = _T("");
		CString str_Picasso = _T("RD");

		SYSTEMTIME systemTime;
		::GetLocalTime(&systemTime);
		str_LogPath.Format(_T("D:\\LOG\\Android\\%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);

		if(! g_strPicasso.empty() )
			str_Picasso = g_strPicasso.c_str();

		// Execute logcat file //
		str_command.Format(_T("mkdir %s"), str_LogPath);
		system(str_command);
		str_command.Format(_T("adb logcat -v time -d > %s\\logcat_%s_%04d%02d%02d_%02d%02d%02d.txt")
			, str_LogPath, str_Picasso, systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
		system(str_command);
		str_msg.Format(_T("Execute command: %s"), str_command);
		TraceLog(MSG_INFO, std::string(CT2A(str_msg)));
		Sleep(1000);
	}

	// Set PICS Data //
	SetPICSData(_T("ADC_CALI"), m_strMeasured);

	return true;
}