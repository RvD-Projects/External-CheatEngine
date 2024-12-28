#pragma once

#include "Module.h"

class EspModule : public Module
{
public:
	void Execute() override
	{
		this->UpdatePointers(this->rootModule);
	}

	void Render() override
	{
		for (Player player : ENEMIES)
		{
			if (!player.takesDamage || player.health <= 0)
				continue;

			vec2 HEAD_SCREEN, FEET_SCREEN;
			const bool HEAD_V = Geo::Get2DVector(player.position, HEAD_SCREEN, VM.matrix, GetClientDimension());
			if (!HEAD_V)
				continue;

			const bool FEET_V = Geo::Get2DVector(player.viewCamPos, FEET_SCREEN, VM.matrix, GetClientDimension());
			if (!FEET_V)
				continue;

			const float HEIGHT = HEAD_SCREEN.y - FEET_SCREEN.y;
			const float WIDTH = HEIGHT * 0.888;
			HEAD_SCREEN.x -= WIDTH / 2;
			HEAD_SCREEN.y -= WIDTH * 1.444;

			const Dimension PAWN_D{WIDTH, HEIGHT * 1.444, NULL};

			Gui::DrawRectangle(HEAD_SCREEN, PAWN_D);
			Gui::DrawTextual(HEAD_SCREEN, player.name.c_str());
		}

		auto half = GetClientDimension() / 2;
		Gui::DrawCircle({ half.w, half.h }, 8, { 255, 0, 0, 255 }, 32);
	}
};
