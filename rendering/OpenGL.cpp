// rendering/OpenGL.cpp
#include "Renderer.hpp"
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>

static SDL_GLContext g_GLContext = nullptr;

bool Renderer_Initialize(SDL_Window* window) {
    g_GLContext = SDL_GL_CreateContext(window);
    if (!g_GLContext) {
        SDL_Log("SDL_GL_CreateContext failed: %s", SDL_GetError());
        return false;
    }

    SDL_GL_SetSwapInterval(1); // Enable VSync

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // Background color
    return true;
}

void Renderer_Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer_Present() {
    SDL_GL_SwapWindow(SDL_GL_GetCurrentWindow());
}

void Renderer_Shutdown() {
    if (g_GLContext) {
        SDL_GL_DeleteContext(g_GLContext);
        g_GLContext = nullptr;
    }
}
