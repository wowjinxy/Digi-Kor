#include <wtypes.h>
#include <ConfigINI.h>
#include <Detouring.hpp>

typedef BOOL(__fastcall* CWndShowWindowReal)(void* thisPtr, int, int);
CWndShowWindowReal OriginalShowWindow = nullptr;

extern "C" BOOL __fastcall MyShowWindowThunk(void* thisPtr, int /*unusedEDX*/, int nCmdShow)
{
    HWND hWnd = reinterpret_cast<HWND>(thisPtr);

    ConfigINI config("config.ini");
    if (config.getBool("Display", "Fullscreen", false)) {
        LONG style = GetWindowLong(hWnd, GWL_STYLE);
        style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
        SetWindowLong(hWnd, GWL_STYLE, style);

        MONITORINFO mi = { sizeof(mi) };
        if (GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
            SetWindowPos(hWnd, HWND_TOP,
                mi.rcMonitor.left, mi.rcMonitor.top,
                mi.rcMonitor.right - mi.rcMonitor.left,
                mi.rcMonitor.bottom - mi.rcMonitor.top,
                SWP_NOZORDER | SWP_FRAMECHANGED);
        }
    }

    if (!OriginalShowWindow) {
        void** thunkPtr = reinterpret_cast<void**>(0x004A71DC);
        if (IsBadReadPtr(thunkPtr, sizeof(void*))) {
            std::cerr << "[!] Invalid thunk pointer for ShowWindow.\n";
            return FALSE;
        }
        OriginalShowWindow = reinterpret_cast<CWndShowWindowReal>(*thunkPtr);
    }

    return OriginalShowWindow(thisPtr, 0, nCmdShow);
}
