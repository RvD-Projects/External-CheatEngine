#pragma once

#include <vector>
#include <cstdint>

#include "Module.h"
#include "EspModule.h"

#include "../../ImGui/imgui.h"
#include "../../Engine/Engine.h"
#include "../../Engine/Math/Geo.h"
#include <thread>

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

	void _Render()
	{
		Gui::DrawTextual({0, 0}, "Overlay v 1.0.0", 64);
		Gui::DrawRectangle({0, 0}, {1920, 1080}, {255, 0, 0, 255});

		for (uintptr_t entity : ENTITIES)
		{
			vec3 ABS_ORIG = Memory::Read<vec3>(entity + m_vOldOrigin);
			vec3 PAWN_VC = Memory::Read<vec3>(entity + m_vecViewOffset);
			vec3 VC_POS = ABS_ORIG + PAWN_VC;

			vec2 HEAD_SCREEN, FEET_SCREEN;
			const bool HEAD_V = Geo::Get2DVector(ABS_ORIG, HEAD_SCREEN, VM.matrix);
			if (!HEAD_V)
				continue;

			const bool FEET_V = Geo::Get2DVector(VC_POS, FEET_SCREEN, VM.matrix);
			if (!FEET_V)
				continue;

			const Dimension PAWN_D{HEAD_SCREEN.x - FEET_SCREEN.x, HEAD_SCREEN.y - FEET_SCREEN.y};

			Gui::DrawRectangle(HEAD_SCREEN, PAWN_D);
		}
	}

	void _PreparePointers()
	{
		Engine::VM = Memory::Read<ViewMatrix>(CLIENT_DLL + dwViewMatrix);
		Engine::ENTITIES_LIST = Memory::Read<uintptr_t>(CLIENT_DLL + dwEntityList);
		Engine::PLAYER_PAWN = Memory::Read<uintptr_t>(CLIENT_DLL + dwLocalPlayerPawn);

		Engine::TEAM = Memory::Read<BYTE>(PLAYER_PAWN + m_iTeamNum);
		const uint32_t iHealth = Memory::Read<uint32_t>(PLAYER_PAWN + m_iHealth);
		const uint32_t iMaxHealth = Memory::Read<uint32_t>(PLAYER_PAWN + m_iMaxHealth);
		const uint8_t lifeState = Memory::Read<uint8_t>(PLAYER_PAWN + m_lifeState);
		const bool bTakesDamage = Memory::Read<bool>(PLAYER_PAWN + m_bTakesDamage);
	}

	void _ParseEntities()
	{
		Engine::ENTITIES = {};
		for (int i = 1; i < 32; i++)
		{
			const uintptr_t LIST_ENTRY = Memory::Read<uintptr_t>(ENTITIES_LIST + ((8 * (i & 0x7ff) >> 9) + 16));
			if (!LIST_ENTRY)
				continue;

			const uintptr_t ENTITY_CTRL = Memory::Read<uintptr_t>(LIST_ENTRY + 120 * (i & 0x1ff));
			if (!ENTITY_CTRL)
				continue;

			const uintptr_t PAWN_CTRL = Memory::Read<uintptr_t>(ENTITY_CTRL + m_hPlayerPawn);
			if (!PAWN_CTRL)
				continue;

			const uintptr_t ENTITY = Memory::Read<uintptr_t>(LIST_ENTRY + 120 * (PAWN_CTRL & 0x1ff));
			if (!ENTITY)
				continue;

			const std::string NAME = Memory::Read<std::string>(ENTITY_CTRL + m_sSanitizedPlayerName);
			if (!NAME.size())
				continue;

			const UINT32 HEALT = Memory::Read<UINT32>(ENTITY_CTRL + m_iPawnHealth);

			ENTITIES.emplace_back(ENTITY);
		}
	}

public:
	void Init() override
	{
		Engine::Init();

		_PreparePointers();

		EspModule esp{};
		esp.Init();
	}

	void Loop() override
	{
		while (true)
		{
			_ParseEntities();
			Sleep(16);
		}
	}

	void Render() override
	{
		_Render();
		esp.Render();
	}
};
