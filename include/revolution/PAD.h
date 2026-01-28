#ifndef RVL_SDK_PUBLIC_PAD_H
#define RVL_SDK_PUBLIC_PAD_H

// On PC, Pad.h includes SDL2 (C++ headers), so don't wrap in extern "C"
#ifndef PLATFORM_PC
#ifdef __cplusplus
extern "C" {
#endif
#endif

#include <revolution/PAD/Pad.h>

#ifndef PLATFORM_PC
#ifdef __cplusplus
}
#endif
#endif

#endif
