#include "stdafx.h"
#include "CheckDeviceInfo.h"
#include <atlenc.h>

RF_IMPLEMENT_DYNCREATE(CCheckDeviceInfo)

bool CCheckDeviceInfo::PreRun()
{
	m_strResult = "";
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strBand = "";
	m_strChannel = "";
	m_strUpper = "";
	m_strLower = "";
	m_strMeasured = "";
	m_strUnit = "";
	m_strMessage = "";
	return true;
}

bool CCheckDeviceInfo::Run()
{
	bool bRes = MainFunction();
	return bRes;
}

bool CCheckDeviceInfo::PostRun()
{
	return true;
}

bool CCheckDeviceInfo::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CCheckDeviceInfo' default Retry=3"));
		m_iRetry = 3;
	}

	if (paramMap.find(std::string("CheckItems")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'CheckItems' for class 'CCheckDeviceInfo'"));
		return false;
	}
	std::vector<std::string> vCheckList;
	STDStringTokenizer(paramMap["CheckItems"], ",", vCheckList);
	if(vCheckList.size() != 6)
	{
		TraceLog(MSG_INFO, _T("Failed to get 'CheckItems' param for class 'CCheckDeviceInfo', size != 6"));
		return false;
	}
	m_bCheckIMEI = (atoi(vCheckList[0].c_str()) == 1) ?	true : false;
	m_bCheckBT = (atoi(vCheckList[1].c_str()) == 1) ? true : false;
	m_bCheckWIFI = (atoi(vCheckList[2].c_str()) == 1) ? true : false;
	m_bCheckMSN = (atoi(vCheckList[3].c_str()) == 1) ? true : false;
	m_bCheckFSN = (atoi(vCheckList[4].c_str()) == 1) ? true : false;
	m_bCheckSIMData = (atoi(vCheckList[5].c_str()) == 1) ? true : false;

	return true;
}

bool CCheckDeviceInfo::MainFunction()
{
	if(m_bCheckIMEI)
	{
		if( !CheckIMEI()) return false;
	}
	if(m_bCheckBT)
	{
		if( !CheckBT()) return false;
	}
	if(m_bCheckWIFI)
	{
		if( !CheckWIFI()) return false;
	}
	if(m_bCheckMSN)
	{
		if( !CheckMSN()) return false;
	}
	if(m_bCheckFSN)
	{
		if( !CheckFSN()) return false;
	}
	if(m_bCheckSIMData)
	{
		if( !CheckSIMData()) return false;
	}
	m_strMessage = "Check OTP success";
	m_strResult = "PASS";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return true;
}

bool CCheckDeviceInfo::CheckIMEI()
{
	char sz_outData[FTD_BUF_SIZE] = {0};
	char szIMEI[32] = {0};
	strcpy(szIMEI, g_strIMEI.c_str());

	m_strMessage = "Check IMEI Start.";
	FactoryLog();
	if (!m_pIPhone->FTD_FAC_OTPRead(m_nFtdPort, m_nFtdTimeOut, "IMEI", sz_outData))
	{
		m_strMessage = "Get IMEI from phone fail.....";
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Read_IMEI_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	m_strMessage = MakeStr("Get IMEI from phone Success...%s", sz_outData);
	FactoryLog();

	TraceLog(MSG_ERROR, "Compare IMEI from UI with from phone...");

	if(strcmp(szIMEI, sz_outData) != 0)
	{
		m_strMessage = MakeStr("IMEI from UI: %s", szIMEI);
		FactoryLog();
		m_strMessage = MakeStr("IMEI from Phone: %s", sz_outData);
		FactoryLog();
		m_strMessage = "IMEI read from phone is not equal to from UI ......";
		FactoryLog();
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Compare_IMEI_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	SetPICSData("IMEI", szIMEI);
	m_strMessage = "Check IMEI Success...";
	FactoryLog();
	return true;
}

bool CCheckDeviceInfo::CheckBT()
{
	char sz_outData[FTD_BUF_SIZE] = {0};
	char szBT[32] = {0};
	strcpy(szBT, m_mapQueryData["BT_ADDR"].c_str());

	m_strMessage = "Check BT ADDR Start.";
	FactoryLog();
	if (!m_pIPhone->FTD_FAC_OTPRead(m_nFtdPort, m_nFtdTimeOut, "BT", sz_outData))
	{
		m_strMessage = "Get BT ADDR from phone fail.....";
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Read_BT_Address_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	m_strMessage = MakeStr("Get BT ADDR from phone Success...%s", sz_outData);
	FactoryLog();

	TraceLog(MSG_ERROR, "Compare BT ADDR from UI with from phone...");
	m_strMessage = MakeStr("BT ADDR from UI: %s", szBT);
	FactoryLog();
	m_strMessage = MakeStr("BT ADDR from Phone: %s", sz_outData);
	FactoryLog();

	if( !CheckNoCaseString(szBT, sz_outData))
	{
		m_strErrorCode = CommErr_Compare_BT_Address_Fail;
		m_strMessage = "BT ADDR read from phone is not equal to from UI ......";
		FactoryLog();
		return false;
	}

	SetPICSData("BT", szBT);
	m_strMessage = "Check BT ADDR Success...";
	FactoryLog();
	return true;
}
bool CCheckDeviceInfo::CheckWIFI()
{
	char sz_outData[FTD_BUF_SIZE] = {0};
	char szWIFI[32] = {0};
	strcpy(szWIFI, m_mapQueryData["MAC_ADDR"].c_str());

	m_strMessage = "Check WIFI ADDR Start.";
	FactoryLog();
	if (!m_pIPhone->FTD_FAC_OTPRead(m_nFtdPort, m_nFtdTimeOut, "Wifi", sz_outData))
	{
		m_strMessage = "Get WIFI ADDR from phone fail.....";
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Read_WLAN_Address_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	m_strMessage = MakeStr("Get WIFI ADDR from phone Success...%s", sz_outData);
	FactoryLog();

	TraceLog(MSG_ERROR, "Compare WIFI ADDR from UI with from phone...");
	m_strMessage = MakeStr("WIFI ADDR from UI: %s", szWIFI);
	FactoryLog();
	m_strMessage = MakeStr("WIFI ADDR from Phone: %s", sz_outData);
	FactoryLog();

	if( !CheckNoCaseString(szWIFI, sz_outData))
	{
		m_strErrorCode = CommErr_Compare_WLAN_Address_Fail;
		m_strMessage = "WIFI ADDR read from phone is not equal to from UI ......";
		FactoryLog();
		return false;
	}

	SetPICSData("WLAN", szWIFI);
	m_strMessage = "Check WIFI ADDR Success...";
	FactoryLog();
	return true;
}


bool CCheckDeviceInfo::CheckMSN()
{
	char sz_outData[FTD_BUF_SIZE] = {0};
	char szMSN[32] = {0};
	strcpy(szMSN, m_mapQueryData["MSN"].c_str());

	// Read Service Code from phone
	m_strMessage = "Read Mobile Serial Number from Target start...";
	FactoryLog();
	if (!m_pIPhone->FTD_FAC_CFGRead(m_nFtdPort, m_nFtdTimeOut, "1024,20", sz_outData))
	{
		m_strMessage = "Read Mobile Serial Number from Target Fail...";
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Read_Serial_Number_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	m_strMessage = MakeStr("Read Mobile Serial Number from Target Success...%s", sz_outData);
	FactoryLog();


	// Comare
	TraceLog(MSG_ERROR, "Compare Mobile Serial Number from UI with from phone...");
	m_strMessage = MakeStr("Mobile Serial Number from UI: %s", szMSN);
	FactoryLog();
	m_strMessage = MakeStr("Mobile Serial Number from Phone: %s", sz_outData);
	FactoryLog();

	if( !CheckNoCaseString(szMSN, sz_outData))
	{
		m_strErrorCode = CommErr_Compare_Serial_Number_Fail;
		m_strMessage = "Mobile Serial Number from phone is not equal to from UI ......";
		FactoryLog();
		return false;
	}

	SetPICSData("PPID", szMSN);
	m_strMessage = "Check Mobile Serial Number Success...";
	FactoryLog();
	return true;
}
bool CCheckDeviceInfo::CheckFSN()
{
	char sz_outData[FTD_BUF_SIZE] = {0};
	char szFSN[64] = {0};
	strcpy(szFSN, m_mapQueryData["FSN"].c_str());

	// Read FSN from OTP //
	m_strMessage = "Read FSN Start.";
	FactoryLog();
	if (!m_pIPhone->FTD_FAC_OTPRead(m_nFtdPort, m_nFtdTimeOut, "SerialNo", sz_outData))
	{
		m_strResult = "FAIL";
		m_strMessage = "Read FSN from phone fail.....";
		m_strErrorCode = CommErr_Read_FSN_Fail;
		FactoryLog();
		return false;
	}
	m_strMessage = MakeStr("Read FSN from OTP: %s", sz_outData);
	FactoryLog();

	// Check FSN //
	if( !CheckNoCaseString(szFSN, sz_outData))
	{
		m_strErrorCode = CommErr_Compare_Serial_Number_Fail;
		m_strMessage = "Mobile Serial Number from phone is not equal to from UI ......";
		FactoryLog();
		return false;
	}

	SetPICSData("FSN", szFSN);
	m_strMessage = "Check Flash Serial Number Success...";
	FactoryLog();
	return true;
}
bool CCheckDeviceInfo::CheckSIMData()
{
	char sz_outData[FTD_BUF_SIZE] = {0};
	char szSIMPublicKey[FTD_BUF_SIZE] = {0};
	strcpy(szSIMPublicKey, m_mapQueryData["SIMPublicKey"].c_str());

	// Read Public Key from OTP //
	m_strMessage = "Read Public Key Start.";
	FactoryLog();
	if (!m_pIPhone->FTD_FAC_OTPRead(m_nFtdPort, 15000, "Qlock", sz_outData))
	{
		m_strResult = "FAIL";
		m_strMessage = "Read Public Key from phone fail.....";
		m_strErrorCode = CommErr_Read_Public_key_Fail;
		FactoryLog();
		return false;
	}
	m_strMessage = MakeStr("Read Public Key from OTP: %s", sz_outData);
	FactoryLog();

	// Check Public Key //
	if(strcmp(szSIMPublicKey, sz_outData) != 0)
	{
		m_strResult = "FAIL";
		m_strMessage = "Compare Public Key from phone and Server fail...";
		m_strErrorCode = CommErr_Compare_Public_key_Fail;
		FactoryLog();
		return false;
	}

	m_strMessage = "Check Public Key Success...";
	FactoryLog();
	return true;
}
