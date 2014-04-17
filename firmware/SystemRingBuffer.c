/*
 * Ring Buffer Implementation
 */

#include "SystemRingBuffer.h"

/* Read/Write *****************************************************************/

bool SystemRingBufferRead(volatile SystemRingBuffer_t *buf, uint32_t *elem)
{
    if(!SystemRingBufferIsEmpty(buf))
    {
        *elem = buf->Buffer[buf->ReadPos];
        buf->Count--;
        buf->ReadPos++;
        buf->ReadPos = buf->ReadPos >= buf->Size ? 0 : buf->ReadPos;
        return true;
    }
    else
    {
        elem = NULL;
        return false;
    }
}

bool SystemRingBufferWrite(volatile SystemRingBuffer_t *buf,
    const uint32_t elem)
{
    if(!SystemRingBufferIsFull(buf))
    {
        buf->Buffer[buf->WritePos] = elem;
        buf->Count++;
        buf->WritePos++;
        buf->WritePos = buf->WritePos >= buf->Size ? 0 : buf->WritePos;
        return true;
    }
    else
    {
        return false;
    }
}

/* Status *********************************************************************/

bool SystemRingBufferIsEmpty(volatile SystemRingBuffer_t *buf)
{
    return (buf->Count == 0);
}

bool SystemRingBufferIsFull(volatile SystemRingBuffer_t *buf)
{
    return (buf->Count == buf->Size);
}

