#include <revolution/PAD.h>

#ifdef PLATFORM_PC

#include <SDL2/SDL.h>
#include <string.h>

static SDL_GameController* controller = NULL;

void PADInit(void) {
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
    if (SDL_NumJoysticks() > 0) {
        controller = SDL_GameControllerOpen(0);
    }
}

u32 PADRead(PADStatus* status) {
    if (!status) return 0;
    
    memset(status, 0, sizeof(PADStatus));
    SDL_PumpEvents();
    
    const u8* keys = SDL_GetKeyboardState(NULL);
    u16 buttons = 0;
    
    // Keyboard mapping
    if (keys[SDL_SCANCODE_UP])     buttons |= PAD_BUTTON_UP;
    if (keys[SDL_SCANCODE_DOWN])   buttons |= PAD_BUTTON_DOWN;
    if (keys[SDL_SCANCODE_LEFT])   buttons |= PAD_BUTTON_LEFT;
    if (keys[SDL_SCANCODE_RIGHT])  buttons |= PAD_BUTTON_RIGHT;
    if (keys[SDL_SCANCODE_Z])      buttons |= PAD_BUTTON_A;
    if (keys[SDL_SCANCODE_X])      buttons |= PAD_BUTTON_B;
    if (keys[SDL_SCANCODE_C])      buttons |= PAD_BUTTON_X;
    if (keys[SDL_SCANCODE_V])      buttons |= PAD_BUTTON_Y;
    if (keys[SDL_SCANCODE_RETURN]) buttons |= PAD_BUTTON_START;
    if (keys[SDL_SCANCODE_Q])      buttons |= PAD_BUTTON_L;
    if (keys[SDL_SCANCODE_E])      buttons |= PAD_BUTTON_R;
    if (keys[SDL_SCANCODE_LSHIFT]) buttons |= PAD_BUTTON_Z;
    
    // WASD for analog stick
    if (keys[SDL_SCANCODE_W]) status->stickY = 127;
    if (keys[SDL_SCANCODE_S]) status->stickY = -127;
    if (keys[SDL_SCANCODE_A]) status->stickX = -127;
    if (keys[SDL_SCANCODE_D]) status->stickX = 127;
    
    // Controller support
    if (controller) {
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
            buttons |= PAD_BUTTON_A;
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B))
            buttons |= PAD_BUTTON_B;
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X))
            buttons |= PAD_BUTTON_X;
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y))
            buttons |= PAD_BUTTON_Y;
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START))
            buttons |= PAD_BUTTON_START;
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP))
            buttons |= PAD_BUTTON_UP;
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
            buttons |= PAD_BUTTON_DOWN;
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
            buttons |= PAD_BUTTON_LEFT;
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
            buttons |= PAD_BUTTON_RIGHT;
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
            buttons |= PAD_BUTTON_L;
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
            buttons |= PAD_BUTTON_R;
            
        s16 leftX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
        s16 leftY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
        s16 rightX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
        s16 rightY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
        
        status->stickX = (s8)(leftX / 256);
        status->stickY = (s8)(-leftY / 256);
        status->substickX = (s8)(rightX / 256);
        status->substickY = (s8)(-rightY / 256);
        
        u16 triggerL = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
        u16 triggerR = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
        status->triggerLeft = (u8)(triggerL / 128);
        status->triggerRight = (u8)(triggerR / 128);
    }
    
    status->button = buttons;
    status->err = 0;
    
    return 1;
}

void PADClamp(PADStatus* status) {
    // No-op for PC
}

#endif // PLATFORM_PC
