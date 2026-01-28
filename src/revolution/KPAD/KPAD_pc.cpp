#ifdef PLATFORM_PC

// Include C++ headers BEFORE revolution headers to avoid extern "C" conflicts
#include <SDL2/SDL.h>
#include <string.h>

#endif

#include <revolution/KPAD.h>

#ifdef PLATFORM_PC

void KPADInit(void) {
    // Initialize with WPAD
}

s32 KPADRead(s32 chan, KPADStatus* pSamples, s32 numSamples) {
    if (!pSamples || chan != 0 || numSamples < 1) return 0;
    
    memset(pSamples, 0, sizeof(KPADStatus));
    
    SDL_PumpEvents();
    
    int mouseX, mouseY;
    u32 mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);
    const u8* keys = SDL_GetKeyboardState(NULL);
    
    u32 buttons = 0;
    
    // Mouse
    if (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT))  buttons |= (1 << 11); // A
    if (mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT)) buttons |= (1 << 10); // B
    
    // Keyboard
    if (keys[SDL_SCANCODE_1])      buttons |= (1 << 9);  // 1
    if (keys[SDL_SCANCODE_2])      buttons |= (1 << 8);  // 2
    if (keys[SDL_SCANCODE_MINUS])  buttons |= (1 << 12); // Minus
    if (keys[SDL_SCANCODE_EQUALS]) buttons |= (1 << 4);  // Plus
    if (keys[SDL_SCANCODE_UP])     buttons |= (1 << 3);
    if (keys[SDL_SCANCODE_DOWN])   buttons |= (1 << 2);
    if (keys[SDL_SCANCODE_LEFT])   buttons |= (1 << 0);
    if (keys[SDL_SCANCODE_RIGHT])  buttons |= (1 << 1);
    if (keys[SDL_SCANCODE_ESCAPE]) buttons |= (1 << 15); // Home
    
    pSamples->hold = buttons;
    
    // Button triggers
    static u32 lastButtons = 0;
    pSamples->trig = buttons & ~lastButtons;
    pSamples->release = lastButtons & ~buttons;
    lastButtons = buttons;
    
    // IR pointer position
    SDL_Window* window = SDL_GL_GetCurrentWindow();
    if (window) {
        int winWidth, winHeight;
        SDL_GetWindowSize(window, &winWidth, &winHeight);
        
        pSamples->pos.x = (mouseX / (f32)winWidth) * 2.0f - 1.0f;
        pSamples->pos.y = 1.0f - (mouseY / (f32)winHeight) * 2.0f;
        pSamples->dpd_valid_fg = 1;
    }
    
    // Mock accelerometer
    pSamples->acc[0] = 0.0f;
    pSamples->acc[1] = 1.0f;
    pSamples->acc[2] = 0.0f;
    pSamples->acc_value = 1.0f;
    pSamples->acc_speed = 0.0f;
    
    pSamples->dev_type = 0;
    pSamples->wpad_err = 0;
    pSamples->data_format = 0;
    
    return 1;
}

#endif // PLATFORM_PC
