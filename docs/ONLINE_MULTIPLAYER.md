# Online Multiplayer with WiiOTN

This document explains how to use **WiiOTN** (Wii Offline to Online) to play Wii Sports online with friends.

## What is WiiOTN?

[WiiOTN](https://github.com/Identityofsine/wii-otn) is a solution for enabling online multiplayer in local-only Wii games. It allows you to:
- Stream your game to remote players
- Share controller inputs across the network
- Play local multiplayer games like Wii Sports with friends anywhere in the world

## Integration Status

WiiOTN is included as a Git submodule in `external/wii-otn/`. 

**Current Status:**
- ✅ WiiOTN included as submodule
- ✅ Documentation provided
- ⏭️ Full integration requires PC port completion

## How WiiOTN Works

### Architecture

```
┌─────────────────┐         Network          ┌─────────────────┐
│  Host (Server)  │◄──────────────────────────►│ Client (Player) │
│                 │                            │                 │
│ - Runs game     │   Controller Data          │ - Sends input   │
│ - Virtual PADs  │◄───────────────────────────│ - Receives A/V  │
│ - Streams A/V   ├───────────────────────────►│ - Displays game │
│                 │   Video/Audio Stream       │                 │
└─────────────────┘                            └─────────────────┘
```

### Components

1. **Server (Host's Machine)**
   - Runs the PC port of Wii Sports
   - Uses ViGEmBus to create virtual controllers
   - Streams game video/audio to clients
   - Receives controller input from network

2. **Client (Player's Machine)**
   - Electron app (cross-platform)
   - Captures local controller/keyboard input
   - Sends input to server
   - Displays streamed game

## Setup Guide

### Prerequisites

**Host (Server):**
- Windows (for ViGEmBus support)
- [ViGEmBus](https://github.com/nefarius/ViGEmBus) installed
- Visual Studio 2022
- CMake
- This PC port of Wii Sports

**Client:**
- Node.js installed
- Cross-platform (Windows/Linux/macOS)

### Building WiiOTN Server

```bash
cd external/wii-otn/demos/socket-server
mkdir build
cd build

# Generate project files
cmake ..

# Build
cmake --build .

# Copy required DLL (Windows)
copy ..\..\..\..\lib\ViGEmClient\ViGEmClient.dll Debug\

# Run
Debug\socket-server-demo.exe 0.0.0.0
```

### Building WiiOTN Client

```bash
cd external/wii-otn/wiiotn-client

# Install dependencies
npm install

# Run in development
npm start

# Or build for distribution
npm run package
# Built files will be in ./release/
```

## Usage

### Quick Start

1. **On Host Machine:**
   ```bash
   # Terminal 1: Start WiiOTN server
   cd external/wii-otn/demos/socket-server/build
   Debug\socket-server-demo.exe 0.0.0.0
   
   # Terminal 2: Run Wii Sports PC
   python build_from_rom.py --rom wii_sports.iso
   ./WiiSports_PC_Build/WiiSports
   ```

2. **On Client Machine(s):**
   ```bash
   cd external/wii-otn/wiiotn-client
   npm start
   
   # Connect to host's IP address in the app
   ```

3. **Share your screen** using Discord or another service while WiiOTN handles controller input

### Tested Games & Features

According to WiiOTN documentation:

**Wii Sports Status:**
- ✅ **Bowling** - Works flawlessly
- ✅ **Tennis** - Minor issues
- ⚠️ **Golf** - Lacks precision (WiiMote motion needed)
- ❌ **Boxing** - Unplayable (requires WiiMote motion)
- ⚠️ **Baseball** - Not tested

## Controller Support

### Currently Supported
- ✅ Keyboard + Mouse
- ✅ Xbox Controllers (buttons + joystick)
- ✅ Generic gamepads

### Planned Support
- ⏭️ WiiMote buttons
- ⏭️ WiiMote IR sensor
- ⏭️ WiiMote accelerometer

## Network Configuration

### Hosting
- Forward port (default varies, check WiiOTN server output)
- Clients connect to host's public IP
- Low-latency network recommended

### Streaming
WiiOTN is adding native streaming support. Currently:
- Use Discord screen share for video/audio
- WiiOTN handles controller input
- Stream quality affects input lag

**Tip:** Stream your desktop instead of just the game window for lower latency.

## Troubleshooting

### ViGEmClient.dll Missing
Copy `external/wii-otn/lib/ViGEmClient/ViGEmClient.dll` to the server executable directory.

### Controller Not Detected
- Ensure ViGEmBus is installed on Windows
- Check WiiOTN server console for connection logs
- Verify client is connected to correct IP

### High Input Lag
- Use wired connection when possible
- Reduce Discord stream quality
- Close bandwidth-heavy applications
- Check server CPU usage

### Client Connection Failed
- Verify server is running and accessible
- Check firewall settings
- Confirm port forwarding is configured
- Test with host's local IP first

## Integration with PC Port

### Current State
WiiOTN is ready to use but requires the PC port to be fully functional first.

### Future Integration
Once the PC port is complete:
1. WiiOTN server can be integrated into build
2. Virtual controller handling in PAD_pc.cpp
3. Network input routing
4. Optional: Built-in streaming (when WiiOTN adds it)

## References

- **WiiOTN Repository:** https://github.com/Identityofsine/wii-otn
- **Dolphin Emulator:** https://github.com/dolphin-emu/dolphin (reference for Wii emulation)
- **Wii Sports Info:** https://github.com/kiwi515/ogws_info (game structure)

## Contributing

WiiOTN is actively developed. Contributions welcome:
- Controller support improvements
- Streaming implementation
- Cross-platform compatibility
- Documentation

## License

WiiOTN has its own license. See `external/wii-otn/` for details.

---

For PC port status and build instructions, see:
- [QUICKSTART.md](../QUICKSTART.md)
- [USER_GUIDE.md](../USER_GUIDE.md)
- [docs/faq.md](./faq.md)
