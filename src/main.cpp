// Include platform-specific headers first
#ifdef PLATFORM_PC
    #include <platform.h>
    #include <SDL2/SDL_opengl.h>
#endif

// Include game headers
#include <Pack/RPSystem.h>

#ifdef PLATFORM_PC
    
    // PC-compatible OSReport
    void OSReport(const char* msg, ...) {
        va_list args;
        va_start(args, msg);
        vprintf(msg, args);
        va_end(args);
    }
    
    // PC entry point
    int main(int argc, char** argv) {
        // Initialize SDL2
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
            fprintf(stderr, "SDL2 init failed: %s\n", SDL_GetError());
            return 1;
        }
        
        // Create window with OpenGL context
        SDL_Window* window = SDL_CreateWindow(
            "Wii Sports PC",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1280, 720,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );
        
        if (!window) {
            fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        
        // Create OpenGL context
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        
        SDL_GLContext glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            fprintf(stderr, "OpenGL context creation failed: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        
        SDL_GL_SetSwapInterval(1);  // VSync
        
        printf("Wii Sports PC Port starting...\n");
        printf("SDL2 initialized successfully\n");
        printf("OpenGL version: %s\n", glGetString(GL_VERSION));
        
        // Initialize game systems
        RPSysSystem::initialize();
        RPSysSystem::create();
        RP_GET_INSTANCE(RPSysSystem)->setup();
        
        // Main game loop
        RP_GET_INSTANCE(RPSysSystem)->mainLoop();
        
        // Cleanup
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        
        return 0;
    }
#else
    // Original Wii version
    #include <revolution/OS.h>
    
    // Stubbed for release
    void OSReport(const char* /* msg */, ...) {
        ;
    }
    
    #if defined(__MWERKS__)
    void main(int /* argc */, char** /* argv */) {
    #else
    int main(int /* argc */, char** /* argv */) {
    #endif
        RPSysSystem::initialize();
        RPSysSystem::create();
        RP_GET_INSTANCE(RPSysSystem)->setup();
        RP_GET_INSTANCE(RPSysSystem)->mainLoop();
    }
#endif
