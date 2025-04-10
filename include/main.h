#pragma once

#include <windows.h>
#include <atomic>

#ifdef __cplusplus
extern "C" {
#endif

    void* __cdecl GetGameContext();

#ifdef __cplusplus
}
#endif


// Initialization and cleanup
bool InitOpenGL(HWND hWnd);
void CleanupOpenGL();
void RenderFrame();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int RunOpenGLLoop();

int SetModuleStateAndCodePage(bool enableFlag, int codePage);

// Declare the function pointer type
using GameLoopFunc = void(__fastcall*)(void*);
using MessageLoopFunc = void(__fastcall*)(void*);

// Declare the external variable
extern GameLoopFunc GameLoopTick;

// Hook replacements
//extern "C" __declspec(dllexport) bool __stdcall MyCWndCreateHook(HWND hWnd);
//extern "C" __declspec(dllexport) bool __stdcall CWndCreateThunk(HWND hWnd);
int __stdcall DigiMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine, int nCmdShow);

#pragma comment(lib, "opengl32.lib")

#define ASSERT(expr) assert(expr)

extern HWND g_hWnd;
extern HDC g_hDC;
extern HGLRC g_hGLRC;
extern std::atomic<bool> g_OpenGLInitialized;

// Global MFC object replica
struct AppGlobalStruct {
    void** vtable;
    uint8_t pad[0x1C];
    void* gameContext; // 0x20
};
