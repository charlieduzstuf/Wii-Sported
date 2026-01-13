#ifndef GX_TEXTURE_H
#define GX_TEXTURE_H

#ifdef PLATFORM_PC

#include <revolution/GX/GX_pc.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Texture cache entry
typedef struct {
    uint32_t glTexture;
    uint16_t width;
    uint16_t height;
    GXTexFmt format;
    void* dataPtr;
    int inUse;
} GX_TextureCacheEntry;

// Initialize texture system
void GX_InitTextureSystem(void);

// Shutdown texture system
void GX_ShutdownTextureSystem(void);

// Load texture to OpenGL
uint32_t GX_LoadTextureToGL(const GXTexObj* obj);

// Bind texture for rendering
void GX_BindTexture(uint32_t glTexture, uint8_t mapID);

// Convert GX format to OpenGL format
void GX_GetGLFormat(GXTexFmt gxFormat, uint32_t* glFormat, uint32_t* glType, uint32_t* glInternalFormat);

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_PC
#endif // GX_TEXTURE_H
