#include "visuals.h"
#include "cs2/settings.h"
#include "cs2/signatures.h"
#include "cs2/helpers/devlog.h"
#include "cs2/client.h"
#include "offsets/offsets.h"
#include "core/math/math.h"
#include <iostream>
#include <algorithm>
#include <Windows.h>

extern uintptr_t g_GetBaseEntityAddr;

void Features::Visuals::RenderESP(ImDrawList* drawList, uintptr_t entityList, uintptr_t localController, uint8_t localTeam, const ViewMatrix& viewMatrix, int width, int height) {
	if (!Settings::Visuals::EspEnabled || !CCS2Client::Get().IsInGame())
		return;

	static bool loggedOnce = false;
	if (!loggedOnce) {
		DEV_LOG("[debug] entity listesi taraniyor...");
		loggedOnce = true;
	}

	for (int i = 1; i <= 1024; ++i) {
		__try {
			uintptr_t entity = 0;
			uintptr_t identity = 0;

			if (g_GetBaseEntityAddr) {
				using GetBaseEntityFn = uintptr_t(__fastcall*)(uintptr_t, int);
				entity = ((GetBaseEntityFn)g_GetBaseEntityAddr)(entityList, i);
				if (entity) {
					identity = *(uintptr_t*)(entity + 0x10);
				}
			} else {
				uintptr_t listEntry = *(uintptr_t*)(entityList + 8 * ((i & 0x7FFF) >> 9) + 16);
				if (listEntry) {
					identity = listEntry + 120 * (i & 0x1FF);
					if (identity) {
						entity = *(uintptr_t*)(identity + 0x0);
					}
				}
			}

			if (!entity || !identity) continue;

			const char* szDesignerName = *(const char**)(identity + 0x20);
			if (!szDesignerName || (uintptr_t)szDesignerName < 0x10000) continue;

			bool isPlayerController = (strcmp(szDesignerName, "cs_player_controller") == 0);

			if (!isPlayerController) continue;

			if (entity == localController) continue;

			uint32_t pawnHandle = *(uint32_t*)(entity + offsets::client_dll::CCSPlayerController::m_hPawn);
			if (!pawnHandle) continue;

			uint8_t team = *(uint8_t*)(entity + offsets::client_dll::C_BaseEntity::m_iTeamNum);

			if (team == localTeam && !Settings::Visuals::EspTeammates) {
				continue;
			}

			int pawnIndex = pawnHandle & 0x7FFF;
			uintptr_t pawnEntity = 0;

			if (g_GetBaseEntityAddr) {
				using GetBaseEntityFn = uintptr_t(__fastcall*)(uintptr_t, int);
				pawnEntity = ((GetBaseEntityFn)g_GetBaseEntityAddr)(entityList, pawnIndex);
			} else {
				uintptr_t pawnListEntry = *(uintptr_t*)(entityList + 8 * ((pawnIndex & 0x7FFF) >> 9) + 16);
				if (pawnListEntry) {
					uintptr_t pawnIdentity = pawnListEntry + 120 * (pawnIndex & 0x1FF);
					if (pawnIdentity) {
						pawnEntity = *(uintptr_t*)(pawnIdentity + 0x0);
					}
				}
			}

			if (!pawnEntity) {
				continue;
			}

			int health = *(int*)(pawnEntity + offsets::client_dll::C_BaseEntity::m_iHealth);
			if (health <= 0 || health > 500) continue;

			uintptr_t gameSceneNode = *(uintptr_t*)(pawnEntity + offsets::client_dll::C_BaseEntity::m_pGameSceneNode);
			if (!gameSceneNode) continue;

			uintptr_t collision = *(uintptr_t*)(pawnEntity + offsets::client_dll::C_BaseEntity::m_pCollision);
			if (!collision) continue;

			Vec3 absOrigin = *(Vec3*)(gameSceneNode + 0xC8);
			Vec3 mins = *(Vec3*)(collision + 0x40);
			Vec3 maxs = *(Vec3*)(collision + 0x4C);

			mins = mins + absOrigin;
			maxs = maxs + absOrigin;

			float minX = 99999.f, minY = 99999.f;
			float maxX = -99999.f, maxY = -99999.f;
			bool anyVisible = false;

			const Vec3 points[8] = {
				Vec3(mins.x, mins.y, mins.z), Vec3(mins.x, maxs.y, mins.z),
				Vec3(maxs.x, maxs.y, mins.z), Vec3(maxs.x, mins.y, mins.z),
				Vec3(maxs.x, maxs.y, maxs.z), Vec3(mins.x, maxs.y, maxs.z),
				Vec3(mins.x, mins.y, maxs.z), Vec3(maxs.x, mins.y, maxs.z)
			};

			for (int p = 0; p < 8; ++p) {
				Vec2 screenPos;
				if (Math::WorldToScreen(points[p], screenPos, viewMatrix, width, height)) {
					minX = (std::min)(minX, screenPos.x);
					minY = (std::min)(minY, screenPos.y);
					maxX = (std::max)(maxX, screenPos.x);
					maxY = (std::max)(maxY, screenPos.y);
					anyVisible = true;
				}
			}

			if (!anyVisible) continue;

			ImColor espColor = (team == 3) ? ImColor(0, 150, 255, 255) : ImColor(255, 100, 0, 255);

			if (Settings::Visuals::BoxEsp) {
				drawList->AddRect(ImVec2(minX - 1.f, minY - 1.f), ImVec2(maxX + 1.f, maxY + 1.f), ImColor(0, 0, 0, 200), 0.f, 0, 1.f);
				drawList->AddRect(ImVec2(minX + 1.f, minY + 1.f), ImVec2(maxX - 1.f, maxY - 1.f), ImColor(0, 0, 0, 200), 0.f, 0, 1.f);
				drawList->AddRect(ImVec2(minX, minY), ImVec2(maxX, maxY), espColor, 0.f, 0, 1.f);
			}

			if (Settings::Visuals::NameEsp) {
				uintptr_t nameAddr = *(uintptr_t*)(entity + offsets::client_dll::CCSPlayerController::m_sSanitizedPlayerName);
				if (nameAddr) {
					const char* szName = (const char*)nameAddr;
					if (szName && szName[0] != '\0' && szName[0] != '?' && (uintptr_t)szName > 0x10000) {
						char cleanName[32] = { 0 };
						int cleanIdx = 0;
						for (int charIdx = 0; szName[charIdx] != '\0' && cleanIdx < 31; ++charIdx) {
							char c = szName[charIdx];
							if (c >= 32 && c <= 126) {
								cleanName[cleanIdx++] = c;
							}
						}
						if (cleanIdx > 0) {
							ImVec2 textSize = ImGui::CalcTextSize(cleanName);
							float textX = minX + (maxX - minX) * 0.5f - textSize.x * 0.5f;
							float textY = minY - textSize.y - 2.f;

							drawList->AddText(ImVec2(textX + 1.f, textY + 1.f), ImColor(0, 0, 0, 255), cleanName);
							drawList->AddText(ImVec2(textX, textY), ImColor(255, 255, 255, 255), cleanName);
						}
					}
				}
			}
		} __except (EXCEPTION_EXECUTE_HANDLER) {
		}
	}
}
