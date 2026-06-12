#include "present.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Hook_CreateMove.h"
#include "offsets/offsets.h"
#include "core/math/math.h"
#include "core/mem/mem.h"
#include "cs2/settings.h"
#include "cs2/signatures.h"
#include "cs2/features/visuals.h"
#include "cs2/helpers/devlog.h"
#include "cs2/helpers/projectsettings.h"
#include <iostream>

PresentFn oPresent = nullptr;
ResizeBuffersFn oResizeBuffers = nullptr;
CreateSwapChainFn oCreateSwapChain = nullptr;
uintptr_t g_GetBaseEntityAddr = 0;

bool Initialized = false;
bool g_MenuVisible = true;

HWND window = nullptr;
WNDPROC oWndProc = nullptr;

ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* mainRenderTargetView = nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_KEYDOWN) {
		if (wParam == VK_INSERT) {
			g_MenuVisible = !g_MenuVisible;
		}
	}

	if (g_MenuVisible) {
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void InitImGui() {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

bool initHkPresent(IDXGISwapChain* pSwapChain) {
	if (!Initialized) {
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			g_GetBaseEntityAddr = Mem::PatternScan(GETBASEENTITY_PATTERN, CLIENT_DLL);
			if (g_GetBaseEntityAddr) {
				DEV_LOG_HEX("[debug] GetBaseEntity adresi bulundu: ", g_GetBaseEntityAddr);
			} else {
				DEV_LOG("[debug] GetBaseEntity adresi bulunamadi");
			}
			Initialized = true;
		}
	}
	return Initialized;
}

HRESULT hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT flags)
{
	initHkPresent(pSwapChain);

	if (!mainRenderTargetView && pDevice) {
		ID3D11Texture2D* pBackBuffer = nullptr;
		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer))) {
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
		}
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	#ifdef CHEAT_NAME
	ImGui::GetForegroundDrawList()->AddText(ImVec2(11.f, 11.f), ImColor(0, 0, 0, 255), CHEAT_NAME);
	ImGui::GetForegroundDrawList()->AddText(ImVec2(10.f, 10.f), ImColor(255, 0, 0, 255), CHEAT_NAME);
	#endif



	uintptr_t clientBase = (uintptr_t)GetModuleHandleA(CLIENT_DLL);
	if (clientBase) {
		uintptr_t entityList = 0;
		uintptr_t localController = 0;

		__try {
			entityList = *(uintptr_t*)(clientBase + offsets::client_dll::dwEntityList);
			localController = *(uintptr_t*)(clientBase + offsets::client_dll::dwLocalPlayerController);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			entityList = 0;
			localController = 0;
		}

		if (entityList && localController) {
			uint8_t localTeam = 0;
			ViewMatrix viewMatrix = {};
			bool baseSetupOk = false;

			__try {
				localTeam = *(uint8_t*)(localController + offsets::client_dll::C_BaseEntity::m_iTeamNum);
				viewMatrix = *(ViewMatrix*)(clientBase + offsets::client_dll::dwViewMatrix);
				baseSetupOk = true;
			} __except (EXCEPTION_EXECUTE_HANDLER) {
				baseSetupOk = false;
			}

			if (baseSetupOk) {
				ImVec2 displaySize = ImGui::GetIO().DisplaySize;
				int width = (int)displaySize.x;
				int height = (int)displaySize.y;

				ImDrawList* drawList = ImGui::GetForegroundDrawList();
				Features::Visuals::RenderESP(drawList, entityList, localController, localTeam, viewMatrix, width, height);
			}
		}
	}

	if (g_MenuVisible) {
		ImGui::Begin("cs2 internal base by nocontex", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		
		ImGui::Checkbox("Enable Anti-Aim", &Settings::AntiAim::Enabled);

		if (Settings::AntiAim::Enabled) {
			ImGui::Separator();
			
			const char* pitchModes[] = { "None", "Down (89)", "Up (-89)", "Zero" };
			ImGui::Combo("Pitch Mode", &Settings::AntiAim::PitchMode, pitchModes, IM_ARRAYSIZE(pitchModes));

			const char* yawModes[] = { "None", "Backward (180)", "Left (-90)", "Right (90)" };
			ImGui::Combo("Yaw Mode", &Settings::AntiAim::YawMode, yawModes, IM_ARRAYSIZE(yawModes));

			ImGui::SliderFloat("Yaw Offset", &Settings::AntiAim::YawOffset, -180.f, 180.f, "%.1f");

			ImGui::SliderFloat("Roll", &Settings::AntiAim::Roll, -50.f, 50.f, "%.1f");
		}

		ImGui::Separator();
		ImGui::Text("Visuals");
		ImGui::Checkbox("Enable ESP", &Settings::Visuals::EspEnabled);
		if (Settings::Visuals::EspEnabled) {
			ImGui::Checkbox("Box ESP", &Settings::Visuals::BoxEsp);
			ImGui::Checkbox("Name ESP", &Settings::Visuals::NameEsp);
			ImGui::Checkbox("Draw Teammates", &Settings::Visuals::EspTeammates);
		}

		ImGui::Separator();
		ImGui::Text("Chams");
		ImGui::Checkbox("Active Chams", &Settings::Chams::Active);
		if (Settings::Chams::Active) {
			ImGui::Checkbox("Visible Chams", &Settings::Chams::VisibleActive);
			
			const char* materials[] = { 
				"Generic", "Unlit", "Solid", "Latex"
			};
			
			if (Settings::Chams::VisibleActive) {
				ImGui::Combo("Visible Material", &Settings::Chams::Material, materials, IM_ARRAYSIZE(materials));
				ImGui::ColorEdit4("Visible Enemy Color", Settings::Chams::VisibleEnemy);
				ImGui::ColorEdit4("Visible Team Color", Settings::Chams::VisibleTeam);
			}

			ImGui::Checkbox("Invisible Chams", &Settings::Chams::WallhackMode);
			if (Settings::Chams::WallhackMode) {
				ImGui::Combo("Invisible Material", &Settings::Chams::WallhackMaterial, materials, IM_ARRAYSIZE(materials));
				ImGui::ColorEdit4("Invisible Enemy Color", Settings::Chams::InvisibleEnemy);
				ImGui::ColorEdit4("Invisible Team Color", Settings::Chams::InvisibleTeam);
			}

			ImGui::Checkbox("Chams Teammates", &Settings::Chams::Team);
		}

		ImGui::End();
	}

	ImGui::Render();

	ID3D11RenderTargetView* originalRenderTarget = nullptr;
	ID3D11DepthStencilView* originalDepthStencil = nullptr;
	pContext->OMGetRenderTargets(1, &originalRenderTarget, &originalDepthStencil);

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	pContext->OMSetRenderTargets(1, &originalRenderTarget, originalDepthStencil);

	if (originalRenderTarget) {
		originalRenderTarget->Release();
	}
	if (originalDepthStencil) {
		originalDepthStencil->Release();
	}

	return oPresent(pSwapChain, SyncInterval, flags);
}

HRESULT hkResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
	if (mainRenderTargetView) {
		mainRenderTargetView->Release();
		mainRenderTargetView = nullptr;
	}
	return oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

HRESULT hkCreateSwapChain(IDXGIFactory* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain) {
	if (mainRenderTargetView) {
		mainRenderTargetView->Release();
		mainRenderTargetView = nullptr;
	}
	return oCreateSwapChain(pFactory, pDevice, pDesc, ppSwapChain);
}
