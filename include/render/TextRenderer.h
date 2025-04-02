#pragma once
#include <windows.h>
#include <string>

// Struct representing the in-memory layout of the string render context
struct RenderContext {
    void* vtable;         // +0x00 - virtual table pointer
    int fontIndex;        // +0x04 - font index
    int someValue;        // +0x08 - possibly alignment/style
    char padding[0x0C];   // +0x0C to +0x14 - unknown/reserved area
    int stringWidth;      // +0x14 - calculated width
    int stringHeight;     // +0x18 - calculated height
    const char* str;      // +0x1C - pointer to CString-style text
};

// Hooked function to replace the game's original text sizing logic
void MeasureStringDimensions(RenderContext* ctx, void*, uint32_t strParam, int fontIdx, uint32_t extraParam);
void __fastcall Hook_MeasureStringDimensions(void* ecx, void* /*unused_edx*/, uint32_t param1, int fontIdx, uint32_t param3);

// MFC-style CDC::SelectObject thunk (0x004a1ccc)
inline HGDIOBJ(*MFC_SelectObject)(HDC hdc, HGDIOBJ obj) =
reinterpret_cast<HGDIOBJ(*)(HDC, HGDIOBJ)>(0x004a1ccc);
