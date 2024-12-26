#pragma once

#include "Module.h"
#include "EspModule.h"

class RootModule : public Module
{
	EspModule esp;

	void _Render()
	{
		Gui::DrawTextual({ 0, 0 }, "Overlay v 1.0.0");
		Gui::DrawRectangle({ 0, 0 }, { 1920, 1080 }, { 255, 0, 0, 255 });
	}

	void _ParsePointers()
	{
		VM = ReadClient<ViewMatrix>(dwViewMatrix);
		ENTITIES_LIST = ReadClient<uintptr_t>(dwEntityList);

		std::vector<Player> b = {};
		std::vector<Player> bE = {};
		std::vector<Player> bF = {};

		for (int i = 1; i < 32; i++)
		{
			Player player(ENTITIES_LIST, i);

			if (!player.isInitialized) continue;

			b.emplace_back(player);

			player.isLocalPlayerTeam = player.isLocalPlayer || PLAYER.team == player.team;

			player.isLocalPlayer
				? PLAYER = player
				: (player.isLocalPlayerTeam
					? bF.emplace_back(player)
					: bE.emplace_back(player));
		}

		ENTITIES = b;
		ENEMIES = bE;
		FRIENDLIES = bF;

		esp.ENEMIES = ENEMIES;
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
		while (true)
		{
			_ParsePointers();
			Sleep(10);
		}
	}

	void Render() override
	{
		_Render();
		esp.Render();
	}
};
