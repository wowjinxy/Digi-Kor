#include "main.h"
#include "system/FWinApp.h"
#include <iostream>
#include <gl/GL.h>
#include <windows.h>
#include <atomic>
#include <Detouring.hpp>
#include <cstring>
#include <cassert>

#pragma comment(lib, "opengl32.lib")

#define ASSERT(expr) assert(expr)

HWND g_hWnd = nullptr;
HDC g_hDC = nullptr;
HGLRC g_hGLRC = nullptr;
std::atomic<bool> g_OpenGLInitialized = false;

// Replicating the global MFC object
struct AppGlobalStruct {
    void** vtable;     // 0x00
    uint8_t pad[0x1C]; // 0x04 - 0x1F
    void* gameContext; // 0x20
};

// Bind directly to game's RAM area
AppGlobalStruct* g_AppGlobal = reinterpret_cast<AppGlobalStruct*>(0x004c93d0);

// Optional: expose it like the original did
extern "C" void* __cdecl GetGameContext() {
    return g_AppGlobal ? g_AppGlobal->gameContext : nullptr;
}

bool InitOpenGL(HWND hWnd)
{
    g_hWnd = hWnd;
    g_hDC = GetDC(hWnd);
    if (!g_hDC) return false;

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int pf = ChoosePixelFormat(g_hDC, &pfd);
    if (pf == 0 || !SetPixelFormat(g_hDC, pf, &pfd)) return false;

    g_hGLRC = wglCreateContext(g_hDC);
    if (!g_hGLRC) return false;

    if (!wglMakeCurrent(g_hDC, g_hGLRC)) return false;

    std::cout << "[+] OpenGL context initialized successfully.\n";
    return true;
}

void CleanupOpenGL()
{
    if (g_hGLRC) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(g_hGLRC);
        g_hGLRC = nullptr;
    }
    if (g_hDC && g_hWnd) {
        ReleaseDC(g_hWnd, g_hDC);
        g_hDC = nullptr;
    }
}

void RenderFrame()
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SwapBuffers(g_hDC);
}

using GameLoopFunc = void(__fastcall*)(int);
GameLoopFunc GameLoopTick = reinterpret_cast<GameLoopFunc>(0x0040ec70);

int RunOpenGLLoop()
{
    MSG msg;
    void* gameContext = GetGameContext();// <- Replace with correct way to retrieve param_1

    while (true) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                return (int)msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (gameContext) {
            GameLoopTick((int)gameContext);
        }

        RenderFrame();
    }
    return 0;
}

extern "C" __declspec(dllexport)
bool __stdcall MyCWndCreateHook(HWND hWnd)
{
    if (!g_OpenGLInitialized.exchange(true)) {
        std::cout << "[HOOK] CWnd::Create intercepted, initializing OpenGL on HWND=" << hWnd << std::endl;
        InitOpenGL(hWnd);
    }
    return true;
}

extern "C" __declspec(dllexport)
bool(__stdcall* OriginalCWndCreate)(HWND) = nullptr;

extern "C" __declspec(dllexport)
bool __stdcall CWndCreateThunk(HWND hWnd)
{
    MyCWndCreateHook(hWnd);
    if (OriginalCWndCreate)
        return OriginalCWndCreate(hWnd);
    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

extern "C" __declspec(dllexport)
BOOL __stdcall MyShowWindowThunkImpl(HWND hWnd, int nCmdShow)
{
    std::cout << "[HOOK] MyShowWindowThunkImpl called with HWND=" << hWnd << " and nCmdShow=" << nCmdShow << std::endl;

    if (!g_OpenGLInitialized) {
        g_OpenGLInitialized = InitOpenGL(hWnd);
    }

    typedef BOOL(__stdcall* ShowWindowPtr)(HWND, int);
    ShowWindowPtr RealShowWindow = reinterpret_cast<ShowWindowPtr>(*reinterpret_cast<void**>(0x004A71DC));
    return RealShowWindow ? RealShowWindow(hWnd, nCmdShow) : FALSE;
}

const wchar_t* kWindowClassName = L"GameWindow";
const wchar_t* kWindowTitle = L"My Game Title - OpenGL Edition";

HWND CreateSimpleWindow(HINSTANCE hInstance, const char* className = "GameWindow")
{
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = kWindowClassName;
    wc.style = CS_OWNDC;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClassW(&wc)) {
        MessageBoxW(nullptr, L"Failed to register window class!", L"Error", MB_ICONERROR);
        return nullptr;
    }

    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;

    RECT rect = { 0, 0, 1280, 720 };
    AdjustWindowRect(&rect, style, FALSE);

    HWND hWnd = CreateWindowExW(
        0,
        kWindowClassName,
        L"Custom Game Window",
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd) {
        MessageBoxA(nullptr, "Failed to create window!", "Error", MB_ICONERROR);
        return nullptr;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    return hWnd;
}

int __stdcall AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine, int nCmdShow)
{
    assert(hPrevInstance == NULL);

    int nReturnCode = -1;
    FWinThread* pThread = AfxGetThread();

    HWND hWnd = CreateSimpleWindow(hInstance);
    if (!hWnd)
        return -1;

    if (!InitOpenGL(hWnd)) {
        std::cerr << "[!] OpenGL init failed.\n";
        return -1;
    }

    pThread->m_pMainWnd = hWnd;

    // --- Call InitializeMainWindowAndRenderer --- //
    using InitRendererFunc = void* (__fastcall*)(void*);
    InitRendererFunc InitializeMainWindowAndRenderer = reinterpret_cast<InitRendererFunc>(0x00406600);

    if (g_AppGlobal)
    {
        InitializeMainWindowAndRenderer(g_AppGlobal);
        std::cout << "[+] Called InitializeMainWindowAndRenderer\n";
    }
    else
    {
        std::cerr << "[!] App global structure not initialized.\n";
    }

    // Proceed with original run loop
    if (!pThread->InitInstance())
    {
        if (pThread->m_pMainWnd != NULL)
        {
            OutputDebugStringA("Warning: Destroying non-NULL m_pMainWnd\n");
            DestroyWindow(pThread->m_pMainWnd);
        }
        nReturnCode = pThread->ExitInstance();
    }
    else
    {
        nReturnCode = pThread->Run();
    }

    return nReturnCode;
}
