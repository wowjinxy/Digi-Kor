#pragma once
#include <windows.h>
#include <cstdint>

extern uint8_t g_KeyDownFlags[32];

void InitializeInputSystem();
void UpdateInputState_Native();  // Using native Windows input instead of SDL
bool IsKeyPressed(int actionIndex);    // just pressed
bool IsKeyHeld(int actionIndex);       // still held
bool IsKeyReleased(int actionIndex);   // just released

#define INPUT_SPACE  (uint8_t*)0x004f3f54
#define INPUT_LEFT   (uint8_t*)0x004f3f58
#define INPUT_RIGHT  (uint8_t*)0x004f3f5c
#define INPUT_UP     (uint8_t*)0x004f3f60
#define INPUT_DOWN   (uint8_t*)0x004f3f64
#define INPUT_Z      (uint8_t*)0x004f3f68
#define INPUT_X      (uint8_t*)0x004f3f6c
#define INPUT_D      (uint8_t*)0x004f3f70
#define INPUT_S      (uint8_t*)0x004f3f74
#define INPUT_ESC    (uint8_t*)0x004f3f7c