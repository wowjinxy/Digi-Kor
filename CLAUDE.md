# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Digi-Kor is a preservation-focused toolkit for the Korean PC port of Digimon World. The project implements a DLL injection framework that enables runtime modifications, localization support, and enhanced compatibility for this rare gaming artifact.

## Build Configuration

### Prerequisites
- Visual Studio 2022 (v143 toolset)
- Windows SDK
- Target platform: Win32 (x86) architecture only

### Build Commands
```
MSBuild Digi-Kor.sln /p:Configuration=Debug /p:Platform=Win32
MSBuild Digi-Kor.sln /p:Configuration=Release /p:Platform=Win32
```

**Output**: Generates `D3D8.dll` for DLL injection via DirectX proxy

## System Architecture

### Core Components

**DLL Injection Layer** (`dllmain.cpp`)
- Primary entry point implementing D3D8.dll proxy pattern
- Initializes debugging console and crash handling
- Applies binary patches for copy protection removal
- Manages background input processing thread

**Cicada Hooking Framework** (`Cicada/`)
- Lightweight function interception system
- Supports multiple hooking methodologies:
  - Call-site patching with trampolines
  - Direct pointer replacement
  - Import Address Table (IAT) modification
- Primary API: `CICADA_REGISTER_PTRPATCH`, `CICADA_REGISTER_CALLSITE`

**Hook Management System** (`hook_manager.hpp/.cpp`)
- Centralized lifecycle management for all function hooks
- Coordinates between Cicada framework and MinHook library
- Provides clean initialization and teardown sequences

**Configuration Management** (`ConfigINI.h/.cpp`)
- Runtime configuration parser for `config.ini`
- Manages display parameters, audio settings, and localization preferences

### Project Structure

```
src/                    # Core implementation
├── localization/       # Text encoding and translation systems
├── render/            # Graphics and windowing subsystems
└── system/            # Low-level platform integration

include/               # Organized header files
├── system/           # Platform and OS interfaces
├── render/           # Graphics and display headers
└── localization/     # Translation infrastructure

lib/                  # External dependencies
└── MinHook libraries # Function hooking library variants

ASM/                  # Assembly patches and optimizations
```

### Subsystem Overview

**Localization Engine**
- Character encoding conversion for Korean text (`ConvertJKParser`)
- Dynamic text replacement and translation pipeline (`TokenHandlers`)
- Multi-language support with runtime language switching

**Graphics Pipeline**
- OpenGL-based rendering subsystem with modern context management
- SDL2 integration for cross-platform windowing and event handling
- Custom font rendering with Unicode support

**Input Management**
- Asynchronous input polling via dedicated SDL2 thread
- Game input virtualization and key mapping
- Runtime input configuration

## Technical Implementation

### Memory Modification Techniques
- Runtime binary patching using `VirtualProtect` for memory page manipulation
- Automated copy protection bypass through `ApplyNoCD()` function
- String literal replacement via `PATCH_STRING` macro for localization

### Function Interception Methods
1. **Direct Pointer Replacement**: Function table patching for immediate redirection
2. **Call-site Modification**: Targeted instruction patching with trampoline generation
3. **Import Table Hooking**: IAT manipulation for library function interception

### Memory Layout Considerations
- Base address resolution and offset calculation (`dllmain.cpp:67-92`)
- Hardcoded function addresses as offsets from game base (e.g., `GameLoopTick` at `0x0040ec70`)
- Runtime address validation and patch verification

### Configuration System
- Centralized settings management through `config.ini`
- Supports display parameters, audio configuration, and localization options
- Runtime reconfiguration capabilities for development workflows

## Dependencies and Requirements

### Core Libraries
- **MinHook**: Professional-grade function hooking library
- **SDL2**: Cross-platform multimedia and input handling
- **Windows SDK**: Platform API access and development tools

### Development Environment
- **Visual Studio 2022**: Primary IDE with v143 platform toolset
- **Win32 Architecture**: x86 target platform requirement

## Installation and Deployment

### Build Process
1. Compile project using provided MSBuild commands
2. Verify `D3D8.dll` generation in target directory

### Target Environment Setup
1. Locate original game installation directory
2. Create backup: Rename existing `D3D8.dll` to `D3D8_org.dll`
3. Deploy built `D3D8.dll` and `config.ini` to game directory
4. Launch game through standard executable - injection occurs automatically