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
using namespace cs2_dumper::schemas::client_dll::CCSPlayerController;

namespace Engine
{
	DWORD PID;
	uintptr_t CLIENT_DLL;

	ViewMatrix VM;
	uintptr_t ENTITIES_LIST;

    const wchar_t* GAME_EXE = L"cs2.exe";
    const wchar_t* CLIENT_DLL_NAME = L"client.dll";

	void Init()
	{
		PID = Memory::GetProcessID(GAME_EXE);
		CLIENT_DLL = Memory::GetModuleBaseAddress(PID, CLIENT_DLL_NAME);
	}

	ptrdiff_t EntityEntryDiff(const uintptr_t& ptr) {
		return (0x8 * (ptr & 0x7ff) >> 0x9) + 16;
	}

	ptrdiff_t EntityDiff(const uintptr_t& ptr) {
		return 120 * (ptr & 0x1ff);
	}
		
	template <typename T>
	T ReadClient(ptrdiff_t ptr_diff)
	{
		return Read<T>(CLIENT_DLL + ptr_diff);
	};

	template <typename T>
	T ReadEntities(ptrdiff_t ptr_diff)
	{
		return Read<T>(ENTITIES_LIST + ptr_diff);
	};
}
