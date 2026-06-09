#include "mem.h"

#include <Windows.h>
#include <Psapi.h>
#include <vector>

std::vector<std::pair<uint8_t, bool>> PatternToBytes(const std::string& pattern) {
    std::vector<std::pair<uint8_t, bool>> patternBytes;
    const char* start = pattern.c_str();
    const char* end = start + pattern.size();

    for (const char* current = start; current < end; ++current) {
        if (*current == ' ') continue;
        if (*current == '?') {
            patternBytes.emplace_back(0, false);
            if (*(current + 1) == '?') ++current;
        }
        else {
            patternBytes.emplace_back(strtoul(current, nullptr, 16), true);
            if (*(current + 1) != ' ') ++current;
        }
    }

    return patternBytes;
}

uintptr_t Mem::PatternScan(const std::string& pattern, const std::string& modStr) {
    if (modStr.empty())
        return 0;

    auto mod = GetModuleHandle(modStr.c_str());
    if (!mod)
        return 0;

    MODULEINFO modInfo;
    GetModuleInformation(GetCurrentProcess(), mod, &modInfo, sizeof(MODULEINFO));

    size_t moduleSize = modInfo.SizeOfImage;
    uintptr_t moduleBase = (uintptr_t)modInfo.lpBaseOfDll;

    std::vector<std::pair<uint8_t, bool>> patternBytes = PatternToBytes(pattern);
    size_t patternLength = patternBytes.size();

    for (size_t i = 0; i < moduleSize - patternLength; ++i) {
        bool found = true;
        for (size_t j = 0; j < patternLength; ++j) {
            if (patternBytes[j].second && patternBytes[j].first != *reinterpret_cast<uint8_t*>(moduleBase + i + j)) {
                found = false;
                break;
            }
        }
        if (found) {
            return moduleBase + i;
        }
    }

    return 0;
}