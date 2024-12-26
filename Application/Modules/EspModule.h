#pragma once

#include "Module.h"
#include "../Gui/Gui.h"

class EspModule : public Module
{
public:
	void Render() override
	{
		for (uintptr_t entity : ENTITIES)
		{
			vec3 ABS_ORIG = Read<vec3>(entity + m_vOldOrigin);
			vec3 PAWN_VC = Read<vec3>(entity + m_vecViewOffset);

			vec3 VC_POS = ABS_ORIG + PAWN_VC;

			vec2 HEAD_SCREEN, FEET_SCREEN;
			const bool HEAD_V = Geo::Get2DVector(ABS_ORIG, HEAD_SCREEN, VM.matrix);
			if (!HEAD_V)
				continue;

			const bool FEET_V = Geo::Get2DVector(VC_POS, FEET_SCREEN, VM.matrix);
			if (!FEET_V)
				continue;

			const float HEIGHT = HEAD_SCREEN.y - FEET_SCREEN.y;
			const float WIDTH = HEIGHT * 0.888;
			HEAD_SCREEN.x -= WIDTH / 2;
			HEAD_SCREEN.y -= WIDTH * 1.444;

			const Dimension PAWN_D{ WIDTH, HEIGHT * 1.444, NULL };

			Gui::DrawRectangle(HEAD_SCREEN, PAWN_D);
			Gui::DrawTextual({ HEAD_SCREEN.x, HEAD_SCREEN.y }, "THIS IS BOB");
		}

		Gui::DrawCircle({ 1920 / 2, 1080 / 2 }, 8, { 255, 0, 0, 255 });
	}
};
