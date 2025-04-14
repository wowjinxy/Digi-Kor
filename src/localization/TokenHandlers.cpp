#include "localization/TokenHandlers.h"

inline int Atoi(const uint8_t* ptr, size_t len) {
    char buf[16] = { 0 };
    for (size_t i = 0; i < len && i < 15; ++i)
        buf[i] = static_cast<char>(ptr[i]);
    return atoi(buf);
}

void HandleBitToken(std::vector<JKToken>& out) {
    out.push_back({ JKTOKEN_BIT, 0 });
}

void HandleColorToken(std::vector<JKToken>& out, const uint8_t* ptr) {
    int val = Atoi(ptr, 2);
    out.push_back({ JKTOKEN_COLOR, 0 });
    out.push_back({ JKTOKEN_CHAR, static_cast<uint32_t>(val) });
}

void HandleIconToken(std::vector<JKToken>& out, const uint8_t* ptr) {
    int val = Atoi(ptr, 4);
    out.push_back({ JKTOKEN_ICON, 0 });
    out.push_back({ JKTOKEN_CHAR, static_cast<uint32_t>(val) });
}

void HandleDelayToken(std::vector<JKToken>& out, const uint8_t* ptr) {
    int val = Atoi(ptr, 4);
    out.push_back({ JKTOKEN_DELAY, 0 });
    out.push_back({ JKTOKEN_CHAR, static_cast<uint32_t>(val) });
}

void HandleFontToken(std::vector<JKToken>& out, const uint8_t* ptr) {
    int val = Atoi(ptr, 4);
    out.push_back({ JKTOKEN_FONT, 0 });
    out.push_back({ JKTOKEN_CHAR, static_cast<uint32_t>(val) });
}

void HandleMiscToken(std::vector<JKToken>& out, const uint8_t* ptr) {
    int val = Atoi(ptr, 4);
    out.push_back({ JKTOKEN_MISC, 0 });
    out.push_back({ JKTOKEN_CHAR, static_cast<uint32_t>(val) });
}

void HandleRCommandToken(std::vector<JKToken>& out, const uint8_t* ptr) {
    out.push_back({ JKTOKEN_CMD, 0 });
    if (IsDBCSLeadByte(ptr[2]) && ptr[1] == '1') {
        out.push_back({ JKTOKEN_CHAR, 1 });
    }
    else {
        int val = Atoi(&ptr[1], 3);
        out.push_back({ JKTOKEN_CHAR, static_cast<uint32_t>(val) });
    }
}

void HandleDollarToken(std::vector<JKToken>& out, uint8_t cmd) {
    int val = 0x20;
    switch (cmd) {
    case 'B': val = 10; break;
    case 'D': val = 0x13; break;
    case 'E': val = 0x14; break;
    case 'F': val = 0x15; break;
    case 'M': val = 6; break;
    case 'N': val = 5; break;
    case 'P': val = 0x0b; break;
    case 'S': val = 0x10; break;
    }
    out.push_back({ JKTOKEN_CMD, static_cast<uint32_t>(val) });
    out.push_back({ JKTOKEN_CMD, 0x20 });
}

int HandleDefaultChar(std::vector<JKToken>& out, const uint8_t* input, size_t offset, size_t length) {
    out.push_back({ JKTOKEN_CHAR, input[offset] });
    if (IsDBCSLeadByte(input[offset]) && offset + 1 < length) {
        out.push_back({ JKTOKEN_CHAR, static_cast<uint32_t>(input[offset + 1]) });
        return 2;
    }
    return 1;
}
