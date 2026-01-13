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
#include <cstdarg>

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
    
    // Add using declarations for std namespace on Windows
    using std::fprintf;
    using std::printf;
    using std::vprintf;
    using std::fopen;
    using std::fclose;
    using std::fseek;
    using std::ftell;
    using std::fread;
    using std::fwrite;
    using std::malloc;
    using std::free;
    using std::calloc;
    using std::memset;
    using std::memcpy;
    using std::strcmp;
    using std::strlen;
    using std::va_list;
#endif

#endif // PLATFORM_PC

#endif // PLATFORM_H
