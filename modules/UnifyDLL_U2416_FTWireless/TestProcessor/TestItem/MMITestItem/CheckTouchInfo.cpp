/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .cpp file of CCheckTouch object.
 * FileName   : BostonTouch.cpp
 * Version    : $Revision:   1.0  $
 * Author     :  Lion.Wu
 ********************************************************************************/

#include "stdafx.h"
#include "CheckTouchInfo.h"


RF_IMPLEMENT_DYNCREATE(CCheckTouch)

bool CCheckTouch::PreRun()
{
	m_strItemCode = CStr::IntToStr(TOUCHMODEL_Test_BaseItemCode);
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

bool CCheckTouch::Run()
{
	bool bRes = false;
	CString cs_TouchType = m_str_TouchType.c_str();

	if (cs_TouchType.Compare("TouchModel") == 0)
	{
		m_strErrorCode = FunErr_Touch_Model_Test_Fail;
		return readTouchModel();
	}
	else if (cs_TouchType.Compare("TouchModelLimitLength") == 0)
	{
		m_strErrorCode = FunErr_Touch_Model_Test_Fail;
		return readTouchModelLimitLength();
	}
	else if (cs_TouchType.Compare("TouchFWVersion") == 0)
	{
		m_strErrorCode = FunErr_Check_Touch_FW_Fail;
		return readTouchFWVersion();
	}
	return bRes;
}

bool CCheckTouch::PostRun()
{
	return true;
}

bool CCheckTouch::InitData(std::map<std::string, std::string>& paramMap)
{
	/* PatternIndex */
	if (paramMap.find(std::string("IDType")) != paramMap.end())
	{
		m_str_TouchType = paramMap[std::string("IDType")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter IDType for CCheckTouch");
		return false;
	}

	if (paramMap.find(std::string("ID")) != paramMap.end())
	{
		m_str_TouchData = paramMap[std::string("ID")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter ID for CCheckTouch");
		return false;
	}

	return true;
}

bool CCheckTouch::readTouchModelLimitLength()
{
	bool b_res = false;
	std::string str_msg;

	char sz_outData[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->ReadTouchModel(m_nFtdPort, m_nFtdTimeOut, sz_outData)))
	{
		str_msg = "ReadTouchModel Command fail";
		TraceLog(MSG_INFO, str_msg);
	}
	
	if (b_res)
	{
		char sz_TouchDataA[100] = {0};
		char sz_TouchDataB[100] = {0};
		char sz_TouchDataC[100] = {0};

		sscanf(sz_outData,"%[^','],%[^',']", sz_TouchDataA, sz_TouchDataB);
		strcat(sz_TouchDataC, sz_TouchDataA);
		strcat(sz_TouchDataC, _T(","));
		strcat(sz_TouchDataC, sz_TouchDataB);
		m_strMeasured = sz_TouchDataC;

		StrVtr vToken;
		CStr::ParseString(m_str_TouchData.c_str(), _T(";"), vToken);
		bool b_match = false;
		for (size_t i = 0; i < vToken.size(); i++)
		{
			if ((strcmp(sz_TouchDataC, vToken[i].c_str()) == 0))
			{
				str_msg = "Check Touch Model pass! ID = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
				b_match = true;
				break;
			}
		}
		if (!b_match)
		{
			str_msg = "Touch Model is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_TouchData;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}

	}
	
	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "readTouchModel pass!";
		TraceLog(MSG_INFO, "readTouchModel pass!");
		SetPICSData("readTouchModel", sz_outData);
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "readTouchModel fail";
		TraceLog(MSG_INFO, "readTouchModel fail");
		SetPICSData("readTouchModel", "FAIL");
	}
	FactoryLog();
	return b_res;
}

bool CCheckTouch::readTouchModel()
{
	bool b_res = false;
	std::string str_msg;

	char sz_outData[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->ReadTouchModel(m_nFtdPort, m_nFtdTimeOut, sz_outData)))
	{
		str_msg = "ReadTouchModel Command fail";
		TraceLog(MSG_INFO, str_msg);
	}
	m_strMeasured = sz_outData;
	if (b_res)
	{
		char sz_TouchDataA[100] = {0};
		char sz_TouchDataB[100] = {0};
		sscanf(m_str_TouchData.c_str(),"%[^:]:%s", sz_TouchDataA, sz_TouchDataB);
		b_res = ((strcmp(sz_outData ,sz_TouchDataA) == 0) || (strcmp(sz_outData ,sz_TouchDataB) == 0));
	}
	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "readTouchModel pass!";
		TraceLog(MSG_INFO, "readTouchModel pass!");
		SetPICSData("readTouchModel", sz_outData);
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "readTouchModel fail";
		TraceLog(MSG_INFO, "readTouchModel fail");
		SetPICSData("readTouchModel", "FAIL");
	}
	FactoryLog();
	return b_res;
}

bool CCheckTouch::readTouchFWVersion()
{
	bool b_res = false;
	std::string str_msg;

	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->TOUCHversion(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_outData)))
	{
		str_msg = "readTouchFWVersion Command fail";
		TraceLog(MSG_INFO, str_msg);
	}
	m_strMeasured = sz_outData;
	if (b_res)
	{
		char sz_TouchDataA[100] = {0};
		char sz_TouchDataB[100] = {0};
		sscanf(m_str_TouchData.c_str(),"%[^:]:%s", sz_TouchDataA, sz_TouchDataB);
		b_res = ((strcmp(sz_outData ,sz_TouchDataA) == 0) || (strcmp(sz_outData ,sz_TouchDataB) == 0));
	}
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "readTouchFWVersion pass!";
		TraceLog(MSG_INFO, "readTouchFWVersion pass!");
		SetPICSData("TouchFWVersion", sz_outData);
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "readTouchFWVersion fail";
		TraceLog(MSG_INFO, "readTouchFWVersion fail");
		SetPICSData("TouchFWVersion", "FAIL");
	}
	FactoryLog();
	return b_res;
}
