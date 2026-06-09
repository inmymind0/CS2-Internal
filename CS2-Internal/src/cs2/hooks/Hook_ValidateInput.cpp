#include "Hook_ValidateInput.h"
#include "Hook_SetViewAngles.h"
#include "Hook_CreateMove.h"
#include "offsets/offsets.h"
#include <Windows.h>

ValidateInputFn oValidateInput = nullptr;

void __fastcall hkValidateInput(CCSGOInput* pInput, int unk)
{
	if (!pInput || !oValidateInput || !oSetViewAngles) {
		if (oValidateInput)
			oValidateInput(pInput, unk);
		return;
	}

	uintptr_t clientBase = (uintptr_t)GetModuleHandleA("client.dll");
	if (!clientBase) {
		oValidateInput(pInput, unk);
		return;
	}

	uintptr_t localController = *(uintptr_t*)(clientBase + offsets::client_dll::dwLocalPlayerController);
	bool isAlive = localController ? *(bool*)(localController + offsets::client_dll::CCSPlayerController::m_bPawnIsAlive) : false;

	if (g_AntiAimEnabled && isAlive && !g_IsUsing) {
		oSetViewAngles(pInput, 0, g_FakeAngles);
		oValidateInput(pInput, unk);
		oSetViewAngles(pInput, 0, g_RealAngles);
		return;
	}

	oValidateInput(pInput, unk);
}


