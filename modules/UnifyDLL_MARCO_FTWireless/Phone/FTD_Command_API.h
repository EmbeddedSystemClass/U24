/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .h file of FTD command API for Mobile Phone operate object use.
 * FileName   : CPhone.h
 * Version    : $Revision:   1.14  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/16
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/16      Gilpin.Xi             Create.
 ********************************************************************************/

#ifndef _FTD_Command_API_H
#define _FTD_Command_API_H


// USB4702 Control Board API
typedef int (__stdcall * PFUSB4702_InitFunc)(int nAddress);
typedef int (__stdcall * PFUSB4702_CloseFunc)(int nAddress);
typedef int (__stdcall * PFUSB4702_GetDOValFunc)(int nAddress, char * buf);
typedef int (__stdcall * PFUSB4702_SetDOValFunc)(int nAddress, char * val);
typedef int (__stdcall * PFUSB4702_GetADCVolFunc)(int nAddress, char * Channel, char * buf);
typedef int (__stdcall * PFUSB4702_GetADCVolExFunc)(int nAddress, char * Channel, char * buf);
typedef int (__stdcall * PFUSB4702_SetAOVolFunc)(int nAddress, char * Channel, char * vol);
typedef int (__stdcall * PFUSB4702_SetDOBitFunc)(int nAddress, char * bit);
typedef int (__stdcall * PFUSB4702_ClearDOBitFunc)(int nAddress, char * bit);

// Mobile FTD command API
typedef bool (__stdcall * PFGetAdbDeviceFunc)();
typedef bool (__stdcall * PFSearchPortUsbFunc)(const UINT secTimeout);
typedef bool (__stdcall * PFExecAdbOutFunc)(CString Command, char* output, char* ErrorCode);

typedef bool (__stdcall * PFNVDA_TestFtdModeFunc)(int nPort, int nTimeout, char* ErrorCode);
typedef bool (__stdcall * PFNVDA_AutoFtdModeFunc)(int nPort, int nTimeout, char* ErrorCode);

typedef int (__stdcall * PFInitDeviceFunc)(int nPort, int nTimeout);
typedef int (__stdcall * PFFreeDeviceFunc)(int nPort, int nTimeout);
typedef int (__stdcall * PFReceiveCmdOutFunc)(int nPort, int nTimeout, char* cIDCommand, char* OutBuffer);
typedef int (__stdcall * PFGenCmdInOut)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
typedef int (__stdcall * PFGenCmdOut)(int nPort, int nTimeout, char* OutBuffer);
typedef int (__stdcall * PFGenCmdInInOut)(int nPort, int nTimeout, char* InBuffer, size_t nInSize, char* OutBuffer);
typedef int (__stdcall * PFExactControl)(int nPort, int nTimeout);

typedef bool (*PFEFS_ReadFileFunc)(char* p_sz_FileSrc, char* p_sz_FileDst, char* p_sz_ErrorMsg, char* p_sz_ErrorCode);


#endif //_FTD_Command_API_H
