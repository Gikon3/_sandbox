#ifndef UTIL_RING_BUFFER_H_
#define UTIL_RING_BUFFER_H_

#include "stddef.h"
#include "stdint.h"

typedef struct
{
	char* pBegin;
	char* pEnd;
} RingBuffer_t;

void RingBufInit(RingBuffer_t* pRingBuf, void* pBuffer, size_t Size);
void RingBufDeinit(RingBuffer_t* pRingBuf);

void* RingBufFind(const RingBuffer_t* pRingBuf, void* pBegin, void* pEnd, const void* pPattern, size_t Size);
size_t RingBufSpaceUsed(const RingBuffer_t* pRingBuf, const void* pBegin, const void* pEnd);
void* RingBufMoveCursor(const RingBuffer_t* pRingBuf, void* pCursor, uint32_t Value);
size_t RingBufCopy(const RingBuffer_t* pRingBuf, const void* pBegin, const void* pEnd, void* pData);

#endif /* UTIL_RING_BUFFER_H_ */
