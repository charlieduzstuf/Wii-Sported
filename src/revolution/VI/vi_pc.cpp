#ifdef PLATFORM_PC

// Include C++ headers BEFORE revolution headers to avoid extern "C" conflicts
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#endif

#include <revolution/VI.h>

#ifdef PLATFORM_PC

static u32 retraceCount = 0;
static BOOL isBlack = FALSE;

void VIInit(void) {
    // VI initialization happens with SDL window creation in main.cpp
}

void VIWaitForRetrace(void) {
    // Wait for vsync (already handled by SDL_GL_SetSwapInterval(1))
    SDL_Delay(16); // ~60 FPS
    retraceCount++;
}

void VIFlush(void) {
    // Swap buffers
    SDL_Window* window = SDL_GL_GetCurrentWindow();
    if (window) {
        SDL_GL_SwapWindow(window);
    }
}

void VISetBlack(BOOL black) {
    isBlack = black;
    if (black) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        VIFlush();
    }
}

s32 VIGetRetraceCount(void) {
    return (s32)retraceCount;
}

#endif // PLATFORM_PC
