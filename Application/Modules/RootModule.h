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
		Gui::DrawRectangle({ 0, 0}, { 1920, 1080}, { 255, 0, 0, 255 });
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
