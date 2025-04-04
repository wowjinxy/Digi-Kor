// === Cicada.hpp ===
#pragma once
#include <cstddef>
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

    // Patch a hardcoded const char* string in memory
    void PatchString(uintptr_t target, const char* replacement);

    // Apply all hooks (basic IAT and function pointer patching)
    void ApplyAllHooks();

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

#define GET_EXE_IMPORT(mod, name, type) \
    reinterpret_cast<type>(Cicada::GetProcFromLoadedModule(mod, name))

} // namespace Cicada

typedef unsigned int uint;