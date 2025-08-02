#pragma once

#include <windows.h>

namespace Engine {
namespace Application {

class MFCApplication {
public:
    static bool Initialize();
    static void Shutdown();
    static void ProcessSystemMessages();
    static bool InitializeAndShowMainWindow(int param);
    
    // MFC App lifecycle
    static void* ConstructMFCApp(void* param);
    static void* CleanUpCWinApp(void* app, bool condition);
    static void RegisterGlobalAppDestructor();
    static void DestroyGlobalAppInstance();
    
    // Message handling
    static void* GetMessageMap();
    static void UpdateGameLoop(int* param);

private:
    static bool m_initialized;
};

// Function declarations from reverse engineering
extern "C" {
    void* __stdcall ConstructMFCApp(void* param);
    void* __stdcall CleanUpCWinApp(void* app, bool condition);
    void __stdcall RegisterGlobalAppDestructor();
    void __stdcall DestroyGlobalAppInstance();
    void* __stdcall GetMessageMap();
    void __stdcall processSystemMessagesAndUpdateGameLoop(int* param);
}

} // namespace Application
} // namespace Engine