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

	void GetEspBox(Position &EYE_SCREEN, Position &FEET_SCREEN, Position &ESP_P, Dimension &ESP_D)
	{
		const float HEIGHT = (EYE_SCREEN.y - FEET_SCREEN.y) * 1.1777f;
		const float WIDTH = HEIGHT * 0.777f;

		ESP_D.w = WIDTH;
		ESP_D.h = HEIGHT;

		ESP_P.x = EYE_SCREEN.x - (WIDTH * 0.5f);
		ESP_P.y = EYE_SCREEN.y - HEIGHT;
	}

	void RenderPlayerBox(const Player &player, Position &EYE_SCREEN, Position &FEET_SCREEN)
	{
		Position ESP_P;
		Dimension ESP_D;
		GetEspBox(EYE_SCREEN, FEET_SCREEN, ESP_P, ESP_D);

		Gui::DrawRectangle(ESP_P, ESP_D, {255, 255, 255, 255});
		Gui::DrawTextual(ESP_P + Position{ESP_D.w + 4, 0}, player.name.data(), 8);
		Gui::DrawTextual(ESP_P + Position{ESP_D.w + 4, 16}, std::to_string(player.health).append(" HP").data(), 8, {255, 0, 0, 255});

		if (player.armor)
		{
			Gui::DrawTextual(ESP_P + Position{ESP_D.w + 4, 32}, std::to_string(player.armor).append(" AP").data(), 8, {0, 0, 255, 255});
		}
	}

	void RenderPlayerBoxStats(const Player &player, Position &EYE_SCREEN, Position &FEET_SCREEN)
	{
		Position ESP_P;
		Dimension ESP_D;
		GetEspBox(EYE_SCREEN, FEET_SCREEN, ESP_P, ESP_D);

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

public:
	void Render() override
	{
		for (Player player : ENEMIES)
		{
			if (!player.IsValidTarget())
				continue;

			Position EYE_SCREEN, FEET_SCREEN;
			if (!Geo::Get2DVector(player.position, EYE_SCREEN, VM.matrix, GetClientDimension()))
				continue;

			if (!Geo::Get2DVector(player.viewCamPos, FEET_SCREEN, VM.matrix, GetClientDimension()))
				continue;

			RenderPlayerBox(player, EYE_SCREEN, FEET_SCREEN);
			RenderPlayerBoxStats(player, EYE_SCREEN, FEET_SCREEN);
		}

		const Dimension half = GetClientDimension() / 2;
		Gui::DrawCircle({half.w, half.h}, 16, {255, 0, 0, 255}, 128);
	}
};
