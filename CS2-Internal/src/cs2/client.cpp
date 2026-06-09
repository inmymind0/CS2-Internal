#include "client.h"
#include "cs2/hooks/hooks.h"
#include "cs2/helpers/devlog.h"
#include "core/mem/mem.h"
#include "cs2/signatures.h"
#include <thread>

bool CCS2Client::Initialize(HMODULE hModule) {
	m_hModule = hModule;

	AllocConsole();
	freopen_s(&m_ConsoleFile, "CONOUT$", "w", stdout);

	DEV_LOG("[+] [client] CCS2Client baslatiliyor...");

	uintptr_t isInGameAddr = Mem::PatternScan(ISINGAME_PATTERN, ENGINE2_DLL);
	if (isInGameAddr) {
		m_IsInGameFn = reinterpret_cast<IsInGameFn>(isInGameAddr);
		DEV_LOG_HEX("[+] [client] IsInGame adresi bulundu: ", isInGameAddr);
	} else {
		DEV_LOG("[-] [client] IsInGame adresi bulunamadi");
	}

	if (hooks::Init()) {
		DEV_LOG("[+] [client] tum hooklar aktif edildi");
	} else {
		DEV_LOG("[-] [client] hook kurulumu basarisiz");
		return false;
	}

	DEV_LOG("[+] [client] CCS2Client yuklendi");
	return true;
}

void CCS2Client::Shutdown() {
	DEV_LOG("[!] [client] CCS2Client kapatiliyor...");

	hooks::Shutdown();
	DEV_LOG("[+] [client] hooklar kaldirildi");

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	if (m_ConsoleFile) {
		fclose(m_ConsoleFile);
		m_ConsoleFile = nullptr;
	}
	FreeConsole();
}

bool CCS2Client::IsInGame() {
	if (!m_IsInGameFn)
		return false;
	return m_IsInGameFn();
}
