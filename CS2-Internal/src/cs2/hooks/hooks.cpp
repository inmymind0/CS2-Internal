#include "hooks.h"

#include "minhook/include/MinHook.h"

#include <iostream>
#include "core/mem/mem.h"
#include "present.h"
#include "Hook_CreateMove.h"
#include "Hook_ValidateInput.h"
#include "Hook_SetViewAngles.h"

bool hooks::Init() {
	if (MH_Initialize() != MH_OK) {
		std::cout << "[-] [hata] MH_Initialize basarisiz" << std::endl;
		return false;
	}

	std::cout << "[+] [minhook] minhook baslatildi" << std::endl;

	if (!SetupPresentHook()) {
		std::cout << "[-] [hata] SetupPresentHook olusturulamadi" << std::endl;
		return false;
	}
	std::cout << "[+] [hook] Present hooku kuruldu" << std::endl;

	if (!SetupResizeBuffersHook()) {
		std::cout << "[-] [hata] SetupResizeBuffersHook olusturulamadi" << std::endl;
		return false;
	}
	std::cout << "[+] [hook] ResizeBuffers hooku kuruldu" << std::endl;

	if (!SetupCreateSwapChainHook()) {
		std::cout << "[-] [hata] SetupCreateSwapChainHook olusturulamadi" << std::endl;
		return false;
	}
	std::cout << "[+] [hook] CreateSwapChain hooku kuruldu" << std::endl;

	if (!SetupCreateMoveHook()) {
		std::cout << "[-] [hata] SetupCreateMoveHook olusturulamadi" << std::endl;
		return false;
	}
	std::cout << "[+] [hook] CreateMove hooku kuruldu" << std::endl;

	if (!SetupValidateInputHook()) {
		std::cout << "[-] [hata] SetupValidateInputHook olusturulamadi" << std::endl;
		return false;
	}
	std::cout << "[+] [hook] ValidateInput hooku kuruldu" << std::endl;

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		std::cout << "[-] [hata] hooklar aktif edilemedi" << std::endl;
		return false;
	}
	std::cout << "[+] [hook] tum hooklar aktif edildi" << std::endl;

	return true;
}

void hooks::Shutdown() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

bool hooks::SetupValidateInputHook() {
	uintptr_t setviewanglesAddr = Mem::PatternScan(SETVIEWANGLES_PATTERN, "client.dll");
	if (!setviewanglesAddr) {
		return false;
	}
	oSetViewAngles = reinterpret_cast<SetViewAnglesFn>(setviewanglesAddr);

	uintptr_t validateinputAddr = Mem::PatternScan(VALIDATEINPUT_PATTERN, "client.dll");
	if (!validateinputAddr) {
		return false;
	}

	if (MH_CreateHook(reinterpret_cast<LPVOID>(validateinputAddr), &hkValidateInput, reinterpret_cast<LPVOID*>(&oValidateInput)) != MH_OK) {
		return false;
	}

	return true;
}

bool hooks::SetupPresentHook() {
	uintptr_t presentAddr = Mem::PatternScan(PRESENT_PATTERN, "GameOverlayRenderer64.dll");
	if (!presentAddr) {
		return false;
	}

	if (MH_CreateHook(reinterpret_cast<LPVOID>(presentAddr), &hkPresent, reinterpret_cast<LPVOID*>(&oPresent)) != MH_OK) {
		return false;
	}

	return true;
}

bool hooks::SetupCreateMoveHook() {
	uintptr_t createmoveAddr = Mem::PatternScan(CREATEMOVE_PATTERN, "client.dll");
	if (!createmoveAddr) {
		return false;
	}

	if (MH_CreateHook(reinterpret_cast<LPVOID>(createmoveAddr), &hkCreateMove, reinterpret_cast<LPVOID*>(&oCreateMove)) != MH_OK) {
		return false;
	}

	return true;
}

bool hooks::SetupResizeBuffersHook() {
	uintptr_t resizebuffersAddr = Mem::PatternScan(RESIZEBUFFERS_PATTERN, "GameOverlayRenderer64.dll");
	if (!resizebuffersAddr) {
		return false;
	}

	if (MH_CreateHook(reinterpret_cast<LPVOID>(resizebuffersAddr), &hkResizeBuffers, reinterpret_cast<LPVOID*>(&oResizeBuffers)) != MH_OK) {
		return false;
	}

	return true;
}

bool hooks::SetupCreateSwapChainHook() {
	uintptr_t createswapchainAddr = Mem::PatternScan(CREATESWAPCHAIN_PATTERN, "GameOverlayRenderer64.dll");
	if (!createswapchainAddr) {
		return false;
	}

	if (MH_CreateHook(reinterpret_cast<LPVOID>(createswapchainAddr), &hkCreateSwapChain, reinterpret_cast<LPVOID*>(&oCreateSwapChain)) != MH_OK) {
		return false;
	}

	return true;
}