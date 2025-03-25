#pragma once
#include <cstddef>

struct Detouring {
    size_t addressInOriginalBinary;
    const char* nameInDllReplacement;
    bool stub;

    void* addrToReplace = nullptr;
    void* replaceWith = nullptr;
};

// External declaration (for use in hook_manager.cpp)
extern Detouring detours[];
extern const size_t detourCount;
