#include "memory.h"

// Function to get the PID of a process by name
DWORD Memory::GetProcessID(const wchar_t* process) {
    HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (handle == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 proc;
    proc.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(handle, &proc)) {
        do {
            if (!wcscmp(process, proc.szExeFile)) {
                CloseHandle(handle);
                return proc.th32ProcessID;
            }
        } while (Process32Next(handle, &proc));
    }

    CloseHandle(handle);
    return 0;
}

// Function to get the module base address of a process
HMODULE Memory::GetModuleBaseAddress(DWORD processID, const wchar_t* moduleName) {
    HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
    if (handle == INVALID_HANDLE_VALUE) {
        std::wcerr << L"CreateToolhelp32Snapshot (module) failed" << std::endl;
        return NULL;
    }

    MODULEENTRY32 mod;
    mod.dwSize = sizeof(MODULEENTRY32);
    if (Module32First(handle, &mod)) {
        do {
            std::wcout << L"Module: " << mod.szModule << std::endl; // Debug print
            if (!wcscmp(moduleName, mod.szModule)) {
                CloseHandle(handle);
                return mod.hModule;
            }
        } while (Module32Next(handle, &mod));
    }
    else {
        std::wcerr << L"Module32First failed" << std::endl;
    }

    CloseHandle(handle);
    return NULL;
}