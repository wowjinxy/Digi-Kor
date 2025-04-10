// DigimonApp.hpp
#pragma once
#include <SDL.h>

struct DigimonApp {
    bool isRunning;
    SDL_Window* windowHandle;

    bool Initialize();
    void Run();
    void Shutdown();
};

extern DigimonApp g_App;