#pragma once

#define PRESENT_PATTERN "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 41 8B E8"
#define CREATEMOVE_PATTERN "48 8B C4 4C 89 40 18 48 89 48 08 55 53 41 54 41 55"
#define VALIDATEINPUT_PATTERN "40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 33 C0 C6 83 ? ? ? ? 00"
#define SETVIEWANGLES_PATTERN "85 D2 75 3D 48 63 81 ? ? ? ?"
#define RESIZEBUFFERS_PATTERN "40 53 55 56 57 41 54 41 56 41 57 48 83 EC ? 44 8B E2"
#define CREATESWAPCHAIN_PATTERN "40 53 55 56 57 48 83 EC ? 48 8B F9 49 8B F1 48 8D 0D ? ? ? ? 49 8B D8 48 8B EA E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B 05 ? ? ? ? 4C 8B CE 4C 8B C3 48 8B D5 48 8B CF FF D0 8B D8 85 C0 78 ? 48 85 F6 74 ? 48 83 3E ? 74 ? 48 8B D5 48 8B CE E8 ? ? ? ? 8B C3 48 83 C4 ? 5F 5E 5D 5B C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC"

namespace hooks {
	bool Init();
	void Shutdown();

	bool SetupPresentHook();
	bool SetupResizeBuffersHook();
	bool SetupCreateSwapChainHook();
	bool SetupCreateMoveHook();
	bool SetupValidateInputHook();
}