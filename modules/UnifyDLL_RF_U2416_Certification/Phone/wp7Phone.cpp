#include "stdafx.h"
#include "wp7Phone.h"
#include "Shlwapi.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(Cwp7Phone)


Cwp7Phone::Cwp7Phone()
	: m_hQcomDiagModule(NULL)
	, m_hQMSLPhone(NULL)
{
}

Cwp7Phone::~Cwp7Phone()
{
	if (m_hQcomDiagModule != NULL)
	{
		FreeLibrary(m_hQcomDiagModule);
		m_hQcomDiagModule = NULL;
	}

	if (m_hQMSLPhone != NULL)
	{
		QLIB_DisconnectServer(m_hQMSLPhone);
		m_hQMSLPhone = NULL;
	}
}

bool Cwp7Phone::InitData(std::map<std::string, std::string>& paramMap)
{
	// COM port
	if (paramMap.find(std::string("COMPort")) != paramMap.end())
	{
		m_strCOMport = paramMap[std::string("COMPort")];
		m_strDeviceName = m_strCOMport;
	}
	else
		return false;

	// Qisda command module
	if (paramMap.find(std::string("QisdaCmdModule")) != paramMap.end())
	{
		m_strQisdaCommandModuleName = paramMap[std::string("QisdaCmdModule")];
	}
	else
		return false;

	return true;
}

bool Cwp7Phone::MyExecAdbCmd(const char* chCmd, char **chReturn)
{
	bool ret = true;
	HANDLE hWrite, hRead;

	PROCESS_INFORMATION processInfo;
	memset(&processInfo, 0, sizeof(PROCESS_INFORMATION));

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	try
	{
		if (! CreatePipe(&hRead, &hWrite, &saAttr, 0))
			throw _T("CreatePipe");

		STARTUPINFO si = {0};
		si.cb = sizeof (STARTUPINFO);
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;
		si.hStdInput = hRead;
		si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		TCHAR szWorkingDir[MAX_PATH], szExePath[MAX_PATH];
		TCHAR szCmdLine[1024];

		// Set WorkPath and ExePath
		GetModuleFileName(NULL, szExePath, MAX_PATH);
		PathRemoveFileSpec(szExePath);
		_tcscpy(szWorkingDir, szExePath);

		SetCurrentDirectory(szWorkingDir);

		PathAppend(szExePath, "adb.exe");

		_stprintf(szCmdLine, _T("\"%s\" shell %s"), szExePath, chCmd);

		if (! CreateProcess(NULL,	// do not send parameter
			szCmdLine,				// need enclosing quotation
			NULL,
			NULL,
			TRUE,					// important!
			CREATE_DEFAULT_ERROR_MODE,
			NULL,
			szWorkingDir,			// child process's full path.
			&si,
			&processInfo))
		{
			throw _T("CreateProcess");
		}

		WaitForSingleObject(processInfo.hProcess, INFINITE);

		// if needs return value
		if (chReturn != NULL)
		{
			DWORD dwAvail, numread;

			BOOL bRet = PeekNamedPipe (hRead, NULL, 0, NULL, &dwAvail, 0);
			if (bRet && dwAvail > 0)
			{
				*chReturn = new char [dwAvail+1];
				char *pBuffer = *chReturn;
				memset(pBuffer, 0, dwAvail+1);

				::ReadFile(hRead, pBuffer, dwAvail, &numread, 0);
			}
		}
	}
	catch (const TCHAR* szError)
	{
		LPVOID lpMsgBuf;

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(),
			//MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		TCHAR szMessage[1024];
		_stprintf (szMessage, "%s: %s", szError, lpMsgBuf);

		LocalFree(lpMsgBuf);
		ret = false;
	}

	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(processInfo.hProcess);
	processInfo.hProcess = NULL;
	CloseHandle(processInfo.hThread);
	processInfo.hThread = NULL;

	return ret;
}


// --------------------- FA functions ----------------------

bool Cwp7Phone::GetFAData(char* szValue, int iSize)
{
	pFA_Read FA_Read = (pFA_Read)GetProcAddress(m_hQcomDiagModule, "FA_Read");

	char pBuffer[512];
	memset(pBuffer, 0, 512);
	char szErrCode[512];
	memset(szErrCode, 0, 512);
	char szErrMsg[512];
	memset(szErrMsg, 0, 512);

	if (FA_Read)
	{
		if (FA_Read(m_strCOMport.c_str(), pBuffer, szErrCode, szErrMsg))
		{
			if (memcpy_s(szValue, iSize, pBuffer, iSize) == 0)
				return true;
		}
	}

	return false;
}

bool Cwp7Phone::SetFAData(char* szValue)
{
	pFA_Write FA_Write = (pFA_Write)GetProcAddress(m_hQcomDiagModule, "FA_Write");

	char szErrCode[512];
	memset(szErrCode, 0, 512);
	char szErrMsg[512];
	memset(szErrMsg, 0, 512);

	if (FA_Write)
		return FA_Write(m_strCOMport.c_str(), szValue, szErrCode, szErrMsg);

	return false;
}

bool Cwp7Phone::GetFAData_QMSL(char* szValue, int iSize)
{
	unsigned char pBuffer1[128];
	unsigned char pBuffer2[128];
	unsigned char pBuffer3[128];
	unsigned char pBuffer4[128];

	unsigned short unStatus;

	if (m_hQMSLPhone != NULL)
	{
		if (! QLIB_DIAG_NV_READ_F(m_hQMSLPhone, 2497, pBuffer1, 128, &unStatus))
			return false;
		if (! QLIB_DIAG_NV_READ_F(m_hQMSLPhone, 2498, pBuffer2, 128, &unStatus))
			return false;
		if (! QLIB_DIAG_NV_READ_F(m_hQMSLPhone, 2499, pBuffer3, 128, &unStatus))
			return false;
		if (! QLIB_DIAG_NV_READ_F(m_hQMSLPhone, 2500, pBuffer4, 128, &unStatus))
			return false;

		memcpy(szValue, pBuffer1, 128);
		memcpy(szValue + 128, pBuffer2, 128);
		memcpy(szValue + 256, pBuffer3, 128);
		memcpy(szValue + 384, pBuffer4, 128);
		return true;
	}

	return false;
}

bool Cwp7Phone::SetFAData_QMSL(char* szValue)
{
	unsigned short unStatus;

	if (m_hQMSLPhone != NULL)
	{
		if (! QLIB_DIAG_NV_WRITE_F(m_hQMSLPhone, 2497, (unsigned char*)szValue, 128, &unStatus))
			return false;
		if (! QLIB_DIAG_NV_WRITE_F(m_hQMSLPhone, 2498, (unsigned char*)(szValue + 128), 128, &unStatus))
			return false;
		if (! QLIB_DIAG_NV_WRITE_F(m_hQMSLPhone, 2499, (unsigned char*)(szValue + 256), 128, &unStatus))
			return false;
		if (! QLIB_DIAG_NV_WRITE_F(m_hQMSLPhone, 2500, (unsigned char*)(szValue + 384), 128, &unStatus))
			return false;

		return true;
	}

	return false;
}


// -------------------- Phone functions --------------------

bool Cwp7Phone::Initial()
{
	// Load library
	if (NULL == m_hQcomDiagModule)
	{
		m_hQcomDiagModule = LoadLibrary(m_strQisdaCommandModuleName.c_str());
		if (NULL == m_hQcomDiagModule)
			return false;
	}

	return true;
}

bool Cwp7Phone::Disconnect()
{
	if (m_hQcomDiagModule)
	{
		FreeLibrary(m_hQcomDiagModule);
		m_hQcomDiagModule = NULL;
	}

	return true;
}

bool Cwp7Phone::IsConnected()
{
	pNV_IsConnect_SPU Is_Connected = (pNV_IsConnect_SPU)GetProcAddress(m_hQcomDiagModule, "NV_IsConnect_SPU");
	if (NULL == Is_Connected)
		return false;

	return Is_Connected(m_strCOMport.c_str());
}

bool Cwp7Phone::Reboot()
{
	pNV_ResetHandset_SPU ResetHandset = (pNV_ResetHandset_SPU)GetProcAddress(m_hQcomDiagModule, "NV_ResetHandset_SPU");
	if (! ResetHandset)
		return false;

	return ResetHandset(m_strCOMport.c_str());
}

bool Cwp7Phone::ReadNVItem(WORD NVItem, BYTE* pBuffer, const int BufferSize)
{
	pNV_ReadItem NV_ReadItem = (pNV_ReadItem)GetProcAddress(m_hQcomDiagModule, "NV_ReadItem");
	if (! NV_ReadItem)
		return false;

	WORD nvstatus;

	return NV_ReadItem(atoi(m_strCOMport.c_str()), NVItem, pBuffer, BufferSize, &nvstatus);
}

bool Cwp7Phone::WriteNVItem(WORD NVItem, BYTE* pBuffer, const int BufferSize)
{
	pNV_WriteItem NV_WriteItem = (pNV_WriteItem)GetProcAddress(m_hQcomDiagModule, "NV_WriteItem");
	if (! NV_WriteItem)
		return false;

	WORD nvstatus;

	return NV_WriteItem(atoi(m_strCOMport.c_str()), NVItem, pBuffer, BufferSize, &nvstatus);
}

bool Cwp7Phone::BackupNVpartition()
{
	pNV_BackupPartition NVbackup = (pNV_BackupPartition)GetProcAddress(m_hQcomDiagModule, "NV_BackupPartition");
	if (! NVbackup)
		return false;

	return NVbackup(m_strCOMport.c_str());
}


bool Cwp7Phone::ReadHWVersion(char* szVarientVer, char* szMemoryVer, char* szEvtVer, char* szEvtMinorVer, char* szPilotRunVer)
{
	pWP7_ReadHWversion_SPU WP7_ReadHWVersion = (pWP7_ReadHWversion_SPU)GetProcAddress(m_hQcomDiagModule, "WP7_ReadHWversion_SPU");
	if (! WP7_ReadHWVersion)
		return false;

	return WP7_ReadHWVersion(m_strCOMport.c_str(), szVarientVer, szMemoryVer, szEvtVer, szEvtMinorVer, szPilotRunVer);
}

bool Cwp7Phone::ReadSWVersion(char* szVersion)
{
	pWP7_ReadSWversion_SPU WP7_ReadSWVersion = (pWP7_ReadSWversion_SPU)GetProcAddress(m_hQcomDiagModule, "WP7_ReadSWversion_SPU");
	if (! WP7_ReadSWVersion)
		return false;

	return WP7_ReadSWVersion(m_strCOMport.c_str(), szVersion);
}

bool Cwp7Phone::GetComponentValue(const int ID, const int SubID, char* szValue)
{
	pSensor_GetComponentValue_SPU Sensor_GetComponentValue = (pSensor_GetComponentValue_SPU)GetProcAddress(m_hQcomDiagModule, "Sensor_GetComponentValue_SPU");
	if (! Sensor_GetComponentValue)
		return false;

	char szID[32], szSubID[32];
	sprintf_s(szID, 31, "%d", ID);
	sprintf_s(szSubID, 31, "%d", SubID);

	return Sensor_GetComponentValue(m_strCOMport.c_str(), szID, szSubID, szValue);
}

bool Cwp7Phone::GsensorTest()
{
	pSensor_CheckGsensor_SPU CheckGSensor = (pSensor_CheckGsensor_SPU)GetProcAddress(m_hQcomDiagModule, "Sensor_CheckGSensor_SPU");
	if (! CheckGSensor)
		return false;

	return CheckGSensor(m_strCOMport.c_str());
}

bool Cwp7Phone::ECompassTest()
{
	pSensor_CheckECompass_SPU CheckECompass = (pSensor_CheckECompass_SPU)GetProcAddress(m_hQcomDiagModule, "Sensor_CheckECompass_SPU");
	if (! CheckECompass)
		return false;

	return CheckECompass(m_strCOMport.c_str());
}

bool Cwp7Phone::GetSDStatus(int* pStatus)
{
	pSD_GetStatus_SPU ReadSDCardStatus = (pSD_GetStatus_SPU)GetProcAddress(m_hQcomDiagModule, "SD_GetStatus_SPU");
	if (! ReadSDCardStatus)
		return false;

	char szStatus[512];
	bool ret = ReadSDCardStatus(m_strCOMport.c_str(), szStatus);
	*pStatus = atoi(szStatus);

	return ret;
}

bool Cwp7Phone::GetSDSize(int* pSize)
{
	pSD_GetSize_SPU ReadSDCardSize = (pSD_GetSize_SPU)GetProcAddress(m_hQcomDiagModule, "SD_GetSize_SPU");
	if (! ReadSDCardSize)
		return false;

	char szSize[512];
	bool ret = ReadSDCardSize(m_strCOMport.c_str(), szSize);
	*pSize = atoi(szSize);

	return ret;
}

// --------------------------- BT --------------------------

bool Cwp7Phone::BTModuleOnOffSwitch(bool bOn)
{
	if (bOn)
	{
		pBT_ModuleOn_SPU BT_ModuleOn_SPU = (pBT_ModuleOn_SPU)GetProcAddress(m_hQcomDiagModule, "BT_ModuleOn_SPU");
		if (! BT_ModuleOn_SPU)
			return false;

		return BT_ModuleOn_SPU(m_strCOMport.c_str());
	}
	else
	{
		pBT_ModuleOff_SPU BT_ModuleOff_SPU = (pBT_ModuleOff_SPU)GetProcAddress(m_hQcomDiagModule, "BT_ModuleOff_SPU");
		if (! BT_ModuleOff_SPU)
			return false;

		return BT_ModuleOff_SPU(m_strCOMport.c_str());
	}
}

bool Cwp7Phone::BTPowerOff()
{
	pBT_PowerOff_SPU BTStopPower = (pBT_PowerOff_SPU)GetProcAddress(m_hQcomDiagModule, "BT_PowerOff_SPU");
	if (! BTStopPower)
		return false;

	return BTStopPower(m_strCOMport.c_str());
}

bool Cwp7Phone::BTStartTxPower(int degree)
{
	pBT_StartPower_SPU BTStartPower = (pBT_StartPower_SPU)GetProcAddress(m_hQcomDiagModule, "BT_StartPower_SPU");
	if (! BTStartPower)
		return false;

	char szDegree[64];
	wsprintf (szDegree, "%d", degree);
	return BTStartPower(m_strCOMport.c_str(), szDegree);
}


// ------------------------- WiFi --------------------------

bool Cwp7Phone::WiFiModuleOnOffSwitch(bool bOn)
{
	if (bOn)
	{
		pWiFi_ModuleSwitchOn_SPU WiFi_ModuleSwitchOn_SPU =
			(pWiFi_ModuleSwitchOn_SPU)GetProcAddress(m_hQcomDiagModule, "WiFi_ModuleSwitchOn_SPU");
		if (! WiFi_ModuleSwitchOn_SPU)
			return false;

		return WiFi_ModuleSwitchOn_SPU(m_strCOMport.c_str());
	}
	else
	{
		pWiFi_ModuleSwitchOff_SPU WiFi_ModuleSwitchOff_SPU =
			(pWiFi_ModuleSwitchOff_SPU)GetProcAddress(m_hQcomDiagModule, "WiFi_ModuleSwitchOff_SPU");
		if (! WiFi_ModuleSwitchOff_SPU)
			return false;

		return WiFi_ModuleSwitchOff_SPU(m_strCOMport.c_str());
	}
}

bool Cwp7Phone::WiFiStartTxPower(int iRate, int iChannel, int iDesirePower)
{
	pWiFi_TurnOnTXPower2_SPU WiFi_TurnOnTXPower2 =
		(pWiFi_TurnOnTXPower2_SPU)GetProcAddress(m_hQcomDiagModule, "WiFi_TurnOnTXPower2_SPU");
	if (! WiFi_TurnOnTXPower2)
		return false;

	char szRate[64];
	memset(szRate, 0, 64);
	sprintf_s(szRate, 63, "%d", iRate);

	char szChannel[64];
	memset(szChannel, 0, 64);
	sprintf_s(szChannel, 63, "%d", iChannel);

	char szPower[64];
	memset(szPower, 0, 64);
	sprintf_s(szPower, 63, "%d", iDesirePower);

	return WiFi_TurnOnTXPower2(m_strCOMport.c_str(), szRate, szChannel, szPower);
}


// -------------------------- GPS --------------------------

bool Cwp7Phone::GPS_FunctionOn()
{
	pGPS_TrackingSwitch_SPU GPS_TrackingSwitch_SPU =
		(pGPS_TrackingSwitch_SPU)GetProcAddress(m_hQcomDiagModule, "GPS_TrackingSwitch_SPU");
	if (! GPS_TrackingSwitch_SPU)
		return false;

	char szOnOff[256];
	memset(szOnOff, 0, 256);
	sprintf_s(szOnOff, 255, "1");

	return GPS_TrackingSwitch_SPU(m_strCOMport.c_str(), szOnOff);
}

bool Cwp7Phone::GPS_FunctionOff()
{
	pGPS_TrackingSwitch_SPU GPS_TrackingSwitch_SPU =
		(pGPS_TrackingSwitch_SPU)GetProcAddress(m_hQcomDiagModule, "GPS_TrackingSwitch_SPU");
	if (! GPS_TrackingSwitch_SPU)
		return false;

	char szOnOff[256];
	memset(szOnOff, 0, 256);
	sprintf_s(szOnOff, 255, "0");

	return GPS_TrackingSwitch_SPU(m_strCOMport.c_str(), szOnOff);
}

bool Cwp7Phone::GPS_GetCN0value(int& iCN0)
{
	iCN0 = 0;

	pGPS_GetCNOvalue_SPU GPS_GetCNOvalue_SPU =
		(pGPS_GetCNOvalue_SPU)GetProcAddress(m_hQcomDiagModule, "GPS_GetCNOvalue_SPU");
	if (! GPS_GetCNOvalue_SPU)
		return false;

	char szCN0[256];
	memset(szCN0, 0, 256);

	if (! GPS_GetCNOvalue_SPU(m_strCOMport.c_str(), szCN0))
		return false;
	else
	{
		iCN0 = atoi(szCN0);
		return true;
	}
}

bool Cwp7Phone::GPS_GetTTFFCN0(int& iTTFF, int& iCN0)
{
	iTTFF = 0;
	iCN0 = 0;

	pGPS_PollingTTFFCN0_SPU GPS_PollingTTFFCN0_SPU =
		(pGPS_PollingTTFFCN0_SPU)GetProcAddress(m_hQcomDiagModule, "GPS_PollingTTFFCN0_SPU");
	if (! GPS_PollingTTFFCN0_SPU)
		return false;

	char szTTFF[256];
	memset(szTTFF, 0, 256);
	char szCN0[256];
	memset(szCN0, 0, 256);

	if (! GPS_PollingTTFFCN0_SPU(m_strCOMport.c_str(), szTTFF, szCN0))
		return false;
	else
	{
		iTTFF = atoi(szTTFF);
		iCN0 = atoi(szCN0);
		return true;
	}
}


// ------------------------- Power -------------------------

bool Cwp7Phone::GetGaugeName(char* szName)
{
	pPW_GetGaugeDeviceName_SPU PW_GetGaugeDeviceName_SPU =
		(pPW_GetGaugeDeviceName_SPU)GetProcAddress(m_hQcomDiagModule, "PW_GetGaugeDeviceName_SPU");
	if (! PW_GetGaugeDeviceName_SPU)
		return false;

	return PW_GetGaugeDeviceName_SPU(m_strCOMport.c_str(), szName);
}

bool Cwp7Phone::ChargeICTest()
{
	pPW_GetChargeICDeviceID_SPU PW_GetChargeICDeviceID_SPU =
		(pPW_GetChargeICDeviceID_SPU)GetProcAddress(m_hQcomDiagModule, "PW_GetChargeICDeviceID_SPU");
	if (! PW_GetChargeICDeviceID_SPU)
		return false;

	char szDeviceID[512];
	memset(szDeviceID, 0, 512);

	if (PW_GetChargeICDeviceID_SPU(m_strCOMport.c_str(), szDeviceID))
	{
		if (strcmp(szDeviceID, "0A") == 0)
			return true;
	}

	return false;
}

bool Cwp7Phone::USBIsCharging()
{
	pPW_USBChargingStatus_SPU USBChargeStatus = (pPW_USBChargingStatus_SPU)GetProcAddress(m_hQcomDiagModule, "PW_USBChargingStatus_SPU");
	if (! USBChargeStatus)
		return false;

	char szStatus[512];
	if (! USBChargeStatus(m_strCOMport.c_str(), szStatus))
		return false;

	return _stricmp(szStatus, "Charging") == 0;
}

bool Cwp7Phone::USBChargeSwitch(bool bEnable)
{
	if (bEnable)
	{
		pPW_USBChargeStart_SPU USBChargeStart = (pPW_USBChargeStart_SPU)GetProcAddress(m_hQcomDiagModule, "PW_USBChargeStart_SPU");
		if  (! USBChargeStart)
			return false;

		return USBChargeStart (m_strCOMport.c_str());
	}
	else
	{
		pPW_USBChargeStop_SPU USBChargeStop = (pPW_USBChargeStop_SPU)GetProcAddress(m_hQcomDiagModule, "PW_USBChargeStop_SPU");
		if  (! USBChargeStop)
			return false;

		return USBChargeStop (m_strCOMport.c_str());
	}

	return false;
}

bool Cwp7Phone::ReadBatteryVoltage(int* piVoltage)
{
	pPW_ReadBatteryVoltage_SPU PW_ReadBatteryVoltage_SPU = (pPW_ReadBatteryVoltage_SPU)GetProcAddress(m_hQcomDiagModule, "PW_ReadBatteryVoltage_SPU");
	if (! PW_ReadBatteryVoltage_SPU)
		return false;

	char szVol[512];
	memset(szVol, 0, 512);

	if (! PW_ReadBatteryVoltage_SPU(m_strCOMport.c_str(), szVol))
		return false;

	*piVoltage = atoi(szVol);
	return true;
}

bool Cwp7Phone::GetCoinCellVoltage(int* piVoltage)
{
	pPW_GetCoinCellVoltage_SPU PW_GetCoinCellVoltage_SPU = (pPW_GetCoinCellVoltage_SPU)GetProcAddress(m_hQcomDiagModule, "PW_GetCoinCellVoltage_SPU");
	if (! PW_GetCoinCellVoltage_SPU)
		return false;

	char szVol[512];
	memset(szVol, 0, 512);

	if (! PW_GetCoinCellVoltage_SPU(m_strCOMport.c_str(), szVol))
		return false;

	*piVoltage = atoi(szVol);
	return true;
}


// -------------------------- NFC --------------------------

bool Cwp7Phone::NFC_IsConnected()
{
	pNFC_IsConnected_SPU NFC_IsConnected_SPU = (pNFC_IsConnected_SPU)GetProcAddress(m_hQcomDiagModule, "NFC_IsConnected_SPU");
	if (! NFC_IsConnected_SPU)
		return false;

	return NFC_IsConnected_SPU(m_strCOMport.c_str());
}


// ------------------------- NLED --------------------------

bool Cwp7Phone::NLED_IsConnected()
{
	pNLED_IsConnected_SPU NLED_IsConnected_SPU = (pNLED_IsConnected_SPU)GetProcAddress(m_hQcomDiagModule, "NLED_IsConnected_SPU");
	if (! NLED_IsConnected_SPU)
		return false;

	return NLED_IsConnected_SPU(m_strCOMport.c_str());
}


// ------------------------ Speaker ------------------------

bool Cwp7Phone::Audio_SpeakerAmpTest()
{
	pAudio_SpeakerAmpTest_SPU Audio_SpeakerAmpTest_SPU = (pAudio_SpeakerAmpTest_SPU)GetProcAddress(m_hQcomDiagModule, "Audio_SpeakerAmpTest_SPU");
	if (! Audio_SpeakerAmpTest_SPU)
		return false;

	return Audio_SpeakerAmpTest_SPU(m_strCOMport.c_str());
}


// -------------------------- EFs --------------------------

bool Cwp7Phone::EFS_Sync_Combo(int iRetryTime)
{
	pEFS_Sync_Combo_SPU EFS_Sync_Combo_SPU = (pEFS_Sync_Combo_SPU)GetProcAddress(m_hQcomDiagModule, "EFS_Sync_Combo_SPU");
	if (! EFS_Sync_Combo_SPU)
		return false;

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%d", iRetryTime);

	return EFS_Sync_Combo_SPU(const_cast<char*>(m_strCOMport.c_str()), szTmp);
}

bool Cwp7Phone::EFs_GoldenCopy()
{
	pEFS_GoldenCopy_SPU EFS_GoldenCopy_SPU = (pEFS_GoldenCopy_SPU)GetProcAddress(m_hQcomDiagModule, "EFs_GoldenCopy_SPU");
	if (! EFS_GoldenCopy_SPU)
		return false;

	return EFS_GoldenCopy_SPU(const_cast<char*>(m_strCOMport.c_str()));
}

// ------------------------- FM --------------------------

bool Cwp7Phone::FM_TuneFM(int iChannel)
{
	pFM_TuneFM_SPU FM_TuneFM_SPU = (pFM_TuneFM_SPU)GetProcAddress(m_hQcomDiagModule, "FM_TuneFM_SPU");
	if (! FM_TuneFM_SPU)
		return false;

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%d", iChannel);

	return FM_TuneFM_SPU(const_cast<char*>(m_strCOMport.c_str()), szTmp);
}

bool Cwp7Phone::FM_ConnectFM(bool bConnect,int iTimeout)
{
	pFM_ConnectFM_SPU FM_ConnectFM_SPU = (pFM_ConnectFM_SPU)GetProcAddress(m_hQcomDiagModule, "FM_ConnectFM_SPU");
	if (! FM_ConnectFM_SPU)
		return false;

	char szTmp[256];
	char *szConnect;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%d", iTimeout);

	szConnect=bConnect?_T("true"):_T("false");

	return FM_ConnectFM_SPU(const_cast<char*>(m_strCOMport.c_str()), szConnect, szTmp);
}

bool Cwp7Phone::FM_GetRSSI(int &iRSSI)
{
	pFM_GetRSSI_SPU FM_GetRSSI_SPU = (pFM_GetRSSI_SPU)GetProcAddress(m_hQcomDiagModule, "FM_GetRSSI_SPU");
	if (! FM_GetRSSI_SPU)
		return false;

	char szRSSI[256];

	bool bRet=FM_GetRSSI_SPU(const_cast<char*>(m_strCOMport.c_str()), szRSSI);
	if(!bRet)
		return false;
	else
	{
		sscanf_s(szRSSI, "%d", &iRSSI);
		return true;
	}

}



//////////////////////////////////////////////////////////////////////////
// ----------------- QMSL phone functions ------------------
//////////////////////////////////////////////////////////////////////////

bool Cwp7Phone::Initial_QMSL()
{
	if (m_hQMSLPhone)
	{
		if (QLIB_IsPhoneConnected(m_hQMSLPhone))
		{
			QLIB_FlushRxBuffer(m_hQMSLPhone);
			return true;
		}
	}
	else
	{
		QLIB_SetLibraryMode(false);

		m_hQMSLPhone = QLIB_ConnectServerWithWait((unsigned)atoi(m_strCOMport.c_str()), 5000);
        //m_hQMSLPhone = QLIB_ConnectServerWithHandShake((unsigned)atoi(m_strCOMport.c_str()));
          
		if (m_hQMSLPhone != NULL)
		{
			if (QLIB_IsPhoneConnected(m_hQMSLPhone))
			{
				QLIB_FlushRxBuffer(m_hQMSLPhone);
				return true;
			}
		}
	}

	return false;
}

bool Cwp7Phone::IsConnected_QMSL()
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_IsPhoneConnected(m_hQMSLPhone))
			return true;
	}

	return Initial_QMSL();
}

bool Cwp7Phone::Disconnect_QMSL()
{
	if (m_hQMSLPhone != NULL)
	{
		QLIB_DisconnectServer(m_hQMSLPhone);
		m_hQMSLPhone = NULL;
	}

	return true;
}

bool Cwp7Phone::QMSL_IsFTM_Mode(unsigned char* pMode)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_IsFTM_Mode(m_hQMSLPhone, pMode))
			return true;
	}

	return false;
}

bool Cwp7Phone::ReadNV_QMSL(unsigned short unIndex, unsigned char* pData, int iSize)
{
	if (m_hQMSLPhone != NULL)
	{
		unsigned short unStatus = 0;
		if (QLIB_DIAG_NV_READ_F(m_hQMSLPhone, unIndex, pData, iSize, &unStatus))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_MODE_ID(short nMode)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_MODE_ID(m_hQMSLPhone, nMode))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_PATH_DELAY(short *piDelay)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_PATH_DELAY(m_hQMSLPhone, piDelay))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_GSM_POLAR_TX_SWEEP_CAL(unsigned long iNumDAC, unsigned long* piDACList, unsigned long* piDACDuration)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_GSM_POLAR_TX_SWEEP_CAL(m_hQMSLPhone, iNumDAC, piDACList, piDACDuration))
			return true;
	}

	return false;
}


bool Cwp7Phone::QMSL_FTM_SET_MODE(short nMode)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_MODE(m_hQMSLPhone, nMode))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_CHAN(unsigned short unChannel)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_CHAN(m_hQMSLPhone, unChannel))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_GSM_LINEAR_PA_RANGE(unsigned short unSlot, unsigned short unPARange)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_GSM_LINEAR_PA_RANGE(m_hQMSLPhone, unSlot, unPARange))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_DIAG_CONTROL_F(int iMode)
{
	if(m_hQMSLPhone != NULL)
	{
		if (QLIB_DIAG_CONTROL_F(m_hQMSLPhone,iMode))
			return true;			
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_GSM_SET_LINEAR_RGI(unsigned short unSlot, unsigned short unRGI, unsigned char ucModType)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_GSM_SET_LINEAR_RGI(m_hQMSLPhone, unSlot, unRGI, ucModType))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_TRANSMIT_BURST(
	unsigned char ucSlot,
	short nDataSource,
	unsigned char ucTSCIndex,
	unsigned long ulNumBursts,
	unsigned char ucIsInfiniteDuration
	)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_TRANSMIT_BURST(m_hQMSLPhone, ucSlot, nDataSource, ucTSCIndex, ulNumBursts, ucIsInfiniteDuration))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_TX_FRAME_MATRIX(
	unsigned char iSlotNum, 
	unsigned char iTurnSlotOn, 
	unsigned short iPwrLevel, 
	unsigned char iModScheme
	)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_TX_FRAME_MATRIX(m_hQMSLPhone, iSlotNum, iTurnSlotOn, iPwrLevel, iModScheme))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_TX_ON()
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_TX_ON(m_hQMSLPhone))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_TX_OFF()
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_TX_OFF(m_hQMSLPhone))
			return true;
	}

	return false;
}


bool Cwp7Phone::QMSL_FTM_SET_PA_RANGE(unsigned short unRange)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_PA_RANGE(m_hQMSLPhone, unRange))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_PDM(unsigned short unPDM)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_PDM(m_hQMSLPhone, 2, unPDM))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_PDM_V2(unsigned short iPDMtype, unsigned short unPDM)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_PDM(m_hQMSLPhone, iPDMtype, unPDM))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_PA_DAC_INPUT(unsigned short iDACinput)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_PA_DAC_INPUT(m_hQMSLPhone, iDACinput))
			return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_TX_CS_SWEEP(unsigned char iListSize, short* aiI_DC_Correction, short* aiQ_DC_Correction, 
									 unsigned short* aiSegmentLength, unsigned short* piStatus)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_TX_CS_SWEEP(m_hQMSLPhone, iListSize, aiI_DC_Correction, aiQ_DC_Correction, aiSegmentLength, piStatus))
			return true;
	}

	return false;
}


bool Cwp7Phone::QMSL_ConfigureTimeOut(unsigned long unTimoutID, unsigned long unTimeout)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_ConfigureTimeOut(m_hQMSLPhone, unTimoutID, unTimeout) )
            return true ;
    }
    return false ;
}

bool Cwp7Phone::QMSL_FTM_SET_COMMAND_CODE(short nCmdCode)
{
    if ( 59!=nCmdCode && 75!=nCmdCode )
        return false ;

    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_SET_COMMAND_CODE(m_hQMSLPhone, nCmdCode) )
            return true ;
    }
    return false ;
}

bool Cwp7Phone::QMSL_FTM_SET_SECONDARY_CHAIN(unsigned short unMode)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_SET_SECONDARY_CHAIN(m_hQMSLPhone, unMode) )
            return true ;
    }
    return false ;
}

bool Cwp7Phone::QMSL_FTM_SET_LNA_RANGE(unsigned char iLNArange)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_SET_LNA_RANGE(m_hQMSLPhone, iLNArange) )
            return true ;
    }
    return false ;
}

bool Cwp7Phone::QMSL_FTM_DO_ENH_XO_DC_CAL(void* pFTM_ENH_XO_Cal_Request, void* pFTM_XO_Cal_Response)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_DO_ENH_XO_DC_CAL(m_hQMSLPhone, pFTM_ENH_XO_Cal_Request, pFTM_XO_Cal_Response))
            return true ;
    }
    return false ;
}

bool Cwp7Phone::QMSL_FTM_DO_ENH_XO_FT_COLLECT_SAMPLE(void* pFTM_ENH_XO_Cal_Request, void* pFTM_XO_Cal_Response)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_DO_ENH_XO_FT_COLLECT_SAMPLE(m_hQMSLPhone, pFTM_ENH_XO_Cal_Request, pFTM_XO_Cal_Response) )
            return true ;
    }
    return false ;
}

bool Cwp7Phone::QMSL_FTM_CDMA_CW_WAVEFORM(unsigned char bSelectCW)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_CDMA_CW_WAVEFORM(m_hQMSLPhone, bSelectCW) )
            return true ;
    }
    return false ;
}

bool Cwp7Phone::QMSL_FTM_DO_ENH_XO_FT_CURVE_CAL(void* pFTM_ENH_XO_Cal_Request, void* pFTM_XO_Cal_Response)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_DO_ENH_XO_FT_CURVE_CAL(m_hQMSLPhone, pFTM_ENH_XO_Cal_Request, pFTM_XO_Cal_Response) )
            return true ;
    }
    return false ;
}

bool Cwp7Phone::QMSL_FTM_DA_CAL(unsigned short iListSize, 
					   unsigned short *aiSegLength, 
					   unsigned char *aiRgi,	
					   unsigned char *aiGsmMode, 
					   unsigned char *aiPaRange)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_DA_CAL(m_hQMSLPhone, iListSize, aiSegLength, aiRgi, aiGsmMode, aiPaRange) )
			return true;
	}

	return false;

}


bool Cwp7Phone::QMSL_RFCAL_GSM_Tx_DA_Cal_Results(unsigned short iNvModeId,
										void *aGSM_DA_Cal,
										void *aGSM_DA_Cal_NV,
										unsigned char bCachedByNVManager)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_RFCAL_GSM_Tx_DA_Cal_Results(m_hQMSLPhone, iNvModeId, aGSM_DA_Cal, aGSM_DA_Cal_NV, bCachedByNVManager) )
			return true;
	}

	return false;

}

bool Cwp7Phone::QMSL_FTM_TX_CFG2_AMAM_SWEEP(unsigned long iPreDcDuration,
								   unsigned long iEdgeDuration,
								   unsigned long iCalRgi,
								   unsigned long *piDigScale)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_FTM_TX_CFG2_AMAM_SWEEP(m_hQMSLPhone, iPreDcDuration, iEdgeDuration, iCalRgi, piDigScale) ){
		//if( QLIB_FTM_TX_CFG2_AMAM_SWEEP_V2(m_hResourceContext, iPreDcDuration, iEdgeDuration, iCalRgi, 0, piDigScale)){
			return true;
		}
	}

	return false;
}

bool Cwp7Phone::QMSL_RFCAL_GSM_Tx_Linear_Process_PreDist_Cal_Results(unsigned short iNvModeId, 
															void *aGSM_PreDist_Cal,
															void *aGSM_PreDist_Cal_NV,
															unsigned char bCachedByNVManager)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_RFCAL_GSM_Tx_Linear_Process_PreDist_Cal_Results(m_hQMSLPhone, iNvModeId, aGSM_PreDist_Cal, aGSM_PreDist_Cal_NV, bCachedByNVManager) ){
			return true;
		}
	}

	return false;

}

bool Cwp7Phone::QMSL_RFCAL_NV_Manager_WriteToPhone(void *pWriteToPhoneStatus)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_RFCAL_NV_Manager_WriteToPhone(m_hQMSLPhone, pWriteToPhoneStatus) ){
			return true;
		}
	}

	return false;

}

bool Cwp7Phone::QMSL_FTM_GSM_DO_TX_Kvco(unsigned short iLowChan,unsigned short iHiChan,
							   unsigned short *piLowChanKvco,unsigned short *piHiChanKvco)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_FTM_GSM_DO_TX_KV_CAL(m_hQMSLPhone, iLowChan, iHiChan, piLowChanKvco, piHiChanKvco) ){
			return true;
		}
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_RX_BURST(unsigned char iSlotNum, short iRxFunction)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_FTM_SET_RX_BURST(m_hQMSLPhone,  iSlotNum, iRxFunction) ){
			return true;
		}
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_RX_GAIN_RANGE_CAL(unsigned char iRxRangeBitMask, unsigned char iNumAverages, unsigned char bToggleRx, 
										   unsigned long aiRSSI_Result[8])
{

	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_FTM_RX_GAIN_RANGE_CAL(m_hQMSLPhone, iRxRangeBitMask, iNumAverages, bToggleRx, aiRSSI_Result) ){
			return true;
		}
	}

	return false;

} 

bool Cwp7Phone::QMSL_RFCAL_GSM_Rx_Gain_Range_Results(unsigned short iNvModeId, void* aGSM_Rx_Gain_Range_Result, void* aGSM_Rx_Gain_Range_NV, 
													 unsigned char bCachedByNVManager)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_RFCAL_GSM_Rx_Gain_Range_Results(m_hQMSLPhone, iNvModeId, 
																aGSM_Rx_Gain_Range_Result, 
																aGSM_Rx_Gain_Range_NV, 
																bCachedByNVManager)){
			return true;
		}
	}

	return false;

}

bool Cwp7Phone::QMSL_RFCAL_GSM_CAL_ARFCN(unsigned short iNvModeId, 
								unsigned short* aGSM_Cal_ARFCN_List, 
								unsigned char iNumOfChannel, 
								void * pGSM_Cal_ARFCN_NV, 
								unsigned char bCachedByNVManager)

{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_RFCAL_GSM_CAL_ARFCN(m_hQMSLPhone, iNvModeId, 
												aGSM_Cal_ARFCN_List, 
												iNumOfChannel, 
												pGSM_Cal_ARFCN_NV, 
												bCachedByNVManager)){
			return true;
		}
	}

	return false;

}

bool Cwp7Phone::QMSL_FTM_LOAD_RF_NV(unsigned short* pusFTM_Error_Code)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_FTM_LOAD_RF_NV(m_hQMSLPhone, pusFTM_Error_Code) ){
			return true;
		}
	}

	return false;

}

bool Cwp7Phone::QMSL_DIAG_NV_WRITE_F(unsigned short iItemID, unsigned char* szValue, int iLength, unsigned short* iStatus)
{
	if (m_hQMSLPhone != NULL)
	{
		if (! QLIB_DIAG_NV_WRITE_F(m_hQMSLPhone, iItemID, szValue, iLength, iStatus))
			return false;

		return true;
	}

	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_TX_ENH_CAL_ACTION(int iState)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_SET_TX_ENH_CAL_ACTION(m_hQMSLPhone,iState))
			return true;
	}
	return false;
}

bool Cwp7Phone::QMSL_FTM_LOAD_TX_LINEARIZER_CAL_DATA(void* pFTM_Tx_Lin_CalData_Req, void* pFTM_Tx_Lin_CalData_Res)
{
    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_LOAD_TX_LINEARIZER_CAL_DATA(m_hQMSLPhone, pFTM_Tx_Lin_CalData_Req, pFTM_Tx_Lin_CalData_Res))
            return true;
    }
    return false;
}

bool Cwp7Phone::QMSL_FTM_LOAD_TX_FREQCOMP_CAL_DATA(void* pFTM_Tx_Freqcomp_CalData_Req, void* pFTM_Tx_Freqcomp_CalData_Res)
{
    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_LOAD_TX_FREQCOMP_CAL_DATA(m_hQMSLPhone, pFTM_Tx_Freqcomp_CalData_Req, pFTM_Tx_Freqcomp_CalData_Res))
            return true;
    }
    return false;
}

bool Cwp7Phone::QMSL_FTM_SET_SMPS_PA_BIAS_OVERRIDE(bool bOnOff)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_SET_SMPS_PA_BIAS_OVERRIDE(m_hQMSLPhone, bOnOff))
			return true;
	}
	return false;
}


bool Cwp7Phone::QMSL_FTM_SET_SMPS_PA_BIAS_VAL(int iPA_Bias_Value)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_SET_SMPS_PA_BIAS_VAL(m_hQMSLPhone, iPA_Bias_Value))
			return true;
	}
	return false;
}


bool Cwp7Phone::QMSL_FTM_SET_CALIBRATION_STATE(int iState)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_SET_CALIBRATION_STATE(m_hQMSLPhone,iState))
			return true;
	}
	return false;
}

bool Cwp7Phone::QMSL_FTM_SET_TX_SWEEP_STEP_SIZE(unsigned char ucStepSize)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_SET_TX_SWEEP_STEP_SIZE(m_hQMSLPhone,ucStepSize))
			return true;
	}
	return false;
}

bool Cwp7Phone::QMSL_FTM_CONFIGURE_TX_SWEEP_CAL(unsigned short iTimeInterval, unsigned short iNumofHDETReadingsPerStep)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_CONFIGURE_TX_SWEEP_CAL(m_hQMSLPhone,iTimeInterval,iNumofHDETReadingsPerStep))
			return true;
	}
	return false;
}

bool Cwp7Phone::QMSL_FTM_TX_SWEEP_CAL(bool bOnOff)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_TX_SWEEP_CAL(m_hQMSLPhone,(unsigned char)bOnOff))
			return true;
	}
	return false;
}

bool Cwp7Phone::QMSL_FTM_GET_HDET_FROM_TX_SWEEP_CAL(unsigned short usIndex,unsigned short* usHDET)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_GET_HDET_FROM_TX_SWEEP_CAL(m_hQMSLPhone,usIndex,usHDET))
			return true;
	}
	return false;
}

bool Cwp7Phone::QMSL_FTM_TX_RX_FREQ_CAL_SWEEP(FTM_Tx_Rx_Freq_Cal_Sweep_Request_Response *pFTM_Tx_Rx_Freq_Cal_Sweep_Request_Response)
{
   if (m_hQMSLPhone!=NULL)
   {
		if(QLIB_FTM_TX_RX_FREQ_CAL_SWEEP(m_hQMSLPhone,pFTM_Tx_Rx_Freq_Cal_Sweep_Request_Response))
			return true;
   }
   return false;
}

bool Cwp7Phone::QMSL_DIAG_NV_WRITE_SetBatchMode()
{
	if (m_hQMSLPhone!=NULL)
	{
		if(QLIB_DIAG_NV_WRITE_SetBatchMode(m_hQMSLPhone,true))
			return true;
	}
	return false;
}

bool Cwp7Phone::QMSL_DIAG_NV_WRITE_SetBatchMode_False()
{
	if (m_hQMSLPhone!=NULL)
	{
		if(QLIB_DIAG_NV_WRITE_SetBatchMode(m_hQMSLPhone, false))
			return true;
	}
	return false;
}

bool Cwp7Phone::QMSL_DIAG_NV_WRITE_FlushBatchQueue()
{
	unsigned short _iLastErrorNvId;
	unsigned short _iLastTotalStatus;
	unsigned char _iLastItemStatus;
	BOOL BOk = true;
	if (m_hQMSLPhone!=NULL)
	{
		if(QLIB_DIAG_NV_WRITE_FlushBatchQueue(m_hQMSLPhone, &_iLastErrorNvId, &_iLastTotalStatus, &_iLastItemStatus))
			return true;
	}
	return false;
}

bool Cwp7Phone::QMSL_FTM_GET_ENH_THERM(unsigned long* iThermValue)
{
	if (m_hQMSLPhone!=NULL)
	{
		if(QLIB_FTM_GET_ENH_THERM(m_hQMSLPhone, iThermValue))
			return true;
	}
	return false;
}

bool Cwp7Phone::QMSL_FTM_GET_THERM(unsigned char* ucThermValue)
{
	if (m_hQMSLPhone!=NULL)
	{
		if(QLIB_FTM_GET_THERM(m_hQMSLPhone, ucThermValue))
			return true;
	}
	return false;
}



//////////////////////////////////////////////////////////////////////////
// GSM
//////////////////////////////////////////////////////////////////////////


bool Cwp7Phone::QMSL_ChangeFTM_BootMode(unsigned char ucFTMMode, unsigned char ucReset)
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone != NULL)
    {
        if (QLIB_ChangeFTM_BootMode(m_hQMSLPhone, ucFTMMode, ucReset))
            return true;
    }

    return false;
}

bool Cwp7Phone::QMSL_FTM_GSM_BER_StartSession(unsigned long iTimeOut)
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_GSM_BER_StartSession(m_hQMSLPhone, iTimeOut) )
            return true ;
    }

    return false ;
}

bool Cwp7Phone::QMSL_FTM_GSM_BER_AssignBCCH (unsigned short iChannel, int iBand)
{
    unsigned char szBand ;

    // QLIB define:
    // 0 = PGSM 900 = FTM_GSM_BER_PGSM_900
    // 1 = EGSM 900 = FTM_GSM_BER_EGSM_900
    // 2 = PCS 1900 = FTM_GSM_BER_PCS_1900
    // 3 = DCS 1800 = FTM_GSM_BER_DCS_1800
    // 4 = CELL 850 = FTM_GSM_BER_CELL_850
    if (iBand==850)
        szBand = 4 ;
    else if (iBand==1800)
        szBand = 3 ;
    else if (iBand==1900)
        szBand = 2 ;
    else if (iBand==900)
        szBand = 1 ;
    else
        return false ;

    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_GSM_BER_AssignBCCH(m_hQMSLPhone, iChannel, szBand) )
            return true ;
    }

    return false ;
}


bool Cwp7Phone::QMSL_FTM_GSM_BER_Handover_NoLoopbackConfig(unsigned short iChannel,
                                                           int iBand,
                                                           unsigned char iSlot,
                                                           unsigned char iPowerLevel )
{
    const unsigned char iTSC = 0 ;
    const unsigned char iMODE = 0 ;

    // QLIB define:
    // 0 = PGSM 900 = FTM_GSM_BER_PGSM_900
    // 1 = EGSM 900 = FTM_GSM_BER_EGSM_900
    // 2 = PCS 1900 = FTM_GSM_BER_PCS_1900
    // 3 = DCS 1800 = FTM_GSM_BER_DCS_1800
    // 4 = CELL 850 = FTM_GSM_BER_CELL_850
    unsigned char szBand ;
    if (iBand==850)
        szBand = 4 ;
    else if (iBand==1800)
        szBand = 3 ;
    else if (iBand==1900)
        szBand = 2 ;
    else if (iBand==900)
        szBand = 1 ;
    else
        return false ;

    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        unsigned char bRet = false ;
        bRet = QLIB_FTM_GSM_BER_Handover_NoLoopbackConfig(m_hQMSLPhone,
            iChannel,
            szBand,
            iTSC,
            iSlot,
            iMODE,
            iPowerLevel ) ;
        if (bRet)
            return true ;
    }

    return false ;
}


bool Cwp7Phone::QMSL_FTM_GSM_BER_IsTrafficValid()
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_GSM_BER_IsTrafficValid(m_hQMSLPhone))
            return true ;
    }
    return false ;
}


bool Cwp7Phone::QMSL_FTM_GSM_BER_ClearStatus()
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_GSM_BER_ClearStatus(m_hQMSLPhone))
            return true ;
    }
    return false ;
}


bool Cwp7Phone::QMSL_FTM_GSM_BER_StopSession()
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_GSM_BER_StopSession(m_hQMSLPhone))
            return true ;
    }
    return false ;
}

bool Cwp7Phone::QMSL_FTM_GSM_BER_ReleaseTCH()
{
    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_GSM_BER_ReleaseTCH(m_hQMSLPhone))
            return true ;
    }
    return false ;
}

bool Cwp7Phone::QMSL_FTM_EGPRS_BER_Establish_SRB_LOOPBACK()
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_EGPRS_BER_Establish_SRB_LOOPBACK(m_hQMSLPhone))
            return true ;
    }
    return false ;
}



bool Cwp7Phone::QMSL_FTM_EGPRS_BER_AssignBCCH (unsigned short iChannel, int iBand)
{
    unsigned char szBand ;

    // QLIB define:
    // 0 = PGSM 900 = FTM_GSM_BER_PGSM_900
    // 1 = EGSM 900 = FTM_GSM_BER_EGSM_900
    // 2 = PCS 1900 = FTM_GSM_BER_PCS_1900
    // 3 = DCS 1800 = FTM_GSM_BER_DCS_1800
    // 4 = CELL 850 = FTM_GSM_BER_CELL_850
    if (iBand==850)
        szBand = 4 ;
    else if (iBand==1800)
        szBand = 3 ;
    else if (iBand==1900)
        szBand = 2 ;
    else if (iBand==900)
        szBand = 1 ;
    else
        return false ;

    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_EGPRS_BER_AssignBCCH(m_hQMSLPhone, iChannel, szBand) )
            return true ;
    }

    return false ;
}



bool Cwp7Phone::QMSL_FTM_EGPRS_BER_Configure_DL_TBF (unsigned short iChannel,
                                                     int iBand)
{
    unsigned char iDL_TSC = 0 ;
    unsigned char iDL_TFI = 1 ;
    unsigned char bFirstSlotActive = 1 ;
    unsigned char iFirstSlotNum = 3 ;
    unsigned char bSecondSlotActive = 1 ;
    unsigned char iSecondSlotNum  = 4 ;

    // QLIB define:
    // 0 = PGSM 900 = FTM_GSM_BER_PGSM_900
    // 1 = EGSM 900 = FTM_GSM_BER_EGSM_900
    // 2 = PCS 1900 = FTM_GSM_BER_PCS_1900
    // 3 = DCS 1800 = FTM_GSM_BER_DCS_1800
    // 4 = CELL 850 = FTM_GSM_BER_CELL_850
    unsigned char szBand ;
    if (iBand==850)
        szBand = 4 ;
    else if (iBand==1800)
        szBand = 3 ;
    else if (iBand==1900)
        szBand = 2 ;
    else if (iBand==900)
        szBand = 1 ;
    else
        return false ;

    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        unsigned char bRet = false ;
        bRet = QLIB_FTM_EGPRS_BER_Configure_DL_TBF (m_hQMSLPhone,
            iDL_TSC,
            iDL_TFI,
            iChannel,
            szBand,
            bFirstSlotActive,
            iFirstSlotNum,
            bSecondSlotActive,
            iSecondSlotNum) ;
        if (bRet)
            return true ;
    }

    return false ;
}


bool Cwp7Phone::QMSL_FTM_EGPRS_BER_Configure_UL_TBF (unsigned short iChannel,
                                                     int iBand,
                                                     unsigned char iGamma)
{
    unsigned char iUL_TSC = 0 ;
    unsigned char iUSF = 0 ;
    unsigned char bFirstSlotActive = 1 ;
    unsigned char iFirstSlotNum = 3 ;
    unsigned char bSecondSlotActive = 1 ;
    unsigned char iSecondSlotNum  = 4 ;

    // QLIB define:
    // 0 = PGSM 900 = FTM_GSM_BER_PGSM_900
    // 1 = EGSM 900 = FTM_GSM_BER_EGSM_900
    // 2 = PCS 1900 = FTM_GSM_BER_PCS_1900
    // 3 = DCS 1800 = FTM_GSM_BER_DCS_1800
    // 4 = CELL 850 = FTM_GSM_BER_CELL_850
    unsigned char szBand ;
    if (iBand==850)
        szBand = 4 ;
    else if (iBand==1800)
        szBand = 3 ;
    else if (iBand==1900)
        szBand = 2 ;
    else if (iBand==900)
        szBand = 1 ;
    else
        return false ;

    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        unsigned char bRet = false ;
        bRet = QLIB_FTM_EGPRS_BER_Configure_UL_TBF (m_hQMSLPhone,
            iUL_TSC,
            iUSF,
            iGamma,
            bFirstSlotActive,
            iFirstSlotNum,
            bSecondSlotActive,
            iSecondSlotNum ) ;

        if (bRet)
            return true ;
    }

    return false ;
}


bool Cwp7Phone::QMSL_FTM_EGPRS_BER_Release_All_TBF()
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_EGPRS_BER_Release_All_TBF(m_hQMSLPhone) )
            return true ;
    }

    return false ;
}


bool Cwp7Phone::QMSL_FTM_GSM_ConfigLoopbackType(unsigned short usLookbackType)
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_GSM_ConfigLoopbackType(m_hQMSLPhone, usLookbackType) )
            return true ;
    }

    return false ;
}


bool Cwp7Phone::QMSL_FTM_GSM_BER_GetRxMetrics(unsigned char* szRxLevel, unsigned char* szRxQual)
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_GSM_BER_GetRxMetrics(m_hQMSLPhone, szRxLevel, szRxQual) )
            return true ;
    }

    return false ;
}


bool Cwp7Phone::QMSL_CheckPhoneConnect(int iRetryConnect)
{
    bool bIsConnect = false ;
    int  iRetry = iRetryConnect ;
    do
    {
        if ( IsConnected_QMSL() ) {
            bIsConnect = true;
            break ;
        }

        Sleep(500);
        --iRetry ;
    } while (iRetry > 0);

    if (! bIsConnect)
        return false ;

    return true ;
}

bool Cwp7Phone::QMSL_FTM_SET_TX_POWER_LEVEL(unsigned char nPowerLevel)
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_SET_TX_POWER_LEVEL(m_hQMSLPhone, nPowerLevel) )
            return true ;
    }

    return false ;
}


//////////////////////////////////////////////////////////////////////////
// WCDMA Test
//////////////////////////////////////////////////////////////////////////

bool Cwp7Phone::QMSL_FTM_UMTS_BER_ForceStopped()
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_UMTS_BER_ForceStopped(m_hQMSLPhone) )
			return true ;
	}

	return false ;
}

bool Cwp7Phone::QMSL_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ(unsigned char ucTFCS_Select, unsigned char* ucTFCS_Config)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ(m_hQMSLPhone, ucTFCS_Select, ucTFCS_Config) )
			return true ;
	}

	return false ;
}

bool Cwp7Phone::QMSL_FTM_WCDMA_BER_StartSession(unsigned long ulTimeOut_ms)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_WCDMA_BER_StartSession(m_hQMSLPhone, ulTimeOut_ms) )
			return true ;
	}

	return false ;
}

bool Cwp7Phone::QMSL_FTM_WCDMA_BER_AcquirePilot(unsigned short usChannel)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_WCDMA_BER_AcquirePilot(m_hQMSLPhone, usChannel) )
			return true ;
	}

	return false ;
}

bool Cwp7Phone::QMSL_FTM_WCDMA_BER_StopSession()
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_WCDMA_BER_StopSession(m_hQMSLPhone) )
			return true ;
	}

	return false ;
}

bool Cwp7Phone::QMSL_FTM_WCDMA_BER_Handover(unsigned char iRMC_Type, unsigned char iUL_MaxTxPwr, unsigned long iUL_DPCH_ScrCode, unsigned char iUL_PCA, unsigned char iUL_TPC_StepSize,  
											unsigned short iDL_PriCPICH_Scr_Code, unsigned short iDL_ChanCode, unsigned char iBetaC, unsigned char iBetaD, unsigned short iChannel)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_WCDMA_BER_Handover(m_hQMSLPhone, iRMC_Type, iUL_MaxTxPwr, iUL_DPCH_ScrCode, iUL_PCA, iUL_TPC_StepSize, iDL_PriCPICH_Scr_Code, iDL_ChanCode, iBetaC, iBetaD, iChannel) )
			return true ;
	}

	return false ;
}

bool Cwp7Phone::QMSL_FTM_WCDMA_BER_Handover_V2B(
												unsigned char iRMC_Type, 
												unsigned char iUL_MaxTxPwr, 
												unsigned long iUL_DPCH_ScrCode, 
												unsigned char iUL_PCA,
												unsigned char iUL_TPC_StepSize, 
												unsigned short iDL_PriCPICH_Scr_Code, 
												unsigned short iDL_ChanCode,
												unsigned char iBetaC, 
												unsigned char iBetaD,
												short iDPCCH_PowerOffset,
												unsigned char n_312,
												unsigned char n_313,
												unsigned char n_315,
												unsigned char t_312,
												unsigned char t_313,
												unsigned short iChannel
												)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_WCDMA_BER_Handover_V2B(
										m_hQMSLPhone			, iRMC_Type		, iUL_MaxTxPwr	, iUL_DPCH_ScrCode	, iUL_PCA			, iUL_TPC_StepSize, 
										iDL_PriCPICH_Scr_Code	, iDL_ChanCode	, iBetaC		, iBetaD			, iDPCCH_PowerOffset, n_312,
										n_313					,n_315			, t_312			, t_313				, iChannel) )
			return true ;
	}

	return false ;
}


bool Cwp7Phone::QMSL_FTM_DO_DC_CAL()
{
    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_DO_DC_CAL(m_hQMSLPhone) )
            return true ;
    }

    return false ;
}

bool Cwp7Phone::QMSL_FTM_GET_DVGA_OFFSET(short iExpectedRX_AGC, short* iDVGAoffset)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_GET_DVGA_OFFSET(m_hQMSLPhone, iExpectedRX_AGC, iDVGAoffset) )
			return true ;
	}

	return false ;
}

bool Cwp7Phone::QMSL_FTM_GET_CDMA_IM2(unsigned char* piI_Value, unsigned char* piQ_Value,
										 unsigned char* piTransConductance_Value)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_GET_CDMA_IM2(m_hQMSLPhone, piI_Value, piQ_Value, piTransConductance_Value) )
			return true ;
	}

	return false ;
}