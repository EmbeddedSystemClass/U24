/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .cpp file of CCheckGSensorValue object.
 * FileName   : checkGSensorValue.cpp
 * Version    : $Revision:   1.0  $
 * Author     :  Lion.Wu
 * Time       :  2013/02/22
 ********************************************************************************/

#include "stdafx.h"
#include "checkGSensorValue.h"


RF_IMPLEMENT_DYNCREATE(CCheckGSensorValue)

bool CCheckGSensorValue::PreRun()
{
	m_strItemCode = CStr::IntToStr(ReadSNSRgsensorAcc_BaseItemCode);
	m_strErrorCode = FunErr_GSensor_Test_Fail;
	m_strResult = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CCheckGSensorValue::Run()
{
	bool bRes = false;
	bRes = checkGSensorValue();
	return bRes;
}

bool CCheckGSensorValue::PostRun()
{
	/* show hint and wait */
	Sleep(m_i_wait_for_k * 1000);
	return true;
}

bool CCheckGSensorValue::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string strSleepTime;
	std::string pstrRange;
	if (paramMap.find(std::string("GSensorSpec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("GSensorSpec")];
		cs_sz_GSensorSpec = pstrRange.c_str();
	}
	else
	{
		TraceLog(MSG_INFO, "Get GSensorSpec Fail.");
		return false;
	}

	if (paramMap.find(std::string("SleepTime")) != paramMap.end())
	{
		strSleepTime = paramMap[std::string("SleepTime")];
		m_i_wait_for_k = CStr::StrToInt(strSleepTime);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get SleepTime for class CCheckGSensorValue");
		return false;
	}
	return true;
}

bool CCheckGSensorValue::checkGSensorValue()
{
	bool b_res = false;
	CString msg;
	std::string str_msg;
	std::string str_msgGsensorData;

	double dGSensorSpecX, dGSensorSpecY, dGSensorSpecZ;
	double dGSensorValueX, dGSensorValueY, dGSensorValueZ;
	bool b_GSensorSpec;

	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_SNSRgsensorAcc(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_outData)))
	{
		str_msg = "FTD_SNSRgsensorAcc Command fail";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		CString sOutBuffer;
		sOutBuffer = sz_outData;
		m_strMeasured = sz_outData;
		sscanf(sz_outData, "%lf,%lf,%lf", &dGSensorValueX, &dGSensorValueY, &dGSensorValueZ);
		str_msgGsensorData = sz_outData;
	}

	/*check range*/
	if (b_res)
	{
		/*set value from xml*/
		sscanf(cs_sz_GSensorSpec, "%lf,%lf,%lf", &dGSensorSpecX, &dGSensorSpecY, &dGSensorSpecZ);
		m_obj_GSensorX.SetRange(dGSensorSpecX - 1, dGSensorSpecX + 1);
		m_obj_GSensorY.SetRange(dGSensorSpecY - 1, dGSensorSpecY + 1);
		m_obj_GSensorZ.SetRange(dGSensorSpecZ - 1, dGSensorSpecZ + 1);

		//	dGSensorValueX
		b_GSensorSpec = m_obj_GSensorX.InRange(dGSensorValueX);
		if (b_GSensorSpec != true)	b_res = b_GSensorSpec;

		b_GSensorSpec = m_obj_GSensorY.InRange(dGSensorValueY);
		if (b_GSensorSpec != true)	b_res = b_GSensorSpec;

		b_GSensorSpec = m_obj_GSensorZ.InRange(dGSensorValueZ);
		if (b_GSensorSpec != true)	b_res = b_GSensorSpec;
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "GSENSOR_CALIBRATION_VALUE_TEST pass!";
		TraceLog(MSG_INFO, "GSENSOR_CALIBRATION_VALUE_TEST pass!");
		SetPICSData("GSENSOR_CALIBRATION_VALUE_TEST", str_msgGsensorData);
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "GSENSOR_CALIBRATION_VALUE_TEST fail: " + msg;
		TraceLog(MSG_INFO, "GSENSOR_CALIBRATION_VALUE_TEST fail");
		SetPICSData("GSENSOR_CALIBRATION_VALUE_TEST", "FAIL");
	}
	FactoryLog();
	return b_res;
}
