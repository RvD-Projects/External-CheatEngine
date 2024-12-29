#pragma once

#include "Module.h"

struct EspConfig
{
	bool pBx, pName, pHealth, pArmor, pBxHealth, pBxArmor;
	GuiColor pBxCol, pBxBorderCol, pHealthCol, pArmorCol, pBxHealthCol, pBxArmorCol;
	Position pBxPosOffset, pBxDimOffset;
};

class EspModule : public Module
{
	void Execute() override
	{
		this->UpdatePointers(this->rootModule);
	}

	void GetEspBox(const Position &TOP, const Position &BOTTOM, Position &ESP_P, Dimension &ESP_D)
	{
		const float HEIGHT = (BOTTOM.y - TOP.y);
		const float WIDTH = HEIGHT * 0.777f;

		ESP_D.h = HEIGHT * 1.1777f;
		ESP_D.w = WIDTH;

		ESP_P.x = BOTTOM.x - (ESP_D.w * 0.5f);
		ESP_P.y = BOTTOM.y - ESP_D.h;
	}

	void RenderPlayerBox(const Player &player)
	{
		Position ESP_P;
		Dimension ESP_D;
		GetEspBox(player.eyeScreen, player.feetScreen, ESP_P, ESP_D);

		Gui::DrawRectangle(ESP_P, ESP_D, White50);
		Gui::DrawTextual(ESP_P + Position{ESP_D.w + 4, 0}, player.name.data(), 8);
		Gui::DrawTextual(ESP_P + Position{ESP_D.w + 4, 16}, std::to_string(player.health).append(" HP").data(), 8, Red64);

		if (player.armor)
		{
			Gui::DrawTextual(ESP_P + Position{ESP_D.w + 4, 32}, std::to_string(player.armor).append(" AP").data(), 8, Blue64);
		}
	}

	void RenderPlayerBoxStats(const Player &player)
	{
		Position ESP_P;
		Dimension ESP_D;
		GetEspBox(player.eyeScreen, player.feetScreen, ESP_P, ESP_D);

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

	void RenderPlayerSkeleton(const Player &player)
	{
		Position ESP_P;
		Dimension ESP_D;
		GetEspBox(player.eyeScreen, player.feetScreen, ESP_P, ESP_D);

		Gui::DrawFilledCircle(player.eyeScreen, 32, Green25);
		Gui::DrawCircle(player.eyeScreen, 32, White50);
	}

public:
	void Render() override
	{
		for (Player player : ENEMIES)
		{
			if (!player.IsValidTarget())
				continue;

			RenderPlayerSkeleton(player);
			RenderPlayerBox(player);
			RenderPlayerBoxStats(player);
		}

		const Dimension half = GetClientDimension() / 2;
		Gui::DrawFilledCircle({half.w, half.h}, 64, White12);
	}
};
