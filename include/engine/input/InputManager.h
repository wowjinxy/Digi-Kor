#pragma once

#include <windows.h>

namespace Engine {
namespace Input {

class InputManager {
public:
    static bool Initialize();
    static void Shutdown();
    
    // Input state management
    static void UpdateInputState(int currentKeyStates);
    static void ProcessInput(void* gameContext);
    static void InitializeInput(void* context, short param);
    
    // Input processing
    static void UpdateMovementFromInput();
    static void ResetInputQueue(void* context, bool param);
    static int PollNumberKeyAndLookupValue(int playerIndex, int* outMappedValue);
    
    // Macro system
    static void UpdateMacroOverlayState(void* param);
    static int ProcessMacroQueueStep(void* context, uint8_t* param);
    static void UpdateMacroOrDebugRenderState(int param);

private:
    static bool m_initialized;
    static void* m_inputState;
};

// Function declarations from reverse engineering
extern "C" {
    void __stdcall UpdateInputState(int currentKeyStates);
    void __stdcall ProcessInput(void* gameContext);
    void __stdcall InitializeInput(void* context, short param);
    void __stdcall UpdateMovementFromInput();
    void __stdcall ResetInputQueue(void* context, bool param);
    int __stdcall PollNumberKeyAndLookupValue(int playerIndex, int* outMappedValue);
    void __stdcall UpdateMacroOverlayState(void* param);
    int __stdcall ProcessMacroQueueStep(void* context, uint8_t* param);
    void __stdcall UpdateMacroOrDebugRenderState(int param);
}

} // namespace Input
} // namespace Engine