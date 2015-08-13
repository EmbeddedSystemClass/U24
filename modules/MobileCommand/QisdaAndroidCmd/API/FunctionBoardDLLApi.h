#include "DLLDefine.h"
/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI int __stdcall FB_InitDevice(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_VBatMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_VRDBBMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_BackupBatMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_VRIOMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_VMEMMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_VRSIMMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_VCAPMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_Buck1P8VMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_BLCurrentMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_SideKeyLEDCurrentMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_StatusKeyCurrentMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_VibratorCurrentMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_LoadSpeakerAudioMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_ReceiverAudioMeasurement(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_PressPwrKey(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_AccessoryInsert(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_AccessoryPullout(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_PwrSwitchOVP(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_PwrSwitchPreCharge(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_PwrSwitchFullCharge(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_PwrSwitchNormalMode(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_PwrSwitchNoPwrToHandset(int port, int timeout_ms);

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_PwrSwitchUsbCharge(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_AlsLedOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_AlsLedOff(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_ElectromagnetOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_ElectromagnetOff(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_LdoOn4Leakage(int port, int timeout_ms);
NEPDLLAPI int __stdcall FB_LdoOff4Leakage(int port, int timeout_ms);

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcSpeakPositive(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcSpeakNegative(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcReceiverPositive(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcReceiverNegative(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcAlsLed1(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcAlsLed2(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcAlsLed3(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcHsPositive(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcHsNegative(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcLoPositive(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_AdcLoNegative(int port, int timeout_ms, char* OutBuffer);

NEPDLLAPI int __stdcall FB_GetVersion(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall FB_FreeDevice(int port, int timeout_ms);

NEPDLLAPI int __stdcall EnableComport(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall DisableComport(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall ExecuteBatchFile(int port, int timeout_ms, char* InBuffer);

// 2009-10-26 Mick: new relay board function
NEPDLLAPI int __stdcall RB_InitDevice(int port, int timeout_ms);
NEPDLLAPI int __stdcall RB_FreeDevice(int port, int timeout_ms);
NEPDLLAPI int __stdcall RB_Relay2(int port, int timeout_ms, char* InBuffer); // DTR
NEPDLLAPI int __stdcall RB_Relay1(int port, int timeout_ms, char* InBuffer); // RTS
NEPDLLAPI int __stdcall RB_Relay0(int port, int timeout_ms); // Tx
NEPDLLAPI int __stdcall RB_USB_Charging(int port, int timeout_ms, char* InBuffer); // Relay2
NEPDLLAPI int __stdcall RB_HC_Charging(int port, int timeout_ms, char* InBuffer);  // Relay1
NEPDLLAPI int __stdcall RB_PowerKey(int port, int timeout_ms); // Relay0