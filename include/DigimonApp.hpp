// DigimonApp.hpp
#pragma once
#include <windows.h>

struct DigimonApp {
    bool isRunning;
    HWND windowHandle;  // Using Windows HWND instead of SDL_Window

    bool Initialize();
    void Run();
    void Shutdown();
};

extern DigimonApp g_App;