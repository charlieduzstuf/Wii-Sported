cmake_minimum_required(VERSION 3.15)
project(WiiSportsPC VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Platform definition
add_definitions(-DPLATFORM_PC)

# ROM Path - Required for building
set(ROM_PATH "${CMAKE_SOURCE_DIR}/orig/RSPE01_01" CACHE PATH "Path to Wii Sports ROM directory")

# Include ROM extraction module
include(${CMAKE_SOURCE_DIR}/cmake/rom_extract.cmake)

# Extract ROM code (required at compile time for semi-complete decompilation)
if(EXISTS "${ROM_PATH}")
    extract_rom_code("${ROM_PATH}" "${CMAKE_BINARY_DIR}/rom_extracted")
else()
    message(WARNING "ROM not found at ${ROM_PATH}")
    message(WARNING "This is a semi-complete decompilation - ROM is required at compile time")
    message(WARNING "Please place your Wii Sports ROM in: ${ROM_PATH}/")
endif()

# Find SDL2 - vcpkg provides it via CONFIG mode
# Note: vcpkg should be set up via -DCMAKE_TOOLCHAIN_FILE when building
find_package(SDL2 CONFIG QUIET)

if(NOT SDL2_FOUND)
    # Try standard find_package
    find_package(SDL2 QUIET)
endif()

if(NOT SDL2_FOUND)
    # Fall back to pkg-config on Unix systems
    if(UNIX)
        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_search_module(SDL2 QUIET sdl2)
            if(SDL2_FOUND)
                message(STATUS "Found SDL2 via pkg-config")
            endif()
        endif()
    endif()
endif()

if(SDL2_FOUND)
    # Check if we have target-based SDL2 (vcpkg/modern CMake)
    if(TARGET SDL2::SDL2)
        set(SDL2_LIBRARIES SDL2::SDL2)
        if(WIN32 AND TARGET SDL2::SDL2main)
            list(APPEND SDL2_LIBRARIES SDL2::SDL2main)
        endif()
        set(SDL2_INCLUDE_DIRS "")
        message(STATUS "Found SDL2 via CONFIG mode (target SDL2::SDL2)")
    else()
        # pkg-config or old-style find module
        message(STATUS "Found SDL2 (non-target mode)")
    endif()
else()
    message(FATAL_ERROR "SDL2 not found. Please install SDL2:\n"
                        "  - Windows: vcpkg install sdl2\n"
                        "  - Linux: sudo apt-get install libsdl2-dev\n"
                        "  - macOS: brew install sdl2")
endif()

find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/MSL
    ${CMAKE_SOURCE_DIR}/include/revolution
    ${CMAKE_SOURCE_DIR}/include/nw4r
    ${CMAKE_SOURCE_DIR}/include/egg
    ${CMAKE_SOURCE_DIR}/include/Pack
    ${CMAKE_SOURCE_DIR}/src/gx_opengl
    ${SDL2_INCLUDE_DIRS}
)

# PC implementation sources
set(PC_SOURCES
    src/revolution/OS/OSTime.c
    src/revolution/PAD/Pad_pc.cpp
    src/revolution/WPAD/WPAD_pc.cpp
    src/revolution/KPAD/KPAD_pc.cpp
    src/revolution/VI/vi_pc.cpp
    src/revolution/MTX/mtx_pc.c
    src/gx_opengl/gx_state.cpp
    src/gx_opengl/gx_texture.cpp
    src/gx_opengl/gx_vertex.cpp
    src/gx_opengl/gx_matrix.cpp
    src/pc_asset_loader.cpp
    src/rom_loader.cpp
)

# Main sources
set(GAME_SOURCES
    src/main.cpp
)

# Create executable
add_executable(WiiSportsPC
    ${GAME_SOURCES}
    ${PC_SOURCES}
)

# Link libraries
target_link_libraries(WiiSportsPC
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

# Platform-specific
if(WIN32)
    target_link_libraries(WiiSportsPC winmm)
    if(MSVC)
        target_compile_options(WiiSportsPC PRIVATE /W3 /MP /EHsc)
        target_compile_definitions(WiiSportsPC PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
elseif(UNIX)
    target_link_libraries(WiiSportsPC dl pthread)
    target_compile_options(WiiSportsPC PRIVATE -Wall -Wno-unused-parameter)
endif()

set_target_properties(WiiSportsPC PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME "WiiSports"
)

message(STATUS "Wii Sports PC Port - Build Configuration")
message(STATUS "SDL2: ${SDL2_LIBRARIES}")
message(STATUS "OpenGL: ${OPENGL_LIBRARIES}")
