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
	if (m_str_Name == CPUid)
	{
		m_strItemCode = CStr::IntToStr(ReadCPUid_BaseItemCode);
		m_strErrorCode = FunErr_Check_CPU_Version_Fail;

		return ReadCPUid();
	}
	else if (m_str_Name == VERftd)
	{
		m_strItemCode = CStr::IntToStr(ReadVERftd_BaseItemCode);
		m_strErrorCode = CommErr_Get_Software_Version_Fail;

		return ReadVERftd();
	}
	else if (m_str_Name == VERchange)
	{
		m_strItemCode = CStr::IntToStr(ReadVERchange_BaseItemCode);
		m_strErrorCode = CommErr_Get_Software_Version_Fail;

		return ReadVERchange();
	}
	else if (m_str_Name == BATTrtcTime)
	{
		m_strItemCode = CStr::IntToStr(ReadBATTrtcTime_BaseItemCode);
		m_strErrorCode = FunErr_RTC_Test_Fail;

		return ReadBATTrtcTime();
	}
	else if (m_str_Name == MCPflashID)
	{
		m_strItemCode = CStr::IntToStr(ReadMCPflashID_BaseItemCode);
		m_strErrorCode = FunErr_Flash_ReadWrite_Data_Fail;

		return ReadMCPflashID();
	}
	else if (m_str_Name == SNSRgsensorAcc)
	{
		m_strItemCode = CStr::IntToStr(ReadSNSRgsensorAcc_BaseItemCode);
		m_strErrorCode = FunErr_GSensor_ID_Fail;

		return ReadSNSRgsensorAcc();
	}
	else if (m_str_Name == LEDid)
	{
		m_strItemCode = CStr::IntToStr(ReadLEDid_BaseItemCode);
		m_strErrorCode = FunErr_Flash_LED_ID_Fail;

		return ReadLEDid();
	}
	else if (m_str_Name == MCPddrID)
	{
		m_strItemCode = CStr::IntToStr(ReadMCPddrID_BaseItemCode);
		//m_strErrorCode = FunErr_Check_eMMC_Fail;
		m_strErrorCode = FunErr_DDR_ID_Fail;
		return ReadMCPddrID();
	}
	else if (m_str_Name == BoardRev)
	{
		m_strItemCode = CStr::IntToStr(ReadBoardRev_BaseItemCode);
		m_strErrorCode = CommErr_Get_Hardware_Version_Fail;

		return ReadBoardRev();
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
	else if (m_str_Name == MCPddrmr6)
	{
		m_strItemCode = CStr::IntToStr(MCPddrmr6_BaseItemCode);
		m_strErrorCode = FunErr_DDR_SelfTest_Fail; //沒用到的ERROR CODE 拿來用 ， MR6 是確認DDR 的製程。
		return ReadMCPddrmr6();
	}

	//---------------------------------------------------------------------------
	else if (m_str_Name == AudioID)
	{
		m_strItemCode = CStr::IntToStr(AudioID_BaseItemCode);
		m_strErrorCode = FunErr_Audio_ID_Fail;
		return ReadAudioID();
	}
	else if (m_str_Name == RF_PA_QFE_ID)
	{
		m_strItemCode = CStr::IntToStr(RF_PA_QFE_ID_BaseItemCode);
		m_strErrorCode = FunErr_RF_PA_ID_Fail;
		return ReadRF_PA_QFE_ID();
	}
	else if (m_str_Name == RF_ASM_ID)
	{
		m_strItemCode = CStr::IntToStr(RF_ASM_ID_BaseItemCode);
		m_strErrorCode = FunErr_ASM_ID_Fail;
		return ReadRF_ASM_ID();
	}

	else if (m_str_Name == SKU_ID)
	{
		m_strItemCode = CStr::IntToStr(SKU_ID_BaseItemCode);
		m_strErrorCode = CommErr_SKU_ID_Fail;
		return ReadSKU_ID();
	}
	else if (m_str_Name == SOCversion)
	{
		m_strItemCode = CStr::IntToStr(SOCversion_BaseItemCode);
		m_strErrorCode = FunErr_Check_CPU_Version_Fail;
		return ReadSOCversion();
	}
	else if (m_str_Name == SOCfeature_ID)
	{
		m_strItemCode = CStr::IntToStr(SOCfeature_ID_BaseItemCode);
		m_strErrorCode = CommErr_Get_Hardware_Version_Fail;
		return ReadSOCfeature_ID();
	}
	else if (m_str_Name == SGAUGE)
	{
		m_strItemCode = CStr::IntToStr(GaugeICIDTest_BaseItemCode);
		m_strErrorCode = FunErr_Guage_ID_Fail;
		return GaugeCheckID();
	}
	//------------------------------------------------------------

	else if (m_str_Name == SNSR_GsensorID)
	{
		m_strItemCode = CStr::IntToStr(ReadSNSRgsensorID_BaseItemCode);
		m_strErrorCode = FunErr_GSensor_ID_Fail;
		return Read_GsensorID();
	}
	else if (m_str_Name == SNSR_GsensorSelfTest)
	{
		m_strItemCode = CStr::IntToStr(Gsensor_SelfTest_BaseItemCode);
		m_strErrorCode = FunErr_GSensor_SelfTest_Fail;
		return Read_GsensorSelfTest();
	}
	else if (m_str_Name == SNSR_EcompassID)
	{
		m_strItemCode = CStr::IntToStr(ReadSNSRecompassID_BaseItemCode);
		m_strErrorCode = FunErr_ECompassr_ID_Fail;
		return Read_EcompassID();
	}
	else if (m_str_Name == SNSR_EcompassSelfTest)
	{
		m_strItemCode = CStr::IntToStr(ECompass_SelfTest_BaseItemCode);
		m_strErrorCode = FunErr_ECompass_SelfTest_Fail;
		return Read_EcompassSelfTest();
	}
	//-----------------------------------------------------------
	else if (m_str_Name == FM_SelfTest)
	{
		m_strItemCode = CStr::IntToStr(FM_SelfTest_BaseItemCode);
		m_strErrorCode = FunErr_FM_Test_Set_Command_Fail; // 目前沒有 // T47
		return Read_FMSelfTest();
	}
	else if (m_str_Name == BT_SelfTest)
	{
		m_strItemCode = CStr::IntToStr(BT_SelfTest_BaseItemCode);
		m_strErrorCode = FunErr_BT_Test_Set_Command_Fail;
		return Read_BTSelfTest();
	}
	else if (m_str_Name == NFC_ID)
	{
		m_strItemCode = CStr::IntToStr(ReadNFCID_BaseItemCode);
		m_strErrorCode = FunErr_NFC_Chip_ID_Fail;
		return Read_NFC_ID();
	}
	else if (m_str_Name == PMICID)
	{
		m_strItemCode = CStr::IntToStr(PMICID_BaseItemCode);
		m_strErrorCode = FunErr_PMIC_ID_Fail;
		return ReadPMICID();
	}
	else if (m_str_Name == FlashLED_ID)
	{
		m_strItemCode = CStr::IntToStr(ReadLEDid_BaseItemCode);
		m_strErrorCode = FunErr_Flash_LED_ID_Fail;
		return Read_FlashLED_ID();
	}
	//-------------------------------------------------------------------
	//F80 ADD
	else if (m_str_Name == BuckID) 
	{
		m_strItemCode = CStr::IntToStr(ReadBuckID_BaseItemCode);
		m_strErrorCode = FunErr_Buck_Check_ID_Fail;
		return Read_BuckID();
	}
	else if (m_str_Name == Buckfeature) 
	{
		m_strItemCode = CStr::IntToStr(ReadBuckfeature_BaseItemCode);
		m_strErrorCode = FunErr_Buck_Check_FeatureID_Fail;
		return Read_Buckfeature();
	}
	else if (m_str_Name == BacklightDriver) 
	{
		m_strItemCode = CStr::IntToStr(ReadBacklightDriver_BaseItemCode);
		m_strErrorCode = FunErr_LCD_Backlight_Test_Fail;
		return Read_BacklightDriver();
	}
	else if (m_str_Name == GyroID) 
	{
		m_strItemCode = CStr::IntToStr(GYROIDTest_BaseItemCode);
		m_strErrorCode = FunErr_Gyro_ID_Fail;
		return Read_GyroID();
	}
	//-----------------------------------------------------------------------
	else if (m_str_Name == ChargerID) 
	{
		m_strItemCode = CStr::IntToStr(CHARGERICTest_BaseItemCode);
		m_strErrorCode = FunErr_Charge_ID_Fail;
		return Read_ChargerID();
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

bool CStringCheckProc::ReadCPUid()
{
	bool b_res = false;
	std::string str_msg;

	/* read CPU ID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_CPUid(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read CPU ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare CPU ID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read CPU ID = " + m_strMeasured;
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
			if (m_strMeasured == m_str_Value)
			{
				str_msg = "Check CPU ID pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "CPU ID is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read CPU ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read CPU ID FAIL!");
	}
	SetPICSData("CPU_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CStringCheckProc::ReadVERftd()
{
	bool b_res = false;
	std::string str_msg;

	/* read ftd version from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_VERftd(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read ftd version from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare ftd version with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read ftd version = " + m_strMeasured;
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
			if (m_strMeasured == m_str_Value)
			{
				str_msg = "Check ftd version pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "ftd version is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read ftd version PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read ftd version FAIL!");
	}
	SetPICSData("VER_FTD", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CStringCheckProc::ReadVERchange()
{
	bool b_res = false;
	std::string str_msg;

	/* read change version from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_VERchange(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read change version from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare change version with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read change version = " + m_strMeasured;
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
			if (m_strMeasured == m_str_Value)
			{
				str_msg = "Check change version pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "change version is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read change version PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read change version FAIL!");
	}
	SetPICSData("VER_CHANGE", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CStringCheckProc::ReadBATTrtcTime()
{
	bool b_res = false;
	std::string str_msg;

	/* read BATTrtcTime from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_BATTrtcTime(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read BATTrtcTime from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare BATTrtcTime with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read BATTrtcTime = " + m_strMeasured;
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
			if (m_strMeasured == m_str_Value)
			{
				str_msg = "Check BATTrtcTime pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "BATTrtcTime is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read BATTrtcTime PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read BATTrtcTime FAIL!");
	}
	SetPICSData("BATT_RTC_TIME", m_strMeasured);
	FactoryLog();
	return b_res;
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

bool CStringCheckProc::ReadSNSRgsensorAcc()
{
	bool b_res = false;
	std::string str_msg;

	/* read SNSRgsensorAcc from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_SNSRgsensorAcc(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read SNSRgsensorAcc from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare SNSRgsensorAcc with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read SNSRgsensorAcc = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//if (m_strMeasured == m_str_Value)
			//{
			//	str_msg = "Check SNSRgsensorAcc pass! Value = " + m_strMeasured;
			//	TraceLog(MSG_INFO, str_msg);
			//}
			//else
			//{
			//	str_msg = "SNSRgsensorAcc is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
					str_msg = "Check SNSRgsensorAcc pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "SNSRgsensorAcc value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read SNSRgsensorAcc PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read SNSRgsensorAcc FAIL!");
	}
	SetPICSData("SNSRgsensor_ACC", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CStringCheckProc::ReadLEDid()
{
	bool b_res = false;
	std::string str_msg;

	/* read CPU ID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_LEDid(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read LED ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare CPU ID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read LED ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//if (m_strMeasured == m_str_Value)
			//{
			//	str_msg = "Check LED ID pass! Value = " + m_strMeasured;
			//	TraceLog(MSG_INFO, str_msg);
			//}
			//else
			//{
			//	str_msg = "LED ID is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
					str_msg = "Check LED ID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "LED ID value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read LED ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read LED ID FAIL!");
	}
	SetPICSData("LED_ID", m_strMeasured);
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

bool CStringCheckProc::ReadBoardRev()
{
	bool b_res = false;
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	std::string str_msg;

	/* read BoardRev from mobile */
	if (!(b_res = m_pIPhone->FTDCmdCtrl(m_nFtdPort, m_nFtdTimeOut, _T("Detroit_BoardRev"), szInput, szOutput)))
	{
		str_msg = "Read BoardRev from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare BoardRev with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read BoardRev = " + m_strMeasured;
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
			if (m_strMeasured == m_str_Value)
			{
				str_msg = "Check BoardRev pass! Value = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				str_msg = "BoardRev is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read BoardRev PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read BoardRev FAIL!");
	}
	SetPICSData("BOARD_REV", m_strMeasured);
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

//----------------------------------------------------------------------------
bool CStringCheckProc::ReadAudioID()
{

	bool b_res = false;
	std::string str_msg;

	/* read  AudioID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_Audioid(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read AudioIDfrom mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare  AudioID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read AudioID = " + m_strMeasured;
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
			//if (m_strMeasured == m_str_Value)
			//{ 
			//	str_msg = "Check AudioID pass! Value = " + m_strMeasured;
			//	TraceLog(MSG_INFO, str_msg);
			//}
			//else
			//{
			//	str_msg = "AudioID is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
					str_msg = "Check AudioID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "AudioID value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read AudioID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read AudioID FAIL!");
	}
	SetPICSData("AudioID", m_strMeasured);
	FactoryLog();
	return b_res;

}


bool CStringCheckProc::ReadRF_PA_QFE_ID() 
{

	bool b_res = false;
	std::string str_msg;

	/* read  RF PAQFE ID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_RFPAQFEid(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read RF PAQFE ID mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare  RF PAQFE ID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read RF PAQFE ID = " + m_strMeasured;
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
			//if (m_strMeasured == m_str_Value)
			//{ 
			//	str_msg = "Check RF PAQFE ID pass! Value = " + m_strMeasured;
			//	TraceLog(MSG_INFO, str_msg);
			//}
			//else
			//{
			//	str_msg = "RF PAQFE ID is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
					str_msg = "Check RF PAQFE pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "RF PAQFE value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read RF PAQFE ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read PAQFE FAIL!");
	}
	SetPICSData("PAQFE", m_strMeasured);
	FactoryLog();
	return b_res;
}
bool CStringCheckProc::ReadRF_ASM_ID()
{
	bool b_res = false;
	std::string str_msg;

	/* read  RF_ASM_ID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_RFASMid(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read RFASMID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare  RFASMID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read RFASMID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//if (m_strMeasured == m_str_Value)
			//{ 
			//	str_msg = "Check RFASMID pass! Value = " + m_strMeasured;
			//	TraceLog(MSG_INFO, str_msg);
			//}
			//else
			//{
			//	str_msg = "RFASMID is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
					str_msg = "Check RFASMID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "RFASMID value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read RFASMID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read RFASMID FAIL!");
	}
	SetPICSData("RFASMID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CStringCheckProc::ReadSKU_ID()
{

	bool b_res = false;
	std::string str_msg;

	/* read  SKUID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_SKUid(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read SKUID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare  SKUID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read SKUID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//if (m_strMeasured == m_str_Value)
			//{ 
			//	str_msg = "Check SKUID pass! Value = " + m_strMeasured;
			//	TraceLog(MSG_INFO, str_msg);
			//}
			//else
			//{
			//	str_msg = "SKUID is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
					str_msg = "Check SKUID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "SKUID value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read SKUID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read SKUID FAIL!");
	}
	SetPICSData("SKUID", m_strMeasured);
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
bool CStringCheckProc::ReadSOCfeature_ID()
{
	bool b_res = false;
	std::string str_msg;

	/* read  featureID( from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_SOCfeature(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read SOCfeatureID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare  SOCfeatureID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read SOCfeatureID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//if (m_strMeasured == m_str_Value)
			//{ 
			//	str_msg = "Check SOCfeatureID pass! Value = " + m_strMeasured;
			//	TraceLog(MSG_INFO, str_msg);
			//}
			//else
			//{
			//	str_msg = "SOCfeatureID is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
					str_msg = "Check SOCfeatureID pass! ID = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "SOCfeatureID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read SOCfeatureID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read SOCfeatureID FAIL!");
	}
	SetPICSData("SOCfeatureID", m_strMeasured);
	FactoryLog();
	return b_res;



}
bool CStringCheckProc::GaugeCheckID()
{
	bool b_res = false;
	std::string str_msg;

	/* read  GaugeID( from mobile */
	//char szId[FTD_BUF_SIZE] = {0};
	
	//char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	//if (!(b_res = m_pIPhone->ReadGaugeID(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	if (!(b_res = m_pIPhone->ReadGaugeID(m_nFtdPort, m_nFtdTimeOut, szOutput)))
	{
		str_msg = "Read GaugeID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare  GaugeID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read GaugeID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			//if (m_strMeasured == m_str_Value)
			//{ 
			//	str_msg = "Check GaugeID pass! Value = " + m_strMeasured;
			//	TraceLog(MSG_INFO, str_msg);
			//}
			//else
			//{
			//	str_msg = "GaugeID is not matched. Moble:" + m_strMeasured + "-- Config File:" + m_str_Value;
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
					str_msg = "Check GaugeID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "GaugeID value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read GaugeID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read GaugeID FAIL!");
	}
	SetPICSData("GaugeID", m_strMeasured);
	FactoryLog();
	return b_res;



}
bool CStringCheckProc::ReadMCPddrmr6()
{
	bool b_res = false;
	std::string str_msg;

	/* read CPU ID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_MCPddrmr6(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read DDR MR6 from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare DDR MR6 with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read DDR MR6 = " + m_strMeasured;
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
					str_msg = "Check DDR MR6 pass! ID = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "DDR MR6 is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read DDR MR6 PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read DDR MR6 FAIL!");
	}
	SetPICSData("DDR_MR6", m_strMeasured);
	FactoryLog();
	return b_res;
}



bool CStringCheckProc::Read_GsensorID()
{
	bool b_res = false;
	std::string str_msg;

	/* read Gsensor ID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_SNSRgsensorID(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read Gsensor ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare Gsensor ID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read Gsensor ID = " + m_strMeasured;
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
					str_msg = "Check Gsensor IDpass! ID = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "Gsensor ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read Gsensor ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read Gsensor ID FAIL!");
	}
	SetPICSData("Gsensor_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}
bool CStringCheckProc::Read_GsensorSelfTest()
{
	bool b_res = false;
	std::string str_msg;

	/* read GsensorSelfTest from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_SNSRgsensorSelfTest(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read Gsensor SelfTest from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare GsensorSelfTest with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read Gsensor SelfTest = " + m_strMeasured;
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
					str_msg = "Gsensor SelfTest pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "Gsensor SelfTest value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read Gsensor SelfTest PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read Gsensor SelfTest FAIL!");
	}
	SetPICSData("Gsensor_SelfTest", m_strMeasured);
	FactoryLog();
	return b_res;
}
bool CStringCheckProc::Read_EcompassID()
{
	bool b_res = false;
	std::string str_msg;

	/* read Ecompass ID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_SNSRecompassID(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read Ecompass ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare Ecompass ID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read Ecompass ID = " + m_strMeasured;
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
					str_msg = "Check Ecompass ID pass! ID = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "Ecompass ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read Ecompass ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read Ecompass ID FAIL!");
	}
	SetPICSData("Ecompass_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}
bool CStringCheckProc::Read_EcompassSelfTest()
{

	bool b_res = false;
	std::string str_msg;

	/* read EcompassSelfTest from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_SNSREcompassSelfTest(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read Ecompass SelfTest from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare EcompassSelfTest with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read Ecompass SelfTest = " + m_strMeasured;
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
					str_msg = "Ecompass SelfTest pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "Ecompass SelfTest value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read Ecompass SelfTest  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read Ecompass SelfTest  FAIL!");
	}
	SetPICSData("Ecompass_SelfTest ", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CStringCheckProc:: Read_BTSelfTest()
{
	bool b_res = false;
	std::string str_msg;

	/* read BTSelfTest from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_BTSelfTest(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read BT SelfTest from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare BTSelfTest with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read BT SelfTest = " + m_strMeasured;
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
					str_msg = "BT SelfTest pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "BT SelfTest value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read BT SelfTest  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read BT SelfTest  FAIL!");
	}
	SetPICSData("BT_SelfTest", m_strMeasured);
	FactoryLog();
	return b_res;
}
bool CStringCheckProc::Read_FMSelfTest()
{
	bool b_res = false;
	std::string str_msg;

	/* read FMSelfTest from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_FMR_SelfTest(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read FM SelfTest from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare FMSelfTest with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read FM SelfTest = " + m_strMeasured;
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
					str_msg = "FM SelfTest pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "FM SelfTest value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read FM SelfTest  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read FM SelfTest  FAIL!");
	}
	SetPICSData("FM_SelfTest", m_strMeasured);
	FactoryLog();
	return b_res;
}


bool CStringCheckProc::Read_NFC_ID()
{

	bool b_res = false;
	std::string str_msg;

	/* read NFC ID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	int iRetryCount = 0;
	int iRetryCountMax = 1;


	for ( iRetryCount = 0 ; iRetryCount < iRetryCountMax ; iRetryCount++)
	{
		memset( szInput , '\0' , FTD_BUF_SIZE );
		memset( szOutput , '\0' , FTD_BUF_SIZE );
		b_res = m_pIPhone->FTD_NFCid(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput);

		if ( b_res ) break;

		if ( iRetryCount == iRetryCountMax-1 )
		{
			str_msg = "Read NFC ID from mobile fail";
			TraceLog(MSG_INFO, str_msg);
		}

	}

	//if (!(b_res = m_pIPhone->FTD_NFCid(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	//{
	//	str_msg = "Read NFC ID from mobile fail";
	//	TraceLog(MSG_INFO, str_msg);
	//}

	/* Compare NFC ID with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read NFC ID = " + m_strMeasured;
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
					str_msg = "NFC ID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "NFC ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read NFC ID  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read NFC ID  FAIL!");
	}
	SetPICSData("NFC_ID", m_strMeasured);
	FactoryLog();
	return b_res;

}

bool CStringCheckProc::ReadPMICID()
{
	bool b_res = false;
	std::string str_msg;

	/* read Read PMIC ID  from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_PMICid(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read PMIC ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare Read PMIC ID  with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read Read PMIC ID  = " + m_strMeasured;
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
					str_msg = "Read PMIC ID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "Read PMIC ID  value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read PMIC ID  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read PMIC ID FAIL!");
	}
	SetPICSData("PMIC_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CStringCheckProc::Read_FlashLED_ID()
{
	bool b_res = false;
	std::string str_msg;

	/* read Read FlashLED ID  from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_CAMFlashLEDID(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read FlashLED ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare Read FlashLED ID  with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read Read FlashLED ID  = " + m_strMeasured;
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
					str_msg = "Read FlashLED ID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "Read FlashLED ID  value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read FlashLED ID  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read FlashLED ID   FAIL!");
	}
	SetPICSData("FlashLED_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}



bool CStringCheckProc::Read_BuckID()
{
	bool b_res = false;
	std::string str_msg;

	/* read Read Buck ID  from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_ReadBuckID(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read Buck ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare Read Buck ID  with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read Read Buck ID  = " + m_strMeasured;
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
					str_msg = "Read Buck ID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "Read Buck ID  value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read Buck ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read Buck ID FAIL!");
	}
	SetPICSData("Buck_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}
bool CStringCheckProc::Read_Buckfeature()
{
	bool b_res = false;
	std::string str_msg;

	/* read Read Buckfeature ID  from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_ReadBuckfeature(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "Read Buckfeature ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare Read Buckfeature ID  with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read Read Buckfeature ID  = " + m_strMeasured;
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
					str_msg = "Read Buckfeature ID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "Read Buckfeature ID  value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read Buckfeature ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read Buckfeature ID FAIL!");
	}
	SetPICSData("Buckfeature_ID", m_strMeasured);
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

bool CStringCheckProc::Read_GyroID()
{
	bool b_res = false;
	std::string str_msg;

	/* read Read Gyro ID  from mobile */
	//char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};

	if (!(b_res = m_pIPhone->ReadGyroID(m_nFtdPort, m_nFtdTimeOut, szOutput)))
	{
		str_msg = "Read Gyro ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare Read Gyro ID  with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read Read Gyro ID  = " + m_strMeasured;
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
					str_msg = "Read Gyro ID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "Read Gyro ID  value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read Gyro ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read Gyro ID FAIL!");
	}
	SetPICSData("Gyro_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}
bool CStringCheckProc::Read_ChargerID()
{
	bool b_res = false;
	std::string str_msg;

	/* read Read Charger ID  from mobile */
	//char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};

	if (!(b_res = m_pIPhone->ReadChargerID(m_nFtdPort, m_nFtdTimeOut, szOutput)))
	{
		str_msg = "Read Charger ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare Read Charger ID  with config file */
	if (b_res)
	{
		m_strMeasured = szOutput;
		if (m_b_CheckMatch == false)
		{
			str_msg = "Read Read Charger ID  = " + m_strMeasured;
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
					str_msg = "Read Charger ID pass! Value = " + m_strMeasured;
					TraceLog(MSG_INFO, str_msg);
					b_match = true;
					break;
				}
			}
			if (!b_match)
			{
				str_msg = "Read Charger ID  value is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_Value;
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
		TraceLog(MSG_INFO, "Read Charger ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Read Charger ID FAIL!");
	}
	SetPICSData("Charger_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}

