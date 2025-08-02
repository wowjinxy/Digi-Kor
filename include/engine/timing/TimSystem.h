#pragma once

#include <windows.h>
#include <cstdint>

namespace Engine {
namespace Timing {

class TimSystem {
public:
    static bool Initialize();
    static void Shutdown();
    
    // TIM subsystem management
    static void InitTimSubsystem();
    static void InitializeTimSystemEx();
    static void RegisterTimShutdownCallback();
    static void ShutdownTimSystem();
    
    // TIM object management
    static void* CreateTimObject(void* param);
    static void DestroyTimObject(void* timObject);
    static bool LoadTimFromFile(void* context, uint32_t param);
    static bool LoadTimFromMemory(void* context, int filePtr, int mode, int copyBackup);
    
    // TIM object pool
    static void InitializeTimObjectExSystem();
    static void InitTimObjectExPool();
    static void* CreateTimObjectEx(void* param);
    static void DestroyTimObjectEx(void* timObjectEx);
    static void RegisterTimObjectPoolShutdown();
    static void ShutdownTimObjectExPool();
    
    // TIM registration and management
    static void RegisterTim(void* context, void* param1, uint32_t param2, void* param3);
    static void* CopyTimArray(void* param1, void* param2, void* param3);
    static void InsertTimObject(void* param1, int param2, void* param3);
    static void UpdateTimListStats();

private:
    static bool m_initialized;
    static void* m_timObjectPool;
};

// Function declarations from reverse engineering
extern "C" {
    void __stdcall InitTimSubsystem();
    void __stdcall InitializeTimSystem();
    void __stdcall RegisterTimShutdownCallback();
    void __stdcall ShutdownTimSystem();
    void* __stdcall CreateTimObject(void* param);
    void __stdcall DestroyTimObject(void* timObject);
    bool __stdcall LoadTimFromFile(void* context, uint32_t param);
    bool __stdcall LoadTimFromMemory(void* context, int filePtr, int mode, int copyBackup);
    void __stdcall InitializeTimObjectExSystem();
    void __stdcall InitTimObjectExPool();
    void* __stdcall CreateTimObjectEx(void* param);
    void __stdcall DestroyTimObjectEx(void* timObjectEx);
    void __stdcall RegisterTimObjectPoolShutdown();
    void __stdcall ShutdownTimObjectExPool();
    void __stdcall RegisterTim(void* context, void* param1, uint32_t param2, void* param3);
    void* __stdcall CopyTimArray(void* param1, void* param2, void* param3);
    void __stdcall InsertTimObject(void* param1, int param2, void* param3);
    void __stdcall UpdateTimListStats();
}

} // namespace Timing
} // namespace Engine