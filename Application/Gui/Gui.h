#pragma once

#include <Windows.h>

#define WIDTH 1920
#define HEIGHT 1080	
#define WINDOW_W 1920
#define WINDOW_H 1080

namespace Gui
{
	inline MSG msg;
	inline bool running = true;

	bool init(HWND hwnd);
	void destroy();

	void frame();
	void handle_events();
};

namespace Window
{
	inline LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT Result = 0;
		switch (message)
		{
		case WM_DESTROY:
		{
			Gui::running = false;
			break;
		}
		case WM_CLOSE:
		{
			Gui::running = false;
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

	inline HWND InitWindow(HINSTANCE hInstance)
	{
		HWND hwnd = NULL;

		WNDCLASS wc = { };

		wc.lpfnWndProc = WinProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = L"CS2_Overlay";

		if (!RegisterClass(&wc))
			return 0;

		hwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, wc.lpszClassName, wc.lpszClassName,
			WS_VISIBLE | WS_POPUP,
			0, 0, WIDTH, HEIGHT,
			NULL, NULL, hInstance, NULL);

		SetLayeredWindowAttributes(hwnd, 0, 0, LWA_ALPHA);
		SetLayeredWindowAttributes(hwnd, 0, RGB(0, 0, 0), LWA_COLORKEY);

		return hwnd;
	}
}