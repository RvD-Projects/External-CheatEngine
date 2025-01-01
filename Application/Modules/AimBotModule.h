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
		if (!config.isActive)
			return;

		Player target;
		for (Player &player : ENEMIES)
		{
			if (!player.IsValidTarget())
				continue;

			if (!InterSects(player.screenBox, config.aimCircle))
				continue;

			target = player;
			break;
		}

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
		if (!config.showAimCircle)
			return;

		DrawCircle(config.aimCircle.p, config.aimCircle.radius, config.aimCircle.color);
		DrawFilledCircle(config.aimCircle.p, config.aimCircle.radius, config.aimCircle.borderColor);
	}

public:
	void Render() override
	{
		if (config.isHidden || !config.isActive || !config.isReady)
			return;

		this->RenderAimZone();
	};
};
