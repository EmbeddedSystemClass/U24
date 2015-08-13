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
| Filename:   serialdriver.cpp
| Author:     Thomas Lund Soehus (tls@ti.com)
|
| Purpose:    This file contains the serial driver for CSST Host side.
+----------------------------------------------------------------------------*/

/*==== DECLARATION CONTROL ==================================================*/

/*==== INCLUDES =============================================================*/
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "SerialDriver.h"


/*==== MACROS ===============================================================*/
/*==== CONSTS ===============================================================*/
/*==== TYPES ================================================================*/
/*==== GLOBALS ==============================================================*/
/*==== PUBLIC FUNCTIONS =====================================================*/

/*-----------------------------------------------------------------------------
| Function    : SerialDrv()
+------------------------------------------------------------------------------
| Description : Serial Driver Function
|
| Parameters  : void
|
| Returns     : None
+----------------------------------------------------------------------------*/
SerialDrv::SerialDrv()
{
	SerialDrv::Hndl = NULL;
	SerialDrv::ThreadHndl = NULL;
	SerialDrv::rxSignal = NULL;
	SerialDrv::CallBack = NULL;
	memset(&(SerialDrv::readOverlapped), 0, sizeof(SerialDrv::readOverlapped));
	memset(&(SerialDrv::writeOverlapped), 0, sizeof(SerialDrv::writeOverlapped));

	InitializeCriticalSection(&(SerialDrv::CriticalSection));

	SerialDrv::Threshold = 0;
	SerialBuffer = new CircBuffer(SER_RX_BUF_SIZE);
	/* Default state is not opened. */
	isOpen = 0;
}


/*-----------------------------------------------------------------------------
| Function    : ~SerialDrv()
+------------------------------------------------------------------------------
| Description : Serial Driver Decontructor
|
| Parameters  : void
|
| Returns     : None
+----------------------------------------------------------------------------*/
SerialDrv::~SerialDrv()
{
	SerialDrv::close();
	if (SerialBuffer != NULL) {
		delete SerialBuffer;
		SerialBuffer = NULL;
	}
}


/*-----------------------------------------------------------------------------
| Function    : SerialDrv::open()
+------------------------------------------------------------------------------
| Description : Open function for the serial driver.
|
| Parameters  : [IN] Port - The serial COM port.
|               [IN] BaudRate - The serial communication speed [bps].
|               [IN] Parity - Even or ord perity bit can be set.
|               [IN] Data - The data bits which can be either 5, 6, 7 or 8 bits
|
| Returns     : None
+----------------------------------------------------------------------------*/
S8 SerialDrv::open(U32 Port, U32 BaudRate, U8 Parity, U8 StopBits, U8 Data)
{
	/**************** TEMPORARY SOLUTION ******************/
	/* This is a temporary solution for solving an issue  */
	/* where an ACCESS_DENIED is returned when opening    */
	/* the UART after ROM-code communication. The issue   */
	/* is only present when executing csst.exe directly,  */
	/* and not when executing from MS Visual or using DL  */
	/* with the test application. With this in mind, it is*/
	/* suspected that the issue is due to timing of the   */
	/* threads.
	/* The correct solution is to verify that the UART is */
	/* properly released before sending the connect req.  */
	Sleep(1000);

	if (isOpen)
		return -1; /* PORT_IS_ALREADY_OPEN */

	SerialDrv::readOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(SerialDrv::readOverlapped.hEvent);
	SerialDrv::writeOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(SerialDrv::writeOverlapped.hEvent);
	SerialDrv::rxSignal = CreateEvent(NULL, FALSE, FALSE, NULL);
	ResetEvent(SerialDrv::rxSignal);

	char comPort[20];
	sprintf(comPort, "%s%d", "\\\\.\\COM", Port);

	SerialDrv::Hndl = CreateFile(comPort,                   // Specify port device.
	                             GENERIC_READ | GENERIC_WRITE,// Specify mode that open device.
	                             0,                         // The devide isn't shared.
	                             NULL,                      // The object gets a default security.
	                             OPEN_EXISTING,             // Specify which action to take on file.
	                             FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // File attributes
	                             0);                        // Default
	DWORD ret = GetLastError();

	if (SerialDrv::Hndl == NULL) {
		return -1;
	}
	if (SerialDrv::configure(BaudRate, Parity, StopBits, Data) != 0) {
		return -1;
	}
	SetCommMask(SerialDrv::Hndl, EV_RXCHAR);
	SerialDrv::ThreadHndl = CreateThread(NULL,
	                                     0,
	                                     SerialDrv::serialListener,
	                                     (void*) this,
	                                     0,
	                                     &(SerialDrv::ThreadID));
	if (!SerialDrv::ThreadHndl) {
		SerialDrv::close();
		return -1;
	}
	SetThreadPriority(SerialDrv::ThreadHndl, THREAD_PRIORITY_ABOVE_NORMAL);
	SerialDrv::reset();
	isOpen = 1;
	return 0;
}


/*-----------------------------------------------------------------------------
| Function    : SerialDrv::close()
+------------------------------------------------------------------------------
| Description : Closing function for the serial driver.
|
| Parameters  : void
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S8 SerialDrv::close(void)
{
	// 2009-09-28 Mick add criticial section for TerminateThread
	EnterCriticalSection(&(SerialDrv::CriticalSection));
	U32 exitCode;
	GetExitCodeThread(SerialDrv::ThreadHndl, &exitCode);
	if (exitCode == STILL_ACTIVE) {
		TerminateThread(SerialDrv::ThreadHndl, exitCode);
	}
	LeaveCriticalSection(&(SerialDrv::CriticalSection));
	if (SerialDrv::ThreadHndl != NULL) {
		CloseHandle(SerialDrv::ThreadHndl);
		SerialDrv::ThreadHndl = NULL;
	}

	if (SerialDrv::Hndl != NULL) {
		CloseHandle(SerialDrv::Hndl);
	}
	if (SerialDrv::rxSignal != NULL) {
		CloseHandle(SerialDrv::rxSignal);
	}
	if (SerialDrv::readOverlapped.hEvent != NULL) {
		CloseHandle(SerialDrv::readOverlapped.hEvent);
	}
	if (SerialDrv::writeOverlapped.hEvent != NULL) {
		CloseHandle(SerialDrv::writeOverlapped.hEvent);
		memset(&(SerialDrv::readOverlapped), 0, sizeof(SerialDrv::readOverlapped));
		memset(&(SerialDrv::writeOverlapped), 0, sizeof(SerialDrv::writeOverlapped));
		SerialDrv::Hndl = NULL;
		SerialDrv::rxSignal = NULL;
		SerialDrv::isOpen = 0;
	}
	return 0;
}


/*-----------------------------------------------------------------------------
| Function    : SerialDrv::reset()
+------------------------------------------------------------------------------
| Description : Reset function for the serial driver.
|
| Parameters  : void
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S8 SerialDrv::reset(void)
{
	return PurgeComm(SerialDrv::Hndl, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
}


/*-----------------------------------------------------------------------------
| Function    : SerialDrv::configure()
+------------------------------------------------------------------------------
| Description : Function for configuring the serial driver.
|
| Parameters  : [IN] BaudRate - Configuring the baud rate for the serial driver.
|               [IN] Parity - Configuring pariy bit.
|               [IN] StopBits - 1, 1? or 2 stop bits can be configured.
|               [IN] Data - Databits which can be either 5, 6, 7, or 8 bits.
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S8 SerialDrv::configure(U32 BaudRate, U8 Parity, U8 StopBits, U8 Data)
{
	DCB dcb = {0};

	COMMTIMEOUTS timeouts; // instance an object of COMMTIMEOUTS.

	dcb.DCBlength = sizeof(DCB);

	// Get current configuration of serial communication port.
	if (!GetCommState (SerialDrv::Hndl, &dcb)) {
		// Get configuration port has a problem.
		TRACE("CSerialCommHelper : Failed to Get Comm State Reason: %d\n", GetLastError());
		return -1;
	}

	dcb.fAbortOnError = FALSE;
	dcb.BaudRate = BaudRate;    // Specify buad rate of communicaiton.
	dcb.ByteSize = Data;        // Specify  byte of size of communication.
	dcb.Parity = Parity;        // Specify parity of communication.
	dcb.StopBits = StopBits;    // Specify stopbit of communication.
	dcb.fOutxCtsFlow = 0;
	dcb.fOutxDsrFlow = 0;
	dcb.fDsrSensitivity = 0;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;


	//set as COM5
	dcb.XonChar = 0;
	dcb.XoffChar = 0;

	/*
	if (Parity==EVENPARITY)
	{
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
	}
	else
	{
		dcb.fRtsControl = RTS_CONTROL_ENABLE;
		dcb.fAbortOnError=1;
		dcb.XonLim=80;
		dcb.XoffLim=200;
	}*/


	if (!SetCommState (SerialDrv::Hndl, &dcb)) {
		// Set configuration port has a problem.
		TRACE( "CSerialCommHelper : Failed to Set Comm State Reason: %d\n", GetLastError());
		return -1;
	}


	/* Set time-out communication. */

	// Specify time-out between charactor for receiving.
	timeouts.ReadIntervalTimeout = 0;
	// Specify value that is multiplied
	// by the requested number of bytes to be read.
	timeouts.ReadTotalTimeoutMultiplier = 0;
	// Specify value is added to the product of the
	// ReadTotalTimeoutMultiplier member
	timeouts.ReadTotalTimeoutConstant = 0;
	// Specify value that is multiplied
	// by the requested number of bytes to be sent.
	timeouts.WriteTotalTimeoutMultiplier = 0;
	// Specify value is added to the product of the
	// WriteTotalTimeoutMultiplier member
	timeouts.WriteTotalTimeoutConstant = 0;

	SetCommTimeouts (SerialDrv::Hndl, &timeouts);

	TRACE( "Serial Configuration : (Baud Rate %d; Parity %d; "
	       "Byte Size %d; Stop Bits %d \n\n", dcb.BaudRate, dcb.Parity, dcb.ByteSize, dcb.StopBits);

	return 0;
}


/*-----------------------------------------------------------------------------
| Function    : SerialDrv::write()
+------------------------------------------------------------------------------
| Description : Write function for the serial driver.
|
| Parameters  :
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S32 SerialDrv::write(U8* Buffer, U32 Size)
{
	U32 tst, written = 0;

	tst = WriteFile(SerialDrv::Hndl, Buffer, Size, &written, &(SerialDrv::writeOverlapped));
	if (tst == 0) {
		// Reading has problem.
		WaitForSingleObject(SerialDrv::writeOverlapped.hEvent, INFINITE);
		ResetEvent(SerialDrv::writeOverlapped.hEvent);
		written = Size;
	}
	return written;
}


/*-----------------------------------------------------------------------------
| Function    : SerialDrv::getRxEventHandle()
+------------------------------------------------------------------------------
| Description : Receive the RX event handle
|
| Parameters  :
|
| Returns     : RX HANDLE
+----------------------------------------------------------------------------*/
HANDLE SerialDrv::getRxEventHandle()
{
	return SerialDrv::rxSignal;
}



/*-----------------------------------------------------------------------------
| Function    : SerialDrv::registerCallback()
+------------------------------------------------------------------------------
| Description : Register the callback function
|
| Parameters  :
|
| Returns     : void
+----------------------------------------------------------------------------*/
void SerialDrv::registerCallback(T_SERIAL_CALL_BACK callback)
{
	SerialDrv::CallBack = callback;
	return;
}


/*-----------------------------------------------------------------------------
| Function    : SerialDrv::unregisterCallback()
+------------------------------------------------------------------------------
| Description : Unregister the callback function
|
| Parameters  :
|
| Returns     : S8
+----------------------------------------------------------------------------*/
void SerialDrv::unregisterCallback()
{
	SerialDrv::CallBack = NULL;
	return;
}


/*-----------------------------------------------------------------------------
| Function    : SerialDrv::read()
+------------------------------------------------------------------------------
| Description : Serial Read Function
|
| Parameters  :
|
| Returns     : S32
+----------------------------------------------------------------------------*/
S32 SerialDrv::read(U8* Buffer, U32 Size, U32 timeout)
{
	S32 ret = 0;

	if (Size == 0) {
		Sleep(timeout);
	}
	else {
		SerialDrv::setEventThreshold(Size);
		switch (WaitForSingleObject(SerialDrv::rxSignal, timeout)) {
		case WAIT_OBJECT_0:
			/* Store in buffer ... */
			ret = SerialDrv::SerialBuffer->pop(Buffer, Size);
			break;
		case WAIT_TIMEOUT:
			ret = WAIT_TIMEOUT;
			break;
		default:
			ret = GetLastError();
			break;
		}
	}
	return ret;
}


/*-----------------------------------------------------------------------------
| Function    : SerialDrv::setEventThreshold()
+------------------------------------------------------------------------------
| Description : Serial Read Function
|
| Parameters  :
|
| Returns     : void
+----------------------------------------------------------------------------*/
void SerialDrv::setEventThreshold(U32 num)
{
	EnterCriticalSection(&(SerialDrv::CriticalSection));
	SerialDrv::Threshold = num;
	if (SerialDrv::SerialBuffer->available() >= SerialDrv::Threshold) {
		SetEvent(SerialDrv::rxSignal);
		SerialDrv::Threshold = 0;
	}
	LeaveCriticalSection(&(SerialDrv::CriticalSection));
	return;
}


/*-----------------------------------------------------------------------------
| Function    : SerialDrv::checkThreshold()
+------------------------------------------------------------------------------
| Description : Check Threshold Function
|
| Parameters  : void
|
| Returns     : void
+----------------------------------------------------------------------------*/
void SerialDrv::checkThreshold()
{
	EnterCriticalSection(&(SerialDrv::CriticalSection));
	if (SerialDrv::Threshold != 0) {
		if (SerialDrv::SerialBuffer->available() >= SerialDrv::Threshold) {
			SetEvent(SerialDrv::rxSignal);
			SerialDrv::Threshold = 0;
		}
	}
	LeaveCriticalSection(&(SerialDrv::CriticalSection));
	return;
}


/*-----------------------------------------------------------------------------
| Function    : SerialDrv::serialListener()
+------------------------------------------------------------------------------
| Description : Check Threshold Function
|
| Parameters  : void
|
| Returns     : unsigned long
+----------------------------------------------------------------------------*/
unsigned long __stdcall SerialDrv::serialListener(void* lpParam)
{
	U32 event_mask;
	U8 buf[64];
	U32 size;

	SerialDrv* drv = (SerialDrv*)lpParam;

	while (1) {
		// Wait the event signalling characters received.
		if (WaitCommEvent (drv->Hndl, &event_mask, NULL) != TRUE) {
			TRACE("WaitCommEvent(): error %d\n", GetLastError());
			return -1;
		}

		do {
			if (!ReadFile(drv->Hndl, buf, 1, &size, &drv->readOverlapped)) {
				if (GetLastError() == ERROR_IO_PENDING) {
					GetOverlappedResult(drv->Hndl, &drv->readOverlapped, &size, TRUE);
				}
				else {
					TRACE("ReadFile(): error %d\n", GetLastError());
					drv->reset();
					break;
				}
			}
			// Event driven or using call-back?
			if (drv->CallBack == NULL) {
				// Push the data to the buffer and signal to the upper layer if the threshold is reached.
				EnterCriticalSection(&(drv->CriticalSection));
				drv->SerialBuffer->push(buf, size);
				drv->checkThreshold();
				LeaveCriticalSection(&(drv->CriticalSection));
			}
			else {
				drv->CallBack(buf, size);
			}
		} while (size);
	}
	return -1;
}


/*==== END OF FILE ===========================================================*/


HANDLE SerialDrv::getHandle()
{
	return SerialDrv::Hndl;
}

