/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .cpp file of Mobile Phone operate object.
 * FileName   : CPhone.cpp
 * Version    : $Revision:   1.23  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/16
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2011/03/16      Gilpin.Xi						Create.
 *  2    2011/07/22      Gilpin.Xi      Modify for FTD command document update
 ********************************************************************************/

#include "stdafx.h"
#include "CPhone.h"
#include "../CommonUtility/CommonUtil.h"
RF_IMPLEMENT_DYNCREATE(CPhone)


CPhone::CPhone()
{
	m_hFTDCmdDLL = NULL;
	m_hFuncBoardDLL = NULL;
	m_hPhone = NULL;
	memset(&processInfo, 0, sizeof(PROCESS_INFORMATION));
}

CPhone::~CPhone()
{
	if (m_hFTDCmdDLL != NULL)
	{
		FreeLibrary(m_hFTDCmdDLL);
		m_hFTDCmdDLL = NULL;
	}
	if (m_hFTDCmdDLL != NULL)
	{
		FreeLibrary(m_hFTDCmdDLL);
		m_hFTDCmdDLL = NULL;
	}
	if (m_hPhone != NULL)
	{
		QLIB_DisconnectServer(m_hPhone);
		m_hPhone = NULL;
	}
	if (processInfo.hProcess)
	{
		CloseHandle(processInfo.hProcess);
		processInfo.hProcess = NULL;
	}
	if (processInfo.hThread)
	{
		CloseHandle(processInfo.hThread);
		processInfo.hThread = NULL;
	}
}

//-------------------- For QMSL functions---------------------
bool CPhone::DetectDevice(char* szErrMsg)
{
	//if (m_str_comPort == "0" && ! m_bIsMultiMode)
	if (m_str_comPort == "0")
	{
		std::map<std::string, std::string> map_strstrCOMDeviceName;
		if (GetCOMPortDevByWDK(map_strstrCOMDeviceName))
		{
			std::map<std::string, std::string>::iterator itr;
			if (map_strstrCOMDeviceName.size() < 1)
			{
				strcpy(szErrMsg, "No COM device detected");
				return false;
			}
			else
			{
				if (m_str_KeyWord.empty())
				{
					if (map_strstrCOMDeviceName.size() == 1)
					{
						itr = map_strstrCOMDeviceName.begin();
						std::string strDev     = itr->first;
						std::string strDevName = itr->second;
						size_t iPosition;
						if ((iPosition = strDev.find("COM")) != std::string::npos)
						{
							m_str_comPort = m_strDeviceName = strDev.substr(3, (strDev.size() - 3));
						}
						else
						{
							m_str_comPort = m_strDeviceName = strDev;
						}
					}
					else
					{
						strcpy(szErrMsg, "Fail to determine COM port number, Keyword for COM device is empty while detect multiple device");
						return false;
					}
				}
				else
				{
					bool bFound = false;
					for (itr = map_strstrCOMDeviceName.begin(); itr != map_strstrCOMDeviceName.end(); ++itr)
					{
						std::string strDev = itr->first;
						std::string strDevName = itr->second;
						if (strDevName.find(m_str_KeyWord) !=  std::string::npos)
						{
							bFound = true;
							size_t iPosition;
							if ((iPosition = strDev.find("COM")) != std::string::npos)
							{
								m_str_comPort = m_strDeviceName = strDev.substr(3, (strDev.size() - 3));
							}
							else
							{
								m_str_comPort = m_strDeviceName = strDev;
							}
						}
					}
					if (!bFound)
					{
						strcpy(szErrMsg, "Fail to find device by Keyword");
						return false;
					}
				}
			}
		}
		else
		{
			strcpy(szErrMsg, "Fail to detect COM device with WDK");
			return false;
		}
	}
	return true;
}

bool CPhone::Initial_QMSL()
{
	if (m_hPhone)
	{
		if (IsConnected_QMSL())
		{
			QLIB_FlushRxBuffer(m_hPhone);
			return true;
		}
	}
	else
	{
		QLIB_SetLibraryMode(false);

		int nRetry = 5;
		while ((m_hPhone == NULL) && (nRetry > 0))
		{
			m_hPhone = QLIB_ConnectServerWithWait((unsigned)atoi(m_str_comPort.c_str()), 5000);
			nRetry--;
			Sleep(1000);
		}
		if (m_hPhone != NULL)
		{
			if (QLIB_IsPhoneConnected(m_hPhone))
			{
				QLIB_FlushRxBuffer(m_hPhone);
				return true;
			}
		}
	}
	CString CStrTemp;
	CStrTemp.Format(_T("Can not find Diag COM%s"), m_str_comPort.c_str());
	::AfxMessageBox(CStrTemp);
	return false;
}

bool CPhone::IsConnected_QMSL()
{
	if (m_hPhone != NULL)
	{
		if (QLIB_IsPhoneConnected(m_hPhone))
			return true;
	}
	return false;
}

bool CPhone::Disconnect_QMSL()
{
	if (m_hPhone != NULL)
	{
		QLIB_DisconnectServer(m_hPhone);
		m_hPhone = NULL;
	}
	return true;
}

bool CPhone::QMSL_GetPhoneOperatingMode(unsigned long* pulPhoneMode)
{
	if (m_hPhone == NULL){ return false; }

	if (!QLIB_GetPhoneOperatingMode(m_hPhone, pulPhoneMode))
	{
		return false;
	}
	return true;
	
}

bool CPhone::QMSL_DIAG_CONTROL_F(int iMode)
{
	if (m_hPhone == NULL){ return false; }

	std::string strMode = "Unknown";
	switch (iMode)
	{
	case MODE_OFFLINE_A_F:
		strMode = "MODE_OFFLINE_A_F";
		break;
	case MODE_OFFLINE_D_F:
		strMode = "MODE_OFFLINE_D_F";
		break;
	case MODE_RESET_F:
		strMode = "MODE_RESET_F";
		break;
	case MODE_FTM_F:
		strMode = "MODE_FTM_F";
		break;
	case MODE_ONLINE_F:
		strMode = "MODE_ONLINE_F";
		break;
	case MODE_LPM_F:
		strMode = "MODE_LPM_F";
		break;
	case MODE_POWER_OFF_F:
		strMode = "MODE_POWER_OFF_F";
		break;
	case MODE_MAX_F:
		strMode = "MODE_MAX_F";
		break;
	}

	if (!QLIB_DIAG_CONTROL_F(m_hPhone, iMode))
	{
		return false;
	}
	return true;

}

bool CPhone::ReadNV_QMSL(unsigned short unIndex, unsigned char* pData, int iSize)
{
	if (m_hPhone == NULL){ return false; }

	unsigned short unStatus = 0;

	if (!QLIB_DIAG_NV_READ_F(m_hPhone, unIndex, pData, iSize, &unStatus))
	{
		return false;
	}
	return true;
}


//-------------------- For Phone functions---------------------
static void GetADBlist(std::vector<std::string>& devices, CString info)
{
	info.Replace(_T("adb server is out of date.  killing..."), _T(""));
	info.Replace(_T("* daemon not running. starting it now on port 5037 *"), _T(""));
	info.Replace(_T("* daemon started successfully *"), _T(""));
	info.Replace(_T("List of devices attached"), _T(""));
	info.TrimLeft();
	info.Replace(_T("\r\n"), _T(","));
	info.Replace(_T("device"), _T(""));
	info.Replace(_T(" "), _T(""));
	info.Replace(_T("\t"), _T(""));
	std::string clean_info = CStringA(info);
	devices.clear();
	STDStringTokenizer(clean_info, std::string(","), devices);
	std::sort(devices.begin(), devices.end());
}

bool CPhone::IsConnected()
{
	CString command(_T("devices"));
	char cmdOut[1024] = {0};
	char errMsg[128] = {0};
	if (!ExecAdbCommand(command, cmdOut, errMsg))
		return false;

	CString sCheckDevices(cmdOut);
	sCheckDevices.Replace(_T("adb server is out of date.  killing..."), _T(""));
	sCheckDevices.TrimRight();
	sCheckDevices.TrimLeft();
	if (sCheckDevices.Find(_T("List of devices attached")) == -1)
		return false;//not found is fail
	if (sCheckDevices.Find(_T("error")) != -1 || sCheckDevices.Right(8) == _T("attached"))
		return false;//found is fail

	GetADBlist(m_devices, sCheckDevices);
	return true;
}

bool CPhone::IsConnectedMulti()
{
	CString command(_T("devices"));
	char cmdOut[1024] = {0};
	char errMsg[128] = {0};
	if (!ExecAdbCommand(command, cmdOut, errMsg))
		return false;

	CString sCheckDevices(cmdOut);
	sCheckDevices.TrimRight();
	sCheckDevices.TrimLeft();
	if (sCheckDevices.Find(_T("List of devices attached")) == -1)
		return false;//not found is fail
	if (sCheckDevices.Find(_T("error")) != -1 || sCheckDevices.Right(8) == _T("attached"))
		return false;//found is fail
	if (sCheckDevices.Find(_T("QPHONE1")) == -1 && sCheckDevices.Find(_T("QPHONE2")) == -1 && sCheckDevices.Find(_T("QPHONE3")) == -1 && sCheckDevices.Find(_T("QPHONE4")) == -1)
		return false;//not found is fail

	return true;
}

bool CPhone::IsMobileInFTDMode(int nPort, int nTimeout)
{
	PFNVDA_TestFtdModeFunc fp = (PFNVDA_TestFtdModeFunc)::GetProcAddress(m_hFTDCmdDLL, _T("FTD_TestFtdMode"));
	if (!fp) return false;
	char out_buf[FTD_BUF_SIZE] = {0};
	return (fp(nPort, nTimeout, out_buf) == 1);//HS_OK
}

bool CPhone::MobileAutoFTDMode(int nPort, int nTimeout)
{
	PFNVDA_AutoFtdModeFunc fp = (PFNVDA_AutoFtdModeFunc)::GetProcAddress(m_hFTDCmdDLL, _T("FTD_AutoFtdMode"));
	if (!fp) return false;
	char out_buf[FTD_BUF_SIZE] = {0};
	return (fp(nPort, nTimeout, out_buf) == 1);//HS_OK
}

bool CPhone::Disconnect()
{
	return true;
}

bool CPhone::ExecAdbCommand(CString str_command, char* szOutput, char* szErrorMsg)
{
	PFExecAdbOutFunc fp = (PFExecAdbOutFunc)::GetProcAddress(m_hFTDCmdDLL, _T("FTD_ExecAdbOut"));
	if (!fp) return false;
	return fp(str_command, szOutput, szErrorMsg);
}

bool CPhone::LoadMobileCommandDLL()
{
	if (m_hFTDCmdDLL == NULL)
		m_hFTDCmdDLL = ::LoadLibrary(_T("QisdaAndroidCmdCtrl.dll"));
	return (!!m_hFTDCmdDLL);
}

bool CPhone::InitDevice(int nPort, int nTimeout)
{
	PFInitDeviceFunc fp = (PFInitDeviceFunc)::GetProcAddress(m_hFTDCmdDLL, _T("InitDevice"));
	if (!fp) return false;
	return (fp(nPort, nTimeout) == 1);//HS_OK
}

bool CPhone::FreeDevice(int nPort, int nTimeout)
{
	PFFreeDeviceFunc fp = (PFFreeDeviceFunc)::GetProcAddress(m_hFTDCmdDLL, _T("FreeDevice"));
	if (!fp) return false;
	return (fp(nPort, nTimeout) == 1);//HS_OK
}

bool CPhone::SearchPortUSB(const UINT nTimeout)
{
	PFSearchPortUsbFunc fp = (PFSearchPortUsbFunc)::GetProcAddress(m_hFTDCmdDLL, _T("SearchPortUsb"));
	if (!fp) return false;
	return fp(nTimeout);
}

bool CPhone::ExactOpenDevice(int nPort, int nTimeout)
{
	PFExactControl fp = (PFExactControl)::GetProcAddress(m_hFTDCmdDLL, _T("ExactOpenDevice"));
	if (!fp) return false;
	return (fp(nPort, nTimeout) == 1);//HS_OK
}

bool CPhone::ExactCloseDevice(int nPort, int nTimeout)
{
	PFExactControl fp = (PFExactControl)::GetProcAddress(m_hFTDCmdDLL, _T("ExactCloseDevice"));
	if (!fp) return false;
	return (fp(nPort, nTimeout) == 1);//HS_OK
}

bool CPhone::ReceiveCommandData(int nPort,  int nTimeout, char* cIDCommand, char* szOutput)
{
	PFReceiveCmdOutFunc fp = (PFReceiveCmdOutFunc)::GetProcAddress(m_hFTDCmdDLL, _T("ReceiveCmdOut"));
	if (!fp) return false;
	return (fp(nPort, nTimeout, cIDCommand, szOutput) == 1);//HS_OK
}

bool CPhone::InitData(std::map<std::string, std::string>& paramMap)
{
	m_str_comPort = "0";

	// COMPort
	if (paramMap.find(std::string("COMPort")) != paramMap.end())
	{
		m_str_comPort = paramMap[std::string("COMPort")];
	}

	// IsDiagCmd
	if (paramMap.find(std::string("Keyword")) != paramMap.end())
	{
		m_str_KeyWord = paramMap[std::string("Keyword")];
	}

	return Initial();
}

bool CPhone::Initial()
{
	if (!LoadMobileCommandDLL())
		return false;

	//if (atoi(m_str_comPort.c_str()) != 0 && !Initial_QMSL())
	//	return false;

	return true;
}

//5M Test
/************Focus Index*************/
//0: Normal Focus
//1: Macro Focus
/************Focus Index*************/
/************Outbuffer*************/
//If return success,no payload. If fail:
//¡®P¡¯:preview fail
//¡®AN¡¯: auto focus Normal mode fail
//¡®AM¡¯: auto focus Macro mode fail
//¡®C¡¯:capture fail
/************Outbuffer*************/
bool CPhone::CAMERA_5M_Test(int nPort, int nTimeout, int index, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	sprintf_s(in_buf, sizeof(in_buf), "%d", index);
	return FTDCmdCtrl(nPort, nTimeout, "NVDA_CAMERA_5MTest", in_buf, szOutput);
}

//LCD
//Display Index - r: Red, g: Green, b: Blue
CPHONE_FTD_CMD_IN_OUT_CREATE(Display_LCD_RGB, "FTD_LCMbacklightColor")

bool CPhone::TouchPanelTest(int nPort,  int nTimeout, bool testFlag, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	sprintf_s(in_buf, sizeof(in_buf), "%d", testFlag);
	return FTDCmdCtrl(nPort, nTimeout, "FTD_TOUCHtest", in_buf, szOutput);
}

bool CPhone::TouchCapkeyTest(int nPort,  int nTimeout, bool testFlag, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	sprintf_s(in_buf, sizeof(in_buf), "%d", testFlag);
	return FTDCmdCtrl(nPort, nTimeout, "FTD_KPtouchTest", in_buf, szOutput);
}

CPHONE_FTD_CMD_IN_OUT_CREATE(TOUCHversion, "FTD_TOUCHversion")

bool CPhone::CheckDFIVersion(int nPort, int nTimeout, char* szOutput)
{
	PFGenCmdOut fp = (PFGenCmdOut)::GetProcAddress(m_hFTDCmdDLL, _T("NVDA_BATTERY_DFIVersion"));
	if (!fp) return false;
	return (fp(nPort, nTimeout, szOutput) == 1);//HS_OK
}

// Read IC ID functions
bool CPhone::ReadGaugeID(int nPort, int nTimeout, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_PWRgaugeID", in_buf, szOutput);
}

bool CPhone::ReadChargerID(int nPort, int nTimeout, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_PWRchargerID", in_buf, szOutput);
}

bool CPhone::ReadBatteryID(int nPort, int nTimeout, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_PWRbattID", in_buf, szOutput);
}

bool CPhone::ReadCpuID(int nPort, int nTimeout, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_CPUid", in_buf, szOutput);
}

bool CPhone::ReadGSensorID(int nPort, int nTimeout, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_SNSRgsensorID", in_buf, szOutput);
}

bool CPhone::ReadECompassID(int nPort, int nTimeout, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_SNSRecompassID", in_buf, szOutput);
}

bool CPhone::ReadGyroID(int nPort, int nTimeout, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_SNSRgyroscopeID", in_buf, szOutput);
}

bool CPhone::FTD_SNSRGyroscopeTest(int nPort, int nTimeout, char* Inbuffer, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_SNSRgyroscopeValue", in_buf, szOutput);
}

bool CPhone::FTD_SNSRGyroscopeSelfTest(int nPort, int nTimeout, char* Inbuffer, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_SNSRGyroscopeSelfTest", in_buf, szOutput);
}

bool CPhone::ReadCapKeyID(int nPort, int nTimeout, char* szId)
{
	PFGenCmdOut fp = (PFGenCmdOut)::GetProcAddress(m_hFTDCmdDLL, _T("NVDA_KEYPAD_GetCapkeyChipID"));
	if (!fp) return false;
	return (fp(nPort, nTimeout, szId) == 1);//HS_OK
}

bool CPhone::ReadTouchID(int nPort, int nTimeout, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_TOUCHid", in_buf, szOutput);
}

bool CPhone::ReadLSensorID(int nPort, int nTimeout, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_SNSRlsensorID", in_buf, szOutput);
}

bool CPhone::ReadPSensorID(int nPort, int nTimeout, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_SNSRpsensorID", in_buf, szOutput);
}

bool CPhone::ReadTouchModel(int nPort, int nTimeout, char* szOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_TouchModel", in_buf, szOutput);
}


CPHONE_FTD_CMD_IN_OUT_CREATE(LED_BacklightOnOff, "FTD_LCMbacklight")

bool CPhone::FTDCmdCtrl(int nPort, int nTimeout, char* szFuncName, char* pszInput, char* pszOutput)
{
	CString dllFunctionName(szFuncName);
	PFGenCmdInOut fp = (PFGenCmdInOut)::GetProcAddress(m_hFTDCmdDLL, dllFunctionName);
	if (!fp) return false;
	return (fp(nPort, nTimeout, pszInput, pszOutput) == 1);
}

int CPhone::FTDCmdCtrl_Ret(int nPort, int nTimeout, char* szFuncName, char* pszInput, char* pszOutput)
{
	CString dllFunctionName(szFuncName);
	PFGenCmdInOut fp = (PFGenCmdInOut)::GetProcAddress(m_hFTDCmdDLL, dllFunctionName);
	if (!fp) return -99999;
	return fp(nPort, nTimeout, pszInput, pszOutput);
}

//FTD function
//01-Battery
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_BATTcoincellVoltage, "FTD_BATTcoincellVoltage")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_BATTcoincellChargeOn, "FTD_BATTcoincellChargeOn")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_BATTrtcTime, "FTD_BATTrtcTime")

// 02-Power
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PWRgaugeID, "FTD_PWRgaugeID")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PWRgaugeVoltage, "FTD_PWRgaugeVoltage")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PWRgaugeCurrent, "FTD_PWRgaugeCurrent")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PWRgaugeTemperature, "FTD_PWRgaugeTemperature")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PWRchargerID, "FTD_PWRchargerID")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PWRchargerOn, "FTD_PWRchargerOn")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PWRbattCapacity, "FTD_PWRbattCapacity")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PWRWriteRsenseData, "FTD_PWRWriteRsenseData")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PWRAverageGaugeCurrent, "FTD_PWRAverageGaugeCurrent")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PWRRFBandIDTemperature, "FTD_PWRRFBandIDTemperature")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_WriteMaxChargeCurrent, "FTD_WriteMaxChargeCurrent")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_ReadPowerMaxCurrent, "FTD_ReadPowerMaxCurrent")





//03-Memory
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_MCPflashID, "FTD_MCPflashID")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_MCPddrID, "FTD_MCPddrID")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_MCPddrSize, "FTD_MCPddrSize")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_MCPCsdInfo, "FTD_MCPCsdInfo")

CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_MCPemmcsize, "FTD_MCPemmcsize")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_MCPemmcFWversion, "FTD_MCPemmcFWversion")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_MCPddrmr8, "FTD_MCPddrmr8")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_MCPddrmr6, "FTD_MCPddrmr6")

	

//04-LCM
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_TouchSelfTest, "FTD_TOUCHSelfTest")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_TouchModel, "FTD_TouchModel")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_TouchCheck, "FTD_TouchCheck")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_OLEDTest, "FTD_OLEDTest")

//05-Audio
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_AUpathSwitch, "FTD_AUoutSwitch")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_AUplaymp3, "FTD_AUplaymp3")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_AUrecordWAV, "FTD_AUrecordWAV")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_AUplayWAV, "FTD_AUplayWAV")

bool CPhone::FTD_AUdetectHeadset(int nPort, int nTimeout, char* pszOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_AUdetectHeadset", in_buf, pszOutput);
}

bool CPhone::FTD_AUButtonCheck(int nPort, int nTimeout, char* pszOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_AUButtonCheck", in_buf, pszOutput);
}

CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_AUpathOut, "FTD_AUpathOut")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_AUloopbackTest, "FTD_AUloopbackTest")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_AUinitADC, "FTD_AUinitADC")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_AUADCCali, "FTD_AUADCCali")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_AUheadsetInfo, "FTD_AUheadsetInfo")

bool CPhone::FTD_AUButtonCount(int nPort, int nTimeout, char* pszOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "HeadsetButtonCount", in_buf, pszOutput);
}

//06-Camera
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_CAMReadID, "FTD_CAMReadID")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_CAMshot, "FTD_CAMshot")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_CAMstart, "FTD_CAMstart")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_CAMend, "FTD_CAMend")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_CAMFlashLEDID, "FTD_CAMFlashLEDID")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_CAM_OTP_AWB, "FTD_CAM_OTP_AWB")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_CAM_OTP_AFMacro, "FTD_CAM_OTP_AFMacro")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_CAM_OTP_AFInfinity, "FTD_CAM_OTP_AFInfinity")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_CAM_OTP_FrontFuseID, "FTD_CAM_OTP_FrontFuseID")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_CAM_OTP_WriteFrontFuseID, "FTD_CAM_OTP_WriteFrontFuseID")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_CAM_Read_Bridge_ID, "FTD_CAM_Read_Bridge_ID")

//07-keypad
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_KeyTest, "FTD_KeyTest")

//08-SIM
bool CPhone::FTD_SIMtest(int nPort, int nTimeout, char* pszOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_SIMtest", in_buf, pszOutput);
}

bool CPhone::FTD_SIMdetect(int nPort, int nTimeout, char* pszOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_SIMdetect", in_buf, pszOutput);
}

//10-Vibrator
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_VibRun, "FTD_VibRun")

//13-BT
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_BTid, "FTD_BTid")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_BTTestMode, "FTD_BTTestMode")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_BTTXPower, "FTD_BTTXPower")

CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_BTRXrssi, "FTD_BTRXrssi")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_BTSelfTest, "FTD_BTSelfTest")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_BTLteTx, "FTD_BTLteTx")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_BTLteRx, "FTD_BTLteRx")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_2_4GTxPower, "FTD_2_4GTxPower")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_2_4GPowerMode, "FTD_2_4GPowerMode")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_2_4GConsumer, "FTD_2_4GConsumer")

//14-WLAN
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_WLAN_Mode, "FTD_WLAN_Mode")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_WLAN_TXPower, "FTD_WLAN_TXPower")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_WLAN_ScanAP, "FTD_WLAN_ScanAP")


CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_WLAN_selfTest, "FTD_WLAN_selfTest")


//15-GPS
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_GPSValueCN0, "FTD_GPSValueCN0")

//16-FMR
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FMRrssi, "FTD_FMRrssi")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FMRModuleOn, "FTD_FMRModuleOn")


CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FMR_SelfTest, "FTD_FMR_SelfTest")


//17-NFC
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_NFCid, "FTD_NFCid")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_NFCFWVersion, "FTD_NFCFWVersion")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_NFCAntennaSelftest, "FTD_NFCAntennaSelftest")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_NFCSwpSelftest, "FTD_NFCSwpSelftest")

//18-Sensor
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SNSRgsensorID, "FTD_SNSRgsensorID")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SNSRgsensorAcc, "FTD_SNSRgsensorAcc")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SNSRgsensorCalib, "FTD_SNSRgsensorCalib")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SNSRecompassID, "FTD_SNSRecompassID")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SNSREcompassSelfTest, "FTD_SNSREcompassSelfTest")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SNSRlsensorID, "FTD_SNSRlsensorID")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SNSRlsensorValue, "FTD_SNSRlsensorValue")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SNSRlsensorCalib, "FTD_SNSRlsensorCalib")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SNSRpsensorID, "FTD_SNSRpsensorID")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SNSRpsensorValue, "FTD_SNSRpsensorValue")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SNSRpsensorCalib, "FTD_SNSRpsensorCalib")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SNSRloadFALPkdata, "FTD_SNSRloadFALPkdata")

CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SNSRgsensorSelfTest, "FTD_SNSRgsensorSelfTest")


//19-LED
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_LEDid, "FTD_LEDid")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_CAMFlashLED, "FTD_CAMFlashLED")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_LEDon, "FTD_LEDon")

//20-SD
bool CPhone::FTD_SDdetect(int nPort, int nTimeout, char* pszOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_SDdetect", in_buf, pszOutput);
}

bool CPhone::FTD_SDdetectPin(int nPort, int nTimeout, char* pszOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_SDdetectPin", in_buf, pszOutput);
}

//21-ID
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_CPUid, "FTD_IDcpu")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_BoardADC, "FTD_BoardADC")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_AntennaIC, "FTD_AntennaIC")


CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_Audioid	, "FTD_Audioid")	//6
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PMICid		, "FTD_PMICid")	//7

CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_RFPAQFEid	, "FTD_RFPAQFEid")	//11
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_RFASMid	, "FTD_RFASMid")	//12

CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_Boardid	, "FTD_Boardid")	//14
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SKUid		, "FTD_SKUid")		//15
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SOCversion	, "FTD_SOCversion") //16
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SOCfeature	, "FTD_SOCfeature") //17


//22-SmartCover
CPHONE_FTD_CMD_IN_OUT_CREATE(Sapporo_SCoverDetect, "Sapporo_SCoverDetect")
CPHONE_FTD_CMD_IN_OUT_CREATE(Sapporo_SCoverInterrupt, "Sapporo_SCoverInterrupt")
CPHONE_FTD_CMD_IN_OUT_CREATE(Sapporo_SCoverEnable3V, "Sapporo_SCoverEnable3V")
CPHONE_FTD_CMD_IN_OUT_CREATE(Sapporo_SCoverCheckI2C, "Sapporo_SCoverCheckI2C")
CPHONE_FTD_CMD_IN_OUT_CREATE(Sapporo_SCoverSwitch5V, "Sapporo_SCoverSwitch5V")

//90-OS
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_OSpwr2home, "FTD_OSpwr2home")
CPHONE_FTD_CMD_IN_OUT_CREATE_RET(FTD_OSgetprop, "FTD_OSgetprop")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_OSmodeSleep, "FTD_OSmodeSleep")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_ShutDown, "FTD_ShutDown")

//91-version
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_VERftd, "FTD_VERftd")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_VERchange, "FTD_VERchange")

//92-factory
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_FARead, "FTD_FAC_FARead")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_FAWrite, "FTD_FAC_FAWrite")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_Picasso, "FTD_FAC_Picasso")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_OTPRead, "FTD_FAC_OTPRead")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_OTPWrite, "FTD_FAC_OTPWrite")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_OTPEnable, "FTD_FAC_OTPEnable")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_CFGRead, "FTD_FAC_CFGRead")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_CFGWrite, "FTD_FAC_CFGWrite")

CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_FSGProtect, "FTD_FAC_FSGProtect")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_BackupFA, "FTD_FAC_BackupFA")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_BackupCFG, "FTD_FAC_BackupCFG")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_RestoreFA, "FTD_FAC_RestoreFA")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_RestoreCFG, "FTD_FAC_RestoreCFG")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_BackupModemCFG, "FTD_FAC_BackupModemCFG")

//93-misc
CPHONE_FTD_CMD_IN_OUT_CREATE(Sapporo_ftdStartEnd, "Sapporo_ftdStartEnd")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_Get_GPIO_Value, "FTD_Get_GPIO_Value")
