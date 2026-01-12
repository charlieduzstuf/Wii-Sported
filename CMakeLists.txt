cmake_minimum_required(VERSION 3.15)
project(WiiSportsPC VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Platform definition
add_definitions(-DPLATFORM_PC)

# Find packages - use pkg-config for SDL2 if Config not found
find_package(SDL2 QUIET)
if(NOT SDL2_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(SDL2 REQUIRED sdl2)
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
    ${SDL2_INCLUDE_DIRS}
)

# PC implementation sources
set(PC_SOURCES
    src/revolution/OS/OSTime.c
    src/revolution/PAD/Pad_pc.cpp
    src/revolution/WPAD/WPAD_pc.cpp
    src/revolution/KPAD/KPAD_pc.cpp
    src/revolution/VI/vi_pc.cpp
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
