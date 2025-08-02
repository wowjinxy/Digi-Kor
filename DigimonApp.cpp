// DigimonApp.cpp
#include "DigimonApp.hpp"
#include "engine/timing/TimSystem.h"
#include <windows.h>
#include <iostream>

DigimonApp g_App;

bool DigimonApp::Initialize() {
    isRunning = false;

    std::cout << "[DigimonApp] Initializing application (D3D8 shim handles rendering)..." << std::endl;

    // Window creation handled by D3D8 shim
    windowHandle = nullptr; // Will be set by game initialization

    // Initialize timing system
    if (!Engine::Timing::TimSystem::Initialize()) {
        std::cerr << "[DigimonApp] Failed to initialize TimSystem." << std::endl;
        return false;
    }

    isRunning = true;
    return true;
}

void DigimonApp::Run() {
    while (isRunning) {
        // Main game loop - most logic handled by hooks
        // D3D8 shim handles window messages and rendering
        
        // Update game state here (placeholder)
        // UpdateGame();
        
        // Sleep to prevent busy waiting
        Sleep(1);
    }
}

void DigimonApp::Shutdown() {
    std::cout << "[DigimonApp] Shutting down application..." << std::endl;
    
    Engine::Timing::TimSystem::Shutdown();
    
    isRunning = false;
}
