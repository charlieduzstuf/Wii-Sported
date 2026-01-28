#ifndef NW4R_PUBLIC_MATH_H
#define NW4R_PUBLIC_MATH_H

// On PC builds, this file is intentionally mostly empty to avoid conflicts
// with system math.h. Game code should use system <cmath> functions directly.
#ifndef PLATFORM_PC

#include <nw4r/math/math_arithmetic.h>
#include <nw4r/math/math_geometry.h>
#include <nw4r/math/math_triangular.h>
#include <nw4r/math/math_types.h>

#endif // !PLATFORM_PC

#endif
