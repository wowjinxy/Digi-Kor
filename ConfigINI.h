#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#pragma once

extern "C" BOOL __fastcall MyShowWindowThunk(void* thisPtr, int /*unusedEDX*/, int nCmdShow);

class ConfigINI {
private:
    std::string filePath;
    std::map<std::string, std::map<std::string, std::string>> sections;
    std::string languagePreference;

    void parse() {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "[ConfigINI] Could not open config: " << filePath << std::endl;
            return;
        }

        std::string line, currentSection;
        while (std::getline(file, line)) {
            // Strip carriage return if present
            if (!line.empty() && line.back() == '\r')
                line.pop_back();

            if (line.empty() || line[0] == ';' || line[0] == '#') continue;

            if (line.front() == '[' && line.back() == ']') {
                currentSection = line.substr(1, line.size() - 2);
            }
            else {
                size_t eqPos = line.find('=');
                if (eqPos != std::string::npos) {
                    std::string key = line.substr(0, eqPos);
                    std::string value = line.substr(eqPos + 1);
                    sections[currentSection][key] = value;

                    if (currentSection == "Localization" && _stricmp(key.c_str(), "Language") == 0) {
                        languagePreference = value;
                    }
                }
            }
        }
    }

public:
    ConfigINI(const std::string& path) : filePath(path) {
        parse();
    }

    std::string get(const std::string& section, const std::string& key, const std::string& defaultVal = "") const {
        auto secIt = sections.find(section);
        if (secIt != sections.end()) {
            auto keyIt = secIt->second.find(key);
            if (keyIt != secIt->second.end()) {
                return keyIt->second;
            }
        }
        return defaultVal;
    }

    int getInt(const std::string& section, const std::string& key, int defaultVal = 0) const {
        std::string val = get(section, key);
        return val.empty() ? defaultVal : std::stoi(val);
    }

    bool getBool(const std::string& section, const std::string& key, bool defaultVal = false) const {
        std::string val = get(section, key);
        if (val.empty()) return defaultVal;
        return (val == "1" || val == "true" || val == "True" || val == "TRUE");
    }

    std::string getLanguagePreference() const {
        return languagePreference;
    }

    std::string getLanguageColumn(const std::vector<std::string>& headers) const {
        // Match the saved languagePreference case-insensitively to one of the headers
        for (const std::string& header : headers) {
            if (_stricmp(header.c_str(), languagePreference.c_str()) == 0) {
                return header;
            }
        }
        return headers.empty() ? "" : headers[1]; // fallback to first column
    }
};