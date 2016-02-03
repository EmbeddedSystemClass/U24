/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .cpp file of CGyroTest object.
 * FileName   : GyroTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     :  Lion.Wu
 * Time       : 2012/06/12
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2012/06/12    Lion.Wu         Create.
 ********************************************************************************/

#include "stdafx.h"
#include "GyroTest.h"


RF_IMPLEMENT_DYNCREATE(CGyroTest)

bool CGyroTest::PreRun()
{
	m_strItemCode = CStr::IntToStr(GYROTest_BaseItemCode);
	m_strErrorCode = FunErr_Gyro_Sensor_Test_Fail;
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

bool CGyroTest::Run()
{
	bool bRes = false;
	bRes = GyroTest();
	return bRes;
}

bool CGyroTest::PostRun()
{
	return true;
}

bool CGyroTest::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string pstrRange;
	if (paramMap.find(std::string("GyroSpec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("GyroSpec")];
		m_obj_GyroSpec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Get GyroSpec Fail."));
		return false;
	}
	return true;
}

bool CGyroTest::GyroTest()
{
	std::string str_msg;
	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};
	bool b_res = false;
	if (!(b_res = m_pIPhone->FTD_SNSRGyroscopeTest(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_outData)))
	{
		str_msg = _T("Touch Self Command fail");
		TraceLog(MSG_INFO, str_msg);
	}
	m_strMeasured = sz_outData;
	/*check range*/
	double nGyroX, nGyroY, nGyroZ;
	bool b_GyroSpec = false;
	CString cs_sz_outData;
	if (b_res)
	{
		nGyroX = nGyroY = nGyroZ = 0;
		cs_sz_outData = sz_outData;
		sscanf(cs_sz_outData, "%lf,%lf,%lf", &nGyroX, &nGyroY, &nGyroZ);
		b_GyroSpec = m_obj_GyroSpec.InRange(nGyroX);
		if (!b_GyroSpec) b_res = b_GyroSpec;

		b_GyroSpec = m_obj_GyroSpec.InRange(nGyroY);
		if (!b_GyroSpec) b_res = b_GyroSpec;

		b_GyroSpec = m_obj_GyroSpec.InRange(nGyroZ);
		if (!b_GyroSpec) b_res = b_GyroSpec;
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Gyroscope_Test pass!";
		TraceLog(MSG_INFO, "Gyroscope_Test pass!");
		SetPICSData("Gyroscope_Test", "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Gyroscope_Test fail";
		TraceLog(MSG_INFO, "Gyroscope_Test fail");
		SetPICSData("Gyroscope_Test", "FAIL");
	}
	FactoryLog();
	return b_res;
}
