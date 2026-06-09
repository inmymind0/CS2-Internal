#include <Windows.h>
#include <thread>
#include "cs2/client.h"

DWORD WINAPI MainThread(LPVOID lpParam) {
	HMODULE hModule = (HMODULE)lpParam;

	if (!CCS2Client::Get().Initialize(hModule)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		FreeLibraryAndExitThread(hModule, 0);
		return 0;
	}

	while (!GetAsyncKeyState(VK_DELETE)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	CCS2Client::Get().Shutdown();

	FreeLibraryAndExitThread(hModule, 0);
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