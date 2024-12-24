#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

namespace Memory
{
	inline HANDLE gHandle;

	inline uintptr_t PID;

	DWORD GetProcessID(const wchar_t* process);
	HMODULE GetModuleBaseAddress(DWORD procID, const wchar_t* module);

	template <typename T> T Read(uintptr_t address)
	{
		T ret;
		ReadProcessMemory(gHandle, (LPCVOID)address, &ret, sizeof(T), nullptr);
		return ret;
	}
	template <typename T> bool Write(uintptr_t address, T value)
	{
		return WriteProcessMemory(gHandle, (LPVOID)address, &value, sizeof(T), nullptr);
	}
}

