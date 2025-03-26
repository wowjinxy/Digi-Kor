#include "hook_manager.hpp"
#include "Detouring.hpp"
#include "FunctionIncludes.h"
#include <windows.h>
#include <MinHook.h>
#include <iostream>

bool InitializeHooks(HINSTANCE /*hInst*/)
{
    if (MH_Initialize() != MH_OK) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::cerr << "[!] MinHook initialization failed.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return false;
    }
    std::cerr << "[!] MinHook init.\n";

    for (size_t i = 0; i < detourCount; ++i) {
        Detouring& d = detours[i];

        if (!d.stub)
            continue;

        if (!d.replaceWith) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cerr << "[-] Replacement function is null for: " << d.nameInDllReplacement << std::endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            continue;
        }

        HookType type = d.type;
        if (type == HookType::AUTO) {
            // Fallback logic if not explicitly defined
            type = (d.addressInOriginalBinary >= 0x004A7000 && d.addressInOriginalBinary <= 0x004A8000)
                ? HookType::IAT
                : HookType::CODE;
        }

        if (type == HookType::IAT) {
            // IAT Hook
            void** iatPtr = reinterpret_cast<void**>(d.addressInOriginalBinary);
            DWORD oldProtect;
            if (VirtualProtect(iatPtr, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect)) {
                *iatPtr = d.replaceWith;
                VirtualProtect(iatPtr, sizeof(void*), oldProtect, &oldProtect);
                std::cout << "[+] Hooked IAT: " << d.nameInDllReplacement << std::endl;
            }
            else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                std::cerr << "[-] IAT hook failed (protection): " << d.nameInDllReplacement << std::endl;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
        }
        else {
            // Normal Code Hook
            void* addrToReplace = reinterpret_cast<void*>(d.addressInOriginalBinary);

            if (MH_CreateHook(addrToReplace, d.replaceWith, d.originalCall) == MH_OK &&
                MH_EnableHook(addrToReplace) == MH_OK) {
                std::cout << "[+] Hooked: " << d.nameInDllReplacement << std::endl;
            }

            else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                std::cerr << "[-] Hook failed: " << d.nameInDllReplacement << std::endl;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
        }
    }

    return true;
}

void ShutdownHooks()
{
    MH_Uninitialize();
}