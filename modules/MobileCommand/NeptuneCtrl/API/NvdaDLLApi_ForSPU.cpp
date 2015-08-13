/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .cpp file of Nvda Command DLL module for SPU Script use.
 * FileName   : NvdaDLLApi_ForSPU.cpp
 * Version    : $Revision:   1.21  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/11
 * History    :
 *  NO      Date        Updated by				Content of change
 *  1    2011/03/11      Gilpin.Xi					Create.
 *  2    2011/07/22      Gilpin.Xi			Modify for Docunment update
 ********************************************************************************/
#include "stdafx.h"
#include "NvdaDLLApi_ForSPU.h"

#include "../CmdType.h"
#include "GeneralDLLApi.h"

NEPDLLAPI bool InitDevice_ForSPU(char* PortNum, char* nTimeout, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = InitDevice(::atoi(PortNum),::atoi(nTimeout));

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool FreeDevice_ForSPU(char* PortNum, char* nTimeout, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = FreeDevice(::atoi(PortNum),::atoi(nTimeout));

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}


NEPDLLAPI bool EFS_ReadFile_ForSPU(char* sFileSrc, char* sFileDst, char* ErrMsg,char* ErrorCode)
{
	bool bRes = false;
	CString cstrFileDst;
	CString cstrCommand;
	CString cstrOutMessage;
	char *msgBuffer = new char[1024 * 1024 * 2];

	CString cstrPathDir;
	::GetModuleFileName(NULL, cstrPathDir.GetBuffer(MAX_PATH), MAX_PATH);
	cstrPathDir.ReleaseBuffer();
	cstrPathDir = cstrPathDir.Left(cstrPathDir.ReverseFind('\\'));

	cstrFileDst.Format("%s",sFileDst);
	cstrFileDst = cstrFileDst.Right(cstrFileDst.GetLength() - cstrFileDst.Find('.') - 1);

	cstrFileDst = cstrPathDir + cstrFileDst;

	//cstrFileDst = _T("'") + cstrFileDst + _T("'") ;

	cstrCommand.Format("%s\"%s\"",_T("pull "),sFileSrc);
	cstrCommand = cstrCommand + _T(" \"") + cstrFileDst + _T("\"");

	bRes = ExecAdbOut(cstrCommand,msgBuffer,ErrorCode);

	strcpy(ErrMsg,msgBuffer);

	cstrOutMessage.Format("%s",msgBuffer);
	if((cstrOutMessage.Find("error") != -1)||cstrOutMessage.Find("does not exist") != -1)
		bRes = false;

	delete[] msgBuffer;

	return bRes;
}

// 1-SD
NEPDLLAPI bool NVDA_SD_DetectStatus_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SD_DetectStatus(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SD_OperateTest_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SD_OperateTest(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 2-CAMERA
NEPDLLAPI bool NVDA_CAMERA_ReadId_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_CAMERA_ReadId(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_CAMERA_8MTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_CAMERA_8MTest(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_CAMERA_5MTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_CAMERA_5MTest(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_CAMERA_1D3MTest_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_CAMERA_1D3MTest(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 3-Display
NEPDLLAPI bool NVDA_Display_LCDRGB_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_Display_LCDRGB(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_Display_DetectHDMI_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_Display_DetectHDMI(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_Display_HDMIRGB_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_Display_HDMIRGB(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_Display_LCDRLE_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_Display_LCDRLE(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_Display_CloseLCD_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_Display_CloseLCD(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 4-SENSOR
NEPDLLAPI bool NVDA_SENSOR_GSENSORReadAcc_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SENSOR_GSENSORReadAcc(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SENSOR_GSENSORReadID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SENSOR_GSENSORReadID(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SENSOR_ECompassI2CTest_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SENSOR_ECompassI2CTest(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SENSOR_TouchID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SENSOR_TouchID(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SENSOR_LSensorReadID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SENSOR_LSensorReadID(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SENSOR_LSensorReadValue_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SENSOR_LSensorReadValue(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SENSOR_PSensorReadID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SENSOR_PSensorReadID(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SENSOR_PSensorReadProximity_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SENSOR_PSensorReadProximity(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SENSOR_GyroReadID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SENSOR_GyroReadID(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SENSOR_LSensorCalibration_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SENSOR_LSensorCalibration(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SENSOR_PSensorFarCalibration_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SENSOR_PSensorFarCalibration(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SENSOR_PSensorNearCalibration_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SENSOR_PSensorNearCalibration(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SENSOR_GSensorCalibration_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SENSOR_GSensorCalibration(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 5-AUDIO
NEPDLLAPI bool NVDA_AUDIO_Headset_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_AUDIO_Headset(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_AUDIO_Speaker_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_AUDIO_Speaker(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_AUDIO_JackCheck_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_AUDIO_JackCheck(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_AUDIO_HookCheck_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_AUDIO_HookCheck(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_AUDIO_LineOut_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_AUDIO_LineOut(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_AUDIO_PlayMusic_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_AUDIO_PlayMusic(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_AUDIO_PlayTone_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_AUDIO_PlayTone(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_AUDIO_RecordWav_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_AUDIO_RecordWav(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_AUDIO_LoopTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_AUDIO_LoopTest(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_AUDIO_SwitchRX_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_AUDIO_SwitchRX(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_AUDIO_SwitchTX_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_AUDIO_SwitchTX(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_AUDIO_PlayWav_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_AUDIO_Playwav(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 6-GPS
NEPDLLAPI bool NVDA_GPS_EnterSARFMode_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_GPS_EnterSARFMode(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_GPS_CWTest_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_GPS_CWTest(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_GPS_FreqStability_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_GPS_FreqStability(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_GPS_ExitSARFMode_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_GPS_ExitSARFMode(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 7-BT
NEPDLLAPI bool NVDA_BT_TxPowerOn_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	/*
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_BT_TxPowerOn(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}
	*/

	return false;
}

NEPDLLAPI bool NVDA_BT_TxPowerOff_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_BT_TxPowerOff(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 8-FMR
NEPDLLAPI bool NVDA_FMR_ReadRSSI_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FMR_ReadRSSI(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FMR_PathTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FMR_PathTest(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 9-SIM
NEPDLLAPI bool NVDA_SIM_CheckStatus_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SIM_CheckStatus(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_SIM_DetectHotPlug_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_SIM_DetectHotPlug(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 10-CLK

// 11-USB

// 12-TOUCH
NEPDLLAPI bool NVDA_TOUCH_Test_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_TOUCH_Test(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_TOUCH_DownloadFirmware_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_TOUCH_DownloadFirmware(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 13-KEYPAD
NEPDLLAPI bool NVDA_KEYPAD_Detect_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_KEYPAD_Detect(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_KEYPAD_GetCapkeyChipID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_KEYPAD_GetCapkeyChipID(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 14-VIBRATOR
NEPDLLAPI bool NVDA_VIBRATOR_Switch_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VIBRATOR_Switch(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_VIBRATOR_RunningTime_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VIBRATOR_RunningTime(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 15-BATTERY
NEPDLLAPI bool NVDA_BATTERY_BatteryCharge_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_BATTERY_BatteryCharge(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_BATTERY_BatteryCurrent_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_BATTERY_BatteryCurrent(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_BATTERY_BatteryTemperature_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_BATTERY_BatteryTemperature(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_BATTERY_BatteryVoltage_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_BATTERY_BatteryVoltage(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_BATTERY_BatteryCapacity_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_BATTERY_BatteryCapacity(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_BATTERY_GaugeDFICalibration_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_BATTERY_GaugeDFICalibration(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_BATTERY_ReadGaugeID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_BATTERY_ReadGaugeID(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_BATTERY_ReadChargerID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_BATTERY_ReadChargerID(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_BATTERY_CoinCellCharge_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_BATTERY_CoinCellCharge(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_BATTERY_DFIITEnable_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_BATTERY_DFIITEnable(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_BATTERY_CoinCellVoltage_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_BATTERY_CoinCellVoltage(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 16-DOWNLOAD

// 17-OS

// 18-WLAN
NEPDLLAPI bool NVDA_WLAN_EnableTestMode_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_WLAN_EnableTestMode(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_WLAN_EnableNormalMode_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_WLAN_EnableNormalMode(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_WLAN_Disable_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_WLAN_Disable(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_WLAN_TxPowerSwitch_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_WLAN_TxPowerSwitch(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_WLAN_ScanAP_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_WLAN_ScanAP(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_WLAN_GetScanResult_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_WLAN_GetScanResult(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_WLAN_ConnectAP_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_WLAN_ConnectAP(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_WLAN_GetRSSI_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_WLAN_GetRSSI(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 19-LED
NEPDLLAPI bool NVDA_LED_BacklightLEDTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_LED_BacklightLEDTest(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_LED_LEDRGBTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_LED_LEDRGBTest(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_LED_CapKeyLEDTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_LED_CapKeyLEDTest(::atoi(nPort),::atoi(nTimeout),InBuffer,OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_LED_FlashLEDTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_LED_FlashLEDTest(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_LED_ReadFlashID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_LED_ReadFlashID(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 20-ID
NEPDLLAPI bool NVDA_ID_ReadHWID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_ID_ReadHWID(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 21-Regulator
NEPDLLAPI bool NVDA_REGULATOR_ReadREGValue_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_REGULATOR_ReadREGValue(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 22-RF
NEPDLLAPI bool NVDA_WIRELESS_GSMTxPower_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer,char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_WIRELESS_GSMTxPower(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_WIRELESS_WCDMATxPower_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer,char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_WIRELESS_WCDMATxPower(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_WIRELESS_TXPowerOFF_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode) //cmd:3
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_WIRELESS_TXPowerOFF(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 23-WWAN

// 24-TV_MOUDLE
NEPDLLAPI bool NVDA_TV_ModuleDetect_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_TV_ModuleDetect(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_TV_InBandPower_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_TV_InBandPower(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 25-VERSION
NEPDLLAPI bool NVDA_VERSION_ReadSWVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VERSION_ReadSWVersion(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_VERSION_ReadDDRInfo_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VERSION_ReadDDRInfo(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_VERSION_ReadeMMCInfo_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VERSION_ReadeMMCInfo(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_VERSION_ReadSDInfo_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VERSION_ReadSDInfo(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_VERSION_ReadTouchInfo_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VERSION_ReadTouchInfo(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_VERSION_ReadCameraInfo_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VERSION_ReadCameraInfo(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_VERSION_ReadBTVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VERSION_ReadBTVersion(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_VERSION_ReadWIFIVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VERSION_ReadWIFIVersion(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_VERSION_ReadLCDInfo_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VERSION_ReadLCDInfo(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_VERSION_ReadGyroFWVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VERSION_ReadGyroFWVersion(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_VERSION_ReadRFVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VERSION_ReadRFVersion(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_VERSION_ReadBBVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VERSION_ReadBBVersion(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_VERSION_ReadPMVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_VERSION_ReadPMVersion(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 26-FACTORY
NEPDLLAPI bool NVDA_FACTORY_FARead_ForSPU(char* nPort, char* nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_FARead(::atoi(nPort), ::atoi(nTimeout), InBuffer, i_InLength, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_FAWrite_ForSPU(char* nPort, char* nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_FAWrite(::atoi(nPort), ::atoi(nTimeout), InBuffer, i_InLength, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_ReadPicasso_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_ReadPicasso(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_ReadPicassoCheck_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_ReadPicassoCheck(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_ReadWifiMac_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_ReadWifiMac(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_WriteWifiMac_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WriteWifiMac(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_ReadBtMac_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_ReadBtMac(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_WriteBtMac_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WriteBtMac(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_TriggerNVrestore_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_TriggerNVrestore(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_ReadPPID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_ReadPPID(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_WritePPID_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WritePPID(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_ReadServiceTag_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_ReadServiceTag(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_WriteServiceTag_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WriteServiceTag(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_WriteDLFlag_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WriteDLFlag(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_WriteFTFlag_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WriteFTFlag(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_WriteMMIBBFlag_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WriteMMIBBFlag(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_WriteMMIRFFlag_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WriteMMIRFFlag(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_CheckAndMoveFA_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_CheckAndMoveFA(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_ReadIMEI_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_ReadIMEI(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_WriteIMEI_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WriteIMEI(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}
NEPDLLAPI bool NVDA_FACTORY_ReadLCSN_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_ReadLCSN(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_ReadSN_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_ReadSN(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_WriteLCSN_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WriteLCSN(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_WriteSN_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WriteSN(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_ReadPhoneTime_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_ReadPhoneTime(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_FACTORY_WritePhoneTime_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WritePhoneTime(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}
NEPDLLAPI bool NVDA_FACTORY_WriteOTP_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WriteOTP(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}
NEPDLLAPI bool NVDA_FACTORY_WriteFCFlag_ForSPU(char* nPort, char* nTimeout,char* InBuffer, size_t i_InLength, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_WriteFCFlag(::atoi(nPort), ::atoi(nTimeout), InBuffer, i_InLength, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}


NEPDLLAPI bool NVDA_FACTORY_ResetPhone_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_ResetPhone(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 2011/08/30 add by jackiesen :Set ftd Cmd to Noramal
NEPDLLAPI bool NVDA_FACTORY_SetBootMode_ForSPU(char* nPort, char* nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer,char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_SetBootMode(::atoi(nPort), ::atoi(nTimeout), InBuffer, i_InLength, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}
//2011/09/19 add by jackiesen
NEPDLLAPI bool NVDA_FACTORY_EFS_Sync_ForSPU(char* nPort, char* nTimeout, char* OutBuffer,char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_FACTORY_EFS_Sync(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}
// 27-HouseKeeping
NEPDLLAPI bool NVDA_HOUSEKEEP_PowerGlueLogicCheck_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_HOUSEKEEP_PowerGlueLogicCheck(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 28-EFS

// 29-NV
NEPDLLAPI bool NVDA_NV_ReadNV_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_NV_ReadNV(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_NV_WriteNV_ForSPU(char* nPort, char* nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_NV_WriteNV(::atoi(nPort), ::atoi(nTimeout), InBuffer, i_InLength, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_NV_WriteInitNV_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_NV_WriteInitNV(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_NV_BackupNV_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_NV_BackupNV(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

// 30-DIAG
NEPDLLAPI bool NVDA_DIAG_SetRequest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_DIAG_SetRequest(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}
NEPDLLAPI bool NVDA_FACTORY_ReadMEID_ForSPU(char* nPort, char* OutBuffer)
{
	////return 2 is NULL and read false
	//if(LoadDLLNeptuneCtrl())
	//{
	//	char tempOutBuffer[4096] = {0};
	//	int nTimeout = 0;
	//	int nRes = -1;
	//	nRes = pfInitDevice(atoi(nPort), nTimeout);
	//	if(nRes != 1) 
	//	{
	//		FreeDLLNeptuneCtrl();
	//		return false;
	//	}
	//	nRes = pfReadIMEI(atoi(nPort), nTimeout, tempOutBuffer);
	//	if(nRes != 1) 
	//	{
	//		FreeDLLNeptuneCtrl();
	//		return false;
	//	}
	//	nRes = pfFreeDevice(atoi(nPort), nTimeout);
	//	if(nRes != 1) 
	//	{
	//		FreeDLLNeptuneCtrl();
	//		return false;
	//	}
	//	//strcpy_s(OutBuffer, strlen(tempOutBuffer), tempOutBuffer);
	//	/*int i = 0;
	//	for(; i < (int)strlen(tempOutBuffer); i++)
	//	{
	//	*(OutBuffer + i) = tempOutBuffer[i];
	//	}
	//	*(OutBuffer + i) = 0;*/

	//	int i = 0;
	//	int kkkktemptemojtp = strlen(tempOutBuffer);
	//	for(; i < 7; i++)
	//	{
	//		*(OutBuffer + i) = tempOutBuffer[i];
	//	}
	//	*(OutBuffer + i) = 0;

	//	BYTE byteOutBuffer[7] = {0};
	//	for(i = 0; i < 7; i++)
	//	{
	//		byteOutBuffer[i] = (BYTE)OutBuffer[i];
	//	}
	//	char sDigit[7*2 + 1] = {0};
	//	HexBcdToDigit(byteOutBuffer, 7, sDigit, 7*2 + 1);	

	//	for(i = 0; i < 14; i++)
	//	{
	//		OutBuffer[i] = sDigit[13 - i];
	//	}
	//	OutBuffer[i] = 0;

	//}
	//else
	//{
	//	FreeDLLNeptuneCtrl();
	//	return false;
	//}
	//FreeDLLNeptuneCtrl();
	return true;
}
NEPDLLAPI bool NVDA_FACTORY_WriteMEID_ForSPU(char* nPort, char* InBuffer)
{
	/*if(LoadDLLNeptuneCtrl())
	{
		char OutBuffer[4096] = {0};
		int nTimeout = 0;
		int nRes = -1;
		nRes = pfInitDevice(atoi(nPort), nTimeout);
		if(nRes != 1) 
		{
			FreeDLLNeptuneCtrl();
			return false;
		}
		if(strlen(InBuffer) >= 14)
		{
			char szReverseMEID[15] = {0};
			for(int i=0; i<14; i++)
			{
				szReverseMEID[i] = InBuffer[i];
			}
			BYTE byteMEID[7]= {0};
			DigitToHexBcd(szReverseMEID, 15,byteMEID, 7);
			nRes = pfWriteIMEI(atoi(nPort), nTimeout, byteMEID, OutBuffer);
		}
		if(nRes != 1) 
		{
			FreeDLLNeptuneCtrl();
			return false;
		}
		nRes = pfFreeDevice(atoi(nPort), nTimeout);
		if(nRes != 1) 
		{
			FreeDLLNeptuneCtrl();
			return false;
		}
	}
	else
	{
		FreeDLLNeptuneCtrl();
		return false;
	}
	FreeDLLNeptuneCtrl();*/
	return true;
}

// 31-RTC
NEPDLLAPI bool NVDA_RTC_Read_ForSPU(char* nPort, char* nTimeout,char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_RTC_Read(::atoi(nPort), ::atoi(nTimeout), OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}

NEPDLLAPI bool NVDA_RTC_Write_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode)
{
	int bRes = -1*ERROR_FUNCTION_FAILED;

	bRes = NVDA_RTC_Write(::atoi(nPort), ::atoi(nTimeout), InBuffer, OutBuffer);

	if(bRes != HS_OK)
	{
		wsprintf(ErrorCode, _T("%d"), bRes); 
		return false;
	}
	else
	{
		wsprintf(ErrorCode, _T("%d"), 1); 
		return true;
	}

	return false;
}
