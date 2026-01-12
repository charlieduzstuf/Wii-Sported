#ifndef RVL_SDK_OS_ALLOC_H
#define RVL_SDK_OS_ALLOC_H
#include <types.h>

#ifdef PLATFORM_PC
// PC memory allocation using standard malloc/free
#include <stdlib.h>

// Simplified heap system for PC
#define OSAllocFromHeap(handle, size) malloc(size)
#define OSFreeToHeap(handle, p) free(p)
#define OSSetCurrentHeap(handle) (handle)
#define OSInitAlloc(start, end, numHeaps) (start)
#define OSCreateHeap(start, end) (0)

static volatile s32 __OSCurrHeap = 0;

#else
// Original Wii implementation
#ifdef __cplusplus
extern "C" {
#endif

extern volatile s32 __OSCurrHeap;

void* OSAllocFromHeap(s32 handle, s32 size);
void OSFreeToHeap(s32 handle, void* p);
s32 OSSetCurrentHeap(s32 handle);
void* OSInitAlloc(void* start, void* end, s32 numHeaps);
s32 OSCreateHeap(void* start, void* end);

#ifdef __cplusplus
}
#endif
#endif // Wii implementation

#endif // RVL_SDK_OS_ALLOC_H
