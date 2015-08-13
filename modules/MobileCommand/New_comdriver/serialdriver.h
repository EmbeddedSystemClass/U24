#ifndef __SerialDriver_H__ 
#define __SerialDriver_H__ 

#include "types.h" // U32/S32, U8/S8
#include "CircBuffer.h"
#include "Comdriver.h"

class DLLAPI SerialDrv : public BaseDrv {
public:
	HANDLE getHandle();
	SerialDrv();
	~SerialDrv();
	// 2006-12-28 YTT: modify Port type from U8 to U32 for USB device
	S8 open(U32 Port, U32 BaudRate, U8 Parity, U8 StopBits, U8 Data);
	S8 close();
	S8 reset();
	S8 configure(U32 BaudRate, U8 Parity, U8 StopBits, U8 Data);
	S32 write(U8* Buffer, U32 Size);
	S32 read(U8* Buffer, U32 Size, U32 timeout);
	HANDLE getRxEventHandle();
	void setEventThreshold(U32 num);
	void registerCallback(T_SERIAL_CALL_BACK callback);
	void unregisterCallback();
private:
	U8 isOpen;
	HANDLE Hndl, ThreadHndl, rxSignal;
	OVERLAPPED readOverlapped, writeOverlapped;
	CRITICAL_SECTION CriticalSection;
	U32 ThreadID;
	T_SERIAL_CALL_BACK CallBack;
	CircBuffer* SerialBuffer;
	U32 Threshold;

	void checkThreshold();
	static unsigned long __stdcall serialListener(void* lpParam);
protected:
};

#endif // __SerialDriver_H__
