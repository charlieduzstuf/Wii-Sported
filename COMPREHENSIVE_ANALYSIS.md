# Comprehensive Wii Sports PC Port Analysis

## Summary of Our Conversation

**User's Goal:** Port Wii Sports decompilation to Windows PC executable

**Key Requirements:**
1. Native port (no compatibility layers unless unavoidable)
2. Reference ogws_info and Dolphin repos for code/info
3. Replace Wii libraries with PC equivalents
4. Work incrementally with separate commits per major step
5. Minimize directory changes

**References Provided:**
- https://github.com/kiwi515/ogws_info - Documentation of Wii Sports structures
- https://github.com/dolphin-emu/dolphin - Emulator with HLE implementations

## Commits Made So Far

1. **Initial analysis** - Created strategy documents
2. **Modified src/main.cpp** - Added SDL2 initialization with OpenGL
3. **OS/OSTime** - Implemented SDL2 time functions
4. **OS/OSThread, OSMutex, OSCache** - Implemented SDL2 threading/mutexes, no-op cache
5. **OS/OSAlloc, DVD** - Implemented malloc/free wrappers, basic file I/O
6. **PAD input** - Implemented SDL2 gamepad/keyboard (in progress)

## Codebase Statistics

**Total Files:** 704 source files, 866 headers, 317K+ LOC

**Files Using Revolution SDK:** 223 files

**Library Breakdown:**
- src/nw4r: 187 files
- src/egg: 68 files
- src/Pack: 145+ files
- src/revolution: ~150 files (SDK implementation)
- src/MSL: ~50 files (standard library)
- src/runtime: ~10 files
- src/RVLFaceLib: 15 files
- src/homebuttonMiniLib: 6 files

## Revolution SDK Usage Analysis

**Most Used Headers (Priority Order):**
1. **revolution/OS.h** - 110 uses (DONE: Time, Thread, Mutex, Cache, Alloc)
2. **revolution/GX.h** - 59 uses (TODO: Complex - OpenGL translation)
3. **revolution/AX.h** - 19 uses (TODO: Complex - OpenAL translation)
4. **revolution/DVD.h** - 16 uses (DONE: Basic file I/O)
5. **revolution/NWC24.h** - 15 uses (TODO: Network - may stub out)
6. **revolution/MTX.h** - 14 uses (TODO: Keep existing or use GLM)
7. **revolution/BASE.h** - 14 uses (TODO: Check contents)
8. **revolution/VI.h** - 13 uses (TODO: SDL2 window/display)
9. **revolution/SC.h** - 13 uses (TODO: System config)
10. **revolution/MEM.h** - 13 uses (TODO: Memory management)
11. **revolution/IPC.h** - 12 uses (TODO: Inter-process comm)
12. **revolution/WPAD.h** - 11 uses (TODO: Wii Remote → mouse/keyboard)
13. **revolution/NAND.h** - 11 uses (TODO: Save data storage)
14. **revolution/AXFX.h** - 9 uses (TODO: Audio effects)
15. **revolution/PAD.h** - 6 uses (DONE: SDL2 gamepad)

## Detailed Component Mapping

### CATEGORY 1: Direct Library Replacement (Use Existing PC Libraries)

#### Completed ✓
- **OS/OSTime** → SDL2 performance counters
- **OS/OSThread** → SDL2 threads
- **OS/OSMutex** → SDL2 mutexes
- **OS/OSCache** → No-ops (x86 handles automatically)
- **OS/OSAlloc** → malloc/free
- **PAD** → SDL2 gamepad/keyboard

#### TODO - Simple Replacements
- **MTX** → GLM (OpenGL Mathematics) or keep existing C structs
  - Types: Mtx[3][4], Vec, Quaternion already compatible
  - Functions: PSMTXIdentity, PSMTXConcat, PSVECAdd, etc.
  - Action: Map to GLM or keep (already portable C)

- **VI (Video Interface)** → SDL2 window management
  - VIInit → SDL_CreateWindow
  - VISetBlack → Clear screen
  - VIFlush → SDL_GL_SwapWindow
  - Action: Simple mapping, ~10 functions

- **WPAD/KPAD** → SDL2 mouse/keyboard
  - WPADInit, KPADRead → SDL_GetMouseState, SDL_GetKeyboardState
  - Action: Similar to PAD, already started

- **NAND** → PC file system
  - NANDOpen → fopen
  - NANDRead/Write → fread/fwrite
  - Action: Map to standard file I/O for save data

- **ARC** → Archive reading
  - ARCOpen → Custom format reader
  - Action: Port existing decompressed arc reader or use zlib

- **TPL** → Texture loading
  - TPLBind → Load texture to OpenGL
  - Action: Decode Nintendo formats, upload to OpenGL

### CATEGORY 2: Small Library Rewrites

#### TODO - Rewrites Needed
- **MEM (Memory Manager)** → Custom heap system
  - MEMInitExpHeap, MEMAllocFromExpHeap
  - Action: Implement simple heap allocator on top of malloc
  - Complexity: Medium (~200 lines)

- **IPC** → Stub or minimal implementation
  - IPCReadMessage, IPCWriteMessage
  - Action: May not be needed for single-player, stub out
  - Complexity: Low (stubs) or Medium (if needed for save data)

- **SC (System Config)** → Config file
  - SCGetLanguage, SCGetAspectRatio
  - Action: Read from config file or hardcode defaults
  - Complexity: Low (~50 lines)

- **NWC24** → Network features
  - May not be needed for offline play
  - Action: Stub out or remove
  - Complexity: Low (stubs)

- **BASE** → PowerPC utilities
  - PPCSync, PPCHalt
  - Action: No-ops or minimal x86 equivalents
  - Complexity: Low (~20 lines)

### CATEGORY 3: Complex - Use Dolphin Code

#### TODO - Reference Dolphin Emulator

**GX (Graphics) - HIGHEST PRIORITY**
- **Files affected:** 59 source files
- **Complexity:** Very High
- **Dolphin reference:**
  - `Source/Core/VideoCommon/BPMemory.cpp` - Blend/pixel processor
  - `Source/Core/VideoCommon/XFMemory.cpp` - Transform/lighting
  - `Source/Core/VideoCommon/VertexLoader*.cpp` - Vertex handling
  - `Source/Core/VideoBackends/OGL/` - OpenGL backend
  - `Source/Core/VideoCommon/TextureDecoder.cpp` - Nintendo texture formats

- **Implementation strategy:**
  1. Extract Dolphin's GX state tracking
  2. Create OpenGL renderer that interprets GX commands
  3. Port TEV (Texture Environment) to GLSL shaders
  4. Decode Nintendo texture formats (CMPR, I4, I8, RGB565, etc.)
  5. Map GX primitives to OpenGL (GX_TRIANGLES → GL_TRIANGLES)

- **Key GX functions to implement:**
  - GXInit, GXSetViewport, GXSetScissor
  - GXBegin, GXEnd, GXPosition3f32, GXColor4u8, GXTexCoord2f32
  - GXLoadPosMtxImm, GXSetNumChans, GXSetCullMode
  - GXInitTexObj, GXLoadTexObj, GXSetTevOp
  - GXSetBlendMode, GXSetAlphaCompare, GXSetZMode
  - GXCopyDisp, GXDrawDone, GXFlush

**AX (Audio) - HIGH PRIORITY**
- **Files affected:** 19 source files (+ 9 AXFX)
- **Complexity:** High
- **Dolphin reference:**
  - `Source/Core/Core/HW/DSPHLE/UCodes/AX.cpp` - AX emulation
  - `Source/Core/Core/HW/DSPHLE/UCodes/AXWii.cpp` - Wii-specific
  - `Source/Core/AudioCommon/Mixer.cpp` - Audio mixing
  - `Source/Core/AudioCommon/` - Audio backend

- **Implementation strategy:**
  1. Extract Dolphin's AX voice management
  2. Map AX voices to OpenAL sources
  3. Implement audio mixing (Dolphin has this)
  4. Port audio effects (reverb, chorus, delay)
  5. Handle 3D audio positioning

- **Key AX functions to implement:**
  - AXInit, AXQuit, AXRegisterCallback
  - AXSetVoiceState, AXSetVoiceAddr, AXSetVoiceLoop
  - AXSetVoiceVe, AXSetVoiceSrc, AXSetVoiceSrcType
  - AXFXReverbHiInit, AXFXChorusInit, AXFXDelayInit
  - AXGetAuxCallback, AXSetCompressor

**DSP (Digital Signal Processor)**
- **Files affected:** 8 source files
- **Complexity:** Very High
- **Action:** Use Dolphin's HLE DSP or OpenAL for most audio
- **May not need full DSP emulation** if AX layer works

## NW4R Library Analysis

**187 files** - Nintendo middleware framework

**Components:**
- **nw4r::ut** (Utilities) - Lists, color, streams, fonts
  - Action: Port to C++ STL or keep (mostly portable)
  - Complexity: Low-Medium

- **nw4r::math** - Vector/matrix math
  - Action: Use GLM or keep (already C-compatible)
  - Complexity: Low

- **nw4r::g3d** - 3D graphics on top of GX
  - Action: Update to use our GX→OpenGL layer
  - Complexity: Medium (depends on GX port)

- **nw4r::snd** - Sound on top of AX
  - Action: Update to use our AX→OpenAL layer
  - Complexity: Medium (depends on AX port)

- **nw4r::lyt** - Layout/UI system
  - Action: Update to use our GX→OpenGL layer
  - Complexity: Medium

- **nw4r::ef** - Effects system
  - Action: Update to use our GX→OpenGL layer
  - Complexity: Medium

## EGG Framework Analysis

**68 files** - Game framework on top of NW4R

**Components:**
- **egg::core** - System, heap, threads, scene management
  - Action: Update to use SDL2/C++ STL
  - Complexity: Medium

- **egg::gfx** - Graphics framework
  - Action: Update to use our GX→OpenGL layer
  - Complexity: High (tightly coupled to GX)

- **egg::audio** - Audio framework
  - Action: Update to use our AX→OpenAL layer
  - Complexity: Medium

- **egg::math** - Math utilities
  - Action: Keep or use GLM
  - Complexity: Low

## Pack/RP Game Code Analysis

**145+ files** - Actual Wii Sports game logic

**Components:**
- **RPKernel** - Core game systems
- **RPSystem** - Game initialization, scenes
- **RPGraphics** - Game rendering
- **RPAudio** - Game audio
- **RPSports** - Individual sport implementations

**Action:** Should work with minimal changes once SDK/middleware ported
**Complexity:** Low (after dependencies resolved)

## Implementation Priority

### Phase 1: Foundation (THIS WEEK) - MOSTLY DONE
- [x] Build system (CMake)
- [x] Entry point (SDL2 init)
- [x] OS layer basics (time, threads, mutex, cache, alloc)
- [x] Input basics (PAD)
- [ ] WPAD/KPAD (mouse/keyboard)
- [ ] VI (window management)
- [ ] MTX (math - decide keep vs GLM)

### Phase 2: Core SDK (NEXT 1-2 WEEKS)
- [ ] MEM (memory manager)
- [ ] DVD/FS (file I/O completion)
- [ ] NAND (save data)
- [ ] ARC/TPL (archives/textures)
- [ ] SC (config)
- [ ] Stub non-essential (NWC24, IPC, etc.)

### Phase 3: Graphics (2-3 WEEKS) - CRITICAL PATH
- [ ] Study Dolphin's VideoCommon
- [ ] Extract GX state machine
- [ ] Implement OpenGL backend
- [ ] Port texture decoders
- [ ] Generate GLSL from TEV
- [ ] Test with simple rendering
- [ ] Update nw4r::g3d
- [ ] Update egg::gfx

### Phase 4: Audio (1-2 WEEKS) - CRITICAL PATH
- [ ] Study Dolphin's DSPHLE
- [ ] Extract AX emulation
- [ ] Implement OpenAL backend
- [ ] Port audio effects
- [ ] Test with simple sounds
- [ ] Update nw4r::snd
- [ ] Update egg::audio

### Phase 5: Integration (2-3 WEEKS)
- [ ] Build all source files
- [ ] Fix compilation errors
- [ ] Link everything
- [ ] Test game initialization
- [ ] Test each sport
- [ ] Fix runtime issues
- [ ] Optimize performance

## Files Needing Modification

### Immediate (This Session):
- include/revolution/WPAD/*.h
- include/revolution/KPAD/*.h
- include/revolution/VI/*.h
- include/revolution/MTX/*.h
- src/revolution/WPAD/*.c
- src/revolution/VI/*.c

### Short Term:
- include/revolution/MEM/*.h
- include/revolution/ARC/*.h
- include/revolution/TPL/*.h
- src/revolution/MEM/*.c
- src/revolution/ARC/*.c

### Medium Term (GX):
- include/revolution/GX/*.h (all)
- src/revolution/GX/*.c (all)
- Create: src/gx_opengl/ directory with new OpenGL backend

### Medium Term (AX):
- include/revolution/AX/*.h (all)
- include/revolution/AXFX/*.h (all)
- src/revolution/AX/*.c (all)
- Create: src/ax_openal/ directory with new OpenAL backend

## Dolphin Code to Extract

**Essential Dolphin files for reference:**

### Graphics (GX → OpenGL):
```
Source/Core/VideoCommon/
├── BPMemory.h/.cpp           ~2000 lines
├── XFMemory.h/.cpp           ~1500 lines
├── CPMemory.h/.cpp           ~800 lines
├── VertexLoader.cpp          ~500 lines
├── VertexManagerBase.cpp     ~600 lines
├── TextureDecoder*.cpp       ~3000 lines
└── ShaderGenCommon.cpp       ~400 lines

Source/Core/VideoBackends/OGL/
├── OGLRender.cpp             ~1000 lines
├── OGLTexture.cpp            ~800 lines
├── OGLShader.cpp             ~500 lines
├── OGLPipeline.cpp           ~400 lines
└── OGLVertexManager.cpp      ~300 lines
```

### Audio (AX → OpenAL):
```
Source/Core/Core/HW/DSPHLE/UCodes/
├── AX.cpp                    ~1500 lines
├── AXWii.cpp                 ~800 lines
└── UCodes.cpp                ~200 lines

Source/Core/AudioCommon/
├── Mixer.cpp                 ~600 lines
├── AudioCommon.cpp           ~400 lines
└── OpenALStream.cpp          ~300 lines
```

**Total Dolphin code to port/reference:** ~15,000 lines

## Build System Updates Needed

Current CMakeLists.txt needs:
- Add all src/revolution/PAD/Pad_pc.cpp type files
- Conditional compilation for PC sources
- Proper include paths
- Link SDL2, OpenGL, OpenAL, potentially GLM
- Exclude Wii-specific .c files when building PC version

## Testing Strategy

1. **Build incrementally** - Compile after each SDK component
2. **Unit test SDK replacements** - Ensure each works in isolation
3. **Test NW4R** - Ensure middleware works with new SDK
4. **Test EGG** - Ensure framework works
5. **Test game init** - RPSysSystem::initialize()
6. **Test each sport** - One at a time
7. **Performance profiling** - Optimize bottlenecks

## Risk Assessment

**High Risk:**
- GX → OpenGL translation (most complex)
- AX → OpenAL translation (second most complex)
- TEV shader generation (complex)
- Nintendo texture format decoding

**Medium Risk:**
- NW4R integration with new backends
- EGG integration with new backends
- Memory layout differences between Wii and PC
- Endianness issues (Wii is big-endian, PC is little-endian)

**Low Risk:**
- OS layer (mostly done)
- Input (mostly done)
- File I/O (straightforward)
- Math libraries (portable)

## Next Immediate Actions

1. Complete WPAD/KPAD input
2. Add VI window management
3. Commit these changes
4. Begin GX → OpenGL research/extraction from Dolphin
5. Create GX OpenGL backend skeleton
6. Implement basic GX functions
7. Test rendering
