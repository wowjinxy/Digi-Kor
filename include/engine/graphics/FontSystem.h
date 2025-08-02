#pragma once

#include <windows.h>
#include <cstdint>

namespace Engine {
namespace Graphics {

class FontSystem {
public:
    static bool Initialize();
    static void Shutdown();
    
    // Font system management
    static void InitializeFontSystem();
    static void InitializeFontSlots();
    static void ProcessFontsAndMetrics(int* fontMetricsArray);
    
    // Font metrics (no rendering - shim handles that)
    static int InitializeFontMetricsFromLogFont(int* param, LOGFONTA* logFont, void** param3);
    static int ExtractAndProcessFontMetrics(int* fontMetricsArray, HANDLE currentHandle, void** fontMetricsArrayPtr);
    
    // Text measurement only (rendering via shim)
    static void MeasureStringDimensions(void* context, uint32_t param1, int param2, uint32_t param3);
    static void UpdateMenuStringMetrics(void* context, uint32_t menuMode);
    
    // Font object management
    static void InitCFontObject(void* param);

private:
    static bool m_initialized;
    static void* m_fontSlots;
};

// Function declarations from reverse engineering
extern "C" {
    void __stdcall InitializeFontSystem();
    void __stdcall InitializeFontSlots();
    void __stdcall ProcessFontsAndMetrics(int* fontMetricsArray);
    int __stdcall InitializeFontMetricsFromLogFont(int* param, LOGFONTA* logFont, void** param3);
    int __stdcall ExtractAndProcessFontMetrics(int* fontMetricsArray, HANDLE currentHandle, void** fontMetricsArrayPtr);
    void __stdcall MeasureStringDimensions(void* context, uint32_t param1, int param2, uint32_t param3);
    void __stdcall UpdateMenuStringMetrics(void* context, uint32_t menuMode);
    void __stdcall InitCFontObject(void* param);
}

} // namespace Graphics
} // namespace Engine