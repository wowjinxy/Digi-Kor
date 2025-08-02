#pragma once

#include <windows.h>
#include <dsound.h>
#include <cstdint>

namespace Media {
namespace Audio {

class SoundSystem {
public:
    static bool Initialize();
    static void Shutdown();
    
    // Sound system initialization
    static uint32_t InitSoundSystem(int** param);
    static void LoadSoundFile(void* context, int soundId, uint32_t param);

private:
    static bool m_initialized;
    static IDirectSound8* m_directSound;
};

// Function declarations from reverse engineering
extern "C" {
    uint32_t __stdcall InitSoundSystem(int** param);
    void __stdcall LoadSoundFile(void* context, int soundId, uint32_t param);
}

} // namespace Audio
} // namespace Media