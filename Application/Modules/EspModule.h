#pragma once

#include "Module.h"

class EspModule : public Module
{
	void Execute() override
	{
		this->config.isActive = true;

		this->UpdatePointers(this->rootModule);

		this->config.isReady = true;
	}

	void RenderPlayerSkeleton(Player &player)
	{
		const float headRadius = player.screen_d.h / 16;
		DrawFilledCircle(player.screenEye, headRadius, Green25);
		DrawCircle(player.screenEye, headRadius, White50);

		for (const auto &line : player.screenBones)
			DrawLine(line.pStart, line.pEnd, White75, 2.5f);
	}

	void RenderPlayerBoxStats(Player &player)
	{
		Position ESP_P;
		Dimension ESP_D;
		player.GetEsp(ESP_P, ESP_D);

		const float healthRatio = (float)player.health / player.maxHealth;
		const float armorRatio = (float)player.armor / 100.0f;

		const float divider = 12.f;
		const float sections = player.armor ? 2.f : 1.f;

		const Position BORDER_POS = ESP_P;
		const Dimension BORDER_D{ESP_D.w / divider, ESP_D.h};

		const Dimension BOX_D{BORDER_D.w / sections, BORDER_D.h * healthRatio};
		const Position BOX_POS{BORDER_POS.x, BORDER_POS.y + BORDER_D.h - BOX_D.h};

		DrawFilledRectangle(BOX_POS, BOX_D, GameRed);

		if (player.armor)
		{
			const Dimension BOX_D2{BORDER_D.w / sections, BORDER_D.h * armorRatio};
			const Position BOX_POS2{BORDER_POS.x + BOX_D2.w, BORDER_POS.y + BORDER_D.h - BOX_D2.h};
			DrawFilledRectangle(BOX_POS2, BOX_D2, GameBlue);
		}

		DrawRectangle(BORDER_POS, BORDER_D);
	}

	void RenderPlayerBox(Player &player)
	{
		Position ESP_P;
		Dimension ESP_D;
		player.GetEsp(ESP_P, ESP_D);

		DrawRectangle(ESP_P, ESP_D, White50);
		DrawTextual(ESP_P + Position{ESP_D.w + 4, 0}, player.name.data());
	}

	void RenderGameObjects(Player &player)
	{
		if (!C4Bomb.FuseChrono.IsRunning())
			return;

		const Position pos = ClientCenterPosition;
		DrawTextual(pos, ("Time left: " + C4Bomb.FuseChrono.ToString()).data(), GetTimerColor(C4Bomb.FuseChrono));
		DrawTextual({pos.x, pos.y + 16}, ("Defuse Kit: " + C4Bomb.DefuseKitChrono.ToString()).data(), GetTimerColor(C4Bomb.DefuseKitChrono));
		DrawTextual({pos.x, pos.y + 32}, ("Defuse: " + C4Bomb.DefuseChrono.ToString()).data(), GetTimerColor(C4Bomb.DefuseChrono));
	}

public:
	void Render() override
	{
		for (Player player : ENEMIES)
		{
			if (!player.IsValidTarget())
				continue;

			RenderPlayerSkeleton(player);
			RenderPlayerBoxStats(player);
			RenderPlayerBox(player);
			RenderGameObjects(player);
		}

		DrawFilledCircle(GetClientCenterPosition(), 32, White12);
		DrawCircle(GetClientCenterPosition(), 32, White25);
	}
};
