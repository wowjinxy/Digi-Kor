#pragma once
#include <cstdint>
#include <Windows.h>

class CDWWnd {
public:
    int __fastcall OnCreate(int thisPtr, uint8_t dl, uint32_t lparam);
};

#pragma once

// PrepareDisplayModeParameters @ 0x0048656d
inline int(__stdcall* PrepareDisplayModeParameters)(
    void* thisPtr,
    int* d3dDevice,
    uint* width,
    uint* height,
    uint* bpp,
    uint flags,
    int* extraParams,
    int modeSelect
    ) = reinterpret_cast<decltype(PrepareDisplayModeParameters)>(0x0048656d);

// CreateDisplaySurface @ 0x00486595
inline int(__stdcall* CreateDisplaySurface)(
    void* thisPtr,
    int* d3dDevice,
    uint deviceID,
    uint param3,
    uint param4,
    uint param5,
    int param6,
    int param7,
    int param8
    ) = reinterpret_cast<decltype(CreateDisplaySurface)>(0x00486595);

// TitleScreen::Initialize @ 0x004070a0
inline int(__fastcall* TitleScreen_Initialize)(
    void* thisPtr,
    int* graphicsDevice
    ) = reinterpret_cast<decltype(TitleScreen_Initialize)>(0x004070a0);

using PrepareDisplayModeParameters_t = bool(__thiscall*)(
    void* _this,
    int* param1,
    uint* param2,
    uint* param3,
    uint* param4,
    uint  param5,
    int* param6,
    int   param7
    );
