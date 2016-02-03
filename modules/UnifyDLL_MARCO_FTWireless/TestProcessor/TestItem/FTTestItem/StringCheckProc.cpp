/*********************************************************************************
 * COPYRIGHT Qisda WR24 2011
 *
 * Description: .h file of CStringCheckProc object.
 * FileName   : CStringCheckProc.h
 * Version    : $Revision:   1.1  $
 * Author     : Neil.Du
 * Time       : 2011/11/22
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/11/22      Neil.Du             Create.
 ********************************************************************************/

#include "stdafx.h"
#include "StringCheckProc.h"


RF_IMPLEMENT_DYNCREATE(CStringCheckProc)


bool CStringCheckProc::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = m_str_Value;
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CStringCheckProc::Run()
{
	/* run different API function based on name */

	if (m_str_Name == MCPflashID)
	{
		m_strItemCode = CStr::IntToStr(ReadMCPflashID_BaseItemCode);
		m_strErrorCode = FunErr_Flash_ReadWrite_Data_Fail;

		return ReadMCPflashID();
	}
	else if (m_str_Name == MCPddrID)
	{
		m_strItemCode = CStr::IntToStr(ReadMCPddrID_BaseItemCode);
		//m_strErrorCode = FunErr_Check_eMMC_Fail;
		m_strErrorCode = FunErr_DDR_ID_Fail;
		return ReadMCPddrID();
	}
	//---------------------------------------------------------------------------
	else if (m_str_Name == MCPemmcsize)
	{
		m_strItemCode = CStr::IntToStr(MCPemmcsize_BaseItemCode);
		m_strErrorCode = FunErr_Check_eMMC_Fail;
		return ReadMCPemmcsize();
	}
	else if (m_str_Name == MCPemmcFWversion)
	{
		m_strItemCode = CStr::IntToStr(MCPemmcFWversion_BaseItemCode);
		m_strErrorCode = FunErr_eMMC_Version_Fail;
		return ReadMCPemmcFWversion();
	}

	else if (m_str_Name == MCPddrmr8)
	{
		m_strItemCode = CStr::IntToStr(MCPddrmr8_BaseItemCode);
		m_strErrorCode = FunErr_DDR_Size_Fail;
		return ReadMCPddrmr8();
	}
	else if (m_str_Name == SOCversion)
	{
		m_strItemCode = CStr::IntToStr(SOCversion_BaseItemCode);
		m_strErrorCode = FunErr_Check_CPU_Version_Fail;
		return ReadSOCversion();
	}
	else if (m_str_Name == PMid)
	{
		m_strItemCode = CStr::IntToStr(PMid_BaseItemCode);
		m_strErrorCode = FunErr_Check_PMid_Fail;
		return ReadPMid();
	}	
	else if (m_str_Name == PMIid)
	{
		m_strItemCode = CStr::IntToStr(PMIid_BaseItemCode);
		m_strErrorCode = FunErr_Check_PMIid_Fail;
		return ReadPMIid();
	}
	else if (m_str_Name == APQ_UART)
	{
		m_strItemCode = CStr::IntToStr(APQ_UART_BaseItemCode);
		m_strErrorCode = FunErr_Check_APQ_UART_Fail;
		return Check_APQ_UART();
	}
	else if (m_str_Name == HDMI_DDC_RX)
	{
		m_strItemCode = CStr::IntToStr(HDMI_DDC_BaseItemCode);
		m_strErrorCode = FunErr_Check_DDC_RX_Fail;
		return Check_HDMI_DDC_RX();
	}
	else if (m_str_Name == HDMI_OUTPUT)
	{
		m_strItemCode = CStr::IntToStr(HDMI_OUTPUT_BaseItemCode);
		m_strErrorCode = FunErr_Check_HDMI_OUTPUT_Fail;
		return Check_HDMI_OUTPUT();
	}
	else if (m_str_Name == WLAN_MODULE)
	{
		m_strItemCode = CStr::IntToStr(WLAN_MODULE_BaseItemCode);
		m_strErrorCode = FunErr_Check_WLAN_MODULE_Fail;
		return Check_WLAN_MODULE();
	}
	else if (m_str_Name == BT_INQUIRY)
	{
		m_strItemCode = CStr::IntToStr(BT_INQUIRY_BaseItemCode);
		m_strErrorCode = FunErr_Check_BT_INQUIRY_Fail;
		return Check_BT_INQUIRY();
	}	
	else
	{
		m_strMessage = "The Type of ID is not defined";
		TraceLog(MSG_INFO, "The Type of ID is not defined");
		return false;
	}
	return true;
}

bool CStringCheckProc::PostRun()
{
	return true;
}

bool CStringCheckProc::InitData(std::map<std::string, std::string>& paramMap)
{
	/* Name */
	if (paramMap.find(std::string("Name")) != paramMap.end())
	{
		m_str_Name = paramMap[std::string("Name")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'Name' for class 'CStringCheckProc'");
		return false;
	}


	/* Value */
	//if (paramMap.find(std::string("Value")) != paramMap.end())
	//{
	//	m_str_Value = paramMap[std::string("Value")];
	//}
	//else
	//{
	//	TraceLog(MSG_INFO, "Failed to Get filed 'Value' for class 'CStringCheckProc'");
	//	return false;
	//}

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
		TraceLog(MSG_INFO, "Failed to Get filed 'CheckMatch' for class 'CStringCheckProc'");
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
		TraceLog(MSG_INFO, "Failed to Get filed 'Sleep' for class 'CStringCheckProc'");
		return false;
	}


	//Eason Check from Server E-----------------------------------------------
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
		str_msg_server = "Get " + m_str_Name + " Pass criteria from server, Value = " + m_str_Value;
		TraceLog(MSG_INFO, str_msg_server );
	}
	else
	{

		if (paramMap.find(std::string("Value")) != paramMap.end())
		{
			m_str_Value = paramMap[std::string("Value")];
		}
		else
		{
			TraceLog(MSG_INFO, "Failed to Get filed 'Value' for class 'CStringCheckProc'");
			return false;
		}
	}
	//Eason Check from Server E-----------------------------------------------

	return true;
}


bool CStringCheckProc::ReadMCPflashID()
{
	bool b_res = false;
	std::string str_msg;

	/* read MCPflashID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_MCPflashID(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read MCPflashID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare MCPflashID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read MCPflashID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//if (m_strMeasured == m_str_Value)
			//{
			//	str_msg = "Check MCPflashID pass! Value = " + m_strMeasured;
			//	TraceLog(MSG_INFO, str_msg);
			//}
			//else
			//{
			//	str_msg = "MCPflashID is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
			//	TraceLog(MSG_INFO, str_msg);
			//	b_res = false;
			//}
			//Eason Check from Server S-----------------------------------------------
			if (m_b_CheckFromServer)
			{
				str_msg = "Get Pass criteria from server = " + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
			}
			//Eason Check from Server E-----------------------------------------------
			StrVtr vToken;
			CStr::ParseString(m_str_Value.c_str(), _T(";"), vToken);
			bool b_match = false;
			for (size_t i = 0; i < vToken.size(); i++)
			{
				if ((strcmp(m_strMeasured.c_str(), vToken[i].c_str()) == 0))
				{
					str_msg = "Check MCPflashID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "MCPflashID value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read MCPflashID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read MCPflashID FAIL!");
	}
	SetPICSData("MCP_FLASH_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}
bool CStringCheckProc::ReadMCPddrID()
{
	bool b_res = false;
	std::string str_msg;

	/* read MCPddrID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_MCPddrID(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read MCPddrID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare MCPddrID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read MCPddrID = " + m_strMeasured;
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

			StrVtr vToken;
			CStr::ParseString(m_str_Value.c_str(), _T(";"), vToken);
			bool b_match = false;
			for (size_t i = 0; i < vToken.size(); i++)
			{
				if ((strcmp(m_strMeasured.c_str(), vToken[i].c_str()) == 0))
				{
					str_msg = "Check MCPddrID pass! ID = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "MCPddrID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read MCPddrID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read MCPddrID FAIL!");
	}
	SetPICSData("MCP_DDR_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}


//=============================================================================================================

bool CStringCheckProc::ReadMCPddrmr8()
{
	bool b_res = false;
	std::string str_msg;

	/* read CPU ID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_MCPddrmr8(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read DDR MR8 from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare DDR MR8 with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read DDR MR8 = " + m_strMeasured;
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

			StrVtr vToken;
			CStr::ParseString(m_str_Value.c_str(), _T(";"), vToken);
			bool b_match = false;
			for (size_t i = 0; i < vToken.size(); i++)
			{
				if ((strcmp(m_strMeasured.c_str(), vToken[i].c_str()) == 0))
				{
					str_msg = "Check DDR MR8 pass! ID = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "DDR MR8 is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read DDR MR8 PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read DDR MR8 FAIL!");
	}
	SetPICSData("DDR_MR8", m_strMeasured);
	FactoryLog();
	return b_res;
}
bool CStringCheckProc::ReadMCPemmcsize()
{
	bool b_res = false;
	std::string str_msg;

	/* read CPU ID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_MCPemmcsize(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read eMMC Size from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare DDR MR8 with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read eMMC Size = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//if (m_strMeasured == m_str_Value)
			//{
			//	str_msg = "Check eMMC Size pass! Value = " + m_strMeasured;
			//	TraceLog(MSG_INFO, str_msg);
			//}
			//else
			//{
			//	str_msg = "eMMC Size is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
			//	TraceLog(MSG_INFO, str_msg);
			//	b_res = false;
			//}
			StrVtr vToken;
			CStr::ParseString(m_str_Value.c_str(), _T(";"), vToken);
			bool b_match = false;
			for (size_t i = 0; i < vToken.size(); i++)
			{
				if ((strcmp(m_strMeasured.c_str(), vToken[i].c_str()) == 0))
				{
					str_msg = "Check eMMC Size pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "eMMC Size value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read eMMC Size PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read eMMC Size FAIL!");
	}
	SetPICSData("eMMC_Size", m_strMeasured);
	FactoryLog();
	return b_res;
}
bool CStringCheckProc::ReadMCPemmcFWversion()
{
	bool b_res = false;
	std::string str_msg;

	/* read  eMMC FW version from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_MCPemmcFWversion(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read eMMC FW version from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare  eMMC FW version with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read eMMC FW version = " + m_strMeasured;
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
			StrVtr vToken;
			CStr::ParseString(m_str_Value.c_str(), _T(";"), vToken);
			bool b_match = false;
			for (size_t i = 0; i < vToken.size(); i++)
			{
				if ((strcmp(m_strMeasured.c_str(), vToken[i].c_str()) == 0))
				{
					str_msg = "Check eMMC FW pass! ID = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "Check eMMC FW is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
				b_res = false;
			}
			//if (m_strMeasured == m_str_Value)
			//{
			//	str_msg = "Check eMMC FW version pass! Value = " + m_strMeasured;
			//	TraceLog(MSG_INFO, str_msg);
			//}
			//else
			//{
			//	str_msg = "eMMC FW version is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read eMMC FW version PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read eMMC FW version FAIL!");
	}
	SetPICSData("eMMC_FW_version", m_strMeasured);
	FactoryLog();
	return b_res;
}





bool CStringCheckProc::ReadPMid()
{

	bool b_res = false;
	std::string str_msg;

	/* read  SOCversion from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_PMid(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read PMid from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare  SOCversion with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read PMid = " + m_strMeasured;
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
			StrVtr vToken;
			CStr::ParseString(m_str_Value.c_str(), _T(";"), vToken);
			bool b_match = false;
			for (size_t i = 0; i < vToken.size(); i++)
			{
				if ((strcmp(m_strMeasured.c_str(), vToken[i].c_str()) == 0))
				{
					str_msg = "Check PMid pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "PMid value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read PMid PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read PMid FAIL!");
	}
	SetPICSData("PMid", m_strMeasured);
	FactoryLog();
	return b_res;

}

bool CStringCheckProc::ReadPMIid()
{

	bool b_res = false;
	std::string str_msg;

	/* read  PMIid from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_PMIid(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read PMIid from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare  PMIid with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read PMid = " + m_strMeasured;
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
			StrVtr vToken;
			CStr::ParseString(m_str_Value.c_str(), _T(";"), vToken);
			bool b_match = false;
			for (size_t i = 0; i < vToken.size(); i++)
			{
				if ((strcmp(m_strMeasured.c_str(), vToken[i].c_str()) == 0))
				{
					str_msg = "Check PMIid pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "PMIid value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read PMIid PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read PMIid FAIL!");
	}
	SetPICSData("PMIid", m_strMeasured);
	FactoryLog();
	return b_res;

}


bool CStringCheckProc::Check_HDMI_DDC_RX()
{
	bool b_res = false;
	std::string str_msg;

	/* read  PMIid from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_DDCRX(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read HDMI_DDC_RX from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}


	/* test result */
	if (b_res)
	{
		str_msg = CW2A(L"HDMI_DDC_RX ok");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check_HDMI_DDC_RX  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check_HDMI_DDC_RX  FAIL!");
	}
	SetPICSData("Check_HDMI_DDC_RX", "PASS");
	FactoryLog();
	return b_res;

}

bool CStringCheckProc::Check_APQ_UART()
{
	bool b_res = false;
	std::string str_msg;

	/* read  PMIid from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_Check_APQ_UART(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read Check_APQ_UART from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}


	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check_APQ_UART  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check_APQ_UART  FAIL!");
	}
	SetPICSData("Check_APQ_UART", "PASS");
	FactoryLog();
	return b_res;

}


bool CStringCheckProc::Check_HDMI_OUTPUT()
{
	bool b_res = false;
	std::string str_msg;

	/* read  PMIid from mobile */
	str_msg = CW2A(L"HDMI_OUTPUT Is PASS ?");
	b_res = showMsg(str_msg.c_str());


	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check_HDMI_OUTPUT  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check_HDMI_OUTPUT  FAIL!");
	}
	SetPICSData("Check_HDMI_OUTPUT", "PASS");
	FactoryLog();
	return b_res;

}

bool CStringCheckProc::Check_WLAN_MODULE()
{
	bool b_res = false;
	std::string str_msg;

	/* read  PMIid from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_WLAN_MODULE_Test(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read Check_WLAN_MODULE from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}


	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check_WLAN_MODULE  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check_WLAN_MODULE  FAIL!");
	}
	SetPICSData("Check_WLAN_MODULE", "PASS");
	FactoryLog();
	return b_res;

}

bool CStringCheckProc::Check_BT_INQUIRY()
{
	bool b_res = false;
	std::string str_msg;

	/* read  PMIid from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_BT_INQUIRY(m_nFtdPort, 30000, szInput, szOutput)))
	{
		str_msg = "Read Check_BT_INQUIRY from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}


	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check_BT_INQUIRY  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check_BT_INQUIRY  FAIL!");
	}
	SetPICSData("Check_BT_INQUIRY", "PASS");
	FactoryLog();
	return b_res;

}


bool CStringCheckProc::ReadSOCversion()
{

	bool b_res = false;
	std::string str_msg;

	/* read  SOCversion from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_SOCversion(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read SOCversion from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare  SOCversion with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read SOCversion = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//if (m_strMeasured == m_str_Value)
			//{ 
			//	str_msg = "Check SOCversion pass! Value = " + m_strMeasured;
			//	TraceLog(MSG_INFO, str_msg);
			//}
			//else
			//{
			//	str_msg = "SOCversion is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
			//	TraceLog(MSG_INFO, str_msg);
			//	b_res = false;
			//}
			//Eason Check from Server S-----------------------------------------------
			if (m_b_CheckFromServer)
			{
				str_msg = "Get Pass criteria from server = " + m_str_Value;
				TraceLog(MSG_INFO, str_msg);
			}
			//Eason Check from Server E-----------------------------------------------
			StrVtr vToken;
			CStr::ParseString(m_str_Value.c_str(), _T(";"), vToken);
			bool b_match = false;
			for (size_t i = 0; i < vToken.size(); i++)
			{
				if ((strcmp(m_strMeasured.c_str(), vToken[i].c_str()) == 0))
				{
					str_msg = "Check SOCversion pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "SOCversion value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read SOCversion PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read SOCversion FAIL!");
	}
	SetPICSData("SOCversion", m_strMeasured);
	FactoryLog();
	return b_res;

}



bool CStringCheckProc::Read_BacklightDriver()
{
	bool b_res = false;
	std::string str_msg;

	/* read Read BacklightDriver ID  from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_BacklightDriver(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read BacklightDriver ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare Read BacklightDriver ID  with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read Read BacklightDriver ID  = " + m_strMeasured;
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
			StrVtr vToken;
			CStr::ParseString(m_str_Value.c_str(), _T(";"), vToken);
			bool b_match = false;
			for (size_t i = 0; i < vToken.size(); i++)
			{
				if ((strcmp(m_strMeasured.c_str(), vToken[i].c_str()) == 0))
				{
					str_msg = "Read BacklightDriver ID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "Read BacklightDriver ID  value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read BacklightDriver ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read BacklightDriver ID FAIL!");
	}
	SetPICSData("BacklightDriver", m_strMeasured);
	FactoryLog();
	return b_res;
}


bool CStringCheckProc::showMsg(const char* szMsg)
{
	/* Init dialog response flag to false */
	m_b_DialogResponse = false;
	m_b_waitFlag = false;

	/* Notify UI to show message dialog */
	ShowDialog(szMsg);

	/* Wait UI response(Max:10s) */
	int i_waitCount = 0;
	while (m_b_waitFlag != true)
	{
		if (i_waitCount == 50)
		{
			str_msg = "Wait UI response time out.";
			TraceLog(MSG_INFO, "Wait UI response time out.");
			break;
		}
		i_waitCount++;
		Sleep(200);
	}
	/* Get UI dialog operator response */
	if (m_b_DialogResponse == true)
	{
		str_msg = "Pressed PASS";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		str_msg = "Pressed FAIL";
		TraceLog(MSG_INFO, str_msg);
	}
	return m_b_DialogResponse;
}