#include "rom_loader.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

using std::fopen;
using std::fclose;
using std::fseek;
using std::ftell;
using std::fread;
using std::fprintf;
using std::printf;
using std::malloc;
using std::free;
using std::calloc;
using std::memset;
using std::memcpy;
using std::strdup;

// Global ROM loader instance
ROMLoader g_romLoader = {0};

// Byte swap utilities (Wii is big-endian, PC is little-endian)
static uint32_t swap32(uint32_t val) {
    return ((val >> 24) & 0xFF) |
           ((val >> 8) & 0xFF00) |
           ((val << 8) & 0xFF0000) |
           ((val << 24) & 0xFF000000);
}

bool ROMLoader_Init(void) {
    memset(&g_romLoader, 0, sizeof(ROMLoader));
    g_romLoader.isLoaded = false;
    return true;
}

bool ROMLoader_Load(const char* romPath) {
    if (!romPath) {
        fprintf(stderr, "ROM path is NULL\n");
        return false;
    }

    // Open ROM file
    FILE* f = fopen(romPath, "rb");
    if (!f) {
        fprintf(stderr, "Failed to open ROM: %s\n", romPath);
        return false;
    }

    // Get file size
    fseek(f, 0, SEEK_END);
    g_romLoader.romSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Allocate memory for ROM
    g_romLoader.romData = (uint8_t*)malloc(g_romLoader.romSize);
    if (!g_romLoader.romData) {
        fprintf(stderr, "Failed to allocate memory for ROM\n");
        fclose(f);
        return false;
    }

    // Read entire ROM into memory
    size_t read = fread(g_romLoader.romData, 1, g_romLoader.romSize, f);
    fclose(f);

    if (read != g_romLoader.romSize) {
        fprintf(stderr, "Failed to read ROM completely\n");
        free(g_romLoader.romData);
        return false;
    }

    // Find and parse DOL (at offset 0x420 in ISO, or start of WBFS)
    // For Wii Sports, DOL is typically at 0x420 in ISO format
    uint32_t dolOffset = 0x420;
    
    // Read DOL header
    DOLHeader* dol = (DOLHeader*)(g_romLoader.romData + dolOffset);
    
    // Swap endianness for all header fields
    for (int i = 0; i < 7; i++) {
        g_romLoader.dolHeader.textOffset[i] = swap32(dol->textOffset[i]);
        g_romLoader.dolHeader.textAddress[i] = swap32(dol->textAddress[i]);
        g_romLoader.dolHeader.textSize[i] = swap32(dol->textSize[i]);
    }
    
    for (int i = 0; i < 11; i++) {
        g_romLoader.dolHeader.dataOffset[i] = swap32(dol->dataOffset[i]);
        g_romLoader.dolHeader.dataAddress[i] = swap32(dol->dataAddress[i]);
        g_romLoader.dolHeader.dataSize[i] = swap32(dol->dataSize[i]);
    }
    
    g_romLoader.dolHeader.bssAddress = swap32(dol->bssAddress);
    g_romLoader.dolHeader.bssSize = swap32(dol->bssSize);
    g_romLoader.dolHeader.entryPoint = swap32(dol->entryPoint);

    // Count sections
    g_romLoader.sectionCount = 0;
    for (int i = 0; i < 7; i++) {
        if (g_romLoader.dolHeader.textSize[i] > 0) {
            g_romLoader.sectionCount++;
        }
    }
    for (int i = 0; i < 11; i++) {
        if (g_romLoader.dolHeader.dataSize[i] > 0) {
            g_romLoader.sectionCount++;
        }
    }

    // Allocate sections array
    g_romLoader.sections = (ROMSection*)calloc(g_romLoader.sectionCount, sizeof(ROMSection));
    if (!g_romLoader.sections) {
        fprintf(stderr, "Failed to allocate sections array\n");
        free(g_romLoader.romData);
        return false;
    }

    // Fill sections array
    int sectionIdx = 0;
    
    // Text (code) sections
    for (int i = 0; i < 7; i++) {
        if (g_romLoader.dolHeader.textSize[i] > 0) {
            g_romLoader.sections[sectionIdx].address = g_romLoader.dolHeader.textAddress[i];
            g_romLoader.sections[sectionIdx].fileOffset = dolOffset + g_romLoader.dolHeader.textOffset[i];
            g_romLoader.sections[sectionIdx].size = g_romLoader.dolHeader.textSize[i];
            g_romLoader.sections[sectionIdx].isCode = true;
            g_romLoader.sections[sectionIdx].data = g_romLoader.romData + g_romLoader.sections[sectionIdx].fileOffset;
            sectionIdx++;
        }
    }
    
    // Data sections
    for (int i = 0; i < 11; i++) {
        if (g_romLoader.dolHeader.dataSize[i] > 0) {
            g_romLoader.sections[sectionIdx].address = g_romLoader.dolHeader.dataAddress[i];
            g_romLoader.sections[sectionIdx].fileOffset = dolOffset + g_romLoader.dolHeader.dataOffset[i];
            g_romLoader.sections[sectionIdx].size = g_romLoader.dolHeader.dataSize[i];
            g_romLoader.sections[sectionIdx].isCode = false;
            g_romLoader.sections[sectionIdx].data = g_romLoader.romData + g_romLoader.sections[sectionIdx].fileOffset;
            sectionIdx++;
        }
    }

    g_romLoader.romPath = strdup(romPath);
    g_romLoader.isLoaded = true;

    printf("ROM Loaded: %s\n", romPath);
    printf("  Entry Point: 0x%08X\n", g_romLoader.dolHeader.entryPoint);
    printf("  Sections: %d\n", g_romLoader.sectionCount);

    return true;
}

const ROMSection* ROMLoader_GetCodeSection(int index) {
    if (!g_romLoader.isLoaded) return NULL;
    
    int codeIdx = 0;
    for (int i = 0; i < g_romLoader.sectionCount; i++) {
        if (g_romLoader.sections[i].isCode) {
            if (codeIdx == index) {
                return &g_romLoader.sections[i];
            }
            codeIdx++;
        }
    }
    return NULL;
}

const ROMSection* ROMLoader_GetDataSection(int index) {
    if (!g_romLoader.isLoaded) return NULL;
    
    int dataIdx = 0;
    for (int i = 0; i < g_romLoader.sectionCount; i++) {
        if (!g_romLoader.sections[i].isCode) {
            if (dataIdx == index) {
                return &g_romLoader.sections[i];
            }
            dataIdx++;
        }
    }
    return NULL;
}

bool ROMLoader_ReadMemory(uint32_t address, void* buffer, size_t size) {
    if (!g_romLoader.isLoaded || !buffer) return false;
    
    // Find section containing this address
    for (int i = 0; i < g_romLoader.sectionCount; i++) {
        const ROMSection* section = &g_romLoader.sections[i];
        if (address >= section->address && 
            address + size <= section->address + section->size) {
            uint32_t offset = address - section->address;
            memcpy(buffer, section->data + offset, size);
            return true;
        }
    }
    
    return false;
}

bool ROMLoader_IsDecompiled(uint32_t address) {
    // TODO: Maintain a map of decompiled function addresses
    // For now, assume nothing is decompiled - all needs interpretation
    return false;
}

uint32_t ROMLoader_GetEntryPoint(void) {
    return g_romLoader.dolHeader.entryPoint;
}

void ROMLoader_Shutdown(void) {
    if (g_romLoader.romData) {
        free(g_romLoader.romData);
        g_romLoader.romData = NULL;
    }
    if (g_romLoader.sections) {
        free(g_romLoader.sections);
        g_romLoader.sections = NULL;
    }
    if (g_romLoader.romPath) {
        free(g_romLoader.romPath);
        g_romLoader.romPath = NULL;
    }
    g_romLoader.isLoaded = false;
}
