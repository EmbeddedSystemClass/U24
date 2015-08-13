#ifndef __BaseDriver_H__ 
#define __BaseDriver_H__ 

#include "types.h" // U32/S32, U8/S8

typedef S8 (*PSERIAL_CALL_BACK)(
    U8* buf, 
    U32 size
);

typedef PSERIAL_CALL_BACK T_SERIAL_CALL_BACK;

#define SER_RX_BUF_SIZE (256*1024)
#define RX_BUF_SIZE (2048)

#ifdef DLLAPI
#undef DLLAPI
#endif

#ifdef SERIAL_DRV_EXPORTS
#define DLLAPI	__declspec(dllexport)
#else
#define DLLAPI	__declspec(dllimport)
#endif

class DLLAPI BaseDrv {
    public:
        virtual ~BaseDrv(){};
		// 2006-12-28 YTT: modify Port type from U8 to U32 for USB device
        virtual S8 open(U32 Port, U32 BaudRate, U8 Parity, U8 StopBits, U8 Data) = 0;
        virtual S8 close() = 0;
        virtual S8 reset() = 0;
        virtual S8 configure(U32 BaudRate, U8 Parity, U8 StopBits, U8 Data) = 0;
        virtual S32 write(U8* Buffer, U32 Size) = 0;
        virtual S32 read(U8* Buffer, U32 Size, U32 timeout) = 0;
        virtual HANDLE getRxEventHandle() = 0;
        virtual void setEventThreshold(U32 num) = 0;
        virtual void registerCallback(T_SERIAL_CALL_BACK callback) = 0;
        virtual void unregisterCallback() = 0;
    private:
    protected:
};

#endif // __BaseDriver_H__
