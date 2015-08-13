#include "DLLDefine.h"

/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI int __stdcall SendDataToLog(int port, int timeout_ms, char* szLog);
NEPDLLAPI int __stdcall InitDevice(int port, int timeout_ms);
NEPDLLAPI int __stdcall FreeDevice(int port, int timeout_ms);
NEPDLLAPI int __stdcall OpenDevice(int port, int timeout_ms);
NEPDLLAPI int __stdcall CloseDevice(int port, int timeout_ms);
NEPDLLAPI int __stdcall ExactOpenDevice(int port, int timeout_ms);
NEPDLLAPI int __stdcall ExactCloseDevice(int port, int timeout_ms);
NEPDLLAPI int __stdcall ExactReadDevice( int port, int timeout_ms, char* in_buf,  int in_size);
NEPDLLAPI int __stdcall ExactWriteDevice(int port, int timeout_ms, char* out_buf, int out_size);

//110420 gilpin add for receive serial command such as touch panel pixel and key value
NEPDLLAPI int __stdcall ReceiveCmdOut(int port, int timeout_ms, byte* pkt_cmd, char* out_buf);
NEPDLLAPI int __stdcall GeneralCmd(   int port, int timeout_ms, byte* pkt_cmd);
NEPDLLAPI int __stdcall GeneralCmdOut(int port, int timeout_ms, byte* pkt_cmd, char* out_buf);
NEPDLLAPI int __stdcall GeneralCmdIn( int port, int timeout_ms, byte* pkt_cmd, char* in_buf, size_t in_size);
NEPDLLAPI int __stdcall GeneralCmdInOut(int port, int timeout_ms, byte* pkt_cmd, char* in_buf, size_t in_size, char* out_buf);
NEPDLLAPI int __stdcall PowerOnToUARTPacketMode(int port, int timeout_ms, int DTRFlag);
NEPDLLAPI bool __stdcall ExecAdbOut(CString Command, char* output, char* error_msg);
NEPDLLAPI bool __stdcall ExecFastbootOut(CString Command, char* output, char* error_msg);
NEPDLLAPI int __stdcall TestFtdMode(int port, int timeout_ms, char* error_msg);
NEPDLLAPI int __stdcall AutoFtdMode(int port, int timeout_ms, char* error_msg);
