#pragma once

#include "Module.h"

struct EspConfig
{
	bool pBx, pName, pHealth, pArmor, pBxHealth, pBxArmor;
	GuiColor pBxCol, pBxBorderCol, pHealthCol, pArmorCol, pBxHealthCol, pBxArmorCol;
	Position pBxPosOffset, pBxDimOffset;
	std::map<std::string, std::string> objects;
};

class EspModule : public Module
{
	void Execute() override
	{
		this->UpdatePointers(this->rootModule);
	}

	void RenderPlayerSkeleton(Player &player)
	{
		const float headRadius = player.screen_d.h / 16;
		Gui::DrawFilledCircle(player.screenEye, headRadius, Green25);
		Gui::DrawCircle(player.screenEye, headRadius, White50);

		for (const auto &line : player.screenBones)
			Gui::DrawLine(line.p1, line.p2);
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

		Gui::DrawFilledRectangle(BOX_POS, BOX_D, GameRed);

		if (player.armor)
		{
			const Dimension BOX_D2{BORDER_D.w / sections, BORDER_D.h * armorRatio};
			const Position BOX_POS2{BORDER_POS.x + BOX_D2.w, BORDER_POS.y + BORDER_D.h - BOX_D2.h};
			Gui::DrawFilledRectangle(BOX_POS2, BOX_D2, GameBlue);
		}

		Gui::DrawRectangle(BORDER_POS, BORDER_D);
	}

	void RenderPlayerBox(Player &player)
	{
		Position ESP_P;
		Dimension ESP_D;
		player.GetEsp(ESP_P, ESP_D);

		Gui::DrawRectangle(ESP_P, ESP_D, White50);
		Gui::DrawTextual(ESP_P + Position{ESP_D.w + 4, 0}, player.name.data());
	}

	void RenderGameObjects(Player &player)
	{
		if (!C4Bomb.FuseChrono.IsRunning())
			return;

		const Position pos = GetClientCenterPosition();
		Gui::DrawTextual(pos, ("Time left: " + C4Bomb.FuseChrono.ToString()).data(), GetTimerColor(C4Bomb.FuseChrono));
		Gui::DrawTextual({pos.x, pos.y + 16}, ("Defuse Kit: " + C4Bomb.DefuseKitChrono.ToString()).data(), GetTimerColor(C4Bomb.DefuseKitChrono));
		Gui::DrawTextual({pos.x, pos.y + 32}, ("Defuse: " + C4Bomb.DefuseChrono.ToString()).data(), GetTimerColor(C4Bomb.DefuseChrono));
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

		Gui::DrawFilledCircle(GetClientCenterPosition(), 32, White12);
		Gui::DrawCircle(GetClientCenterPosition(), 32, White25);
	}
};
