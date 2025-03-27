#pragma once
#include <cstddef>

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

#define CALL_ORIGINAL(name, ...) \
    reinterpret_cast<decltype(&name)>(detours[DetourIndex_##name].stub)(__VA_ARGS__)

