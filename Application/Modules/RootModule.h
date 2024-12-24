#pragma once

#include <vector>
#include <cstdint>

#include "Module.h"
#include "EspModule.h"

#include "../../ImGui/imgui.h"
#include "../../Engine/Math/ViewMatrix.h"

class RootModule : public Module
{
	EspModule esp;
	ViewMatrix vm = ViewMatrix();

	void _Render() {
		ImGui::GetBackgroundDrawList()->AddRect(
			ImVec2(0, 0),
			ImVec2(1920, 1080),
			IM_COL32(255, 0, 0, 255),
			0,
			0,
			2);
	}

public:
	void Init() override
	{
		EspModule esp{};
		esp.Init();
	}

	void Render() override
	{
		_Render();
		esp.Render();
	}
};
