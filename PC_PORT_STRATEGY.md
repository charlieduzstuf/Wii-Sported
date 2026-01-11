# Wii Sports PC Port - Three-Tier Strategy

## Category 1: Library Replacement (Drop-in PC Libraries)
These Wii libraries can be directly replaced with existing PC equivalents:

### Graphics Foundation
- **GX Init/Context** → SDL2 + OpenGL context
  - Library: SDL2 (window/context management)
  - Library: GLAD/GLEW (OpenGL loader)

### Math Libraries  
- **MTX (revolution/MTX)** → GLM (OpenGL Mathematics)
  - Library: GLM (header-only, nearly identical API)
  - Minimal wrapper needed for exact function names

- **nw4r::math** → GLM
  - Library: GLM
  - Already very similar to GLM design

### Audio Framework
- **Basic Audio** → SDL2_mixer or OpenAL
  - Library: OpenAL-Soft (3D audio, similar to AX)
  - Alternative: SDL2_mixer (simpler)

### Image Loading
- **TPL (Texture Palette Library)** → stb_image
  - Library: stb_image.h (header-only)
  - Handles all common formats

### Compression/Archives
- **ARC (Archive)** → libarchive or minizip
  - Library: zlib + custom ARC parser
  - Or use existing archive library

### Font Rendering
- **ut::Font** → FreeType or stb_truetype
  - Library: FreeType2
  - Alternative: stb_truetype.h (header-only)

### Threading/OS
- **OS (revolution/OS threads)** → SDL2 threads
  - Library: SDL2 (cross-platform threading)
  - Alternative: C++11 std::thread

### File I/O
- **DVD/FS** → Standard C/C++ file I/O or SDL_RWops
  - Library: stdio.h / fstream
  - SDL_RWops for compatibility

### Input Basics
- **PAD/WPAD/KPAD input** → SDL2 input
  - Library: SDL2 (keyboard, mouse, gamepad)
  - Already done in compat layer

---

## Category 2: Library Rewrites (Small/Simple Internal Code)
These are small enough to rewrite from scratch for PC:

### Revolution SDK - Simple Components
- **OS/OSTime.h** - Time functions
  - Rewrite: Use SDL_GetTicks64() / QueryPerformanceCounter
  - ~50 lines of code

- **OS/OSCache.h** - Cache management  
  - Rewrite: No-op macros (PC handles cache automatically)
  - ~10 lines of code

- **OS/OSAlloc.h** - Basic allocation
  - Rewrite: Wrapper around malloc/free
  - ~20 lines of code

- **MEM/mem_allocator.h** - Simple allocators
  - Rewrite: Custom allocators using HeapAlloc/malloc
  - ~200 lines of code

- **VI (Video Interface)** - Simple video control
  - Rewrite: SDL2 window control wrapper
  - ~100 lines of code

### NW4R - Utility Components
- **ut::List, ut::LinkList** - Linked lists
  - Rewrite: Standard C++ std::list wrapper
  - ~50 lines of code

- **ut::Color** - Color utilities
  - Rewrite: Simple struct with helpers
  - ~30 lines of code

### EGG - Simple Utilities
- **eggAssert** - Assertions
  - Rewrite: Standard assert() wrapper
  - ~10 lines of code

- **eggMath** - Basic math helpers
  - Rewrite: Standard C math library wrappers
  - ~50 lines of code

---

## Category 3: Dolphin Integration (Complex/Critical Systems)
Use Dolphin emulator code to port these complex systems:

### Graphics Pipeline (HIGH PRIORITY)
- **GX (revolution/GX)** - GPU commands
  - Source: `dolphin/Source/Core/VideoCommon/`
  - Approach: Port Dolphin's GX→OpenGL translator
  - Complexity: High (~5000 lines)
  - Files needed:
    - `VideoCommon/BPMemory.cpp` (blend/pixel processor)
    - `VideoCommon/CPMemory.cpp` (command processor)
    - `VideoCommon/XFMemory.cpp` (transform/lighting)
    - `VideoBackends/OGL/` (OpenGL backend)

### Audio System (HIGH PRIORITY)
- **AX/AXFX (revolution/AX)** - Audio processing
  - Source: `dolphin/Source/Core/Core/HW/DSPHLE/`
  - Approach: Use Dolphin's HLE audio implementation
  - Complexity: High (~3000 lines)
  - Files needed:
    - `Core/HW/DSPHLE/UCodes/AX.cpp`
    - `Core/HW/DSPHLE/UCodes/AXWii.cpp`
    - `AudioCommon/Mixer.cpp`

### DSP (Digital Signal Processor)
- **DSP** - Sound processing
  - Source: `dolphin/Source/Core/Core/DSP/`
  - Approach: Use Dolphin's DSP HLE
  - Complexity: Very High
  - Note: OpenAL can replace most functionality

### Hardware Emulation Layer
- **IPC (Inter-Process Communication)**
  - Source: `dolphin/Source/Core/Core/IPC_HLE/`
  - Approach: Port relevant IPC handlers
  - Complexity: Medium (~1000 lines)
  - Needed for: Save data, network features

- **DVD (Disc I/O)**
  - Source: `dolphin/Source/Core/Core/HW/DVD/`
  - Approach: Use Dolphin's disc image reader
  - Complexity: Medium (~800 lines)
  - Supports: ISO, RVZ, WIA, WBFS formats

### Memory Management (if needed)
- **MEM (revolution/MEM)** - Complex heap management
  - Source: `dolphin/Source/Core/Core/HW/Memmap.cpp`
  - Approach: Port if game relies on specific memory layout
  - Complexity: Medium
  - Alternative: Standard allocators may suffice

### Specific Middleware (if not replaceable)
- **NW4R g3d** - 3D graphics framework
  - If replacement fails, port with Dolphin's help
  - Source: Study Dolphin's game-specific patches
  - Complexity: Very High

- **EGG framework** - Game framework
  - If replacement fails, minimal port
  - May work with Category 1+2 changes
  - Complexity: High

---

## Implementation Order

### Phase 1: Category 1 (Week 1-2)
- Set up SDL2, OpenGL, GLM, OpenAL
- Replace all simple libraries
- Get basic window/rendering working

### Phase 2: Category 2 (Week 3-4)  
- Rewrite small Revolution SDK components
- Rewrite NW4R utilities
- Get basic game initialization working

### Phase 3: Category 3 (Week 5-12)
- Port Dolphin's GX→OpenGL translator
- Port Dolphin's AX audio HLE
- Port Dolphin's DVD reader (for ROM loading)
- Port IPC for save data
- Integration and testing

### Phase 4: Game-Specific (Week 13-16)
- Test each sport mini-game
- Fix game-specific issues
- Performance optimization
- Polish and release

---

## Dolphin Code to Extract

### Essential Files from Dolphin:
```
Source/Core/VideoCommon/
├── BPMemory.h/.cpp           (Blending/Pixel Processor)
├── CPMemory.h/.cpp           (Command Processor)  
├── XFMemory.h/.cpp           (Transform/Lighting)
├── VertexLoader*.cpp         (Vertex data handling)
└── TextureDecoder*.cpp       (Texture formats)

Source/Core/VideoBackends/OGL/
├── OGLRender.cpp             (OpenGL renderer)
├── OGLTexture.cpp            (Texture handling)
├── OGLShader.cpp             (Shader compilation)
└── OGLPipeline.cpp           (Pipeline state)

Source/Core/Core/HW/DSPHLE/UCodes/
├── AX.cpp                    (AX audio HLE)
└── AXWii.cpp                 (Wii-specific AX)

Source/Core/Core/HW/
├── DVD/DVDInterface.cpp      (DVD reading)
└── Memmap.cpp                (Memory mapping)

Source/Core/AudioCommon/
├── Mixer.cpp                 (Audio mixing)
└── AudioCommon.cpp           (Audio backend)
```

These ~20-30 files from Dolphin will handle the most complex porting challenges.
