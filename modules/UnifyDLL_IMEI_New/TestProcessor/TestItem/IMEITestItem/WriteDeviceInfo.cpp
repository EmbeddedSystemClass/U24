#include "stdafx.h"
#include "WriteDeviceInfo.h"

#define Zero_IMEI                   "000000000000000"

RF_IMPLEMENT_DYNCREATE(CWriteDeviceInfo)

bool CWriteDeviceInfo::PreRun()
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

bool CWriteDeviceInfo::Run()
{
	bool bRes = MainFunction();
	return bRes;
}

bool CWriteDeviceInfo::PostRun()
{
	return true;
}

bool CWriteDeviceInfo::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CWriteDeviceInfo' default Retry=3"));
		m_iRetry = 3;
	}

	m_bClearData = false;
	if (paramMap.find(std::string("ClaerData")) != paramMap.end())
	{
		if(paramMap[std::string("ClaerData")].compare("TRUE") == 0)		m_bClearData = true;
	}

	if (paramMap.find(std::string("WriteItems")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'WriteItems' for class 'CWriteDeviceInfo'"));
		return false;
	}
	std::vector<std::string> vWriteList;
	STDStringTokenizer(paramMap["WriteItems"], ",", vWriteList);
	if(vWriteList.size() != 6)
	{
		TraceLog(MSG_INFO, _T("Failed to get 'WriteItems' param for class 'CWriteDeviceInfo', size != 6"));
		return false;
	}
	m_bWriteIMEI = (atoi(vWriteList[0].c_str()) == 1) ?	true : false;
	m_bWriteBT = (atoi(vWriteList[1].c_str()) == 1) ? true : false;
	m_bWriteWIFI = (atoi(vWriteList[2].c_str()) == 1) ? true : false;
	m_bWriteMSN = (atoi(vWriteList[3].c_str()) == 1) ? true : false;
	m_bWriteFSN = (atoi(vWriteList[4].c_str()) == 1) ? true : false;
	m_bWriteSIMData = (atoi(vWriteList[5].c_str()) == 1) ? true : false;

	return true;
}

bool CWriteDeviceInfo::MainFunction()
{
	if(m_bWriteIMEI)
	{
		if( !WriteIMEI()) return false;
	}
	if(m_bWriteBT)
	{
		if( !WriteBT()) return false;
	}
	if(m_bWriteWIFI)
	{
		if( !WriteWIFI()) return false;
	}
	if(m_bWriteMSN)
	{
		if( !WriteMSN()) return false;
	}
	if(m_bWriteFSN)
	{
		if( !WriteFSN()) return false;
	}
	if(m_bWriteSIMData)
	{
		if( !WriteSIMData()) return false;
	}

	m_strMessage = "Write OTP success";
	m_strResult = "PASS";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return true;
}

bool CWriteDeviceInfo::WriteIMEI()
{
	char sz_inData[FTD_BUF_SIZE] = {0}; 
	char sz_outData[FTD_BUF_SIZE] = {0};

	if(m_bClearData)	sprintf(sz_inData, "IMEI,%s", Zero_IMEI);
	else				sprintf(sz_inData, "IMEI,%s", g_strIMEI.c_str());

	m_strMessage = "Write IMEI Start.";
	FactoryLog();
	if (!m_pIPhone->FTD_FAC_OTPWrite(m_nFtdPort, m_nFtdTimeOut, sz_inData, sz_outData))
	{
		m_strMessage = MakeStr("Write IMEI (%s) to phone fail.....", g_strIMEI.c_str());
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Write_IMEI_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		SetPICSData("IMEIUSED", "N");
		return false;
	}
	m_strMessage = MakeStr("Write IMEI to phone Success...%s", sz_inData);
	FactoryLog();
	SetPICSData("IMEIUSED", "Y");
	return true;
}

bool CWriteDeviceInfo::WriteBT()
{
	char sz_inData[FTD_BUF_SIZE] = {0}; 
	char sz_outData[FTD_BUF_SIZE] = {0};

	sprintf(sz_inData, "BT,%s", m_mapQueryData["BT_ADDR"].c_str());

	m_strMessage = "Write BT ADDR Start.";
	FactoryLog();
	if (!m_pIPhone->FTD_FAC_OTPWrite(m_nFtdPort, m_nFtdTimeOut, sz_inData, sz_outData))
	{
		m_strMessage = "Write BT ADDR to phone fail.....";
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Write_BT_Address_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	m_strMessage = MakeStr("Write BT ADDR to phone Success...%s", m_mapQueryData["BT_ADDR"].c_str());
	FactoryLog();
	return true;
}
bool CWriteDeviceInfo::WriteWIFI()
{
	char sz_inData[FTD_BUF_SIZE] = {0}; 
	char sz_outData[FTD_BUF_SIZE] = {0};

	sprintf(sz_inData, "Wifi,%s", m_mapQueryData["MAC_ADDR"].c_str());

	m_strMessage = "Write WIFI ADDR Start.";
	FactoryLog();
	if (!m_pIPhone->FTD_FAC_OTPWrite(m_nFtdPort, m_nFtdTimeOut, sz_inData, sz_outData))
	{
		m_strMessage = "Write WIFI ADDR to phone fail.....";
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Write_WLAN_Address_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	m_strMessage = MakeStr("Write WIFI ADDR to phone Success...%s", m_mapQueryData["MAC_ADDR"].c_str());
	FactoryLog();
	return true;
}
bool CWriteDeviceInfo::WriteMSN()
{
	char sz_inData[FTD_BUF_SIZE] = {0}; 
	char sz_outData[FTD_BUF_SIZE] = {0};

	sprintf(sz_inData, "1024,20,%s", m_mapQueryData["MSN"].c_str());
	m_strMessage = "Write Mobile Serial Number to Target start...";
	FactoryLog();
	if (!m_pIPhone->FTD_FAC_CFGWrite(m_nFtdPort, m_nFtdTimeOut, sz_inData, sz_outData))
	{
		m_strMessage = "Write Mobile Serial Number to Target Fail...";
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Write_Serial_Number_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	m_strMessage = MakeStr("Write Mobile Serial Number to Target Success...%s", sz_outData);
	FactoryLog();
	return true;
}
bool CWriteDeviceInfo::WriteFSN()
{
	char sz_inData[FTD_BUF_SIZE] = {0}; 
	char sz_outData[FTD_BUF_SIZE] = {0};

	sprintf(sz_inData, "SerialNo,%s", m_mapQueryData["FSN"].c_str());

	m_strMessage = "Write FSN Start.";
	FactoryLog();
	if (!m_pIPhone->FTD_FAC_OTPWrite(m_nFtdPort, m_nFtdTimeOut, sz_inData, sz_outData))
	{
		m_strResult = "FAIL";
		m_strMessage = "Write FSN to phone fail.....";
		m_strErrorCode = CommErr_Write_FSN_Fail;
		FactoryLog();
		return false;
	}
	m_strMessage = MakeStr("Write FSN to OTP Success... %s", m_mapQueryData["FSN"].c_str());
	FactoryLog();
	return true;
}
bool CWriteDeviceInfo::WriteSIMData()
{
	// 1. Write Public Key to OTP //
	char sz_inData[FTD_BUF_SIZE] = {0}; 
	char sz_outData[FTD_BUF_SIZE] = {0};
	char szSIMPublicKey[2048] = {0};
	char szSIMData[2048] = {0};
	strcpy(szSIMPublicKey, m_mapQueryData["SIMPublicKey"].c_str());
	strcpy(szSIMData, m_mapQueryData["SIMData"].c_str());

	sprintf(sz_inData, "Qlock,%s", szSIMPublicKey);

	m_strMessage = "Write Public Key Start.";
	FactoryLog();
	if (!m_pIPhone->FTD_FAC_OTPWrite(m_nFtdPort, 15000, sz_inData, sz_outData))
	{
		m_strResult = "FAIL";
		m_strMessage = "Write Public Key to phone fail.....";
		m_strErrorCode = CommErr_Write_Public_key_Fail;
		FactoryLog();
		return false;
	}
	m_strMessage = MakeStr("Write Public Key to OTP Success... %s", szSIMPublicKey);
	FactoryLog();

	m_pIPhone->Disconnect_QMSL();

	// 2. Write SIM Data to phone //
	HMODULE hQcomDiagAPI = ::LoadLibrary("QcomDiagModule.dll");
	char szCOMPort[8] = {0};
	strcpy(szCOMPort, m_pIPhone->m_str_comPort.c_str());
	PFWriteSimDataNew_SPU fpWriteSimData = (PFWriteSimDataNew_SPU)GetProcAddress(hQcomDiagAPI, "WriteSimDataNew_SPU");
	if (fpWriteSimData == NULL)
	{
		m_strErrorCode = CommErr_Load_DLL_Fail;
		m_strMessage = "Fail to GetProcAddress: WriteSimDataNew";
		FactoryLog();
		FreeLibrary(hQcomDiagAPI);
		hQcomDiagAPI = NULL;
		return false;
	}

	if( !fpWriteSimData(szCOMPort, szSIMData))
	{
		m_strResult = "FAIL";
		m_strMessage = "Write SIM data to phone fail...";
		m_strErrorCode = CommErr_Write_Sim_Data_Fail;
		FactoryLog();
		FreeLibrary(hQcomDiagAPI);
		hQcomDiagAPI = NULL;
		return false;
	}
	if (hQcomDiagAPI != NULL)
	{
		FreeLibrary(hQcomDiagAPI);
		hQcomDiagAPI = NULL;
	}

	m_pIPhone->Initial_QMSL();
	for (int i = 0; i < 10; ++i)
	{
		if (m_pIPhone->IsConnected_QMSL())
		{
			break;
		}
		Sleep(500);
	}

	m_strMessage = MakeStr("Write SIM Data Success... %s", szSIMData);
	FactoryLog();
	return true;
}