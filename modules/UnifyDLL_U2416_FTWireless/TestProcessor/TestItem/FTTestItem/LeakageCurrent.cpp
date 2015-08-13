/*********************************************************************************
* COPYRIGHT Qisda WR24 2014
*
* Description: .cpp file of CLeakageCurrent object.
* FileName   : CLeakageCurrent.cpp
* Version    : $Revision:   1.3  $
* Author     : Eason
* Time       : 2014/08/08
* History    :
*  NO      Date        Updated by        Content of change
*  1    2014/08/08      Eason              Create.
********************************************************************************/

#include "stdafx.h"
#include "LeakageCurrent.h"


RF_IMPLEMENT_DYNCREATE(CLeakageCurrent)


bool CLeakageCurrent::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = CStr::DblToStr(m_rp_Value.GetMax());
	m_strLower = CStr::DblToStr(m_rp_Value.GetMin());
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CLeakageCurrent::Run()
{
	if (m_str_Name == dLeakageCurrent)	
	{
		m_strItemCode  = CStr::IntToStr(LeakageCurrent_BaseItemCode);
		m_strErrorCode = FunErr_Leakage_Current_Test_Fail; 
		return CheckLeakageCurrent();
	}	
	else
	{
		m_strMessage = "The Type of ID is not defined";
		TraceLog(MSG_INFO, "The Type of ID is not defined");
		return false;
	}
}

bool CLeakageCurrent::PostRun()
{
	return true;
}

bool CLeakageCurrent::InitData(std::map<std::string, std::string>& paramMap)
{

	/* Name */
	if (paramMap.find(std::string("Name")) != paramMap.end())
	{
		m_str_Name = paramMap[std::string("Name")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'Name' for class 'CLeakageCurrent'");
		return false;
	}

	/* Value */
	if (paramMap.find(std::string("Value")) != paramMap.end())
	{
		m_str_Value = paramMap[std::string("Value")];
		m_rp_Value.SetRange(m_str_Value);
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'CheckMatch' for class 'CLeakageCurrent'");
		return false;
	}

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
		TraceLog(MSG_INFO, "Failed to Get filed 'CheckMatch' for class 'CLeakageCurrent'");
		return false;
	}
	/* WaitTimeForShutdown */
	std::string str_WaitTimeForShutdown;
	if (paramMap.find(std::string("WaitTimeForShutdown")) != paramMap.end())
	{
		str_WaitTimeForShutdown = paramMap[std::string("WaitTimeForShutdown")];
		m_iWaitTimeForShutdown = CStr::StrToInt(str_WaitTimeForShutdown);
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'WaitTimeForShutdown' for class 'CLeakageCurrent'");
		return false;
	}
	/* WaitTimeForFetch */
	std::string str_WaitTimeForFetch;
	if (paramMap.find(std::string("WaitTimeForFetch")) != paramMap.end())
	{
		str_WaitTimeForFetch = paramMap[std::string("WaitTimeForFetch")];
		m_iWaitTimeForFetch = CStr::StrToInt(str_WaitTimeForFetch);
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'WaitTimeForFetch' for class 'CLeakageCurrent'");
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
		TraceLog(MSG_INFO, "Failed to Get filed 'Sleep' for class 'CLeakageCurrent'");
		return false;
	}
	return true;
}

bool CLeakageCurrent::CheckLeakageCurrent()
{

	bool b_res = true;
	double dCurrent;
	std::string str_msg;
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};


	if ( b_res && m_pIPS == NULL )
	{
		str_msg = "Power supply1 not detected";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}
	if ( b_res && m_pIPS2 == NULL )
	{
		str_msg = "Power supply2 not detected";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	//----------------------------------------------------------------------------
	if ( b_res && !m_pIPS2->SetPSOnOff(false))
	{
		str_msg = "Fail to set PS2 off";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	if ( b_res && !m_pIPS->SetPSOnOff(false))
	{
		str_msg = "Fail to set PS off";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	Sleep(m_iWaitTimeForShutdown);
	//----------------------------------------------------------------------------
	if ( b_res && !m_pIPS->SetCurrRangeMin())
	{
		str_msg = "Fail to set power supply current range to minimum";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	if ( b_res && ! m_pIPS->SetPSOnOff(true))
	{
		str_msg = "Fail to set power supply on";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}
	Sleep(m_iWaitTimeForFetch);
	//----------------------------------------------------------------------------
	if ( b_res )
	{
		int iRetryTimes = 10;
		for ( int i = 0; i < iRetryTimes; ++i )
		{
			if (! m_pIPS->GetCurrent(dCurrent))
			{
				str_msg = "Fail to get current value from power supply";
				TraceLog(MSG_INFO, str_msg);
				m_strMessage = str_msg;
				
				return false;
			}

			dCurrent *= 1000.0;

			char szMeasured[256];
			memset(szMeasured, 0, 256);
			sprintf_s(szMeasured, 255, "%.3f", dCurrent);
			m_strMeasured = szMeasured;

			char szTmp[256];
			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, "%0d - Leakage Current : %s", i + 1, szMeasured);
			str_msg = szTmp;
			TraceLog(MSG_INFO, str_msg);

			double f_Value;
			f_Value = CStr::StrToDouble(m_strMeasured);
			if ( m_rp_Value.InRange(f_Value) )
			{
				str_msg = "Check Leakage current pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
				b_res = true;
				break;
			}
			else
			{
				if (i < iRetryTimes - 1)
				{
					str_msg = "Leakage current not in range, try again";
					TraceLog(MSG_ERROR, str_msg);
					Sleep(1000);
				}
				else
				{
					str_msg = "Leakage current is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
					TraceLog(MSG_INFO, str_msg);
					b_res = false;
				}
			}		
		}
	}
	
	Sleep(m_i_sleep);
	//* test result */
	m_strMessage = str_msg;

	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		TraceLog(MSG_INFO, "Read Leakage current  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_INFO, "Read Leakage current  FAIL!");
	}
	SetPICSData("Leakage_current", m_strMeasured);
	FactoryLog();
	return b_res;

}
