#include "stdafx.h"
#include "USB4702.h"
#pragma comment(lib, "adsapi/Adsapi32.lib")


static LONG lDriverHandle;           //Driver handle

static PT_DioWritePortByte      ptDioWritePortByte;      // DioWriteByte table
static PT_DioGetCurrentDOByte   ptDioGetCurrentDOByte;
static char    szErrMsg[80];             // used for MESSAGEBOX function
static LRESULT ErrCde;                   // return error code
static PT_AIVoltageIn ptAIVolIn;       //for ADC voltage in
static PT_AOVoltageOut ptAOVolOut;
//PT_AIConfig ptAIConfig;         //for ADC config

//Initial USB Board
LRESULT UB_init()
{
	//initial setting
	ptDioWritePortByte.port = 0;
	ptDioWritePortByte.mask = 0xFF;
	ptDioGetCurrentDOByte.port = 0;

	ErrCde = DRV_DeviceOpen(0, (LONG far*) & lDriverHandle);

#ifdef MSG_DIALOG_DISPLAY
	if (ErrCde != SUCCESS) {
		DRV_GetErrorMessage(ErrCde, (LPSTR)szErrMsg);
		Application->MessageBox((char *)szErrMsg, "Driver Message", MB_OK);
	}
#endif
	return ErrCde;
}

//Close USB Board
void UB_close()
{
	DRV_DeviceClose((LONG far*)&lDriverHandle);
}

//Get the DO port current value
LRESULT UB_get_DO_value(USHORT* val)
{
	ptDioGetCurrentDOByte.value = val;
	ErrCde = DRV_DioGetCurrentDOByte(lDriverHandle, (LPT_DioGetCurrentDOByte) & ptDioGetCurrentDOByte);

#ifdef MSG_DIALOG_DISPLAY
	if (ErrCde != SUCCESS) {
		DRV_GetErrorMessage(ErrCde, (LPSTR)szErrMsg);
		Application->MessageBoxA((char*)szErrMsg, "Driver Message");
	}
#endif
	return ErrCde;
}

//Set DO value
LRESULT UB_set_DO_value(USHORT val)
{
	ptDioWritePortByte.state = val;
	ptDioWritePortByte.mask = 0xff;
	ErrCde = DRV_DioWritePortByte(lDriverHandle, (LPT_DioWritePortByte) & ptDioWritePortByte);

#ifdef MSG_DIALOG_DISPLAY
	if (ErrCde != SUCCESS) {
		DRV_GetErrorMessage(ErrCde, (LPSTR)szErrMsg);
		Application->MessageBoxA((char *)szErrMsg, "Driver Message");
	}
#endif
	return ErrCde;
}

//Set specified bit (0~7)
LRESULT UB_set_DO_bit(USHORT bit)
{
	ptDioWritePortByte.state = 0xff;
	ptDioWritePortByte.mask = 1 << bit;
	ErrCde = DRV_DioWritePortByte(lDriverHandle, (LPT_DioWritePortByte) & ptDioWritePortByte);

#ifdef MSG_DIALOG_DISPLAY
	if (ErrCde != SUCCESS) {
		DRV_GetErrorMessage(ErrCde, (LPSTR)szErrMsg);
		Application->MessageBoxA((char *)szErrMsg, "Driver Message");
	}
#endif
	return ErrCde;
}

//Clear specified bit (0~7)
LRESULT UB_clear_DO_bit(USHORT bit)
{
	ptDioWritePortByte.state = 0;
	ptDioWritePortByte.mask = 1 << bit;
	ErrCde = DRV_DioWritePortByte(lDriverHandle, (LPT_DioWritePortByte) & ptDioWritePortByte);

#ifdef MSG_DIALOG_DISPLAY
	if (ErrCde != SUCCESS) {
		DRV_GetErrorMessage(ErrCde, (LPSTR)szErrMsg);
		Application->MessageBoxA((char *)szErrMsg, "Driver Message");
	}
#endif
	return ErrCde;
}

//Get ADC voltage value
LRESULT UB_get_ADC_voltage(USHORT channel, float* voltage)
{
	ptAIVolIn.chan = channel;
	ptAIVolIn.gain = 4; //gian code is 4
	ptAIVolIn.TrigMode = 0; //internal triger
	ptAIVolIn.voltage = voltage;

	ErrCde = DRV_AIVoltageIn(lDriverHandle, &ptAIVolIn);

#ifdef MSG_DIALOG_DISPLAY
	if (ErrCde != 0) {
		DRV_GetErrorMessage(ErrCde, szErrMsg);
		Application->MessageBox(szErrMsg, "Error!!", MB_OK);
	}
#endif

	return ErrCde;
}

LRESULT UB_set_AO_voltage(USHORT channel, float * voltage)
{
    ptAOVolOut.chan = channel;
	ptAOVolOut.OutputValue = *voltage;

	ErrCde = DRV_AOVoltageOut(lDriverHandle, &ptAOVolOut);

#ifdef MSG_DIALOG_DISPLAY
	if (ErrCde != 0) {
		DRV_GetErrorMessage(ErrCde, szErrMsg);
		Application->MessageBox(szErrMsg, "Error!!", MB_OK);
	}
#endif

	return ErrCde;
}

