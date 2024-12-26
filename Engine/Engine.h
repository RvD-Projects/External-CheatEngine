#pragma once

#include <vector>

#include "Memory.h"
#include "Math/ViewMatrix.h"

#include "GameDumper/Dumps/offsets.hpp"
#include "GameDumper/Dumps/client_dll.hpp"
#include "GameDumper/Dumps/server_dll.hpp"

namespace Engine
{
	inline DWORD PID;
	inline uintptr_t CLIENT_DLL;

	BYTE TEAM;
	ViewMatrix VM;
	uintptr_t ENTITIES_LIST, PLAYER_PAWN, PLAYER_CTRL;

	std::vector<uintptr_t> ENTITIES;
	std::vector<uintptr_t> ENEMIES;
	std::vector<uintptr_t> FRIENDLIES;

	inline const wchar_t *GAME_EXE = L"cs2.exe";
	inline const wchar_t *CLIENT_DLL_NAME = L"client.dll";
	inline const wchar_t *SERVER_DLL_NAME = L"server.dll";

	void Init()
	{
		PID = Memory::GetProcessID(GAME_EXE);
		CLIENT_DLL = Memory::GetModuleBaseAddress(PID, CLIENT_DLL_NAME);
	}
}
