#include "present.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Hook_CreateMove.h"

PresentFn oPresent = nullptr;
ResizeBuffersFn oResizeBuffers = nullptr;
CreateSwapChainFn oCreateSwapChain = nullptr;

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

	if (g_MenuVisible) {
		ImGui::Begin("cs2 anti-aim by nocontex", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		
		ImGui::Checkbox("Enable Anti-Aim", &g_AntiAimEnabled);

		if (g_AntiAimEnabled) {
			ImGui::Separator();
			
			const char* pitchModes[] = { "None", "Down (89)", "Up (-89)", "Zero" };
			ImGui::Combo("Pitch Mode", &g_AntiAimPitchMode, pitchModes, IM_ARRAYSIZE(pitchModes));

			const char* yawModes[] = { "None", "Backward (180)", "Left (-90)", "Right (90)" };
			ImGui::Combo("Yaw Mode", &g_AntiAimYawMode, yawModes, IM_ARRAYSIZE(yawModes));

			ImGui::SliderFloat("Yaw Offset", &g_AntiAimYawOffset, -180.f, 180.f, "%.1f");

			ImGui::SliderFloat("Roll", &g_AntiAimRoll, -50.f, 50.f, "%.1f");
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
