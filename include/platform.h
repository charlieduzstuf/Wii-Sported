/**
 * Platform compatibility header
 * Handles platform-specific include order and macro conflicts
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef PLATFORM_PC

// Include SDL2 first (which may include Windows headers on Windows)
#include <SDL2/SDL.h>

// Fix Windows macro conflicts that break C++ standard library
#ifdef _WIN32
    // Windows SDK headers define these macros that conflict with C++
    #ifdef byte
    #undef byte
    #endif
    #ifdef min
    #undef min
    #endif
    #ifdef max
    #undef max
    #endif
#endif

#endif // PLATFORM_PC

#endif // PLATFORM_H
