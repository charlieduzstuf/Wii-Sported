#!/usr/bin/env python3
"""
Wii Sports Asset Extractor for PC Builds

Extracts assets from Wii Sports ROM for use with PC port.
ROM is only needed once - extracted assets are loaded at runtime.

Usage:
    python asset_extractor.py --rom /path/to/wii_sports.iso
    
This creates a pc_assets/ directory with all game data.
The PC executable loads assets from this directory at runtime.
"""

import argparse
import os
import sys
import subprocess
import json
from pathlib import Path

def check_dependencies():
    """Check if required tools are available"""
    print("Checking dependencies...")
    
    # Check for wit (Wiimms ISO Tools)
    try:
        subprocess.run(['wit', '--version'], capture_output=True, check=True)
        print("✓ wit (Wiimms ISO Tools) found")
    except (subprocess.CalledProcessError, FileNotFoundError):
        print("✗ wit not found. Please install Wiimms ISO Tools:")
        print("  https://wit.wiimm.de/")
        return False
    
    return True

def extract_rom(rom_path, temp_dir):
    """Extract ROM contents using wit"""
    print(f"\nExtracting ROM: {rom_path}")
    
    if not os.path.exists(rom_path):
        print(f"Error: ROM file not found: {rom_path}")
        return False
    
    # Create temp directory
    os.makedirs(temp_dir, exist_ok=True)
    
    # Extract ROM
    try:
        subprocess.run([
            'wit', 'extract', rom_path, temp_dir, '--psel=data'
        ], check=True)
        print(f"✓ ROM extracted to {temp_dir}")
        return True
    except subprocess.CalledProcessError as e:
        print(f"✗ ROM extraction failed: {e}")
        return False

def organize_assets(temp_dir, output_dir):
    """Organize extracted files into pc_assets structure"""
    print(f"\nOrganizing assets to {output_dir}")
    
    # Create directory structure
    dirs = [
        'textures',
        'models',
        'sounds',
        'data',
        'fonts',
        'layouts'
    ]
    
    for d in dirs:
        os.makedirs(os.path.join(output_dir, d), exist_ok=True)
    
    # Copy and convert files (basic implementation)
    # In a full implementation, would convert formats here
    
    files_path = os.path.join(temp_dir, 'DATA', 'files')
    if os.path.exists(files_path):
        print("  Copying game files...")
        # Copy files maintaining structure
        import shutil
        for root, dirs, files in os.walk(files_path):
            for file in files:
                src = os.path.join(root, file)
                # Determine destination based on file extension
                ext = os.path.splitext(file)[1].lower()
                
                if ext in ['.tpl', '.tex']:
                    dst_dir = os.path.join(output_dir, 'textures')
                elif ext in ['.brres', '.mdl']:
                    dst_dir = os.path.join(output_dir, 'models')
                elif ext in ['.brsar', '.brstm', '.wav']:
                    dst_dir = os.path.join(output_dir, 'sounds')
                else:
                    dst_dir = os.path.join(output_dir, 'data')
                
                dst = os.path.join(dst_dir, file)
                shutil.copy2(src, dst)
                print(f"    {file} → {os.path.basename(dst_dir)}/")
    
    print("✓ Assets organized")
    return True

def create_manifest(output_dir):
    """Create manifest file listing all assets"""
    print("\nCreating asset manifest...")
    
    manifest = {
        'version': '1.0',
        'game': 'Wii Sports',
        'assets': {}
    }
    
    # Scan all asset directories
    for category in ['textures', 'models', 'sounds', 'data', 'fonts', 'layouts']:
        asset_dir = os.path.join(output_dir, category)
        if os.path.exists(asset_dir):
            files = []
            for root, _, filenames in os.walk(asset_dir):
                for filename in filenames:
                    rel_path = os.path.relpath(os.path.join(root, filename), asset_dir)
                    files.append(rel_path)
            manifest['assets'][category] = files
            print(f"  {category}: {len(files)} files")
    
    # Write manifest
    manifest_path = os.path.join(output_dir, 'manifest.json')
    with open(manifest_path, 'w') as f:
        json.dump(manifest, f, indent=2)
    
    print(f"✓ Manifest created: {manifest_path}")
    return True

def main():
    parser = argparse.ArgumentParser(description='Extract Wii Sports assets for PC build')
    parser.add_argument('--rom', required=True, help='Path to Wii Sports ROM (ISO, WBFS, etc.)')
    parser.add_argument('--output', default='pc_assets', help='Output directory (default: pc_assets)')
    parser.add_argument('--temp', default='temp_extract', help='Temporary extraction directory')
    parser.add_argument('--keep-temp', action='store_true', help='Keep temporary files')
    
    args = parser.parse_args()
    
    print("=" * 60)
    print("Wii Sports Asset Extractor for PC")
    print("=" * 60)
    
    # Check dependencies
    if not check_dependencies():
        sys.exit(1)
    
    # Extract ROM
    if not extract_rom(args.rom, args.temp):
        sys.exit(1)
    
    # Organize assets
    if not organize_assets(args.temp, args.output):
        sys.exit(1)
    
    # Create manifest
    if not create_manifest(args.output):
        sys.exit(1)
    
    # Cleanup temp directory
    if not args.keep_temp:
        import shutil
        shutil.rmtree(args.temp, ignore_errors=True)
        print(f"\n✓ Cleaned up temporary files")
    
    print("\n" + "=" * 60)
    print("Asset extraction complete!")
    print("=" * 60)
    print(f"\nAssets extracted to: {args.output}/")
    print("You can now build and run the PC version:")
    print("  cmake -B build -DPLATFORM_PC=ON")
    print("  cmake --build build")
    print("  ./build/bin/WiiSports")
    print("\nThe ROM is no longer needed for building or running the game.")
    print("=" * 60)

if __name__ == '__main__':
    main()
