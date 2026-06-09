#pragma once
#include "cs2/classes/CCSGOInput.h"
#include "core/math/math.h"

namespace Features {
	namespace AntiAim {
		extern QAngle_t g_RealAngles;
		extern QAngle_t g_FakeAngles;
		extern bool g_IsUsing;

		void Run(CUserCmd* pCmd);
	}
}
