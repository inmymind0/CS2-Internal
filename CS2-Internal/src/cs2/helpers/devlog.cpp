#include "devlog.h"
#include <cstdarg>
#include <cstdio>

namespace Helpers {
	namespace Log {
		void Console(const char* fmt, ...) {
			#if ENABLE_DEV_LOG
			va_list args;
			va_start(args, fmt);
			std::vprintf(fmt, args);
			std::putchar('\n');
			va_end(args);
			#endif
		}

		void Hex(const char* label, uintptr_t val) {
			#if ENABLE_DEV_LOG
			std::printf("%s0x%I64X\n", label, (unsigned __int64)val);
			#endif
		}
	}
}
