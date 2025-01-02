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

		config.aimCircle.p = ClientCenterPosition;

		Player *target = nullptr;
		for (Player &player : ENEMIES)
		{
			if (!player.IsValidTarget())
				continue;

			if (!InterSects(player.screenBox, config.aimCircle))
				continue;

			target = &player;
			break;
		}

		if (!target)
			return;

		// Empty the old states first (insure next check are actual)
		GetAsyncKeyState(VK_RBUTTON);

		if (config.isAimActive && GetAsyncKeyState(VK_RBUTTON))
		{
			WriteClient<Vector3>(dwViewAngles, target->distance.RelativeAngle());

			if (MyLocalPlayer.crossIndex < 0)
				return;

			if (config.isClickActive)
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}
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
	void Init(Module *rootModule) override
	{
		this->config.isActive = true;
		this->UpdatePointers(rootModule);
		this->config.isReady = true;
	}

	void Render() override
	{
		if (config.isHidden || !config.isActive || !config.isReady)
			return;
	};
};
