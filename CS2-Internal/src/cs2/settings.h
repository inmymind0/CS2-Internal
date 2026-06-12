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

	namespace Chams {
		inline bool Active = false;
		inline bool VisibleActive = false;
		inline int Material = 0;
		inline bool WallhackMode = false;
		inline int WallhackMaterial = 0;
		inline bool Team = false;
		inline float VisibleTeam[4] = { 1.f, 1.f, 1.f, 1.f };
		inline float VisibleEnemy[4] = { 0.15f, 1.f, 0.f, 1.f };
		inline float InvisibleTeam[4] = { 1.f, 1.f, 1.f, 1.f };
		inline float InvisibleEnemy[4] = { 1.f, 0.f, 0.f, 1.f };
	}
}
