#pragma once

#include <windows.h>
#include <cstdint>

namespace Engine {
namespace Graphics {

class TextProcessor {
public:
    static bool Initialize();
    static void Shutdown();
    
    // Text processing operations (actual rendering handled by D3D8 shim)
    static void ProcessTextForRender(void* context, void* param1, void* param2, uint32_t param3, int* param4, 
                                   uint32_t param5, uint32_t param6, LPCWSTR param7);
    static void InitializeTextSurface(void* param);
    static int CreateTextRenderSurface(void* context, int* param);

private:
    static bool m_initialized;
};

// Function declarations from reverse engineering
extern "C" {
    void __stdcall InitializeTextSurface(void* param);
    int __stdcall CreateTextRenderSurface(void* context, int* param);
}

} // namespace Graphics
} // namespace Engine