#ifdef PLATFORM_PC

#include "gx_vertex.h"
#include <SDL2/SDL_opengl.h>
#include <cstdlib>
#include <cstring>

using std::malloc;
using std::free;
using std::memset;

static GX_VertexFormat currentFormat = {0};
static GXPrimitive currentPrimitive;
static uint16_t currentNumVerts;
static int vertexSystemInitialized = 0;

// Temporary vertex data storage
static float* vertexData = NULL;
static int vertexDataSize = 0;
static int vertexDataCapacity = 0;

void GX_InitVertexSystem(void) {
    if (vertexSystemInitialized) return;
    
    memset(&currentFormat, 0, sizeof(currentFormat));
    vertexDataCapacity = 10000; // Initial capacity
    vertexData = (float*)malloc(vertexDataCapacity * sizeof(float));
    vertexDataSize = 0;
    
    vertexSystemInitialized = 1;
}

void GX_ShutdownVertexSystem(void) {
    if (vertexData) {
        free(vertexData);
        vertexData = NULL;
    }
    vertexDataSize = 0;
    vertexDataCapacity = 0;
    vertexSystemInitialized = 0;
}

void GX_ConfigureVertexFormat(uint8_t vtxfmt) {
    // Apply current vertex format to OpenGL
    for (int i = 0; i < currentFormat.numAttrs; i++) {
        GX_VertexAttrDesc* desc = &currentFormat.attrs[i];
        
        switch (desc->attr) {
            case GX_VA_POS:
                glEnableClientState(GL_VERTEX_ARRAY);
                break;
            case GX_VA_NRM:
                glEnableClientState(GL_NORMAL_ARRAY);
                break;
            case GX_VA_CLR0:
                glEnableClientState(GL_COLOR_ARRAY);
                break;
            case GX_VA_TEX0:
            case GX_VA_TEX1:
            case GX_VA_TEX2:
            case GX_VA_TEX3:
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                break;
        }
    }
}

void GX_BeginVertexData(void) {
    vertexDataSize = 0;
}

void GX_EndVertexData(void) {
    // Vertex data will be submitted in GXEnd()
}

// GX API implementations

void GXClearVtxDesc(void) {
    memset(&currentFormat, 0, sizeof(currentFormat));
    currentFormat.numAttrs = 0;
}

void GXSetVtxDesc(GXAttr attr, GXAttrType type) {
    if (currentFormat.numAttrs < 16) {
        currentFormat.attrs[currentFormat.numAttrs].attr = attr;
        currentFormat.attrs[currentFormat.numAttrs].type = type;
        currentFormat.numAttrs++;
    }
}

void GXSetVtxAttrFmt(uint8_t vtxfmt, GXAttr attr, GXCompType type, uint8_t frac, uint8_t shift) {
    // Find attribute in current format and set component type
    for (int i = 0; i < currentFormat.numAttrs; i++) {
        if (currentFormat.attrs[i].attr == attr) {
            currentFormat.attrs[i].compType = type;
            currentFormat.attrs[i].shift = shift;
            break;
        }
    }
}

void GXBegin(GXPrimitive type, uint8_t vtxfmt, uint16_t nverts) {
    currentPrimitive = type;
    currentNumVerts = nverts;
    GX_BeginVertexData();
    
    // Map GX primitive to OpenGL
    GLenum glPrimitive;
    switch (type) {
        case GX_POINTS:
            glPrimitive = GL_POINTS;
            break;
        case GX_LINES:
            glPrimitive = GL_LINES;
            break;
        case GX_LINESTRIP:
            glPrimitive = GL_LINE_STRIP;
            break;
        case GX_TRIANGLES:
            glPrimitive = GL_TRIANGLES;
            break;
        case GX_TRIANGLESTRIP:
            glPrimitive = GL_TRIANGLE_STRIP;
            break;
        case GX_TRIANGLEFAN:
            glPrimitive = GL_TRIANGLE_FAN;
            break;
        case GX_QUADS:
            glPrimitive = GL_QUADS;
            break;
        default:
            glPrimitive = GL_TRIANGLES;
            break;
    }
    
    glBegin(glPrimitive);
}

void GXEnd(void) {
    glEnd();
    GX_EndVertexData();
}

void GXPosition3f32(float x, float y, float z) {
    glVertex3f(x, y, z);
}

void GXPosition2f32(float x, float y) {
    glVertex2f(x, y);
}

void GXNormal3f32(float x, float y, float z) {
    glNormal3f(x, y, z);
}

void GXColor4u8(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    glColor4ub(r, g, b, a);
}

void GXTexCoord2f32(float s, float t) {
    glTexCoord2f(s, t);
}

#endif // PLATFORM_PC
