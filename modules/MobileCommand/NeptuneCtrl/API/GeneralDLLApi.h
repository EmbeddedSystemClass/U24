#include "DLLDefine.h"
#include <setupapi.h>
#pragma comment(lib, "setupapi")


/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
//110325 gilpin gilpin add for search port ang get adb device
// HW Detect
bool FindDevice();
bool FindQcomDeviceId(CString sDevId);
bool FindP21DeviceId(CString sDevId);
CString GetDeviceName(HDEVINFO& hDevInfo, SP_DEVINFO_DATA& spDevInfoData);

NEPDLLAPI bool __stdcall GetAdbDevice();
NEPDLLAPI bool __stdcall SearchPortUsb(const UINT secTimeout);

NEPDLLAPI int __stdcall SendDataToLog(int nPort, int nTimeout, char* szLog);
NEPDLLAPI int __stdcall InitDevice(int PortNum, int nTimeout);
NEPDLLAPI int __stdcall FreeDevice(int PortNum, int nTimeout);
NEPDLLAPI int __stdcall OpenDevice(int nPort, int nTimeout);
NEPDLLAPI int __stdcall CloseDevice(int nPort, int nTimeout);

//110420 gilpin add for receive serial command such as touch panel pixel and key value 
NEPDLLAPI int __stdcall ReceiveCmdOut(int nPort, int nTimeout, char* cGenCommand, char* OutBuffer);

/* 111020 Gilpin add parameter of in buffer size transfer to solve the problem of data lose */
NEPDLLAPI int __stdcall GeneralCmd(int nPort, int nTimeout, char*cGenCommand);
NEPDLLAPI int __stdcall GeneralCmdOut(int nPort, int nTimeout, char* cGenCommand, char* OutBuffer);
NEPDLLAPI int __stdcall GeneralCmdIn(int nPort, int nTimeout, char* cGenCommand, char* InBuffer, size_t nInLength);
NEPDLLAPI int __stdcall GeneralCmdInOut(int nPort, int nTimeout, char* cGenCommand, char* InBuffer, size_t i_InLength, char* OutBuffer);
NEPDLLAPI int __stdcall PowerOnToUARTPacketMode(int nPort, int nTimeout, int DTRFlag);

NEPDLLAPI int __stdcall FB_PassOrFail(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall HW_PassRange(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall HW_PassStr(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall MTKCmdInOut(int nPort, int nTimeout, size_t nReqBody, BYTE* ReqBody, size_t nPeerBuffer, BYTE* PeerBuffer,BYTE* OutConfirm, BYTE* OutPeerData);
