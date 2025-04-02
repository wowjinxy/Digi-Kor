#pragma once
#include <cstddef>
#include <cstring>
#include <windows.h>

enum class HookType { AUTO, CODE, IAT };

struct Detouring {
    const char* nameInDllReplacement;
    uintptr_t addressInOriginalBinary;
    void* replaceWith;  // <-- direct pointer!
    HookType type;
    bool stub;          // whether to attempt hooking

    void* addrToReplace = nullptr;
    void** originalCall = nullptr;
};

// External declaration (for use in hook_manager.cpp)
extern Detouring detours[];
extern const size_t detourCount;

inline void* GetOriginalFunction(const char* name) {
    for (size_t i = 0; i < detourCount; ++i) {
        if (strcmp(detours[i].nameInDllReplacement, name) == 0 && detours[i].originalCall) {
            return *detours[i].originalCall;
        }
    }
    return nullptr;
}

#define CALL_ORIGINAL(name, ...) \
    reinterpret_cast<decltype(&name)>(GetOriginalFunction(#name))(__VA_ARGS__)

#define MEMBER_HOOK(cls, method) reinterpret_cast<void*>(&cls::method)

#define PATCH_STR PatchString
inline void PatchString(uintptr_t targetPtr, const char* replacementText) {
    char* persistentCopy = _strdup(replacementText);  // Allocated on heap
    DWORD oldProtect;

    if (VirtualProtect(reinterpret_cast<void*>(targetPtr), sizeof(const char*), PAGE_EXECUTE_READWRITE, &oldProtect)) {
        *reinterpret_cast<const char**>(targetPtr) = persistentCopy;
        VirtualProtect(reinterpret_cast<void*>(targetPtr), sizeof(const char*), oldProtect, &oldProtect);
    }
}

