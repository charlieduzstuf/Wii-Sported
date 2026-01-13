#ifdef PLATFORM_PC

#include "gx_texture.h"
#include <SDL2/SDL_opengl.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXTURE_CACHE 256

static GX_TextureCacheEntry textureCache[MAX_TEXTURE_CACHE];
static int textureCacheInitialized = 0;

void GX_InitTextureSystem(void) {
    if (textureCacheInitialized) return;
    
    memset(textureCache, 0, sizeof(textureCache));
    textureCacheInitialized = 1;
}

void GX_ShutdownTextureSystem(void) {
    for (int i = 0; i < MAX_TEXTURE_CACHE; i++) {
        if (textureCache[i].inUse && textureCache[i].glTexture != 0) {
            glDeleteTextures(1, &textureCache[i].glTexture);
        }
    }
    memset(textureCache, 0, sizeof(textureCache));
    textureCacheInitialized = 0;
}

void GX_GetGLFormat(GXTexFmt gxFormat, uint32_t* glFormat, uint32_t* glType, uint32_t* glInternalFormat) {
    switch (gxFormat) {
        case GX_TF_RGBA8:
            *glFormat = GL_RGBA;
            *glType = GL_UNSIGNED_BYTE;
            *glInternalFormat = GL_RGBA;
            break;
        case GX_TF_RGB565:
            *glFormat = GL_RGB;
            *glType = GL_UNSIGNED_SHORT_5_6_5;
            *glInternalFormat = GL_RGB;
            break;
        case GX_TF_I4:
        case GX_TF_I8:
            *glFormat = GL_LUMINANCE;
            *glType = GL_UNSIGNED_BYTE;
            *glInternalFormat = GL_LUMINANCE;
            break;
        case GX_TF_IA4:
        case GX_TF_IA8:
            *glFormat = GL_LUMINANCE_ALPHA;
            *glType = GL_UNSIGNED_BYTE;
            *glInternalFormat = GL_LUMINANCE_ALPHA;
            break;
        case GX_TF_CMPR:
            // DXT1 compression - would need decompression
            *glFormat = GL_RGBA;
            *glType = GL_UNSIGNED_BYTE;
            *glInternalFormat = GL_RGBA;
            break;
        default:
            *glFormat = GL_RGBA;
            *glType = GL_UNSIGNED_BYTE;
            *glInternalFormat = GL_RGBA;
            break;
    }
}

uint32_t GX_LoadTextureToGL(const GXTexObj* obj) {
    if (!obj || !obj->data) return 0;
    
    // Check cache first
    for (int i = 0; i < MAX_TEXTURE_CACHE; i++) {
        if (textureCache[i].inUse && 
            textureCache[i].dataPtr == obj->data &&
            textureCache[i].width == obj->width &&
            textureCache[i].height == obj->height &&
            textureCache[i].format == obj->format) {
            return textureCache[i].glTexture;
        }
    }
    
    // Create new texture
    GLuint glTexture;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    
    // Get GL format
    uint32_t glFormat, glType, glInternalFormat;
    GX_GetGLFormat(obj->format, &glFormat, &glType, &glInternalFormat);
    
    // Upload texture data
    // Note: For complex formats like CMPR, we'd need to decode first
    // For now, this handles simple formats
    glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, 
                 obj->width, obj->height, 0,
                 glFormat, glType, obj->data);
    
    // Find free cache slot
    for (int i = 0; i < MAX_TEXTURE_CACHE; i++) {
        if (!textureCache[i].inUse) {
            textureCache[i].glTexture = glTexture;
            textureCache[i].width = obj->width;
            textureCache[i].height = obj->height;
            textureCache[i].format = obj->format;
            textureCache[i].dataPtr = obj->data;
            textureCache[i].inUse = 1;
            break;
        }
    }
    
    return glTexture;
}

void GX_BindTexture(uint32_t glTexture, uint8_t mapID) {
    glActiveTexture(GL_TEXTURE0 + mapID);
    glBindTexture(GL_TEXTURE_2D, glTexture);
}

// GX API implementations

void GXInitTexObj(GXTexObj* obj, void* data, uint16_t width, uint16_t height, 
                  GXTexFmt format, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXBool mipmap) {
    if (!obj) return;
    
    obj->data = data;
    obj->width = width;
    obj->height = height;
    obj->format = format;
    obj->texObj = 0; // Will be set when loaded
}

void GXInitTexObjLOD(GXTexObj* obj, GXTexFilter minFilter, GXTexFilter magFilter,
                     float minLOD, float maxLOD, float LODBias, GXBool biasClamp,
                     GXBool doEdgeLOD, uint8_t maxAniso) {
    // LOD parameters - would set OpenGL texture parameters
    // For now, basic implementation
}

void GXLoadTexObj(GXTexObj* obj, uint8_t mapID) {
    if (!obj) return;
    
    uint32_t glTexture = GX_LoadTextureToGL(obj);
    obj->texObj = glTexture;
    GX_BindTexture(glTexture, mapID);
    
    // Set default texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void GXInvalidateTexAll(void) {
    // Clear texture cache
    for (int i = 0; i < MAX_TEXTURE_CACHE; i++) {
        if (textureCache[i].inUse && textureCache[i].glTexture != 0) {
            glDeleteTextures(1, &textureCache[i].glTexture);
        }
        textureCache[i].inUse = 0;
    }
}

#endif // PLATFORM_PC
