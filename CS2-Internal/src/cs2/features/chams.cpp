#include "chams.h"
#include <Windows.h>
#include "cs2/settings.h"
#include "cs2/hooks/Hook_DrawObject.h"
#include "cs2/hooks/Hook_OnGeneratePrimitives.h"
#include "cs2/helpers/devlog.h"
#include "offsets/offsets.h"
#include "cs2/signatures.h"
#include "core/mem/mem.h"

#include <vector>
#include <algorithm>
#include <optional>

static fn_LoadKV3_t g_fnLoadKV3 = nullptr;
static fn_SetTypeKV3_t g_fnSetTypeKV3 = nullptr;
static fn_CreateMaterial_t g_fnCreateMaterial = nullptr;

extern uintptr_t g_GetBaseEntityAddr;

namespace Features {
	namespace Chams {
		static ChamsMaterialPair_t m_Materials[CHAMS_MATERIAL_COUNT]{};
		static bool m_bInitialized = false;

		template <typename T>
		static constexpr const T& Clamp(const T& v, const T& lo, const T& hi) {
			return (v < lo) ? lo : (hi < v) ? hi : v;
		}

		static auto InitFunctionPointers() -> bool {
			const HMODULE hTier0 = GetModuleHandleA(TIER0_DLL);
			if (!hTier0) return false;

			g_fnLoadKV3 = reinterpret_cast<fn_LoadKV3_t>(
				GetProcAddress(hTier0, LOADKV3_PROC)
			);

			g_fnSetTypeKV3 = reinterpret_cast<fn_SetTypeKV3_t>(
				Mem::PatternScan(SETTYPEKV3_PATTERN, CLIENT_DLL)
			);

			g_fnCreateMaterial = reinterpret_cast<fn_CreateMaterial_t>(
				Mem::PatternScan(CREATEMATERIAL_PATTERN, MATERIALSYSTEM2_DLL)
			);

			return (g_fnLoadKV3 && g_fnSetTypeKV3 && g_fnCreateMaterial);
		}

		static CMaterial2* CreateMaterial(const char* szName, const char* szVmatBuffer) {
			if (!g_fnSetTypeKV3 || !g_fnLoadKV3 || !g_fnCreateMaterial)
				return nullptr;

			void* pKV3 = reinterpret_cast<void*>(new uint8_t[0x10 * 0x120]{});
			g_fnSetTypeKV3(pKV3, 1U, 6U);

			kv3_id_t kv3ID("generic", 0x41B818518343427Eull, 0xB5F447C23C0CDF8Cull);
			g_fnLoadKV3(pKV3, nullptr, szVmatBuffer, &kv3ID, nullptr, 0);

			CMaterialStrongHandle_t handle{};
			g_fnCreateMaterial(nullptr, &handle, szName, pKV3, 0, 1);

			delete[] reinterpret_cast<uint8_t*>(pKV3);

			if (!handle.binding || !handle.binding->data)
				return nullptr;

			return reinterpret_cast<CMaterial2*>(handle.binding->data);
		}

		bool Initialize() {
			if (m_bInitialized) return true;

			if (!InitFunctionPointers()) {
				DEV_LOG("[-] [chams] fonksiyon isaretcileri bulunamadi");
				return false;
			}

			__try {
				m_Materials[CHAMS_MATERIAL_GENERIC] = {
					CreateMaterial("materials/dev/chams_generic_vis.vmat", ChamsMaterials::szVMatGeneric_Visible),
					CreateMaterial("materials/dev/chams_generic_invis.vmat", ChamsMaterials::szVMatGeneric_Invisible)
				};
				m_Materials[CHAMS_MATERIAL_UNLIT] = {
					CreateMaterial("materials/dev/chams_unlit_vis.vmat", ChamsMaterials::szVMatUnlit_Visible),
					CreateMaterial("materials/dev/chams_unlit_invis.vmat", ChamsMaterials::szVMatUnlit_Invisible)
				};
				m_Materials[CHAMS_MATERIAL_SOLID] = {
					CreateMaterial("materials/dev/chams_solid_vis.vmat", ChamsMaterials::szVMatSolid_Visible),
					CreateMaterial("materials/dev/chams_solid_invis.vmat", ChamsMaterials::szVMatSolid_Invisible)
				};
				m_Materials[CHAMS_MATERIAL_LATEX] = {
					CreateMaterial("materials/dev/chams_latex_vis.vmat", ChamsMaterials::szVMatCharacter_Visible),
					CreateMaterial("materials/dev/chams_latex_invis.vmat", ChamsMaterials::szVMatCharacter_Invisible)
				};
			} __except (EXCEPTION_EXECUTE_HANDLER) {
				DEV_LOG("[-] [chams] materyal baslatilirken istisna olustu");
				return false;
			}

			if (!m_Materials[CHAMS_MATERIAL_GENERIC].m_pVisible || !m_Materials[CHAMS_MATERIAL_GENERIC].m_pInvisible) {
				DEV_LOG("[-] [chams] materyaller yuklenemedi");
				return false;
			}

			m_bInitialized = true;
			DEV_LOG("[+] [chams] basariyla baslatildi");
			return true;
		}

		void Shutdown() {
			m_bInitialized = false;
		}

		static auto TryEntity(uintptr_t pEnt) -> uintptr_t {
			if (!pEnt) return 0;

			using GetSchemaClassInfoFn = void* (__fastcall*)(void*, void**);
			void* pBinding = nullptr;
			void* vtable = *(void**)pEnt;
			if (!vtable) return 0;
			void* fn = ((void**)vtable)[46];
			if (!fn) return 0;

			reinterpret_cast<GetSchemaClassInfoFn>(fn)((void*)pEnt, &pBinding);
			if (!pBinding) return 0;

			const char* className = *reinterpret_cast<const char**>(reinterpret_cast<uintptr_t>(pBinding) + 0x8);
			if (!className || (uintptr_t)className < 0x10000) return 0;

			static int loggedCount = 0;
			if (loggedCount < 5) {
				DEV_LOG("[debug] chams sinif tespiti: %s", className);
				loggedCount++;
			}

			bool isPlayer = (strcmp(className, "C_CSPlayerPawn") == 0);
			bool isRagdoll = (strcmp(className, "C_CSRagdoll") == 0);

			if (!isPlayer && !isRagdoll)
				return 0;

			if (isPlayer) {
				int health = *(int*)(pEnt + offsets::client_dll::C_BaseEntity::m_iHealth);
				if (health <= 0)
					return 0;
			}

			return pEnt;
		}

		static auto TryResolvePlayerPawnFromSAO(CSceneAnimatableObject* pSAO) -> uintptr_t {
			if (!pSAO || pSAO->m_hOwnerIndex == 0xFFFFFFFF)
				return 0;

			uintptr_t clientBase = (uintptr_t)GetModuleHandleA(CLIENT_DLL);
			if (!clientBase) return 0;

			uintptr_t entityList = *(uintptr_t*)(clientBase + offsets::client_dll::dwEntityList);
			if (!entityList) return 0;

			const int entryIdx = pSAO->m_hOwnerIndex & 0x7FFF;
			uintptr_t pawnEntity = 0;

			if (g_GetBaseEntityAddr) {
				using GetBaseEntityFn = uintptr_t(__fastcall*)(uintptr_t, int);
				pawnEntity = ((GetBaseEntityFn)g_GetBaseEntityAddr)(entityList, entryIdx);
			} else {
				uintptr_t listEntry = *(uintptr_t*)(entityList + 8 * (entryIdx >> 9) + 16);
				if (listEntry) {
					uintptr_t identity = listEntry + 120 * (entryIdx & 0x1FF);
					if (identity) {
						pawnEntity = *(uintptr_t*)(identity + 0x0);
					}
				}
			}

			uintptr_t pResolvedPawn = TryEntity(pawnEntity);

			if (pResolvedPawn) {
				uint32_t* pFlags = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(pSAO) + 0x78);
				if (pFlags && (*pFlags & (1 << 3))) {
					*pFlags &= ~(1 << 3);
				}
			}

			return pResolvedPawn;
		}

		static auto TryResolvePlayerPawnFromMesh(CMeshData* pMeshData, int nDataCount) -> uintptr_t {
			if (!pMeshData || nDataCount <= 0)
				return 0;

			for (int i = 0; i < nDataCount; ++i) {
				if (auto pPawn = TryResolvePlayerPawnFromSAO(pMeshData[i].m_pSceneAnimatableObject))
					return pPawn;
			}

			return 0;
		}

		struct ChamsTarget_t {
			bool bIsTeam = false;
			CMaterial2* m_pVisMat = nullptr;
			CMaterial2* m_pWallMat = nullptr;
			float visColor[4]{};
			float invisColor[4]{};
		};

		static auto BuildChamsTarget(uintptr_t pPawn, const ChamsMaterialPair_t* pMaterials) -> std::optional<ChamsTarget_t> {
			if (!pPawn || !pMaterials)
				return std::nullopt;

			uintptr_t clientBase = (uintptr_t)GetModuleHandleA(CLIENT_DLL);
			if (!clientBase) return std::nullopt;

			uintptr_t localController = *(uintptr_t*)(clientBase + offsets::client_dll::dwLocalPlayerController);
			if (!localController) return std::nullopt;

			uint32_t localPawnHandle = *(uint32_t*)(localController + offsets::client_dll::CCSPlayerController::m_hPawn);
			if (localPawnHandle == 0xFFFFFFFF) return std::nullopt;

			uintptr_t entityList = *(uintptr_t*)(clientBase + offsets::client_dll::dwEntityList);
			if (!entityList) return std::nullopt;

			const int localPawnEntryIdx = localPawnHandle & 0x7FFF;
			uintptr_t pLocalPawn = 0;

			if (g_GetBaseEntityAddr) {
				using GetBaseEntityFn = uintptr_t(__fastcall*)(uintptr_t, int);
				pLocalPawn = ((GetBaseEntityFn)g_GetBaseEntityAddr)(entityList, localPawnEntryIdx);
			} else {
				uintptr_t listEntry = *(uintptr_t*)(entityList + 8 * (localPawnEntryIdx >> 9) + 16);
				if (listEntry) {
					uintptr_t identity = listEntry + 120 * (localPawnEntryIdx & 0x1FF);
					if (identity) {
						pLocalPawn = *(uintptr_t*)(identity + 0x0);
					}
				}
			}

			if (!pLocalPawn || pPawn == pLocalPawn)
				return std::nullopt;

			ChamsTarget_t target{};
			uint8_t pawnTeam = *(uint8_t*)(pPawn + offsets::client_dll::C_BaseEntity::m_iTeamNum);
			uint8_t localTeam = *(uint8_t*)(pLocalPawn + offsets::client_dll::C_BaseEntity::m_iTeamNum);
			target.bIsTeam = (pawnTeam == localTeam);

			if (target.bIsTeam && !Settings::Chams::Team)
				return std::nullopt;

			if (!Settings::Chams::VisibleActive && !Settings::Chams::WallhackMode)
				return std::nullopt;

			if (Settings::Chams::VisibleActive) {
				const int visIdx = Clamp(Settings::Chams::Material, 0, (int)CHAMS_MATERIAL_COUNT - 1);
				target.m_pVisMat = pMaterials[visIdx].m_pVisible;
				const float* srcCol = target.bIsTeam ? Settings::Chams::VisibleTeam : Settings::Chams::VisibleEnemy;
				memcpy(target.visColor, srcCol, sizeof(target.visColor));
			}

			if (Settings::Chams::WallhackMode) {
				const int wallIdx = Clamp(Settings::Chams::WallhackMaterial, 0, (int)CHAMS_MATERIAL_COUNT - 1);
				target.m_pWallMat = pMaterials[wallIdx].m_pInvisible;
				const float* srcCol = target.bIsTeam ? Settings::Chams::InvisibleTeam : Settings::Chams::InvisibleEnemy;
				memcpy(target.invisColor, srcCol, sizeof(target.invisColor));
			}

			return target;
		}

		static auto ApplyColorToMesh(MeshColor_t& color, const float* col) -> void {
			color.r = static_cast<uint8_t>(col[0] * 255.f);
			color.g = static_cast<uint8_t>(col[1] * 255.f);
			color.b = static_cast<uint8_t>(col[2] * 255.f);
			color.a = static_cast<uint8_t>(col[3] * 255.f);
		}

		static bool OnDrawObjectImpl(
			void* pDesc,
			void* pDX11,
			CMeshData* pMeshData,
			int nDataCount,
			void* pView,
			void* pLayer,
			void* pUnk,
			void* pUnk2
		) {
			if (!Settings::Chams::Active)
				return false;

			auto pPawn = TryResolvePlayerPawnFromMesh(pMeshData, nDataCount);
			if (!pPawn)
				return false;

			const auto target = BuildChamsTarget(pPawn, m_Materials);
			if (!target)
				return false;

			auto ApplyToAll = [&](CMaterial2* pMat, const float* col) {
				for (int i = 0; i < nDataCount; ++i) {
					pMeshData[i].m_pMaterial = pMat;
					pMeshData[i].m_pMaterial2 = pMat;
					ApplyColorToMesh(pMeshData[i].m_Color, col);
				}
			};

			if (Settings::Chams::WallhackMode && target->m_pWallMat) {
				ApplyToAll(target->m_pWallMat, target->invisColor);
				oDrawObject(pDesc, pDX11, pMeshData, nDataCount, pView, pLayer, pUnk, pUnk2);
			}

			if (target->m_pVisMat) {
				ApplyToAll(target->m_pVisMat, target->visColor);
				oDrawObject(pDesc, pDX11, pMeshData, nDataCount, pView, pLayer, pUnk, pUnk2);
			}

			return true;
		}

		bool OnDrawObject(
			void* pDesc,
			void* pDX11,
			CMeshData* pMeshData,
			int nDataCount,
			void* pView,
			void* pLayer,
			void* pUnk,
			void* pUnk2
		) {
			if (!m_bInitialized || !pMeshData || nDataCount <= 0)
				return false;

			__try {
				return OnDrawObjectImpl(pDesc, pDX11, pMeshData, nDataCount, pView, pLayer, pUnk, pUnk2);
			} __except (EXCEPTION_EXECUTE_HANDLER) {
				return false;
			}
		}

		static bool OnGeneratePrimitivesImpl(
			void* pSceneSys,
			CSceneAnimatableObject* pSceneObject,
			void* pUnk,
			CMeshPrimitiveOutputBuffer_t* pBuf
		) {
			if (!Settings::Chams::Active)
				return false;

			auto pPawn = TryResolvePlayerPawnFromSAO(pSceneObject);
			if (!pPawn) return false;

			const auto target = BuildChamsTarget(pPawn, m_Materials);
			if (!target) return false;

			auto ApplyPrimitiveRange = [&](int iFrom, int iTo, CMaterial2* pMat, const float* col) {
				if (!pMat || iFrom < 0 || iTo <= iFrom) return;
				const int iEnd = (std::min)(iTo, pBuf->m_iMaxPrimitives);
				for (int i = iFrom; i < iEnd; ++i) {
					auto& prim = pBuf->m_pPrimitives[i];
					prim.m_pMaterial = pMat;
					prim.m_pMaterial2 = pMat;
					ApplyColorToMesh(prim.m_Color, col);
				}
			};

			if (Settings::Chams::WallhackMode && target->m_pWallMat) {
				const int iStart = pBuf->m_iStartPrimitive;
				oOnGeneratePrimitives(pSceneSys, pSceneObject, pUnk, pBuf);
				ApplyPrimitiveRange(iStart, pBuf->m_iStartPrimitive, target->m_pWallMat, target->invisColor);
			}

			if (target->m_pVisMat) {
				const int iStart = pBuf->m_iStartPrimitive;
				oOnGeneratePrimitives(pSceneSys, pSceneObject, pUnk, pBuf);
				ApplyPrimitiveRange(iStart, pBuf->m_iStartPrimitive, target->m_pVisMat, target->visColor);
			}

			return true;
		}

		bool OnGeneratePrimitives(
			void* pSceneSys,
			CSceneAnimatableObject* pSceneObject,
			void* pUnk,
			CMeshPrimitiveOutputBuffer_t* pBuf
		) {
			if (!m_bInitialized || !pSceneObject || !pBuf || !pBuf->m_pPrimitives)
				return false;

			__try {
				return OnGeneratePrimitivesImpl(pSceneSys, pSceneObject, pUnk, pBuf);
			} __except (EXCEPTION_EXECUTE_HANDLER) {
				return false;
			}
		}
	}
}