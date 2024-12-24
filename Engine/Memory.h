#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

namespace Memory
{
    //DWORD PID;
    HANDLE gHandle;

    // Function to get the PID of a process by name
    DWORD GetProcessID(const wchar_t *process)
    {
        HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (handle == INVALID_HANDLE_VALUE)
        {
            return 0;
        }

        PROCESSENTRY32 proc;
        proc.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(handle, &proc))
        {
            do
            {
                if (!wcscmp(process, proc.szExeFile))
                {
                    CloseHandle(handle);
                    return proc.th32ProcessID;
                }
            } while (Process32Next(handle, &proc));
        }

        CloseHandle(handle);
        return 0;
    }

    // Function to get the module base address of a process
    HMODULE GetModuleBaseAddress(DWORD processID, const wchar_t *moduleName)
    {
        HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
        if (handle == INVALID_HANDLE_VALUE)
        {
            std::wcerr << L"CreateToolhelp32Snapshot (module) failed" << std::endl;
            return NULL;
        }

        MODULEENTRY32 mod;
        mod.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(handle, &mod))
        {
            do
            {
                std::wcout << L"Module: " << mod.szModule << std::endl;
                if (!wcscmp(moduleName, mod.szModule))
                {
                    CloseHandle(handle);
                    return mod.hModule;
                }
            } while (Module32Next(handle, &mod));
        }
        else
        {
            std::wcerr << L"Module32First failed" << std::endl;
        }

        CloseHandle(handle);
        return NULL;
    }

    template <typename T>
    T Read(HMODULE address)
    {
        T ret;
        ReadProcessMemory(gHandle, (LPCVOID)address, &ret, sizeof(T), nullptr);
        return ret;
    }

    template <typename T>
    bool Write(HMODULE address, T value)
    {
        return WriteProcessMemory(gHandle, (LPVOID)address, &value, sizeof(T), nullptr);
    }
}
