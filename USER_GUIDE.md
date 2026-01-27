# Wii Sports PC - User Guide

## Overview

This repository allows you to build a PC version of Wii Sports from your own ROM. Since only ~30% of the game has been decompiled, the build process requires your ROM to provide the remaining code.

## Two-Stage Build Process

### What You Get

1. **Stage 1**: Download this repository (contains decompiled code + build tools)
2. **Stage 2**: Run the builder with your ROM to create the PC executable

### Why This Approach?

- **Legal**: You provide your own ROM (we don't distribute any Nintendo code)
- **Practical**: Works with incomplete decompilation (~30% done)
- **Simple**: One command to build everything

## Quick Start

### Prerequisites

Install these tools on your system:

#### Windows
1. **Visual Studio 2019 or later** (with C++ desktop development)
   - Download from: https://visualstudio.microsoft.com/downloads/
   - Or install Visual Studio Build Tools
   
2. **CMake 3.15+**
   - Download from: https://cmake.org/download/
   - Or: `winget install Kitware.CMake`

3. **vcpkg** (for SDL2)
   ```powershell
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   ./bootstrap-vcpkg.bat
   ./vcpkg integrate install
   ./vcpkg install sdl2:x64-windows
   ```

4. **Python 3.8+**
   - Download from: https://www.python.org/downloads/

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libsdl2-dev libgl1-mesa-dev pkg-config python3
```

#### Linux (Fedora/RHEL)
```bash
sudo dnf install gcc-c++ cmake SDL2-devel mesa-libGL-devel pkg-config python3
```

#### macOS
```bash
brew install cmake sdl2 python3
```

### Building Your PC Executable

1. **Clone or download this repository**
   ```bash
   git clone https://github.com/charlieduzstuf/Wii-Sported.git
   cd Wii-Sported
   ```

2. **Run the builder with your ROM**
   ```bash
   python build_from_rom.py --rom /path/to/your/wii_sports.iso
   ```

3. **Wait for compilation** (5-15 minutes depending on your system)

4. **Done!** Your executable is in `WiiSports_PC_Build/`

### Running the Game

After building, you need to extract assets from your ROM (one-time operation):

```bash
python asset_extractor.py --rom /path/to/your/wii_sports.iso
```

Then run the game:

```bash
# Windows
./WiiSports_PC_Build/WiiSports.exe

# Linux/Mac
./WiiSports_PC_Build/WiiSports
```

## Advanced Options

### Build Options

```bash
# Build debug version (for developers)
python build_from_rom.py --rom game.iso --debug

# Specify output directory
python build_from_rom.py --rom game.iso --output ./MyCustomBuild

# Keep temporary files for debugging
python build_from_rom.py --rom game.iso --keep-temp

# Verbose output
python build_from_rom.py --rom game.iso --verbose
```

### Supported ROM Versions

| Version         | SHA-1 Hash                                 | Status      |
| --------------- | ------------------------------------------ | ----------- |
| US (Revision 1) | `8bb422971b88b5551a37de98db69557df7b46637` | ✅ Verified |

To check your ROM version, use the in-game button combination on the title screen:
```
A, D-Up, 2, D-Down, B, 1, 1, B, D-Left, A, D-Right
```

## Troubleshooting

### "CMake not found"
- **Windows**: Install CMake from https://cmake.org/download/ and add to PATH
- **Linux**: `sudo apt-get install cmake` or `sudo dnf install cmake`
- **macOS**: `brew install cmake`

### "SDL2 not found"
- **Windows**: Install via vcpkg: `./vcpkg/vcpkg install sdl2:x64-windows`
- **Linux**: `sudo apt-get install libsdl2-dev`
- **macOS**: `brew install sdl2`

### "Compiler not found"
- **Windows**: Install Visual Studio or Visual Studio Build Tools
- **Linux**: `sudo apt-get install build-essential`
- **macOS**: Install Xcode Command Line Tools: `xcode-select --install`

### "ROM hash doesn't match"
Make sure you have the correct ROM version. The builder will warn you but may continue if you choose. Results may vary with untested ROM versions.

### Build fails with errors
1. Ensure all prerequisites are installed
2. Try with `--verbose` flag to see detailed error messages
3. Check that your ROM is valid (not corrupted)
4. Try with `--debug` flag for more diagnostic information

## How It Works

### The Build Process

1. **ROM Validation**: Checks that your ROM exists and is a valid size
2. **ROM Extraction**: Copies ROM to build location for processing
3. **Dependency Check**: Verifies CMake, compiler, and SDL2 are available
4. **CMake Configuration**: Sets up the build with ROM location
5. **Compilation**: Compiles decompiled C++ code + ROM integration
6. **Executable Creation**: Produces final PC executable

### What Gets Built

```
WiiSports_PC_Build/
├── WiiSports.exe (or WiiSports)  ← Your PC executable
└── temp/                          ← Temporary files (auto-cleaned)
```

The executable contains:
- ✅ Decompiled code (~30% of game) - compiled to native x86-64
- ✅ ROM loader - loads remaining code from ROM at runtime
- ✅ PC implementations - SDL2, OpenGL replacements for Wii APIs

### ROM Requirements

**Why is the ROM needed?**

This is a partial decompilation project. About 70% of the game code has not been decompiled yet. The ROM provides:
- **Binary code**: Non-decompiled game functions
- **Assets**: Textures, models, sounds, animations
- **Data**: Game configurations, scripts, layouts

**When is the ROM used?**

- **Build time**: ROM is referenced during compilation
- **Runtime**: Game loads binary code and assets from ROM

**Can I distribute the built executable?**

⚠️ **No** - The executable contains ROM code and is a derivative work. You can build it for personal use only. Each user must provide their own ROM.

## For Developers

### Manual Build (Without build_from_rom.py)

If you want to build manually:

1. Place ROM in expected location:
   ```bash
   mkdir -p orig/RSPE01_01
   cp /path/to/rom.iso orig/RSPE01_01/
   ```

2. Configure and build:
   ```bash
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build --config Release
   ```

3. Extract assets:
   ```bash
   python asset_extractor.py --rom /path/to/rom.iso
   ```

4. Run:
   ```bash
   ./build/bin/WiiSports
   ```

### Project Structure

```
Wii-Sported/
├── build_from_rom.py       ← User-friendly builder script
├── asset_extractor.py      ← Asset extraction tool
├── CMakeLists.txt          ← Build configuration
├── src/                    ← Decompiled source code
│   ├── main.cpp
│   ├── rom_loader.cpp      ← ROM integration
│   └── revolution/         ← Wii SDK replacements
├── include/                ← Headers
├── orig/                   ← ROM location (user provides)
│   └── RSPE01_01/
└── docs/                   ← Documentation
```

## Contributing

Want to help improve the PC port? See [CONTRIBUTING.md](docs/contributing.md)

Key areas:
- **Decompilation**: Help reverse engineer more game code
- **Graphics**: Improve GX → OpenGL translation
- **Audio**: Implement AX → OpenAL translation
- **Testing**: Report bugs and compatibility issues

## License

This project is for educational and preservation purposes. You must own a legal copy of Wii Sports to use this software.

## Credits

- Original decompilation: doldecomp team
- PC port: Community contributors
- This is a derivative work - Nintendo owns all rights to Wii Sports

---

**Questions?** Check the [FAQ](docs/faq.md) or open an issue on GitHub.
