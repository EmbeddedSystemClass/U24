#if !defined(BQPACKETLIB_H__3D46E606_32E3_47C4_9053_68ECEEF82D74__INCLUDED_)
#define BQPACKETLIB_H__3D46E606_32E3_47C4_9053_68ECEEF82D74__INCLUDED_
#include "CRCMachine.h"
//#include "Logger.h"
// 2004/09/22 Embert Tsai
#define PACKET_NULL_CHAR NULL

#define RUN_SUCCESSFULLY	1
#define RUN_FAILED			(-1)
#define ERROR_MASK			0xF0000000

class CBQPacketLib {
public:
    enum ErrorType {
	    ERROR_CBQPACKETLIB_CRC_ERROR	=	ERROR_MASK | 0x20101,	// CRC error
		ERROR_CBQPACKETLIB_BUFFER_ERROR	=	ERROR_MASK | 0x20102,
	    ERROR_CBQPACKETLIB_UNKNOWN		=	ERROR_MASK | 0x201FF	// unknown error catched by run-time lib	
	};
	static int PackBuffer(const void *pvSrc, int nSrcLen, CHAR chStart, CHAR chEnd, void **pvDest);
	static int UnpackBuffer(const void *pvSrc, int nSrcLen, CHAR chStart, CHAR chEnd, void **pvDest);
};
#endif