#include "hook_manager.hpp"
#include "Cicada/Cicada.hpp"
#include "CicadaHooks.hpp"
#include <windows.h>
#include <MinHook.h>
#include <iostream>

void InitializeHooks(HMODULE /*module*/) {
    // MinHook init (optional for CODE hooks)
    if (MH_Initialize() != MH_OK) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::cerr << "[!] MinHook initialization failed.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    std::cerr << "[!] MinHook initialized.\n";

    for (size_t i = 0; i < Cicada::hookCount; ++i) {
        Cicada::Hook& h = Cicada::hooks[i];
        if (!h.enabled || h.replacement == nullptr)
            continue;

        if (h.type == Cicada::HookType::AUTO || h.type == Cicada::HookType::IAT) {
            // Let Cicada handle AUTO and IAT
            continue;
        }

        // Handle CODE hooks with MinHook
        void* target = reinterpret_cast<void*>(h.address);
        if (MH_CreateHook(target, h.replacement, h.originalFuncPtr) == MH_OK &&
            MH_EnableHook(target) == MH_OK) {
            std::cout << "[+] Hooked CODE: " << h.name << std::endl;
        }
        else {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cerr << "[-] CODE hook failed: " << h.name << std::endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
    }

    // Handle IAT + AUTO patching
    Cicada::ApplyAllHooks();
}

void ShutdownHooks() {
    MH_Uninitialize();
}
