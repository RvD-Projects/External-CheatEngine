#pragma once

#include <vector>

#include "GameDumper/memory.h"
#include "GameDumper/Dumps/offsets.hpp"
#include "GameDumper/Dumps/client_dll.hpp"
#include "GameDumper/Dumps/server_dll.hpp"

namespace Engine
{
	inline DWORD PID;
	inline HMODULE CLIENT_DLL_ADDR;
	inline HMODULE SERVER_DLL_ADDR;
	std::vector<uintptr_t> ENTITIES;

	inline const wchar_t *GAME_EXE = L"cs2.exe";
	inline const wchar_t *CLIENT_DLL = L"client.dll";
	inline const wchar_t *SERVER_DLL = L"server.dll";

	void Init()
	{
		PID = Memory::GetProcessID(GAME_EXE);
		CLIENT_DLL_ADDR = Memory::GetModuleBaseAddress(PID, CLIENT_DLL);
		SERVER_DLL_ADDR = Memory::GetModuleBaseAddress(PID, SERVER_DLL);
	}
}
