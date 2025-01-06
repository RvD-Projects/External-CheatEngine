#pragma once

#include "Module.h"

struct ModulesConfig
{
	bool aimAssist, clickAssist;
};

class AimBotModule : public Module
{
	AimConfig config;
	uintptr_t currentTargetPtr;

	void UpdateConfigs()
	{
		config.isClickActive = 0;
		config.aimCircle.p = ClientCenterPosition;

		// uses page up to increment smoothness
		if (GetAsyncKeyState(SB_PAGEUP) & 1)
			config.smoothness += 0.1F;

		// uses page up to decrement smoothness
		if (GetAsyncKeyState(SB_PAGEDOWN) & 1)
			config.smoothness -= 0.1F;

		// VK_XBUTTON1 (Mouse 4) toggles aim lock
		if (GetAsyncKeyState(VK_XBUTTON1) & 1)
			config.isAimActive = !config.isAimActive;

		// VK_XBUTTON2 (Mouse 5) hold for auto shoot on target
		GetAsyncKeyState(VK_XBUTTON2) & 1;
		if (GetAsyncKeyState(VK_XBUTTON2) & 1)
			config.isClickActive = 1;
	}

	void UpdateTarget()
	{
		if (!config.isAimActive)
		{
			currentTargetPtr = NULL;
			return;
		}

		Player target;
		uintptr_t ptrFound = NULL;
		float minDistance = NULL;

		for (Player &player : ENEMIES)
		{
			if (!player.IsValidTarget())
				continue;

			if (!InterSects(player.screenBox, config.aimCircle))
				continue;

			if (currentTargetPtr && currentTargetPtr == player.entity)
			{
				ptrFound = player.entity;
				target = player;
				break;
			}

			float distance = player.position.EuclideanDistance(MyLocalPlayer.position);
			if (!minDistance || distance < minDistance)
			{
				minDistance = distance;
				ptrFound = player.entity;
				target = player;
			}
		}

		if (ptrFound)
		{
			currentTargetPtr = ptrFound;
			MyLocalPlayer.SetViewAngles(target.aimAngle, config.smoothness);
		}
	}

	void Execute() override
	{
		UpdateConfigs();

		if (!config.isActive)
			return;

		UpdateTarget();

		if (config.isClickActive && MyLocalPlayer.crossIndex >= 0)
		{
			if (!config.isClickActive)
				return;

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

		RenderAimZone();
	};
};
