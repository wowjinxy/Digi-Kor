#pragma once

#include <windows.h>
#include <cstdint>

namespace Engine {
namespace Application {

class WindowManager {
public:
    static bool Initialize();
    static void Shutdown();
    
    // Window management
    static void* CDWMainWndCtor(void* param);
    static void* InitializeMainWindowAndRenderer(void* param);
    static bool PreCreateWindow(int param);
    static void* GetCDWWndClassName();
    
    // Window procedures and message handling
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static void UpdateCameraOrViewTransform(short* position);
    
    // Frame and timer management
    static void SetFrameTimerIfNotActive(void* window, uint32_t durationMs);
    static void InitializeFrameTimer(void* window, uint32_t interval);

private:
    static HWND m_mainWindow;
    static bool m_initialized;
};

// Function declarations from reverse engineering
extern "C" {
    void* __stdcall CDWMainWndCtor(void* param);
    void* __stdcall InitializeMainWindowAndRenderer(void* param);
    bool __stdcall PreCreateWindow(int param);
    void* __stdcall GetCDWWndClassName();
    void __stdcall UpdateCameraOrViewTransform(short* position);
    void __stdcall SetFrameTimerIfNotActive(void* window, uint32_t durationMs);
    void __stdcall InitializeFrameTimer(void* window, uint32_t interval);
}

} // namespace Application
} // namespace Engine