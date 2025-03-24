// hook_system.h
#pragma once
#include <windows.h>
#include <vector>
#include <iostream>

struct HookEntry {
    const char* name;
    uintptr_t offset;
    void* originalAddress;
    void* redirectAddress;
};

extern std::vector<HookEntry> g_hooks;
extern uintptr_t baseAddress;

void RedirectFunction(void* targetFunction, void* customFunction);
void RegisterAllHooks();
void UnregisterAllHooks();

#define DEFINE_HOOK(name, offset) \
    extern "C" void name##_handler(); \
    uintptr_t name##_offset = offset; \
    void* name##_original = nullptr;

#define IMPLEMENT_HOOK_HANDLER(name) \
    extern "C" void name##_handler()

#define REGISTER_HOOK(name) \
    do { \
        void* target = reinterpret_cast<void*>(baseAddress + name##_offset); \
        name##_original = target; \
        RedirectFunction(target, (void*)name##_handler); \
        g_hooks.push_back({ #name, name##_offset, target, (void*)name##_handler }); \
        std::cout << "[HOOK] " << #name << " registered at offset 0x" << std::hex << name##_offset << std::endl; \
    } while (0)

// Example Usage:
// DEFINE_HOOK(SomeFunction, 0x123456);
// IMPLEMENT_HOOK_HANDLER(SomeFunction) {
//     __asm {
//         // Your custom logic here
//         ret
//     }
// }
// In RegisterAllHooks():
// REGISTER_HOOK(SomeFunction);
