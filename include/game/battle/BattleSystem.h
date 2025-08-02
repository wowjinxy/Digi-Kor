#pragma once

#include <windows.h>
#include <cstdint>

namespace Game {
namespace Battle {

class BattleSystem {
public:
    static bool Initialize();
    static void Shutdown();
    
    // Battle management
    static uint32_t StartBattle(uint32_t param);
    static void TickBattle(short param);
    static void AdvanceBattleTime(uint32_t param1, uint32_t param2, char param3);
    
    // Battle timing
    static void StopGameTime();

private:
    static bool m_initialized;
    static void* m_battleState;
};

// Function declarations from reverse engineering
extern "C" {
    uint32_t __stdcall startBattle(uint32_t param);
    void __stdcall tickBattle(short param);
    void __stdcall advanceBattleTime(uint32_t param1, uint32_t param2, char param3);
    void __stdcall stopGameTime();
}

} // namespace Battle
} // namespace Game