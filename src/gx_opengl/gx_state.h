// GX to OpenGL Translation Layer
// Portions adapted from Dolphin Emulator
// Copyright 2008-2024 Dolphin Emulator Project  
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef GX_OPENGL_STATE_H
#define GX_OPENGL_STATE_H

#ifdef PLATFORM_PC

#include <types.h>
#include <revolution/MTX.h>
#include <revolution/GX/GXTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// GX State Management
typedef struct GXBlendState {
    GXBlendMode mode;
    GXBlendFactor srcFactor;
    GXBlendFactor dstFactor;
    GXLogicOp logicOp;
} GXBlendState;

typedef struct GXZState {
    bool enable;
    GXCompare func;
    bool update;
} GXZState;

typedef struct GXCullState {
    GXCullMode mode;
} GXCullState;

typedef struct GXTexState {
    u32 boundTextures[8];
    u32 numTextures;
} GXTexState;

typedef struct GXTransformState {
    Mtx modelView;
    Mtx projection;
    Mtx44 proj44;
    Mtx texMtx[8];
    u8 numChans;
    bool lighting;
} GXTransformState;

typedef struct GXRenderState {
    GXBlendState blend;
    GXZState zmode;
    GXCullState cull;
    GXTexState tex;
    GXTransformState xform;
    
    // Current primitive
    GXPrimitive currentPrim;
    bool inBeginEnd;
    
    // Viewport
    f32 vpX, vpY, vpW, vpH;
    f32 vpNear, vpFar;
} GXRenderState;

// Global GX state
extern GXRenderState g_gxState;

// State management functions
void GX_InitState(void);
void GX_SetBlendState(const GXBlendState* state);
void GX_SetZState(const GXZState* state);
void GX_SetCullState(const GXCullState* state);
void GX_ApplyState(void);

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_PC
#endif // GX_OPENGL_STATE_H
