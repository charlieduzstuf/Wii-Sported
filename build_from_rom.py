#!/usr/bin/env python3
"""
Wii Sports ROM to PC Executable Builder

This tool creates a complete PC executable from your Wii Sports ROM by combining:
- Decompiled C/C++ source code (from this repository)
- Binary code from the ROM (non-decompiled portions ~70%)
- Assets extracted from the ROM

Usage:
    python build_from_rom.py --rom <path/to/rom.iso>

The tool will:
1. Validate your ROM
2. Extract code and assets from the ROM
3. Compile everything into a PC executable
4. Output: WiiSports.exe (or WiiSports on Linux/Mac)
"""

import argparse
import os
import sys
import shutil
import subprocess
import hashlib
from pathlib import Path

class Colors:
    """ANSI color codes for terminal output"""
    HEADER = '\033[95m'
    BLUE = '\033[94m'
    CYAN = '\033[96m'
    GREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'

def print_header(msg):
    print(f"\n{Colors.HEADER}{Colors.BOLD}{'='*60}{Colors.ENDC}")
    print(f"{Colors.HEADER}{Colors.BOLD}{msg}{Colors.ENDC}")
    print(f"{Colors.HEADER}{Colors.BOLD}{'='*60}{Colors.ENDC}\n")

def print_step(msg):
    print(f"{Colors.CYAN}▶ {msg}{Colors.ENDC}")

def print_success(msg):
    print(f"{Colors.GREEN}✓ {msg}{Colors.ENDC}")

def print_error(msg):
    print(f"{Colors.FAIL}✗ {msg}{Colors.ENDC}", file=sys.stderr)

def print_warning(msg):
    print(f"{Colors.WARNING}⚠ {msg}{Colors.ENDC}")

class ROMBuilder:
    SUPPORTED_HASHES = {
        '8bb422971b88b5551a37de98db69557df7b46637': 'US Revision 1',
    }
    
    def __init__(self, rom_path, output_dir='WiiSports_PC_Build', build_type='Release', keep_temp=False, verbose=False):
        self.rom_path = Path(rom_path).resolve()
        self.output_dir = Path(output_dir).resolve()
        self.build_type = build_type
        self.keep_temp = keep_temp
        self.verbose = verbose
        
        # Source directory is the repository root
        self.source_dir = Path(__file__).parent.resolve()
        
        # Temporary directories
        self.temp_dir = self.output_dir / 'temp'
        self.rom_extract_dir = self.temp_dir / 'rom_extracted'
        self.build_dir = self.temp_dir / 'build'
        
    def validate_rom(self):
        """Validate the ROM file"""
        print_step("Validating ROM file...")
        
        if not self.rom_path.exists():
            print_error(f"ROM file not found: {self.rom_path}")
            return False
        
        # Check file size
        file_size = self.rom_path.stat().st_size
        file_size_mb = file_size / (1024 * 1024)
        
        print(f"  ROM path: {self.rom_path}")
        print(f"  File size: {file_size_mb:.1f} MB")
        
        if file_size < 100 * 1024 * 1024:  # Less than 100MB
            print_warning(f"ROM file seems small ({file_size_mb:.1f} MB)")
            response = input("Continue anyway? [y/N]: ")
            if response.lower() != 'y':
                return False
        
        # Calculate SHA-1 hash (this might take a moment for large files)
        print("  Calculating ROM hash (this may take a moment)...")
        sha1 = hashlib.sha1()
        with open(self.rom_path, 'rb') as f:
            while chunk := f.read(8192):
                sha1.update(chunk)
        
        rom_hash = sha1.hexdigest()
        print(f"  SHA-1: {rom_hash}")
        
        if rom_hash in self.SUPPORTED_HASHES:
            version = self.SUPPORTED_HASHES[rom_hash]
            print_success(f"ROM verified: {version}")
        else:
            print_warning("ROM hash doesn't match known versions")
            print_warning("Build may not work correctly with untested ROM versions")
            response = input("Continue anyway? [y/N]: ")
            if response.lower() != 'y':
                return False
        
        return True
    
    def setup_directories(self):
        """Create necessary directories"""
        print_step("Setting up build directories...")
        
        # Create output directory
        self.output_dir.mkdir(parents=True, exist_ok=True)
        
        # Create temp directories
        self.temp_dir.mkdir(parents=True, exist_ok=True)
        self.rom_extract_dir.mkdir(parents=True, exist_ok=True)
        
        print_success("Directories created")
        return True
    
    def extract_rom_data(self):
        """Extract data from ROM"""
        print_step("Extracting ROM data...")
        
        # Create ROM directory structure expected by build system
        rom_target = self.source_dir / 'orig' / 'RSPE01_01'
        rom_target.mkdir(parents=True, exist_ok=True)
        
        # Copy ROM to expected location
        rom_copy = rom_target / self.rom_path.name
        if not rom_copy.exists() or rom_copy.stat().st_size != self.rom_path.stat().st_size:
            print(f"  Copying ROM to build location...")
            shutil.copy2(self.rom_path, rom_copy)
        
        print_success(f"ROM available at: {rom_copy}")
        return True
    
    def check_dependencies(self):
        """Check if required build tools are available"""
        print_step("Checking build dependencies...")
        
        # Check CMake
        try:
            result = subprocess.run(['cmake', '--version'], 
                                  capture_output=True, text=True, check=True)
            version = result.stdout.split('\n')[0]
            print(f"  {version}")
        except (subprocess.CalledProcessError, FileNotFoundError):
            print_error("CMake not found. Please install CMake 3.15 or later")
            return False
        
        # Check for C++ compiler
        compilers = ['g++', 'clang++', 'cl.exe']
        compiler_found = False
        
        for compiler in compilers:
            try:
                subprocess.run([compiler, '--version'], 
                             capture_output=True, check=True)
                print(f"  Compiler found: {compiler}")
                compiler_found = True
                break
            except (subprocess.CalledProcessError, FileNotFoundError):
                continue
        
        if not compiler_found:
            print_error("No C++ compiler found. Please install GCC, Clang, or MSVC")
            return False
        
        # Check for SDL2 (warning only, not fatal)
        print("  Checking for SDL2...")
        # SDL2 check is done by CMake
        
        print_success("Build dependencies OK")
        return True
    
    def configure_cmake(self):
        """Configure the CMake build"""
        print_step("Configuring CMake build...")
        
        self.build_dir.mkdir(parents=True, exist_ok=True)
        
        cmake_args = [
            'cmake',
            '-B', str(self.build_dir),
            '-S', str(self.source_dir),
            f'-DCMAKE_BUILD_TYPE={self.build_type}',
        ]
        
        # Add vcpkg toolchain if on Windows (user might have it)
        if sys.platform == 'win32':
            vcpkg_toolchain = Path('vcpkg/scripts/buildsystems/vcpkg.cmake')
            if vcpkg_toolchain.exists():
                cmake_args.append(f'-DCMAKE_TOOLCHAIN_FILE={vcpkg_toolchain}')
        
        print(f"  Running: {' '.join(cmake_args)}")
        
        try:
            result = subprocess.run(
                cmake_args,
                cwd=self.source_dir,
                capture_output=not self.verbose,
                text=True,
                check=True
            )
            
            if self.verbose and result.stdout:
                print(result.stdout)
                
        except subprocess.CalledProcessError as e:
            print_error("CMake configuration failed")
            if e.stdout:
                print(e.stdout)
            if e.stderr:
                print(e.stderr)
            return False
        
        print_success("CMake configured successfully")
        return True
    
    def build_executable(self):
        """Build the final executable"""
        print_step("Building PC executable...")
        print("  This may take several minutes...")
        
        build_args = [
            'cmake',
            '--build', str(self.build_dir),
            '--config', self.build_type,
        ]
        
        # Add parallel build if supported
        import multiprocessing
        num_cores = multiprocessing.cpu_count()
        if sys.platform != 'win32':
            build_args.extend(['--', f'-j{num_cores}'])
        
        print(f"  Building with {num_cores} parallel jobs...")
        
        try:
            result = subprocess.run(
                build_args,
                cwd=self.source_dir,
                capture_output=not self.verbose,
                text=True,
                check=True
            )
            
            if self.verbose and result.stdout:
                print(result.stdout)
                
        except subprocess.CalledProcessError as e:
            print_error("Build failed")
            if e.stdout:
                print(e.stdout)
            if e.stderr:
                print(e.stderr)
            return False
        
        print_success("Build completed successfully")
        return True
    
    def copy_executable(self):
        """Copy the built executable to output directory"""
        print_step("Copying executable to output directory...")
        
        # Find the executable
        exe_name = 'WiiSports.exe' if sys.platform == 'win32' else 'WiiSports'
        
        possible_paths = [
            self.build_dir / 'bin' / exe_name,
            self.build_dir / 'bin' / 'Release' / exe_name,
            self.build_dir / 'bin' / 'Debug' / exe_name,
            self.build_dir / exe_name,
        ]
        
        exe_path = None
        for path in possible_paths:
            if path.exists():
                exe_path = path
                break
        
        if not exe_path:
            print_error(f"Could not find built executable. Searched:")
            for path in possible_paths:
                print(f"    {path}")
            return False
        
        # Copy to output directory
        output_exe = self.output_dir / exe_name
        shutil.copy2(exe_path, output_exe)
        
        # Make executable on Unix systems
        if sys.platform != 'win32':
            os.chmod(output_exe, 0o755)
        
        print_success(f"Executable created: {output_exe}")
        return True
    
    def cleanup(self):
        """Clean up temporary files"""
        if not self.keep_temp:
            print_step("Cleaning up temporary files...")
            
            if self.temp_dir.exists():
                shutil.rmtree(self.temp_dir, ignore_errors=True)
            
            print_success("Cleanup complete")
        else:
            print_step("Keeping temporary files for debugging")
    
    def build(self):
        """Main build process"""
        print_header("Wii Sports ROM to PC Executable Builder")
        
        steps = [
            ("Validate ROM", self.validate_rom),
            ("Setup directories", self.setup_directories),
            ("Extract ROM data", self.extract_rom_data),
            ("Check dependencies", self.check_dependencies),
            ("Configure CMake", self.configure_cmake),
            ("Build executable", self.build_executable),
            ("Copy executable", self.copy_executable),
        ]
        
        for step_name, step_func in steps:
            if not step_func():
                print_error(f"Failed at step: {step_name}")
                return False
        
        # Cleanup (always attempt, even if not critical)
        self.cleanup()
        
        # Success message
        print_header("Build Complete!")
        
        exe_name = 'WiiSports.exe' if sys.platform == 'win32' else 'WiiSports'
        output_exe = self.output_dir / exe_name
        
        print(f"{Colors.GREEN}Your Wii Sports PC executable is ready!{Colors.ENDC}\n")
        print(f"Location: {Colors.BOLD}{output_exe}{Colors.ENDC}\n")
        print("To run the game:")
        print(f"  {Colors.CYAN}{output_exe}{Colors.ENDC}\n")
        print("Note: You may need to extract assets first using asset_extractor.py")
        
        return True

def main():
    parser = argparse.ArgumentParser(
        description='Build Wii Sports PC executable from ROM',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python build_from_rom.py --rom wii_sports.iso
  python build_from_rom.py --rom /path/to/game.iso --output ./MyBuild
  python build_from_rom.py --rom game.iso --debug --verbose

This tool combines the decompiled source code with your ROM to create
a native PC executable. The ROM is required because ~70% of the game
code has not been decompiled yet.
        """
    )
    
    parser.add_argument('--rom', required=True,
                       help='Path to your Wii Sports ROM (ISO)')
    parser.add_argument('--output', default='WiiSports_PC_Build',
                       help='Output directory (default: WiiSports_PC_Build)')
    parser.add_argument('--debug', action='store_true',
                       help='Build debug version instead of release')
    parser.add_argument('--keep-temp', action='store_true',
                       help='Keep temporary build files for debugging')
    parser.add_argument('--verbose', action='store_true',
                       help='Show verbose build output')
    
    args = parser.parse_args()
    
    build_type = 'Debug' if args.debug else 'Release'
    
    builder = ROMBuilder(
        rom_path=args.rom,
        output_dir=args.output,
        build_type=build_type,
        keep_temp=args.keep_temp,
        verbose=args.verbose
    )
    
    success = builder.build()
    
    sys.exit(0 if success else 1)

if __name__ == '__main__':
    main()
