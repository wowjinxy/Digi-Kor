#include "hook_manager.hpp"
#include "Detouring.hpp"
#include <windows.h>
#include <MinHook.h>
#include <iostream>

bool InitializeHooks(HINSTANCE hInst)
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

        d.replaceWith = reinterpret_cast<void*>(GetProcAddress(hInst, d.nameInDllReplacement));

        if (!d.replaceWith) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cerr << "[-] Replacement function not found for: " << d.nameInDllReplacement << std::endl;
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
            d.addrToReplace = reinterpret_cast<void*>(d.addressInOriginalBinary);

            if (MH_CreateHook(d.addrToReplace, d.replaceWith, nullptr) == MH_OK &&
                MH_EnableHook(d.addrToReplace) == MH_OK) {
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
