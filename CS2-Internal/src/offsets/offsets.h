#pragma once
#include <cstdint>

namespace offsets {
    namespace client_dll {
        constexpr uintptr_t dwLocalPlayerController = 0x2320720;
        constexpr uintptr_t dwLocalPlayerPawn = 0x2341698;
        constexpr uintptr_t dwEntityList = 0x24E76A0;
        constexpr uintptr_t dwViewMatrix = 0x2346B30;

        namespace CCSPlayerController {
            constexpr uintptr_t m_bPawnIsAlive = 0x914;
            constexpr uintptr_t m_sSanitizedPlayerName = 0x860;
            constexpr uintptr_t m_hPawn = 0x6BC;
        }

        namespace C_BaseEntity {
            constexpr uintptr_t m_MoveType = 0x525;
            constexpr uintptr_t m_iHealth = 0x34C;
            constexpr uintptr_t m_iTeamNum = 0x3EB;
            constexpr uintptr_t m_pCollision = 0x340;
            constexpr uintptr_t m_pGameSceneNode = 0x330;
        }
    }
}
