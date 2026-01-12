#ifndef MSL_FILE_IO_H
#define MSL_FILE_IO_H

#ifdef PLATFORM_PC
// PC stub for MSL file_io.h
// On PC, we use standard C library FILE operations
#include <stdio.h>

#else
// Wii implementation would go here
#error "Wii file_io.h not implemented yet"

#endif // PLATFORM_PC

#endif // MSL_FILE_IO_H
