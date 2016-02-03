/*********************************************************************************
* COPYRIGHT Qisda WR24 2014
*
* Description: .cpp file of ChargeAverageCurrent object.
* FileName   : ChargeAverageCurrent.cpp
* Version    : $Revision:   1.3  $
* Author     : Eason
* Time       : 2014/08/08
* History    :
*  NO      Date        Updated by        Content of change
*  1    2014/08/08      Eason              Create.
********************************************************************************/

#include "stdafx.h"
#include "ChargeAverageCurrent.h"


RF_IMPLEMENT_DYNCREATE(ChargeAverageCurrent)


bool ChargeAverageCurrent::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	if ( m_iReadMode == 1 )
	{
		m_strUpper = CStr::DblToStr(m_rp_Value.GetMax());
		m_strLower = CStr::DblToStr(m_rp_Value.GetMin());
	}
	return true;
}

bool ChargeAverageCurrent::Run()
{
	if (m_str_Name == ChargingAverageCurrent)	
	{
		m_strItemCode  = CStr::IntToStr(Charging_Average_Current_BaseItemCode);
		m_strErrorCode = FunErr_Average_Current_Test_Fail; 
		return CheckSystemChargingAverageCurrent();
	}	
	else
	{
		m_strMessage = "The Type of ID is not defined";
		TraceLog(MSG_INFO, "The Type of ID is not defined");
		return false;
	}
}

bool ChargeAverageCurrent::PostRun()
{
	return true;
}

bool ChargeAverageCurrent::InitData(std::map<std::string, std::string>& paramMap)
{

/* Name */
	if (paramMap.find(std::string("Name")) != paramMap.end())
	{
		m_str_Name = paramMap[std::string("Name")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'Name' for class 'ChargeAverageCurrent'");
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
		TraceLog(MSG_INFO, "Failed to Get filed 'CheckMatch' for class 'ChargeAverageCurrent'");
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
		TraceLog(MSG_INFO, "Failed to Get filed 'CheckMatch' for class 'ChargeAverageCurrent'");
		return false;
	}
	/* Charge Time */
	std::string str_ChargeTime;
	if (paramMap.find(std::string("ChargeTime")) != paramMap.end())
	{
		str_ChargeTime = paramMap[std::string("ChargeTime")];
		m_i_ChargeTime = CStr::StrToInt(str_ChargeTime);	
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'ChargeTime' for class 'ChargeAverageCurrent'");
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
		TraceLog(MSG_INFO, "Failed to Get filed 'Sleep' for class 'ChargeAverageCurrent'");
		return false;
	}

	/* CheckType */
	std::string str_checkmode;
	if (paramMap.find(std::string("CheckMode")) != paramMap.end())
	{
		str_checkmode = paramMap[std::string("CheckMode")];
		m_iCheckMode = CStr::StrToInt(str_checkmode); 

	}
	else // No Data Use Standard 0 
	{
		m_iCheckMode = 0;
	}
	/* CheckType */
	std::string str_Readmode;
	if (paramMap.find(std::string("ReadMode")) != paramMap.end())
	{
		str_Readmode = paramMap[std::string("ReadMode")];
		m_iReadMode = CStr::StrToInt(str_Readmode); 

	}
	else // No Data Use Standard 0 
	{
		m_iReadMode = 0;
	}
	return true;
}

bool ChargeAverageCurrent::CheckSystemChargingAverageCurrent()
{

	bool b_res = true;
	std::string str_msg;


	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	char szOutput1[FTD_BUF_SIZE] = {0};
	char szOutput2[FTD_BUF_SIZE] = {0};
	strcpy(szInput, "internal");

	std::string str_firstCurrent;
	std::string str_SecondCurrent;
	double dfirstCurrent = 0.0;
	double dSecondCurrent = 0.0;
	//------------------------------------------------------------------------------------
	if ( b_res && !(b_res = m_pIPhone->FTD_WriteMaxChargeCurrent( m_nFtdPort , m_nFtdTimeOut , "1" , szOutput)))
	{
		str_msg = "Write Max ChargeCurrent fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;

	}
	if ( b_res && !(b_res = m_pIPhone->FTD_PWRchargerOn(m_nFtdPort, m_nFtdTimeOut, "0", szOutput)))
	{
		str_msg = "Failed to Set PWRchargerOn off.";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}
	//------------------------------------------------------------------------------------
	Sleep(m_i_ChargeTime);
	if ( m_iReadMode == 1 )
	{
		if ( b_res && m_pIPS == NULL )
		{
			str_msg = "Power supply1 not detected";
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	//if ( b_res && !m_pIPS->SetCurrRangeMin())
	//	{
	//		str_msg = "Fail to set power supply current range to minimum";
	//		TraceLog(MSG_INFO, str_msg);
	//		b_res = false;
	//		}
		Sleep(100);

		if (! m_pIPS->GetCurrent(dfirstCurrent))
		{
			str_msg = "Fail to get current value from power supply";
			TraceLog(MSG_INFO, str_msg);
			m_strMessage = str_msg;
			return false;
		}

		//dfirstCurrent *= 1000.0;
		sprintf_s(szOutput1, 255, "%f", dfirstCurrent);
		str_firstCurrent = szOutput1;		
		TraceLog(MSG_INFO, "First Read Current = " + str_firstCurrent );
	}
	else
	{

		if ( b_res && !(b_res = m_pIPhone->FTD_PWRgaugeCurrent(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput1)))
		{
			str_msg = "Read PWRgaugeCurrent from mobile fail";
			TraceLog(MSG_INFO, str_msg);
			b_res = false;	
		}
		else
		{
			str_firstCurrent = szOutput1;
			TraceLog(MSG_INFO, "First Read Current = " + str_firstCurrent );
		}

	}
	//------------------------------------------------------------------------------------
	if (b_res && !(b_res = m_pIPhone->FTD_PWRchargerOn(m_nFtdPort, m_nFtdTimeOut, "1", szOutput)))
	{
		str_msg = "Failed to Set PWRchargerOn on.";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}
	//------------------------------------------------------------------------------------
	Sleep(m_i_ChargeTime);
	//------------------------------------------------------------------------------------
	if ( m_iReadMode == 1 )
	{
		if (! m_pIPS->GetCurrent(dSecondCurrent))
		{
			str_msg = "Fail to get current value from power supply";
			TraceLog(MSG_INFO, str_msg);
			m_strMessage = str_msg;
			return false;
		}

		//dSecondCurrent *= 1000.0;
		sprintf_s(szOutput2, 255, "%f", dSecondCurrent);
		str_SecondCurrent = szOutput2;		
		TraceLog(MSG_INFO, "Second Read Current = " + str_SecondCurrent );
	}
	else
	{
		if ( b_res && !(b_res = m_pIPhone->FTD_PWRgaugeCurrent(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput2)))
		{
			str_msg = "Read PWRgaugeCurrent from mobile fail";
			TraceLog(MSG_INFO, str_msg);
			b_res = false;	
		}
		else
		{
			str_SecondCurrent = szOutput2;
			TraceLog(MSG_INFO, "Second Read Current = " + str_SecondCurrent );
		}	
	}
	//------------------------------------------------------------------------------------

	if ( m_b_CheckMatch == false )
	{
		str_msg = "Read Current = " + str_firstCurrent + "," +str_SecondCurrent;
		m_strMeasured = str_SecondCurrent;
		TraceLog( MSG_INFO , str_msg );
	}
	else
	{
		b_res = false;

		double f_Value1;
		f_Value1 = CStr::StrToDouble(str_SecondCurrent);

		if ( m_iCheckMode == 1 ) // T47
		{
			if ( m_rp_Value.InRange( f_Value1 ) )
			{
				str_msg = "check chargingaveragecurrent pass! value = " + str_SecondCurrent;
				m_strMeasured = str_SecondCurrent;
				TraceLog(MSG_INFO, str_msg);
				b_res = true;
			}
			else
			{
				str_msg = "chargingaveragecurrent is not matched. moble:" + str_SecondCurrent + "-- config file:" + m_str_Value;
				m_strMeasured = str_SecondCurrent;
				TraceLog(MSG_INFO, str_msg);
				b_res = false;
			}
		}
		else // QC1 
		{
			if ( f_Value1 < 0 )
			{
				m_strMeasured = str_SecondCurrent;
				b_res = true;
				str_msg = "Check ChargingAverageCurrent pass! SecondValue = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				double f_Value0;
				f_Value0 = CStr::StrToDouble(str_firstCurrent);		

				if ( (f_Value1 - f_Value0) < 0 )
				{
					b_res = true;
					m_strMeasured = CStr::FloatToStr(f_Value1 - f_Value0);
					str_msg = "Check ChargingAverageCurrent pass! AverageValue = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
				}
			}
		}
	}

	if ( b_res )
	{
		str_msg = "ChargingAverageCurrent is matched. First:" + str_firstCurrent + "-- Second:" + str_SecondCurrent;
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		str_msg = "ChargingAverageCurrent is not matched. First:" + str_firstCurrent + "-- Second:" + str_SecondCurrent;
		TraceLog(MSG_INFO, str_msg);
	}

	
	Sleep(m_i_sleep);

	/* test result */
	m_strMessage = str_msg;

	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		TraceLog(MSG_INFO, "Read ChargingAverageCurrent PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_INFO, "Read ChargingAverageCurrent FAIL!");
	}
	SetPICSData("Charging_Average_Current", m_strMeasured);
	FactoryLog();
	return b_res;
	
}
