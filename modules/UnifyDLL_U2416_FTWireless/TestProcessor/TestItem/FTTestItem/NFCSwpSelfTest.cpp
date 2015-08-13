/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .cpp file of CNFCSwpSelfTest object.
 * FileName   : NFCSwpSelfTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/01/16
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2013/01/16    Evonne.Hsieh          Create.
 ********************************************************************************/

#include "stdafx.h"
#include "NFCSwpSelfTest.h"


RF_IMPLEMENT_DYNCREATE(CNFCSwpSelfTest)

bool CNFCSwpSelfTest::PreRun()
{
	m_strItemCode = CStr::IntToStr(NFCMODEL_Test_BaseItemCode);
	m_strErrorCode = FunErr_NFC_SWP_Selftest_Fail;
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

bool CNFCSwpSelfTest::Run()
{
	bool bRes = false;
	bRes = NFCSwpSelfTest();
	return bRes;
}

bool CNFCSwpSelfTest::PostRun()
{
	return true;
}

bool CNFCSwpSelfTest::InitData(std::map<std::string, std::string>& paramMap)
{
	//-- SWP Result --//
	if (paramMap.find(std::string("SWP_Result")) != paramMap.end())
	{
		m_str_SwpResult = paramMap[std::string("SWP_Result")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Get SWP Result Fail."));
		return false;
	}
	return true;
}

bool CNFCSwpSelfTest::NFCSwpSelfTest()
{
	bool b_res = false;
	std::string str_msg;
	int nRetry = 2;
	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};
	do {
		if (!(b_res = m_pIPhone->FTD_NFCSwpSelftest(m_nFtdPort, m_nFtdTimeOut,sz_input, sz_outData)))
		{
			str_msg = "NFC SWP Self Command fail";
			TraceLog(MSG_INFO, str_msg);
			break;
		}
		else
		{
			m_strMeasured = sz_outData;
			if (strncmp(m_strMeasured.c_str(), m_str_SwpResult.c_str(), 3) == 0)
			{
				b_res = true;
				break;
			}
			else
			{
				b_res = false;
			}
		}
		nRetry--;
	} while (nRetry > 0);

	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "NFC SWP Self Test pass!";
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "NFC SWP Self Test fail";
	}
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("NFC_SWP_SELF_TEST", m_strMeasured);
	FactoryLog();
	return b_res;
}
