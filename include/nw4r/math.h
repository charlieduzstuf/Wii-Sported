#ifndef NW4R_PUBLIC_MATH_H
#define NW4R_PUBLIC_MATH_H

#ifndef PLATFORM_PC
// Full Wii implementation
#include <nw4r/math/math_types.h>
#include <nw4r/math/math_arithmetic.h>
#include <nw4r/math/math_geometry.h>
#include <nw4r/math/math_triangular.h>
#else
// PC: This file should NOT be included via <math.h>
// Only include if explicitly requested as <nw4r/math.h>
// For system math, use <cmath> instead
#warning "nw4r/math.h included on PC - this may cause conflicts with system math.h"
#endif // !PLATFORM_PC

#endif
