#include "stdafx.h"
#include <cstddef> // size_t

#include "../CmdType.h"
#include "GeneralDLLApi.h" // GeneraCmd
#include "QualDLLApi.h"
#include "../FactoryBlock.h"


static int DtCmdQcom(int nPort, int nTimeout, char* command,
					 char* InBuffer, size_t InSize, char* OutBuffer,
					 const char* error_code, const char* Func)
{
	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	for (int nTry = 0; nTry < QC_DT_CMD_MAX_RETRY; nTry++) {
		if (InBuffer != NULL && OutBuffer != NULL) {
			TRACE(_T("GeneralCmdInOut "));
			nRetCode = GeneralCmdInOut(nPort, nTimeout, command, InBuffer, InSize, OutBuffer);
			if (nTry == 0 && nRetCode != HS_OK) {
				strncat(OutBuffer, error_code, strlen(error_code));
			}
		}
		else if (InBuffer != NULL) {
			TRACE(_T("GeneralCmdIn "));
			size_t nInLength = (InSize == 0) ? strlen(InBuffer) : InSize;
			nRetCode = GeneralCmdIn(nPort, nTimeout, command, InBuffer, nInLength);
		}
		else if (OutBuffer != NULL) {
			TRACE(_T("GeneralCmdOut "));
			nRetCode = GeneralCmdOut(nPort, nTimeout, command, OutBuffer);
			if (nTry == 0 && error_code != NULL && nRetCode != HS_OK) {
				strncat(OutBuffer, error_code, strlen(error_code));
			}
		}
		else {
			TRACE(_T("GeneralCmd "));
			nRetCode = GeneralCmd(nPort, nTimeout, command);
			if (nTry == 0 && error_code != NULL && nRetCode != HS_OK) {
				strncat(OutBuffer, error_code, strlen(error_code));
			}
		}
		TRACE(_T("%s(%d)\n"), Func, nRetCode);
		if (nRetCode == HS_OK) {
			break;
		}
		else {
			Sleep(500);
		}
	}

	return nRetCode;
}

typedef bool (__cdecl *QueryMobileIDByPicassoType)(char*, char*, char*, char*, char*);
int QUAL_UpdateMobileIDByPicasso(char* szModelName, char* szPicasso, CString sMobileId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	int nRetCode = HS_OK;
	CString sDll;
	::GetModuleFileName(NULL, sDll.GetBuffer(MAX_PATH), MAX_PATH);
	sDll.ReleaseBuffer();
	sDll = sDll.Left(sDll.ReverseFind('\\'));
	sDll += _T("\\Pics_SSSO2_Factory_StaticM.dll");
	HMODULE hmLib = ::LoadLibrary(sDll);
	if (hmLib == NULL) {
		CString msg;
		msg.Format(_T("Load: %s Fail!"), sDll);
		AfxMessageBox(msg);
		return -1*ERROR_DLL_INIT_FAILED;
	}
	QueryMobileIDByPicassoType pFnUpdateMobileIDByPicasso = (QueryMobileIDByPicassoType)GetProcAddress(hmLib, "UpdateMobileIDByPicasso_SSS02");
	if (pFnUpdateMobileIDByPicasso == NULL) {
		::FreeLibrary(hmLib);
		hmLib = NULL;
		CString message;
		message = _T("Get DLL Function: UpdateMobileIDByPicasso_SSS02 Fail!");
		AfxMessageBox(message);
		return -1*ERROR_FUNCTION_NOT_CALLED;
	}
	char szMobileId[32] = {0};
	strcpy(szMobileId, sMobileId);
	char ostrErrCode[1024];
	char ostrErrMsg[1024];
	if ( !pFnUpdateMobileIDByPicasso(szModelName, szPicasso, szMobileId, ostrErrCode, ostrErrMsg) ) {
		char error_string[512] = {0};
		strcat(error_string, "[");
		strncat(error_string, ostrErrCode, strlen(ostrErrCode));
		strcat(error_string, "] ");
		strncat(error_string, ostrErrMsg, strlen(ostrErrMsg));
		nRetCode = -1*ERROR_INVALID_DATA;
		CString err_msg;
		err_msg.Format(_T("Fail to update Mobile ID: %s."), error_string);
		::AfxMessageBox(err_msg);
	}
	if (hmLib) {
		::FreeLibrary(hmLib);
		hmLib = NULL;
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall QUAL_QueryMobileIDByPicasso(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	int nRetCode = HS_OK;
	CString sDll;
	::GetModuleFileName(NULL, sDll.GetBuffer(MAX_PATH), MAX_PATH);
	sDll.ReleaseBuffer();
	sDll = sDll.Left(sDll.ReverseFind('\\'));
	sDll += _T("\\Pics_SSSO2_Factory_StaticM.dll");
	HMODULE hmLib = ::LoadLibrary(sDll);
	if (hmLib == NULL) {
		CString msg;
		msg.Format(_T("Load: %s Fail!"), sDll);
		AfxMessageBox(msg);
		return -1*ERROR_DLL_INIT_FAILED;
	}
	QueryMobileIDByPicassoType pFnQueryMobileIDByPicasso = (QueryMobileIDByPicassoType)GetProcAddress(hmLib, "QueryMobileIDByPicasso_SSS02");
	if (pFnQueryMobileIDByPicasso == NULL) {
		::FreeLibrary(hmLib);
		hmLib = NULL;
		CString message;
		message = _T("Get DLL Function: QueryMobileIDByPicasso_SSS02 Fail!");
		AfxMessageBox(message);
		return -1*ERROR_FUNCTION_NOT_CALLED;
	}

	const int PAYLOAD_SIZE = 4096;
	char szModelName[32] = {0};
	char szPicasso[PAYLOAD_SIZE] = {0};
	char ostrMobileId[32];
	char ostrErrCode[1024];
	char ostrErrMsg[1024];

	unsigned char fa_data[4096];
	if (HS_OK != QUAL_FACTORY_FARead(nPort, nTimeout, (char*)&fa_data)) {
		CString message;
		message = _T("Read FA Fail!");
		AfxMessageBox(message);
		return -1*ERROR_FUNCTION_FAILED;;
	}
	memcpy(szPicasso, &fa_data[FA_PICASSO_NO_START], 10);
	CString sModelName = _T("GSUMB1A");
	strncpy(szModelName, sModelName, sModelName.GetLength());
	if ( pFnQueryMobileIDByPicasso(szModelName, szPicasso, ostrMobileId, ostrErrCode, ostrErrMsg) ) {
		char* szMobileId[11] = {0};
		memcpy(szMobileId, &fa_data[FA_MOBILE_ID_START], 11);
		CString mobile_id_db = ostrMobileId;
		CString mobile_id_device;
		mobile_id_device.Format(_T("%x%x%x%x%x%x%x%x%x%x%x"), szMobileId[0], 
			szMobileId[1], szMobileId[2], szMobileId[3], szMobileId[4], szMobileId[5], 
			szMobileId[6], szMobileId[7], szMobileId[8], szMobileId[9], szMobileId[10]);
		if (mobile_id_db.Find(_T("0000000000000000000000")) != -1) {
			// picasso is not used, update mobile id to server
			if (HS_OK != QUAL_UpdateMobileIDByPicasso(szModelName, szPicasso, mobile_id_device)) {
				nRetCode = -1*ERROR_BAD_COMMAND;
			}
		}
		else {
			// picasso is used, then check mobile id between device and db
			if (strcmp(mobile_id_device, mobile_id_db) != 0) {
				// picasso reused
				nRetCode = -1*ERROR_NOT_SAME_DEVICE;
				::AfxMessageBox(_T("Check mobile id fail. Picasso is reused!!!"));
			}
		}
	} 
	else {
		char error_string[512] = {0};
		strcat(error_string, "[");
		strncat(error_string, ostrErrCode, strlen(ostrErrCode));
		strcat(error_string, "] ");
		strncat(error_string, ostrErrMsg, strlen(ostrErrMsg));
		nRetCode = -1*ERROR_INVALID_DATA;
		CString err_msg;
		err_msg.Format(_T("Fail to query Mobile ID: %s."), error_string);
		::AfxMessageBox(err_msg);
	}
	if (hmLib) {
		::FreeLibrary(hmLib);
		hmLib = NULL;
	}

	return nRetCode;
}

// 2009-10-27 Mick: power on to DT UART packet mode
NEPDLLAPI int __stdcall QUAL_PwrOn2UartPktMode(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " [FT7001]";
	strncat(ErrorCode, error_code, strlen(error_code));
	return PowerOnToUARTPacketMode(nPort, nTimeout, false);
}

NEPDLLAPI int __stdcall QUAL_DummyInOut(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " [FT7001]";
	//strncat(ErrorCode, error_code, strlen(error_code));

	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	return nRetCode;
}

// 1-SD
NEPDLLAPI int __stdcall QUAL_SD_CheckExternal(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SD,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7001]", __FUNCTION__);
}


// 2-CAMERA
NEPDLLAPI int __stdcall QUAL_CAMERA_GetModuleId(int nPort, int nTimeout, char* OutBuffer) // cmd: 9
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_CAMERA,9,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7002]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CAMERA_GetSensorId(int nPort, int nTimeout, char* OutBuffer) // cmd: 10
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_CAMERA,10,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7002]", __FUNCTION__);
}


// 3-LCD
// L1-	LCD no display
// L4-  LCD no back light
// L4-C Can not switch on/off in backlight test
// L41- LCD back light has some color NG
NEPDLLAPI int __stdcall QUAL_LCD_MDDIBootSplashScreen(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LCD,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LCD_MDDIDisplayRGB(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LCD,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, InBuffer, strlen(InBuffer), NULL, NULL, __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LCD_MDDIReset(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LCD,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LCD_MDDIInitLCD(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LCD,4,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LCD_ReadTPCoordinate(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LCD,5,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7003]", __FUNCTION__);//QUAL_LCD_ReadTPCoordinate
}

NEPDLLAPI int __stdcall QUAL_LCD_ReadTPCoordinate_new(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LCD,6,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7003]", __FUNCTION__);//QUAL_LCD_ReadTPCoordinate_new
}

NEPDLLAPI int __stdcall QUAL_LCD_ReadTPCoordinate_new_activelow(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LCD,7,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7003]", __FUNCTION__);//QUAL_LCD_ReadTPCoordinate_new_activelow
}

NEPDLLAPI int __stdcall QUAL_LCD_DisplayGIF(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LCD,8,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7003]", __FUNCTION__);
}

// 4-SENSOR
// F380 Ambient Light Sensor
// F381 G-sensor 
// F382 E-compass
// F383 Proximity
// F384 Cap key
NEPDLLAPI int __stdcall QUAL_SENSOR_ALSOn(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ALSOff(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ALSRead(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_Proximity(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,4,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_PSSDEnable(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,5,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, NULL, __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_PSSDDisable(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,6,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, NULL, __FUNCTION__);//QUAL_SENSOR_PSSDDisable
}

NEPDLLAPI int __stdcall QUAL_SENSOR_GSENSORReadAcc(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,7,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_GSENSORReadID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,8,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_InitCapSenseKey(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,9,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ReadCapSenseKey(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,10,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ECompassI2CTest(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,11,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ECompassX(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,12,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ECompassY(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,13,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ECompassZ(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,14,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_GSensorCalibration(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,16,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_GSensorAverage(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SENSOR,17,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}


// 5-AUDIO
NEPDLLAPI int __stdcall QUAL_AUDIO_HandsetLoopback(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_AUDIO,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, NULL, __FUNCTION__);//QUAL_AUDIO_HandsetLoopback
}

NEPDLLAPI int __stdcall QUAL_AUDIO_HeadsetLoopback(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_AUDIO,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, NULL, __FUNCTION__);//QUAL_AUDIO_HeadsetLoopback
}

NEPDLLAPI int __stdcall QUAL_AUDIO_LoudspeakerLoopback(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_AUDIO,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, NULL, __FUNCTION__);//QUAL_AUDIO_LoudspeakerLoopback
}

NEPDLLAPI int __stdcall QUAL_AUDIO_HookKeyDetection(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_AUDIO,4,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, NULL, __FUNCTION__);//QUAL_AUDIO_HookKeyDetection
}


// 6-MODEM
// F390 GSM Tx Max Power
// F391 SIM test
// F392 GPS test
NEPDLLAPI int __stdcall QUAL_MODEM_GSMPowerOn(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SIM,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7006]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_MODEM_GSMPowerOff(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SIM,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7006]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_MODEM_SIMPowerOn(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SIM,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7006]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_MODEM_SIMPowerOff(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SIM,4,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7006]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_MODEM_SIMTest(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SIM,5,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7006]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_MODEM_GPSTest(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SIM,6,0,0};
#ifdef _DEBUG
	strcpy(OutBuffer, "500");
	return HS_OK;
#else
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7006]", __FUNCTION__);
#endif
}

NEPDLLAPI int __stdcall QUAL_MODEM_GPS_Read_I(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SIM,7,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7006]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_MODEM_GPS_Read_Q(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_SIM,8,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7006]", __FUNCTION__);
}

// 7-BT
// F393 BT Tx Max Power
NEPDLLAPI int __stdcall QUAL_BT_BTPowerOn(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BT,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BT_TxPower_2402MHz(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BT,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BT_TxPower_2441MHz(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BT,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BT_TxPower_2480MHz(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BT,4,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BT_TxPower_2438MHz(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BT,5,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BT_FMOn(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BT,10,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BT_BTRFTest(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BT,12,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);//QUAL_BT_BTRFTest
}

NEPDLLAPI int __stdcall QUAL_BT_TxStop(int nPort, int nTimeout, char* ErrorCode) // cmd: 13
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BT,13,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);//QUAL_BT_TxStop
}

NEPDLLAPI int __stdcall QUAL_BT_FMReadRSSI(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BT,14,0,0};
	return DtCmdQcom(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7007]", __FUNCTION__);
}


// 8-FLASH
NEPDLLAPI int __stdcall QUAL_FLASH_ReadWriteLoopTest(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_FLASH,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7008]", __FUNCTION__);//QUAL_FLASH_ReadWriteLoopTest
}

NEPDLLAPI int __stdcall QUAL_FLASH_ReadPage(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_FLASH,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7008]", __FUNCTION__);//QUAL_FLASH_ReadPage
}

NEPDLLAPI int __stdcall QUAL_FLASH_WritePage(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_FLASH,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7008]", __FUNCTION__);//QUAL_FLASH_WritePage
}

// F226 HWID version error
NEPDLLAPI int __stdcall QUAL_FLASH_ReadID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_FLASH,4,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7008]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_FLASH_EraceBlock(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_FLASH,5,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7008]", __FUNCTION__);//QUAL_FLASH_EraceBlock
}

NEPDLLAPI int __stdcall QUAL_FLASH_FindImageStartingPage(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_FLASH,6,0,0};
	return DtCmdQcom(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7008]", __FUNCTION__);//QUAL_FLASH_FindImageStartingPage
}

NEPDLLAPI int __stdcall QUAL_FLASH_MCPTest(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_FLASH,7,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7008]", __FUNCTION__);//QUAL_FLASH_MCPTest
}


// 9-DDR
NEPDLLAPI int __stdcall QUAL_DDR_SetEBI1Clkto99MHZ(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_DDR,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7009]", __FUNCTION__);//QUAL_DDR_SetEBI1Clkto99MHZ
}

NEPDLLAPI int __stdcall QUAL_DDR_SetEBI1Clkto128MHZ(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_DDR,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7009]", __FUNCTION__);//QUAL_DDR_SetEBI1Clkto128MHZ
}

NEPDLLAPI int __stdcall QUAL_DDR_ReadWriteTest(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_DDR,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7009]", __FUNCTION__);//QUAL_DDR_ReadWriteTest
}

NEPDLLAPI int __stdcall QUAL_DDR_ReadWriteLoopTest(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_DDR,4,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7009]", __FUNCTION__);//QUAL_DDR_ReadWriteLoopTest
}

NEPDLLAPI int __stdcall QUAL_DDR_ReadAddr(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_DDR,5,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7009]", __FUNCTION__);//QUAL_DDR_ReadAddr
}

NEPDLLAPI int __stdcall QUAL_DDR_RoughTest(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_DDR,7,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7009]", __FUNCTION__);//QUAL_DDR_RoughTest
}

// 10-RTC
// E5 Alarm setting fail (RTC)
NEPDLLAPI int __stdcall QUAL_RTC_ReadYear(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_ReadMonth(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_ReadDay(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_ReadHour(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,4,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_ReadMinute(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,5,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_ReadSecond(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,6,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_ReadDayOfWeek(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,7,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteYear(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,8,0,0};
	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteMonth(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,9,0,0};
	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteDay(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,10,0,0};
	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteHour(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,11,0,0};
	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteMinute(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,12,0,0};
	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteSecond(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,13,0,0};
	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteDayOfWeek(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,14,0,0};
	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall QUAL_RTC_Alert(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,15,0,0};
	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall QUAL_RTC_Reset_Second(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RTC,16,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [F396]", __FUNCTION__);
}

// 11-USB

// 12-CHARGER
// F332 Battery ID failed
// F372 Charging failed of BB test
// F228 USB CHARGE Fail
// E4   Can not charge
// E41  Coin Li-ion battery can't charge
NEPDLLAPI int __stdcall QUAL_CHARGER_PreCharge(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_CHARGER,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_FullCharge(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_CHARGER,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_StopCharge(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_CHARGER,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_Verify_IC(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_CHARGER,4,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_Verify_TPS65023(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_CHARGER,5,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_EnableCharing(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_CHARGER,6,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_ReadVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_CHARGER,7,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_500mACharge(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_CHARGER,8,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7011]", __FUNCTION__);
}

// 13-KEYPAD
NEPDLLAPI int __stdcall QUAL_KEYPAD_Detect(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_KEYPAD,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7013]", __FUNCTION__);//QUAL_KEYPAD_Detect
}


// 14-VIBRATOR
// F370 Vibrator failed of BB test
NEPDLLAPI int __stdcall QUAL_VIBRATOR_On(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_VIBRATOR,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7014]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_VIBRATOR_Off(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_VIBRATOR,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7014]", __FUNCTION__);
}


// 15-BATTERY
// F334	Battery failed
// F372 Charging failed of BB test
// F228 USB CHARGE Fail
// E4   Can not charge
// E41  Coin Li-ion battery can't charge
// F396 Backup battery open
// F397 Backup battery short
NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadTemperature(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BATTERY,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadVoltage(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BATTERY,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadStateOfCharge(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BATTERY,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadCurrent(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BATTERY,4,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_Reset(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BATTERY,5,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_BackupBatteryPowerLevel(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BATTERY,6,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_BackupBatteryChargerOn(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BATTERY,7,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_BackupBatteryChargerOff(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BATTERY,8,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7015]", __FUNCTION__);
}

// F385 Battery Cover Detect
NEPDLLAPI int __stdcall QUAL_BATTERY_BatteryCoverDetect(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BATTERY,9,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BATTERY,11,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadDFIVersion(int nPort, int nTimeout, char* OutBuffer) // cmd: 12
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_BATTERY,12,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

// 16-DOWNLOAD
NEPDLLAPI int __stdcall QUAL_DOWNLOAD_FASTBOOT_MODE(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_DOWNLOAD,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7016]", __FUNCTION__);//QUAL_DOWNLOAD_FASTBOOT_MODE
}

NEPDLLAPI int __stdcall QUAL_DOWNLOAD_NewFlashWrite0XFF(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_DOWNLOAD,2,0,0};
	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall QUAL_DOWNLOAD_NewFlashRead(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_DOWNLOAD,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7016]", __FUNCTION__);//QUAL_DOWNLOAD_NewFlashRead
}


// 17-OS
NEPDLLAPI int __stdcall QUAL_OS_JumpToAPPSBL(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_OS,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7017]", __FUNCTION__);//QUAL_OS_JumpToAPPSBL
}


// 18-WLAN
// F395 WLAN Tx Max
NEPDLLAPI int __stdcall QUAL_WLAN_TxMaxPower(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_WLAN,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_WLAN_TxMaxPower_CH01_2412MHz(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_WLAN,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_WLAN_TxMaxPower_CH07_2442MHz(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_WLAN,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_WLAN_TxMaxPower_CH13_2472MHz(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_WLAN,4,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7018]", __FUNCTION__);
}

// 19-LED
// L3   Keypad LED no good
// L31  Indicator LED no good
// L4-C Can not switch on/off in backlight test
// L3   Keypad LED no good
// L5-  Back light is too dark, LED light not uniform or black mark
// L6-	LED wrong color
// L7-  Flash light no function
NEPDLLAPI int __stdcall QUAL_LED_KeypadOn(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LED,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_KeypadOff(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LED,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_BacklightOn(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LED,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_BacklightOff(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LED,4,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_FlashOn(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LED,5,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_FlashOff(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LED,6,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_CapSenseKeyOn(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LED,7,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_CapSenseKeyOff(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_LED,8,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}


// 20-ID
// F226 HWID version error
NEPDLLAPI int __stdcall QUAL_ID_ReadBoardID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_ID,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7020]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_ID_ReadHWID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_ID,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7020]", __FUNCTION__);
}


// 23-RD
NEPDLLAPI int __stdcall QUAL_RD_LoadAPPSBL(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_LoadAPPSBL
}

NEPDLLAPI int __stdcall QUAL_RD_EnterUARTPktMode(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_EnterUARTPktMode
}

NEPDLLAPI int __stdcall QUAL_RD_EVT0AutoTest(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_EVT0AutoTest
}

NEPDLLAPI int __stdcall QUAL_RD_SendSMEMCmd(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,4,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_SendSMEMCmd
}

NEPDLLAPI int __stdcall QUAL_RD_AudioAMP(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,5,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_AudioAMP
}

NEPDLLAPI int __stdcall QUAL_RD_ReadMemoryAddress(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,6,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7023]", __FUNCTION__);//QUAL_RD_ReadMemoryAddress
}

NEPDLLAPI int __stdcall QUAL_RD_ChangeHDMIAddress(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,7,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_ChangeHDMIAddress
}

NEPDLLAPI int __stdcall QUAL_RD_ReadHDMIAddress(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,8,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7023]", __FUNCTION__);//QUAL_RD_ReadHDMIAddress
}

NEPDLLAPI int __stdcall QUAL_RD_GetPartitionInfo(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,9,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7023]", __FUNCTION__);//QUAL_RD_GetPartitionInfo
}

NEPDLLAPI int __stdcall QUAL_RD_JumpToAPPSBL_new(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,10,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_JumpToAPPSBL_new
}

NEPDLLAPI int __stdcall QUAL_RD_Reboot(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,11,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_Reboot
}

NEPDLLAPI int __stdcall QUAL_RD_SetGP2Vol(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,12,0,0};
	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));//QUAL_RD_SetGP2Vol
}

NEPDLLAPI int __stdcall QUAL_RD_Enter301Mode(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,13,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_Enter301Mode
}

NEPDLLAPI int __stdcall QUAL_RD_JumpToAPPSBL_WriteFlag(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,14,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_JumpToAPPSBL_WriteFlag
}

NEPDLLAPI int __stdcall QUAL_RD_ReadI2CStatus(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,15,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7023]", __FUNCTION__);//QUAL_RD_ReadI2CStatus
}

NEPDLLAPI int __stdcall QUAL_RD_I2CStressTest(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,16,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_I2CStressTest
}

NEPDLLAPI int __stdcall QUAL_RD_GPIO7678_High(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,17,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_GPIO7678_High
}

NEPDLLAPI int __stdcall QUAL_RD_GPIO7678_Low(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,18,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_GPIO7678_Low
}

NEPDLLAPI int __stdcall QUAL_RD_LoopReadTouchCoordinate(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,19,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_LoopReadTouchCoordinate
}

NEPDLLAPI int __stdcall QUAL_RD_PowerDown(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,20,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_PowerDown
}

NEPDLLAPI int __stdcall QUAL_RD_HdmiTest(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,21,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_HdmiTest
}

NEPDLLAPI int __stdcall QUAL_RD_PictureTest(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,22,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_PictureTest
}

NEPDLLAPI int __stdcall QUAL_RD_RecoveryMenu(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,23,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_RecoveryMenu
}

NEPDLLAPI int __stdcall QUAL_RD_AutoSetEbi1KernelAddr(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,24,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_AutoSetEbi1KernelAddr
}

NEPDLLAPI int __stdcall QUAL_RD_ReadUartLogStatus(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_RD,25,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7023]", __FUNCTION__);//QUAL_RD_ReadUartLogStatus
}

// 25-VERSION
// F211 SW version error
// F226 HWID version error
NEPDLLAPI int __stdcall QUAL_VERSION_ReadDTVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_VERSION,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7025]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_VERSION_ReadBoardID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_VERSION,2,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7025]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_VERSION_ReadAccessaryID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_VERSION,3,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7025]", __FUNCTION__);
}

// 26-FACTORY
// F398 PCBA version error
NEPDLLAPI int __stdcall QUAL_FACTORY_FARead(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_FA,1,0,0};
	return DtCmdQcom(nPort, nTimeout, command, NULL, 0, OutBuffer, NULL, __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_FACTORY_FAWrite(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {QC_DT_FA,2,0,0};
	// FIXME: only send 384 byte, all data is 512
	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, 384);
}

