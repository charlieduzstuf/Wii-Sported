#ifndef ROM_LOADER_H
#define ROM_LOADER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// DOL (Dolphin Executable) header structure
typedef struct {
    uint32_t textOffset[7];
    uint32_t dataOffset[11];
    uint32_t textAddress[7];
    uint32_t dataAddress[11];
    uint32_t textSize[7];
    uint32_t dataSize[11];
    uint32_t bssAddress;
    uint32_t bssSize;
    uint32_t entryPoint;
} DOLHeader;

// ROM Section information
typedef struct {
    uint32_t address;     // Virtual address in game memory
    uint32_t fileOffset;  // Offset in ROM file
    uint32_t size;        // Section size
    uint8_t* data;        // Loaded data (NULL if not loaded)
    bool isCode;          // true for code, false for data
} ROMSection;

// ROM Loader state
typedef struct {
    char* romPath;
    DOLHeader dolHeader;
    ROMSection* sections;
    int sectionCount;
    uint8_t* romData;     // Full ROM data in memory
    size_t romSize;
    bool isLoaded;
} ROMLoader;

// Global ROM loader instance
extern ROMLoader g_romLoader;

// Initialize ROM loader
bool ROMLoader_Init(void);

// Load ROM from file
bool ROMLoader_Load(const char* romPath);

// Get code section by index
const ROMSection* ROMLoader_GetCodeSection(int index);

// Get data section by index  
const ROMSection* ROMLoader_GetDataSection(int index);

// Read memory from ROM address space
bool ROMLoader_ReadMemory(uint32_t address, void* buffer, size_t size);

// Check if address is in decompiled code
bool ROMLoader_IsDecompiled(uint32_t address);

// Get entry point address
uint32_t ROMLoader_GetEntryPoint(void);

// Cleanup
void ROMLoader_Shutdown(void);

#ifdef __cplusplus
}
#endif

#endif // ROM_LOADER_H
