#include "render/TextRenderer.h"
#include <windows.h>
#include <string>

// Extracts the CString-style string length (stored 8 bytes before data pointer)
int ExtractCStringLength(const char* str) {
    if (!str || reinterpret_cast<uintptr_t>(str) < 8) return -1;

    __try {
        int length = *(int*)(str - 8);
        if (length > 0 && length < 100000) // sanity check
            return length;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return -1;
    }

    return -1;
}

// Attempts to guess text encoding by analyzing byte patterns
int DetectEncoding(const char* input, int length) {
    if (!input || length <= 0) return CP_ACP;

    bool hasUTF8 = false, hasSJIS = false, hasEUC = false, isPureASCII = true;

    for (int i = 0; i < length; ++i) {
        unsigned char c = static_cast<unsigned char>(input[i]);
        if (c >= 0x80) isPureASCII = false;

        if (c >= 0xC2 && c <= 0xF7) hasUTF8 = true;
        else if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xEF)) hasSJIS = true;
        else if (c >= 0xA1 && c <= 0xFE) hasEUC = true;
    }

    if (isPureASCII) return CP_ACP;
    if (hasUTF8) return CP_UTF8;
    if (hasEUC) return 949;   // CP949 for EUC-KR
    if (hasSJIS) return 932;  // CP932 for Shift-JIS

    return CP_ACP;
}

// Converts a multibyte string to wide characters using the guessed code page
std::wstring ConvertToWideString(const char* input, int length, int codePage) {
    int wideLen = MultiByteToWideChar(codePage, 0, input, length, nullptr, 0);
    if (wideLen <= 0) return L"";

    std::wstring result(wideLen, 0);
    MultiByteToWideChar(codePage, 0, input, length, &result[0], wideLen);
    return result;
}

// Replacement for renderString function, hooked into the game
void MeasureStringDimensions(void* renderContext, uint32_t stringParam, int fontIndex, uint32_t extra) {
    HGDIOBJ selectedFont;
    SIZE textSize;

    // Store font index in context
    *(int*)((uint8_t*)renderContext + 0x04) = fontIndex;

    // Select the correct font object from game font table
    selectedFont = SelectObject((HDC)(0x004D8128 + fontIndex * 8), nullptr);

    // Resolve the raw CString pointer from render context
    const char* rawString = *(const char**)((uint8_t*)renderContext + 0x1C);
    int rawLength = ExtractCStringLength(rawString);
    if (rawLength <= 0) return;

    // Try to guess the encoding and convert to wide string
    int codePage = DetectEncoding(rawString, rawLength);
    std::wstring wideText = ConvertToWideString(rawString, rawLength, codePage);

    // Measure text dimensions
    GetTextExtentPoint32W((HDC)0x004D8000, wideText.c_str(), wideText.length(), &textSize);

    // Store measured size back into context
    *(LONG*)((uint8_t*)renderContext + 0x14) = textSize.cx;
    *(LONG*)((uint8_t*)renderContext + 0x18) = textSize.cy;
    *(uint32_t*)((uint8_t*)renderContext + 0x08) = extra;
}

void __fastcall Hook_MeasureStringDimensions(void* ecx, void* /*unused_edx*/, uint32_t param1, int fontIdx, uint32_t param3) {
    // Forward ECX as 'this'
    return MeasureStringDimensions(ecx, param1, fontIdx, param3);
}
