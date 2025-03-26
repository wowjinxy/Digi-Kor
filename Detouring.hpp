#pragma once
#include <cstddef>

enum class HookType { AUTO, CODE, IAT };

struct Detouring {
    uintptr_t addressInOriginalBinary;
    const char* nameInDllReplacement;
    bool stub;
    HookType type = HookType::AUTO;

    void* addrToReplace = nullptr;
    void* replaceWith = nullptr;
};

// External declaration (for use in hook_manager.cpp)
extern Detouring detours[];
extern const size_t detourCount;
