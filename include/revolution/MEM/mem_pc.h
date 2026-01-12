#ifndef PLATFORM_PC_MEM_H
#define PLATFORM_PC_MEM_H

#ifdef PLATFORM_PC

#include <types.h>
#include <stdlib.h>
#include <string.h>

// Simplified MEM implementation for PC using standard malloc/free

typedef struct MEMiHeapHead {
    void* start;
    void* end;
    u32 size;
} MEMiHeapHead;

// Exp Heap functions
static inline MEMiHeapHead* MEMCreateExpHeapEx(void* start, u32 size, u16 opt) {
    MEMiHeapHead* heap = (MEMiHeapHead*)malloc(sizeof(MEMiHeapHead));
    if (heap) {
        heap->start = start;
        heap->end = (u8*)start + size;
        heap->size = size;
    }
    return heap;
}

static inline MEMiHeapHead* MEMCreateExpHeap(void* start, u32 size) {
    return MEMCreateExpHeapEx(start, size, 0);
}

static inline MEMiHeapHead* MEMDestroyExpHeap(MEMiHeapHead* heap) {
    if (heap) {
        free(heap);
    }
    return NULL;
}

static inline void* MEMAllocFromExpHeapEx(MEMiHeapHead* heap, u32 size, s32 align) {
    // For PC, just use regular malloc (alignment handled by system)
    return malloc(size);
}

static inline void* MEMAllocFromExpHeap(MEMiHeapHead* heap, u32 size) {
    return MEMAllocFromExpHeapEx(heap, size, 4);
}

static inline void MEMFreeToExpHeap(MEMiHeapHead* heap, void* memBlock) {
    free(memBlock);
}

static inline u32 MEMGetAllocatableSizeForExpHeapEx(MEMiHeapHead* heap, s32 align) {
    return heap ? heap->size : 0;
}

static inline u32 MEMGetAllocatableSizeForExpHeap(MEMiHeapHead* heap) {
    return MEMGetAllocatableSizeForExpHeapEx(heap, 4);
}

// Frame Heap stubs
static inline void* MEMAllocFromFrmHeapEx(void* heap, u32 size, s32 align) {
    return malloc(size);
}

static inline void MEMFreeToFrmHeap(void* heap, void* memBlock) {
    free(memBlock);
}

#endif // PLATFORM_PC
#endif // PLATFORM_PC_MEM_H
