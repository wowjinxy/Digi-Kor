#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <string> 
#include "ConvertJKParser.h"

void HandleBitToken(std::vector<JKToken>& out);
void HandleColorToken(std::vector<JKToken>& out, const uint8_t* ptr);
void HandleIconToken(std::vector<JKToken>& out, const uint8_t* ptr);
void HandleDelayToken(std::vector<JKToken>& out, const uint8_t* ptr);
void HandleFontToken(std::vector<JKToken>& out, const uint8_t* ptr);
void HandleMiscToken(std::vector<JKToken>& out, const uint8_t* ptr);
void HandleRCommandToken(std::vector<JKToken>& out, const uint8_t* ptr);
void HandleDollarToken(std::vector<JKToken>& out, uint8_t cmd);
int  HandleDefaultChar(std::vector<JKToken>& out, const uint8_t* input, size_t offset, size_t length);
