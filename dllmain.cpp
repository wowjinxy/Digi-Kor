#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <cstdio>
#include "hook_manager.hpp"
#include "system/ExceptionHandler.h"
#include "ConfigINI.h"
#include "system/InputSystem.hpp"
#include <Cicada/Cicada.hpp>
#include <CicadaHooks.hpp>
#include <SDL.h>
#include <thread>

FARPROC p[3] = { 0 };

ConfigINI config("config.ini");

std::atomic<bool> g_Running = true;
std::thread g_InputThread;

void PollInputLoop() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    while (g_Running.load()) {
        //SDL_PumpEvents();

        //const uint8_t* keys = SDL_GetKeyboardState(NULL);

        //*INPUT_SPACE = keys[SDL_SCANCODE_SPACE] ? 1 : 0;
        //*INPUT_LEFT = keys[SDL_SCANCODE_LEFT] ? 1 : 0;
        //*INPUT_RIGHT = keys[SDL_SCANCODE_RIGHT] ? 1 : 0;
        //*INPUT_UP = keys[SDL_SCANCODE_UP] ? 1 : 0;
        //*INPUT_DOWN = keys[SDL_SCANCODE_DOWN] ? 1 : 0;
        //*INPUT_Z = keys[SDL_SCANCODE_Z] ? 1 : 0;
        //*INPUT_X = keys[SDL_SCANCODE_X] ? 1 : 0;
        //*INPUT_D = keys[SDL_SCANCODE_D] ? 1 : 0;
        //*INPUT_S = keys[SDL_SCANCODE_S] ? 1 : 0;
        //*INPUT_ESC = keys[SDL_SCANCODE_ESCAPE] ? 1 : 0;

        //SDL_Delay(16); // ~60 FPS
    }

    SDL_Quit();
}

void CreateConsole() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);

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
    if (config.getBool("Display", "Fullscreen", false)) {
        DEVMODE dm = {};
        dm.dmSize = sizeof(DEVMODE);
        dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
        dm.dmPelsWidth = config.getInt("Display", "Width", 640);
        dm.dmPelsHeight = config.getInt("Display", "Height", 480);
        dm.dmBitsPerPel = 32;

        //if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL) {
            //std::cout << "[+] Fullscreen mode set to " << dm.dmPelsWidth << "x" << dm.dmPelsHeight << "\n";
        //}
        //else {
            //std::cerr << "[-] Failed to set fullscreen display mode.\n";
        //}
    }
}

DWORD WINAPI DeferredStartup(LPVOID)
{
    CreateConsole();
    ApplyDisplaySettings(config);
    InitializeHooks(GetModuleHandle(nullptr));
    InitCrashHandler();
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

        ApplyNoCD();

        g_InputThread = std::thread(PollInputLoop);

        CreateThread(0, 0, DeferredStartup, 0, 0, 0);
    }
    else if (reason == DLL_PROCESS_DETACH) {
        if (hL) FreeLibrary(hL);
        g_Running = false;
        if (g_InputThread.joinable()) g_InputThread.join();
        ShutdownHooks();
    }

    return TRUE;
}

#pragma comment(linker, "/export:Direct3DCreate8=D3D8_org.Direct3DCreate8")
