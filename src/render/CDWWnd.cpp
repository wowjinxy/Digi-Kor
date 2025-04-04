#include "USER32.hpp"
#include "MFC42.hpp"
#include "CDWWnd.hpp"
#include <cstdint>

int IsCDInserted = 1;
uint32_t DAT_004e1734;

int __fastcall CDWWnd::OnCreate(int thisPtr, uint8_t dl, uint32_t lparam)
{
    if (CFrameWnd_OnCreate(reinterpret_cast<CFrameWnd*>(thisPtr)) == -1)
        return -1;

    if (IsCDInserted != 0) {
        USER32::MessageBoxA(nullptr, "Cannot find the Digimon CD.", nullptr, 0);
        return -1;
    }

    USER32::ShowCursor(FALSE);

    RECT clientRect = { 0, 0, 640, 480 };
    const int width = clientRect.right - clientRect.left;
    const int height = clientRect.bottom - clientRect.top;

    CWnd_MoveWindow(reinterpret_cast<CWnd*>(thisPtr), clientRect.left, clientRect.top, width, height, FALSE);
    const DWORD windowStyle = CWnd_GetStyle(reinterpret_cast<CWnd*>(thisPtr));
    const BOOL hasMenu = FALSE;

    USER32::AdjustWindowRect(&clientRect, windowStyle, hasMenu);
    USER32::OffsetRect(&clientRect, -clientRect.left, -clientRect.top);

    CWnd_MoveWindow(reinterpret_cast<CWnd*>(thisPtr), clientRect.left, clientRect.top, width, height, FALSE);
    CWnd_CenterWindow(reinterpret_cast<CWnd*>(thisPtr));
    CWnd_SetFocus(reinterpret_cast<CWnd*>(thisPtr));

    // Attempt to initialize rendering device
    auto deviceVTable = *reinterpret_cast<void***>(thisPtr + 0xC0);
    auto InitDevice = reinterpret_cast<int(__thiscall*)(void*, int)>(deviceVTable[1]);
    if (InitDevice(reinterpret_cast<void*>(thisPtr + 0x20), 640) != 0) {

        uint32_t format = 0xE7;
        uint32_t width = 0x73;
        uint32_t height = 1;
        uint32_t quality = 0;

        int mode = 0x1A;
        RECT dummy = { 0 };
        int prepareResult = PrepareDisplayModeParameters(
            &dummy,
            *reinterpret_cast<int**>(thisPtr + 200),
            reinterpret_cast<uint*>(&dummy),
            reinterpret_cast<uint*>(&dummy.right),
            reinterpret_cast<uint*>(&dummy.bottom),
            0,
            &mode,
            1
        );

        if (prepareResult >= 0) {
            int createResult = CreateDisplaySurface(
                reinterpret_cast<void*>(format),
                *reinterpret_cast<int**>(thisPtr + 200),
                format, width, height, quality, mode, 1,
                dummy.right
            );

            if (createResult >= 0) {
                void** vtable = *reinterpret_cast<void***>(thisPtr + 0xC0);
                using ReleaseDirect3DResources_t = void(*)(void*);
                ReleaseDirect3DResources_t ReleaseDirect3DResources = reinterpret_cast<ReleaseDirect3DResources_t>(vtable[2]);
                ReleaseDirect3DResources(reinterpret_cast<void*>(thisPtr + 0xC0));

                if (!TitleScreen_Initialize(reinterpret_cast<void*>(thisPtr + 0x114), *reinterpret_cast<int**>(thisPtr + 200))) {
                    return -1;
                }

                DAT_004e1734 = *reinterpret_cast<uint32_t*>(thisPtr + 200);
                return 0;
            }
        }

        // If we get here, something failed
        auto ShutdownDevice = reinterpret_cast<void(__thiscall*)(void*)>(deviceVTable[2]);
        ShutdownDevice(reinterpret_cast<void*>(thisPtr + 0x20));

        USER32::MessageBoxA(nullptr, "The current graphics card is not compatible with running Digimon. Please reinstall the latest graphics driver and then try running Digimon again.", "DirectX Error", 0);
    }

    return -1;
}
