#ifndef NW4R_PUBLIC_MATH_H
#define NW4R_PUBLIC_MATH_H

// On PC builds, we still need the type definitions even if functions are stubbed
#include <nw4r/math/math_types.h>

#ifndef PLATFORM_PC
// Full implementations only on Wii
#include <nw4r/math/math_arithmetic.h>
#include <nw4r/math/math_geometry.h>
#include <nw4r/math/math_triangular.h>
#endif // !PLATFORM_PC

#endif
