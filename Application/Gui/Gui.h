#pragma once

#include "../AppEngine.h"
#include "../../ImGui/imgui.h"

namespace Gui
{
	ImU32 Color(const GuiColor color)
	{
		return IM_COL32(color.R, color.G, color.B, color.A);
	}

	void DrawRectangle(const Position &p, const Dimension &d, const GuiColor color = {255, 255, 255, 255}, const float thickness = 1.5f)
	{
		ImGui::GetBackgroundDrawList()->AddRect(
			ImVec2(p.x, p.y),
			ImVec2(p.x + d.w, p.y + d.h),
			Color(color),
			0,
			0,
			thickness);
	}

	void DrawFilledRectangle(const Position &p, const Dimension &d, const GuiColor color = {255, 255, 255, 255})
	{
		ImGui::GetBackgroundDrawList()->AddRectFilled(
			ImVec2(p.x, p.y),
			ImVec2(p.x + d.w, p.y + d.h),
			Color(color));
	}

	void DrawLine(const Position &p1, const Position &p2, const GuiColor color = {0, 0, 0, 255}, const float thickness = 1.5f)
	{
		ImGui::GetBackgroundDrawList()->AddLine(
			ImVec2(p1.x, p1.y),
			ImVec2(p2.x, p2.y),
			Color(color),
			thickness);
	}

	void DrawCircle(const Position &p, float radius, GuiColor color = {255, 255, 255, 255}, const float thickness = 1.5f, const int segments = 0)
	{
		ImGui::GetBackgroundDrawList()->AddCircle(
			ImVec2(p.x, p.y),
			radius,
			Color(color),
			segments > 0 ? segments : radius * M_PI,
			thickness);
	}

	void DrawFilledCircle(const Position &p, const float radius, GuiColor color = {255, 255, 255, 255}, const int segments = 0)
	{
		ImGui::GetBackgroundDrawList()->AddCircleFilled(
			ImVec2(p.x, p.y),
			radius,
			Color(color),
			segments > 0 ? segments : radius * M_PI);
	}

	void DrawTextual(const Position &p, const char *text, const GuiColor color = {255, 255, 255, 255}, const float fontSize = 2.f)
	{
		ImGui::GetBackgroundDrawList()->AddText(
			ImVec2(p.x, p.y),
			Color(color),
			text);
	}
};