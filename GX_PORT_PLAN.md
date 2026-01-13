# GX to OpenGL Port Plan - Based on Dolphin Emulator

## Overview
Port Dolphin's GX High-Level Emulation (HLE) to standalone OpenGL renderer for Wii Sports.

## Dolphin Code References

### Core GX State Management
**From: `Source/Core/VideoCommon/`**

1. **BPMemory.cpp/.h** (~2000 LOC)
   - Blend Processor state
   - Pixel processing configuration
   - Z-buffer, blending, alpha test
   - **Extract**: State structures, blend mode handling

2. **XFMemory.cpp/.h** (~1500 LOC)
   - Transform unit state  
   - Lighting configuration
   - Vertex transformations
   - **Extract**: Matrix management, lighting state

3. **CPMemory.cpp/.h** (~800 LOC)
   - Command Processor state
   - Vertex format definitions
   - **Extract**: Vertex attribute setup

### Texture Handling
**From: `Source/Core/VideoCommon/`**

4. **TextureDecoder.cpp** (~3000 LOC)
   - Nintendo texture format decoders
   - Formats: CMPR, I4, I8, IA4, IA8, RGB565, RGB5A3, RGBA8
   - **Extract**: Decode functions for each format
   - **Port**: Convert to OpenGL texture uploads

5. **TextureCache.cpp** (~2000 LOC)
   - Texture caching system
   - **Simplify**: Basic cache with texture ID mapping

### Vertex Processing
**From: `Source/Core/VideoCommon/`**

6. **VertexLoader.cpp** (~1000 LOC)
   - Loads vertex data from GX format
   - **Extract**: Vertex format parsing
   - **Port**: Convert to OpenGL vertex arrays

### Shader Generation
**From: `Source/Core/VideoCommon/`**

7. **PixelShaderGen.cpp** (~1500 LOC)
   - Generates GLSL pixel shaders from TEV state
   - **Critical**: TEV combiner → fragment shader
   - **Extract**: Shader generation logic

8. **VertexShaderGen.cpp** (~1000 LOC)
   - Generates GLSL vertex shaders
   - **Extract**: Transform/lighting shader code

### OpenGL Backend
**From: `Source/Core/VideoBackends/OGL/`**

9. **OGLRender.cpp** (~1000 LOC)
   - Main OpenGL renderer
   - **Extract**: Rendering loop, state management

10. **OGLTexture.cpp** (~800 LOC)
    - OpenGL texture wrapper
    - **Extract**: Texture creation/binding

11. **OGLShader.cpp** (~500 LOC)
    - Shader compilation
    - **Extract**: GLSL compilation helpers

12. **OGLPipeline.cpp** (~400 LOC)
    - Pipeline state management
    - **Extract**: Pipeline setup

## Implementation Strategy

### Phase 1: Basic GX State (Week 1)
Create `src/gx_opengl/` directory with:

**File: gx_state.h/cpp**
- GX state tracking structures
- Based on Dolphin's BP/XF/CP memory
- Functions: GXSetBlendMode, GXSetCullMode, GXSetZMode, etc.

```cpp
struct GXState {
    // Blend processor state
    struct {
        GXBlendMode blendMode;
        GXCullMode cullMode;
        GXCompare zMode;
        bool zUpdate;
    } bp;
    
    // Transform state
    struct {
        Mtx modelView;
        Mtx projection;
        bool lighting;
    } xf;
    
    // Texture state
    struct {
        u32 boundTextures[8];
        GXTexObj texObj[8];
    } tex;
};
```

### Phase 2: Texture Decoders (Week 1-2)
**File: texture_decoder.h/cpp**
- Port Dolphin's texture format decoders
- Implement: DecodeCMPR, DecodeI4, DecodeI8, DecodeIA4, DecodeIA8, DecodeRGB565, DecodeRGB5A3, DecodeRGBA8
- Upload to OpenGL textures

```cpp
void DecodeTexture(const u8* src, u8* dst, u32 width, u32 height, GXTexFmt format);
GLuint UploadTexture(const u8* data, u32 width, u32 height, GXTexFmt format);
```

### Phase 3: Vertex Handling (Week 2)
**File: vertex_loader.h/cpp**
- Parse GX vertex formats
- Convert to OpenGL vertex arrays
- Handle different attribute types

```cpp
struct VertexFormat {
    GXAttr attr;
    GXAttrType type;
    u8 componentCount;
};

void LoadVertices(const void* data, const VertexFormat* format, u32 count);
```

### Phase 4: Shader Generation (Week 2-3)
**File: shader_gen.h/cpp**
- Generate GLSL shaders from GX state
- TEV combiner → fragment shader
- Lighting → vertex shader

```cpp
std::string GenerateVertexShader(const GXState& state);
std::string GenerateFragmentShader(const GXState& state);
GLuint CompileShaderProgram(const std::string& vs, const std::string& fs);
```

### Phase 5: Rendering (Week 3)
**File: gx_renderer.h/cpp**
- Main GX→OpenGL command translator
- Implement all GX draw calls

```cpp
void GXBegin(GXPrimitive type, GXVtxFmt fmt, u16 nverts);
void GXPosition3f32(f32 x, f32 y, f32 z);
void GXColor4u8(u8 r, u8 g, u8 b, u8 a);
void GXTexCoord2f32(f32 s, f32 t);
void GXEnd();
void GXDrawDone();
```

## Key GX Functions to Implement

### Initialization
- GXInit
- GXSetViewport
- GXSetScissor

### Transform/Lighting
- GXLoadPosMtxImm
- GXLoadNrmMtxImm  
- GXSetNumChans
- GXSetChanCtrl
- GXSetNumTexGens

### Rendering
- GXBegin / GXEnd
- GXPosition3f32
- GXNormal3f32
- GXColor4u8
- GXTexCoord2f32

### Textures
- GXInitTexObj
- GXInitTexObjLOD
- GXLoadTexObj
- GXInvalidateTexAll

### TEV (Texture Environment)
- GXSetTevOp
- GXSetTevColor
- GXSetTevOrder
- GXSetTevAlphaIn/ColorIn

### State
- GXSetBlendMode
- GXSetAlphaCompare
- GXSetZMode
- GXSetCullMode
- GXSetDither

### Display
- GXCopyDisp
- GXSetCopyFilter
- GXSetDispCopyGamma

## Dolphin License Note
Dolphin is GPL-2.0 licensed. Extracted code must:
1. Credit Dolphin Emulator Project
2. Maintain GPL-2.0 license for those portions
3. Document which code comes from Dolphin

Add to each ported file:
```cpp
// Portions adapted from Dolphin Emulator
// Copyright 2008-2024 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later
```

## Testing Strategy
1. Implement basic triangle rendering
2. Test texture loading/display
3. Test lighting
4. Test TEV combiners
5. Integrate with NW4R
6. Test with Wii Sports scenes

## Estimated Complexity
- **Lines of Code**: ~8,000-10,000 LOC to port/adapt
- **Time**: 2-3 weeks for experienced graphics programmer
- **Difficulty**: High (requires graphics pipeline knowledge)

## Success Criteria
- Can render basic 3D geometry
- Textures load and display correctly
- Lighting works
- TEV combiners produce correct output
- NW4R g3d works with new backend
- Wii Sports title screen renders
