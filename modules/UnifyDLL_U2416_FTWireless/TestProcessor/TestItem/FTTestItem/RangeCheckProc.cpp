/*********************************************************************************
 * COPYRIGHT Qisda WR24 2011
 *
 * Description: .h file of CRangeCheckProc object.
 * FileName   : CRangeCheckProc.h
 * Version    : $Revision:   1.1  $
 * Author     : Neil.Du
 * Time       : 2011/11/22
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/11/22      Neil.Du             Create.
 ********************************************************************************/

#include "stdafx.h"
#include "RangeCheckProc.h"


RF_IMPLEMENT_DYNCREATE(CRangeCheckProc)


bool CRangeCheckProc::PreRun()
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

bool CRangeCheckProc::Run()
{
	/* run different API function based on name */
	if (m_str_Name == BoardADC)
	{
		m_strItemCode  = CStr::IntToStr(ReadBoardADC_BaseItemCode);
		m_strErrorCode = CommErr_Check_Hardware_Version_Fail;
		return ReadBoardADC();
	}
	else if (m_str_Name == PWRgaugeVoltage)
	{
		m_strItemCode  = CStr::IntToStr(ReadPWRgaugeVoltage_BaseItemCode);
		m_strErrorCode = FunErr_Check_Battery_Voltage_Fail;
		return ReadPWRgaugeVoltage();
	}
	else if (m_str_Name == PWRgaugeCurrent)
	{
		m_strItemCode  = CStr::IntToStr(ReadPWRgaugeCurrent_BaseItemCode);
		m_strErrorCode = FunErr_Check_Battery_Current_Fail;
		return ReadPWRgaugeCurrent();
	}
	else if (m_str_Name == PWRgaugeTemperature)
	{
		m_strItemCode  = CStr::IntToStr(ReadPWRgaugeTemperature_BaseItemCode);
		m_strErrorCode = FunErr_Check_Battery_Temperature_Fail;
		return ReadPWRgaugeTemperature();
	}
	else if (m_str_Name == SNSRlsensorValue)
	{
		m_strItemCode  = CStr::IntToStr(ReadSNSRlsensorValue_BaseItemCode);
		m_strErrorCode = FunErr_LSensor_Verification_Fail;
		return ReadSNSRlsensorValue();
	}
	else if (m_str_Name == SNSRpsensorValue)
	{
		m_strItemCode  = CStr::IntToStr(ReadSNSRpsensorValue_BaseItemCode);
		m_strErrorCode = FunErr_PSensor_Verification_Fail;
		return ReadSNSRpsensorValue();
	}

	else if (m_str_Name == MCPddrSize)	//-- CY.Lee 120604 --//
	{
		m_strItemCode  = "-";
		m_strErrorCode = FunErr_DDR_Size_Fail;
		return ReadMCPddrSize();
	}
	else if (m_str_Name == Board_ID)
	{
		m_strItemCode = CStr::IntToStr(Board_ID_BaseItemCode);
		m_strErrorCode = CommErr_Check_HW_ID_Fail;
		return ReadBoard_ID();
	}	
	else if (m_str_Name == RFBandIDTemperature)	
	{
		m_strItemCode  = CStr::IntToStr(RF_Band_ID_Temperature_BaseItemCode);
		m_strErrorCode = CommErr_BandIDTemperature_Fail; 
		return ReadRFBandIDTemperature();
	}
	else if (m_str_Name == PowerMaxCurrent)	
	{
		m_strItemCode  = CStr::IntToStr(Power_Max_Current_BaseItemCode);
		m_strErrorCode = FunErr_Max_Current_Test_Fail; 
		return ReadPowerMaxCurrent();
	}
	else
	{
		m_strMessage = "The Type of ID is not defined";
		TraceLog(MSG_INFO, "The Type of ID is not defined");
		return false;
	}
	return true;
}

bool CRangeCheckProc::PostRun()
{
	return true;
}

bool CRangeCheckProc::InitData(std::map<std::string, std::string>& paramMap)
{
	/* Name */
	if (paramMap.find(std::string("Name")) != paramMap.end())
	{
		m_str_Name = paramMap[std::string("Name")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'Name' for class 'CRangeCheckProc'");
		return false;
	}

	//Eason Check from Server S-----------------------------------------------
	/* Value */
	//if (paramMap.find(std::string("Value")) != paramMap.end())
	//{
	//	m_str_Value = paramMap[std::string("Value")];
	//	m_rp_Value.SetRange(m_str_Value);
	//}
	//else
	//{
	//	//TraceLog(MSG_INFO, "Failed to Get filed 'CheckMatch' for class 'CRangeCheckProc'");
	//	TraceLog(MSG_INFO, "Failed to Get filed 'Value' for class 'CRangeCheckProc'");
	//	return false;
	//}
	//Eason Check from Server E-----------------------------------------------

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
		TraceLog(MSG_INFO, "Failed to Get filed 'CheckMatch' for class 'CRangeCheckProc'");
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
		TraceLog(MSG_INFO, "Failed to Get filed 'Sleep' for class 'CRangeCheckProc'");
		return false;
	}

	//Eason Check from Server S-----------------------------------------------
	//bool CheckFromServer = false;
	//std::string str_CheckFromServer;
	//std::string str_CheckFromStation;
	//
	//if (paramMap.find(std::string("CheckFromServer")) != paramMap.end())
	//{
	//	str_CheckFromServer = paramMap[std::string("CheckFromServer")];
	//	m_b_CheckFromServer = CStr::StrToBool(str_CheckFromServer);
	//	if ( m_b_CheckFromServer && paramMap.find(std::string("ServerStation")) != paramMap.end() )
	//	{
	//		str_CheckFromStation = paramMap[std::string("ServerStation")];
	//       if ( str_CheckFromStation.length() > 0 )
	//		{
	//			std::string ServerValue = GetServerPara( str_CheckFromStation );
	//			if ( !ServerValue.empty() )
	//			{
	//				CheckFromServer = true;
	//				m_str_ServerValue = ServerValue;
	//				m_str_Value = m_str_ServerValue;
	//				m_rp_Value.SetRange(m_str_Value);
	//			}
	//		}
	//	}
	//}

	m_b_CheckFromServer = false;
	std::string str_CheckFromStation;
	std::string str_ServerValue;
	if ( paramMap.find(std::string("ServerStation")) != paramMap.end() )
	{
		str_CheckFromStation = paramMap[std::string("ServerStation")];
		if ( str_CheckFromStation.length() > 0 )
		{
			std::string ServerValue = GetServerPara( str_CheckFromStation );
			if ( ServerValue.length() > 0 )
			{
				m_b_CheckFromServer = true;
				str_ServerValue = ServerValue;
			}
		}
	}
	// Server 上如果沒資料 或是不齊全 ~ 就讀 XML
	if ( m_b_CheckFromServer )
	{
		std::string str_msg_server;
		m_str_Value = str_ServerValue;
		m_rp_Value.SetRange(m_str_Value);
		str_msg_server = "Get " + m_str_Name + " Pass criteria from server, Value = " + m_str_Value;
		TraceLog(MSG_INFO, str_msg_server );
	}
	else
	{
		if (paramMap.find(std::string("Value")) != paramMap.end())
		{
			m_str_Value = paramMap[std::string("Value")];
			m_rp_Value.SetRange(m_str_Value);
		}
		else
		{
			TraceLog(MSG_INFO, "Failed to Get filed 'Value' for class 'CRangeCheckProc'");
			return false;
		}
	}
	//Eason Check from Server E-----------------------------------------------
	return true;
}

bool CRangeCheckProc::ReadBoardADC()
{
	bool b_res = false;
	std::string str_msg;

	/* read BoardADC from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_BoardADC(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read BoardADC from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare BoardADC with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read BoardADC = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//Eason Check from Server S-----------------------------------------------
			if (m_b_CheckFromServer)
			{
				str_msg = "Get Pass criteria from server = " + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
			}
			//Eason Check from Server E-----------------------------------------------
			double f_Value;
			f_Value = CStr::StrToDouble(m_strMeasured);
			if (m_rp_Value.InRange(f_Value))
			{
				str_msg = "Check BoardADC pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "BoardADC is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read BoardADC PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_INFO, "Read BoardADC FAIL!");
	}
	SetPICSData("Board_ADC", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CRangeCheckProc::ReadPWRgaugeVoltage()
{
	bool b_res = false;
	std::string str_msg;

	/* read PWRgaugeVoltage from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	strcpy(szInput, "internal");
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_PWRgaugeVoltage(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read PWRgaugeVoltage from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare PWRgaugeVoltage with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read PWRgaugeVoltage = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//Eason Check from Server S-----------------------------------------------
			if (m_b_CheckFromServer)
			{
				str_msg = "Get Pass criteria from server = " + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
			}
			//Eason Check from Server E-----------------------------------------------
			double f_Value;
			f_Value = CStr::StrToDouble(m_strMeasured);
			if (m_rp_Value.InRange(f_Value))
			{
				str_msg = "Check PWRgaugeVoltage pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "PWRgaugeVoltage is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read PWRgaugeVoltage PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_INFO, "Read PWRgaugeVoltage FAIL!");
	}
	SetPICSData("PWR_GAUGE_VOLTAGE", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CRangeCheckProc::ReadPWRgaugeCurrent()
{
	bool b_res = false;
	std::string str_msg;

	/* read PWRgaugeCurrent from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	strcpy(szInput, "internal");
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_PWRgaugeCurrent(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read PWRgaugeCurrent from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare PWRgaugeCurrent with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read PWRgaugeCurrent = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//Eason Check from Server S-----------------------------------------------
			if (m_b_CheckFromServer)
			{
				str_msg = "Get Pass criteria from server = " + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
			}
			//Eason Check from Server E-----------------------------------------------
			double f_Value;
			f_Value = CStr::StrToDouble(m_strMeasured);
			if (m_rp_Value.InRange(f_Value))
			{
				str_msg = "Check PWRgaugeCurrent pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "PWRgaugeCurrent is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read PWRgaugeCurrent PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_INFO, "Read PWRgaugeCurrent FAIL!");
	}
	SetPICSData("PWR_GAUGE_CURRENT", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CRangeCheckProc::ReadPWRgaugeTemperature()
{
	bool b_res = false;
	std::string str_msg;

	/* read PWRgaugeTemperature from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	strcpy(szInput, "internal");
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_PWRgaugeTemperature(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read PWRgaugeTemperature from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare PWRgaugeTemperature with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read PWRgaugeTemperature = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//Eason Check from Server S-----------------------------------------------
			if (m_b_CheckFromServer)
			{
				str_msg = "Get Pass criteria from server = " + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
			}
			//Eason Check from Server E-----------------------------------------------
			double f_Value;
			f_Value = CStr::StrToDouble(m_strMeasured);
			if (m_rp_Value.InRange(f_Value))
			{
				str_msg = "Check PWRgaugeTemperature pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "PWRgaugeTemperature is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read PWRgaugeTemperature PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_INFO, "Read PWRgaugeTemperature FAIL!");
	}
	SetPICSData("PWR_GAUGE_TEMPERATURE", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CRangeCheckProc::ReadSNSRlsensorValue()
{
	bool b_res = false;
	std::string str_msg;

	/* read SNSRlsensorValue from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_SNSRlsensorValue(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read SNSRlsensorValue from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare SNSRlsensorValue with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read SNSRlsensorValue = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//Eason Check from Server S-----------------------------------------------
			if (m_b_CheckFromServer)
			{
				str_msg = "Get Pass criteria from server = " + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
			}
			//Eason Check from Server E-----------------------------------------------
			double f_Value;
			f_Value = CStr::StrToDouble(m_strMeasured);
			if (m_rp_Value.InRange(f_Value))
			{
				str_msg = "Check SNSRlsensorValue pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "SNSRlsensorValue is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read SNSRlsensorValue PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_INFO, "Read SNSRlsensorValue FAIL!");
	}
	SetPICSData("SNSRlsensor_VALUE", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CRangeCheckProc::ReadSNSRpsensorValue()
{
	bool b_res = false;
	std::string str_msg;

	/* read SNSRpsensorValue from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_SNSRpsensorValue(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read SNSRpsensorValue from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare SNSRpsensorValue with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read SNSRpsensorValue = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//Eason Check from Server S-----------------------------------------------
			if (m_b_CheckFromServer)
			{
				str_msg = "Get Pass criteria from server = " + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
			}
			//Eason Check from Server E-----------------------------------------------
			double f_Value;
			f_Value = CStr::StrToDouble(m_strMeasured);
			if (m_rp_Value.InRange(f_Value))
			{
				str_msg = "Check SNSRpsensorValue pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "SNSRpsensorValue is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read SNSRpsensorValue PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_INFO, "Read SNSRpsensorValue FAIL!");
	}
	SetPICSData("SNSRpsensor_VALUE", m_strMeasured);
	FactoryLog();
	return b_res;
}

//-- CY.Lee 120604 --//
bool CRangeCheckProc::ReadMCPddrSize()
{
	bool b_res = false;
	std::string str_msg;

	/* read MCPddrSize from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_MCPddrSize(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read MCPddrSize from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare MCPddrSize with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read MCPddrSize = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//Eason Check from Server S-----------------------------------------------
			if (m_b_CheckFromServer)
			{
				str_msg = "Get Pass criteria from server = " + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
			}
			//Eason Check from Server E-----------------------------------------------
			double f_Value;
			f_Value = CStr::StrToDouble(m_strMeasured);
			if (m_rp_Value.InRange(f_Value))
			{
				str_msg = "Check MCPddrSize pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "MCPddrSize is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read MCPddrSize PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_INFO, "Read MCPddrSize FAIL!");
	}
	SetPICSData("MCP_DDR_SIZE", m_strMeasured);
	FactoryLog();
	return b_res;
}
bool CRangeCheckProc::ReadRFBandIDTemperature()
{
	bool b_res = false;
	std::string str_msg;

	/* read ReadRFBandIDTemperature from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_PWRRFBandIDTemperature(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read RFBandIDTemperature from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare ReadRFBandIDTemperature with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read RFBandIDTemperature = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//Eason Check from Server S-----------------------------------------------
			if (m_b_CheckFromServer)
			{
				str_msg = "Get Pass criteria from server = " + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
			}
			//Eason Check from Server E-----------------------------------------------
			double f_Value;
			f_Value = CStr::StrToDouble(m_strMeasured);
			if (m_rp_Value.InRange(f_Value))
			{
				str_msg = "Check RFBandIDTemperature pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "RFBandIDTemperature is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read RFBandIDTemperature PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_INFO, "Read RFBandIDTemperature FAIL!");
	}
	SetPICSData("RFBandIDTemperature", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CRangeCheckProc::ReadBoard_ID()
{
	bool b_res = false;
	std::string str_msg;

	/* read  BoardID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_Boardid(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read BoardID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare  BoardID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read BoardID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//Eason Check from Server S-----------------------------------------------
			if (m_b_CheckFromServer)
			{
				str_msg = "Get Pass criteria from server = " + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
			}
			//Eason Check from Server E-----------------------------------------------

			double f_Value;
			f_Value = CStr::StrToDouble(m_strMeasured);
			if (m_rp_Value.InRange(f_Value))
			{
				//str_msg = "Check RF BandID Temperature pass! Value = " + m_strMeasured;
				str_msg = "Check BoardID pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				//str_msg = "RF BandID Temperature is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
				str_msg = "BoardID is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
				b_res = false;
			}
			//if (m_strMeasured == m_str_Value)
			//{ 
			//	str_msg = "Check BoardID pass! Value = " + m_strMeasured;
			//	TraceLog(MSG_INFO, str_msg);
			//}
			//else
			//{
			//	str_msg = "BoardID is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
			//	TraceLog(MSG_INFO, str_msg);
			//	b_res = false;
			//}
		}
	}
	Sleep(m_i_sleep);

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read BoardID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read BoardID FAIL!");
	}
	SetPICSData("BoardID", m_strMeasured);
	FactoryLog();
	return b_res;

}


bool CRangeCheckProc::ReadPowerMaxCurrent()
{

	bool b_res = true;
	std::string str_msg;

	/* read  ReadPowerMaxCurrent from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};

	char szInput1[FTD_BUF_SIZE] = {0};
	char szOutput1[FTD_BUF_SIZE] = {0};
	strcpy(szInput1, "internal");

	if (b_res && !(b_res = m_pIPhone->FTD_PWRchargerOn(m_nFtdPort, m_nFtdTimeOut, "0", szOutput)))
	{
		str_msg = "Failed to Set PWRchargerOn off.";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	
	}

	if (b_res &&!(b_res = m_pIPhone->FTD_ReadPowerMaxCurrent(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read PowerMaxCurrent from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	if (b_res &&!(b_res = m_pIPhone->FTD_PWRgaugeCurrent(m_nFtdPort, m_nFtdTimeOut, szInput1, szOutput1)))
	{
		str_msg = "Read PWRgaugeCurrent from mobile fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}
	//--------------------------------------------------------------------------------------------
	if (b_res &&!(b_res = m_pIPhone->FTD_PWRchargerOn(m_nFtdPort, m_nFtdTimeOut, "1", szOutput)))
	{
		str_msg = "Failed to Set PWRchargerOn off.";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}	




	/* Compare  ReadPowerMaxCurrent with config file */
	if (b_res)
	{
		m_strMeasured = szOutput1;
		double f_Value;
		f_Value = CStr::StrToDouble(m_strMeasured);
		f_Value /= 1000;
		m_strMeasured = CStr::FloatToStr(f_Value);		

		if (m_b_CheckMatch == false)
		{
			str_msg = "Read PowerMaxCurrent = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//Eason Check from Server S-----------------------------------------------
			if (m_b_CheckFromServer)
			{
				str_msg = "Get Pass criteria from server = " + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
			}
			//Eason Check from Server E-----------------------------------------------
			if (m_rp_Value.InRange(f_Value))
			{
				str_msg = "Check PowerMaxCurrent pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "PowerMaxCurrent is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
				b_res = false;
			}
		}
	}
	Sleep(m_i_sleep);

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read PowerMaxCurrent PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read PowerMaxCurrent FAIL!");
	}
	SetPICSData("PowerMaxCurrent", m_strMeasured);
	FactoryLog();
	return b_res;

}
