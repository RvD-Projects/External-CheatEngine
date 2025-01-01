#pragma once

#include "Module.h"

class EspModule : public Module
{
	void RenderPlayerSkeleton(Player &player)
	{
		if (!player.screenBones.size())
			return;

		for (const auto &line : player.screenBones)
			DrawLine(line.pStart, line.pEnd, White75, 2.5f);

		const float headRadius = player.screenBox.d.h / 13;
		const Position headPos = player.screenBones[0].pStart;

		DrawFilledCircle(headPos, headRadius, White25);
		DrawCircle(headPos, headRadius, White75, 2.5f);
	}

	void RenderPlayerBoxStats(Player &player)
	{
		const float healthRatio = (float)player.health / player.maxHealth;
		const float armorRatio = (float)player.armor / 100.0f;

		const float divider = 12.f;
		const float sections = player.armor ? 2.f : 1.f;

		const Position BORDER_POS = player.screenBox.pStart;
		const Dimension BORDER_D{player.screenBox.d.w / divider, player.screenBox.d.h};

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
		DrawRectangle(player.screenBox.pStart, player.screenBox.d, White50);
		DrawTextual(player.screenBox.pStart + Position{0, -16}, player.name.data(), White50);
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
		if (config.isHidden || !config.isActive || !config.isReady)
			return;

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
