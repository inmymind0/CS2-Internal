#pragma once
#include <Windows.h>
#include <iostream>

typedef bool(__fastcall* IsInGameFn)();

class CCS2Client {
public:
	static CCS2Client& Get() {
		static CCS2Client instance;
		return instance;
	}

	bool Initialize(HMODULE hModule);
	void Shutdown();

	HMODULE GetModule() const { return m_hModule; }
	bool IsInGame();

private:
	CCS2Client() = default;
	~CCS2Client() = default;

	HMODULE m_hModule = nullptr;
	FILE* m_ConsoleFile = nullptr;

	IsInGameFn m_IsInGameFn = nullptr;
};
