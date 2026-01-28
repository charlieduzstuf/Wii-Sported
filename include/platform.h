/**
 * Platform compatibility header
 * Handles platform-specific include order and macro conflicts
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef PLATFORM_PC

// Define this early to prevent SDL2 from pulling in system math.h 
// which would conflict with nw4r headers
#define SDL_DISABLE_MATH_INCLUDES

// Include standard library headers first
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cmath>

// Include SDL2 BEFORE any game headers to avoid conflicts
#include <SDL2/SDL.h>

// Fix Windows macro conflicts
#ifdef _WIN32
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

// Bring standard library functions into global namespace for compatibility
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
using std::va_start;
using std::va_end;
using std::FILE;
using std::size_t;

#endif // PLATFORM_PC

#endif // PLATFORM_H
