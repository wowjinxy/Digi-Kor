#pragma once

#include <windows.h>
#include <cstdint>

namespace Game {
namespace World {

class MapSystem {
public:
    static bool Initialize();
    static void Shutdown();
    
    // Map loading and processing
    static void LoadMap(int param);
    static void* LoadMapAndProcessBMP(void* context, void* param1, uint32_t param2, uint32_t param3);
    static void ProcessMapGridAndLoadTileTextures(void* param);
    static void LoadDoors(short param);
    
    // Tile management
    static void RemapTileIndicesOrPaletteValues(int param);
    static int GetNewIndex(int param);
    
    // Map utilities
    static void GetMapRotation();

private:
    static bool m_initialized;
    static void* m_currentMap;
};

// Function declarations from reverse engineering
extern "C" {
    void __stdcall loadMap(int param);
    void* __stdcall LoadMapAndProcessBMP(void* context, void* param1, uint32_t param2, uint32_t param3);
    void __stdcall ProcessMapGridAndLoadTileTextures(void* param);
    void __stdcall loadDoors(short param);
    void __stdcall RemapTileIndicesOrPaletteValues(int param);
    int __stdcall GetNewIndex(int param);
    void __stdcall getMapRotation();
}

} // namespace World
} // namespace Game