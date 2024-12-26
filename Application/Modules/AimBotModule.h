#pragma once

#include "Module.h"

class AimBotModule : public Module
{
	bool aimAssist = false;
	bool clickAssist = true;

public:
	void Execute() override
	{
		if (!PLAYER.isInitialized || PLAYER.crossIndex < 0)
			return;

		const uintptr_t crossList = ReadEntities<uintptr_t>(0x8 * (PLAYER.crossIndex >> 0x9) + 16);
		const uintptr_t player_t = Read<uintptr_t>(crossList + 120 * (PLAYER.crossIndex & 0x1ff));

		auto team = Read<UINT>(player_t + m_iTeamNum);
		auto health = Read<UINT>(player_t + m_iHealth);
		auto position = Read<vec3>(player_t + m_vOldOrigin);

		if (team == PLAYER.team || health <= 0)
			return;

		vec2 out;
		if (Geo::Get2DVector(position, out, VM.matrix, SD))
		{
			if (aimAssist && GetAsyncKeyState(VK_SHIFT))
			{
				SetCursorPos(out.x, out.y);
			}

			if (clickAssist && GetAsyncKeyState(VK_SHIFT))
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}
		}
	}
};
