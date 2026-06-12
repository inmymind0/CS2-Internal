#pragma once

#define GAMEOVERLAYRENDERER64_DLL "GameOverlayRenderer64.dll"
#define CLIENT_DLL "client.dll"
#define ENGINE2_DLL "engine2.dll"
#define SCHEMASYSTEM_DLL "schemasystem.dll"
#define SCENESYSTEM_DLL "scenesystem.dll"
#define MATERIALSYSTEM2_DLL "materialsystem2.dll"
#define TIER0_DLL "tier0.dll"


// gameoverlayrenderer64.dll
#define PRESENT_PATTERN "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 41 8B E8"
#define RESIZEBUFFERS_PATTERN "40 53 55 56 57 41 54 41 56 41 57 48 83 EC ? 44 8B E2"
#define CREATESWAPCHAIN_PATTERN "40 53 55 56 57 48 83 EC ? 48 8B F9 49 8B F1 48 8D 0D ? ? ? ? 49 8B D8 48 8B EA E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B 05 ? ? ? ? 4C 8B CE 4C 8B C3 48 8B D5 48 8B CF FF D0 8B D8 85 C0 78 ? 48 85 F6 74 ? 48 83 3E ? 74 ? 48 8B D5 48 8B CE E8 ? ? ? ? 8B C3 48 83 C4 ? 5F 5E 5D 5B C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC"

// client.dll
#define GETBASEENTITY_PATTERN "4C 8D 49 10 81 FA ? ? 00 00 77 ? 8B CA C1 F9 09"
#define SCREENTRANSFORM_PATTERN "48 89 5C 24 ? 57 48 83 EC ? 48 83 3D ? ? ? ? ? 48 8B DA"
#define CREATEMOVE_PATTERN "48 8B C4 4C 89 40 18 48 89 48 08 55 53 41 54 41 55"
#define VALIDATEINPUT_PATTERN "40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 33 C0 C6 83 ? ? ? ? 00"
#define SETVIEWANGLES_PATTERN "85 D2 75 3D 48 63 81 ? ? ? ?"
#define SETTYPEKV3_PATTERN "40 53 48 83 EC ? ? ? ? 41 B9 ? ? ? ? 49 83 CA"

// engine2.dll
#define ISINGAME_PATTERN "48 8B 05 ? ? ? ? 48 85 C0 74 ? 80 B8 ? ? ? ? 00 75 ? 83 B8 ? ? ? ? ? 7C"

// materialsystem2.dll
#define CREATEMATERIAL_PATTERN "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 8B F2"

// scenesystem.dll
#define ONGENERATEPRIMITIVES_PATTERN "48 8B C4 48 89 58 08 48 89 50 10 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ?"
#define DRAWOBJECT_PATTERN "48 8B C4 53 57 41 54 48 81 EC D0 00 00 00 49 63 F9 49"

// tier0.dll
#define LOADKV3_PROC "?LoadKV3@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEBDAEBUKV3ID_t@@2I@Z"