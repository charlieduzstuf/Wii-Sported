/**
 * OS PC Implementation Header
 * Provides PC-compatible stubs for Wii OS functions
 * Similar to Dolphin Emulator's approach
 */

#ifndef RVL_SDK_OS_PC_H
#define RVL_SDK_OS_PC_H

#ifdef PLATFORM_PC

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

// PowerPC-specific fast cast functions - stubbed for PC
static inline void OSInitFastCast(void) {
    // No-op on PC - not needed for x86
}

static inline void OSSetGQR6(u32 type, u32 scale) {
    // No-op on PC
}

static inline void OSSetGQR7(u32 type, u32 scale) {
    // No-op on PC
}

// Fast cast conversion functions - use standard C on PC
static inline void OSu8tof32(u8* in, volatile f32* out) {
    *out = (f32)(*in);
}

static inline void OSf32tou8(f32* in, volatile u8* out) {
    *out = (u8)(*in);
}

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_PC
#endif // RVL_SDK_OS_PC_H
