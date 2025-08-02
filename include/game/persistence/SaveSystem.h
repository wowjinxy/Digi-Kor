#pragma once

#include <windows.h>

namespace Game {
namespace Persistence {

class SaveSystem {
public:
    static bool Initialize();
    static void Shutdown();
    
    // Save/Load operations
    static void LoadSaveGames(int param);
    static void InitGameState(int param);

private:
    static bool m_initialized;
};

// Function declarations from reverse engineering
extern "C" {
    void __stdcall loadSaveGames(int param);
    void __stdcall InitGameState(int param);
}

} // namespace Persistence
} // namespace Game