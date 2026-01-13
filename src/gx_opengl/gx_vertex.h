#ifndef GX_VERTEX_H
#define GX_VERTEX_H

#ifdef PLATFORM_PC

#include <revolution/GX/GX_pc.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Vertex attribute descriptor
typedef struct {
    GXAttr attr;
    GXAttrType type;
    GXCompType compType;
    uint8_t compCnt;
    uint8_t shift;
} GX_VertexAttrDesc;

// Current vertex format state
typedef struct {
    GX_VertexAttrDesc attrs[16];
    int numAttrs;
    uint32_t vao;
    uint32_t vbo;
} GX_VertexFormat;

// Initialize vertex system
void GX_InitVertexSystem(void);

// Shutdown vertex system
void GX_ShutdownVertexSystem(void);

// Configure vertex attributes for rendering
void GX_ConfigureVertexFormat(uint8_t vtxfmt);

// Begin collecting vertex data
void GX_BeginVertexData(void);

// End and submit vertex data
void GX_EndVertexData(void);

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_PC
#endif // GX_VERTEX_H
