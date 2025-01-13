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

long long refreshRate = 5;
bool isReady = false, isRunning = false, isMainWindowHidden = false;

const wchar_t *APP_NAME = L"CS2 Overlay";
const wchar_t *EXE_NAME = L"Application.exe";

/**
 * Toggles the display affinity of a specified window.
 *
 * This function changes the visibility state of a window by toggling
 * the `isMainWindowHidden` atomic boolean. It uses the `SetWindowDisplayAffinity`
 * function to set the window's display affinity based on the current
 * state of `isMainWindowHidden`. If no window handle is provided, the default
 * value is NULL.
 *
 * @param window A constant reference to the window handle (HWND) for
 * @return The new state of the `isMainWindowHidden` boolean, indicating whether
 *         the window is now hidden.
 */
bool ToggleWindowAffinity(const HWND &window = NULL)
{
    ::isMainWindowHidden = !::isMainWindowHidden;
    SetWindowDisplayAffinity(window, ::isMainWindowHidden ? 17 : 0);

    return ::isMainWindowHidden;
}

void UpdateOverlayPosition(HWND window)
{
    if (GetAsyncKeyState(VK_F8))
    {
        ToggleWindowAffinity(window);
    }

    if (GetAsyncKeyState(VK_F9))
    {
        ::isRunning = false;
        return;
    }

    APP_INFO TargetClient = Engine::TargetClient;
    auto pos = TargetClient.position;
    auto dim = TargetClient.dimension;

    SetWindowPos(window, HWND_TOPMOST, pos.x, pos.y, dim.w, dim.h, SWP_NOZORDER | SWP_NOACTIVATE);

    ImGui::GetIO().DisplaySize.x = dim.w;
    ImGui::GetIO().DisplaySize.y = dim.h;
    ImGui::SetNextWindowSize({dim.w, dim.h});
    ImGui::SetNextWindowPos({pos.x, pos.y});

    // Apply the DPI scale factor to the ImGui context
    ImGui::GetIO().FontGlobalScale = ImGui_ImplWin32_GetDpiScaleForHwnd(window);
}

/**
 * Executes the main loop for the application, continuously updating the overlay position.
 *
 * @param appWindow The handle to the application window.
 *
 * The function sets the global running state to true and enters a loop that continues
 * as long as the application is running. It checks if the application and engine are ready,
 * pausing for 2000 milliseconds if not. Once ready, it updates the overlay position
 * and pauses for a duration defined by the refresh rate.
 */
void Execute(HWND appWindow)
{
    ::isRunning = true;
    while (::isRunning)
    {
        if (!::isReady || !Engine::isReady)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }

        UpdateOverlayPosition(appWindow);
        std::this_thread::sleep_for(std::chrono::milliseconds(::refreshRate));
    }
}

/**
 * Starts the execution of the application by initializing the engine
 * with the specified target executable and window names, and launching
 * a detached thread to execute the overlay update loop.
 *
 * @param targetExeName The name of the target executable file.
 * @param targetWindowName The name of the target window.
 * @param appWindow The handle to the application window.
 */
void Start(const wchar_t *targetExeName, const wchar_t *targetWindowName, HWND appWindow)
{
    Engine::Run(targetExeName, targetWindowName, {"client.dll", "server.dll"});
    std::thread(
        [appWindow]()
        { Execute(appWindow); })
        .detach();
}