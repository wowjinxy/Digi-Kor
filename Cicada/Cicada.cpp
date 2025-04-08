// === Cicada.cpp ===
#include "Cicada.hpp"
#include "CicadaHooks.hpp" // This is the generated file containing hook list

namespace Cicada {

    void* GetOriginalFunction(const char* name) {
        for (size_t i = 0; i < hookCount; ++i) {
            if (strcmp(hooks[i].name, name) == 0 && hooks[i].originalFuncPtr) {
                return *hooks[i].originalFuncPtr;
            }
        }
        return nullptr;
    }

    void PatchString(uintptr_t target, const char* replacement) {
        DWORD oldProtect;
        if (VirtualProtect(reinterpret_cast<void*>(target), sizeof(const char*), PAGE_EXECUTE_READWRITE, &oldProtect)) {
            *reinterpret_cast<const char**>(target) = replacement;
            VirtualProtect(reinterpret_cast<void*>(target), sizeof(const char*), oldProtect, &oldProtect);
        }
    }

    void ApplyAllHooks() {
        RegisterCallsiteHooks();  // 🔼 Make sure callsite patches happen before anything else

        for (size_t i = 0; i < hookCount; ++i) {
            Hook& h = hooks[i];
            if (!h.enabled || h.replacement == nullptr) continue;

            DWORD oldProtect;
            if (h.type == HookType::IAT || h.type == HookType::AUTO) {
                VirtualProtect(reinterpret_cast<void*>(h.address), sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect);
                h.originalFuncPtr = reinterpret_cast<void**>(h.address);
                h.resolvedAddr = *h.originalFuncPtr;
                *h.originalFuncPtr = h.replacement;
                VirtualProtect(reinterpret_cast<void*>(h.address), sizeof(void*), oldProtect, &oldProtect);

                printf("[CICADA] Applied IAT/AUTO hook: %s → %p\n", h.name, h.replacement);
            }
        }
    }


    // === Call-Site Patch Support ===

    void PatchCallAbsolute(uintptr_t address, void* hookFunc, void** originalOut) {
        DWORD oldProtect;
        VirtualProtect(reinterpret_cast<void*>(address), 5, PAGE_EXECUTE_READWRITE, &oldProtect);

        if (originalOut) {
            int32_t rel = *reinterpret_cast<int32_t*>(address + 1);
            uintptr_t orig = address + 5 + rel;
            *originalOut = reinterpret_cast<void*>(orig);
        }

        uintptr_t relOffset = reinterpret_cast<uintptr_t>(hookFunc) - (address + 5);
        *reinterpret_cast<uint8_t*>(address) = 0xE8;
        *reinterpret_cast<int32_t*>(address + 1) = static_cast<int32_t>(relOffset);

        VirtualProtect(reinterpret_cast<void*>(address), 5, oldProtect, &oldProtect);
    }

    void* CreateTrampoline(void* target, size_t patchSize) {
        void* gateway = VirtualAlloc(nullptr, patchSize + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (!gateway) return nullptr;

        memcpy(gateway, target, patchSize);

        uintptr_t jumpBackAddr = (uintptr_t)target + patchSize;
        uintptr_t relOffset = jumpBackAddr - ((uintptr_t)gateway + patchSize + 5);

        uint8_t* trampolineEnd = reinterpret_cast<uint8_t*>(gateway) + patchSize;
        trampolineEnd[0] = 0xE9;
        *reinterpret_cast<int32_t*>(trampolineEnd + 1) = static_cast<int32_t>(relOffset);

        return gateway;
    }

    void PatchWithTrampoline(uintptr_t address, void* hookFunc, void** trampolineOut, size_t patchSize) {
        if (!trampolineOut) return;

        void* trampoline = CreateTrampoline(reinterpret_cast<void*>(address), patchSize);
        if (!trampoline) return;

        *trampolineOut = trampoline;

        DWORD oldProtect;
        VirtualProtect(reinterpret_cast<void*>(address), patchSize, PAGE_EXECUTE_READWRITE, &oldProtect);

        uintptr_t relOffset = reinterpret_cast<uintptr_t>(hookFunc) - (address + 5);
        *reinterpret_cast<uint8_t*>(address) = 0xE9;
        *reinterpret_cast<int32_t*>(address + 1) = static_cast<int32_t>(relOffset);

        // NOP the rest if more than 5 bytes
        for (size_t i = 5; i < patchSize; ++i) {
            *reinterpret_cast<uint8_t*>(address + i) = 0x90;
        }

        VirtualProtect(reinterpret_cast<void*>(address), patchSize, oldProtect, &oldProtect);
    }

    void PatchPointer(void** address, void* hookFunc, void** originalOut) {
        DWORD oldProtect;
        VirtualProtect(address, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect);

        if (originalOut)
            *originalOut = *address;

        *address = hookFunc;

        VirtualProtect(address, sizeof(void*), oldProtect, &oldProtect);
    }
} // namespace Cicada
