# Wii Sports PC Port - Native Port Strategy

## Approach: Direct Native Port (No Compatibility Layers)

Instead of creating wrapper/compatibility layers, we will:
1. **Directly modify source files** to use PC libraries
2. **Replace Wii SDK headers** with PC equivalents
3. **Use conditional compilation** (#ifdef PLATFORM_PC) where needed
4. **Replace libraries at build time** via CMake

---

## Category 1: Direct Library Replacement

### Replace Revolution SDK with PC Libraries

#### revolution/OS → C++11 std::thread + Windows/POSIX APIs
- `OS/OSThread.h` → Delete, use `<thread>`, `<mutex>`, `<condition_variable>`
- `OS/OSTime.h` → Delete, use `<chrono>` or SDL2 timer
- `OS/OSAlloc.h` → Delete, use `new`/`delete` or custom allocator

**Action**: Globally replace includes and function calls in source files.

#### revolution/GX → OpenGL 3.3+ (Direct Calls)
- `GX/GX.h` → Delete, create `opengl/gl_renderer.h` with direct OpenGL
- `GX/GXTexture.h` → Delete, use `glTexImage2D()` etc directly
- `GX/GXTransform.h` → Delete, use OpenGL matrix stack or GLM

**Action**: Replace all GX function calls with OpenGL equivalents in cpp files.

#### revolution/MTX → GLM (Direct Usage)
- `MTX/mtx.h` → Delete, use `<glm/glm.hpp>`
- `MTX/vec.h` → Delete, use `<glm/vec3.hpp>`
- `Mtx[3][4]` → `glm::mat4`
- `Vec` → `glm::vec3`

**Action**: Search/replace types and function calls globally.

#### revolution/AX → OpenAL-Soft (Direct Calls)
- `AX/AX.h` → Delete, use `<AL/al.h>` and `<AL/alc.h>`
- `AX/AXVPB.h` → Delete, use OpenAL buffers/sources directly

**Action**: Rewrite audio code to use OpenAL API.

#### revolution/DVD → Standard File I/O
- `DVD/dvd.h` → Delete, use `<fstream>` or `<stdio.h>`
- `DVDOpen()` → `fopen()` or `std::ifstream`

**Action**: Replace DVD functions with file I/O.

#### revolution/PAD/WPAD → SDL2 Input (Direct)
- `PAD/Pad.h` → Delete, use SDL2 input directly
- `WPAD/WPAD.h` → Delete, use SDL2 mouse/keyboard

**Action**: Replace input polling with SDL2 calls.

---

## Category 2: Rewrite Internal Libraries Natively

### NW4R Libraries - Minimal Changes Needed

#### nw4r::math → Already compatible with GLM
- Keep existing code structure
- Change `#include <revolution/MTX.h>` to `#include <glm/glm.hpp>`
- Change types: `Mtx` → `glm::mat4`, `Vec` → `glm::vec3`

#### nw4r::ut (Utilities) → C++ STL
- `ut::List` → `std::list`
- `ut::Color` → Keep custom struct, just remove Wii-specific code
- `ut::Font` → Rewrite to use FreeType or stb_truetype

#### nw4r::g3d → Port to OpenGL directly
- Keep scene graph structure
- Replace GX calls with OpenGL
- Use modern shader pipeline

#### nw4r::snd → Port to OpenAL
- Keep sound architecture
- Replace AX calls with OpenAL
- Use OpenAL for 3D positioning

### EGG Framework - Native Rewrite

#### egg::core → Standard C++
- `eggHeap` → Custom allocator using `malloc`/`VirtualAlloc`
- `eggThread` → `std::thread`
- `eggSystem` → SDL2 initialization wrapper

#### egg::gfx → OpenGL Native
- `eggScreen` → SDL2 window
- `eggDrawGX` → OpenGL rendering
- Keep scene management, replace rendering backend

---

## Category 3: Port Complex Systems from Dolphin

### Use Dolphin's Code as Reference/Base

#### GX Graphics System
**Instead of wrapping GX**, port Dolphin's VideoCommon to standalone library:

1. Extract from Dolphin:
   - `Source/Core/VideoCommon/BPMemory.cpp` 
   - `Source/Core/VideoCommon/XFMemory.cpp`
   - `Source/Core/VideoBackends/OGL/OGLRender.cpp`

2. Create native OpenGL renderer:
   - `src/graphics/gx_to_gl.cpp` - Translate GX state to OpenGL
   - `src/graphics/texture_decoder.cpp` - Handle GX texture formats
   - `src/graphics/shader_gen.cpp` - Generate shaders from TEV

3. Replace in source files:
   ```cpp
   // Before:
   #include <revolution/GX.h>
   GXBegin(GX_TRIANGLES, ...);
   
   // After:
   #include "graphics/gx_to_gl.h"
   gl::Begin(GL_TRIANGLES, ...);
   ```

#### AX Audio System
**Instead of wrapping AX**, port Dolphin's DSPHLE to standalone:

1. Extract from Dolphin:
   - `Source/Core/Core/HW/DSPHLE/UCodes/AX.cpp`
   - `Source/Core/AudioCommon/Mixer.cpp`

2. Create native OpenAL audio:
   - `src/audio/ax_to_openal.cpp` - Translate AX to OpenAL
   - `src/audio/voice_manager.cpp` - Manage OpenAL sources

3. Replace in source files:
   ```cpp
   // Before:
   #include <revolution/AX.h>
   AXSetVoice(...);
   
   // After:
   #include "audio/ax_to_openal.h"
   al::SetVoice(...);
   ```

---

## Implementation Plan

### Week 1-2: Setup + Category 1 (Direct Replacements)

1. **Update CMakeLists.txt** to use SDL2, OpenGL, GLM, OpenAL
2. **Delete revolution SDK headers** (keep as reference backup)
3. **Create new headers**:
   - `src/platform/types.h` - Basic types (u8, u32, etc)
   - `src/platform/window.h` - SDL2 window wrapper
   - `src/platform/input.h` - SDL2 input wrapper

4. **Global search/replace in ALL source files**:
   ```
   #include <revolution/OS.h> → #include <thread>
   #include <revolution/MTX.h> → #include <glm/glm.hpp>
   OSThread → std::thread
   OSMutex → std::mutex  
   Mtx → glm::mat4
   Vec → glm::vec3
   ```

5. **Update src/main.cpp** to native SDL2 (already done)

### Week 3-4: Category 2 (Rewrite NW4R/EGG)

1. **Modify nw4r headers** to use GLM/STL
2. **Rewrite nw4r::ut** to use C++ STL
3. **Update EGG** to use std::thread, SDL2, OpenGL
4. **Fix compilation errors** from library changes

### Week 5-8: Category 3 (Port Dolphin Graphics)

1. **Extract Dolphin VideoCommon code**
2. **Create src/graphics/** directory with OpenGL renderer
3. **Replace all GX calls** with native OpenGL
4. **Implement texture decoding** for GX formats
5. **Generate shaders** from TEV combiner settings

### Week 9-12: Category 3 (Port Dolphin Audio)

1. **Extract Dolphin audio HLE code**
2. **Create src/audio/** with OpenAL backend
3. **Replace all AX calls** with native OpenAL
4. **Implement voice/channel management**

### Week 13-16: Integration & Testing

1. **Build and fix linker errors**
2. **Test each sport mini-game**
3. **Performance optimization**
4. **Bug fixing**

---

## File Modification Strategy

### Automated Replacements (sed/perl scripts)
Create scripts to bulk-replace common patterns:

```bash
# Replace types
find src -name "*.cpp" -o -name "*.h" | xargs sed -i 's/\bMtx\b/glm::mat4/g'
find src -name "*.cpp" -o -name "*.h" | xargs sed -i 's/\bVec\b/glm::vec3/g'

# Replace includes
find src -name "*.cpp" -o -name "*.h" | xargs sed -i 's/#include <revolution\/MTX.h>/#include <glm\/glm.hpp>/g'
```

### Manual Replacements (complex logic)
- GX rendering calls → OpenGL (requires understanding context)
- AX audio calls → OpenAL (requires voice management)
- Game-specific code → Case-by-case basis

---

## No Compatibility Layer Exception

**Only use wrappers for these specific cases**:
1. **GX texture formats** - No direct PC equivalent for Nintendo formats
   - Need decoder for CMPR, I4, I8, IA4, IA8, RGB565, RGB5A3
2. **DVD disc image reading** - Need to support .iso, .rvz formats
   - Use Dolphin's disc reader code
3. **Save data encryption** - Wii uses specific encryption
   - May need to emulate or convert

Everything else should be **direct native replacements**.
