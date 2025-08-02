#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <cstdio>
#include "ConfigINI.h"
// Hooks temporarily disabled to isolate No-CD patch
// #include "hook_manager.hpp"
// #include "system/ExceptionHandler.h"
// #include "system/InputSystem.hpp"
// #include "Cicada/Cicada.hpp"
// #include "CicadaHooks.hpp"
// #include "HookEntryPoints.hpp"
#include <thread>

FARPROC p[3] = { 0 };

ConfigINI config("config.ini");

std::atomic<bool> g_Running = true;
std::thread g_InputThread;

void PollInputLoop() {
    // Input polling now handled by the game's native input system
    // We'll hook into the game's input processing instead of using SDL2
    
    while (g_Running.load()) {
        // TODO: Hook into game's native input processing
        // No longer using SDL2 since D3D8 shim handles input/windowing
        
        Sleep(16); // ~60 FPS timing
    }
}

void CreateConsole() {
    // Check if console already exists
    if (GetConsoleWindow() != NULL) {
        return; // Console already allocated
    }
    
    // Try to allocate console with error checking
    if (!AllocConsole()) {
        return; // Failed to allocate console, continue silently
    }
    
    // Try to redirect streams with error checking
    if (freopen_s(nullptr, "CONOUT$", "w", stdout) != 0) {
        FreeConsole();
        return;
    }
    if (freopen_s(nullptr, "CONOUT$", "w", stderr) != 0) {
        FreeConsole();
        return;
    }
    if (freopen_s(nullptr, "CONIN$", "r", stdin) != 0) {
        FreeConsole();
        return;
    }

    std::ios::sync_with_stdio();

    std::cout << R"(
  ____ ____ ____ ____ ____ ____ ____ ____ ____ ____ 
 ||P |||R |||O |||D |||I |||G |||I |||O |||U |||S ||
 ||__|||__|||__|||__|||__|||__|||__|||__|||__|||__||
 |/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|
  Intercepting Program... PRODIGIOUS!
  Fixing Bugs and Optimizing for the Digital World!
  [PRODIGIOUS] Applying in-memory binary patch...
)" << std::endl;
}

void ApplyNoCD() {
    uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

    struct Patch {
        uintptr_t offset;
        BYTE data[8];
        size_t size;
    } patches[] = {
        { 0x00667C, { 0xEB }, 1 },
        { 0x0066FD, { 0xEB, 0x06 }, 2 },
        { 0x006A6B, { 0x00 }, 1 },
        { 0x006FFA, { 0x00 }, 1 },
    };

    for (const auto& patch : patches) {
        void* address = reinterpret_cast<void*>(baseAddress + patch.offset);
        DWORD oldProtect;
        if (VirtualProtect(address, patch.size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
            memcpy(address, patch.data, patch.size);
            VirtualProtect(address, patch.size, oldProtect, &oldProtect);
            std::cout << "[+] Patched 0x" << std::hex << patch.offset << " with " << std::dec << patch.size << " byte(s).\n";
        }
        else {
            std::cerr << "[-] Failed to patch at 0x" << std::hex << patch.offset << std::endl;
        }
    }
}

void ApplyDisplaySettings(const ConfigINI& config) {
    // Display settings now handled by D3D8 shim
    // We just log the intended settings for reference
    if (config.getBool("Display", "Fullscreen", false)) {
        int width = config.getInt("Display", "Width", 640);
        int height = config.getInt("Display", "Height", 480);
        std::cout << "[+] Display settings: " << width << "x" << height << " (handled by D3D8 shim)\n";
    }
}

DWORD WINAPI DeferredStartup(LPVOID)
{
    // Wait a bit for the game process to fully initialize
    Sleep(1000);
    
    // Only create console if enabled in config
    bool enableConsole = config.getBool("Debug", "EnableConsole", false);
    if (enableConsole) {
        CreateConsole();
    }
    
    ApplyDisplaySettings(config);
    
    // Apply only No-CD patch for now - disable all function hooks
    ApplyNoCD();
    
    // TODO: Re-enable these once No-CD patch is working
    // InitializeHooks(GetModuleHandle(nullptr));
    // InitCrashHandler();
    return 0;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID) {
    static HINSTANCE hL;

    if (reason == DLL_PROCESS_ATTACH) {
        hL = LoadLibrary(_T(".\\D3D8_org.dll"));
        if (!hL) return FALSE;

        p[0] = GetProcAddress(hL, "DebugSetMute");
        p[1] = GetProcAddress(hL, "ValidatePixelShader");
        p[2] = GetProcAddress(hL, "ValidateVertexShader");

        DisableThreadLibraryCalls(hInst);

        // Hook registration disabled for testing
        // CICADA_REGISTER_PTRPATCH("WinMain", 0x004a21ac, DigiMain);

        ApplyNoCD();

        // Input thread disabled - using game's native input system via hooks
        // g_InputThread = std::thread(PollInputLoop);

        CreateThread(0, 0, DeferredStartup, 0, 0, 0);
    }
    else if (reason == DLL_PROCESS_DETACH) {
        if (hL) FreeLibrary(hL);
        g_Running = false;
        // Input thread cleanup no longer needed
        // if (g_InputThread.joinable()) g_InputThread.join();
        // Hook shutdown disabled for testing
        // ShutdownHooks();
    }

    return TRUE;
}

#pragma comment(linker, "/export:Direct3DCreate8=D3D8_org.Direct3DCreate8")
