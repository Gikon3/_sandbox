#include "assert.h"
#include "string.h"
#include "RingBuffer.h"

void RingBufInit(RingBuffer_t* pRingBuf, void* pBuffer, size_t Size)
{
    assert(pRingBuf);

    pRingBuf->pBegin = pBuffer;
    pRingBuf->pEnd = (char*)pBuffer + Size;
}

void RingBufDeinit(RingBuffer_t* pRingBuf)
{
    assert(pRingBuf);

    pRingBuf->pBegin = NULL;
    pRingBuf->pEnd = NULL;
}

void* RingBufMoveCursor(const RingBuffer_t* pRingBuf, void* pCursor, uint32_t Value)
{
    assert(pRingBuf);
    assert((char*)pCursor >= pRingBuf->pBegin && (char*)pCursor < pRingBuf->pEnd);
    assert(Value < (pRingBuf->pEnd - pRingBuf->pBegin));

    const size_t SpaceLeft = pRingBuf->pEnd - (char*)pCursor;

    return SpaceLeft > Value ? (char*)pCursor + Value : pRingBuf->pBegin + (Value - SpaceLeft);
}

void* RingBufFind(const RingBuffer_t* pRingBuf, void* pBegin, void* pEnd, const void* pPattern, size_t Size)
{
    assert(pRingBuf);
    assert((char*)pBegin >= pRingBuf->pBegin && (char*)pBegin < pRingBuf->pEnd);
    assert((char*)pEnd >= pRingBuf->pBegin && (char*)pEnd <= pRingBuf->pEnd);
    assert(pPattern);

    while ((char*)pBegin != (char*)pEnd)
    {
        int i = 0;

        for (char* pIterData = (char*)pBegin; i < Size; ++i)
        {
            if (*pIterData != ((uint8_t*)pPattern)[i])
                break;

            pIterData = RingBufMoveCursor(pRingBuf, pIterData, 1);
        }

        if (i == Size)
            return pBegin;

        pBegin = RingBufMoveCursor(pRingBuf, pBegin, 1);
    }

    return NULL;
}

size_t RingBufSpaceUsed(const RingBuffer_t* pRingBuf, const void* pBegin, const void* pEnd)
{
    assert(pRingBuf);

    return (char*)pEnd >= (char*)pBegin ? (char*)pEnd - (char*)pBegin :
        (pRingBuf->pEnd - (char*)pBegin) + ((char*)pEnd - pRingBuf->pBegin);
}

size_t RingBufCopy(const RingBuffer_t* pRingBuf, const void* pBegin, const void* pEnd, void* pData)
{
    assert(pRingBuf);
    assert((char*)pBegin >= pRingBuf->pBegin && (char*)pBegin < pRingBuf->pEnd);
    assert((char*)pEnd >= pRingBuf->pBegin && (char*)pEnd <= pRingBuf->pEnd);
    assert(pData);

    size_t Size = 0;

    if ((char*)pEnd >= (char*)pBegin)
    {
        Size = (char*)pEnd - (char*)pBegin;
        memcpy(pData, pBegin, Size);
    }
    else
    {
        Size = pRingBuf->pEnd - (char*)pBegin;
        memcpy(pData, pBegin, Size);
        const size_t CopySize2 = (char*)pEnd - pRingBuf->pBegin;
        memcpy(&((char*)pData)[Size], pRingBuf->pBegin, CopySize2);
        Size += CopySize2;
    }

    return Size;
}