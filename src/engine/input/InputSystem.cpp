#include "system/InputSystem.hpp"
#include <cstring>

// == Flag target, matching game's g_KeyDownFlags[14 + i]
uint8_t g_KeyDownFlags[32] = { 0 };

// Windows virtual key codes mapped to action slots 0-13
static const int InputKeyLookupTable[14] = {
    VK_SPACE,    // 0
    VK_LEFT,     // 1
    VK_RIGHT,    // 2
    VK_UP,       // 3
    VK_DOWN,     // 4
    'Z',         // 5
    'X',         // 6
    'D',         // 7
    'S',         // 8
    VK_ESCAPE,   // 9
    VK_F10,      // 10
    VK_F11,      // 11
    VK_F9,       // 12
    VK_F5        // 13
};

// Internal state
static uint8_t s_CurrentKeyState[256];  // Windows virtual key state
static uint8_t s_PreviousKeyState[256];

void InitializeInputSystem()
{
    std::memset(s_CurrentKeyState, 0, sizeof(s_CurrentKeyState));
    std::memset(s_PreviousKeyState, 0, sizeof(s_PreviousKeyState));
    std::memset(g_KeyDownFlags, 0, sizeof(g_KeyDownFlags));
}

void UpdateInputState_Native()
{
    // Copy current state to previous
    std::memcpy(s_PreviousKeyState, s_CurrentKeyState, sizeof(s_CurrentKeyState));

    // Poll new state using Windows API
    for (int i = 0; i < 256; ++i) {
        s_CurrentKeyState[i] = (GetAsyncKeyState(i) & 0x8000) ? 1 : 0;
    }

    // Update flags
    for (int i = 0; i < 14; ++i) {
        int key = InputKeyLookupTable[i];

        if (s_CurrentKeyState[key]) {
            g_KeyDownFlags[14 + i] = 1;
        }
        else {
            g_KeyDownFlags[14 + i] = 0;
        }
    }
}

bool IsKeyPressed(int actionIndex)
{
    int key = InputKeyLookupTable[actionIndex];
    return s_CurrentKeyState[key] && !s_PreviousKeyState[key];
}

bool IsKeyHeld(int actionIndex)
{
    int key = InputKeyLookupTable[actionIndex];
    return s_CurrentKeyState[key];
}

bool IsKeyReleased(int actionIndex)
{
    int key = InputKeyLookupTable[actionIndex];
    return !s_CurrentKeyState[key] && s_PreviousKeyState[key];
}
