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
| Filename:   usbdriver.cpp
| Author:     Morten Klostergaard (mkg@ti.com), 
|             Modified by Christian Lodberg (clo@ti.com)
|
| Purpose:    This file contains the USB driver for CSST Host side which is
|             operating in BULK mode.
+----------------------------------------------------------------------------*/

/*==== DECLARATION CONTROL ==================================================*/

/*==== INCLUDES =============================================================*/
#include <windows.h>
#include <process.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <assert.h>
#include <time.h>
#include <setupapi.h>
#pragma comment(lib, "setupapi")
#include <basetyps.h>
#include "devioctl.h"
#include "debug.h"
#include "usbdi.h"
#include "usbdriver.h"

// 67AB1FAB-5952-462d-8A93-C88A4EA373C3
//const GUID GUID_CLASS_TI_SAMPLE = {0x67ab1fab, 0x5952, 0x462d, 0x8a, 0x93, 0xc8, 0x8a, 0x4e, 0xa3, 0x73, 0xc3};

const GUID GUID_CLASS_TI_SAMPLE = {0x873fdf, 0x61a8, 0x11d1, 0xaa, 0x5e, 0x0, 0xc0, 0x4f, 0xb1, 0x72, 0x8b };

// {96e73b6e-7a5a-11d4-9f24-0080c82727f4}
// const GUID GUID_CLASS_TI_SAMPLE = {0x96e73b6e, 0x7a5a, 0x11d1, 0x9f, 0x24, 0x00, 0x80, 0xc8, 0x27, 0x27, 0xf4 };


char inPipe[32] = "PIPE00";     
char outPipe[32] = "PIPE01";    
char completeDeviceName[256] = "";  

UsbDrv::UsbDrv(void)
{
    UsbDrv::HndlTx = NULL;
	UsbDrv::ThreadHndl = NULL;
	UsbDrv::rxSignal = NULL;
	UsbDrv::CallBack = NULL;
  
    InitializeCriticalSection(&(UsbDrv::CriticalSection));
    
    UsbDrv::Threshold = 0;

    UsbBuffer = new CircBuffer(SER_RX_BUF_SIZE); 
    /* Default state is not opened. */
    isOpen = 0;
}

UsbDrv::~UsbDrv()
{
    UsbDrv::close();
    if (UsbBuffer != NULL)
    {
        delete UsbBuffer;
        UsbBuffer = NULL;
    }
}

S8 UsbDrv::open(U32 Port, U32 BaudRate, U8 Parity, U8 StopBits, U8 Data)
{

    if (isOpen)
    {
        return -1; /* Port is already open */
    }
    
    UsbDrv::HndlTx = OpenFile(outPipe); /* Open transmit pipe */
    UsbDrv::HndlRx = OpenFile(inPipe);  /* Open receive pipe  */

    /* Check if the read pipe opened */
    if (UsbDrv::HndlRx == INVALID_HANDLE_VALUE) 
    {
        UsbDrv::close();
        return -1; 
    }

    /* Check if the write pipe is opened */
    if(UsbDrv::HndlTx == INVALID_HANDLE_VALUE) 
	{
        UsbDrv::close();
        return -1;
	}

    UsbDrv::rxSignal = CreateEvent(NULL,  /* default security attributes */
                                   FALSE, /* auto-reset event */
                                   FALSE, /* initial state is signaled */
                                   NULL); /* object not named */
    
    if (UsbDrv::configure(BaudRate, Parity, StopBits, Data) != 0)
    {
        return -1;
    }
        
    UsbDrv::ThreadHndl = CreateThread(NULL,                 /* no security attributes */
                                      0,                    /* use default stack size */
                                      UsbDrv::UsbListener,  /* thread function */
                                      (void*) this,             
                                      0,                    /* use default creation flags */
                                      &(UsbDrv::ThreadID));
    
    if (!UsbDrv::ThreadHndl) 
    {
        UsbDrv::close();
        return -1;
    }
    
    isOpen = 1;
    return 0;
}


/*-----------------------------------------------------------------------------
| Function    : UsbDrv::close()
+------------------------------------------------------------------------------
| Description : Closing function for the USB driver. 
|
| Parameters  : void
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S8 UsbDrv::close(void)
{
	// 2009-09-28 Mick add criticial section for TerminateThread
    U32 exitCode;
    if (UsbDrv::ThreadHndl != NULL)
    {
		EnterCriticalSection(&(UsbDrv::CriticalSection));
        GetExitCodeThread(UsbDrv::ThreadHndl, &exitCode);
        if (exitCode == STILL_ACTIVE)
        {
            TerminateThread(UsbDrv::ThreadHndl, exitCode);
        }
		LeaveCriticalSection(&(UsbDrv::CriticalSection));
        CloseHandle(UsbDrv::ThreadHndl);
		UsbDrv::ThreadHndl = NULL;
    }
    
    if(UsbDrv::HndlTx != NULL)
    {
		CloseHandle(UsbDrv::HndlTx);        
    }
    if(UsbDrv::rxSignal != NULL)
    {        
	    CloseHandle(UsbDrv::rxSignal);
    }
    UsbDrv::HndlTx = NULL;
	UsbDrv::rxSignal = NULL;
	UsbDrv::isOpen = 0; /* USB Closed */
    
    return 0;
}


/*-----------------------------------------------------------------------------
| Function    : UsbDrv::reset()
+------------------------------------------------------------------------------
| Description : USB Reset Function
|
| Parameters  : None
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S8 UsbDrv::reset()
{
    return 0;
}


/*-----------------------------------------------------------------------------
| Function    : UsbDrv::ResetHandle()
+------------------------------------------------------------------------------
| Description : USB Reset Function
|
| Parameters  : 
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S8 UsbDrv::ResetHandle(HANDLE &Hndl, char Pipe[32])
{    
    CloseHandle(Hndl);
    
    Sleep(10); /* Delay added to avoid windows USB driver error */

    Hndl = OpenFile(Pipe);

    if (Hndl == NULL)
    {
        return -1;
    }
    return 0;
}


/*-----------------------------------------------------------------------------
| Function    : UsbDrv::configure()
+------------------------------------------------------------------------------
| Description : USB Reset Function
|
| Parameters  : 
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S8 UsbDrv::configure(U32 BaudRate, U8 Parity, U8 StopBits, U8 Data)
{
	return 0;
}


/*-----------------------------------------------------------------------------
| Function    : UsbDrv::write()
+------------------------------------------------------------------------------
| Description : USB write Function
|
| Parameters  :
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S32 UsbDrv::write(U8* Buffer, U32 Size)
{
    U32 written_total = 0, written = 0;
    int iResetCount=0;
    while (written_total < Size) 
    {
		//FF:設錯200次才return 因為之前設2次很快就return了
        if (iResetCount>200) 
		{
			break;
        }

        if (!WriteFile(UsbDrv::HndlTx, Buffer, Size, &written, NULL)) 
        {
            switch(GetLastError())
            {
                case ERROR_WRITE_FAULT:
                    ResetHandle(UsbDrv::HndlTx,outPipe);
					iResetCount++;
					Sleep(10);
                    break;
                case ERROR_NOT_READY:
                    ResetHandle(UsbDrv::HndlTx,outPipe);
					iResetCount++;
					Sleep(10);
                	break;
                case ERROR_GEN_FAILURE:
                    /* The simplified usb boot code does result in this! */
                    /* Just continue as nothing has happend!!! */
                    ResetHandle(UsbDrv::HndlTx,outPipe);
					iResetCount++;
					Sleep(10);
                    break;
                case ERROR_BAD_COMMAND:
                    ResetHandle(UsbDrv::HndlTx,outPipe);
					iResetCount++;
					Sleep(10);
                    break;
				case ERROR_INVALID_HANDLE:
                    ResetHandle(UsbDrv::HndlTx,outPipe);
					iResetCount++;
					Sleep(10);
                    break;
                case ERROR_DEVICE_NOT_CONNECTED:
                    ResetHandle(UsbDrv::HndlTx,outPipe);
					iResetCount++;
					Sleep(10);
                    break;
                default:
                    ResetHandle(UsbDrv::HndlTx,outPipe);
					iResetCount++;
					Sleep(10);
                    break;
            }
        }
        written_total =+ written;    
    }
    return written_total;
}


/*-----------------------------------------------------------------------------
| Function    : UsbDrv::getRxEventHandle()
+------------------------------------------------------------------------------
| Description : Get RX event handle function
|
| Parameters  : None
|
| Returns     : HANDLE rxSignal
+----------------------------------------------------------------------------*/
HANDLE UsbDrv::getRxEventHandle()
{
    return UsbDrv::rxSignal;
}


/*-----------------------------------------------------------------------------
| Function    : UsbDrv::registerCallback()
+------------------------------------------------------------------------------
| Description : Register Callback Function
|
| Parameters  : 
|
| Returns     : void
+----------------------------------------------------------------------------*/
void UsbDrv::registerCallback(T_SERIAL_CALL_BACK callback)
{
    UsbDrv::CallBack = callback;
    return;
}


/*-----------------------------------------------------------------------------
| Function    : UsbDrv::unregisterCallback()
+------------------------------------------------------------------------------
| Description : Unregister function
|
| Parameters  : None
|
| Returns     : void
+----------------------------------------------------------------------------*/
void UsbDrv::unregisterCallback()
{
    UsbDrv::CallBack = NULL;
    return;
}


/*-----------------------------------------------------------------------------
| Function    : UsbDrv::read()
+------------------------------------------------------------------------------
| Description : USB Read Function
|
| Parameters  : 
|
| Returns     : S32
+----------------------------------------------------------------------------*/
S32 UsbDrv::read(U8* Buffer, U32 Size, U32 timeout)
{
    S32 ret = 0;
    if (Size == 0) 
    {
        Sleep(timeout);
    }
    else 
    {
        UsbDrv::setEventThreshold(Size);
        // Reading is pending.
        switch(WaitForSingleObject(UsbDrv::rxSignal, timeout)) 
        {
            // Overlapped event has been signaled.
            case WAIT_OBJECT_0:
                /* Store in buffer ... */
                ret = UsbDrv::UsbBuffer->pop(Buffer, Size);
                break;
            case WAIT_TIMEOUT:
                ret = WAIT_TIMEOUT;
                break;
            default:
                ret = GetLastError();
                ExitProcess(0); 
                break;
        }
    }
    return ret;
}


/*-----------------------------------------------------------------------------
| Function    : UsbDrv::setEventThreshold()
+------------------------------------------------------------------------------
| Description : USB Set Event Threshold Function
|
| Parameters  : 
|
| Returns     : void
+----------------------------------------------------------------------------*/
void UsbDrv::setEventThreshold(U32 num)
{
    EnterCriticalSection(&(UsbDrv::CriticalSection));
    UsbDrv::Threshold = num;

    if (UsbDrv::UsbBuffer->available() >= UsbDrv::Threshold) 
    {
        SetEvent(UsbDrv::rxSignal);
        UsbDrv::Threshold = 0;
    }
    LeaveCriticalSection(&(UsbDrv::CriticalSection));
    return;
}


/*-----------------------------------------------------------------------------
| Function    : UsbDrv::checkThreshold()
+------------------------------------------------------------------------------
| Description : USB Check Threshold Function
|
| Parameters  : 
|
| Returns     : void
+----------------------------------------------------------------------------*/
void UsbDrv::checkThreshold()
{
    EnterCriticalSection(&(UsbDrv::CriticalSection));

    if (UsbDrv::Threshold != 0) 
    {
        if (UsbDrv::UsbBuffer->available() >= UsbDrv::Threshold) 
        {
            SetEvent(UsbDrv::rxSignal);
            UsbDrv::Threshold = 0;
        }
    }
    LeaveCriticalSection(&(UsbDrv::CriticalSection));
    return;
}


/*-----------------------------------------------------------------------------
| Function    : UsbDrv::UsbListener()
+------------------------------------------------------------------------------
| Description : USB Check Threshold Function
|
| Parameters  : 
|
| Returns     : unsigned long
+----------------------------------------------------------------------------*/
unsigned long __stdcall UsbDrv::UsbListener(void* lpParam)
{
    U8 * rxbuf;
    U32 size = 0;
    bool StayAlive = true;

    rxbuf = (U8*) malloc(RX_BUF_SIZE);
    memset(rxbuf,0xFF,RX_BUF_SIZE); /* Reset buffer */
    UsbDrv* drv = (UsbDrv*) lpParam;
    
    COMMTIMEOUTS timeouts;                          
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    
    SetCommTimeouts(drv->HndlRx, &timeouts);

    while (StayAlive)
    {
        if (!ReadFile(drv->HndlRx, rxbuf, RX_BUF_SIZE, &size, NULL))
        {            
            // Issue read operation.
            switch(GetLastError()) 
            {
                case ERROR_READ_FAULT:
					size = -1;  /* Clean buffer */
                    drv->ResetHandle(drv->HndlRx,inPipe);
                    break;
                case ERROR_NOT_READY:
                    size = -1;  /* Clean buffer */
                    drv->ResetHandle(drv->HndlRx,inPipe);
                    break;
                case ERROR_GEN_FAILURE:
                    /* The simplified usb boot code does result in this! */
                    /* Just continue as nothing has happend!!! */
                    size = -1;  /* Clean buffer */
                    drv->ResetHandle(drv->HndlRx,inPipe);
                    break;
                case ERROR_INVALID_HANDLE:
					size = -1;  /* Clean buffer */
                    drv->ResetHandle(drv->HndlRx,inPipe);
                    break;
				case ERROR_BAD_COMMAND:
					size = -1;  /* Clean buffer */
                    drv->ResetHandle(drv->HndlRx,inPipe);
                    break;
                default:
                    size = -1;  /* Clean buffer */
                    drv->ResetHandle(drv->HndlRx,inPipe);
                    break;
            }
        }
        else
        {
            // Event driven or using call-back? 
            if (drv->CallBack == NULL) 
            {
                //Push the data to the buffer and signal to the upper layer if the threshold is reached.
                EnterCriticalSection(&(drv->CriticalSection));
                drv->UsbBuffer->push(rxbuf, size);
                drv->checkThreshold();
                LeaveCriticalSection(&(drv->CriticalSection));
            }
            else
            {
                drv->CallBack(rxbuf, size);
                // Clean the receive buffer. Not needed, but easier to debug with
                if ( size <= RX_BUF_SIZE) 
                { 
                    memset(rxbuf, 0xFF, size); 
                }
                else 
                {
                    memset(rxbuf, 0xFF, RX_BUF_SIZE); 
                }
            }

        }

    }

    free(rxbuf);
    CloseHandle(drv->HndlRx);
    return -1;
} 

/* -------------------------------------------------------------------------- */

HANDLE UsbDrv::OpenOneDevice ( HDEVINFO HardwareDeviceInfo, PSP_DEVICE_INTERFACE_DATA DeviceInfoData, char *devName )
/*
Routine Description:

    Given the HardwareDeviceInfo, representing a handle to the plug and
    play information, and deviceInfoData, representing a specific usb device,
    open that device and fill in all the relevant information in the given
    USB_DEVICE_DESCRIPTOR structure.

Arguments:

    HardwareDeviceInfo:  handle to info obtained from PnP mgr via SetupDiGetClassDevs()
    DeviceInfoData:      ptr to info obtained via SetupDiEnumDeviceInterfaces()

Return Value:

    return HANDLE if the open and initialization was successfull,
        else INVLAID_HANDLE_VALUE.

*/
{
    PSP_DEVICE_INTERFACE_DETAIL_DATA     functionClassDeviceData = NULL;
    ULONG                                predictedLength = 0;
    ULONG                                requiredLength = 0;
    HANDLE                               hOut = INVALID_HANDLE_VALUE;

    //
    // allocate a function class device data structure to receive the
    // goods about this particular device.
    //
    SetupDiGetDeviceInterfaceDetail (
            HardwareDeviceInfo,
            DeviceInfoData,
            NULL, // probing so no output buffer yet
            0, // probing so output buffer length of zero
            &requiredLength,
            NULL); // not interested in the specific dev-node


    predictedLength = requiredLength;
    // sizeof (SP_FNCLASS_DEVICE_DATA) + 512;

    functionClassDeviceData = PSP_DEVICE_INTERFACE_DETAIL_DATA (malloc (predictedLength));
    if(NULL == functionClassDeviceData) {
        return INVALID_HANDLE_VALUE;
    }
    functionClassDeviceData->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);

    //
    // Retrieve the information from Plug and Play.
    //
    if (! SetupDiGetDeviceInterfaceDetail (
               HardwareDeviceInfo,
               DeviceInfoData,
               functionClassDeviceData,
               predictedLength,
               &requiredLength,
               NULL)) {
                free( functionClassDeviceData );
        return INVALID_HANDLE_VALUE;
    }

        strcpy( devName,functionClassDeviceData->DevicePath) ;
        TRACE( "Attempting to open %s\n", devName );

    hOut = CreateFile (
                  functionClassDeviceData->DevicePath,
                  GENERIC_READ | GENERIC_WRITE,
                  FILE_SHARE_READ | FILE_SHARE_WRITE,
                  NULL, // no SECURITY_ATTRIBUTES structure
                  OPEN_EXISTING, // No special create flags
                  0, // No special attributes
                  NULL); // No template file

    if (INVALID_HANDLE_VALUE == hOut) {
                TRACE( "FAILED to open %s\n", devName );
    }
        free( functionClassDeviceData );
        return hOut;
}


HANDLE UsbDrv::OpenUsbDevice( LPGUID pGuid, char *outNameBuf)
/*
Routine Description:

   Do the required PnP things in order to find
   the next available proper device in the system at this time.

Arguments:

    pGuid:      ptr to GUID registered by the driver itself
    outNameBuf: the generated name for this device

Return Value:

    return HANDLE if the open and initialization was successful,
        else INVLAID_HANDLE_VALUE.
*/
{
   ULONG NumberDevices;
   HANDLE hOut = INVALID_HANDLE_VALUE;
   HDEVINFO                 hardwareDeviceInfo;
   SP_DEVICE_INTERFACE_DATA deviceInfoData;
   ULONG                    i;
   BOOLEAN                  done;
   PUSB_DEVICE_DESCRIPTOR   usbDeviceInst;
   PUSB_DEVICE_DESCRIPTOR       *UsbDevices = &usbDeviceInst;
   PUSB_DEVICE_DESCRIPTOR   tempDevDesc;

   *UsbDevices = NULL;
   tempDevDesc = NULL;
   NumberDevices = 0;

   //
   // Open a handle to the plug and play dev node.
   // SetupDiGetClassDevs() returns a device information set that contains info on all
   // installed devices of a specified class.
   //
   hardwareDeviceInfo = SetupDiGetClassDevs (
                           pGuid,
                           NULL, // Define no enumerator (global)
                           NULL, // Define no
                           (DIGCF_PRESENT | // Only Devices present
                            DIGCF_DEVICEINTERFACE)); // Function class devices.

   //
   // Take a wild guess at the number of devices we have;
   // Be prepared to realloc and retry if there are more than we guessed
   //
   NumberDevices = 4;
   done = FALSE;
   deviceInfoData.cbSize = sizeof (SP_DEVICE_INTERFACE_DATA);

   i=0;
   while (!done) {
      NumberDevices *= 2;

      if (*UsbDevices) {
            tempDevDesc = PUSB_DEVICE_DESCRIPTOR (
               realloc (*UsbDevices, (NumberDevices * sizeof (USB_DEVICE_DESCRIPTOR))));
            if(tempDevDesc) {
                *UsbDevices = tempDevDesc;
                tempDevDesc = NULL;
            }
            else {
                free(*UsbDevices);
                *UsbDevices = NULL;
            }
      } else {
         *UsbDevices = PUSB_DEVICE_DESCRIPTOR (calloc (NumberDevices, sizeof (USB_DEVICE_DESCRIPTOR)));
      }

      if (NULL == *UsbDevices) {

         // SetupDiDestroyDeviceInfoList destroys a device information set
         // and frees all associated memory.

         SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);
         return INVALID_HANDLE_VALUE;
      }

      usbDeviceInst = *UsbDevices + i;

      for (; i < NumberDevices; i++) {

         // SetupDiEnumDeviceInterfaces() returns information about device interfaces
         // exposed by one or more devices. Each call returns information about one interface;
         // the routine can be called repeatedly to get information about several interfaces
         // exposed by one or more devices.

         if (SetupDiEnumDeviceInterfaces (hardwareDeviceInfo,
                                         0, // We don't care about specific PDOs
                                                                                 pGuid,
                                         i,
                                         &deviceInfoData)) {

            hOut = OpenOneDevice (hardwareDeviceInfo, &deviceInfoData, outNameBuf);
                        if ( hOut != INVALID_HANDLE_VALUE ) {
               done = TRUE;
               break;
                        }
         } else {
            if (ERROR_NO_MORE_ITEMS == GetLastError()) {
               done = TRUE;
               break;
            }
         }
      }
   }

   NumberDevices = i;

   // SetupDiDestroyDeviceInfoList() destroys a device information set
   // and frees all associated memory.

   SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);
   free ( *UsbDevices );
   return hOut;
}


HANDLE UsbDrv::OpenDev()
{

        HANDLE hDEV = OpenUsbDevice( (LPGUID)&GUID_CLASS_TI_SAMPLE, completeDeviceName);


        if (hDEV == INVALID_HANDLE_VALUE) {
                TRACE("Failed to open (%s) = %d", completeDeviceName, GetLastError());
        } else {
                TRACE("DeviceName = (%s)\n", completeDeviceName);
    }           

        return hDEV;
}

BOOL UsbDrv::GetUsbDeviceFileName( LPGUID pGuid, char *outNameBuf)
/*
Routine Description:

    Given a ptr to a driver-registered GUID, give us a string with the device name
    that can be used in a CreateFile() call.
    Actually briefly opens and closes the device and sets outBuf if successfully;
    returns FALSE if not

Arguments:

    pGuid:      ptr to GUID registered by the driver itself
    outNameBuf: the generated zero-terminated name for this device

Return Value:

    TRUE on success else FALSE

*/
{
//        HANDLE hDev = OpenUsbDevice( pGuid, outNameBuf );


        HANDLE hDev = INVALID_HANDLE_VALUE;
                
        hDev = OpenUsbDevice( pGuid, outNameBuf );

        /* TODO: If anything went wrong interact with user... */
        
        if ( hDev != INVALID_HANDLE_VALUE )
        {
                CloseHandle( hDev );
                return TRUE;
        }
        return FALSE;

}


HANDLE UsbDrv::OpenFile( char *filename) {

        int success = 1;
        HANDLE h;


        if ( !GetUsbDeviceFileName(
                (LPGUID) &GUID_CLASS_TI_SAMPLE,
                completeDeviceName) )
        {
                TRACE(("Failed to GetUsbDeviceFileName err - %d\n", GetLastError()));
                return  INVALID_HANDLE_VALUE;
        }

        strcat (completeDeviceName,
                        "\\"
                        );                      

    if((strlen(completeDeviceName) + strlen(filename)) > 255) {
        TRACE(("Failed to open handle - possibly long filename\n"));
        return INVALID_HANDLE_VALUE;
    }

    strcat (completeDeviceName,
                        filename
                        );                                      

        TRACE("completeDeviceName = (%s)\n", completeDeviceName);

        h = CreateFile(completeDeviceName,
                GENERIC_WRITE | GENERIC_READ,
                NULL,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

        if (h == INVALID_HANDLE_VALUE) {
                TRACE(("Failed to open (%s) = %d", completeDeviceName, GetLastError()));
                success = 0;
        } else {
                        TRACE(("Opened successfully.\n"));
    }           
    return h;
}

/*==== END OF FILE ===========================================================*/

