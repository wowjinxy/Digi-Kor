#include "main.h"
#include "WinProc.h"
#include <iostream>
#include <windows.h>
#include <cstring>
#include <cassert>
#include <fstream>
#include <mbctype.h>
#include <atomic>

// --- Globals ---
AppGlobalStruct* g_AppGlobal = reinterpret_cast<AppGlobalStruct*>(0x004c93d0);
HWND g_hWnd = nullptr;
HDC g_hDC = nullptr;

extern "C" void* __cdecl GetGameContext() {
    return g_AppGlobal ? g_AppGlobal->gameContext : nullptr;
}

// --- Function pointers ---
using InitMainAndRendererFunc = void* (__fastcall*)(void*);
InitMainAndRendererFunc InitializeMainWindowAndRenderer = reinterpret_cast<InitMainAndRendererFunc>(0x00406620);

using InitGraphicsFunc = bool (*)();
InitGraphicsFunc InitializeGraphicsSystemGlobals = reinterpret_cast<InitGraphicsFunc>(0x00475200);

// MFC InitApp function removed - was causing access violations
// using InitAppFunc = bool (*)();
// InitAppFunc InitializeMainApplicationObject = reinterpret_cast<InitAppFunc>(0x00428e7b);

// --- Window Management (D3D8 shim handles rendering) ---
bool InitWindow(HWND hWnd) {
    g_hWnd = hWnd;
    g_hDC = GetDC(hWnd);
    if (!g_hDC) return false;

    std::cout << "[+] Window context initialized (D3D8 shim handles rendering).\n";
    return true;
}

void RenderFrame() {
    // Rendering handled by D3D8 shim - this is a stub for compatibility
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    FullWndProc(hWnd, msg, wParam, lParam);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND CreateGameWindow(HINSTANCE hInstance) {
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

    HWND hWnd = CreateWindowExW(
        0, CLASS_NAME, L"Digimon World", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr, nullptr, hInstance, nullptr
    );

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    return hWnd;
}

void SetCodePage() {
    _setmbcp(949); // EUC-KR for Digimon World (Korean release)
}

#ifdef STANDALONE
int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
#else 
int(__stdcall* ORIG_DigiMain)(HINSTANCE, HINSTANCE, LPSTR, int) = nullptr;

int __stdcall DigiMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
    //__debugbreak();
    int len = MultiByteToWideChar(CP_ACP, 0, lpCmdLine, -1, nullptr, 0);
    std::wstring wideCmdLine(len, L'\0');
    MultiByteToWideChar(CP_ACP, 0, lpCmdLine, -1, &wideCmdLine[0], len);
    HWND hWnd = CreateGameWindow(hInstance);
    if (!hWnd) {
        MessageBoxA(NULL, "Failed to create window.", "Error", MB_ICONERROR);
        return 1;
    }

    if (!InitWindow(hWnd)) {
        MessageBoxA(NULL, "Failed to initialize window.", "Error", MB_ICONERROR);
        return 2;
    }

    if (!InitializeGraphicsSystemGlobals()) {
        MessageBoxA(NULL, "Failed to initialize graphics system globals.", "Error", MB_ICONERROR);
        return 3;
    }

    // MFC application object initialization removed - was causing access violations
    // if (!InitializeMainApplicationObject()) {
    //     MessageBoxA(NULL, "Failed to initialize game application object.", "Error", MB_ICONERROR);
    //     return 4;
    // }

    MSG msg = { 0 };
    void* context = GetGameContext();

    while (true) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                goto quit;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (context)
            GameLoopTick(context);

        RenderFrame();
    }

quit:
    // Add GameShutdown() call here if available
    return static_cast<int>(msg.wParam);
}
