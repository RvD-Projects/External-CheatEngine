#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <dxgi.h>
#include <d3d11.h>
#include <dwmapi.h>
#include <d3dcommon.h>
#include <Uxtheme.h>
#include <thread>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"

#include "../Engine/Engine.h"

DWORD refreshRate = 16; // Update every ~16ms (60 FPS)
std::atomic<bool> isReady, isRunning, isHidden;

const wchar_t *APP_NAME = L"CS2 Overlay";
const wchar_t *EXE_NAME = L"Application.exe";

void UpdateOverlayPosition(HWND window)
{
    APP_INFO TargetClient = Engine::TargetClient;
    auto pos = TargetClient.position;
    auto dim = TargetClient.dimension;

    SetWindowPos(window, HWND_TOPMOST, pos.x, pos.y, dim.w, dim.h, SWP_NOZORDER | SWP_NOACTIVATE);

    // Apply the DPI scale factor to the ImGui context
    // ImGui::GetIO().FontGlobalScale = ImGui_ImplWin32_GetDpiScaleForHwnd(window);
}

void Execute(HWND appWindow)
{
    ::isRunning = true;
    while (::isRunning)
    {
        if (!::isReady || !Engine::isReady)
        {
            std::wcerr << L"Waiting for Engine to be ready..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }

        UpdateOverlayPosition(appWindow);
        std::this_thread::sleep_for(std::chrono::milliseconds(::refreshRate));
    }
}

void Start(const wchar_t *targetExeName, const wchar_t *targetWindowName, HWND appWindow)
{
    Engine::Run(targetExeName, targetWindowName, {"client.dll", "server.dll"});
    std::thread([appWindow]()
                { Execute(appWindow); })
        .detach();
}

bool ToggleWindowAffinity(const HWND &window)
{
    isHidden = !isHidden;
    SetWindowDisplayAffinity(window, isHidden ? 0x00000000 : 0x00000017);

    return isHidden;
}