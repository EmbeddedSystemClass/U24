#ifndef __TcpDriver_H__ 
#define __TcpDriver_H__ 

#include <winsock2.h>  // 2010-06-22 Evonne: for TCP port
#pragma comment(lib, "ws2_32.lib")
#include "types.h" // U32/S32, U8/S8
#include "CircBuffer.h"
#include "comdriver.h"
#include <string>


//class DLLAPI TcpDrv : public BaseDrv {
class TcpDrv : public BaseDrv {
    public:
		HANDLE getHandle();
		TcpDrv();
		~TcpDrv();
		// 2006-12-28 YTT: modify Port type from U8 to U32 for USB device
		virtual S8 open(U32 Port, U32 BaudRate, U8 Parity, U8 StopBits, U8 Data);
		virtual S8 close();
		virtual S8 reset();
		virtual S8 configure(U32 BaudRate, U8 Parity, U8 StopBits, U8 Data);
		virtual S32 write(U8* Buffer, U32 Size);
		virtual S32 read(U8* Buffer, U32 Size, U32 timeout);
		virtual HANDLE getRxEventHandle();
		virtual void setEventThreshold(U32 num);
		virtual void registerCallback(T_SERIAL_CALL_BACK callback);
		virtual void unregisterCallback();
    private:
		SOCKET sockfd;
		U8 isOpen;
		HANDLE ThreadHndl, rxSignal;
		U32 ThreadID;
		U32 Threshold;
		CircBuffer* TcpBuffer;
		T_SERIAL_CALL_BACK CallBack;
		CRITICAL_SECTION CriticalSection; 
		void checkThreshold();
		static unsigned long __stdcall TcpListener(void* lpParam);
		bool ExecAdb(std::string Command);
		int OpenSocket();
		S8 ResetSocket(SOCKET sockfd);
    protected:
};

#endif /* __TcpDriver_H__  */

