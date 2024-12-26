#pragma once

#include "Module.h"
#include "EspModule.h"
#include "AimBotModule.h"

class RootModule : public Module
{
	EspModule esp;
	AimBotModule aimBot;

	void _Render()
	{
		Gui::DrawTextual({ 0, 0 }, "Overlay v 1.0.0");
		Gui::DrawRectangle({ 0, 0 }, SD, { 255, 0, 0, 255 });
	}

	void _Execute()
	{
		VM = ReadClient<ViewMatrix>(dwViewMatrix);
		ENTITIES_LIST = ReadClient<uintptr_t>(dwEntityList);

		std::vector<Player> b = {};
		std::vector<Player> bE = {};
		std::vector<Player> bF = {};

		for (int i = 1; i < 32; i++)
		{
			Player player = Player(ENTITIES_LIST, i);

			if (!player.isInitialized)
				continue;

			if (GetLocalPlayer_T() == player.entity)
			{
				player.isLocalPlayer = true;
				player.crossIndex = ReadLocalPlayer<int>(m_iIDEntIndex);
				MyLocalPlayer = player;
			}

			player.isLocalPlayerTeam = !player.isLocalPlayer 
				&& GameIsTeamPlay()
				&& ReadLocalPlayer<int>(m_iTeamNum) == player.team;

			b.emplace_back(player);

			if (player.isLocalPlayer) {
				continue;
			}

			player.isLocalPlayerTeam
				? bF.emplace_back(player)
				: bE.emplace_back(player);
		}

		ENTITIES = b;
		ENEMIES = bE;
		FRIENDLIES = bF;
	}

public:
	void Init() override
	{
		esp.Init();
		aimBot.Init();
	}

	void Loop() override
	{
		while (true)
		{
			_Execute();

			esp.Execute();
			aimBot.Execute();
			Sleep(16);
		}
	}

	void Render() override
	{
		_Render();

		esp.Render();
		aimBot.Render();
	}
};
