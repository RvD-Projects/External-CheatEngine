#pragma once

#include "Module.h"

class AimBotModule : public Module
{
	bool aimAssist = false;
	bool clickAssist = true;

	void Execute() override
	{
		this->UpdatePointers(this->rootModule);

		Player target;
		if (!GetCrosshairTarget(target) || !target.IsValidTarget())
			return;

		Position out;
		if (!Geo::Get2DVector(target.position, out, VM.matrix, GetClientDimension()))
			return;

		if (aimAssist && GetAsyncKeyState(VK_LSHIFT))
		{
			mouse_event(MOUSEEVENTF_MOVE, out.x, out.y, 0, 0);
		}

		if (clickAssist && GetAsyncKeyState(VK_LSHIFT))
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
	}

public:
	void Render() override {};
};
