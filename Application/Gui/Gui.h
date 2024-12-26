#pragma once

#include "../../ImGui/imgui.h"
#include "../../Engine/Math/Vector.h"

typedef struct
{
	int R;
	int G;
	int B;
	int A;
} GuiColor;

ImU32 Color(const GuiColor color)
{
	return IM_COL32(color.R, color.G, color.B, color.A);
}

class Gui
{
public:
	static void DrawRectangle(const vec2 &p, const Dimension &d, const GuiColor color = {255, 255, 255, 255}, const int thickness = 1)
	{
		ImGui::GetBackgroundDrawList()->AddRect(
			ImVec2(p.x, p.y),
			ImVec2(p.x + d.w, p.y + d.h),
			Color(color),
			0,
			0,
			thickness);
	}

	static void DrawFilledRectangle(const vec2 &p, const Dimension &d, const GuiColor color = {255, 255, 255, 255})
	{
		ImGui::GetBackgroundDrawList()->AddRectFilled(
			ImVec2(p.x, p.y),
			ImVec2(p.x + d.w, p.y + d.h),
			Color(color));
	}

	static void DrawLine(const vec2 &p1, const vec2 &p2, const GuiColor color = {0, 0, 0, 255}, const int thickness = 1)
	{
		ImGui::GetBackgroundDrawList()->AddLine(
			ImVec2(p1.x, p1.y),
			ImVec2(p2.x, p2.y),
			Color(color),
			thickness);
	}

	static void DrawCircle(const vec2 &p, float radius, GuiColor color = {255, 255, 255, 255}, const int segments = 12, const int thickness = 1)
	{
		ImGui::GetBackgroundDrawList()->AddCircle(
			ImVec2(p.x, p.y),
			radius,
			Color(color),
			segments,
			thickness);
	}

	static void DrawFilledCircle(const vec2 &p, const float radius, GuiColor color = {255, 255, 255, 255}, const int segments = 12)
	{
		ImGui::GetBackgroundDrawList()->AddCircleFilled(
			ImVec2(p.x, p.y),
			radius,
			Color(color),
			segments);
	}

	static void DrawTextual(const vec2 &p, const char *text, const float fontSize = 2.f, const GuiColor color = {255, 255, 255, 255})
	{
		ImGui::GetBackgroundDrawList()->AddText(
			ImVec2(p.x, p.y),
			Color(color),
			text);
	}
};