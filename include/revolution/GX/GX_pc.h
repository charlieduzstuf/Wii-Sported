// GX PC Implementation Header
// Portions adapted from Dolphin Emulator
// Copyright 2008-2024 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef GX_PC_H
#define GX_PC_H

#ifdef PLATFORM_PC

#include <types.h>
#include <revolution/GX/GXTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declare state
void GX_InitState(void);
void GX_ApplyState(void);

// GX PC Implementation - Basic stubs for now
static inline void GXInit(void* base, u32 size) {
    GX_InitState();
}

static inline void GXSetViewport(f32 x, f32 y, f32 w, f32 h, f32 nearZ, f32 farZ) {
    // Will implement in gx_renderer.cpp
}

static inline void GXSetScissor(u32 x, u32 y, u32 w, u32 h) {
    // Will implement in gx_renderer.cpp
}

static inline void GXSetBlendMode(GXBlendMode mode, GXBlendFactor src, GXBlendFactor dst, GXLogicOp op) {
    // Will implement properly
}

static inline void GXSetZMode(bool enable, GXCompare func, bool update) {
    // Will implement properly
}

static inline void GXSetCullMode(GXCullMode mode) {
    // Will implement properly
}

// Stubbed for now - will implement
#define GXBegin(prim, fmt, nverts) ((void)0)
#define GXEnd() ((void)0)
#define GXPosition3f32(x, y, z) ((void)0)
#define GXNormal3f32(x, y, z) ((void)0)
#define GXColor4u8(r, g, b, a) ((void)0)
#define GXTexCoord2f32(s, t) ((void)0)
#define GXLoadPosMtxImm(mtx, id) ((void)0)
#define GXSetNumChans(n) ((void)0)
#define GXSetNumTexGens(n) ((void)0)
#define GXSetTevOp(stage, mode) ((void)0)
#define GXSetVtxDesc(attr, type) ((void)0)
#define GXClearVtxDesc() ((void)0)
#define GXInvalidateVtxCache() ((void)0)
#define GXSetLineWidth(width, fmt) ((void)0)
#define GXSetPointSize(size, fmt) ((void)0)
#define GXEnableTexOffsets(coord, line, point) ((void)0)
#define GXSetCopyClear(color, z) ((void)0)
#define GXSetDispCopyGamma(gamma) ((void)0)
#define GXCopyDisp(dest, clear) ((void)0)
#define GXPixModeSync() ((void)0)
#define GXPokeAlphaMode(func, threshold) ((void)0)
#define GXPokeAlphaRead(mode) ((void)0)
#define GXPokeAlphaUpdate(enable) ((void)0)
#define GXPokeBlendMode(mode, src, dst, op) ((void)0)
#define GXPokeColorUpdate(enable) ((void)0)
#define GXPokeDither(dither) ((void)0)
#define GXPokeDstAlpha(enable, alpha) ((void)0)
#define GXPokeZMode(enable, func, update) ((void)0)
#define GXFlush() ((void)0)
#define GXDrawDone() ((void)0)

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_PC
#endif // GX_PC_H
