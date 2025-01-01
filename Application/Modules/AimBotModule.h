#pragma once

#include "Module.h"

struct ModulesConfig
{
	bool aimAssist, clickAssist;
};

class AimBotModule : public Module
{
	AimConfig config;

	void Execute() override
	{
		this->config.isActive = true;

		this->UpdatePointers(this->rootModule);

		this->config.isReady = true;

		Player target;
		for (Player &player : ENEMIES)
		{
			if (!player.IsValidTarget())
				continue;

			target = player;
			break;
		}

		Position out;
		if (!Get2DVector(target.position, out, VM.matrix, ClientDimension))
			return;

		WriteClient<Vector3>(dwViewAngles, (target.position - MyLocalPlayer.position).RelativeAngle());

		// Empty the old states first (insure next check are actual)
		GetAsyncKeyState(VK_SHIFT);

		if (config.isAimActive && GetAsyncKeyState(VK_SHIFT))
		{
			WriteClient<Vector3>(dwViewAngles, target.distance.RelativeAngle());
		}

		if (config.isClickActive && GetAsyncKeyState(VK_SHIFT))
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
	}

	void RenderAimZone()
	{
		DrawFilledCircle(ClientCenterPosition, 32, White12);
		DrawCircle(ClientCenterPosition, 32, White25);
	}

public:
	void Render() override {};
};
