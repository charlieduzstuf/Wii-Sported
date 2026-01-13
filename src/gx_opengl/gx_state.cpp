// GX to OpenGL Translation Layer - State Management
// Portions adapted from Dolphin Emulator
// Copyright 2008-2024 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#ifdef PLATFORM_PC

#include "gx_state.h"
#include <SDL2/SDL_opengl.h>
#include <string.h>

// Global GX state
GXRenderState g_gxState;

void GX_InitState(void) {
    memset(&g_gxState, 0, sizeof(GXRenderState));
    
    // Initialize with sensible defaults
    g_gxState.blend.mode = GX_BM_NONE;
    g_gxState.blend.srcFactor = GX_BL_ONE;
    g_gxState.blend.dstFactor = GX_BL_ZERO;
    
    g_gxState.zmode.enable = TRUE;
    g_gxState.zmode.func = GX_LESS;
    g_gxState.zmode.update = TRUE;
    
    g_gxState.cull.mode = GX_CULL_BACK;
    
    g_gxState.vpNear = 0.0f;
    g_gxState.vpFar = 1.0f;
    
    g_gxState.inBeginEnd = FALSE;
}

static GLenum GXBlendFactorToGL(GXBlendFactor factor) {
    switch (factor) {
        case GX_BL_ZERO: return GL_ZERO;
        case GX_BL_ONE: return GL_ONE;
        case GX_BL_SRCCLR: return GL_SRC_COLOR;
        case GX_BL_INVSRCCLR: return GL_ONE_MINUS_SRC_COLOR;
        case GX_BL_SRCALPHA: return GL_SRC_ALPHA;
        case GX_BL_INVSRCALPHA: return GL_ONE_MINUS_SRC_ALPHA;
        case GX_BL_DSTALPHA: return GL_DST_ALPHA;
        case GX_BL_INVDSTALPHA: return GL_ONE_MINUS_DST_ALPHA;
        case GX_BL_DSTCLR: return GL_DST_COLOR;
        case GX_BL_INVDSTCLR: return GL_ONE_MINUS_DST_COLOR;
        default: return GL_ONE;
    }
}

static GLenum GXCompareFuncToGL(GXCompare func) {
    switch (func) {
        case GX_NEVER: return GL_NEVER;
        case GX_LESS: return GL_LESS;
        case GX_EQUAL: return GL_EQUAL;
        case GX_LEQUAL: return GL_LEQUAL;
        case GX_GREATER: return GL_GREATER;
        case GX_NEQUAL: return GL_NOTEQUAL;
        case GX_GEQUAL: return GL_GEQUAL;
        case GX_ALWAYS: return GL_ALWAYS;
        default: return GL_LESS;
    }
}

void GX_SetBlendState(const GXBlendState* state) {
    if (state->mode == GX_BM_NONE) {
        glDisable(GL_BLEND);
    } else {
        glEnable(GL_BLEND);
        GLenum src = GXBlendFactorToGL(state->srcFactor);
        GLenum dst = GXBlendFactorToGL(state->dstFactor);
        glBlendFunc(src, dst);
    }
}

void GX_SetZState(const GXZState* state) {
    if (state->enable) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GXCompareFuncToGL(state->func));
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    glDepthMask(state->update ? GL_TRUE : GL_FALSE);
}

void GX_SetCullState(const GXCullState* state) {
    switch (state->mode) {
        case GX_CULL_NONE:
            glDisable(GL_CULL_FACE);
            break;
        case GX_CULL_FRONT:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;
        case GX_CULL_BACK:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
        case GX_CULL_ALL:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT_AND_BACK);
            break;
    }
}

void GX_ApplyState(void) {
    GX_SetBlendState(&g_gxState.blend);
    GX_SetZState(&g_gxState.zmode);
    GX_SetCullState(&g_gxState.cull);
}

#endif // PLATFORM_PC
