# ROM Integration Plan for PC Port

## Problem Analysis

The Wii Sports decompilation project is **incomplete**. It requires the original ROM because:
1. **Binary code**: Some functions are not decompiled, exist only as compiled code in ROM
2. **Assets**: Textures, models, sounds, etc. are in the ROM
3. **Build process**: Current Wii build extracts code/data from ROM during compilation

## Solution: Hybrid Approach

The PC executable needs to:
1. **At build time**: Include all decompiled/ported C/C++ code
2. **At runtime**: Load binary code and assets from ROM

### Architecture

```
┌─────────────────────────────────────┐
│    WiiSports.exe (PC Executable)    │
│  ┌──────────────────────────────┐   │
│  │ Decompiled Code (Ported)     │   │
│  │ - RPSystem, RPKernel         │   │
│  │ - NW4R, EGG (ported)         │   │
│  │ - Revolution SDK (replaced)  │   │
│  └──────────────────────────────┘   │
│  ┌──────────────────────────────┐   │
│  │ ROM Loader Module            │   │
│  │ - Loads ROM at startup       │   │
│  │ - Extracts binary functions  │   │
│  │ - Extracts assets            │   │
│  └──────────────────────────────┘   │
└─────────────────────────────────────┘
              │
              ▼
    ┌──────────────────┐
    │  wii_sports.iso  │
    │ - Binary code    │
    │ - Assets         │
    └──────────────────┘
```

### Implementation Components

#### 1. ROM Loader (`src/rom_loader.cpp`)
- Open and parse Wii Sports ISO file
- Extract DOL executable sections
- Map binary code regions
- Extract file system (assets)

#### 2. Binary Code Integration
Two approaches:

**Option A: JIT Recompilation**
- Use LLVM or similar to JIT-compile PowerPC code to x86
- Complex but allows full execution of binary code

**Option B: Function Stubs + Dynamic Linking**
- For undecompiled functions, create stubs that:
  - Load binary code from ROM
  - Use interpreter or cached translation
  - Call into decompiled code when available

**Recommended: Start with Option B, migrate to Option A if needed**

#### 3. Asset System
Keep existing `pc_asset_loader` but integrate with ROM loader:
- ROM loader extracts assets to memory or temp directory
- Asset loader accesses extracted data
- No pre-extraction step needed

### User Workflow

**Simple approach:**
```bash
# 1. Build executable (decompiled code only)
cmake -B build
cmake --build build

# 2. Run with ROM path
./WiiSports.exe --rom path/to/wii_sports.iso

# First run: Extracts and caches binary code and assets
# Subsequent runs: Uses cached data
```

**Advanced approach (optional):**
```bash
# Prepare ROM data (one time)
./WiiSports.exe --prepare-rom path/to/wii_sports.iso

# Run game (no ROM path needed)
./WiiSports.exe
```

## Implementation Plan

### Phase 1: ROM Loader Infrastructure
1. Create `rom_loader.cpp` - ISO parsing and file extraction
2. Create `binary_cache.cpp` - Cache extracted code/assets
3. Update `main.cpp` - Initialize ROM loader at startup
4. Add command-line argument parsing for ROM path

### Phase 2: Binary Code Integration
1. Identify which functions are not decompiled (check `orig/` directory)
2. Create function stub system
3. Implement basic PowerPC interpreter for undecompiled functions
4. OR: Use Dolphin's cached interpreter/JIT

### Phase 3: Asset Integration
1. Integrate ROM loader with `pc_asset_loader`
2. On-demand asset extraction from ROM
3. Asset caching to disk for performance

### Phase 4: Testing & Optimization
1. Test with actual Wii Sports ROM
2. Profile performance (binary interpretation overhead)
3. Gradually replace interpreted functions with ported code
4. Optimize asset loading

## Benefits

✅ **Realistic**: Accounts for incomplete decompilation
✅ **Legal**: Users provide their own ROM
✅ **Practical**: Works with current state of decompilation
✅ **Progressive**: Can improve as more code is decompiled
✅ **Simple**: Single executable + ROM file

## Technical Challenges

1. **PowerPC to x86 translation**: Need interpreter or JIT
2. **Binary compatibility**: Ensuring decompiled code can call binary code
3. **Memory layout**: PowerPC vs x86 differences
4. **Performance**: Interpreted code will be slower

## Alternative: Use Dolphin Integration

Instead of building custom interpreter, leverage Dolphin Emulator:
1. Embed Dolphin's PowerPC interpreter
2. Use Dolphin's HLE for Revolution SDK
3. Replace HLE implementations with our PC implementations
4. Run decompiled code natively, binary code via interpreter

This is **essentially what Dolphin does** - we can reuse their work!
