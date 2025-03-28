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

int RunOpenGLLoop()
{
    MSG msg;
    while (true) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                return (int)msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
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

int __stdcall AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine, int nCmdShow)
{
    assert(hPrevInstance == NULL);

    int nReturnCode = -1;
    FWinThread* pThread = AfxGetThread();

    // Perform specific initializations
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
