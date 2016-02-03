/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CSapporoSmartCover object.
 * FileName   : CameraLEDTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2012/1210
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/1210    Lion.Wu            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "SapporoSmartCover.h"


RF_IMPLEMENT_DYNCREATE(CSapporoSmartCover)


bool CSapporoSmartCover::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(Smart_Cover_BaseItemCode);
	//m_strErrorCode = FunErr_SmartCover_Detect_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CSapporoSmartCover::Run()
{
	CString csSmartCoverTestItem = m_str_SmartCoverTestItem.c_str();
	bool b_res = false;

	if (csSmartCoverTestItem.Compare("CheckNoSCoverDetect") == 0)
	{
		m_strErrorCode = FunErr_SmartCover_Detect_Fail;
		return SCoverDetect("1"); //must return 1;
	}

	if (csSmartCoverTestItem.Compare("CheckNoSCoverInterrupt") == 0)
	{
		m_strErrorCode = FunErr_SmartCover_Interrupt_Fail;
		return SCoverInterrupt("1"); //must return 1;
	}

	if (csSmartCoverTestItem.Compare("CheckSCoverDetect") == 0)
	{
		m_strErrorCode = FunErr_SmartCover_Detect_Fail;
		return SCoverDetect(_T("0")); //must return 0;
	}

	if (csSmartCoverTestItem.Compare("CheckSCoverInterrupt") == 0)
	{
		m_strErrorCode = FunErr_SmartCover_Interrupt_Fail;
		return SCoverInterrupt(_T("0"));//must return 0;
	}

	if (csSmartCoverTestItem.Compare("CheckSCoverCheckI2C") == 0)
	{
		m_strErrorCode = FunErr_SmartCover_CheckI2C_Fail;
		return SCoverCheckI2C();
	}

	if (csSmartCoverTestItem.Compare("CheckSCoverSwitch5V") == 0)
	{
		m_strErrorCode = FunErr_SmartCover_Switch5V_Fail;
		return SCoverSwitch5V();
	}

	return true;
}

bool CSapporoSmartCover::PostRun()
{
	return true;
}

bool CSapporoSmartCover::InitData(std::map<std::string, std::string>& paramMap)
{
	/* PatternIndex */

	if (paramMap.find(std::string("SmartCoverTestItem")) != paramMap.end())
	{
		m_str_SmartCoverTestItem = paramMap[std::string("SmartCoverTestItem")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter SmartCoverTestItem for CSapporoSmartCover"));
		return false;
	}

	if (paramMap.find(std::string("SCoverCheckI2C_ID")) != paramMap.end())
	{
		m_str_SCoverCheckI2C_ID = paramMap[std::string("SCoverCheckI2C_ID")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter SCoverCheckI2C_ID for CSapporoSmartCover"));
	}

	if (paramMap.find(std::string("SCover_CMD")) != paramMap.end())
	{
		m_str_SCover_CMD = paramMap[std::string("SCover_CMD")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter SCover_CMD for CSapporoSmartCover"));
	}

	if (paramMap.find(std::string("SCover_Sleep")) != paramMap.end())
	{
		m_str_SCover_Sleep = paramMap[std::string("SCover_Sleep")];
		n_SCover_Sleep = atoi(m_str_SCover_Sleep.c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter SCover_Sleep for CSapporoSmartCover"));
	}


	return true;
}

bool CSapporoSmartCover::SCoverDetect(char* szFlag)
{
	bool b_res = true;
	m_str_Pics = "SCover_Detect";

	char sz_inBuffer[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	b_res = m_pIPhone->Sapporo_SCoverDetect(m_nFtdPort, m_nFtdTimeOut, sz_inBuffer, sz_outBuffer);

	if (!b_res)
	{
		TraceLog(MSG_INFO, "Sapporo Sapporo_SCoverDetect command fail");
	}
	else
	{
		TraceLog(MSG_INFO, "Sapporo Sapporo_SCoverDetect command success");
		CString cs_ReadBuffer = sz_outBuffer;
		cs_ReadBuffer.Trim();
		m_strMeasured = cs_ReadBuffer;
		b_res = (cs_ReadBuffer.Compare(szFlag) == 0);    // have smartcover
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Sapporo SCoverDetect PASS";
		TraceLog(MSG_INFO, m_strMessage);
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Sapporo SCoverDetect FAIL";
		TraceLog(MSG_INFO, m_strMessage);
	}
	SetPICSData(m_str_Pics, m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CSapporoSmartCover::SCoverInterrupt(char* szFlag)
{
	bool b_res = true;
	m_str_Pics = "SCover_Interrupt";

	char sz_inBuffer[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	b_res = m_pIPhone->Sapporo_SCoverInterrupt(m_nFtdPort, m_nFtdTimeOut, sz_inBuffer, sz_outBuffer);
	if (!b_res)
	{
		TraceLog(MSG_INFO, "Sapporo Sapporo_SCoverInterrupt command fail");
	}
	else
	{
		TraceLog(MSG_INFO, "Sapporo Sapporo_SCoverInterrupt command success");
		CString cs_ReadBuffer = sz_outBuffer;
		cs_ReadBuffer.Trim();
		m_strMeasured = cs_ReadBuffer;
		b_res = (cs_ReadBuffer.Compare(szFlag) == 0);// have SCoverInterrupt
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Sapporo SCoverInterrupt PASS";
		TraceLog(MSG_INFO, m_strMessage);
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Sapporo SCoverInterrupt FAIL";
		TraceLog(MSG_INFO, m_strMessage);
	}
	SetPICSData(m_str_Pics, m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CSapporoSmartCover::SCoverCheckI2C()
{
	bool b_res = true;
	m_str_Pics = "SCover_I2C";

	TraceLog(MSG_INFO, "CSapporoSmartCover check output 3.3V on.");
	char sz_inBuffer[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->Sapporo_SCoverEnable3V(m_nFtdPort, m_nFtdTimeOut, "1", sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strMessage = "CSapporoSmartCover Sapporo_SCoverEnable3V on fail.";
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, "CSapporoSmartCover Sapporo_SCoverEnable3V on success.");
	Sleep(n_SCover_Sleep);

	char c_CheckID_1[10] = {0};
	char c_CheckID_2[10] = {0};
	sscanf(m_str_SCoverCheckI2C_ID.c_str(), "%[^,],%s", c_CheckID_1, c_CheckID_2);
	sprintf(sz_inBuffer, "%s", m_str_SCover_CMD.c_str());
	b_res = m_pIPhone->Sapporo_SCoverCheckI2C(m_nFtdPort, m_nFtdTimeOut, sz_inBuffer, sz_outBuffer);
	if (!b_res)
	{
		TraceLog(MSG_INFO, "Sapporo SCoverCheckI2C command fail");
	}
	else
	{
		TraceLog(MSG_INFO, "Sapporo SCoverCheckI2C command success");
		CString cs_ReadBuffer_ID;
		cs_ReadBuffer_ID = sz_outBuffer;
		cs_ReadBuffer_ID.Trim();
		m_strMeasured = cs_ReadBuffer_ID;
		b_res = ((cs_ReadBuffer_ID.Compare(c_CheckID_2) == 0) ||
		         (cs_ReadBuffer_ID.Compare(c_CheckID_1) == 0));//SCoverCheckI2C
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = m_str_SCoverCheckI2C_ID + ", Sapporo SCoverCheckI2C PASS";
		TraceLog(MSG_INFO, m_strMessage);
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = m_str_SCoverCheckI2C_ID + ", Sapporo SCoverCheckI2C Fail!";
		TraceLog(MSG_INFO, m_strMessage);
	}
	SetPICSData(m_str_Pics, m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CSapporoSmartCover::SCoverSwitch5V()
{
	bool b_res = true;
	m_str_Pics = "SCover_Switch5V";

	char sz_inBuffer[FTD_BUF_SIZE] = {0};
	strcpy(sz_inBuffer, "1");
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	b_res = m_pIPhone->Sapporo_SCoverSwitch5V(m_nFtdPort, m_nFtdTimeOut, sz_inBuffer, sz_outBuffer);
	if (!b_res)
	{
		TraceLog(MSG_INFO, "Sapporo Sapporo_SCoverSwitch5V command fail");
	}
	else
	{
		TraceLog(MSG_INFO, "Sapporo Sapporo_SCoverSwitch5V command success");
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Sapporo SCoverSwitch5V PASS";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData(m_str_Pics, "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Sapporo SCoverSwitch5V FAIL";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData(m_str_Pics, "FAIL");
	}
	FactoryLog();
	return b_res;
}
