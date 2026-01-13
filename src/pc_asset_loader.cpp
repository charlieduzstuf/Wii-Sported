/**
 * PC Asset Loader Implementation
 */

#ifdef PLATFORM_PC

#include "pc_asset_loader.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>

using std::fopen;
using std::fclose;
using std::fseek;
using std::ftell;
using std::fread;
using std::malloc;
using std::free;
using std::memcpy;

namespace PCAssets {

static std::string s_assetDir = "pc_assets";
static bool s_initialized = false;

bool Init(const char* assetDir) {
    if (s_initialized) {
        return true;
    }
    
    s_assetDir = assetDir ? assetDir : "pc_assets";
    
    // Check if asset directory exists
    struct stat st;
    if (stat(s_assetDir.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Error: Asset directory not found: %s\n", s_assetDir.c_str());
        fprintf(stderr, "Please run asset_extractor.py first to extract assets from your ROM.\n");
        return false;
    }
    
    printf("PC Asset Loader initialized: %s\n", s_assetDir.c_str());
    s_initialized = true;
    return true;
}

void Shutdown() {
    s_initialized = false;
}

static std::string GetAssetPath(const char* category, const char* name) {
    return s_assetDir + "/" + category + "/" + name;
}

bool AssetExists(const char* category, const char* name) {
    std::string path = GetAssetPath(category, name);
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

void* LoadAssetData(const char* category, const char* name, size_t* outSize) {
    std::string path = GetAssetPath(category, name);
    
    // Open file
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) {
        fprintf(stderr, "Failed to open asset: %s\n", path.c_str());
        if (outSize) *outSize = 0;
        return nullptr;
    }
    
    // Get file size
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Allocate buffer
    void* data = malloc(size);
    if (!data) {
        fprintf(stderr, "Failed to allocate memory for asset: %s\n", path.c_str());
        fclose(f);
        if (outSize) *outSize = 0;
        return nullptr;
    }
    
    // Read file
    size_t read = fread(data, 1, size, f);
    fclose(f);
    
    if (read != size) {
        fprintf(stderr, "Failed to read asset: %s\n", path.c_str());
        free(data);
        if (outSize) *outSize = 0;
        return nullptr;
    }
    
    if (outSize) {
        *outSize = size;
    }
    
    return data;
}

void FreeAssetData(void* data) {
    if (data) {
        free(data);
    }
}

void* LoadTexture(const char* name, int* outWidth, int* outHeight, int* outFormat) {
    // Load texture file
    size_t size;
    void* data = LoadAssetData("textures", name, &size);
    
    if (!data) {
        return nullptr;
    }
    
    // TODO: Parse texture format (TPL, PNG, etc.) and convert to OpenGL format
    // For now, assume raw RGBA data
    // In full implementation, would use image loading library (stb_image, etc.)
    
    if (outWidth) *outWidth = 256;   // Placeholder
    if (outHeight) *outHeight = 256; // Placeholder
    if (outFormat) *outFormat = 0x1908; // GL_RGBA
    
    return data;
}

void* LoadModel(const char* name, size_t* outSize) {
    return LoadAssetData("models", name, outSize);
}

void* LoadSound(const char* name, size_t* outSize, int* outFrequency, int* outChannels) {
    // Load sound file
    size_t size;
    void* data = LoadAssetData("sounds", name, &size);
    
    if (!data) {
        return nullptr;
    }
    
    // TODO: Parse audio format and extract metadata
    // For now, assume default values
    
    if (outSize) *outSize = size;
    if (outFrequency) *outFrequency = 48000; // Placeholder
    if (outChannels) *outChannels = 2;       // Placeholder (stereo)
    
    return data;
}

void* LoadData(const char* name, size_t* outSize) {
    return LoadAssetData("data", name, outSize);
}

const char* GetAssetDir() {
    return s_assetDir.c_str();
}

} // namespace PCAssets

#endif // PLATFORM_PC
