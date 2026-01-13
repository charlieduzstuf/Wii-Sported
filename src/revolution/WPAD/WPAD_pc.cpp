#include <revolution/WPAD.h>

#ifdef PLATFORM_PC

#include <SDL2/SDL.h>
#include <string.h>

static int mouseX = 0, mouseY = 0;

void WPADInit(void) {
    SDL_ShowCursor(SDL_ENABLE);
}

s32 WPADProbe(s32 chan, u32* devType) {
    if (chan == WPAD_CHAN0) {
        if (devType) *devType = 0; // Core controller
        return 0; // Success
    }
    return -1; // No controller
}

void WPADRead(s32 chan, WPADStatus* status) {
    if (!status || chan != WPAD_CHAN0) return;
    
    memset(status, 0, sizeof(WPADStatus));
    
    SDL_PumpEvents();
    u32 mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);
    const u8* keys = SDL_GetKeyboardState(NULL);
    
    u32 buttons = 0;
    
    // Mouse buttons
    if (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT))  buttons |= WPAD_BUTTON_A;
    if (mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT)) buttons |= WPAD_BUTTON_B;
    
    // Keyboard
    if (keys[SDL_SCANCODE_1])      buttons |= WPAD_BUTTON_1;
    if (keys[SDL_SCANCODE_2])      buttons |= WPAD_BUTTON_2;
    if (keys[SDL_SCANCODE_MINUS])  buttons |= WPAD_BUTTON_MINUS;
    if (keys[SDL_SCANCODE_EQUALS]) buttons |= WPAD_BUTTON_PLUS;
    if (keys[SDL_SCANCODE_UP])     buttons |= WPAD_BUTTON_UP;
    if (keys[SDL_SCANCODE_DOWN])   buttons |= WPAD_BUTTON_DOWN;
    if (keys[SDL_SCANCODE_LEFT])   buttons |= WPAD_BUTTON_LEFT;
    if (keys[SDL_SCANCODE_RIGHT])  buttons |= WPAD_BUTTON_RIGHT;
    if (keys[SDL_SCANCODE_ESCAPE]) buttons |= WPAD_BUTTON_HOME;
    
    status->button = buttons;
    
    // Mock accelerometer (neutral - controller pointing forward)
    status->acc[0] = 0.0f;
    status->acc[1] = 1.0f;
    status->acc[2] = 0.0f;
    status->roll = 0.0f;
    status->pitch = 0.0f;
    
    // IR pointer (dpd) from mouse position
    SDL_Window* window = SDL_GL_GetCurrentWindow();
    if (window) {
        int winWidth, winHeight;
        SDL_GetWindowSize(window, &winWidth, &winHeight);
        
        // Normalize to Wii coordinates (0-1023, 0-767)
        status->dpd[0][0] = (f32)(mouseX * 1023 / winWidth);
        status->dpd[0][1] = (f32)(mouseY * 767 / winHeight);
    }
}

#endif // PLATFORM_PC
