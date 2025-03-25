#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <cstdio>
#include "hook_manager.hpp"

FARPROC p[3] = { 0 };

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

extern "C" BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID) {
    static HINSTANCE hL;

    if (reason == DLL_PROCESS_ATTACH) {
        hL = LoadLibrary(_T(".\\D3D8_org.dll"));
        if (!hL) return FALSE;

        p[0] = GetProcAddress(hL, "DebugSetMute");
        p[1] = GetProcAddress(hL, "ValidatePixelShader");
        p[2] = GetProcAddress(hL, "ValidateVertexShader");

        DisableThreadLibraryCalls(hInst);
        CreateConsole();
        ApplyNoCD();
        //InitializeHooks(hInst);  // <--- central hook + pointer setup
    }
    else if (reason == DLL_PROCESS_DETACH) {
        if (hL) FreeLibrary(hL);
        //ShutdownHooks();
    }

    return TRUE;
}

#pragma comment(linker, "/export:Direct3DCreate8=D3D8_org.Direct3DCreate8")
