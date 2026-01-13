#ifndef RVL_SDK_PUBLIC_MEM_H
#define RVL_SDK_PUBLIC_MEM_H

#ifdef PLATFORM_PC
// PC implementation - simplified memory manager
#include <revolution/MEM/mem_pc.h>
#else
// Wii implementation
#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/MEM/mem_allocator.h>
#include <revolution/MEM/mem_expHeap.h>
#include <revolution/MEM/mem_frameHeap.h>
#include <revolution/MEM/mem_heapCommon.h>
#include <revolution/MEM/mem_list.h>

#ifdef __cplusplus
}
#endif
#endif // PLATFORM_PC

#endif // RVL_SDK_PUBLIC_MEM_H
