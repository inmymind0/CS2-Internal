#include <Windows.h>
#include <thread>
#include <iostream>
#include <clocale>
#include "cs2/hooks/hooks.h"

DWORD WINAPI MainThread(LPVOID lpParam) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	if (hooks::Init()) {
		std::cout << "[+] [hook] tum hooklar aktif hale getirildi" << std::endl;
	} else {
		std::cout << "[-] [hata] took kurulumu basarisiz oldu" << std::endl;
	}

	while (!GetAsyncKeyState(VK_DELETE)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	std::cout << "[!] [sistem] cikis tusu algilandi..." << std::endl;
	hooks::Shutdown();
	std::cout << "[+] [hook] hooklar kaldirildi" << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));


	if (f) fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread((HMODULE)lpParam, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}