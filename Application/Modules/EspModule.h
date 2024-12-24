#pragma once

#include "Module.h"

class EspModule : public Module
{
public:
	void Init() override
	{
	}

	void Render() override
	{
		ImGui::GetBackgroundDrawList()->AddRect(
			ImVec2(1920 - 600, 1080 - 600),
			ImVec2(600, 600),
			IM_COL32(0, 255, 0, 255),
			0,
			0,
			2);
	}
};
