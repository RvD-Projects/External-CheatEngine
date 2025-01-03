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

		// VK_XBUTTON1 (Mouse 4) only needs a 'toggle' (on/off)
		const float doAim = config.isAimActive && GetKeyState(VK_XBUTTON1);

		// VK_XBUTTON2 (Mouse 5) need a 'hold'
		const float doShoot = config.isClickActive && MyLocalPlayer.crossIndex >= 0 && GetAsyncKeyState(VK_XBUTTON2) && GetAsyncKeyState(VK_XBUTTON2);

		if (doAim)
			WriteClient<Vector3>(dwViewAngles, target->aimAngle);

		if (doShoot)
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
