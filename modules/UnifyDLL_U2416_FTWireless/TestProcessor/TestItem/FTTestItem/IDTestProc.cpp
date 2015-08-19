/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CDisplay object.
 * FileName   : IDTestProc.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "IDTestProc.h"


RF_IMPLEMENT_DYNCREATE(CIDTestProc)


bool CIDTestProc::PreRun()
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
	return true;
}

bool CIDTestProc::Run()
{
	/* judge sensor type, then run different API function based on type */
	if (m_str_IDType == GAUGE)
	{
		m_strItemCode = CStr::IntToStr(GaugeICIDTest_BaseItemCode);
		m_strErrorCode = FunErr_Guage_ID_Fail;
		return GaugeCheckID();
	}
	else if (m_str_IDType == CHARGER)
	{
		m_strItemCode = CStr::IntToStr(CHARGERICTest_BaseItemCode);
		m_strErrorCode = FunErr_Charge_ID_Fail;
		return ChargerCheckID();
	}
	else if (m_str_IDType == BATTERY)
	{
		m_strItemCode = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_Check_Battery_ID_Fail;
		return BatteryCheckID();
	}
	else if (m_str_IDType == CPUID)
	{
		m_strItemCode = CStr::IntToStr(ReadCPUid_BaseItemCode);
		m_strErrorCode = FunErr_Check_CPU_Version_Fail;
		return CPUCheckID();
	}
	else if (m_str_IDType == LSENSOR)
	{
		m_strItemCode = CStr::IntToStr(LSensor_ReadID_BaseItemCode);
		m_strErrorCode = FunErr_LSensor_Check_ID_Fail;
		return LSensorCheckID();
	}
	else if (m_str_IDType == PSENSOR)
	{
		m_strItemCode = CStr::IntToStr(PSensor_ReadID_BaseItemCode);
		m_strErrorCode = FunErr_PSensor_Check_ID_Fail;
		return PSensorCheckID();
	}
	else if (m_str_IDType == GSENSOR)
	{
		m_strItemCode = CStr::IntToStr(GSensorTestGetID_BaseItemCode);
		m_strErrorCode = FunErr_GSensor_ID_Fail;
		return GSensorCheckID();
	}
	else if (m_str_IDType == E_COMPASS)
	{
		m_strItemCode = CStr::IntToStr(ECompass_BaseItemCode);
		m_strErrorCode = FunErr_ECompassr_ID_Fail;
		return ECompassCheckID();
	}
	else if (m_str_IDType == FRONT_CAMERA)
	{
		m_strItemCode = CStr::IntToStr(Camera_Test_BaseItemCode);
		m_strErrorCode = FunErr_Front_Camera_Test_Read_ID_Fail;
		return FrontCameraCheckID();
	}
	else if (m_str_IDType == REAR_CAMERA)
	{
		m_strItemCode = CStr::IntToStr(Camera_Test_BaseItemCode);
		m_strErrorCode = FunErr_Rear_Camera_Test_Read_ID_Fail;
		return RearCameraCheckID();
	}
	else if (m_str_IDType == BOSTON_REAR_CAMERA)
	{
		m_strItemCode = CStr::IntToStr(Camera_Test_BaseItemCode);
		m_strErrorCode = FunErr_Rear_Camera_Test_Read_ID_Fail;
		return BostonRearCameraCheckID();
	}
	else if (m_str_IDType == GYRO)
	{
		m_strItemCode = CStr::IntToStr(GYROIDTest_BaseItemCode);
		m_strErrorCode = FunErr_Gyro_ID_Fail;
		return GyroCheckID();
	}
	else if (m_str_IDType == CAPKEY)
	{
		m_strItemCode = CStr::IntToStr(CAPKEYCHIPTest_BaseItemCode);
		m_strErrorCode = FunErr_Capkey_ID_Fail;
		return CapKeyCheckID();
	}
	else if (m_str_IDType == TOUCH)
	{
		m_strItemCode = CStr::IntToStr(TOUCHID_Test_BaseItemCode);
		m_strErrorCode = FunErr_Touch_ID_Test_Fail;
		return TouchCheckID();
	}
	else if (m_str_IDType == TOUCHMODEL)
	{
		m_strItemCode = CStr::IntToStr(TOUCHMODEL_Test_BaseItemCode);
		m_strErrorCode = "-";
		return TouchCheckModel();
	}
	else if (m_str_IDType == DFIVERSION)
	{
		m_strItemCode = CStr::IntToStr(Gauge_Check_DFI_Version_BaseItemCode);
		m_strErrorCode = FunErr_DFI_Version_Check_Fail;
		return DFIcheckVersion();
	}
	else if (m_str_IDType == SCI2CID)
	{
		m_strItemCode = CStr::IntToStr(Smart_Cover_BaseItemCode);
		m_strErrorCode = FunErr_SmartCover_CheckI2C_Fail;
		return SmartCoverCheckI2CID();
	}
	else if (m_str_IDType == AntennaICID)
	{
		m_strItemCode = CStr::IntToStr(WLAN_Test_BaseItemCode);
		m_strErrorCode = FunErr_WLAN_Antenna_IC_ID_Fail;

		return CheckAntennaICID();
	}
	else if (m_str_IDType == "USBTest")//YTT TEMP PUT HERER
	{
		//m_strItemCode = CStr::IntToStr(WLAN_Test_BaseItemCode);
		m_strErrorCode = FunErr_USB_Test_Fail;

		return CheckUSBTest();
	}
	else
	{
		m_strMessage = "The Type of ID is not defined";
		TraceLog(MSG_INFO, "The Type of ID is not defined");
		return false;
	}
	return true;
}

bool CIDTestProc::PostRun()
{
	return true;
}

bool CIDTestProc::InitData(std::map<std::string, std::string>& paramMap)
{
	/* SensorType */
	if (paramMap.find(std::string("IDType")) != paramMap.end())
	{
		m_str_IDType = paramMap[std::string("IDType")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'IDType' for class 'CIDTestProc'");
		return false;
	}

	/* ID */
	if (paramMap.find(std::string("ID")) != paramMap.end())
	{
		m_str_ID = paramMap[std::string("ID")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'ID' for class 'CIDTestProc'");
		return false;
	}

	if (m_str_IDType == CPUID)
	{
		if (paramMap.find(std::string("SecID")) != paramMap.end())
		{
			m_str_SecID = paramMap[std::string("SecID")];
		}
		else
		{
			TraceLog(MSG_INFO, "Failed to Get filed 'SecID' for class 'CIDTestProc'");
			return false;
		}
	}

	if (m_str_IDType == SCI2CID)
	{
		if (paramMap.find(std::string("Address")) != paramMap.end())
		{
			m_str_SCI2CId = paramMap[std::string("Address")];
		}
		else
		{
			TraceLog(MSG_INFO, "Failed to Get filed 'Address' for class 'CIDTestProc'");
			return false;
		}
	}
	return true;
}

bool CIDTestProc::GaugeCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = true;
	char szId[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->ReadGaugeID(m_nFtdPort, m_nFtdTimeOut, szId))
	{
		str_msg = "Read Gauge ID fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = szId;
		str_msg = "Check Gauge ID pass! ID = " + m_strMeasured;
		TraceLog(MSG_INFO, str_msg);
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Gauge ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Gauge ID Command FAIL!");
	}
	SetPICSData("GaugeID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::ChargerCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = true;
	char szId[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->ReadChargerID(m_nFtdPort, m_nFtdTimeOut, szId))
	{
		str_msg = "Read Charger ID fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = szId;
		if (m_strMeasured.compare(m_str_ID) == 0)
		{
			str_msg = "Check Charger ID pass! ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "Charger ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Charger ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Charger ID FAIL!");
	}
	SetPICSData("ChargerID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::BatteryCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = true;
	char szId[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->ReadBatteryID(m_nFtdPort, m_nFtdTimeOut, szId))
	{
		str_msg = "Read Battery ID fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = szId;
		int nCheck = atoi(m_strMeasured.c_str());
		size_t find = m_str_ID.find(",") + 1;
		int nCheckLower = atoi(m_str_ID.c_str());
		int nCheckHigher = atoi(m_str_ID.c_str() + find);
		if (nCheckLower < nCheck && nCheck < nCheckHigher)
		{
			str_msg = "Check Battery ID pass! ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "Battery ID is not in range. Moble:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Battery ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Battery ID FAIL!");
	}
	SetPICSData("BatteryID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::CPUCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = true;
	char szId[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->ReadCpuID(m_nFtdPort, m_nFtdTimeOut, szId))
	{
		str_msg = "Read CPU ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = szId;
		if (m_strMeasured.compare(m_str_ID) == 0)
		{
			str_msg = "Check CPU ID pass! ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else if (m_strMeasured.compare(m_str_SecID) == 0)
		{
			str_msg = "Check CPU ID pass! ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "CPU ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_ID + " and " + m_str_SecID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check CPU ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check CPU ID FAIL!");
	}
	SetPICSData("CPU_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::GSensorCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = true;
	char sz_GSensorID[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->ReadGSensorID(m_nFtdPort, m_nFtdTimeOut, sz_GSensorID))
	{
		str_msg = "Read GSensor ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = sz_GSensorID;
		if (m_strMeasured.compare(m_str_ID) == 0)
		{
			str_msg = "Check GSensor ID pass! ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "GSensor ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check GSensor ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check GSensor ID FAIL!");
	}
	SetPICSData("GSENSOR_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::ECompassCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = true;
	char szId[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->ReadECompassID(m_nFtdPort, m_nFtdTimeOut, szId))
	{
		str_msg = "Read E-Compass ID fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = szId;
		if (m_strMeasured.compare(m_str_ID) == 0)
		{
			str_msg = "Check E-Compass ID pass! ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "E-Compass ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check E-Compass ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check E-Compass ID FAIL!");
	}
	SetPICSData("ECompassID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::LSensorCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = true;
	char szId[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->ReadLSensorID(m_nFtdPort, m_nFtdTimeOut, szId))
	{
		str_msg = "Read LSensor ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = szId;
		if (m_strMeasured.compare(m_str_ID) == 0)
		{
			str_msg = "Check LSensor ID pass! ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "LSensor ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check LSensor ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check LSensor ID FAIL!");
	}
	SetPICSData("LSENSOR_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::PSensorCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = true;
	char sz_PSensorID[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->ReadPSensorID(m_nFtdPort, m_nFtdTimeOut, sz_PSensorID))
	{
		str_msg = "Read PSensor ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = sz_PSensorID;
		if (m_strMeasured.compare(m_str_ID) == 0)
		{
			str_msg = "Check PSensor ID pass! ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "PSensor ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check PSensor ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check PSensor ID FAIL!");
	}
	SetPICSData("PSENSOR_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::FrontCameraCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = true;
	char sz_FrontCameraID[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_CAMReadID(m_nFtdPort, m_nFtdTimeOut, "front", sz_FrontCameraID))
	{
		str_msg = "Read camera front ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = sz_FrontCameraID;
		StrVtr vToken;
		CStr::ParseString(m_str_ID.c_str(), _T(","), vToken);
		bool b_match = false;
		for (size_t i = 0; i < vToken.size(); i++)
		{
			if ((strcmp(sz_FrontCameraID, vToken[i].c_str()) == 0))
			{
				str_msg = "Check Front Camera ID pass! ID = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
				b_match = true;
				break;
			}
		}
		if (!b_match)
		{
			str_msg = "Front Camera ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Front Camera ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Front Camera ID FAIL!");
	}
	SetPICSData("readFrontID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::RearCameraCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = true;
	char sz_RearCameraID[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_CAMReadID(m_nFtdPort, m_nFtdTimeOut, "back", sz_RearCameraID))
	{
		str_msg = "Read camera back ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = sz_RearCameraID;
		StrVtr vToken;
		CStr::ParseString(m_str_ID.c_str(), _T(","), vToken);
		bool b_match = false;
		for (size_t i = 0; i < vToken.size(); i++)
		{
			if ((strcmp(sz_RearCameraID, vToken[i].c_str()) == 0))
			{
				str_msg = "Check Back Camera ID pass! ID = " + m_strMeasured;
				TraceLog(MSG_INFO, str_msg);
				b_match = true;
				break;
			}
		}
		if (!b_match)
		{
			str_msg = "Back Camera ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Back Camera ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Back Camera ID FAIL!");
	}
	SetPICSData("readRearID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::BostonRearCameraCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = true;
	char sz_RearCameraID[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_CAMReadID(m_nFtdPort, m_nFtdTimeOut, "back", sz_RearCameraID))
	{
		str_msg = "Read camera back ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare ID with config file */
	if (b_res)
	{
		bool b_match = false;
		//m_strMeasured = sz_RearCameraID;
	//	CString csRearCameraDataA, csRearCameraDataB;
		std::string stdRearCameraData = sz_RearCameraID;;

		//sscanf(sz_RearCameraID, "%8s", csRearCameraDataA);

		StrVtr vToken;
		CStr::ParseString(stdRearCameraData, _T(","), vToken);
		m_strMeasured = vToken[0].c_str();

		if ((strcmp(vToken[0].c_str(), m_str_ID.c_str()) == 0))
		{
			str_msg = "Check Back Camera ID pass! ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
			b_match = true;
		}
		if (!b_match)
		{
			str_msg = "Back Camera ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Back Camera ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Back Camera ID FAIL!");
	}
	SetPICSData("readRearID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::GyroCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = true;
	char szId[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->ReadGyroID(m_nFtdPort, m_nFtdTimeOut, szId))
	{
		str_msg = "Read Gyro ID fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = szId;
		if (m_strMeasured.compare(m_str_ID) == 0)
		{
			str_msg = "Check Gyro ID pass! ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "Gyro ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Gyro ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Gyro ID FAIL!");
	}
	SetPICSData("GyroID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::CapKeyCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = true;
	char szId[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->ReadCapKeyID(m_nFtdPort, m_nFtdTimeOut, szId))
	{
		str_msg = "Read Cap Key ID fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = szId;
		if (m_strMeasured.compare(m_str_ID) == 0)
		{
			str_msg = "Check Cap Key ID pass! ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "Cap Key ID is not matched. Moble:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Cap Key ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Cap Key ID FAIL!");
	}
	SetPICSData("CapKeyID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::TouchCheckID()
{
	std::string str_msg;
	/* read ID from mobile */
	bool b_res = false;
	char szId[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->ReadTouchID(m_nFtdPort, m_nFtdTimeOut, szId)))
	{
		str_msg = "Read touch ID from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare ID with config file */
	if (b_res)
	{
		m_strMeasured = szId;
		if (m_strMeasured.compare(m_str_ID) == 0)
		{
			str_msg = "Check touch ID pass! ID = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "Touch ID is not matched. Mobile:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check touch ID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check touch ID FAIL!");
	}
	SetPICSData("TOUCH_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::TouchCheckModel()
{
	std::string str_msg;
	//Check SWversion. if version=v51.005.03.g_03 or v51.006.03.g_03, skip this test item.
	char output[100] = {0};
	char error[100] = {0};
	if (!m_pIPhone->ExecAdbCommand("shell getprop ro.build.oemversion.main", output, error))
	{
		m_strMessage = "Get Software Version fail";
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Get_Software_Version_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}

	for (int i = 0; i < 100; i++)
	{
		if ((output[i] == 0x0A) || (output[i] == 0x0D))
			output[i] = 0x00;
	}

	//if NOT "v51.005.03.g_03", "v51.006.03.g_03" do.
	if ((strstr(output, "v51.005.03.g_03") == NULL) && (strstr(output, "v51.006.03.g_03") == NULL))
	{
		// read ID from mobile //
		bool b_res = false;
		char szModel[FTD_BUF_SIZE] = {0};
		if (! m_pIPhone->ReadTouchModel(m_nFtdPort, m_nFtdTimeOut, szModel))
		{
			str_msg = "Read touch model from mobile fail";
			TraceLog(MSG_INFO, str_msg);
		}
		m_strMeasured = szModel;
		SetPICSData("TOUCH_ID", m_strMeasured);
		str_msg =  _T("Check touch model PASS! ");
		str_msg = str_msg + output;
		TraceLog(MSG_INFO, output);
	}
	else
	{
		str_msg = "No Check touch model. ";
		str_msg = str_msg + output;
		TraceLog(MSG_INFO, output);
	}
	m_strResult = "PASS";
	m_strErrorCode = "-";
	m_strMessage = str_msg;
	TraceLog(MSG_INFO, str_msg);
	FactoryLog();
	return true;
}

bool CIDTestProc::DFIcheckVersion(void)
{
	std::string str_msg;
	/* Read Version from mobile */
	bool b_res = false;
	char szVersion[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->CheckDFIVersion(m_nFtdPort, m_nFtdTimeOut, szVersion)))
	{
		str_msg = "Read DFI Version from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare Version with config file */
	if (b_res)
	{
		m_strMeasured = szVersion;
		if (m_strMeasured.compare(m_str_ID) == 0)
		{
			str_msg = "Check DFI Version PASS! Version = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "DFI Version is not matched. Mobile:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check DFI Version PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check DFI Version FAIL!");
	}
	SetPICSData("DFI_VERSION", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::SmartCoverCheckI2CID()
{
	std::string str_msg;
	/* Read Version from mobile */
	bool b_res = false;
	char sz_InBuffer[FTD_BUF_SIZE] = {0};
	memcpy(sz_InBuffer, m_str_SCI2CId.c_str(), m_str_SCI2CId.length());
	str_msg = std::string("InBuffer: ") + sz_InBuffer;
	TraceLog(MSG_INFO, str_msg);
	char szVersion[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->Sapporo_SCoverCheckI2C(m_nFtdPort, m_nFtdTimeOut, sz_InBuffer, szVersion)))
	{
		str_msg = "Read smart cover I2C id from mobile fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare Version with config file */
	if (b_res)
	{
		m_strMeasured = szVersion;
		StrVtr vToken;
		CStr::ParseString(m_str_ID, ",", vToken);
		bool b_match = false;
		for (size_t i = 0; i < vToken.size(); i++)
		{
			str_msg = "Token = " + vToken[i];
			TraceLog(MSG_INFO, str_msg);
			if (!(strstr(szVersion, vToken[i].c_str()) == NULL))
			{
				b_match = true;
				break;
			}
		}
		if (b_match)
		{
			str_msg = "Check smart cover I2C id PASS! Id = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "Smart cover I2C id is not matched. Mobile:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check smart cover I2C id PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check smart cover I2C id FAIL!");
	}
	SetPICSData("SC_I2C_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}

bool CIDTestProc::CheckAntennaICID()
{
	bool b_res = false;
	std::string str_msg;

	/* Read Version from mobile */
	char szVersion[FTD_BUF_SIZE] = {0};
	char sz_InBuffer[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_AntennaIC(m_nFtdPort, m_nFtdTimeOut, sz_InBuffer, szVersion)))
	{
		str_msg = "Read FTD_AntennaIC from mobile Fail";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Compare Version with config file */
	if (b_res)
	{
		m_strMeasured = szVersion;
		if(m_strMeasured.compare(m_str_ID) == 0)
		{
			str_msg = "Check FTD_AntennaIC PASS! Version = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "FTD_AntennaIC  is not matched. Mobile:" + m_strMeasured + ",Config:" + m_str_ID;
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check AntennaICID  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check AntennaICID  FAIL!");
	}
	SetPICSData("AntennaIC_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}

//ytt usb put here is dirty , eed to change code after
bool CIDTestProc::CheckUSBTest()
{
	bool b_res = false;
	std::string str_msg;

	/* Read Version from mobile */
	char szVersion[FTD_BUF_SIZE] = {0};
	char sz_InBuffer[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_USBTest(m_nFtdPort, m_nFtdTimeOut, sz_InBuffer)))
	{
		str_msg = "usb test Fail";
		TraceLog(MSG_INFO, str_msg);
	}


	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check USBTest  PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check USBTest  FAIL!");
	}
	//SetPICSData("AntennaIC_ID", m_strMeasured);
	FactoryLog();
	return b_res;
}