# Wii Sports PC Port - Build Instructions

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

### Linux
```bash
sudo apt-get update
sudo apt-get install -y cmake build-essential libsdl2-dev libgl1-mesa-dev pkg-config
```

### macOS
```bash
brew install cmake sdl2
```

## Building

### Configure
```bash
# Linux/macOS
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Windows with vcpkg
cmake -B build -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
```

### Compile
```bash
cmake --build build --config Release
```

### Run
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
