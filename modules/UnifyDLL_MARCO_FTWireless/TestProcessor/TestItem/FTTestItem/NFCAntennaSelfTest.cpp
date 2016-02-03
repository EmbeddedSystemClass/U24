/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .cpp file of CNFCAntennaSelfTest object.
 * FileName   : NFCAntennaSelfTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/01/16
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2013/01/16    Evonne.Hsieh          Create.
 ********************************************************************************/

#include "stdafx.h"
#include "NFCAntennaSelfTest.h"


RF_IMPLEMENT_DYNCREATE(CNFCAntennaSelfTest)

bool CNFCAntennaSelfTest::PreRun()
{
	m_strItemCode = CStr::IntToStr(NFCMODEL_Test_BaseItemCode);
	m_strErrorCode = FunErr_NFC_Antenna_Selftest_Fail;
	m_strResult = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper =
		CStr::FloatToStr(m_f_UpperSpec[0]) + "," +
		CStr::FloatToStr(m_f_UpperSpec[1]) + "," +
		CStr::FloatToStr(m_f_UpperSpec[2]) + "," +
		CStr::FloatToStr(m_f_UpperSpec[3]);
	m_strLower =
		CStr::FloatToStr(m_f_LowerSpec[0]) + "," +
		CStr::FloatToStr(m_f_LowerSpec[1]) + "," +
		CStr::FloatToStr(m_f_LowerSpec[2]) + "," +
		CStr::FloatToStr(m_f_LowerSpec[3]);
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CNFCAntennaSelfTest::Run()
{
	bool bRes = false;
	bRes = NFCAntennaSelfTest();
	return bRes;
}

bool CNFCAntennaSelfTest::PostRun()
{
	return true;
}

bool CNFCAntennaSelfTest::InitData(std::map<std::string, std::string>& paramMap)
{
	StrVtr vtrUpperSpec;
	std::string str_UpperSpec;
	if (paramMap.find(std::string("UpperSpec")) != paramMap.end())
	{
		str_UpperSpec = paramMap[std::string("UpperSpec")];
		vtrUpperSpec.clear();
		CStr::ParseString(str_UpperSpec, _T(","), vtrUpperSpec);
		if (vtrUpperSpec.size() != 4)
		{
			TraceLog(MSG_ERROR, "Get parameter 'UpperSpec' for class 'CNFCAntennaSelfTest' is not correct number 4");
			return false;
		}
		m_f_UpperSpec[0] = atof(vtrUpperSpec[0].c_str());
		m_f_UpperSpec[1] = atof(vtrUpperSpec[1].c_str());
		m_f_UpperSpec[2] = atof(vtrUpperSpec[2].c_str());
		m_f_UpperSpec[3] = atof(vtrUpperSpec[3].c_str());
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get filed 'UpperSpec' for class 'CNFCAntennaSelfTest' ");
		return false;
	}

	StrVtr vtrLowerSpec;
	std::string str_LowerSpec;
	if (paramMap.find(std::string("LowerSpec")) != paramMap.end())
	{
		str_LowerSpec = paramMap[std::string("LowerSpec")];
		vtrLowerSpec.clear();
		CStr::ParseString(str_LowerSpec, _T(","), vtrLowerSpec);
		if (vtrLowerSpec.size() != 4)
		{
			TraceLog(MSG_ERROR, "Get parameter 'LowerSpec' for class 'CNFCAntennaSelfTest' is not correct number 4");
			return false;
		}
		m_f_LowerSpec[0] = atof(vtrLowerSpec[0].c_str());
		m_f_LowerSpec[1] = atof(vtrLowerSpec[1].c_str());
		m_f_LowerSpec[2] = atof(vtrLowerSpec[2].c_str());
		m_f_LowerSpec[3] = atof(vtrLowerSpec[3].c_str());
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get filed 'LowerSpec' for class 'CNFCAntennaSelfTest' ");
		return false;
	}
	return true;
}

bool CNFCAntennaSelfTest::NFCAntennaSelfTest()
{
	bool b_res = true;
	std::string str_msg;

	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_NFCAntennaSelftest(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_outData)))
	{
		str_msg = "NFC Antenna Self Command fail";
		TraceLog(MSG_INFO, str_msg);
	}
	m_strMeasured = sz_outData;

	// compare spec
	char sz_Measured[32] = {0};
	strncpy(sz_Measured, m_strMeasured.c_str(), m_strMeasured.length());
	char *delim =",";
	char *Measured[4];
	Measured[0] = strtok(sz_Measured, delim);
	Measured[1] = strtok(NULL, delim);
	Measured[2] = strtok(NULL, delim);
	Measured[3] = strtok(NULL, delim);
	for (int i = 0; i < 4 ; i++)
	{
		double d_Measure= 0;
		d_Measure = atof(Measured[i]);
		if (d_Measure > m_f_UpperSpec[i] || d_Measure < m_f_LowerSpec[i])
		{
			b_res = false;
			break;
		}
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "NFC Antenna Self Test pass!";
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "NFC Antenna Self Test fail";
	}
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("NFC_ANTENNA_SELFTEST", m_strMeasured);
	FactoryLog();
	return b_res;
}
