# Frequently Asked Questions (FAQ)

## General Questions

### Q: What is this project?

This is a PC port of Wii Sports that uses a partial decompilation (~30% of the game code) combined with your ROM to create a native Windows/Linux/macOS executable.

### Q: Do I need to own Wii Sports?

**Yes!** You must own a legal copy of Wii Sports and dump your own ROM. This project does not include any Nintendo code or assets.

### Q: Can I download a pre-built executable?

**No.** For legal reasons, we cannot distribute executables that contain Nintendo's code. You must build it yourself using your own ROM.

### Q: Which ROM versions are supported?

Currently, only **Wii Sports US Revision 1** is fully tested and verified. Other versions may work but are not guaranteed.

### Q: Why is this only 30% decompiled?

Decompiling a game is a massive undertaking that takes years. The community is actively working on it. Despite being partial, the build system can create a working PC port by combining the decompiled code with the original ROM code.

## Build Process

### Q: How long does building take?

Typically 5-15 minutes depending on your computer's speed. The process includes:
- ROM validation and extraction
- CMake configuration
- C++ compilation
- Linking

### Q: What's the difference between this and an emulator?

An emulator runs the original Wii code unchanged. This PC port:
- Runs natively on x86-64 (no emulation overhead for decompiled parts)
- Uses SDL2 and OpenGL instead of Wii APIs
- Can eventually run entirely natively (as more code is decompiled)

### Q: Can I run this without the ROM after building?

**Partially.** The executable needs the ROM at runtime to load:
- Binary code that hasn't been decompiled yet
- Game assets (textures, models, sounds)

Eventually, as more code is decompiled and assets are converted, ROM dependency will decrease.

### Q: Does this work on all platforms?

The build system supports:
- ✅ Windows (x64) - tested
- ✅ Linux (x64) - tested
- ✅ macOS (x64 and ARM) - should work, less tested

### Q: What if my ROM hash doesn't match?

The builder will warn you but allow you to continue. The build may succeed, but the game may not work correctly if the ROM layout differs from the expected version.

## Technical Questions

### Q: How does the hybrid build work?

1. The decompiled C++ code (~30%) is compiled to native x86-64
2. The ROM is placed in a known location for the build system
3. The build system creates a ROM loader that can:
   - Extract and cache binary code from the ROM
   - Load assets from the ROM
4. At runtime, decompiled code calls into ROM code as needed

### Q: What's the performance like?

- **Decompiled code**: Full native speed
- **ROM code**: Runs via interpretation (slower, but improving)
- **Overall**: Playable, but some slowdown in non-decompiled sections

As more code is decompiled, performance improves.

### Q: Can I help with decompilation?

Yes! The project welcomes contributors. See [CONTRIBUTING.md](contributing.md) for details.

## Troubleshooting

### Q: Build fails with "SDL2 not found"

Install SDL2 development libraries:
- **Windows**: `vcpkg install sdl2:x64-windows`
- **Linux**: `sudo apt-get install libsdl2-dev`
- **macOS**: `brew install sdl2`

### Q: Build fails with compiler errors

Make sure you have:
- **Windows**: Visual Studio 2019+ or Build Tools
- **Linux**: `build-essential` package
- **macOS**: Xcode Command Line Tools

### Q: Game crashes on startup

1. Ensure you extracted assets: `python asset_extractor.py --rom <rom>`
2. Check that you're using a supported ROM version
3. Try building in debug mode: `python build_from_rom.py --rom <rom> --debug`

### Q: Game is very slow

This is expected for ROM code sections that aren't decompiled yet. Performance will improve as more code is decompiled. You can help by contributing to the decompilation effort!

### Q: Can I distribute my built executable?

**No.** The executable contains Nintendo's copyrighted code from the ROM. Distribution would be illegal. Each user must build their own using their legally obtained ROM.

## Legal Questions

### Q: Is this legal?

This project is for educational and preservation purposes. Key points:
- ✅ Reverse engineering for interoperability is generally legal
- ✅ You must own the game to use this
- ✅ No Nintendo code is distributed
- ❌ Don't distribute built executables
- ❌ Don't distribute ROMs

Consult a lawyer if you have specific concerns.

### Q: Will Nintendo shut this down?

We follow clean-room practices and don't distribute any Nintendo code. Similar projects (like the Zelda and Mario 64 decomps) have coexisted with Nintendo. However, there are never guarantees.

### Q: Can I make a commercial game using this?

**Absolutely not.** This contains Nintendo's intellectual property. It's for personal, educational, and preservation use only.

## Future Plans

### Q: What features are coming?

Planned improvements:
- More decompiled code (ongoing)
- Better GX → OpenGL translation
- Full audio system (AX → OpenAL)
- Multiplayer support
- Mod support
- Better performance

### Q: Can I add mods/cheats?

Not yet, but mod support is planned. As more code is decompiled, it becomes easier to modify game behavior.

### Q: Will this support other Wii Sports games?

Possibly! The same techniques could work for:
- Wii Sports Resort
- Wii Sports Club
- Other Wii games

But each would need its own decompilation effort.

---

**Still have questions?** Open an issue on GitHub or check the [User Guide](../USER_GUIDE.md).
