#include "DLLDefine.h"



/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI int __stdcall SendDataToLog(int nPort, int nTimeout, char* szLog);
NEPDLLAPI int __stdcall InitDevice(int PortNum, int nTimeout);
NEPDLLAPI int __stdcall FreeDevice(int PortNum, int nTimeout);
NEPDLLAPI int __stdcall OpenDevice(int nPort, int nTimeout);
NEPDLLAPI int __stdcall CloseDevice(int nPort, int nTimeout);
NEPDLLAPI int __stdcall ExactOpenDevice(int nPort, int nTimeout);
NEPDLLAPI int __stdcall ExactCloseDevice(int nPort, int nTimeout);

//110420 gilpin add for receive serial command such as touch panel pixel and key value 
NEPDLLAPI int __stdcall ReceiveCmdOut(int nPort, int nTimeout, char* cGenCommand, char* OutBuffer);
NEPDLLAPI int __stdcall GeneralCmd(int nPort, int nTimeout, char*cGenCommand);
NEPDLLAPI int __stdcall GeneralCmdOut(int nPort, int nTimeout, char* cGenCommand, char* OutBuffer);
NEPDLLAPI int __stdcall GeneralCmdIn(int nPort, int nTimeout, char* cGenCommand, char* InBuffer, size_t nInLength);
NEPDLLAPI int __stdcall GeneralCmdInOut(int nPort, int nTimeout, char* cGenCommand, char* InBuffer, size_t nInLength, char* OutBuffer);
NEPDLLAPI int __stdcall PowerOnToUARTPacketMode(int nPort, int nTimeout, int DTRFlag);

NEPDLLAPI int __stdcall FB_PassOrFail(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall HW_PassRange(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall HW_PassStr(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
