#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <Windows.h>
#include <algorithm>
#include <stdexcept>
#include <cstring>

enum JKTokenType {
    JKTOKEN_CHAR = 0,
    JKTOKEN_BIT = 100,
    JKTOKEN_COLOR = 1,
    JKTOKEN_ICON = 9,
    JKTOKEN_DELAY = 0x11,
    JKTOKEN_FONT = 0x12,
    JKTOKEN_MISC = 7,
    JKTOKEN_CMD = 4
};

struct JKToken {
    JKTokenType type;
    uint32_t value;

    JKToken(JKTokenType t, uint32_t v) : type(t), value(v) {}
};

struct ConvertLine {
    std::vector<JKToken> tokens;
};

extern "C" void __fastcall ConvertJKParser(void* thisPtr, void* dummy, const char* filename);

// Stubbed types and functions
using u32 = unsigned int;
using u8 = unsigned char;

inline int (*Alloc)(size_t bytes) =
    reinterpret_cast<int(*)(size_t)>(0x004a1a3e);

// AddToStructure - 0x0043E850
using AddToStructure_t = void(__stdcall*)(int param1, int param2, int* param3);
inline AddToStructure_t AddToStructure = reinterpret_cast<AddToStructure_t>(0x0043e850);

// UpdateStructure - 0x0043E590
inline void(__thiscall* UpdateStructure)(void* ctx, int count, int mode, int* data) =
reinterpret_cast<void(__thiscall*)(void*, int, int, int*)>(0x0043e590);

// UpdateMemory - 0x0043E880
using UpdateMemory_t = void(__cdecl*)(int start, int end, int sourceCString);
inline UpdateMemory_t UpdateMemory = reinterpret_cast<UpdateMemory_t>(0x0043e880);

// CleanUpMemory - 0x0043E560
using CleanUpMemory_t = void(__stdcall*)(int start, int end);
inline CleanUpMemory_t CleanUpMemory = reinterpret_cast<CleanUpMemory_t>(0x0043e560);

// GetNewIndex - 0x00404780
using GetNewIndex_t = int(__fastcall*)(int ptr);
inline GetNewIndex_t GetNewIndex = reinterpret_cast<GetNewIndex_t>(0x00404780);

