#pragma once

#include "Module.h"

class AimBotModule : public Module
{
	bool aimAssist = false;
	bool clickAssist = true;

public:
	void Execute() override
	{
		if (!MyLocalPlayer.isInitialized || MyLocalPlayer.crossIndex < 0)
			return;

		const uintptr_t listEntry_t = ReadEntities<uintptr_t>(0x8 * (MyLocalPlayer.crossIndex >> 9) + 16);

		std::vector<Player> filteredPlayers;
		std::copy_if(ENTITIES.begin(),ENTITIES.end(), std::back_inserter(filteredPlayers), [&listEntry_t](const Player& player) {
			return player.health > 0
				&& !player.isLocalPlayer
				&& !player.isLocalPlayerTeam
				&& player.listEntry == listEntry_t;
			});

		if (filteredPlayers.empty())
			return;

		vec2 out;
		Player player = filteredPlayers.at(0);
		if (Geo::Get2DVector(player.position, out, VM.matrix, SD))
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
