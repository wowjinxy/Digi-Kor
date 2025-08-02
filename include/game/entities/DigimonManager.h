#pragma once

#include <windows.h>

namespace Game {
namespace Entities {

class DigimonManager {
public:
    static bool Initialize();
    static void Shutdown();
    
    // Digimon initialization and management
    static void InitializeDigimonObject(short param);
    static void InitializeTamer(int param1, int param2, int param3, int param4, int param5, int param6, int param7);
    static void InitializePartner(int param1, int param2, int param3, int param4, int param5, int param6, int param7);
    
    // Entity updates
    static void RunEntityUpdates();
    static void RunGameLogicSystems();

private:
    static bool m_initialized;
    static void* m_digimonPool;
};

// Function declarations from reverse engineering
extern "C" {
    void __stdcall initializeDigimonObject(short param);
    void __stdcall initializeTamer(int param1, int param2, int param3, int param4, int param5, int param6, int param7);
    void __stdcall initializePartner(int param1, int param2, int param3, int param4, int param5, int param6, int param7);
    void __stdcall RunEntityUpdates();
    void __stdcall RunGameLogicSystems();
}

} // namespace Entities
} // namespace Game