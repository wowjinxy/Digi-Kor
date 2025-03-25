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

        d.addrToReplace = reinterpret_cast<void*>(d.addressInOriginalBinary);
        d.replaceWith = reinterpret_cast<void*>(GetProcAddress(hInst, d.nameInDllReplacement));

        if (!d.addrToReplace || !d.replaceWith) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cerr << "[-] Invalid hook target or replacement for: " << d.nameInDllReplacement << std::endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            continue;
        }

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

    return true;
}

void ShutdownHooks()
{
    MH_Uninitialize();
}
