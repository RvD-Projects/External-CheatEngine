#pragma once

#include "Module.h"
#include "../Gui/Gui.h"

class EspModule : public Module
{
public:
	void Init() override
	{
	}

	void Render() override
	{
		Gui::DrawCircle({1920 / 2, 1080 / 2}, 8, { 0, 255, 0, 255 }, 12, 2);	
	}
};
