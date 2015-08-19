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

RF_IMPLEMENT_DYNCREATE(CPhone)


CPhone::CPhone()
{
	m_hFTDCmdDLL = NULL;
	m_hFuncBoardDLL = NULL;
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
bool CPhone::Initial_QMSL()
{
	if (m_hPhone)
	{
		if (QLIB_IsPhoneConnected(m_hPhone))
		{
			QLIB_FlushRxBuffer(m_hPhone);
			return true;
		}
	}
	else
	{
		QLIB_SetLibraryMode(false);

		int nRetry = 5;
		while ((m_hPhone == NULL) || (nRetry <= 0))
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
	return false;
}

bool CPhone::IsConnected_QMSL()
{
	if (m_hPhone != NULL)
	{
		if (QLIB_IsPhoneConnected(m_hPhone))
			return true;
	}
	return Initial_QMSL();
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

//-------------------- For USB4702 Control Board functions---------------------
bool CPhone::LoadControlBoardDLL()
{
	if (m_hFuncBoardDLL == NULL)
		m_hFuncBoardDLL = ::LoadLibrary(_T("USB4702Ctrl.dll"));
	return (!!m_hFuncBoardDLL);
}

bool CPhone::InitControlBoard()
{
	PFUSB4702_InitFunc fp = (PFUSB4702_InitFunc)::GetProcAddress(m_hFuncBoardDLL, _T("USB4702_Init"));
	if (!fp) return false;
	return (fp(0) == 1);
}

bool CPhone::USB4702_Close()
{
	PFUSB4702_CloseFunc fp = (PFUSB4702_CloseFunc)::GetProcAddress(m_hFuncBoardDLL, _T("USB4702_Close"));
	if (!fp) return false;
	return (fp(0) == 1);
}

bool CPhone::USB4702_SetAOVol(int nAddress, int iChannel, char* voltage)
{
	PFUSB4702_SetAOVolFunc fp = (PFUSB4702_SetAOVolFunc)::GetProcAddress(m_hFuncBoardDLL, _T("USB4702_SetAOVol"));
	if (!fp) return false;
	char Channel[2] = {0};
	_itoa_s(iChannel, Channel, sizeof(Channel), 10);
	return (fp(nAddress, Channel, voltage) == 1);
}

bool CPhone::USB4702_GetADCVol(int nAddress, int iChannel, char* OutValue)
{
	PFUSB4702_GetADCVolFunc fp = (PFUSB4702_GetADCVolFunc)::GetProcAddress(m_hFuncBoardDLL, _T("USB4702_GetADCVol"));
	if (!fp) return false;
	char Channel[2] = {0};
	_itoa_s(iChannel, Channel, sizeof(Channel), 10);
	return (fp(nAddress, Channel, OutValue) == 1);
}

bool CPhone::USB4702_SetDOBit(int nAddress, int ibit)
{
	PFUSB4702_SetDOBitFunc fp = (PFUSB4702_SetDOBitFunc)::GetProcAddress(m_hFuncBoardDLL, _T("USB4702_SetDOBit"));
	if (!fp) return false;
	char bit[2] = {0};
	_itoa_s(ibit, bit, sizeof(bit), 10);
	return (fp(nAddress, bit) == 1);
}

bool CPhone::USB4702_ClearDOBit(int nAddress, int ibit)
{
	PFUSB4702_ClearDOBitFunc fp = (PFUSB4702_ClearDOBitFunc)::GetProcAddress(m_hFuncBoardDLL, _T("USB4702_ClearDOBit"));
	if (!fp) return false;
	char bit[2] = {0};
	_itoa_s(ibit, bit, sizeof(bit), 10);
	return (fp(nAddress, bit) == 1);
}

//Phone functions
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


int CPhone::IsConnectedMode( int iType )
{

	if ( iType == 1 )
	{
		CString command(_T("devices"));
		char cmdOut[1024] = {0};
		char errMsg[128] = {0};
		if (!ExecAdbCommand(command, cmdOut, errMsg))
			return -1;

		CString sCheckDevices(cmdOut);
		sCheckDevices.Replace(_T("adb server is out of date.  killing..."), _T(""));
		sCheckDevices.TrimRight();
		sCheckDevices.TrimLeft();
		if (sCheckDevices.Find(_T("List of devices attached")) == -1)
			return -1;//not found is fail
		if (sCheckDevices.Find(_T("error")) != -1 || sCheckDevices.Right(8) == _T("attached"))
			return -1;//found is fail

		GetADBlist(m_devices, sCheckDevices);
		return 1;
	}
	else if ( iType == 2 )
	{
		CString command(_T("devices"));
		char cmdOut[1024] = {0};
		char errMsg[128] = {0};
		if (!ExecFastbootCommand(command, cmdOut, errMsg))
			return -1;
		if (strlen(cmdOut) > 0)
		{
			return 2;
		}
	}
	else if ( iType == 99 )
	{
		CString command(_T("devices"));
		char cmdOut[1024] = {0};
		char errMsg[128] = {0};
		if (!ExecFastbootCommand(command, cmdOut, errMsg))
			return -1;
		if (strlen(cmdOut) > 0)
		{
			return 2;
		}

		char cmdOut2[1024] = {0};
		char errMsg2[128] = {0};
		if (!ExecAdbCommand(command, cmdOut2, errMsg2))
			return -1;

		CString sCheckDevices(cmdOut2);
		sCheckDevices.Replace(_T("adb server is out of date.  killing..."), _T(""));
		sCheckDevices.TrimRight();
		sCheckDevices.TrimLeft();
		if (sCheckDevices.Find(_T("error")) != -1 || sCheckDevices.Right(8) == _T("attached"))
			return -1;//found is fail

		GetADBlist(m_devices, sCheckDevices);
		if ( m_devices.size() > 0 )
		{
			return 1;
		}

	}
	else
	{
		return -1;
	}
	return 0;
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

bool CPhone::ExecFastbootCommand(CString str_command, char* szOutput, char* szErrorMsg)
{
	PFExecAdbOutFunc fp = (PFExecAdbOutFunc)::GetProcAddress(m_hFTDCmdDLL, _T("FTD_ExecFastbootOut"));
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
	if (m_str_diagCmdFlag == "0") return true;

	PFInitDeviceFunc fp = (PFInitDeviceFunc)::GetProcAddress(m_hFTDCmdDLL, _T("InitDevice"));
	if (!fp) return false;
	return (fp(nPort, nTimeout) == 1);//HS_OK
}

bool CPhone::FreeDevice(int nPort, int nTimeout)
{
	if (m_str_diagCmdFlag == "0") return true;

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
	// COMPort
	if (paramMap.find(std::string("COMPort")) != paramMap.end())
	{
		m_str_comPort = paramMap[std::string("COMPort")];
	}

	// IsDiagCmd
	if (paramMap.find(std::string("IsDiagCmd")) != paramMap.end())
	{
		m_str_diagCmdFlag = paramMap[std::string("IsDiagCmd")];
	}

	// Variant
	if (paramMap.find(std::string("Variant")) != paramMap.end())
	{
		m_strVariant = paramMap[std::string("Variant")];
	}

	return Initial();
}

bool CPhone::Initial()
{
	//if (!LoadControlBoardDLL())
	//	return false;

	if (!LoadMobileCommandDLL())
		return false;

	if (atoi(m_str_comPort.c_str()) != 0 && !Initial_QMSL())
		return false;

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

//NV Functions
bool CPhone::ReadNVItem(int nPort, int nTimeout, int nvID, char* nvData)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	sprintf_s(in_buf, sizeof(in_buf), "%1d%5d", ASYNC, nvID);
	return FTDCmdCtrl(nPort, nTimeout, "NVDA_NV_ReadNV", in_buf, nvData);
}

bool CPhone::WriteNVItem(int nPort, int nTimeout, int nvID, char* szInputData, size_t nInputSize, char* nvStatus)
{
	PFGenCmdInInOut fp = (PFGenCmdInInOut)::GetProcAddress(m_hFTDCmdDLL, _T("NVDA_NV_WriteNV"));
	if (!fp) return false;
	char in_buf[FTD_BUF_SIZE] = {0};
	sprintf(in_buf, "%1d%4d", ASYNC, nvID);
	memcpy(in_buf + 5, szInputData, nInputSize);
	return (fp(nPort, nTimeout, in_buf, nInputSize + 5, nvStatus) == 1);//HS_OK
}

//wireless Diag Command(QLib)
bool CPhone::SetPARange_Diag(unsigned short unSlot, unsigned short unPARange)
{
	return (m_hPhone && QLIB_FTM_SET_GSM_LINEAR_PA_RANGE(m_hPhone, unSlot, unPARange));
}

bool CPhone::SetRGI(unsigned short unSlot, unsigned short unRGI, unsigned char ucModType)
{
	return (m_hPhone && QLIB_FTM_GSM_SET_LINEAR_RGI(m_hPhone, unSlot, unRGI, ucModType));
}

int CPhone::SetRFChannel(unsigned short iChannel)
{
	int RetStatus = QLIB_FUNCTION_SUCCESSFUL;
	if (QLIB_FTM_SET_CHAN(m_hPhone, iChannel))
	{
		// Nothing now
	}
	else
	{
		RetStatus = StrToInt(ERR_QLIB_FUNCTION_FAILED);
	}
	ReleaseMutex(m_hPhone);
	return RetStatus;
}

BOOL CPhone::SetBand(int iTestBand)
{
	BOOL bOk = (QLIB_FTM_SET_MODE(m_hPhone, iTestBand) > 0) ? TRUE : FALSE;
	ReleaseMutex(m_hPhone);
	return bOk;
}

BOOL CPhone::PhoneIsConnected(void)
{
	return true;
}

BOOL CPhone::ConnectServer()
{
	QLIB_SetLibraryMode(m_qLibMode);
	if (m_IsHandshakeconnect)
	{
		m_hPhone = QLIB_ConnectServerWithHandShake(m_nFtdPort);
	}
	else
	{
		m_hPhone = QLIB_ConnectServer(m_nFtdPort);
	}
	if (!m_hPhone)
		return false;
	ReleaseMutex(m_hPhone);

	if (m_IsHandshakeconnect)
		FTM_FlushRX_Buffer();
	int iRetryConnectedTimes = 5;
	for (int i = 0; i < iRetryConnectedTimes; i++)
	{
		if (PhoneIsConnected())
			break;
		Sleep(100);
	}

	if (!m_bConnected)
	{
		QLIB_DisconnectServer(m_hPhone);
		m_hPhone = 0;
	}
	return m_bConnected;
}

void CPhone::SetCOMPort(unsigned int iComPort)
{
	m_nFtdPort = iComPort;
	if (m_bLogging)
	{
		CString content;
		content.Format(_T("COM%d port set\n"), iComPort);
		TRACE0(content);
	}
}

void CPhone::DisconnectServer()
{
	if (m_hPhone)
		QLIB_DisconnectServer(m_hPhone);
	m_hPhone = 0;
	ReleaseMutex(m_hPhone);
}

BOOL CPhone::ConnectThePhone()
{
	return this->ConnectServer();
}

BOOL CPhone::DisconnectThePhone()
{
	this->DisconnectServer();
	return !m_bConnected;
}

int CPhone::FTM_FlushRX_Buffer()
{
	int RetStatus = QLIB_FUNCTION_SUCCESSFUL;
	if (QLIB_FlushRxBuffer(m_hPhone))
	{
		// Nothing now
	}
	else
	{
		RetStatus = StrToInt(ERR_QLIB_FUNCTION_FAILED);
	}
	ReleaseMutex(m_hPhone);
	return RetStatus;
}

BOOL CPhone::SetPhoneBand(FTM_RF_Mode_Enum phoneBand)
{
	switch (phoneBand)
	{
	case PHONE_MODE_GPS:
		return this->SetBand(PHONE_MODE_GPS);
	case PHONE_MODE_GSM_850:
		return this->SetBand(PHONE_MODE_GSM_850);
	case PHONE_MODE_WCDMA_IMT:
		return this->SetBand(PHONE_MODE_WCDMA_IMT);
	case PHONE_MODE_WCDMA_800:
		return this->SetBand(PHONE_MODE_WCDMA_800);
	case PHONE_MODE_WCDMA_1900B:
		return this->SetBand(PHONE_MODE_WCDMA_1900B);
	case PHONE_MODE_WCDMA_BC4://new
		return this->SetBand(PHONE_MODE_WCDMA_BC4);
	case PHONE_MODE_WCDMA_BC8://new
		return this->SetBand(PHONE_MODE_WCDMA_BC8);
	case PHONE_MODE_GSM_900:
		return this->SetBand(PHONE_MODE_GSM_900);
	case PHONE_MODE_GSM_1800:
		return this->SetBand(PHONE_MODE_GSM_1800);
	case PHONE_MODE_GSM_1900:
		return this->SetBand(PHONE_MODE_GSM_1900);
	case PHONE_MODE_CDMA_800:
		return this->SetBand(PHONE_MODE_CDMA_800);
	case PHONE_MODE_CDMA_1900:
		return this->SetBand(PHONE_MODE_CDMA_1900);
	default:
		return FALSE;
	}
}

BOOL CPhone::SetBand(FTM_RF_Mode_Enum Band)
{
	BOOL bOk = (QLIB_FTM_SET_MODE(m_hPhone, Band) > 0) ? TRUE : FALSE;
	ReleaseMutex(m_hPhone);
	return bOk;
}

//Set Phone Power ON OR OFF
int CPhone::SetTxOnOff(int On_Off)
{
	int RetStatus = QLIB_FUNCTION_SUCCESSFUL;
	if (On_Off == ON)
	{
		if (QLIB_FTM_SET_TX_ON(m_hPhone))
		{
			//  Nothing now
		}
		else
		{
			RetStatus = StrToInt(ERR_QLIB_FUNCTION_FAILED) ;
		}
	}
	else if (On_Off == OFF)
	{
		if (QLIB_FTM_SET_TX_OFF(m_hPhone))
		{
			//  Nothing now
		}
		else
		{
			RetStatus = StrToInt(ERR_QLIB_FUNCTION_FAILED);
		}
	}
	else
	{
		RetStatus =  StrToInt(ERR_QLIB_WRONG_VALUE);
	}
	ReleaseMutex(m_hPhone);
	return RetStatus;
}

bool CPhone::ReadWCDMANV(unsigned short unIndex, unsigned char* pData, int iSize)
{
	if (m_hPhone)
	{
		unsigned short unStatus = 0;
		if (QLIB_DIAG_NV_READ_F(m_hPhone, unIndex, pData, iSize, &unStatus))
			return true;
	}
	ReleaseMutex(m_hPhone);
	return false;
}

int CPhone::SetPhoneID(short iModeId)
{
	int RetStatus = QLIB_FUNCTION_SUCCESSFUL;
	if (QLIB_FTM_SET_MODE_ID(m_hPhone, iModeId))
	{
		TRACE0("QLIB_FTM_SET_MODE_ID successful!!\n");
	}
	else
	{
		RetStatus = StrToInt(ERR_QLIB_FUNCTION_FAILED);
	}
	ReleaseMutex(m_hPhone);
	return RetStatus;
}

bool CPhone::GetWCDMAPhoneMode(short nMode)
{
	return (m_hPhone && QLIB_FTM_SET_MODE(m_hPhone, nMode));
}

bool CPhone::SetWCDMAWave(unsigned char iOnOff)
{
	return (m_hPhone && QLIB_FTM_CDMA_CW_WAVEFORM(m_hPhone, iOnOff));
}

bool CPhone::SetPARange(unsigned short unRange)
{
	return (m_hPhone && QLIB_FTM_SET_PA_RANGE(m_hPhone, unRange));
}

bool CPhone::SetPDM(unsigned short unPDM)
{
	return (m_hPhone && QLIB_FTM_SET_PDM(m_hPhone, 2, unPDM));
}

int CPhone::ReadGSMNV(unsigned short iItemID, unsigned char* piItemData, int iLength)
{
	unsigned short unStatus = 0;
	if (m_hPhone && QLIB_DIAG_NV_READ_F(m_hPhone, iItemID, piItemData, iLength, &unStatus))
		return true;
	ReleaseMutex(m_hPhone);
	return false;
}

int CPhone::IsFTM_Mode(unsigned char* pbIsFTMMode)
{
	int RetStatus = QLIB_FUNCTION_SUCCESSFUL;
	if (QLIB_IsFTM_Mode(m_hPhone, pbIsFTMMode))
	{
		// Nothing now
	}
	else
	{
		RetStatus = CStr::StrToInt(ERR_QLIB_FUNCTION_FAILED);
	}
	ReleaseMutex(m_hPhone);
	return RetStatus;
}

int CPhone::ChangeToFTMMode(unsigned char bFTMmode)
{
	int RetStatus = QLIB_FUNCTION_SUCCESSFUL;
	if (QLIB_ChangeFTM_ModeRuntime(m_hPhone, bFTMmode))
	{
		// Nothing now
	}
	else
	{
		RetStatus = CStr::StrToInt(ERR_QLIB_FUNCTION_FAILED);
	}
	ReleaseMutex(m_hPhone);
	return RetStatus;
}

int CPhone::CDMA2000_CW_Waveform(unsigned char bSelectCW)
{
	int RetStatus = QLIB_FUNCTION_SUCCESSFUL;
	if (QLIB_FTM_CDMA_CW_WAVEFORM(m_hPhone, bSelectCW))
	{
		// Nothing now
	}
	else
	{
		RetStatus = CStr::StrToInt(ERR_QLIB_FUNCTION_FAILED);
	}
	ReleaseMutex(m_hPhone);
	return RetStatus;
}

int CPhone::SetCommandCode(short iCommandCode)
{
	int RetStatus = QLIB_FUNCTION_SUCCESSFUL;

	if (QLIB_FTM_SET_COMMAND_CODE(m_hPhone, iCommandCode))
	{
		// Nothing now
	}
	else
	{
		RetStatus = CStr::StrToInt(ERR_QLIB_COMMAND_CODE);
	}
	ReleaseMutex(m_hPhone);
	return RetStatus;
}

bool CPhone::FTDCmdCtrl(int nPort, int nTimeout, char* szFuncName, char* pszInput, char* pszOutput)
{
	CString dllFunctionName(szFuncName);
	PFGenCmdInOut fp = (PFGenCmdInOut)::GetProcAddress(m_hFTDCmdDLL, dllFunctionName);
	if (!fp) return false;
	return (fp(nPort, nTimeout, pszInput, pszOutput) == 1);
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

CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_ReadBuckID, "FTD_ReadBuckID")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_ReadBuckfeature, "FTD_ReadBuckfeature")




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
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_OLEDTest, "FTD_OLEDTest")

CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_BacklightDriver, "FTD_BacklightDriver")



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
bool CPhone::FTD_USBTest(int nPort, int nTimeout, char* pszOutput)
{
	char in_buf[FTD_BUF_SIZE] = {0};
	return FTDCmdCtrl(nPort, nTimeout, "FTD_USBtest", in_buf, pszOutput);
}

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
//11 usb
//CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_USBTest, "FTD_USBTest")

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


CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PMid	, "FTD_PMid")	//6
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SOCversion		, "FTD_SOCversion")	//7

CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PCBVerMPP3		, "FTD_PCBVerMPP3")	//9
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PCBVerMPP2		, "FTD_PCBVerMPP2")	//10

CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_PMIid	, "FTD_PMIid")	//11
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_RFASMid	, "FTD_RFASMid")	//12

CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_Boardid	, "FTD_Boardid")	//14
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SKUid		, "FTD_SKUid")		//15
//CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SOCversion	, "FTD_SOCversion") //16
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_SOCfeature	, "FTD_SOCfeature") //17


//22-SmartCover
CPHONE_FTD_CMD_IN_OUT_CREATE(Sapporo_SCoverDetect, "Sapporo_SCoverDetect")
CPHONE_FTD_CMD_IN_OUT_CREATE(Sapporo_SCoverInterrupt, "Sapporo_SCoverInterrupt")
CPHONE_FTD_CMD_IN_OUT_CREATE(Sapporo_SCoverEnable3V, "Sapporo_SCoverEnable3V")
CPHONE_FTD_CMD_IN_OUT_CREATE(Sapporo_SCoverCheckI2C, "Sapporo_SCoverCheckI2C")
CPHONE_FTD_CMD_IN_OUT_CREATE(Sapporo_SCoverSwitch5V, "Sapporo_SCoverSwitch5V")

//90-OS
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_OSpwr2home, "FTD_OSpwr2home")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_OSgetprop, "FTD_OSgetprop")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_OSmodeSleep, "FTD_OSmodeSleep")

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

//93-misc
CPHONE_FTD_CMD_IN_OUT_CREATE(Sapporo_ftdStartEnd, "Sapporo_ftdStartEnd")
CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_Get_GPIO_Value, "FTD_Get_GPIO_Value")
