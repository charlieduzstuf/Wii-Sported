#ifndef NW4R_PUBLIC_MATH_H
#define NW4R_PUBLIC_MATH_H

// Always include type definitions
#include <nw4r/math/math_types.h>

#ifndef PLATFORM_PC
// Full Wii implementation - include arithmetic functions
#include <nw4r/math/math_arithmetic.h>
#include <nw4r/math/math_geometry.h>
#include <nw4r/math/math_triangular.h>
#else
// PC: Renamed from math.h to nw4r_math.h to avoid conflicts with system math.h
// This allows SDL2 to properly include system <math.h> without finding this header
// For system math functions, use <cmath> instead
#endif // !PLATFORM_PC

#endif
