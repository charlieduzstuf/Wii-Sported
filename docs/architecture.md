# Architecture Overview

## System Architecture

This document explains how the ROM-to-PC-executable build system works.

## High-Level Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                         USER'S COMPUTER                         │
│                                                                 │
│  ┌──────────────────┐           ┌─────────────────────────┐   │
│  │  Wii Sports ROM  │           │  Wii-Sported Repository │   │
│  │  (User Owned)    │           │  (Decompiled Code)      │   │
│  │                  │           │                         │   │
│  │  - Binary Code   │           │  - C/C++ Source (~30%) │   │
│  │  - Assets        │           │  - Build System        │   │
│  │  - Data          │           │  - PC Implementations  │   │
│  └────────┬─────────┘           └───────────┬─────────────┘   │
│           │                                  │                  │
│           │         ┌────────────────────────┘                  │
│           │         │                                           │
│           v         v                                           │
│  ┌─────────────────────────────────────────────┐               │
│  │       build_from_rom.py (Builder Tool)      │               │
│  │  ┌──────────────────────────────────────┐   │               │
│  │  │ 1. Validate ROM                      │   │               │
│  │  │ 2. Extract ROM Data                  │   │               │
│  │  │ 3. Configure CMake                   │   │               │
│  │  │ 4. Compile C++ Code                  │   │               │
│  │  │ 5. Link ROM Loader                   │   │               │
│  │  │ 6. Generate Executable               │   │               │
│  │  └──────────────────────────────────────┘   │               │
│  └─────────────────────┬───────────────────────┘               │
│                        │                                        │
│                        v                                        │
│  ┌─────────────────────────────────────────────┐               │
│  │         WiiSports.exe (PC Executable)       │               │
│  │  ┌──────────────────────────────────────┐   │               │
│  │  │ Decompiled Code (Native x86-64)      │   │               │
│  │  │ - Revolution SDK Replacements        │   │               │
│  │  │ - SDL2 Integration                   │   │               │
│  │  │ - OpenGL Graphics                    │   │               │
│  │  └──────────────────────────────────────┘   │               │
│  │  ┌──────────────────────────────────────┐   │               │
│  │  │ ROM Loader (Runtime Integration)     │   │               │
│  │  │ - Loads ROM at startup               │   │               │
│  │  │ - Extracts binary code sections      │   │               │
│  │  │ - Loads assets on demand             │   │               │
│  │  └──────────────────────────────────────┘   │               │
│  └─────────────────────────────────────────────┘               │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

## Build Process Flow

```
                    ┌──────────────────┐
                    │  User Runs       │
                    │  build_from_rom  │
                    └────────┬─────────┘
                             │
                             v
                    ┌──────────────────┐
                    │  Validate ROM    │
                    │  - Check exists  │
                    │  - Verify hash   │
                    │  - Check size    │
                    └────────┬─────────┘
                             │
                             v
                    ┌──────────────────┐
                    │  Setup Dirs      │
                    │  - Create temp   │
                    │  - Create output │
                    └────────┬─────────┘
                             │
                             v
                    ┌──────────────────┐
                    │  Extract ROM     │
                    │  - Copy to       │
                    │    orig/RSPE01   │
                    └────────┬─────────┘
                             │
                             v
                    ┌──────────────────┐
                    │  Check Deps      │
                    │  - CMake         │
                    │  - Compiler      │
                    │  - SDL2          │
                    └────────┬─────────┘
                             │
                             v
                    ┌──────────────────┐
                    │  Configure       │
                    │  CMake           │
                    │  - Set ROM path  │
                    │  - Find libs     │
                    └────────┬─────────┘
                             │
                             v
                    ┌──────────────────┐
                    │  Compile         │
                    │  - Build sources │
                    │  - Link libs     │
                    │  - Create exe    │
                    └────────┬─────────┘
                             │
                             v
                    ┌──────────────────┐
                    │  Copy Executable │
                    │  to Output Dir   │
                    └────────┬─────────┘
                             │
                             v
                    ┌──────────────────┐
                    │  Cleanup         │
                    │  - Remove temp   │
                    └────────┬─────────┘
                             │
                             v
                    ┌──────────────────┐
                    │  Success! 🎮     │
                    └──────────────────┘
```

## Runtime Architecture

```
When you run WiiSports.exe:

    ┌────────────────────────────────────────┐
    │         WiiSports.exe Process          │
    │                                        │
    │  ┌──────────────────────────────────┐  │
    │  │  1. Initialize SDL2              │  │
    │  │     - Create window              │  │
    │  │     - Setup OpenGL context       │  │
    │  │     - Init input devices         │  │
    │  └──────────────────────────────────┘  │
    │                                        │
    │  ┌──────────────────────────────────┐  │
    │  │  2. Load ROM (rom_loader.cpp)    │  │
    │  │     - Parse DOL header           │  │
    │  │     - Map code sections          │  │
    │  │     - Extract assets             │  │
    │  └──────────────────────────────────┘  │
    │                                        │
    │  ┌──────────────────────────────────┐  │
    │  │  3. Initialize Game Systems      │  │
    │  │     - RPSystem                   │  │
    │  │     - Memory manager             │  │
    │  │     - Graphics system            │  │
    │  └──────────────────────────────────┘  │
    │                                        │
    │  ┌──────────────────────────────────┐  │
    │  │  4. Main Game Loop               │  │
    │  │     ┌────────────────────────┐   │  │
    │  │     │ ◄─ SDL Events          │   │  │
    │  │     │ ◄─ Input Processing    │   │  │
    │  │     │ ◄─ Game Logic          │   │  │
    │  │     │     - Decompiled (fast)│   │  │
    │  │     │     - ROM code (slower)│   │  │
    │  │     │ ◄─ Rendering (OpenGL)  │   │  │
    │  │     └────────────────────────┘   │  │
    │  └──────────────────────────────────┘  │
    │                                        │
    └────────────────────────────────────────┘
              │
              │ Reads from
              v
    ┌──────────────────┐
    │  wii_sports.iso  │
    │  - Binary code   │
    │  - Assets        │
    └──────────────────┘
```

## Code Integration Strategy

### Decompiled Code (~30%)

```cpp
// Native x86-64 code - runs at full speed
void MyDecompiledFunction() {
    // This code runs natively
    // No interpretation needed
    // Full performance
}
```

### ROM Binary Code (~70%)

```cpp
// Binary code from ROM
// Loaded and executed via interpreter/JIT
uint32_t CallROMFunction(uint32_t address) {
    const ROMSection* section = ROMLoader_GetCodeSection(0);
    // Interpret or JIT-compile PowerPC code
    // Slower but functional
    return ExecuteBinaryCode(section, address);
}
```

### Hybrid Execution

```cpp
void GameLogic() {
    // Call decompiled function (fast)
    MyDecompiledFunction();
    
    // Call ROM function (slower)
    uint32_t result = CallROMFunction(0x80001234);
    
    // More decompiled code (fast)
    ProcessResult(result);
}
```

## File Structure

```
Wii-Sported/
│
├── build_from_rom.py          ← Main builder script
├── asset_extractor.py         ← Asset extraction tool
│
├── src/
│   ├── main.cpp               ← Entry point
│   ├── rom_loader.cpp         ← ROM integration
│   ├── pc_asset_loader.cpp    ← Asset loading
│   └── revolution/            ← Wii SDK replacements
│       ├── OS/                ← Operating system layer
│       ├── PAD/               ← Input (SDL2)
│       ├── VI/                ← Video (SDL2)
│       └── GX/                ← Graphics (OpenGL)
│
├── include/                   ← Header files
├── CMakeLists.txt             ← Build configuration
│
└── orig/RSPE01_01/            ← ROM location (user provides)
    └── wii_sports.iso
```

## Dependencies

```
Build-time:
- CMake 3.15+
- C++ Compiler (GCC/Clang/MSVC)
- SDL2 development libraries

Runtime:
- SDL2 runtime libraries
- OpenGL 2.1+
- Wii Sports ROM (for assets and binary code)
```

## Benefits of This Architecture

✅ **Legal**: Users provide their own ROM
✅ **Practical**: Works with incomplete decompilation
✅ **Progressive**: Performance improves as more code is decompiled
✅ **Simple**: One script builds everything
✅ **Cross-platform**: Windows, Linux, macOS support

## Future Improvements

🔄 **In Progress**:
- Better ROM code interpretation/JIT
- More decompiled code (ongoing)
- Asset conversion tools

📋 **Planned**:
- Full OpenGL graphics implementation
- Audio system (OpenAL)
- Multiplayer support
- Mod support
