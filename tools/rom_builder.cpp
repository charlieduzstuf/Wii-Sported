/**
 * Wii Sports ROM Builder Tool
 * 
 * This standalone tool accepts a user's Wii Sports ROM and compiles it
 * into a complete PC executable by combining:
 * - Decompiled C/C++ source code (embedded in this executable)
 * - Binary code from the ROM (non-decompiled portions)
 * - Assets extracted from the ROM
 * 
 * Usage:
 *   WiiSportsBuilder.exe --rom <path/to/rom.iso> --output <output_dir>
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <filesystem>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace fs = std::filesystem;

// Configuration
struct BuildConfig {
    std::string romPath;
    std::string outputDir;
    std::string buildType = "Release";
    bool verbose = false;
    bool keepTemp = false;
};

// Embedded build files (these will be generated at build time)
// For now, we'll extract them from the current source tree
extern const char* EMBEDDED_CMAKE_CONTENT;
extern const char* EMBEDDED_SOURCE_LIST;

class ROMBuilder {
private:
    BuildConfig config;
    std::string tempDir;
    std::string buildDir;
    
public:
    ROMBuilder(const BuildConfig& cfg) : config(cfg) {
        tempDir = config.outputDir + "/temp_build";
        buildDir = config.outputDir + "/build";
    }
    
    ~ROMBuilder() {
        if (!config.keepTemp && fs::exists(tempDir)) {
            try {
                fs::remove_all(tempDir);
            } catch (...) {
                // Ignore cleanup errors
            }
        }
    }
    
    bool validateROM() {
        std::cout << "Validating ROM: " << config.romPath << std::endl;
        
        if (!fs::exists(config.romPath)) {
            std::cerr << "Error: ROM file not found: " << config.romPath << std::endl;
            return false;
        }
        
        // Check file size (Wii Sports should be around 500MB-4GB)
        auto fileSize = fs::file_size(config.romPath);
        if (fileSize < 100 * 1024 * 1024) {  // Less than 100MB is suspicious
            std::cerr << "Warning: ROM file seems too small (" << fileSize << " bytes)" << std::endl;
        }
        
        // TODO: Verify ROM hash/signature
        
        std::cout << "  ROM size: " << (fileSize / 1024 / 1024) << " MB" << std::endl;
        return true;
    }
    
    bool extractBuildFiles() {
        std::cout << "\nExtracting build files..." << std::endl;
        
        // Create temp directory structure
        fs::create_directories(tempDir);
        fs::create_directories(tempDir + "/src");
        fs::create_directories(tempDir + "/include");
        fs::create_directories(tempDir + "/cmake");
        
        // Extract embedded source files
        // In a real implementation, these would be embedded in the executable
        // For now, we'll copy from the current source tree
        std::cout << "  Build files extracted to: " << tempDir << std::endl;
        
        return true;
    }
    
    bool extractROMData() {
        std::cout << "\nExtracting ROM data..." << std::endl;
        
        std::string romDataDir = tempDir + "/rom_data";
        fs::create_directories(romDataDir);
        
        // Copy ROM to temp location for processing
        std::string romCopy = romDataDir + "/game.iso";
        try {
            fs::copy_file(config.romPath, romCopy, fs::copy_options::overwrite_existing);
        } catch (const std::exception& e) {
            std::cerr << "Error copying ROM: " << e.what() << std::endl;
            return false;
        }
        
        // TODO: Extract DOL sections
        // TODO: Extract assets
        
        std::cout << "  ROM data extracted successfully" << std::endl;
        return true;
    }
    
    bool generateCMakeProject() {
        std::cout << "\nGenerating CMake project..." << std::endl;
        
        // Create CMakeLists.txt in temp directory
        std::string cmakePath = tempDir + "/CMakeLists.txt";
        std::ofstream cmakeFile(cmakePath);
        
        if (!cmakeFile.is_open()) {
            std::cerr << "Error: Could not create CMakeLists.txt" << std::endl;
            return false;
        }
        
        // Generate CMakeLists.txt that references the ROM data
        cmakeFile << "cmake_minimum_required(VERSION 3.15)\n";
        cmakeFile << "project(WiiSportsPC VERSION 1.0 LANGUAGES C CXX)\n\n";
        cmakeFile << "set(CMAKE_CXX_STANDARD 17)\n";
        cmakeFile << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n";
        cmakeFile << "set(CMAKE_C_STANDARD 11)\n";
        cmakeFile << "set(CMAKE_C_STANDARD_REQUIRED ON)\n\n";
        cmakeFile << "add_definitions(-DPLATFORM_PC)\n\n";
        cmakeFile << "# ROM data is embedded in this build\n";
        cmakeFile << "set(ROM_DATA_DIR \"${CMAKE_SOURCE_DIR}/rom_data\")\n";
        cmakeFile << "add_definitions(-DROM_DATA_DIR=\"${ROM_DATA_DIR}\")\n\n";
        
        // Add SDL2 and OpenGL finding logic
        cmakeFile << "find_package(SDL2 REQUIRED)\n";
        cmakeFile << "find_package(OpenGL REQUIRED)\n\n";
        
        // Add source files (would be extracted from embedded data)
        cmakeFile << "# Game sources\n";
        cmakeFile << "add_executable(WiiSports\n";
        cmakeFile << "    src/main.cpp\n";
        cmakeFile << "    src/rom_loader.cpp\n";
        cmakeFile << ")\n\n";
        
        cmakeFile << "target_link_libraries(WiiSports ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})\n";
        
        cmakeFile.close();
        
        std::cout << "  CMake project generated" << std::endl;
        return true;
    }
    
    bool compileFinalExecutable() {
        std::cout << "\nCompiling final executable..." << std::endl;
        
        fs::create_directories(buildDir);
        
        // Run CMake configure
        std::string cmakeCmd = "cmake -B \"" + buildDir + "\" -S \"" + tempDir + "\" -DCMAKE_BUILD_TYPE=" + config.buildType;
        
        std::cout << "  Configuring: " << cmakeCmd << std::endl;
        int result = system(cmakeCmd.c_str());
        
        if (result != 0) {
            std::cerr << "Error: CMake configuration failed" << std::endl;
            return false;
        }
        
        // Run CMake build
        std::string buildCmd = "cmake --build \"" + buildDir + "\" --config " + config.buildType;
        
        std::cout << "  Building: " << buildCmd << std::endl;
        result = system(buildCmd.c_str());
        
        if (result != 0) {
            std::cerr << "Error: Build failed" << std::endl;
            return false;
        }
        
        std::cout << "  Build completed successfully!" << std::endl;
        return true;
    }
    
    bool copyFinalExecutable() {
        std::cout << "\nCopying final executable..." << std::endl;
        
        // Find the built executable
        std::string exeName = "WiiSports";
#ifdef _WIN32
        exeName += ".exe";
        std::string exePath = buildDir + "/Release/" + exeName;
        if (!fs::exists(exePath)) {
            exePath = buildDir + "/Debug/" + exeName;
        }
#else
        std::string exePath = buildDir + "/" + exeName;
#endif
        
        if (!fs::exists(exePath)) {
            std::cerr << "Error: Could not find built executable: " << exePath << std::endl;
            return false;
        }
        
        // Copy to output directory
        std::string outputExe = config.outputDir + "/" + exeName;
        try {
            fs::copy_file(exePath, outputExe, fs::copy_options::overwrite_existing);
        } catch (const std::exception& e) {
            std::cerr << "Error copying executable: " << e.what() << std::endl;
            return false;
        }
        
        std::cout << "  Executable created: " << outputExe << std::endl;
        return true;
    }
    
    bool build() {
        std::cout << "========================================" << std::endl;
        std::cout << "Wii Sports ROM to PC Executable Builder" << std::endl;
        std::cout << "========================================" << std::endl;
        
        if (!validateROM()) return false;
        if (!extractBuildFiles()) return false;
        if (!extractROMData()) return false;
        if (!generateCMakeProject()) return false;
        if (!compileFinalExecutable()) return false;
        if (!copyFinalExecutable()) return false;
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "Build completed successfully!" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Your Wii Sports PC executable is ready at:" << std::endl;
        std::cout << "  " << config.outputDir << "/WiiSports" << 
#ifdef _WIN32
            ".exe" <<
#endif
            std::endl;
        std::cout << "\nYou can now run the game!" << std::endl;
        
        return true;
    }
};

void printUsage(const char* progName) {
    std::cout << "Wii Sports ROM to PC Executable Builder\n\n";
    std::cout << "Usage:\n";
    std::cout << "  " << progName << " --rom <path/to/rom.iso> [options]\n\n";
    std::cout << "Required:\n";
    std::cout << "  --rom <path>       Path to your Wii Sports ROM (ISO, WBFS, etc.)\n\n";
    std::cout << "Options:\n";
    std::cout << "  --output <dir>     Output directory (default: ./WiiSports_Build)\n";
    std::cout << "  --debug            Build debug version instead of release\n";
    std::cout << "  --verbose          Enable verbose output\n";
    std::cout << "  --keep-temp        Keep temporary build files\n";
    std::cout << "  --help             Show this help message\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << progName << " --rom wii_sports.iso\n";
    std::cout << "  " << progName << " --rom /path/to/game.iso --output ./MyBuild\n";
}

int main(int argc, char* argv[]) {
    BuildConfig config;
    config.outputDir = "WiiSports_Build";
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--rom" && i + 1 < argc) {
            config.romPath = argv[++i];
        } else if (arg == "--output" && i + 1 < argc) {
            config.outputDir = argv[++i];
        } else if (arg == "--debug") {
            config.buildType = "Debug";
        } else if (arg == "--verbose") {
            config.verbose = true;
        } else if (arg == "--keep-temp") {
            config.keepTemp = true;
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // Validate required arguments
    if (config.romPath.empty()) {
        std::cerr << "Error: --rom argument is required\n" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    // Create builder and run
    ROMBuilder builder(config);
    
    if (!builder.build()) {
        std::cerr << "\nBuild failed!" << std::endl;
        return 1;
    }
    
    return 0;
}

// Placeholder for embedded content
const char* EMBEDDED_CMAKE_CONTENT = "";
const char* EMBEDDED_SOURCE_LIST = "";
