# Wii Sports PC Port - Compatibility Layer Approach

## Strategy: Minimal Directory Changes

Instead of reorganizing the entire project, we'll:

1. **Keep existing directory structure intact**
2. **Create compatibility headers** that replace Wii functions with PC equivalents
3. **Use SDL2 + OpenGL** as drop-in replacements
4. **Modify build system** (configure.py → CMake) to target Windows

## Implementation Plan

### Step 1: Add CMake build (alongside configure.py)
- Create CMakeLists.txt at root
- Target Windows x64
- Link SDL2, OpenGL, OpenAL

### Step 2: Create compatibility layer in include/
- `include/compat/` directory with PC implementations
- Headers map Wii SDK → PC libraries:
  - OS.h → Windows threads/SDL2
  - GX.h → OpenGL
  - AX.h → OpenAL
  - DVD.h → File I/O
  - etc.

### Step 3: Modify source files minimally
- Add `#ifdef PLATFORM_PC` where needed
- Include compat headers before revolution headers
- Most game logic stays unchanged

### Step 4: Replace main.cpp entry point
- Create Windows WinMain/main()
- Initialize SDL2 + OpenGL
- Call original game initialization

### Step 5: Implement library replacements
- Use SDL2 for: window, input, audio, threading, time
- Use OpenGL 3.3+ for GX graphics
- Use GLM for math (matches MTX closely)
- Use stb_image for textures

## Quick Wins (Do These First)
1. ✅ Math libraries (MTX → GLM) - almost no changes needed
2. ⚡ Threading (OS → SDL2) - simple API mapping  
3. ⚡ Time functions (OSTime → SDL_GetTicks)
4. ⚡ File I/O (DVD → fopen/SDL_RWops)
5. 🔨 Graphics (GX → OpenGL) - needs translation layer
6. 🔨 Audio (AX → OpenAL) - needs voice management
7. 🔨 Input (WPAD → SDL2/Gamepad) - needs mapping

Legend: ✅ Trivial, ⚡ Easy, 🔨 Moderate effort

## Files to Modify
- `configure.py` → `CMakeLists.txt` (new build system)
- `src/main.cpp` → add PC entry point
- Minimal changes to existing source files
