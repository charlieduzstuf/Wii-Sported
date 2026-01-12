/**
 * PC Asset Loader
 * 
 * Runtime asset loading system for PC builds.
 * Replaces DVD-based asset loading with file system access.
 * 
 * Assets are extracted from ROM once using asset_extractor.py,
 * then loaded from pc_assets/ directory at runtime.
 */

#ifndef PC_ASSET_LOADER_H
#define PC_ASSET_LOADER_H

#ifdef PLATFORM_PC

#include <string>
#include <map>
#include <vector>

namespace PCAssets {

/**
 * Initialize asset loader
 * @param assetDir Path to pc_assets directory (default: "pc_assets")
 * @return true if successful
 */
bool Init(const char* assetDir = "pc_assets");

/**
 * Shutdown asset loader and free cached assets
 */
void Shutdown();

/**
 * Check if asset exists
 * @param category Asset category (textures, models, sounds, data)
 * @param name Asset name
 * @return true if asset file exists
 */
bool AssetExists(const char* category, const char* name);

/**
 * Load raw asset data
 * @param category Asset category
 * @param name Asset name
 * @param outSize Pointer to receive data size
 * @return Pointer to loaded data (must be freed with FreeAssetData)
 */
void* LoadAssetData(const char* category, const char* name, size_t* outSize);

/**
 * Free asset data loaded with LoadAssetData
 * @param data Pointer to data
 */
void FreeAssetData(void* data);

/**
 * Load texture asset
 * @param name Texture name
 * @param outWidth Pointer to receive texture width
 * @param outHeight Pointer to receive texture height
 * @param outFormat Pointer to receive format (GL_RGBA, etc.)
 * @return Pointer to texture data (must be freed with FreeAssetData)
 */
void* LoadTexture(const char* name, int* outWidth, int* outHeight, int* outFormat);

/**
 * Load model asset
 * @param name Model name
 * @param outSize Pointer to receive data size
 * @return Pointer to model data (must be freed with FreeAssetData)
 */
void* LoadModel(const char* name, size_t* outSize);

/**
 * Load sound asset
 * @param name Sound name
 * @param outSize Pointer to receive data size
 * @param outFrequency Pointer to receive sample rate
 * @param outChannels Pointer to receive channel count
 * @return Pointer to sound data (must be freed with FreeAssetData)
 */
void* LoadSound(const char* name, size_t* outSize, int* outFrequency, int* outChannels);

/**
 * Load generic data asset
 * @param name Data file name
 * @param outSize Pointer to receive data size
 * @return Pointer to data (must be freed with FreeAssetData)
 */
void* LoadData(const char* name, size_t* outSize);

/**
 * Get asset directory path
 * @return Path to pc_assets directory
 */
const char* GetAssetDir();

} // namespace PCAssets

#endif // PLATFORM_PC

#endif // PC_ASSET_LOADER_H
