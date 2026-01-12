#ifdef PLATFORM_PC

#include <revolution/GX/GX_pc.h>
#include <SDL2/SDL_opengl.h>
#include <string.h>

// Matrix stack for position/normal matrices
static float positionMatrices[10][3][4];
static float normalMatrices[10][3][4];
static uint32_t currentMatrixIndex = 0;

// Projection matrix
static float projectionMatrix[4][4];

void GXLoadPosMtxImm(const float mtx[3][4], uint32_t pnidx) {
    if (pnidx >= 10) return;
    
    // Store matrix
    memcpy(positionMatrices[pnidx], mtx, sizeof(float) * 12);
    
    // Convert 3x4 to 4x4 for OpenGL
    float glMatrix[16];
    glMatrix[0] = mtx[0][0];   glMatrix[4] = mtx[0][1];   glMatrix[8] = mtx[0][2];    glMatrix[12] = mtx[0][3];
    glMatrix[1] = mtx[1][0];   glMatrix[5] = mtx[1][1];   glMatrix[9] = mtx[1][2];    glMatrix[13] = mtx[1][3];
    glMatrix[2] = mtx[2][0];   glMatrix[6] = mtx[2][1];   glMatrix[10] = mtx[2][2];   glMatrix[14] = mtx[2][3];
    glMatrix[3] = 0.0f;        glMatrix[7] = 0.0f;        glMatrix[11] = 0.0f;        glMatrix[15] = 1.0f;
    
    // Load into OpenGL modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glMatrix);
}

void GXLoadNrmMtxImm(const float mtx[3][4], uint32_t pnidx) {
    if (pnidx >= 10) return;
    
    // Store matrix
    memcpy(normalMatrices[pnidx], mtx, sizeof(float) * 12);
    
    // Normal matrix is typically the inverse transpose of the modelview matrix
    // For now, just use it as-is
}

void GXSetCurrentMtx(uint32_t mtx) {
    currentMatrixIndex = mtx;
    if (mtx < 10) {
        GXLoadPosMtxImm(positionMatrices[mtx], mtx);
    }
}

void GXSetProjection(const float mtx[4][4], uint8_t type) {
    // Store projection matrix
    memcpy(projectionMatrix, mtx, sizeof(float) * 16);
    
    // Convert to OpenGL column-major format
    float glMatrix[16];
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            glMatrix[col * 4 + row] = mtx[row][col];
        }
    }
    
    // Load into OpenGL projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glMatrix);
    glMatrixMode(GL_MODELVIEW);
}

void GXSetViewport(float xOrig, float yOrig, float width, float height, float nearZ, float farZ) {
    // Set OpenGL viewport
    glViewport((GLint)xOrig, (GLint)yOrig, (GLsizei)width, (GLsizei)height);
    
    // Set depth range
    glDepthRange(nearZ, farZ);
}

void GXSetScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, width, height);
}

void GXInit(GXFifoObj* fifo, uint32_t size) {
    // Initialize GX subsystems
    extern void GX_InitState(void);
    extern void GX_InitTextureSystem(void);
    extern void GX_InitVertexSystem(void);
    
    GX_InitState();
    GX_InitTextureSystem();
    GX_InitVertexSystem();
    
    // Set up default OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    // Initialize matrices to identity
    for (int i = 0; i < 10; i++) {
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 4; col++) {
                positionMatrices[i][row][col] = (row == col) ? 1.0f : 0.0f;
                normalMatrices[i][row][col] = (row == col) ? 1.0f : 0.0f;
            }
        }
    }
}

void GXSetCopyClear(GXColor clearColor, uint32_t clearZ) {
    glClearColor(clearColor[0] / 255.0f, 
                 clearColor[1] / 255.0f, 
                 clearColor[2] / 255.0f, 
                 clearColor[3] / 255.0f);
    glClearDepth(clearZ / 16777215.0); // 24-bit depth
}

void GXDrawDone(void) {
    glFlush();
}

void GXFlush(void) {
    glFlush();
}

void GXPixModeSync(void) {
    // Pixel mode synchronization - no-op on PC
}

#endif // PLATFORM_PC
