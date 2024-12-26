#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#include "GameDumper/Dumps/offsets.hpp"
#include "GameDumper/Dumps/client_dll.hpp"
#include "GameDumper/Dumps/server_dll.hpp"

using namespace cs2_dumper::offsets::client_dll;
using namespace cs2_dumper::schemas::client_dll;
using namespace cs2_dumper::schemas::client_dll::C_BaseEntity;
using namespace cs2_dumper::schemas::client_dll::C_CSGameRules;
using namespace cs2_dumper::schemas::client_dll::C_BasePlayerPawn;
using namespace cs2_dumper::schemas::client_dll::C_BaseModelEntity;
using namespace cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase;
using namespace cs2_dumper::schemas::client_dll::CCSPlayerController;

namespace Memory
{
	HANDLE G_HANDLE;
	uintptr_t G_PID;

	HANDLE GetHandle(const DWORD flags, const DWORD processID)
	{
		HANDLE handle = CreateToolhelp32Snapshot(flags, processID);
		if (handle == INVALID_HANDLE_VALUE)
		{
			std::wcerr << L"GetHandle (processId) failed" << std::endl;
			return NULL;
		}

		return handle;
	}

	uintptr_t GetProcessID(const wchar_t *process)
	{
		HANDLE handle = GetHandle(TH32CS_SNAPPROCESS, NULL);
		if (!handle)
			return NULL;

		PROCESSENTRY32 proc;
		proc.dwSize = sizeof(PROCESSENTRY32);
		Process32First(handle, &proc);

		do
		{
			if (!wcscmp(process, proc.szExeFile))
			{
				CloseHandle(handle);
				G_PID = proc.th32ProcessID;
				G_HANDLE = OpenProcess(PROCESS_ALL_ACCESS, NULL, G_PID);
				return proc.th32ProcessID;
			}
		} while (Process32Next(handle, &proc));

		CloseHandle(handle);
		return 0;
	}

	// Function to get the module base address of a process
	uintptr_t GetModuleBaseAddress(DWORD processID, const wchar_t *moduleName)
	{
		HANDLE handle = GetHandle(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
		if (!handle)
			return NULL;

		MODULEENTRY32 mod;
		mod.dwSize = sizeof(MODULEENTRY32);
		Module32First(handle, &mod);

		do
		{
			if (!wcscmp(moduleName, mod.szModule))
			{
				CloseHandle(handle);
				return (uintptr_t)mod.modBaseAddr;
			}
		} while (Module32Next(handle, &mod));

		CloseHandle(handle);
		return NULL;
	}

	ptrdiff_t EntityEntryDiff(const ptrdiff_t& ptr, const ptrdiff_t& mul = 0x7ff)
	{
		return (0x8 * (ptr & mul) >> 0x9) + 16;
	}

	ptrdiff_t EntityDiff(const ptrdiff_t& ptr)
	{
		return 120 * (ptr & 0x1ff);
	}

	template <typename T>
	T Read(const uintptr_t &address)
	{
		T ret;
		ReadProcessMemory(G_HANDLE, (LPCVOID)address, &ret, sizeof(T), nullptr);
		return ret;
	}

	template <typename T>
	bool Write(const uintptr_t &address, const T &value)
	{
		return WriteProcessMemory(G_HANDLE, (LPVOID)address, &value, sizeof(T), nullptr);
	}

	std::string ReadString(const uintptr_t &address, const size_t &maxLength = 256)
	{
		std::vector<char> buffer(maxLength);
		size_t i = 0;

		// Read characters one by one until we hit the null terminator or the max length
		while (i < maxLength)
		{
			buffer[i] = Read<char>(address + i);
			if (buffer[i] == '\0')
			{
				break;
			}
			i++;
		}

		return std::string(buffer.data());
	}
}
