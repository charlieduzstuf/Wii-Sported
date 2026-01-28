#ifndef NW4R_MATH_ARITHMETIC_H
#define NW4R_MATH_ARITHMETIC_H
#include <nw4r/types_nw4r.h>

#include <nw4r/math/math_constant.h>

#include <revolution/OS.h>

#include <cmath>

namespace nw4r {
namespace math {

/******************************************************************************
 *
 * Implementation details
 *
 ******************************************************************************/
namespace detail {

f32 FExp(f32 x);
f32 FLog(f32 x);

} // namespace detail

/******************************************************************************
 *
 * Arithmetic functions
 *
 ******************************************************************************/
f32 FrSqrt(f32 x);

#ifndef PLATFORM_PC
// PowerPC implementation with assembly
inline f32 FAbs(register f32 x) {
    register f32 ax;

    ASM (
        fabs ax, x
    )

    return ax;
}
#else
// PC implementation
inline f32 FAbs(f32 x) {
    return fabsf(x);
}
#endif

inline f32 FCeil(f32 x) {
#ifdef PLATFORM_PC
    return ceilf(x);
#else
    return std::ceilf(x);
#endif
}

inline f32 FExp(f32 x) {
    return detail::FExp(x);
}

inline f32 FFloor(f32 x) {
#ifdef PLATFORM_PC
    return floorf(x);
#else
    return std::floorf(x);
#endif
}

#ifndef PLATFORM_PC
// PowerPC implementation
inline f32 FInv(register f32 x) {
    register f32 work0, work1, work2, work3;

    ASM (
        fmr  work1, x     // x
        fres work0, work1 // 1/x

        // Refine estimate
        ps_add   work2, work0, work0        // 2/x
        ps_mul   work3, work0, work0        // 1/x^2
        ps_nmsub work0, work1, work3, work2 // -(x * 1/x^2 - 2/x)
    )

    return work0;
}
#else
// PC implementation
inline f32 FInv(f32 x) {
    return 1.0f / x;
}
#endif

inline f32 FMod(f32 x, f32 y) {
#ifdef PLATFORM_PC
    return fmodf(x, y);
#else
    return std::fmodf(x, y);
#endif
}

inline f32 FModf(f32 x, f32* pY) {
#ifdef PLATFORM_PC
    return modff(x, pY);
#else
    return std::modff(x, pY);
#endif
}

inline f32 FSqrt(f32 x) {
    return x <= 0.0f ? 0.0f : x * FrSqrt(x);
}

inline f32 FLog(f32 x) {
    if (x > 0.0f) {
        return detail::FLog(x);
    }

    return NW4R_MATH_QNAN;
}

#ifndef PLATFORM_PC
// PowerPC implementation
inline f32 FSelect(register f32 value, register f32 ge_zero,
                   register f32 lt_zero) {
    register f32 ret;

    ASM (
        fsel ret, value, ge_zero, lt_zero
    )

    return ret;
}
#else
// PC implementation
inline f32 FSelect(f32 value, f32 ge_zero, f32 lt_zero) {
    return value >= 0.0f ? ge_zero : lt_zero;
}
#endif

/******************************************************************************
 *
 * Fastcast functions
 *
 ******************************************************************************/
inline f32 U16ToF32(u16 arg) {
    f32 ret;
    OSu16tof32(&arg, &ret);
    return ret;
}
inline u16 F32ToU16(f32 arg) {
    u16 ret;
    OSf32tou16(&arg, &ret);
    return ret;
}

inline f32 S16ToF32(s16 arg) {
    f32 ret;
    OSs16tof32(&arg, &ret);
    return ret;
}
inline s16 F32ToS16(f32 arg) {
    s16 ret;
    OSf32tos16(&arg, &ret);
    return ret;
}

inline u32 F32AsU32(f32 arg) {
    return *reinterpret_cast<u32*>(&arg);
}
inline f32 U32AsF32(u32 arg) {
    return *reinterpret_cast<f32*>(&arg);
}

inline s32 FGetExpPart(f32 x) {
    s32 s = F32AsU32(x);
    return ((s >> 23) & 0xFF) - 127;
}
inline f32 FGetMantPart(f32 x) {
    u32 u = F32AsU32(x);
    return U32AsF32((u & 0x807FFFFF) | 0x3F800000);
}

} // namespace math
} // namespace nw4r

#endif
