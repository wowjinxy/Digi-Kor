#pragma once
#include <cstddef>
#include <cstring>

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
