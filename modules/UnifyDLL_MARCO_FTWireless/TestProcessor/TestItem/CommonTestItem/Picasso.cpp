/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .cpp file of CReadPicassoNum object.
 * FileName   : CReadPicassoNum.cpp
 * Version    : $Revision:   1.1  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/16
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/16      Gilpin.Xi             Create.
 ********************************************************************************/

#include "stdafx.h"
#include "Picasso.h"


RF_IMPLEMENT_DYNCREATE(CReadPicassoNum)


bool CReadPicassoNum::PreRun()
{
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CReadPicassoNum::Run()
{
	m_strItemCode = CStr::IntToStr(ReadPicasso_BaseItemCode);
	return ReadPicassoNum();
}

bool CReadPicassoNum::PostRun()
{
	return true;
}

bool CReadPicassoNum::InitData(std::map<std::string, std::string>& paramMap)
{
	/* CheckMatch */
	m_b_CheckMatch = true;

	std::string str_CheckMatch;
	if (paramMap.find(std::string("CheckMatch")) != paramMap.end())
	{
		str_CheckMatch = paramMap[std::string("CheckMatch")];
		m_b_CheckMatch = CStr::StrToBool(str_CheckMatch);
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'CheckMatch' for class 'CReadPicassoNum'");
		return false;
	}

	/* Sleep */
	std::string str_sleep;
	if (paramMap.find(std::string("Sleep")) != paramMap.end())
	{
		str_sleep = paramMap[std::string("Sleep")];
		m_i_sleep = CStr::StrToInt(str_sleep);
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'Sleep' for class 'CReadPicassoNum'");
		return false;
	}

	return true;
}

bool CReadPicassoNum::ReadPicassoNum()
{
	bool b_res = false;
	std::string str_msg;

	/* read PICASSO number from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_FAC_Picasso(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read PICASSO number from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Check PICASSO number */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read PICASSO = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			if (m_strMeasured.empty() == false)
			{
				str_msg = "Check PICASSO pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "PICASSO is empty.";
				TraceLog(MSG_INFO, str_msg);
				b_res = false;
			}
		}
	}
	Sleep(m_i_sleep);

	/* test result */
	m_strMessage = str_msg;
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		TraceLog(MSG_INFO, "Read PICASSO number PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_INFO, "Read PICASSO number FAIL!");
	}
	SetPICSData("PICASSO", m_strMeasured);
	FactoryLog();
	return b_res;
}
