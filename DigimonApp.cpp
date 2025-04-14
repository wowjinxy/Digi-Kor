// // DigimonApp.cpp
#include "DigimonApp.hpp"
#include "rendering/Renderer.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <system/TimSystem.hpp>

DigimonApp g_App;

bool DigimonApp::Initialize() {
    isRunning = false;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    if (TTF_Init() != 0) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        return false;
    }

    windowHandle = SDL_CreateWindow(
        "Digimon World",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!windowHandle) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        return false;
    }

    if (!Renderer_Initialize(windowHandle)) {
        SDL_Log("Renderer initialization failed.");
        return false;
    }

	if (!InitializeTimSystem()) {
		SDL_Log("Failed to initialize TimSystem.");
		return false;
	}

    isRunning = true;
    return true;
}

void DigimonApp::Run() {
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }

            // Optional: add input handling here
        }

        // Update game state here (placeholder)
        // UpdateGame();

        // Render
        Renderer_Clear();
        // RenderGame(); ← TODO
        Renderer_Present();

        // SDL_GL_SetSwapInterval(1) already limits to VSync
        // Optionally sleep here for fixed timing if not using VSync
    }
}

void DigimonApp::Shutdown() {
    Renderer_Shutdown();

    if (windowHandle) {
        SDL_DestroyWindow(windowHandle);
        windowHandle = nullptr;
    }

    TTF_Quit();
    SDL_Quit();
}
