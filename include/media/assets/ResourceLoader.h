#pragma once

#include <windows.h>

namespace Media {
namespace Assets {

class ResourceLoader {
public:
    static bool Initialize();
    static void Shutdown();
    
    // Asset loading
    static void LoadTMDFromFile(void* context, LPCSTR filename);
    static void LoadCardTimImages(void* timManager);

private:
    static bool m_initialized;
};

// Function declarations from reverse engineering
extern "C" {
    void __stdcall LoadTMDFromFile(void* context, LPCSTR filename);
    void __stdcall LoadCardTimImages(void* timManager);
}

} // namespace Assets
} // namespace Media