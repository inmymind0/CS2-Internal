#pragma once

namespace Settings {
	namespace AntiAim {
		inline bool Enabled = false;
		inline int PitchMode = 1;
		inline int YawMode = 1;
		inline float YawOffset = 0.f;
		inline float Roll = 0.f;
	}

	namespace Visuals {
		inline bool EspEnabled = true;
		inline bool BoxEsp = true;
		inline bool NameEsp = true;
		inline bool EspTeammates = false;
	}
}
