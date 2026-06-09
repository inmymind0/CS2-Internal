#pragma once

#include <string>
#include <cstdint>

namespace Mem {
	uintptr_t PatternScan(const std::string& pattern, const std::string& module);
}
