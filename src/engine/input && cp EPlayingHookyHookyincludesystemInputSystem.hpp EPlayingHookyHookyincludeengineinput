#include "system/InputSystem.hpp"
#include <cstring>

// == Flag target, matching game's g_KeyDownFlags[14 + i]
uint8_t g_KeyDownFlags[32] = { 0 };

// SDL scan codes mapped to action slots 0-13
static const SDL_Scancode InputKeyLookupTable[14] = {
    SDL_SCANCODE_SPACE,   // 0
    SDL_SCANCODE_LEFT,    // 1
    SDL_SCANCODE_RIGHT,   // 2
    SDL_SCANCODE_UP,      // 3
    SDL_SCANCODE_DOWN,    // 4
    SDL_SCANCODE_Z,       // 5
    SDL_SCANCODE_X,       // 6
    SDL_SCANCODE_D,       // 7
    SDL_SCANCODE_S,       // 8
    SDL_SCANCODE_ESCAPE,  // 9
    SDL_SCANCODE_F10,     // 10
    SDL_SCANCODE_F11,     // 11
    SDL_SCANCODE_F9,      // 12
    SDL_SCANCODE_F5       // 13
};

// Internal state
static uint8_t s_CurrentKeyState[SDL_NUM_SCANCODES];
static uint8_t s_PreviousKeyState[SDL_NUM_SCANCODES];

void InitializeInputSystem()
{
    std::memset(s_CurrentKeyState, 0, sizeof(s_CurrentKeyState));
    std::memset(s_PreviousKeyState, 0, sizeof(s_PreviousKeyState));
    std::memset(g_KeyDownFlags, 0, sizeof(g_KeyDownFlags));
}

void UpdateInputState_SDL()
{
    // Copy current state to previous
    std::memcpy(s_PreviousKeyState, s_CurrentKeyState, sizeof(s_CurrentKeyState));

    // Poll new state
    const uint8_t* keyboard = SDL_GetKeyboardState(nullptr);
    std::memcpy(s_CurrentKeyState, keyboard, sizeof(s_CurrentKeyState));

    // Update flags
    for (int i = 0; i < 14; ++i) {
        SDL_Scancode key = InputKeyLookupTable[i];

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
    SDL_Scancode key = InputKeyLookupTable[actionIndex];
    return s_CurrentKeyState[key] && !s_PreviousKeyState[key];
}

bool IsKeyHeld(int actionIndex)
{
    SDL_Scancode key = InputKeyLookupTable[actionIndex];
    return s_CurrentKeyState[key];
}

bool IsKeyReleased(int actionIndex)
{
    SDL_Scancode key = InputKeyLookupTable[actionIndex];
    return !s_CurrentKeyState[key] && s_PreviousKeyState[key];
}
