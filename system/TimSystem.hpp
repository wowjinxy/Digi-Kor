#pragma once
#include <cstdint>

// TimSystem.hpp

bool InitializeTimSystem(uint32_t param = 0);
void ShutdownTimSystem();

void InitTimObjectExPool(); // Initializes preallocated extended TIM objects
void ShutdownTimObjectExPool();  // Safe, one-time pool cleanup
void RegisterTimObjectPoolShutdown(); // Registers ShutdownTimObjectExPool for exit
void InitializeTimObjectExSystem(); // Initializes extended TIM pool + exit callback

