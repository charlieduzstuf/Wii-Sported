#ifndef RVL_SDK_OS_MUTEX_H
#define RVL_SDK_OS_MUTEX_H
#include <types.h>

#ifdef PLATFORM_PC
// PC mutex using SDL2
#include <SDL2/SDL_mutex.h>

typedef SDL_mutex* OSMutex;
typedef struct OSMutexQueue {
    void* dummy;
} OSMutexQueue;

// Mutex functions mapped to SDL2
static inline void OSInitMutex(OSMutex* mutex) {
    *mutex = SDL_CreateMutex();
}

static inline void OSLockMutex(OSMutex* mutex) {
    SDL_LockMutex(*mutex);
}

static inline void OSUnlockMutex(OSMutex* mutex) {
    SDL_UnlockMutex(*mutex);
}

static inline BOOL OSTryLockMutex(OSMutex* mutex) {
    return SDL_TryLockMutex(*mutex) == 0;
}

#define OSInitMutexQueue(queue) ((void)0)
#define __OSUnlockAllMutex(thread) ((void)0)

#else
// Original Wii implementation
#include <revolution/OS/OSThread.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSMutex {
    OSThreadQueue queue;  // at 0x0
    OSThread* thread;     // at 0x8
    s32 lock;             // at 0xC
    struct OSMutex* next; // at 0x10
    struct OSMutex* prev; // at 0x14
} OSMutex;

void OSInitMutex(OSMutex* mutex);
void OSLockMutex(OSMutex* mutex);
void OSUnlockMutex(OSMutex* mutex);
void __OSUnlockAllMutex(OSThread* thread);
BOOL OSTryLockMutex(OSMutex* mutex);

#ifdef __cplusplus
}
#endif
#endif // Wii implementation

#endif // RVL_SDK_OS_MUTEX_H
