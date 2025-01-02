#pragma once

#include "Module.h"

class EspModule : public Module
{
	EspConfig config;

	void Execute()
	{
		// uses F1 to toggle skeleton
		if (GetAsyncKeyState(VK_F1) & 1)
			config.isSkeletonActive = !config.isSkeletonActive;

		// uses F2 to toggle box
		if (GetAsyncKeyState(VK_F2) & 1)
			config.isBoxActive = !config.isBoxActive;

		// uses F3 to toggle stats
		if (GetAsyncKeyState(VK_F3) & 1)
			config.isStatsActive = !config.isStatsActive;

		// uses F4 to toggle esp
		if (GetAsyncKeyState(VK_F4) & 1)
			config.isActive = !config.isActive;
	}

	void RenderPlayerSkeleton(Player &player)
	{
		if (!player.screenBones.size())
			return;

		if (config.isSkeletonActive)
		{
			for (const auto &line : player.screenBones)
				DrawLine(line.pStart, line.pEnd, White50, 2.5f);
		}

		const float headRadius = player.screenBox.d.h / 14;
		const Position headPos = player.screenBones[0].pStart;

		DrawFilledCircle(headPos, headRadius, White25);
		DrawCircle(headPos, headRadius, White50, 2);
	}

	void RenderPlayerBoxStats(Player &player)
	{
		if (!config.isStatsActive)
			return;

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
		if (!config.isBoxActive)
			return;

		DrawRectangle(player.screenBox.pStart, player.screenBox.d, White50);
		DrawTextual(player.screenBox.pStart + Position{0, -16}, player.name.data(), White50);
	}

	void RenderGameObjects(Player &player)
	{
		if (!C4Bomb.FuseChrono.IsRunning())
			return;

		const Position boxPos{8, ClientDimension.h - 80};
		const Position textStart{boxPos.x + 8, boxPos.y + 8};

		// Draw the box based on text size
		DrawFilledRectangle(boxPos, {180, 64}, Black75);
		DrawRectangle(boxPos, {180, 64}, White75);

		DrawTextual(textStart, ("Time left: " + C4Bomb.FuseChrono.ToString()).data(), GetTimerColor(C4Bomb.FuseChrono));
		DrawTextual({textStart.x, textStart.y + 16}, ("Defuse Kit: " + C4Bomb.DefuseKitChrono.ToString()).data(), GetTimerColor(C4Bomb.DefuseKitChrono));
		DrawTextual({textStart.x, textStart.y + 32}, ("Defuse: " + C4Bomb.DefuseChrono.ToString()).data(), GetTimerColor(C4Bomb.DefuseChrono));
	}

public:
	void Init(Module *rootModule) override
	{
		this->config.isActive = true;
		this->UpdatePointers(rootModule);
		this->config.isReady = true;
	}

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
	}
};
