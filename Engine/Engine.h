#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <ranges>
#include <algorithm>

#include "Memory.h"
#include "Math/Geo.h"
#include "Math/ViewMatrix.h"

using namespace Memory;

namespace Engine
{
	DWORD PID;
	uintptr_t CLIENT_DLL;

	const wchar_t *GAME_EXE = L"cs2.exe";
	const wchar_t *CLIENT_DLL_NAME = L"client.dll";

	const Dimension SD = { 1920, 1080 };
	const Dimension SD_H = { SD.w / 2, SD.h / 2 };

	void Init()
	{
		PID = Memory::GetProcessID(GAME_EXE);
		CLIENT_DLL = Memory::GetModuleBaseAddress(PID, CLIENT_DLL_NAME);
	}

	template <typename T>
	T ReadClient(const ptrdiff_t& ptr_diff)
	{
		return Read<T>(CLIENT_DLL + ptr_diff);
	};

	template <typename T>
	T WriteClient(const ptrdiff_t& ptr_diff, const T& value)
	{
		return Write<T>(CLIENT_DLL + ptr_diff, value);
	};
}
