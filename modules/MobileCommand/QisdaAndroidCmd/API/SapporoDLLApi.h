#include "DLLDefine.h"

//05:audio

NEPDLLAPI int __stdcall HeadsetButtonCount(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:14

//22:SmartCover
NEPDLLAPI int __stdcall Sapporo_SCoverDetect(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall Sapporo_SCoverInterrupt(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall Sapporo_SCoverEnable3V(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall Sapporo_SCoverCheckI2C(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall Sapporo_SCoverSwitch5V(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5

//92:Factory
NEPDLLAPI int __stdcall Sapporo_FAC_FSGProtect(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5


//93:misc
NEPDLLAPI int __stdcall Sapporo_ftdStartEnd(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5


//Special Control Functions
NEPDLLAPI int __stdcall Sapporo_CheckDevelopMode(char* output, char* error_code);
NEPDLLAPI int __stdcall Sapporo_ReadSWversion(char* output, char* error_code);

//Special Control Functions SPU
NEPDLLAPI int Sapporo_CheckDevelopMode_SPU(char* output, char* error_code);
NEPDLLAPI int Sapporo_ReadSWversion_SPU(char* output, char* error_code);
