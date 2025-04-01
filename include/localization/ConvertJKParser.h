#pragma once

struct JKEntry {
    std::string jp;
    std::string kr;
};

using JKTable = std::vector<JKEntry>;

void* ReadEntireFile(const char* filename);

class ConvertJKHandler {
public:
    static void __fastcall Hook(ConvertJKHandler* ecx, void*, const char* filename);

    void ConvertJKParser(const char* filename);

    // Memory layout matches offsets from original reverse engineering
    void* unknown00;       // +0x00
    void* tableStart;      // +0x08
    void* tableEnd;        // +0x0C
    void* unknown10;       // +0x10
    int   allocator[1];    // +0x14 (we use first int here)
    void* blockStart;      // +0x18
    void* blockEnd;        // +0x1C
    void* blockLimit;      // +0x20
};


// Stubbed types and functions
using u32 = unsigned int;
using u8 = unsigned char;

inline void(__stdcall* ProcessData)(int* out) =
reinterpret_cast<void(__stdcall*)(int* out)>(0x0043deb0);

inline int (*Alloc)(size_t bytes) =
    reinterpret_cast<int(*)(size_t)>(0x004a1a3e);

int AllocateMemory(int srcStart, int srcEnd, int dest);

// AddToStructure - 0x0043E850
inline void (*AddToStructure)(int base, int count, int* value) =
reinterpret_cast<void(*)(int, int, int*)>(0x0043e850);

// UpdateStructure - 0x0043E590
inline void(__thiscall* UpdateStructure)(void* ctx, int count, int mode, int* data) =
reinterpret_cast<void(__thiscall*)(void*, int, int, int*)>(0x0043e590);

// UpdateMemory - 0x0043E880
inline void (*UpdateMemory)(int a, int b, int* val) =
reinterpret_cast<void(*)(int, int, int*)>(0x0043e880);

// CleanUpMemory - 0x0043E560
inline void (*CleanUpMemory)(int a, int b) =
reinterpret_cast<void(*)(int, int)>(0x0043e560);

// FreeFileBuffer - 0x80000339
inline void (*FreeFileBuffer)(void* ptr) =
reinterpret_cast<void(*)(void*)>(0x80000339);

// GetNewIndex - 0x00404780
inline int (*GetNewIndex)(void* allocator) =
reinterpret_cast<int(*)(void*)>(0x00404780);

// CFile_Open - 0x80001442
inline void (*CFile_Open)(...) =
reinterpret_cast<void(*)(...)>(0x80001442);

// CFile_GetLength - 0x80000CF6
inline int (*CFile_GetLength)() =
reinterpret_cast<int(*)()>(0x80000cf6);

// CFile_Close - 0x800007BB
inline void (*CFile_Close)() =
reinterpret_cast<void(*)()>(0x800007bb);
