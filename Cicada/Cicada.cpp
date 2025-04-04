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
            }
            // More patch types like CODE could be added here
        }
    }

} // namespace Cicada
