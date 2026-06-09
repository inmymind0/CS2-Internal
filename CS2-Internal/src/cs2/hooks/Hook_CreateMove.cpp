#include "Hook_CreateMove.h"
#include "offsets/offsets.h"
#include "cs2/features/antiaim.h"
#include "cs2/signatures.h"

CreateMoveFn oCreateMove = nullptr;

double __fastcall hkCreateMove(CCSGOInput* pThis, unsigned int slot, CUserCmd* pCmd)
{
	double returnValue = oCreateMove(pThis, slot, pCmd);

	if (!pThis || !pCmd) {
		Features::AntiAim::g_IsUsing = false;
		return returnValue;
	}

	if (pCmd->nButtons.nValue & IN_JUMP) {
		pCmd->nButtons.nValue &= ~IN_JUMP;
	}

	uintptr_t clientBase = (uintptr_t)GetModuleHandleA(CLIENT_DLL);
	if (!clientBase)
		return returnValue;

	uintptr_t localController = *(uintptr_t*)(clientBase + offsets::client_dll::dwLocalPlayerController);
	bool isAlive = localController ? *(bool*)(localController + offsets::client_dll::CCSPlayerController::m_bPawnIsAlive) : false;

	uintptr_t localPawn = *(uintptr_t*)(clientBase + offsets::client_dll::dwLocalPlayerPawn);

	if (!isAlive || !localPawn)
		return returnValue;

	Features::AntiAim::g_IsUsing = (pCmd->nButtons.nValue & IN_USE);
	if (Features::AntiAim::g_IsUsing) {
		return returnValue;
	}

	uint8_t moveType = *(uint8_t*)(localPawn + offsets::client_dll::C_BaseEntity::m_MoveType);
	if (moveType == 9)
		return returnValue;

	Features::AntiAim::Run(pCmd);

	return returnValue;
}
