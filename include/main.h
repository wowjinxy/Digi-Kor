#pragma once

#include <windows.h>

// Initialization and cleanup
bool InitOpenGL(HWND hWnd);
void CleanupOpenGL();
void RenderFrame();
int RunOpenGLLoop();

// Hook replacements
extern "C" __declspec(dllexport) bool __stdcall MyCWndCreateHook(HWND hWnd);
extern "C" __declspec(dllexport) bool __stdcall CWndCreateThunk(HWND hWnd);
int __stdcall AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine, int nCmdShow);

// For storing original function pointers (to optionally call later)
extern "C" __declspec(dllexport) bool(__stdcall* OriginalCWndCreate)(HWND);
