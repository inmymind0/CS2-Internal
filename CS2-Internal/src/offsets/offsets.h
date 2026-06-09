#pragma once
#include <cstdint>

namespace offsets {
    namespace client_dll {
        constexpr uintptr_t dwLocalPlayerController = 0x231F700;
        constexpr uintptr_t dwLocalPlayerPawn = 0x2340698;

        namespace CCSPlayerController {
            constexpr uintptr_t m_bPawnIsAlive = 0x914;
        }

        namespace C_BaseEntity {
            constexpr uintptr_t m_MoveType = 0x525;
        }
    }
}
