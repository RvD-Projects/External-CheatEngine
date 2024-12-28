#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <windows.h>
#include <thread>

#include "Memory.h"
#include "Math/Geo.h"
#include "Math/ViewMatrix.h"

using namespace Memory;

namespace Engine
{
	APP_INFO Target;
	APP_INFO TargetWindow;
	APP_INFO TargetClient;

	DWORD refreshRate = 16;
	std::atomic<bool> isReady;
	std::atomic<bool> isRunning;

	bool InjectTarget()
	{
		Target.pid = GetProcessID(Target.exe_name);
		if (!Target.pid)
		{
			return false;
		}

		Target.dll = GetModuleBaseAddress(Target.pid, Target.dll_name);

		return Target.pid && Target.dll;
	}

	bool UpdateTargetWindowDefinitions()
	{
		HWND window = FindWindow(NULL, Target.w_name);
		if (window == NULL)
			return false;
	

		RECT clientRect;
		GetClientRect(window, &clientRect);
		if (!clientRect.bottom || !clientRect.right)
			return false;
	

		RECT windowRect;
		GetWindowRect(window, &windowRect);

		TargetWindow.rect = windowRect;
		TargetWindow.position = Position{ static_cast<float>(windowRect.left), static_cast<float>(windowRect.top) };
		TargetWindow.dimension = Dimension{ static_cast<float>(windowRect.right - windowRect.left), static_cast<float>(windowRect.bottom - windowRect.top) };

		TargetClient.rect = clientRect;
		TargetClient.dimension = Dimension{1920, 1080};
		TargetClient.position = Position{TargetWindow.position.x, TargetWindow.position.y};

		return true;
	}

	void Execute()
	{
		isRunning = true;
		while (isRunning)
		{
			if (!InjectTarget())
			{
				TargetClient = {};
				TargetWindow = {};
				std::wcerr << L"Waiting for injection target (not found)." << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				continue;
			}

			if (!UpdateTargetWindowDefinitions())
			{
				TargetClient = {};
				TargetWindow = {};
				std::wcerr << L"Waiting for target window (not found)." << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				continue;
			}

			isReady = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(refreshRate));
		}
	}

	void Start(const wchar_t *exe_name, const wchar_t *dll_name, const wchar_t *window_name)
	{
		Target.exe_name = exe_name;
		Target.dll_name = dll_name;
		Target.w_name = window_name;

		std::thread(Execute).detach();
	}

	template <typename T>
	T ReadDLL(const ptrdiff_t &ptr_diff)
	{
		return Read<T>(Target.dll + ptr_diff);
	};

	template <typename T>
	T WriteDLL(const ptrdiff_t &ptr_diff, const T &value)
	{
		return Write<T>(Target.dll + ptr_diff, value);
	};

	Position GetClientPosition()
	{
		return Position{TargetClient.position.x, TargetClient.position.y};
	}

	Dimension GetClientDimension()
	{
		return TargetClient.dimension;
	}

	Position GetWindowPosition()
	{
		return Position{TargetWindow.position.x, TargetWindow.position.y};
	}

	Dimension GetWindowDimension()
	{
		return TargetWindow.dimension;
	}
}
