# Example Build Output

This document shows what you can expect when running the ROM builder.

## Successful Build Example

```
$ python build_from_rom.py --rom wii_sports.iso

============================================================
Wii Sports ROM to PC Executable Builder
============================================================

▶ Validating ROM file...
  ROM path: /home/user/games/wii_sports.iso
  File size: 523.4 MB
  Calculating ROM hash (this may take a moment)...
  SHA-1: 8bb422971b88b5551a37de98db69557df7b46637
✓ ROM verified: US Revision 1

▶ Setting up build directories...
✓ Directories created

▶ Extracting ROM data...
  Copying ROM to build location...
✓ ROM available at: /home/user/Wii-Sported/orig/RSPE01_01/wii_sports.iso

▶ Checking build dependencies...
  cmake version 3.22.1
  Compiler found: g++
  Checking for SDL2...
✓ Build dependencies OK

▶ Configuring CMake build...
  Running: cmake -B /home/user/WiiSports_PC_Build/temp/build ...
✓ CMake configured successfully

▶ Building PC executable...
  This may take several minutes...
  Building with 8 parallel jobs...
  [ 10%] Building C object src/revolution/OS/CMakeFiles/...
  [ 20%] Building CXX object src/revolution/PAD/CMakeFiles/...
  [ 30%] Building CXX object src/revolution/WPAD/CMakeFiles/...
  [ 40%] Building CXX object src/gx_opengl/CMakeFiles/...
  [ 50%] Linking CXX static library librevolution.a
  [ 60%] Building CXX object CMakeFiles/WiiSportsPC.dir/src/main.cpp.o
  [ 70%] Building CXX object CMakeFiles/WiiSportsPC.dir/src/rom_loader.cpp.o
  [ 80%] Building CXX object CMakeFiles/WiiSportsPC.dir/src/pc_asset_loader.cpp.o
  [ 90%] Linking CXX executable bin/WiiSports
  [100%] Built target WiiSportsPC
✓ Build completed successfully

▶ Copying executable to output directory...
✓ Executable created: /home/user/WiiSports_PC_Build/WiiSports

▶ Cleaning up temporary files...
✓ Cleanup complete

============================================================
Build Complete!
============================================================

Your Wii Sports PC executable is ready!

Location: /home/user/WiiSports_PC_Build/WiiSports

To run the game:
  /home/user/WiiSports_PC_Build/WiiSports

Note: You may need to extract assets first using asset_extractor.py

```

## Build Statistics

- **Total time**: ~5-15 minutes (varies by system)
- **Disk space used**: ~500MB (temp files cleaned automatically)
- **Final executable size**: ~10-20MB
- **CPU usage**: 100% during compilation (uses all cores)

## Next Steps After Building

1. **Extract assets** (one-time operation):
   ```bash
   python asset_extractor.py --rom wii_sports.iso
   ```

2. **Run the game**:
   ```bash
   ./WiiSports_PC_Build/WiiSports
   ```

## Troubleshooting Build Issues

### If SDL2 is not found:

```
✗ Failed at step: Configure CMake
Error: SDL2 not found

Solution:
  - Windows: vcpkg install sdl2:x64-windows
  - Linux: sudo apt-get install libsdl2-dev
  - macOS: brew install sdl2
```

### If compiler is not found:

```
✗ Failed at step: Check dependencies
Error: No C++ compiler found

Solution:
  - Windows: Install Visual Studio or Build Tools
  - Linux: sudo apt-get install build-essential
  - macOS: xcode-select --install
```

### If CMake is too old:

```
CMake Error: CMake 3.15 or higher is required

Solution:
  - Download from: https://cmake.org/download/
  - Or use package manager to upgrade
```

## Advanced Build Options

### Debug Build

```bash
python build_from_rom.py --rom wii_sports.iso --debug
```

Output includes debug symbols for troubleshooting.

### Custom Output Directory

```bash
python build_from_rom.py --rom wii_sports.iso --output ~/MyGames/WiiSports
```

### Verbose Output

```bash
python build_from_rom.py --rom wii_sports.iso --verbose
```

Shows full compiler output and detailed progress.

### Keep Temporary Files

```bash
python build_from_rom.py --rom wii_sports.iso --keep-temp
```

Useful for debugging build issues.
