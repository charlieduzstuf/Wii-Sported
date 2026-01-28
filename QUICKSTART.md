# Quick Start Guide

## 🎮 Build Wii Sports for PC in 3 Steps

### Step 1: Install Prerequisites

**Windows:**
```powershell
# Install Visual Studio (Community is free)
# Download from: https://visualstudio.microsoft.com/

# Install vcpkg and SDL2
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg install sdl2:x64-windows
./vcpkg integrate install
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install build-essential cmake libsdl2-dev libgl1-mesa-dev python3
```

**macOS:**
```bash
brew install cmake sdl2 python3
```

---

### Step 2: Build from Your ROM

```bash
# Clone repository
git clone https://github.com/charlieduzstuf/Wii-Sported.git
cd Wii-Sported

# Build PC executable from ROM
python build_from_rom.py --rom /path/to/your/wii_sports.iso
```

⏱️ **Wait 5-15 minutes** while it builds...

---

### Step 3: Extract Assets & Play

```bash
# Extract game assets (one-time operation)
python asset_extractor.py --rom /path/to/your/wii_sports.iso

# Run the game!
./WiiSports_PC_Build/WiiSports     # Linux/Mac
./WiiSports_PC_Build/WiiSports.exe # Windows
```

---

## 🎯 That's It!

Your Wii Sports PC executable is ready to play!

---

## 📚 Need More Help?

- **Detailed instructions**: [USER_GUIDE.md](USER_GUIDE.md)
- **Common problems**: [docs/faq.md](docs/faq.md)
- **Developer info**: [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md)

---

## ⚠️ Important Notes

- ✅ You **must own** a legal copy of Wii Sports
- ✅ Use **US Revision 1** for best compatibility
- ❌ **Don't distribute** the built executable (legal reasons)
- ❌ **Don't share** your ROM (copyright violation)

---

## 🔧 Quick Commands

```bash
# View all options
python build_from_rom.py --help

# Build debug version
python build_from_rom.py --rom game.iso --debug

# Custom output directory
python build_from_rom.py --rom game.iso --output ./MyBuild

# Verbose output (for troubleshooting)
python build_from_rom.py --rom game.iso --verbose
```

---

**Made with ❤️ by the community** | [Report issues](https://github.com/charlieduzstuf/Wii-Sported/issues)
