#include <stdio.h>
#include "RingBuffer.h"

static char Buffer[16] = "123456789EF1CDEF";

int main()
{
    RingBuffer_t RingBuf;
    RingBufInit(&RingBuf, Buffer, sizeof(Buffer));

    const char* p123 = (char*)RingBufFind(&RingBuf, Buffer, Buffer + 4, "123", 3);
    const char* pDEF = (char*)RingBufFind(&RingBuf, Buffer, Buffer + sizeof(Buffer), "DEF", 3);
    const char* pDEF2 = (char*)RingBufFind(&RingBuf, Buffer + 1, Buffer, "DEF", 3);
    const char* pEF12 = (char*)RingBufFind(&RingBuf, Buffer + 4, Buffer + 3, "EF12", 4);
    const char* p456 = (char*)RingBufFind(&RingBuf, Buffer + 6, Buffer + 5, "456", 3);

    printf("123 %s\n", p123);
    printf("DEF %s\n", pDEF);
    printf("DEF2 %s\n", pDEF2);
    printf("EF12 %s\n", pEF12);
    printf("456 %s\n", p456);
}