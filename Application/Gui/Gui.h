#pragma once

#include <imgui.h>

#include "../../Math/Vector.h"

typedef struct
{
	int R = 0;
	int G = 0;
	int B = 0;
	int A = 255;
} GuiColor;

class Gui
{
	void DrawRect(const vec2 &p, const Dimension &d, GuiColor color = {0, 0, 0, 255}, int thickness = 1)
	{
		ImGui::GetBackgroundDrawList()->AddRect(
			ImVec2(p.x, p.y),
			ImVec2(p.x + d.w, p.y + d.h),
			Color(color),
			0,
			0,
			thickness);
	}
};