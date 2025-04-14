#include "main.h"
#include "WinProc.h"
#include "system/FWinApp.h"
#include <iostream>
#include <gl/GL.h>
#include <windows.h>
#include <cstring>
#include <cassert>
#include <fstream>
#include "GameGlue.h"
#include "MFC42.hpp"
#include <mbctype.h>
#include <DigimonApp.hpp>

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

#ifdef STANDALONE
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine, int nCmdShow)
#else 
int __stdcall DigiMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine, int nCmdShow)
#endif
{
    psudo_initterm(_InitTable, _InitTable + _InitTableCount);

    if (!g_App.Initialize()) {
        return -1;
    }

    g_App.Run();
    g_App.Shutdown();
    return 0;
}

void SetCodePage()
{
    _setmbcp(949); // Korean code page
	//_setmbcp(-3); // Automatic code page
	//_setmbcp(-1); // Restore default code page
	//_setmbcp(65001); // UTF-8 code page
}

InitFunc _InitTable[] =
{
    SetCodePage,
    //InitCStringSubsystem,
    //InitAppAndRegisterCleanup,
    //InitTimSubsystem,
    //STUBBED_SYSTEM,
    FUN_00410890,
    FUN_00410910,
    FUN_00411150,
    _InitializeTimObjectExSystem,
    STUBBED_THUNK,
    thunk_FUN_004199d0,
    FUN_0041bc40,
    FUN_0041bc80,
    FUN_0041c0f0,
    thunk_FUN_0041c140,
    FUN_0041c160,
    thunk_FUN_0041c1b0,
    thunk_FUN_0041e6d0,
    FUN_0041eb40,
    FUN_00420980,
    FUN_00420b10,
    FUN_00426470,
    InitializeFontSystem,
    FUN_00428a40,
    FUN_00428a90,
    FUN_00428ba0,
    FUN_0042c1a0,
    FUN_0042c1e0,
    FUN_0042c220,
    thunk_FUN_0042c2e0,
    FUN_0042c440,
    thunk_FUN_0042c500,
    thunk_FUN_0042c6e0,
    FUN_0042c960,
    thunk_FUN_0042c9b0,
    FUN_0042ccc0,
    FUN_0042cee0,
    FUN_0042d230,
    FUN_0042f060,
    thunk_FUN_0042f0b0,
    FUN_00433560,
    FUN_00434440,
    thunk_FUN_00434490,
    thunk_FUN_00434c50,
    FUN_00434d00,
    FUN_004393e0,
    thunk_FUN_0043a110,
    thunk_FUN_00443520,
    thunk_FUN_00443620,
    thunk_FUN_00443690,
    thunk_FUN_004436e0,
    thunk_FUN_00443750,
    thunk_FUN_004437c0,
    thunk_FUN_00443830,
    FUN_004442f0,
    FUN_00444350,
    FUN_00446d80,
    FUN_00446de0,
    STUBBED_THUNK,
    FUN_00449450,
    FUN_004522c0,
    FUN_00452330,
    FUN_00453540,
    FUN_00453580,
    FUN_00454270,
    FUN_004543d0,
    FUN_00454620,
    FUN_00454e00,
    FUN_00454e40,
    FUN_00454ff0,
    FUN_00455ea0,
    thunk_FUN_00456850,
    thunk_FUN_00456890,
    thunk_FUN_004568d0,
    thunk_FUN_00456910,
    thunk_FUN_0045b030,
    thunk_FUN_0045b800,
    thunk_FUN_0045b8c0,
    FUN_0045ca70,
    LAB_0045cad0,
    LAB_0045cbc0,
    LAB_0045de70,
    LAB_0045ded0,
    LAB_0045df10,
    LAB_0045df50,
    LAB_0045df90,
    LAB_0045dfd0,
    LAB_0045e030,
    LAB_0045e160,
    LAB_00461760,
    LAB_004617a0,
    LAB_00461df0,
    LAB_004620b0,
    LAB_004620f0,
    LAB_00462870,
    LAB_00465150,
    LAB_004696e0,
    LAB_0046bcc0,
    LAB_0046d070,
    LAB_0046d0b0,
    LAB_0046d0f0,
    LAB_0046d140,
    LAB_0046d1a0,
    LAB_00473420,
    LAB_00479a10,
    FUN_00479f60,
    FUN_0047a3b0,
    FUN_0047a3f0,
    FUN_0047a430,
    FUN_0047a490,
    thunk_FUN_0047b3e0,
    FUN_0047c6f0,
    FUN_0047d250,
    thunk_FUN_0047d710,
    thunk_FUN_0047da10,
    thunk_FUN_00481df0,
    thunk_FUN_00481e90,
    FUN_004824a0,
    FUN_00485690,
    FUN_004a1960
};

const size_t _InitTableCount = sizeof(_InitTable) / sizeof(_InitTable[0]);