// Add this to your includes
#include <windows.h>
#include <xinput.h>
#include <iostream>

#pragma comment(lib, "xinput.lib") // Link to XInput

// --- RAW INPUT SETUP ---
void RegisterForRawInput(HWND hWnd)
{
    RAWINPUTDEVICE rid[2];

    // Keyboard
    rid[0].usUsagePage = 0x01;
    rid[0].usUsage = 0x06;
    rid[0].dwFlags = RIDEV_INPUTSINK;
    rid[0].hwndTarget = hWnd;

    // Mouse
    rid[1].usUsagePage = 0x01;
    rid[1].usUsage = 0x02;
    rid[1].dwFlags = RIDEV_INPUTSINK;
    rid[1].hwndTarget = hWnd;

    if (!RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE))) {
        //std::cerr << "[RawInput] Failed to register devices.\n";
    }
    else {
        //std::cout << "[RawInput] Devices registered.\n";
    }
}

void HandleRawInput(LPARAM lParam)
{
    UINT dwSize = 0;
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
    BYTE* lpb = new BYTE[dwSize];

    if (lpb == nullptr) return;

    if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {
        std::cerr << "[RawInput] GetRawInputData size mismatch.\n";
        delete[] lpb;
        return;
    }

    RAWINPUT* raw = (RAWINPUT*)lpb;

    if (raw->header.dwType == RIM_TYPEKEYBOARD) {
        RAWKEYBOARD& kbd = raw->data.keyboard;
        //std::cout << "[RawInput] Keyboard: " << kbd.VKey << " " << ((kbd.Flags & RI_KEY_BREAK) ? "Up" : "Down") << "\n";
    }
    else if (raw->header.dwType == RIM_TYPEMOUSE) {
        RAWMOUSE& mouse = raw->data.mouse;
        //std::cout << "[RawInput] Mouse: X=" << mouse.lLastX << ", Y=" << mouse.lLastY << "\n";
    }

    delete[] lpb;
}

// --- XINPUT SUPPORT ---
void PollXInput()
{
    for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i) {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        if (XInputGetState(i, &state) == ERROR_SUCCESS) {
            std::cout << "[XInput] Controller " << i << ": A=" << ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) ? "Down" : "Up") << "\n";
        }
    }
}

// --- In your main loop ---
// Call this once per frame to poll controllers
//PollXInput();
