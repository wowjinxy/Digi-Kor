// GameGlue.h
#pragma once

#include <windows.h>

// Forward declarations
extern "C" void* __cdecl GetGameContext();

using GameLoopFunc = void(__fastcall*)(void*);
extern GameLoopFunc GameLoopTick;

void RenderFrame();  // Regular C++ linkage
