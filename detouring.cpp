#include "Detouring.hpp"
#include "FunctionIncludes.h"

void* pOriginalWinMain = nullptr;

Detouring detours[] = {
    { "InitializeFrameTimer", 0x00410d40, reinterpret_cast<void*>(&InitializeFrameTimer), HookType::CODE, false },//needs fixing
    { "origReadEntireFile", 0x0041a0c0, reinterpret_cast<void*>(&ReadEntireFile), HookType::CODE, true },
    //{ "ShowWindow", 0x004A1B1C, reinterpret_cast<void*>(&MyShowWindowThunk), HookType::CODE, true },
    { "CWnd_WindowProc", 0x004a1bca, reinterpret_cast<void*>(&WndProc), HookType::CODE, false },
    //{ "CWndCreateThunk", 0x004a1c3c, reinterpret_cast<void*>(&CWndCreateThunk), HookType::CODE, true },
    //{ "__CxxFrameHandler", 0x004a1d20, reinterpret_cast<void*>(&HandleStructuredExceptionWithContext), HookType::CODE, false },
	{ "_ftol", 0x004a1e9c, reinterpret_cast<void*>(&_ftol), HookType::CODE, false }, // I have no idea what's going on here
    { "_EH_prolog", 0x004a20e8, reinterpret_cast<void*>(&_EH_prolog), HookType::CODE, false }, // Hard crash
    { "WinMain", 0x004A219C, reinterpret_cast<void*>(&AfxWinMain), HookType::CODE, false },
    // Clearly mark this as IAT hook
    { "timeGetTime", 0x004a738c, reinterpret_cast<void*>(&timeGetTime), HookType::AUTO, true},
};

const size_t detourCount = sizeof(detours) / sizeof(detours[0]);
