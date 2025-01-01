#pragma once

#include "Module.h"

class AimBotModule : public Module
{
	bool aimAssist = true;
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

		// Empty the old states first (insure next check are actual)
		GetAsyncKeyState(VK_SHIFT);

		if (aimAssist && GetAsyncKeyState(VK_SHIFT))
		{
			WriteClient<Vector3>(dwViewAngles, target.distance.RelativeAngle());
		}

		if (clickAssist && GetAsyncKeyState(VK_SHIFT))
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
	}

public:
	void Render() override {};
};
