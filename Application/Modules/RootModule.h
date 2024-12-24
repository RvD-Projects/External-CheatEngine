#pragma once

#include <vector>
#include <cstdint>

#include "Module.h"
#include "EspModule.h"

#include "../../ImGui/imgui.h"

#include "../../Engine/Engine.h"


using namespace Engine;
using namespace cs2_dumper::offsets::client_dll;
using namespace cs2_dumper::schemas::client_dll;
using namespace cs2_dumper::schemas::client_dll::C_BaseEntity;
using namespace cs2_dumper::schemas::client_dll::C_BasePlayerPawn;
using namespace cs2_dumper::schemas::client_dll::C_BaseModelEntity;
using namespace cs2_dumper::schemas::client_dll::CCSPlayerController;

class RootModule : public Module
{
	EspModule esp;
	ViewMatrix vm;

	void _Render() {
		Gui::DrawTextual({ 0,0 }, "Overlay v 1.0.0", 64);
		Gui::DrawRectangle({ 0, 0}, { 1920, 1080}, { 255, 0, 0, 255 });
	}

public:
	void Init() override
	{
		Engine::Init();

		EspModule esp{};
		esp.Init();
	}

	void Loop() override
	{
		Engine::TEAM = Memory::Read<BYTE>(CLIENT_DLL + m_iTeamNum);
		Engine::VM = Memory::Read<ViewMatrix>(CLIENT_DLL + dwViewMatrix);

		Engine::ENTITIES = Memory::Read<uintptr_t>(CLIENT_DLL + dwEntityList);
		Engine::PLAYER_PAWN = Memory::Read<uintptr_t>(CLIENT_DLL + dwLocalPlayerPawn);

		while (true)
		{
			std::vector<uintptr_t> buffer = {};

			for (int i = 1; i < 32; i++)
			{
				uintptr_t listEntry = memory::Read<uintptr_t>(entity_list + ((8 * (i & 0x7ff) >> 9) + 16));
				if (!listEntry)
					continue;

				uintptr_t entityController = memory::Read<uintptr_t>(listEntry + 120 * (i & 0x1ff));
				if (!entityController)
					continue;

				uintptr_t entityControllerPawn = memory::Read<uintptr_t>(entityController + m_hPlayerPawn);
				if (!entityControllerPawn)
					continue;

				uintptr_t entity = memory::Read<uintptr_t>(listEntry + 120 * (entityControllerPawn & 0x1ff));
				if (!entity)
					continue;

				buffer.emplace_back(entity);
			}

			entities = buffer;
			Sleep(16);
		}
	}

	void Render() override
	{
		_Render();
		esp.Render();
	}
};
