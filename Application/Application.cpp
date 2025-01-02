#include "Application.h"
#include "Modules/RootModule.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	default:
	{
		Result = DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	}
	return Result;
}

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEXW wc = {};
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = APP_NAME;
	wc.lpfnWndProc = WinProc;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;

	RegisterClassExW(&wc);

	const HWND window = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, wc.lpszClassName, wc.lpszClassName,
										WS_POPUP, 0, 0, 1920, 1080, NULL, NULL, hInstance, NULL);

	MARGINS margins = {-1};
	SetLayeredWindowAttributes(window, RGB(0, 0, 0), 255, LWA_ALPHA);
	DwmExtendFrameIntoClientArea(window, &margins);

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferCount = 2U;
	sd.SampleDesc.Count = 1U;
	sd.BufferDesc.RefreshRate.Numerator = 60U;
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.OutputWindow = window;
	sd.Windowed = TRUE;

	constexpr D3D_FEATURE_LEVEL levels[]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0};

	ID3D11Device *device;
	ID3D11DeviceContext *context;
	IDXGISwapChain *swap_chain;
	ID3D11RenderTargetView *render_target_view;
	D3D_FEATURE_LEVEL level;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0U,
		levels,
		2U,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&level,
		&context);

	ID3D11Texture2D *back_buffer;
	swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
	if (!back_buffer)
		return 1;

	device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
	back_buffer->Release();

	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, context);

	Start(L"cs2.exe", L"Counter-Strike 2", window);
	RootModule *Modules = new RootModule();

	::isRunning = true;
	while (::isRunning)
	{
		MSG msg;

		while (PeekMessage(&msg, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			switch (msg.message)
			{
			case WM_QUIT:
				break;

			default:
				break;
			}

			ImGui_ImplWin32_WndProcHandler(window, msg.message, msg.wParam, msg.lParam);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		Modules->Render();
		ImGui::Render();

		constexpr FLOAT CLEAR[]{0.F, 0.F, 0.F, 0.0F};
		context->OMSetRenderTargets(1U, &render_target_view, NULL);
		context->ClearRenderTargetView(render_target_view, CLEAR);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		swap_chain->Present(1U, 0U);

		if (::isReady)
			continue;

		ToggleWindowAffinity(window);
		::isReady = true;
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (swap_chain)
		swap_chain->Release();
	if (context)
		context->Release();
	if (device)
		device->Release();
	if (render_target_view)
		render_target_view->Release();

	DestroyWindow(window);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);

	return 0;
}
