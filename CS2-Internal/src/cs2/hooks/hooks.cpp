#include "hooks.h"

#include "minhook/include/MinHook.h"
#include "core/mem/mem.h"
#include "present.h"
#include "Hook_CreateMove.h"
#include "Hook_ValidateInput.h"
#include "Hook_SetViewAngles.h"
#include "cs2/helpers/devlog.h"

bool hooks::Init() {
	if (MH_Initialize() != MH_OK) {
		DEV_LOG("[-] [hata] MH_Initialize basarisiz");
		return false;
	}

	DEV_LOG("[+] [minhook] MinHook baslatildi");

	if (!SetupPresentHook()) {
		DEV_LOG("[-] [hata] SetupPresentHook olusturulamadi");
		return false;
	}
	DEV_LOG("[+] [hook] Present hooku kuruldu");

	if (!SetupResizeBuffersHook()) {
		DEV_LOG("[-] [hata] SetupResizeBuffersHook olusturulamadi");
		return false;
	}
	DEV_LOG("[+] [hook] ResizeBuffers hooku kuruldu");

	if (!SetupCreateSwapChainHook()) {
		DEV_LOG("[-] [hata] SetupCreateSwapChainHook olusturulamadi");
		return false;
	}
	DEV_LOG("[+] [hook] CreateSwapChain hooku kuruldu");

	if (!SetupCreateMoveHook()) {
		DEV_LOG("[-] [hata] SetupCreateMoveHook olusturulamadi");
		return false;
	}
	DEV_LOG("[+] [hook] CreateMove hooku kuruldu");

	if (!SetupValidateInputHook()) {
		DEV_LOG("[-] [hata] SetupValidateInputHook olusturulamadi");
		return false;
	}
	DEV_LOG("[+] [hook] ValidateInput hooku kuruldu");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		DEV_LOG("[-] [hata] hooklar aktif edilemedi");
		return false;
	}
	DEV_LOG("[+] [hook] tum hooklar aktif edildi");

	return true;
}

void hooks::Shutdown() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

bool hooks::SetupValidateInputHook() {
	uintptr_t setviewanglesAddr = Mem::PatternScan(SETVIEWANGLES_PATTERN, CLIENT_DLL);
	if (!setviewanglesAddr) {
		return false;
	}
	oSetViewAngles = reinterpret_cast<SetViewAnglesFn>(setviewanglesAddr);

	uintptr_t validateinputAddr = Mem::PatternScan(VALIDATEINPUT_PATTERN, CLIENT_DLL);
	if (!validateinputAddr) {
		return false;
	}

	if (MH_CreateHook(reinterpret_cast<LPVOID>(validateinputAddr), &hkValidateInput, reinterpret_cast<LPVOID*>(&oValidateInput)) != MH_OK) {
		return false;
	}

	return true;
}

bool hooks::SetupPresentHook() {
	uintptr_t presentAddr = Mem::PatternScan(PRESENT_PATTERN, GAMEOVERLAY_DLL);
	if (!presentAddr) {
		return false;
	}

	if (MH_CreateHook(reinterpret_cast<LPVOID>(presentAddr), &hkPresent, reinterpret_cast<LPVOID*>(&oPresent)) != MH_OK) {
		return false;
	}

	return true;
}

bool hooks::SetupCreateMoveHook() {
	uintptr_t createmoveAddr = Mem::PatternScan(CREATEMOVE_PATTERN, CLIENT_DLL);
	if (!createmoveAddr) {
		return false;
	}

	if (MH_CreateHook(reinterpret_cast<LPVOID>(createmoveAddr), &hkCreateMove, reinterpret_cast<LPVOID*>(&oCreateMove)) != MH_OK) {
		return false;
	}

	return true;
}

bool hooks::SetupResizeBuffersHook() {
	uintptr_t resizebuffersAddr = Mem::PatternScan(RESIZEBUFFERS_PATTERN, GAMEOVERLAY_DLL);
	if (!resizebuffersAddr) {
		return false;
	}

	if (MH_CreateHook(reinterpret_cast<LPVOID>(resizebuffersAddr), &hkResizeBuffers, reinterpret_cast<LPVOID*>(&oResizeBuffers)) != MH_OK) {
		return false;
	}

	return true;
}

bool hooks::SetupCreateSwapChainHook() {
	uintptr_t createswapchainAddr = Mem::PatternScan(CREATESWAPCHAIN_PATTERN, GAMEOVERLAY_DLL);
	if (!createswapchainAddr) {
		return false;
	}

	if (MH_CreateHook(reinterpret_cast<LPVOID>(createswapchainAddr), &hkCreateSwapChain, reinterpret_cast<LPVOID*>(&oCreateSwapChain)) != MH_OK) {
		return false;
	}

	return true;
}