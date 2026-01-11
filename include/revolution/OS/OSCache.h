#ifndef RVL_SDK_OS_CACHE_H
#define RVL_SDK_OS_CACHE_H
#include <types.h>

#ifdef PLATFORM_PC
// Cache functions are no-ops on PC (x86/x64 handles cache coherency automatically)
#define DCEnable() ((void)0)
#define DCInvalidateRange(buf, len) ((void)0)
#define DCFlushRange(buf, len) ((void)0)
#define DCStoreRange(buf, len) ((void)0)
#define DCFlushRangeNoSync(buf, len) ((void)0)
#define DCStoreRangeNoSync(buf, len) ((void)0)
#define DCZeroRange(buf, len) memset((void*)(buf), 0, (len))

#define ICInvalidateRange(buf, len) ((void)0)
#define ICFlashInvalidate() ((void)0)
#define ICEnable() ((void)0)

#define LCEnable() ((void)0)
#define LCDisable() ((void)0)
#define LCLoadBlocks(dst, src, blocks) memcpy((dst), (src), (blocks) * 32)
#define LCStoreBlocks(dst, src, blocks) memcpy((dst), (src), (blocks) * 32)
#define LCStoreData(dst, src, len) (memcpy((dst), (src), (len)), (len))
#define LCQueueLength() (0)
#define LCQueueWait(n) ((void)0)

#define L2Enable() ((void)0)
#define L2Disable() ((void)0)
#define L2GlobalInvalidate() ((void)0)

#define __OSCacheInit() ((void)0)

#else
// Original Wii implementation
#ifdef __cplusplus
extern "C" {
#endif

#define OS_CACHE_BASE 0xE0000000

// Forward declarations
typedef struct OSContext OSContext;

void DCEnable(void);
void DCInvalidateRange(const void* buf, u32 len);
void DCFlushRange(const void* buf, u32 len);
void DCStoreRange(const void* buf, u32 len);
void DCFlushRangeNoSync(const void* buf, u32 len);
void DCStoreRangeNoSync(const void* buf, u32 len);
void DCZeroRange(const void* buf, u32 len);

void ICInvalidateRange(const void* buf, u32 len);
void ICFlashInvalidate(void);
void ICEnable(void);

void LCEnable(void);
void LCDisable(void);
void LCLoadBlocks(void* dst, const void* src, u32 blocks);
void LCStoreBlocks(void* dst, const void* src, u32 blocks);
u32 LCStoreData(void* dst, const void* src, u32 len);
u32 LCQueueLength(void);
void LCQueueWait(u32 n);

void L2Enable(void);
void L2Disable(void);
void L2GlobalInvalidate(void);

void DMAErrorHandler(u8 error, OSContext* ctx, u32 dsisr, u32 dar, ...);

void __OSCacheInit(void);

#ifdef __cplusplus
}
#endif
#endif // Wii implementation

#endif // RVL_SDK_OS_CACHE_H
