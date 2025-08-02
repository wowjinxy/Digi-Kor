#pragma once

#include <windows.h>
#include <atomic>
#include <iterator>

#ifdef __cplusplus
extern "C" {
#endif

    void* __cdecl GetGameContext();

#ifdef __cplusplus
}
#endif

// Window and initialization functions
bool InitWindow(HWND hWnd);
void RenderFrame();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef STANDALONE
int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
#else 
int __stdcall DigiMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
#endif

void SetCodePage();

// Game loop function pointers
using GameLoopFunc = void(__fastcall*)(void*);
using MessageLoopFunc = void(__fastcall*)(void*);

// Game loop tick function at known address
inline void(__fastcall* GameLoopTick)(void* thisPtr) = reinterpret_cast<decltype(GameLoopTick)>(0x0040ec70);

// Hook replacements
extern int(__stdcall* ORIG_DigiMain)(HINSTANCE, HINSTANCE, LPSTR, int);

#define ASSERT(expr) assert(expr)

// Window handles (rendering context managed by shim)
extern HWND g_hWnd;
extern HDC g_hDC;

// Global MFC object replica
struct AppGlobalStruct {
    void** vtable;
    uint8_t pad[0x1C];
    void* gameContext; // 0x20
};
