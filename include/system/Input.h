#pragma once

#include <windows.h>
#include <Xinput.h>
#include <iostream>

void RegisterForRawInput(HWND hwnd);
void HandleRawInput(LPARAM lParam);
void PollXInput();

// Optional: expose state
extern RAWINPUTDEVICE g_RawDevices[2];
extern XINPUT_STATE g_XInputState[4];
