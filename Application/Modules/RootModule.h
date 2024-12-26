#pragma once

#include <vector>
#include <thread>
#include <cstdint>

#include "Module.h"
#include "EspModule.h"

#include "../../Engine/Math/Geo.h"

class RootModule : public Module
{
	EspModule esp;

	void _Render()
	{
		Gui::DrawTextual({0, 0}, "Overlay v 1.0.0", 64);
		Gui::DrawRectangle({0, 0}, {1920, 1080}, {255, 0, 0, 255});

		for (uintptr_t entity : ENTITIES)
		{
			vec3 ABS_ORIG = Read<vec3>(entity + m_vOldOrigin);
			vec3 PAWN_VC = Read<vec3>(entity + m_vecViewOffset);

			vec3 VC_POS = ABS_ORIG + PAWN_VC;

			vec2 HEAD_SCREEN, FEET_SCREEN;
			const bool HEAD_V = Geo::Get2DVector(ABS_ORIG, HEAD_SCREEN, VM.matrix);
			if (!HEAD_V)
				continue;

			const bool FEET_V = Geo::Get2DVector(VC_POS, FEET_SCREEN, VM.matrix);
			if (!FEET_V)
				continue;

			const Dimension PAWN_D{HEAD_SCREEN.y - FEET_SCREEN.y, HEAD_SCREEN.y - FEET_SCREEN.y};

			Gui::DrawRectangle(HEAD_SCREEN, PAWN_D);
		}
	}

	void _PreparePointers()
	{
		Engine::VM = ReadClient<ViewMatrix>(dwViewMatrix);
		Engine::PLAYER_PAWN = ReadClient<uintptr_t>(dwLocalPlayerPawn);
		Engine::PLAYER_CTRL = ReadClient<uintptr_t>(dwLocalPlayerController);

		Engine::TEAM = ReadPlayerPawn<BYTE>(m_iTeamNum);
		const uint32_t iHealth = ReadPlayerPawn<uint32_t>(m_iHealth);
		const bool bTakesDamage = ReadPlayerPawn<bool>(m_bTakesDamage);
		const uint8_t lifeState = ReadPlayerPawn<uint8_t>(m_lifeState);
		const uint32_t iMaxHealth = ReadPlayerPawn<uint32_t>(m_iMaxHealth);
		const uint32_t hammerID = ReadPlayerPawn<uint32_t>(m_sUniqueHammerID);
	}

	void _ParseEntities()
	{
		Engine::ENTITIES = {};
		Engine::ENTITIES_LIST = ReadClient<uintptr_t>(dwEntityList);

		for (int i = 1; i < 32; i++)
		{
			std::ptrdiff_t diff = (8 * (i & 0x7FFF) >> 9) + 16;
			uintptr_t LIST_ENTRY = ReadEntities<uintptr_t>(diff);
			if (!LIST_ENTRY)
				continue;

			diff = 120 * (i & 0x1FF);
			const uintptr_t ENTITY_CTRL = Read<uintptr_t>(LIST_ENTRY + diff);
			if (!ENTITY_CTRL)
				continue;

			const uintptr_t PAWN_CTRL = Read<uintptr_t>(ENTITY_CTRL + m_hPlayerPawn);
			if (!PAWN_CTRL)
				continue;

			diff = 120 * (PAWN_CTRL & 0x1FF);
			const uintptr_t ENTITY = Read<uintptr_t>(LIST_ENTRY + diff);
			if (!ENTITY)
				continue;

			diff = 8 * ((PAWN_CTRL & 0x7FFF) >> 9) + 16;
			LIST_ENTRY = ReadEntities<uintptr_t>(diff);
			if (!LIST_ENTRY)
				continue;

			diff = 120 * (PAWN_CTRL & 0x1FF);
			const uintptr_t PAWN = Read<uintptr_t>(LIST_ENTRY + diff);
			if (!PAWN)
				continue;

			const int TEAM = Read<int>(PAWN + m_iTeamNum);
			const int HEALT = Read<int>(PAWN + m_iHealth);
			const std::string NAME = Read<std::string>(PAWN + m_sSanitizedPlayerName);

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
