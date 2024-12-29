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
		const float HEIGHT = (FEET_SCREEN.y - EYE_SCREEN.y) * 1.1777f;
		const float WIDTH = HEIGHT * 0.777f;

		ESP_D.w = WIDTH;
		ESP_D.h = HEIGHT;

		ESP_P.x = FEET_SCREEN.x - (WIDTH * 0.5f);
		ESP_P.y = FEET_SCREEN.y - HEIGHT;
	}

	void RenderPlayerBox(const Player &player, Position &EYE_SCREEN, Position &FEET_SCREEN)
	{
		Position ESP_P;
		Dimension ESP_D;
		GetEspBox(EYE_SCREEN, FEET_SCREEN, ESP_P, ESP_D);

		Gui::DrawRectangle(ESP_P, ESP_D, White50);
		Gui::DrawTextual(ESP_P + Position{ESP_D.w + 4, 0}, player.name.data(), 8);
		Gui::DrawTextual(ESP_P + Position{ESP_D.w + 4, 16}, std::to_string(player.health).append(" HP").data(), 8, Red64);

		if (player.armor)
		{
			Gui::DrawTextual(ESP_P + Position{ESP_D.w + 4, 32}, std::to_string(player.armor).append(" AP").data(), 8, Blue64);
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

	void RenderPlayerSkeleton(const Player &player, Position &EYE_SCREEN, Position &FEET_SCREEN)
	{
		Position ESP_P;
		Dimension ESP_D;
		GetEspBox(EYE_SCREEN, FEET_SCREEN, ESP_P, ESP_D);

		Gui::DrawFilledCircle(EYE_SCREEN, 32, Green25, 6);
		Gui::DrawCircle(EYE_SCREEN, 32, White50, 6);
	}

public:
	void Render() override
	{
		for (Player player : ENEMIES)
		{
			if (!player.IsValidTarget())
				continue;

			Position FEET_SCREEN, EYE_SCREEN;
			if (!Geo::Get2DVector(player.position, FEET_SCREEN, VM.matrix, GetClientDimension()))
				continue;

			if (!Geo::Get2DVector(player.viewCamPos, EYE_SCREEN, VM.matrix, GetClientDimension()))
				continue;

			RenderPlayerSkeleton(player, EYE_SCREEN, FEET_SCREEN);
			RenderPlayerBox(player, EYE_SCREEN, FEET_SCREEN);
			RenderPlayerBoxStats(player, EYE_SCREEN, FEET_SCREEN);
		}

		const Dimension half = GetClientDimension() / 2;
		Gui::DrawFilledCircle({half.w, half.h}, 64, White12, 32);
	}
};
