/**
 * Platform compatibility header
 * Handles platform-specific include order and macro conflicts
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef PLATFORM_PC

// Include standard library headers first to get FILE, SEEK_SET, etc.
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Include SDL2 (which may include Windows headers on Windows)
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
