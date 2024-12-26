#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include "Memory.h"
#include "Math/Geo.h"
#include "Math/ViewMatrix.h"

#include "GameDumper/Dumps/offsets.hpp"
#include "GameDumper/Dumps/client_dll.hpp"
#include "GameDumper/Dumps/server_dll.hpp"

using namespace Memory;

using namespace cs2_dumper::offsets::client_dll;
using namespace cs2_dumper::schemas::client_dll;
using namespace cs2_dumper::schemas::client_dll::C_BaseEntity;
using namespace cs2_dumper::schemas::client_dll::C_BasePlayerPawn;
using namespace cs2_dumper::schemas::client_dll::C_BaseModelEntity;
using namespace cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase;
using namespace cs2_dumper::schemas::client_dll::CCSPlayerController;

namespace Engine
{
	const Dimension SD = { 1920, 1080 };
	const Dimension SD_H = { SD.w / 2, SD.h / 2 };

	DWORD PID;
	uintptr_t CLIENT_DLL;

	ViewMatrix VM;
	uintptr_t ENTITIES_LIST;

	const wchar_t *GAME_EXE = L"cs2.exe";
	const wchar_t *CLIENT_DLL_NAME = L"client.dll";

	void Init()
	{
		PID = Memory::GetProcessID(GAME_EXE);
		CLIENT_DLL = Memory::GetModuleBaseAddress(PID, CLIENT_DLL_NAME);
	}

	ptrdiff_t EntityEntryDiff(const ptrdiff_t &ptr, const ptrdiff_t &mul = 0x7ff)
	{
		return (0x8 * (ptr & mul) >> 0x9) + 16;
	}

	ptrdiff_t EntityDiff(const ptrdiff_t &ptr)
	{
		return 120 * (ptr & 0x1ff);
	}

	template <typename T>
	T ReadClient(const ptrdiff_t &ptr_diff)
	{
		return Read<T>(CLIENT_DLL + ptr_diff);
	};

	template <typename T>
	T WriteClient(const ptrdiff_t &ptr_diff, const T &value)
	{
		return Write<T>(CLIENT_DLL + ptr_diff, value);
	};

	template <typename T>
	T ReadEntities(const ptrdiff_t &ptr_diff)
	{
		return Read<T>(ENTITIES_LIST + ptr_diff);
	};

	template <typename T>
	T ReadLocalPlayer()
	{
		return ReadClient<uintptr_t>(dwLocalPlayerPawn);
	};
}
