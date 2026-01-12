# Wii Sports PC Port - Build Instructions

## Important: ROM Required At Compile Time

**This is a semi-complete decompilation.** The ROM contains binary PowerPC code that has NOT been decompiled yet.

**ROM is required AT COMPILE TIME as a build input:**
- The decompiled source code (in this repo) will be compiled to native x86-64
- The binary code from the ROM will be extracted and integrated
- Result: Complete PC executable containing both decompiled and binary code

## Prerequisites

### Windows
1. Install Visual Studio 2019 or later with C++ development tools
2. Install vcpkg:
   ```powershell
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   ./bootstrap-vcpkg.bat
   ./vcpkg integrate install
   ```
3. Install SDL2:
   ```powershell
   ./vcpkg install sdl2:x64-windows
   ```
4. Install Python 3.8+ for asset extraction

### Linux
```bash
sudo apt-get update
sudo apt-get install -y cmake build-essential libsdl2-dev libgl1-mesa-dev pkg-config python3
```

### macOS
```bash
brew install cmake sdl2 python3
```

## Step 1: Place Your ROM

**Before building**, you must place your Wii Sports ROM in the expected location:

```bash
mkdir -p orig/RSPE01_01
cp /path/to/your/wii_sports.iso orig/RSPE01_01/
```

The build system will extract code from this ROM during compilation.

## Step 2: Build the PC Executable (ROM Extracted During Build)

### Configure
```bash
# Linux/macOS
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Windows with vcpkg
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
```

### Compile
```bash
cmake --build build --config Release
```

This creates `build/bin/WiiSports` (or `WiiSports.exe` on Windows).

## Extracting Assets from ROM (ONE-TIME OPERATION)

Before running the game, you must extract assets from your Wii Sports ROM:

```bash
python asset_extractor.py --rom /path/to/your/wii_sports.iso
```

This creates a `pc_assets/` directory with all game data.  
**The ROM is no longer needed after this step!**

## Running the Game

```bash
# Linux/macOS
./build/bin/WiiSports

# Windows
./build/bin/Release/WiiSports.exe
```

## Current Status

### Implemented (Ready to Build)
- OS layer (time, threading, mutex, cache, allocation)
- Input (PAD/WPAD/KPAD via SDL2)
- Video interface (VI via SDL2)  
- Basic file I/O (DVD)

### In Progress
- GX Graphics → OpenGL translation
- AX Audio → OpenAL translation
- MEM heap manager
- Game logic integration

### Notes
- Build will compile but game won't run yet (needs GX/AX implementation)
- Window will open with SDL2 context
- Input system is functional
- Asset loading not yet implemented

## Development Build

For active development:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Troubleshooting

### SDL2 not found
- Linux: Ensure `libsdl2-dev` and `pkg-config` are installed
- Windows: Verify vcpkg SDL2 installation
- macOS: Run `brew install sdl2`

### OpenGL not found
- Linux: Install `libgl1-mesa-dev`
- Windows: Usually included with graphics drivers
- macOS: Included with Xcode Command Line Tools

## GitHub Actions

Automated builds run on:
- Ubuntu Latest (Linux build)
- Windows Latest (MSVC build)
- Artifacts uploaded for each platform

See `.github/workflows/build-pc.yml` for CI configuration.
