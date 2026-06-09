#pragma once
#include "projectsettings.h"
#include <cstdint>

namespace Helpers {
	namespace Log {
		void Console(const char* fmt, ...);
		void Hex(const char* label, uintptr_t val);
	}
}

#if ENABLE_DEV_LOG
#define DEV_LOG(fmt, ...) Helpers::Log::Console(fmt, ##__VA_ARGS__)
#define DEV_LOG_HEX(label, val) Helpers::Log::Hex(label, val)
#else
#define DEV_LOG(fmt, ...)
#define DEV_LOG_HEX(label, val)
#endif
