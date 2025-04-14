// === Cicada.hpp ===
#pragma once
#include <cstddef>
#include <cstdint>
#include <stdio.h>
#include <windows.h>

namespace Cicada {

    enum class HookType { AUTO, CODE, IAT };

    struct Hook {
        const char* name;               // Human-readable name
        uintptr_t address;             // Address in target binary
        void* replacement;             // Your detour function
        HookType type;                 // Hook category
        bool enabled;                  // Whether to apply the hook

        void* resolvedAddr = nullptr;  // Set at runtime
        void** originalFuncPtr = nullptr; // For CALL_ORIGINAL retrieval
    };

    // Provided externally by generated hook listing
    extern Hook hooks[];
    extern const size_t hookCount;

    // Lookup an original function pointer by hook name
    void* GetOriginalFunction(const char* name);

    // Apply all hooks (basic IAT and function pointer patching)
    void ApplyAllHooks();

    // === Call-Site Patch Support ===
    void PatchCallAbsolute(uintptr_t address, void* hookFunc, void** originalOut = nullptr);

    void* CreateTrampoline(void* target, size_t patchSize);

    void PatchWithTrampoline(uintptr_t address, void* hookFunc, void** trampolineOut, size_t patchSize);

    void PatchPointer(void** address, void* hookFunc, void** originalOut = nullptr);

    // Generic fallback macro: manually provide return and arg types
#define CALL_ORIG_GENERIC(name, ret_type, ...) \
    reinterpret_cast<ret_type(*)(__VA_ARGS__)>(Cicada::GetOriginalFunction(name))

    // Templated original function getter (cleaner for advanced users)
    template <typename T>
    inline T GetOriginalAs(const char* name) {
        return reinterpret_cast<T>(GetOriginalFunction(name));
    }

    // === Utility: Call a DLL function already loaded by the EXE ===
    inline FARPROC GetProcFromLoadedModule(const char* moduleName, const char* functionName) {
        HMODULE mod = GetModuleHandleA(moduleName);
        return mod ? GetProcAddress(mod, functionName) : nullptr;
    }

    inline void StubbedInitializer() {
        // No-op or logging
    }
}

#define GET_EXE_IMPORT(mod, name, type) \
    reinterpret_cast<type>(Cicada::GetProcFromLoadedModule(mod, name))

#define CICADA_DECLARE_HOOK(ret, conv, name, args) \
    extern "C" ret conv name args; \
    inline ret (conv *ORIG_##name) args = nullptr;

#define CICADA_REGISTER_CALLSITE(name, address, func) \
    Cicada::PatchCallAbsolute(address, reinterpret_cast<void*>(func), reinterpret_cast<void**>(&ORIG_##func))

#define CICADA_REGISTER_CALLSITE_TRAMPOLINE(name, address, func, patchSize) \
    void* TRAMP_##func = nullptr; \
    Cicada::PatchWithTrampoline(address, reinterpret_cast<void*>(func), &TRAMP_##func, patchSize)

#define CICADA_REGISTER_PTRPATCH(name, address, func)                                      \
    do {                                                                                   \
        void* original = nullptr;                                                          \
        Cicada::PatchPointer((void**)(address), reinterpret_cast<void*>(func), &original); \
        ORIG_##func = reinterpret_cast<decltype(func)*>(original);                         \
        printf("[CICADA] Patched %-30s at 0x%08X → %p (original: %p)\n",                   \
               name, address, reinterpret_cast<void*>(func), original);                    \
    } while (0)

#define PATCH_STRING(instructionAddr, replacementPointer) \
    do { \
        DWORD _oldProtect; \
        if (VirtualProtect(reinterpret_cast<void*>((instructionAddr) + 1), sizeof(void*), PAGE_EXECUTE_READWRITE, &_oldProtect)) { \
            *reinterpret_cast<const char**>((instructionAddr) + 1) = replacementPointer; \
            VirtualProtect(reinterpret_cast<void*>((instructionAddr) + 1), sizeof(void*), _oldProtect, &_oldProtect); \
        } \
    } while (0)

typedef unsigned int uint;