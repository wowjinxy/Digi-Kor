#include "localization/ConvertJKParser.h"

static bool IsLeadByte(uint8_t byte) {
    return ::IsDBCSLeadByte(byte); // use :: to force global scope resolution
}


std::vector<JKToken> ProcessData(const uint8_t* input, size_t length) {
    std::vector<JKToken> JKTokens;
    size_t offset = 0;

    auto insertJKToken = [&](JKTokenType type, uint32_t val = 0) {
        JKTokens.push_back({ type, val });
        };

    auto readIntFrom = [&](const uint8_t* ptr, size_t len) -> uint32_t {
        char buf[16] = { 0 };
        for (size_t i = 0; i < len && i < 15; ++i)
            buf[i] = static_cast<char>(ptr[i]);
        return static_cast<uint32_t>(atoi(buf));
        };

    while (offset < length) {
        uint8_t ch = input[offset];

        if (ch == '{' && offset + 1 < length && input[offset + 1] == '}') {
            insertJKToken(JKTOKEN_BIT);
            offset += 2;
            continue;
        }

        if (ch == 'C' && offset + 2 < length) {
            insertJKToken(JKTOKEN_COLOR);
            uint32_t val = readIntFrom(&input[offset + 1], 2);
            insertJKToken(JKTOKEN_CHAR, val);
            offset += 3;
            continue;
        }

        if (ch == 'I' && offset + 4 < length) {
            insertJKToken(JKTOKEN_ICON);
            uint32_t val = readIntFrom(&input[offset + 1], 4);
            insertJKToken(JKTOKEN_CHAR, val);
            offset += 5;
            continue;
        }

        if (ch == 'D' && offset + 4 < length) {
            insertJKToken(JKTOKEN_DELAY);
            uint32_t val = readIntFrom(&input[offset + 1], 4);
            insertJKToken(JKTOKEN_CHAR, val);
            offset += 5;
            continue;
        }

        if (ch == 'F' && offset + 4 < length) {
            insertJKToken(JKTOKEN_FONT);
            uint32_t val = readIntFrom(&input[offset + 1], 4);
            insertJKToken(JKTOKEN_CHAR, val);
            offset += 5;
            continue;
        }

        if (ch == 'M' && offset + 4 < length) {
            insertJKToken(JKTOKEN_MISC);
            uint32_t val = readIntFrom(&input[offset + 1], 4);
            insertJKToken(JKTOKEN_CHAR, val);
            offset += 5;
            continue;
        }

        if (ch == 'R' && offset + 2 < length) {
            uint8_t lead = input[offset + 2];
            if (IsLeadByte(lead)) {
                if (input[offset + 1] == '1') {
                    insertJKToken(JKTOKEN_CMD);
                    insertJKToken(JKTOKEN_CHAR, 1);
                    offset += 2;
                }
            }
            else {
                insertJKToken(JKTOKEN_CMD);
                uint32_t val = readIntFrom(&input[offset + 1], 3);
                insertJKToken(JKTOKEN_CHAR, val);
                offset += 4;
            }
            continue;
        }

        if (ch == '$' && offset + 1 < length) {
            uint8_t cmd = input[offset + 1];
            uint32_t val = 0x20;

            switch (cmd) {
            case 'B': val = 10; break;
            case 'D': val = 0x13; break;
            case 'E': val = 0x14; break;
            case 'F': val = 0x15; break;
            case 'M': val = 6; break;
            case 'N': val = 5; break;
            case 'P': val = 0x0b; break;
            case 'S': val = 0x10; break;
            default: break;
            }
            insertJKToken(JKTOKEN_CMD, val);
            insertJKToken(JKTOKEN_CMD, 0x20);
            offset += 2;
            continue;
        }

        insertJKToken(JKTOKEN_CHAR, ch);
        offset += 1;

        if (IsLeadByte(ch) && offset < length) {
            insertJKToken(JKTOKEN_CHAR, input[offset]);
            offset += 1;
        }
    }

    return JKTokens;
}

void __fastcall ConvertJKParser(void* thisPtr, void*, const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return;

    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    const uint8_t marker[] = { 0x30, 0x64, 0x30, 0x61 };
    size_t offset = 0;
    int lineIndex = 0;

    while (offset < buffer.size()) {
        auto it = std::search(buffer.begin() + offset, buffer.end(),
            std::begin(marker), std::end(marker));
        if (it == buffer.end()) break;

        size_t next = std::distance(buffer.begin(), it);
        std::vector<uint8_t> raw(buffer.begin() + offset, buffer.begin() + next);
        std::vector<JKToken> JKTokens = ProcessData(raw.data(), raw.size());

        // Allocate temporary array and write into game memory using AddToStructure
        std::vector<int> packed(JKTokens.size() * 2);
        for (size_t i = 0; i < JKTokens.size(); ++i) {
            packed[i * 2 + 0] = JKTokens[i].type;
            packed[i * 2 + 1] = JKTokens[i].value;
        }

        int* dataPtr = packed.data();
        int base = *(int*)((uint8_t*)thisPtr + (lineIndex % 2 == 0 ? 0x18 : 0x1c));
        AddToStructure(base, 1, dataPtr);

        offset = next + 4;
        ++lineIndex;
    }
}
