#pragma once

#include <windows.h>
#include <cstdint>

namespace Game {
namespace UI {

class MenuSystem {
public:
    static bool Initialize();
    static void Shutdown();
    
    // Menu management
    static void AddGameMenu();
    static void TickTriangleMenu();
    static void TickDigimonMenu();
    
    // Menu rendering
    static uint32_t CreateMenuBox(uint32_t param1, short param2, short param3, short param4, short param5, 
                                 uint32_t param6, uint32_t param7, uint32_t param8);
    static void RenderPlayerMenu();
    static void RenderCardsView();
    static void RenderCardsViewStrings();
    
    // Menu state updates
    static void UpdateMenuSelectionIndex(void* context, int param);
    static void UpdateMenuTextSceneIfEnabled(void* context, uint32_t param);
    
    // Tournament system
    static void ShowTournamentSchedulePopup();

private:
    static bool m_initialized;
    static void* m_currentMenu;
};

// Function declarations from reverse engineering
extern "C" {
    void __stdcall addGameMenu();
    void __stdcall tickTriangleMenu();
    void __stdcall tickDigimonMenu();
    uint32_t __stdcall createMenuBox(uint32_t param1, short param2, short param3, short param4, short param5, 
                                    uint32_t param6, uint32_t param7, uint32_t param8);
    void __stdcall renderPlayerMenu();
    void __stdcall renderCardsView();
    void __stdcall UpdateMenuSelectionIndex(void* context, int param);
    void __stdcall UpdateMenuTextSceneIfEnabled(void* context, uint32_t param);
    void __stdcall ShowTournamentSchedulePopup();
}

} // namespace UI
} // namespace Game