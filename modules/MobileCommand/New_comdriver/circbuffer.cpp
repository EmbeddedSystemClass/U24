#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "CircBuffer.h"

CircBuffer::CircBuffer(U32 size)
{
    CircBuffer::ErrorCode = CIRC_BUF_OK;
    CircBuffer::BufferSize = size;
    CircBuffer::ReadPtr = CircBuffer::WritePtr = 0;
    CircBuffer::BytesFree = size;
    CircBuffer::BytesAvailable = 0;

    InitializeCriticalSection(&(CircBuffer::CriticalSection));
    CircBuffer::Buffer = (U8*) malloc(CircBuffer::BufferSize);
    if (CircBuffer::Buffer == NULL)
        CircBuffer::ErrorCode = CIRC_BUF_ALLOC_FAIL;
    return;
}

CircBuffer::~CircBuffer()
{
    if (CircBuffer::ErrorCode!= CIRC_BUF_OK)
        return;
    free(CircBuffer::Buffer);
}

S32 CircBuffer::push(U8* buf, U32 size)
{
    if (CircBuffer::ErrorCode != CIRC_BUF_OK)
        return CircBuffer::ErrorCode;

    EnterCriticalSection(&(CircBuffer::CriticalSection));
    if (size > CircBuffer::BytesFree) {
        TRACE("Oups - Not enough space in buffer !!\n");
        LeaveCriticalSection(&(CircBuffer::CriticalSection));
        return CIRC_BUF_NOT_ENOUGH_SPACE;
    }

    while (size--) {
        CircBuffer::Buffer[CircBuffer::WritePtr] = *buf;
        CircBuffer::WritePtr = ((CircBuffer::WritePtr + 1) % CircBuffer::BufferSize);
        CircBuffer::BytesFree--;
        CircBuffer::BytesAvailable++;
        buf++;
    }
    LeaveCriticalSection(&(CircBuffer::CriticalSection));

    return CIRC_BUF_OK;
}

S32 CircBuffer::pop(U8* buf, U32 size)
{
    U32 bufSize = size;
    U8* buffer = buf;
    
    if (CircBuffer::ErrorCode != CIRC_BUF_OK)
        return CircBuffer::ErrorCode;

    EnterCriticalSection(&(CircBuffer::CriticalSection));
    if (CircBuffer::BytesAvailable < bufSize) {
        bufSize = CircBuffer::BytesAvailable;
        TRACE("Oups - Less bytes in buffer than expected Wr: %d  Rd:%d (bufsize: %d)\n", CircBuffer::WritePtr, CircBuffer::ReadPtr, bufSize);
    }

    for (U32 i = 0; i < bufSize; i++) {
        *buffer = CircBuffer::Buffer[CircBuffer::ReadPtr];
        CircBuffer::ReadPtr = ((CircBuffer::ReadPtr + 1) % CircBuffer::BufferSize);
        CircBuffer::BytesFree++;
        CircBuffer::BytesAvailable--;
        buffer++;
    }
    LeaveCriticalSection(&(CircBuffer::CriticalSection));
    
    return bufSize;
}

S32 CircBuffer::freeSpace()
{
    S32 ret;
    
    if (CircBuffer::ErrorCode != CIRC_BUF_OK)
        return CircBuffer::ErrorCode;

    EnterCriticalSection(&(CircBuffer::CriticalSection));
    ret = CircBuffer::BytesFree;
    LeaveCriticalSection(&(CircBuffer::CriticalSection));
    return ret;
}

U32 CircBuffer::available()
{
    U32 ret;
    if (CircBuffer::ErrorCode != CIRC_BUF_OK)
        return CircBuffer::ErrorCode;
    
    EnterCriticalSection(&(CircBuffer::CriticalSection));
    ret = CircBuffer::BytesAvailable;
    LeaveCriticalSection(&(CircBuffer::CriticalSection));
    return ret;
}

