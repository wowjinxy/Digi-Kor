// rendering/Renderer.hpp
#pragma once
#include <SDL.h>

bool Renderer_Initialize(SDL_Window* window);
void Renderer_Shutdown();
void Renderer_Clear();
void Renderer_Present();
