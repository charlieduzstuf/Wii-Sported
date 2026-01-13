/**
 * Compatability macros for deprecated/future C++ features
 */

#ifndef LANG_H
#define LANG_H

#ifndef PLATFORM_PC
// Only define noexcept macro for legacy Wii builds
// Modern C++ compilers have noexcept as a keyword
#if __cplusplus < 201103L
#define noexcept throw()
#define override
#endif
#endif

#endif
