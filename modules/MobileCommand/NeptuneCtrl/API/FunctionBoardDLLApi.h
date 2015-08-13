#include "DLLDefine.h"
/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI int __stdcall FB_InitDevice(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_VBatMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_VRDBBMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_BackupBatMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_VRIOMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_VMEMMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_VRSIMMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_VCAPMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_Buck1P8VMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_BLCurrentMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_SideKeyLEDCurrentMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_StatusKeyCurrentMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_VibratorCurrentMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_LoadSpeakerAudioMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_ReceiverAudioMeasurement(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_PressPwrKey(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_AccessoryInsert(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_AccessoryPullout(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_PwrSwitchOVP(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_PwrSwitchPreCharge(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_PwrSwitchFullCharge(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_PwrSwitchNormalMode(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_PwrSwitchNoPwrToHandset(int nPort, int nTimeout);

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_PwrSwitchUsbCharge(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_AlsLedOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_AlsLedOff(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_ElectromagnetOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_ElectromagnetOff(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_LdoOn4Leakage(int nPort, int nTimeout);
NEPDLLAPI int __stdcall FB_LdoOff4Leakage(int nPort, int nTimeout);

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcSpeakPositive(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcSpeakNegative(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcReceiverPositive(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcReceiverNegative(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcAlsLed1(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcAlsLed2(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcAlsLed3(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcHsPositive(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcHsNegative(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcLoPositive(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcLoNegative(int nPort, int nTimeout, char* OutBuffer);

NEPDLLAPI int __stdcall FB_GetVersion(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall FB_FreeDevice(int nPort, int nTimeout);

NEPDLLAPI int __stdcall EnableComport(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall DisableComport(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall ExecuteBatchFile(int nPort, int nTimeout, char* InBuffer);

// 2009-10-26 Mick: new relay board function
NEPDLLAPI int __stdcall RB_InitDevice(int nPort, int nTimeout);
NEPDLLAPI int __stdcall RB_FreeDevice(int nPort, int nTimeout);
NEPDLLAPI int __stdcall RB_Relay2(int nPort, int nTimeout, char* InBuffer); // DTR
NEPDLLAPI int __stdcall RB_Relay1(int nPort, int nTimeout, char* InBuffer); // RTS
NEPDLLAPI int __stdcall RB_Relay0(int nPort, int nTimeout); // Tx
NEPDLLAPI int __stdcall RB_USB_Charging(int nPort, int nTimeout, char* InBuffer); // Relay2
NEPDLLAPI int __stdcall RB_HC_Charging(int nPort, int nTimeout, char* InBuffer);  // Relay1
NEPDLLAPI int __stdcall RB_PowerKey(int nPort, int nTimeout); // Relay0