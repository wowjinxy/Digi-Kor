#include "main.h"
#include "WinProc.h"
#include "system/FWinApp.h"
#include <iostream>
#include <gl/GL.h>
#include <windows.h>
#include <Detouring.hpp>
#include <cstring>
#include <cassert>
#include <fstream>
#include "GameGlue.h"

AppGlobalStruct* g_AppGlobal = reinterpret_cast<AppGlobalStruct*>(0x004c93d0);

HWND g_hWnd = nullptr;
HDC g_hDC = nullptr;
HGLRC g_hGLRC = nullptr;
std::atomic<bool> g_OpenGLInitialized = false;

extern "C" void* __cdecl GetGameContext() {
    return g_AppGlobal ? g_AppGlobal->gameContext : nullptr;
}

// --- External game logic function ---
GameLoopFunc GameLoopTick = reinterpret_cast<GameLoopFunc>(0x0040ec70);
MessageLoopFunc RunGameLoop = reinterpret_cast<MessageLoopFunc>(0x004066d0);

using InitMainAndRendererFunc = void* (__fastcall*)(void*);
InitMainAndRendererFunc InitializeMainWindowAndRenderer = reinterpret_cast<InitMainAndRendererFunc>(0x00406620);

// --- OpenGL Setup ---
bool InitOpenGL(HWND hWnd)
{
    g_hWnd = hWnd;
    g_hDC = GetDC(hWnd);
    if (!g_hDC) return false;

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), 1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, 32,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        24, 8, 0,
        PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    int pf = ChoosePixelFormat(g_hDC, &pfd);
    if (pf == 0 || !SetPixelFormat(g_hDC, pf, &pfd)) return false;

    g_hGLRC = wglCreateContext(g_hDC);
    if (!g_hGLRC) return false;

    if (!wglMakeCurrent(g_hDC, g_hGLRC)) return false;

    std::cout << "[+] OpenGL context initialized.\n";
    return true;
}

void RenderFrame()
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SwapBuffers(g_hDC);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	FullWndProc(hWnd, msg, wParam, lParam);
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND CreateSimpleWindow(HINSTANCE hInstance)
{
    const wchar_t* CLASS_NAME = L"CustomGameWnd";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.style = CS_OWNDC;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClassW(&wc);

    RECT rect = { 0, 0, g_ViewportWidth, g_ViewportHeight };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hWnd = CreateWindowExW(0, CLASS_NAME, L"Digimon World", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
        nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    return hWnd;
}

// --- Main loop ---
int RunOpenGLLoop()
{
    MSG msg;

    void* context = GetGameContext();  // must match the actual pointer type

    while (true)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) return static_cast<int>(msg.wParam);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (context)
            GameLoopTick(context);  // expects a void*, not int

        RenderFrame();
    }

    return 0;
}

using AllocFunc = void* (__cdecl*)(size_t);
using InitRendererFunc = void* (__fastcall*)(void*);

// Pointers to original game functions
AllocFunc GameAlloc = reinterpret_cast<AllocFunc>(0x004a7274);
InitRendererFunc InitializeRenderer = reinterpret_cast<InitRendererFunc>(0x0040e950);

int __stdcall AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine, int nCmdShow)
{
    assert(hPrevInstance == NULL);

    std::ofstream logFile("HookyLog.txt");
    std::cout.rdbuf(logFile.rdbuf()); // Redirect all std::cout to the file

    int nReturnCode = -1;
    FWinThread* pThread = AfxGetThread();

    // Create our own window
    HWND hWnd = CreateSimpleWindow(hInstance);
    if (!hWnd || !InitOpenGL(hWnd)) {
        MessageBoxA(nullptr, "Failed to create OpenGL context or window!", "Error", MB_ICONERROR);
        return -1;
    }

    // Assign to main window handle
    pThread->m_pMainWnd = hWnd;

    // --- Allocate game memory and initialize its renderer ---
    void* rendererStruct = VirtualAlloc(nullptr, 0x20000, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!rendererStruct) {
        MessageBoxA(nullptr, "GameAlloc failed!", "Error", MB_ICONERROR);
        return -1;
    }

    //void* gameContext = InitializeRenderer(rendererStruct);
    //g_AppGlobal->gameContext = gameContext;

    g_AppGlobal->gameContext = InitializeRenderer(rendererStruct);

    RunGameLoop(g_AppGlobal);
    return 0;
    //return RunOpenGLLoop();
    //return pThread->Run(); // Enters our OpenGL + GameLoopTick loop
}
