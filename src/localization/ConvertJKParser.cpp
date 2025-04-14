#include "localization/TokenHandlers.h"
#include "localization/ConvertJKParser.h"
#include <vector>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <windows.h>

// Utility to load file into memory like original ReadEntireFile
void* ReadEntireFile(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Unable to read %s.", filename);
        MessageBoxA(nullptr, buffer, "Digimon World", MB_OK | MB_ICONERROR);
        return nullptr;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    void* buffer = malloc(size);
    if (!buffer) {
        fclose(f);
        return nullptr;
    }

    fread(buffer, 1, size, f);
    fclose(f);
    return buffer;
}

static bool IsLeadByte(uint8_t byte) {
    return ::IsDBCSLeadByte(byte); // Use WinAPI to detect DBCS
}

std::vector<JKToken> ProcessData(const uint8_t* input, size_t length) {
    std::vector<JKToken> tokens;
    size_t offset = 0;

    while (offset < length) {
        uint8_t ch = input[offset];

        if (ch == '{' && offset + 1 < length && input[offset + 1] == '}') {
            HandleBitToken(tokens);
            offset += 2;
        }
        else if (ch == 'C' && offset + 2 < length) {
            HandleColorToken(tokens, &input[offset + 1]);
            offset += 3;
        }
        else if (ch == 'I' && offset + 4 < length) {
            HandleIconToken(tokens, &input[offset + 1]);
            offset += 5;
        }
        else if (ch == 'D' && offset + 4 < length) {
            HandleDelayToken(tokens, &input[offset + 1]);
            offset += 5;
        }
        else if (ch == 'F' && offset + 4 < length) {
            HandleFontToken(tokens, &input[offset + 1]);
            offset += 5;
        }
        else if (ch == 'M' && offset + 4 < length) {
            HandleMiscToken(tokens, &input[offset + 1]);
            offset += 5;
        }
        else if (ch == 'R' && offset + 3 < length) {
            HandleRCommandToken(tokens, &input[offset]);
            offset += 4;
        }
        else if (ch == '$' && offset + 1 < length) {
            HandleDollarToken(tokens, input[offset + 1]);
            offset += 2;
        }
        else {
            size_t step = HandleDefaultChar(tokens, input, offset, length);
            if (step == 0) step = 1;
            offset += step;
        }
    }

    return tokens;
}

extern "C" void __fastcall ConvertJKParser(void* thisPtr, void*, const char* filename);

void __fastcall ConvertJKParser(void* thisPtr, void*, const char* filename) {
    printf("[ConvertJKParser] Starting parse for: %s\n", filename);

    // === Load file into heap memory (stored in this + 0xAA4) ===
    void* fileMem = ReadEntireFile(filename);
    if (!fileMem) {
        printf(" ! Failed to read file into memory\n");
        return;
    }

    *(void**)((uint8_t*)thisPtr + 0xAA4) = fileMem;

    // === Validate expected internal memory layout ===
    int* begin1 = *(int**)((uint8_t*)thisPtr + 0x08);
    int* end1 = *(int**)((uint8_t*)thisPtr + 0x0C);
    int* begin2 = *(int**)((uint8_t*)thisPtr + 0x18);
    int* end2 = *(int**)((uint8_t*)thisPtr + 0x1C);

    printf(" - begin1: %p, end1: %p\n", (void*)begin1, (void*)end1);
    printf(" - begin2: %p, end2: %p\n", (void*)begin2, (void*)end2);

    if (!begin1 || !end1 || ((end1 - begin1) == 0)) {
        printf(" ! Validation failed: (end1 - begin1) == 0 or null\n");
        return;
    }
    if (begin2 && end2 && ((end2 - begin2) != 0)) {
        printf(" ! Validation failed: (end2 - begin2) != 0 (should be empty)\n");
        return;
    }

    // === Construct byte buffer from raw memory ===
    uint8_t* fileStart = static_cast<uint8_t*>(fileMem);
    size_t fileLength = strlen((char*)fileMem); // This is *not* ideal for binary data!

    // If you know the correct size from disk, use it instead of strlen:
    // std::ifstream f(filename, std::ios::binary | std::ios::ate);
    // size_t fileLength = f.tellg(); // <== better!

    std::vector<uint8_t> buffer(fileStart, fileStart + fileLength);

    const uint8_t marker[] = { '0', 'd', '0', 'a' }; // == 0x30, 0x64, 0x30, 0x61
    size_t offset = 0;
    int lineIndex = 0;

    while (offset < buffer.size()) {
        auto it = std::search(buffer.begin() + offset, buffer.end(),
            std::begin(marker), std::end(marker));
        if (it == buffer.end())
            break;

        size_t next = std::distance(buffer.begin(), it);

        // Create a null-terminated line buffer for tokenization
        std::vector<uint8_t> raw(buffer.begin() + offset, buffer.begin() + next);
        raw.push_back(0); // null-terminate

        std::vector<JKToken> JKTokens = ProcessData(raw.data(), raw.size());

        // Convert to flat int array
        std::vector<int> packed(JKTokens.size() * 2);
        for (size_t i = 0; i < JKTokens.size(); ++i) {
            packed[i * 2 + 0] = JKTokens[i].type;
            packed[i * 2 + 1] = static_cast<int>(JKTokens[i].value);
        }

        // Choose destination memory range (even: begin1 / 0x18, odd: begin2 / 0x1C)
        int baseOffset = (lineIndex % 2 == 0) ? 0x18 : 0x1C;
        int* basePtr = *(int**)((uint8_t*)thisPtr + baseOffset);

        if (!basePtr) {
            printf(" ! Skipping line %d: base at 0x%X is null\n", lineIndex, baseOffset);
        }
        else {
            AddToStructure((int)basePtr, 1, packed.data());
            printf(" + Added %zu tokens to base 0x%08X\n", JKTokens.size(), (unsigned int)(uintptr_t)basePtr);
        }

        offset = next + sizeof(marker);
        ++lineIndex;
    }
}