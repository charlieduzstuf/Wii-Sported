#ifdef PLATFORM_PC
// PC stubs for Wii hardware registers
// On Wii these are mapped to actual hardware addresses
// On PC they're just dummy arrays

#include <types.h>

// Define storage for hardware register arrays
volatile u32 PI_HW_REGS[256] = {0};
volatile u32 MI_HW_REGS[256] = {0};
volatile u32 DI_HW_REGS[256] = {0};
volatile u16 VI_HW_REGS[256] = {0};

#endif // PLATFORM_PC
