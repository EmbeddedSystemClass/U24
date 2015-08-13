#ifndef __TcpDriver_H__ 
#define __TcpDriver_H__ 

#include <winsock2.h>  // 2010-06-22 Evonne: for TCP port
#pragma comment(lib, "ws2_32.lib")
#include "types.h" // U32/S32, U8/S8
#include "CircBuffer.h"
#include "comdriver.h"
#include <string>


#define SERV_PORT 9600
#define TARG_PORT 9600

#define SERV_PORT_1		9601
#define SERV_PORT_2		9602
#define SERV_PORT_3		9603
#define SERV_PORT_4		9604

#define QPHONE_NAME_1	"QPHONE1"
#define QPHONE_NAME_2	"QPHONE2"
#define QPHONE_NAME_3	"QPHONE3"
#define QPHONE_NAME_4	"QPHONE4"


class DLLAPI TcpDrv : public BaseDrv {
    public:
		HANDLE getHandle();
		TcpDrv();
		~TcpDrv();
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
		int OpenSocket(int nMultiCtrlPort = 2000);
		S8 ResetSocket(SOCKET sockfd);
    protected:
};

#endif /* __TcpDriver_H__  */

