/*-----------------------------------------------------------------------------
|  Project :  CSST
|  Module  :  Utilities
+------------------------------------------------------------------------------
|             Copyright 2003 Texas Instruments.
|             All rights reserved. 
| 
|             This file is confidential and a trade secret of Texas 
|             Instruments .
|             The receipt of or possession of this file does not convey 
|             any rights to reproduce or disclose its contents or to 
|             manufacture, use, or sell anything it may describe, in 
|             whole, or in part, without the specific written consent of 
|             Texas Instruments. 
+------------------------------------------------------------------------------
| Filename:   usbdriver.h
| Author:     Morten Klostergaard (mkg@ti.com), 
|             Modified by Christian Lodberg (clo@ti.com)
|
| Purpose:    This header file provides USB prototypes
+----------------------------------------------------------------------------*/
#ifndef __UsbDriver_H__ 
#define __UsbDriver_H__ 

/*==== DECLARATION CONTROL =================================================*/

/*==== INCLUDES ============================================================*/
#include <setupapi.h>
#include "types.h" // U32/S32, U8/S8
#include "CircBuffer.h"
#include "comdriver.h"

/*==== CONSTS ==============================================================*/

/*==== TYPES ===============================================================*/

/*==== EXPORTS =============================================================*/
class DLLAPI UsbDrv : public BaseDrv {
public:
	UsbDrv();
	~UsbDrv();
	S8 open(U32 Port, U32 BaudRate, U8 Parity, U8 StopBits, U8 Data);
	S8 close(void);
	S8 reset(void);
	S8 configure(U32 BaudRate, U8 Parity, U8 StopBits, U8 Data);
	S32 write(U8* Buffer, U32 Size);
	S32 read(U8* Buffer, U32 Size, U32 timeout);
	HANDLE getRxEventHandle();
	void setEventThreshold(U32 num);
	void registerCallback(T_SERIAL_CALL_BACK callback);
	void unregisterCallback();
private:
	U8 isOpen;
	HANDLE HndlRx, HndlTx, ThreadHndl, rxSignal;
	OVERLAPPED readOverlapped, writeOverlapped;
	CRITICAL_SECTION CriticalSection;
	U32 ThreadID;
	T_SERIAL_CALL_BACK CallBack;
	U8* rxBuf;
	CircBuffer* UsbBuffer;
	U32 Threshold;
	void checkThreshold();
	static unsigned long __stdcall UsbListener(void* lpParam);
	static HANDLE OpenOneDevice ( HDEVINFO HardwareDeviceInfo, PSP_DEVICE_INTERFACE_DATA DeviceInfoData, char *devName );
	static HANDLE OpenUsbDevice( LPGUID pGuid, char *outNameBuf);
	static HANDLE OpenDev();
	static BOOL   GetUsbDeviceFileName( LPGUID pGuid, char *outNameBuf);
	static HANDLE OpenFile( char *filename);
	static S8 ResetHandle(HANDLE &Hndl, char inPipe[32]);
protected:
};

#endif /* __UsbDriver_H__  */

/*==== END OF FILE ===========================================================*/
