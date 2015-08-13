#include "stdafx.h"
#include <cstddef> // size_t

#include "../CmdType.h"
#include "GeneralDLLApi.h" // GeneraCmd
#include "QualDLLApi.h"
#include "../FactoryBlock.h"


static int DtCmdQcom(int port, int timeout_ms, byte* dt_cmd,
					 char* in_buf, size_t in_size, char* out_buf,
					 const char* error_code, const char* func_name)
{
	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	for (int nTry = 0; nTry < QC_DT_CMD_MAX_RETRY; nTry++) {
		if (in_buf != NULL && out_buf != NULL) {
			TRACE(_T("GeneralCmdInOut "));
			nRetCode = GeneralCmdInOut(port, timeout_ms, dt_cmd, in_buf, in_size, out_buf);
			if (nTry == 0 && nRetCode != HS_OK) {
				strncat(out_buf, error_code, strlen(error_code));
			}
		}
		else if (in_buf != NULL) {
			TRACE(_T("GeneralCmdIn "));
			size_t nInLength = (in_size == 0) ? strlen(in_buf) : in_size;
			nRetCode = GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, nInLength);
		}
		else if (out_buf != NULL) {
			TRACE(_T("GeneralCmdOut "));
			nRetCode = GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
			if (nTry == 0 && error_code != NULL && nRetCode != HS_OK) {
				strncat(out_buf, error_code, strlen(error_code));
			}
		}
		else {
			TRACE(_T("GeneralCmd "));
			nRetCode = GeneralCmd(port, timeout_ms, dt_cmd);
			if (nTry == 0 && error_code != NULL && nRetCode != HS_OK) {
				strncat(out_buf, error_code, strlen(error_code));
			}
		}
		TRACE(_T("%s(%d)\n"), func_name, nRetCode);
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

NEPDLLAPI int __stdcall QUAL_QueryMobileIDByPicasso(int port, int timeout_ms, char* ErrorCode)
{
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
	if (HS_OK != QUAL_FACTORY_FARead(port, timeout_ms, (char*)&fa_data)) {
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
NEPDLLAPI int __stdcall QUAL_PwrOn2UartPktMode(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	char* error_code = " [FT7001]";
	strncat(ErrorCode, error_code, strlen(error_code));
	return PowerOnToUARTPacketMode(port, timeout_ms, false);
}

NEPDLLAPI int __stdcall QUAL_DummyInOut(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	char* error_code = " [FT7001]";
	//strncat(ErrorCode, error_code, strlen(error_code));

	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	return nRetCode;
}

// 1-SD
NEPDLLAPI int __stdcall QUAL_SD_CheckExternal(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SD,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7001]", __FUNCTION__);
}

// 2-CAMERA
NEPDLLAPI int __stdcall QUAL_CAMERA_GetModuleId(int port, int timeout_ms, char* out_buf) // cmd: 9
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_CAMERA,9,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7002]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CAMERA_GetSensorId(int port, int timeout_ms, char* out_buf) // cmd: 10
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_CAMERA,10,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7002]", __FUNCTION__);
}

// 3-LCD
// L1-	LCD no display
// L4-  LCD no back light
// L4-C Can not switch on/off in backlight test
// L41- LCD back light has some color NG
NEPDLLAPI int __stdcall QUAL_LCD_MDDIBootSplashScreen(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LCD,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LCD_MDDIDisplayRGB(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LCD,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf), NULL, NULL, __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LCD_MDDIReset(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LCD,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LCD_MDDIInitLCD(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LCD,4,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LCD_ReadTPCoordinate(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LCD,5,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7003]", __FUNCTION__);//QUAL_LCD_ReadTPCoordinate
}

NEPDLLAPI int __stdcall QUAL_LCD_ReadTPCoordinate_new(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LCD,6,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7003]", __FUNCTION__);//QUAL_LCD_ReadTPCoordinate_new
}

NEPDLLAPI int __stdcall QUAL_LCD_ReadTPCoordinate_new_activelow(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LCD,7,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7003]", __FUNCTION__);//QUAL_LCD_ReadTPCoordinate_new_activelow
}

NEPDLLAPI int __stdcall QUAL_LCD_DisplayGIF(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LCD,8,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7003]", __FUNCTION__);
}

// 4-SENSOR
// F380 Ambient Light Sensor
// F381 G-sensor
// F382 E-compass
// F383 Proximity
// F384 Cap key
NEPDLLAPI int __stdcall QUAL_SENSOR_ALSOn(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ALSOff(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ALSRead(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_Proximity(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,4,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_PSSDEnable(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,5,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, NULL, __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_PSSDDisable(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,6,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, NULL, __FUNCTION__);//QUAL_SENSOR_PSSDDisable
}

NEPDLLAPI int __stdcall QUAL_SENSOR_GSENSORReadAcc(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,7,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_GSENSORReadID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,8,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_InitCapSenseKey(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,9,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ReadCapSenseKey(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,10,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ECompassI2CTest(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,11,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ECompassX(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,12,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ECompassY(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,13,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_ECompassZ(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,14,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_GSensorCalibration(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,16,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_SENSOR_GSensorAverage(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SENSOR,17,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}


// 5-AUDIO
NEPDLLAPI int __stdcall QUAL_AUDIO_HandsetLoopback(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_AUDIO,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, NULL, __FUNCTION__);//QUAL_AUDIO_HandsetLoopback
}

NEPDLLAPI int __stdcall QUAL_AUDIO_HeadsetLoopback(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_AUDIO,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, NULL, __FUNCTION__);//QUAL_AUDIO_HeadsetLoopback
}

NEPDLLAPI int __stdcall QUAL_AUDIO_LoudspeakerLoopback(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_AUDIO,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, NULL, __FUNCTION__);//QUAL_AUDIO_LoudspeakerLoopback
}

NEPDLLAPI int __stdcall QUAL_AUDIO_HookKeyDetection(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_AUDIO,4,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, NULL, __FUNCTION__);//QUAL_AUDIO_HookKeyDetection
}


// 6-MODEM
// F390 GSM Tx Max Power
// F391 SIM test
// F392 GPS test
NEPDLLAPI int __stdcall QUAL_MODEM_GSMPowerOn(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SIM,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7006]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_MODEM_GSMPowerOff(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SIM,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7006]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_MODEM_SIMPowerOn(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SIM,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7006]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_MODEM_SIMPowerOff(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SIM,4,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7006]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_MODEM_SIMTest(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SIM,5,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7006]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_MODEM_GPSTest(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SIM,6,0,0};
#ifdef _DEBUG
	strcpy(out_buf, "500");
	return HS_OK;
#else
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7006]", __FUNCTION__);
#endif
}

NEPDLLAPI int __stdcall QUAL_MODEM_GPS_Read_I(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SIM,7,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7006]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_MODEM_GPS_Read_Q(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_SIM,8,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7006]", __FUNCTION__);
}

// 7-BT
// F393 BT Tx Max Power
NEPDLLAPI int __stdcall QUAL_BT_BTPowerOn(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BT,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BT_TxPower_2402MHz(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BT,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BT_TxPower_2441MHz(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BT,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BT_TxPower_2480MHz(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BT,4,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BT_TxPower_2438MHz(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BT,5,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BT_FMOn(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BT,10,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BT_BTRFTest(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BT,12,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);//QUAL_BT_BTRFTest
}

NEPDLLAPI int __stdcall QUAL_BT_TxStop(int port, int timeout_ms, char* ErrorCode) // cmd: 13
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BT,13,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7007]", __FUNCTION__);//QUAL_BT_TxStop
}

NEPDLLAPI int __stdcall QUAL_BT_FMReadRSSI(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BT,14,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, in_buf, 0, out_buf, " [FT7007]", __FUNCTION__);
}


// 8-FLASH
NEPDLLAPI int __stdcall QUAL_FLASH_ReadWriteLoopTest(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_FLASH,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7008]", __FUNCTION__);//QUAL_FLASH_ReadWriteLoopTest
}

NEPDLLAPI int __stdcall QUAL_FLASH_ReadPage(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_FLASH,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7008]", __FUNCTION__);//QUAL_FLASH_ReadPage
}

NEPDLLAPI int __stdcall QUAL_FLASH_WritePage(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_FLASH,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7008]", __FUNCTION__);//QUAL_FLASH_WritePage
}

// F226 HWID version error
NEPDLLAPI int __stdcall QUAL_FLASH_ReadID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_FLASH,4,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7008]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_FLASH_EraceBlock(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_FLASH,5,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7008]", __FUNCTION__);//QUAL_FLASH_EraceBlock
}

NEPDLLAPI int __stdcall QUAL_FLASH_FindImageStartingPage(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_FLASH,6,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, in_buf, 0, out_buf, " [FT7008]", __FUNCTION__);//QUAL_FLASH_FindImageStartingPage
}

NEPDLLAPI int __stdcall QUAL_FLASH_MCPTest(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_FLASH,7,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7008]", __FUNCTION__);//QUAL_FLASH_MCPTest
}


// 9-DDR
NEPDLLAPI int __stdcall QUAL_DDR_SetEBI1Clkto99MHZ(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_DDR,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7009]", __FUNCTION__);//QUAL_DDR_SetEBI1Clkto99MHZ
}

NEPDLLAPI int __stdcall QUAL_DDR_SetEBI1Clkto128MHZ(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_DDR,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7009]", __FUNCTION__);//QUAL_DDR_SetEBI1Clkto128MHZ
}

NEPDLLAPI int __stdcall QUAL_DDR_ReadWriteTest(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_DDR,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7009]", __FUNCTION__);//QUAL_DDR_ReadWriteTest
}

NEPDLLAPI int __stdcall QUAL_DDR_ReadWriteLoopTest(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_DDR,4,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7009]", __FUNCTION__);//QUAL_DDR_ReadWriteLoopTest
}

NEPDLLAPI int __stdcall QUAL_DDR_ReadAddr(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_DDR,5,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7009]", __FUNCTION__);//QUAL_DDR_ReadAddr
}

NEPDLLAPI int __stdcall QUAL_DDR_RoughTest(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_DDR,7,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7009]", __FUNCTION__);//QUAL_DDR_RoughTest
}

// 10-RTC
// E5 Alarm setting fail (RTC)
NEPDLLAPI int __stdcall QUAL_RTC_ReadYear(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_ReadMonth(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_ReadDay(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_ReadHour(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,4,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_ReadMinute(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,5,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_ReadSecond(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,6,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_ReadDayOfWeek(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,7,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7010]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteYear(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,8,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteMonth(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,9,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteDay(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,10,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteHour(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,11,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteMinute(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,12,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteSecond(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,13,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall QUAL_RTC_WriteDayOfWeek(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,14,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall QUAL_RTC_Alert(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,15,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall QUAL_RTC_Reset_Second(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RTC,16,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [F396]", __FUNCTION__);
}

// 11-USB

// 12-CHARGER
// F332 Battery ID failed
// F372 Charging failed of BB test
// F228 USB CHARGE Fail
// E4   Can not charge
// E41  Coin Li-ion battery can't charge
NEPDLLAPI int __stdcall QUAL_CHARGER_PreCharge(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_CHARGER,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_FullCharge(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_CHARGER,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_StopCharge(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_CHARGER,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_Verify_IC(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_CHARGER,4,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_Verify_TPS65023(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_CHARGER,5,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_EnableCharing(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_CHARGER,6,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_ReadVersion(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_CHARGER,7,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7011]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_CHARGER_500mACharge(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_CHARGER,8,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7011]", __FUNCTION__);
}

// 13-KEYPAD
NEPDLLAPI int __stdcall QUAL_KEYPAD_Detect(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_KEYPAD,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7013]", __FUNCTION__);//QUAL_KEYPAD_Detect
}


// 14-VIBRATOR
// F370 Vibrator failed of BB test
NEPDLLAPI int __stdcall QUAL_VIBRATOR_On(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_VIBRATOR,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7014]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_VIBRATOR_Off(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_VIBRATOR,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7014]", __FUNCTION__);
}


// 15-BATTERY
// F334	Battery failed
// F372 Charging failed of BB test
// F228 USB CHARGE Fail
// E4   Can not charge
// E41  Coin Li-ion battery can't charge
// F396 Backup battery open
// F397 Backup battery short
NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadTemperature(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BATTERY,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadVoltage(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BATTERY,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadStateOfCharge(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BATTERY,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadCurrent(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BATTERY,4,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_Reset(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BATTERY,5,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_BackupBatteryPowerLevel(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BATTERY,6,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_BackupBatteryChargerOn(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BATTERY,7,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_BackupBatteryChargerOff(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BATTERY,8,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7015]", __FUNCTION__);
}

// F385 Battery Cover Detect
NEPDLLAPI int __stdcall QUAL_BATTERY_BatteryCoverDetect(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BATTERY,9,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadVersion(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BATTERY,11,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadDFIVersion(int port, int timeout_ms, char* out_buf) // cmd: 12
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_BATTERY,12,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

// 16-DOWNLOAD
NEPDLLAPI int __stdcall QUAL_DOWNLOAD_FASTBOOT_MODE(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_DOWNLOAD,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7016]", __FUNCTION__);//QUAL_DOWNLOAD_FASTBOOT_MODE
}

NEPDLLAPI int __stdcall QUAL_DOWNLOAD_NewFlashWrite0XFF(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_DOWNLOAD,2,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall QUAL_DOWNLOAD_NewFlashRead(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_DOWNLOAD,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7016]", __FUNCTION__);//QUAL_DOWNLOAD_NewFlashRead
}


// 17-OS
NEPDLLAPI int __stdcall QUAL_OS_JumpToAPPSBL(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_OS,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7017]", __FUNCTION__);//QUAL_OS_JumpToAPPSBL
}


// 18-WLAN
// F395 WLAN Tx Max
NEPDLLAPI int __stdcall QUAL_WLAN_TxMaxPower(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_WLAN,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_WLAN_TxMaxPower_CH01_2412MHz(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_WLAN,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_WLAN_TxMaxPower_CH07_2442MHz(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_WLAN,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_WLAN_TxMaxPower_CH13_2472MHz(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_WLAN,4,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7018]", __FUNCTION__);
}

// 19-LED
// L3   Keypad LED no good
// L31  Indicator LED no good
// L4-C Can not switch on/off in backlight test
// L3   Keypad LED no good
// L5-  Back light is too dark, LED light not uniform or black mark
// L6-	LED wrong color
// L7-  Flash light no function
NEPDLLAPI int __stdcall QUAL_LED_KeypadOn(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LED,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_KeypadOff(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LED,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_BacklightOn(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LED,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_BacklightOff(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LED,4,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_FlashOn(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LED,5,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_FlashOff(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LED,6,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_CapSenseKeyOn(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LED,7,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_LED_CapSenseKeyOff(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_LED,8,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7019]", __FUNCTION__);
}


// 20-ID
// F226 HWID version error
NEPDLLAPI int __stdcall QUAL_ID_ReadBoardID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_ID,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7020]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_ID_ReadHWID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_ID,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7020]", __FUNCTION__);
}


// 23-RD
NEPDLLAPI int __stdcall QUAL_RD_LoadAPPSBL(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_LoadAPPSBL
}

NEPDLLAPI int __stdcall QUAL_RD_EnterUARTPktMode(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_EnterUARTPktMode
}

NEPDLLAPI int __stdcall QUAL_RD_EVT0AutoTest(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_EVT0AutoTest
}

NEPDLLAPI int __stdcall QUAL_RD_SendSMEMCmd(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,4,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_SendSMEMCmd
}

NEPDLLAPI int __stdcall QUAL_RD_AudioAMP(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,5,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_AudioAMP
}

NEPDLLAPI int __stdcall QUAL_RD_ReadMemoryAddress(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,6,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7023]", __FUNCTION__);//QUAL_RD_ReadMemoryAddress
}

NEPDLLAPI int __stdcall QUAL_RD_ChangeHDMIAddress(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,7,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_ChangeHDMIAddress
}

NEPDLLAPI int __stdcall QUAL_RD_ReadHDMIAddress(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,8,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7023]", __FUNCTION__);//QUAL_RD_ReadHDMIAddress
}

NEPDLLAPI int __stdcall QUAL_RD_GetPartitionInfo(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,9,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7023]", __FUNCTION__);//QUAL_RD_GetPartitionInfo
}

NEPDLLAPI int __stdcall QUAL_RD_JumpToAPPSBL_new(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,10,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_JumpToAPPSBL_new
}

NEPDLLAPI int __stdcall QUAL_RD_Reboot(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,11,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_Reboot
}

NEPDLLAPI int __stdcall QUAL_RD_SetGP2Vol(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,12,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));//QUAL_RD_SetGP2Vol
}

NEPDLLAPI int __stdcall QUAL_RD_Enter301Mode(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,13,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_Enter301Mode
}

NEPDLLAPI int __stdcall QUAL_RD_JumpToAPPSBL_WriteFlag(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,14,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_JumpToAPPSBL_WriteFlag
}

NEPDLLAPI int __stdcall QUAL_RD_ReadI2CStatus(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,15,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7023]", __FUNCTION__);//QUAL_RD_ReadI2CStatus
}

NEPDLLAPI int __stdcall QUAL_RD_I2CStressTest(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,16,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_I2CStressTest
}

NEPDLLAPI int __stdcall QUAL_RD_GPIO7678_High(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,17,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_GPIO7678_High
}

NEPDLLAPI int __stdcall QUAL_RD_GPIO7678_Low(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,18,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_GPIO7678_Low
}

NEPDLLAPI int __stdcall QUAL_RD_LoopReadTouchCoordinate(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,19,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_LoopReadTouchCoordinate
}

NEPDLLAPI int __stdcall QUAL_RD_PowerDown(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,20,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_PowerDown
}

NEPDLLAPI int __stdcall QUAL_RD_HdmiTest(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,21,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_HdmiTest
}

NEPDLLAPI int __stdcall QUAL_RD_PictureTest(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,22,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_PictureTest
}

NEPDLLAPI int __stdcall QUAL_RD_RecoveryMenu(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,23,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_RecoveryMenu
}

NEPDLLAPI int __stdcall QUAL_RD_AutoSetEbi1KernelAddr(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,24,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, ErrorCode, " [FT7023]", __FUNCTION__);//QUAL_RD_AutoSetEbi1KernelAddr
}

NEPDLLAPI int __stdcall QUAL_RD_ReadUartLogStatus(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_RD,25,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7023]", __FUNCTION__);//QUAL_RD_ReadUartLogStatus
}

// 25-VERSION
// F211 SW version error
// F226 HWID version error
NEPDLLAPI int __stdcall QUAL_VERSION_ReadDTVersion(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_VERSION,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7025]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_VERSION_ReadBoardID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_VERSION,2,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7025]", __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_VERSION_ReadAccessaryID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_VERSION,3,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, " [FT7025]", __FUNCTION__);
}

// 26-FACTORY
// F398 PCBA version error
NEPDLLAPI int __stdcall QUAL_FACTORY_FARead(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_FA,1,0,0};
	return DtCmdQcom(port, timeout_ms, dt_cmd, NULL, 0, out_buf, NULL, __FUNCTION__);
}

NEPDLLAPI int __stdcall QUAL_FACTORY_FAWrite(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {QC_DT_FA,2,0,0};
	// FIXME: only send 384 byte, all data is 512
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, 384);
}

