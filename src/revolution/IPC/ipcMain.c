#include <revolution/IPC.h>
#include <revolution/OS.h>

#ifdef PLATFORM_PC
// PC stub - IPC hardware registers not used on PC
volatile u32 IPC_PPC_HW_REGS[256] = {0};
volatile u32 IPC_HW_REGS[256] = {0};
#endif

static u8 Initialized = FALSE;
static void* IPCCurrentBufferLo = NULL;
static void* IPCCurrentBufferHi = NULL;
static void* IPCBufferLo = NULL;
static void* IPCBufferHi = NULL;

void IPCInit(void) {
    if (!Initialized) {
        IPCBufferHi = __OSGetIPCBufferHi();
        IPCBufferLo = __OSGetIPCBufferLo();
        IPCCurrentBufferHi = IPCBufferHi;
        IPCCurrentBufferLo = IPCBufferLo;
        Initialized = TRUE;
    }
}

u32 IPCReadReg(s32 index) {
    return IPC_PPC_HW_REGS[index];
}

void IPCWriteReg(s32 index, u32 value) {
    IPC_PPC_HW_REGS[index] = value;
}

void* IPCGetBufferHi(void) {
    return IPCCurrentBufferHi;
}

void* IPCGetBufferLo(void) {
    return IPCCurrentBufferLo;
}

void IPCSetBufferLo(void* lo) {
    IPCCurrentBufferLo = lo;
}
