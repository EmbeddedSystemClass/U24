#ifndef __CircBuffer_H__ 
#define __CircBuffer_H__ 

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "types.h"

#define CIRC_BUF_OK 0
#define CIRC_BUF_ALLOC_FAIL  -3
#define CIRC_BUF_NOT_ENOUGH_SPACE  -4
#define CIRC_BUF_NOT_IMPLEMENTED -10

#define BUF_DEFAULT_SIZE (32*1024)  /* 4 KB */

class CircBuffer {
    public:
        CircBuffer(U32 size = BUF_DEFAULT_SIZE);
        ~CircBuffer();
        S32 push(U8* buf, U32 size);
        S32 pop(U8* buf, U32 size);
        S32 freeSpace();
        U32 available();
    private:
        U8* Buffer;
        S8 ErrorCode;
        U32 BufferSize;
        U32 ReadPtr, WritePtr;
        U32 BytesFree, BytesAvailable;
        CRITICAL_SECTION CriticalSection;
    protected:
};

#endif /* __CircBuffer_H_ */