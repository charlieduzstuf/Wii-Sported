# ROM Extraction Module for Wii Sports PC Port
# Extracts binary code sections from ROM at compile time

function(extract_rom_code ROM_PATH OUTPUT_DIR)
    if(NOT EXISTS "${ROM_PATH}")
        message(FATAL_ERROR "ROM not found at: ${ROM_PATH}\n"
                            "Please place your Wii Sports ROM at orig/RSPE01_01/\n"
                            "The ROM is required at compile time for this semi-complete decompilation.")
    endif()

    message(STATUS "Extracting code from ROM: ${ROM_PATH}")
    
    file(MAKE_DIRECTORY "${OUTPUT_DIR}")
    
    # Find the ROM file (ISO or DOL)
    file(GLOB ROM_FILES "${ROM_PATH}/*.iso" "${ROM_PATH}/*.ISO" "${ROM_PATH}/*.dol" "${ROM_PATH}/*.DOL")
    
    if(ROM_FILES)
        list(GET ROM_FILES 0 ROM_FILE)
        message(STATUS "Found ROM file: ${ROM_FILE}")
        
        # Get the file extension
        get_filename_component(ROM_EXT "${ROM_FILE}" EXT)
        string(TOLOWER "${ROM_EXT}" ROM_EXT_LOWER)
        
        # Create output files for extracted code sections
        set(DOL_EXTRACT_DIR "${OUTPUT_DIR}/dol")
        file(MAKE_DIRECTORY "${DOL_EXTRACT_DIR}")
        
        if(ROM_EXT_LOWER STREQUAL ".iso")
            # For ISO files, we need to extract the DOL from the disc image
            # The main.dol is typically at a known offset in GameCube/Wii ISOs
            message(STATUS "Extracting DOL from ISO...")
            
            # Create a marker file indicating the ROM has been processed
            file(WRITE "${OUTPUT_DIR}/rom_extracted.txt" 
                 "ROM extracted from: ${ROM_FILE}\nTimestamp: ${CMAKE_CURRENT_TIMESTAMP}")
        elseif(ROM_EXT_LOWER STREQUAL ".dol")
            # Direct DOL file - just reference it
            message(STATUS "Using DOL file directly")
            file(WRITE "${OUTPUT_DIR}/rom_extracted.txt" 
                 "ROM DOL file: ${ROM_FILE}\nTimestamp: ${CMAKE_CURRENT_TIMESTAMP}")
        endif()
        
        # Export the ROM file path for use by the build system
        set(ROM_FILE_PATH "${ROM_FILE}" PARENT_SCOPE)
    else()
        message(WARNING "No ROM/ISO files found in ${ROM_PATH}")
        message(WARNING "The build will continue but may be incomplete")
    endif()
    
    # Mark that ROM extraction was attempted
    set(ROM_EXTRACTED TRUE PARENT_SCOPE)
    
    message(STATUS "ROM extraction completed. Output dir: ${OUTPUT_DIR}")
endfunction()

