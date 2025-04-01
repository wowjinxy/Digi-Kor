#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <windows.h>
#include "localization/ConvertJKParser.h"

// Reads a whole file into memory as a buffer
void* ReadEntireFile(const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::string msg = std::string("Unable to read file:\n") + filename;
        MessageBoxA(nullptr, msg.c_str(), "Error", MB_ICONERROR);
        return nullptr;
    }

    std::streamsize size = file.tellg();
    if (size <= 0) {
        std::string msg = std::string("File is empty or unreadable:\n") + filename;
        MessageBoxA(nullptr, msg.c_str(), "Error", MB_ICONWARNING);
        return nullptr;
    }

    file.seekg(0, std::ios::beg);
    char* buffer = new char[size + 1];

    if (!file.read(buffer, size)) {
        delete[] buffer;
        std::string msg = std::string("Failed to read contents of:\n") + filename;
        MessageBoxA(nullptr, msg.c_str(), "Error", MB_ICONERROR);
        return nullptr;
    }

    buffer[size] = '\0';
    return static_cast<void*>(buffer);
}

void ConvertJKHandler::ConvertJKParser(const char* filename) {
    if (!tableStart || !tableEnd) {
        return;
    }

    char* fileBuffer = static_cast<char*>(ReadEntireFile(filename));
    if (!fileBuffer) return;

    char lineBuffer[256]{};
    int lineLength = 0;
    int charCounter = 0;
    int totalSize = static_cast<int>(strlen(fileBuffer));
    bool isJP = true;
    int parseState = 0;

    for (; charCounter < totalSize; ++charCounter) {
        if (fileBuffer[charCounter] == '0' &&
            fileBuffer[charCounter + 1] == 'd' &&
            fileBuffer[charCounter + 2] == '0' &&
            fileBuffer[charCounter + 3] == 'a') {

            lineBuffer[lineLength] = '\0';

            if (isJP) {
                // TODO: Compare lineBuffer with something like "C07ジジモンC01"
                // (optional based on game logic)
            }
            else {
                int linePtrJP = reinterpret_cast<int>(lineBuffer);
                int linePtrKR = reinterpret_cast<int>(lineBuffer);

                ProcessData(&linePtrJP);
                ProcessData(&linePtrKR);

                UpdateStructure(reinterpret_cast<char*>(this) + 4, reinterpret_cast<int>(tableEnd), 1, &linePtrJP);

                int curStart = reinterpret_cast<int>(blockStart);
                int curEnd = reinterpret_cast<int>(blockEnd);
                int curLimit = reinterpret_cast<int>(blockLimit);

                if ((curLimit - curEnd) / 4 == 0) {
                    int existing = (curStart != 0) ? (curEnd - curStart) / 4 : 0;
                    int grow = (existing < 2) ? 1 : existing;
                    int total = existing + grow;

                    int newBuffer = Alloc(total * 4);
                    int offset = AllocateMemory(curStart, curEnd, newBuffer);
                    AddToStructure(offset, 1, &linePtrKR);

                    // ✅ Corrected copy: copy old memory to new buffer after offset
                    AllocateMemory(curStart, curEnd, offset + 4);

                    CleanUpMemory(curStart, curEnd);
                    FreeFileBuffer(blockStart);

                    blockLimit = reinterpret_cast<void*>(newBuffer + total * 4);
                    blockStart = reinterpret_cast<void*>(newBuffer);
                    blockEnd = reinterpret_cast<void*>(newBuffer + 4 + GetNewIndex(allocator) * 4);
                }
                else {
                    AllocateMemory(curEnd, curEnd, curEnd + 4);
                    AddToStructure(curEnd, 1 - ((reinterpret_cast<int>(blockEnd) - curEnd) / 4), &linePtrKR);
                    UpdateMemory(curEnd, reinterpret_cast<int>(blockEnd), &linePtrKR);
                    blockEnd = reinterpret_cast<void*>(reinterpret_cast<int>(blockEnd) + 4);
                }
            }

            charCounter += 4;
            lineLength = 0;
            memset(lineBuffer, 0, sizeof(lineBuffer));
            isJP = (parseState == 0);
            parseState = isJP;
        }
        else {
            if (lineLength < 255) {
                lineBuffer[lineLength++] = fileBuffer[charCounter];
            }
        }
    }

    FreeFileBuffer(fileBuffer);
}

void __fastcall ConvertJKHandler::Hook(ConvertJKHandler* ecx, void*, const char* filename) {
    ecx->ConvertJKParser(filename);
}

int AllocateMemory(int srcStart, int srcEnd, int dest)
{
    if (srcStart == srcEnd) return dest;

    while (srcStart != srcEnd) {
        *reinterpret_cast<int*>(dest) = *reinterpret_cast<int*>(srcStart);
        srcStart += 4;
        dest += 4;
    }

    return dest;
}

