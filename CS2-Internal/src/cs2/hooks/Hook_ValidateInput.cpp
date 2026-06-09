#include "Hook_ValidateInput.h"
#include "Hook_SetViewAngles.h"
#include "cs2/features/antiaim.h"
#include "offsets/offsets.h"
#include "cs2/settings.h"
#include "cs2/signatures.h"
#include <Windows.h>

ValidateInputFn oValidateInput = nullptr;

void __fastcall hkValidateInput(CCSGOInput* pInput, int unk)
{
	if (!pInput || !oValidateInput || !oSetViewAngles) {
		if (oValidateInput)
			oValidateInput(pInput, unk);
		return;
	}

	uintptr_t clientBase = (uintptr_t)GetModuleHandleA(CLIENT_DLL);
	if (!clientBase) {
		oValidateInput(pInput, unk);
		return;
	}

	uintptr_t localController = *(uintptr_t*)(clientBase + offsets::client_dll::dwLocalPlayerController);
	bool isAlive = localController ? *(bool*)(localController + offsets::client_dll::CCSPlayerController::m_bPawnIsAlive) : false;

	if (Settings::AntiAim::Enabled && isAlive && !Features::AntiAim::g_IsUsing) {
		oSetViewAngles(pInput, 0, Features::AntiAim::g_FakeAngles);
		oValidateInput(pInput, unk);
		oSetViewAngles(pInput, 0, Features::AntiAim::g_RealAngles);
		return;
	}

	oValidateInput(pInput, unk);
}


