#pragma once

#include "Module.h"
#include "EspModule.h"
#include "AimBotModule.h"

class RootModule : public Module
{
	EspModule *Esp;
	AimBotModule *AimBot;

	void Init() override
	{
		this->Esp = new EspModule();
		this->AimBot = new AimBotModule();

		this->Esp->Init(this);
		this->AimBot->Init(this);
	}

	void Execute() override
	{
		UpdateEntities();
		UpdateGamesRules();
	};

	void UpdateGamesRules()
	{
		C4Bomb.Update();
	}

	void UpdateEntities()
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

			SetPlayerScreenDef(VM, player);
			b.emplace_back(player);

			if (player.isLocalPlayer)
			{
				MyLocalPlayer = player;
				continue;
			}

			player.isTeammate
				? bF.emplace_back(player)
				: bE.emplace_back(player);
		}

		ENTITIES = b;
		ENEMIES = bE;
		FRIENDLIES = bF;
	}

	void SetPlayerScreenDef(const ViewMatrix &VM, Player &player) const
	{
		Position FEET, EYES;
		if (Geo::Get2DVector(player.viewCamPos, EYES, VM.matrix, GetClientDimension()))
		{
			player.screenEye = EYES;
			if (Geo::Get2DVector(player.position, FEET, VM.matrix, GetClientDimension()))
			{
				player.screenFeet = FEET;
				player.screen_d.h = (player.screenFeet.y - player.screenEye.y) * 1.1777f;
				player.screen_d.w = player.screen_d.h * 0.777f;

				player.esp_d = player.screen_d;
				player.esp_p = Position{
					player.screenFeet.x - (player.esp_d.w * 0.5f),
					player.screenFeet.y - player.esp_d.h,
				};
			}
		}
	};

public:
	void Render() override
	{
		if (this->Esp)
			this->Esp->Render();

		if (this->AimBot)
			this->AimBot->Render();

		Gui::DrawTextual({2, 2}, "Overlay v 1.0.0");
	}
};
