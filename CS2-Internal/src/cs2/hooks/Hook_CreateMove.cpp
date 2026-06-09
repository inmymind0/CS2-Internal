#include "Hook_CreateMove.h"
#include "core/math/math.h"
#include "offsets/offsets.h"
#include <cmath>
#include <algorithm>

CreateMoveFn oCreateMove = nullptr;

bool g_AntiAimEnabled = true;
int g_AntiAimPitchMode = 1;
int g_AntiAimYawMode = 1;
float g_AntiAimYawOffset = 0.f;
float g_AntiAimRoll = 0.f;

QAngle_t g_RealAngles = {};
QAngle_t g_FakeAngles = {};
bool g_IsUsing = false;

void FixButtons(CUserCmd* pUserCmd) {
	if (!pUserCmd || !pUserCmd->csgoUserCmd.pBaseCmd)
		return;

	auto* pBaseCmd = pUserCmd->csgoUserCmd.pBaseCmd;

	if (pBaseCmd->flForwardMove > 0.f)
		pUserCmd->nButtons.nValue |= IN_FORWARD;
	else if (pBaseCmd->flForwardMove < 0.f)
		pUserCmd->nButtons.nValue |= IN_BACK;

	if (pBaseCmd->flSideMove > 0.f)
		pUserCmd->nButtons.nValue |= IN_MOVELEFT;
	else if (pBaseCmd->flSideMove < 0.f)
		pUserCmd->nButtons.nValue |= IN_MOVERIGHT;
}

void MovementCorrection(CUserCmd* pUserCmd, const QAngle_t& RealAngles, const QAngle_t& FakeAngles) {
	if (!pUserCmd || !pUserCmd->csgoUserCmd.pBaseCmd)
		return;
}

double __fastcall hkCreateMove(CCSGOInput* pThis, unsigned int slot, CUserCmd* pCmd)
{
	double returnValue = oCreateMove(pThis, slot, pCmd);

	if (!pThis || !pCmd) {
		g_IsUsing = false;
		return returnValue;
	}

	g_IsUsing = (pCmd->nButtons.nValue & IN_USE);

	if (pCmd->nButtons.nValue & IN_JUMP) {
		pCmd->nButtons.nValue &= ~IN_JUMP;
	}

	uintptr_t clientBase = (uintptr_t)GetModuleHandleA("client.dll");
	if (!clientBase)
		return returnValue;

	uintptr_t localController = *(uintptr_t*)(clientBase + offsets::client_dll::dwLocalPlayerController);
	bool isAlive = localController ? *(bool*)(localController + offsets::client_dll::CCSPlayerController::m_bPawnIsAlive) : false;

	uintptr_t localPawn = *(uintptr_t*)(clientBase + offsets::client_dll::dwLocalPlayerPawn);

	if (!isAlive || !localPawn)
		return returnValue;

	if (g_IsUsing) {
		return returnValue;
	}

	uint8_t moveType = *(uint8_t*)(localPawn + offsets::client_dll::C_BaseEntity::m_MoveType);
	if (moveType == 9)
		return returnValue;

	if (g_AntiAimEnabled) {
		auto* pBaseCmd = pCmd->csgoUserCmd.pBaseCmd;
		if (pBaseCmd && pBaseCmd->pViewAngles) {
			g_RealAngles = pBaseCmd->pViewAngles->angValue;

			QAngle_t pModifiedViewAngles = g_RealAngles;

			if (g_AntiAimPitchMode == 1) {
				pModifiedViewAngles.pitch = 89.0f;
			} else if (g_AntiAimPitchMode == 2) {
				pModifiedViewAngles.pitch = -89.0f;
			} else if (g_AntiAimPitchMode == 3) {
				pModifiedViewAngles.pitch = 0.f;
			}

			float yawMod = 0.f;
			if (g_AntiAimYawMode == 1) {
				yawMod = 180.f;
			} else if (g_AntiAimYawMode == 2) {
				yawMod = -90.f;
			} else if (g_AntiAimYawMode == 3) {
				yawMod = 90.f;
			}

			pModifiedViewAngles.yaw += yawMod + g_AntiAimYawOffset;
			pModifiedViewAngles.roll = g_AntiAimRoll;

			Math::NormalizeAngles(pModifiedViewAngles);
			Math::ClampAngles(pModifiedViewAngles);

			pBaseCmd->pViewAngles->angValue = pModifiedViewAngles;

			MovementCorrection(pCmd, g_RealAngles, pModifiedViewAngles);

			bool is_firing = (pCmd->nButtons.nValue & IN_ATTACK);
			bool is_knifing = (pCmd->nButtons.nValue & IN_SECOND_ATTACK);

			if (is_firing || is_knifing) {
				pCmd->SetSubTickAngle(g_RealAngles);
			} else {
				pCmd->SetSubTickAngle(pModifiedViewAngles);
			}

			g_FakeAngles = pModifiedViewAngles;
		}
	}

	return returnValue;
}


