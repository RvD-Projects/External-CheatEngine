#pragma once

#include "Module.h"

class AimBotModule : public Module
{
	bool aimAssist = false;
	bool clickAssist = true;

	void Execute() override
	{
		this->UpdatePointers(this->rootModule);

		if (!MyLocalPlayer.isInitialized || MyLocalPlayer.crossIndex < 0)
			return;

		const uintptr_t listEntry_t = ReadEntities<uintptr_t>(0x8 * (MyLocalPlayer.crossIndex >> 9) + 16);
		const uintptr_t entity_t = Read<uintptr_t>(listEntry_t + 120 * (MyLocalPlayer.crossIndex & 0x1ff));

		std::vector<Player> filteredPlayers;
		std::copy_if(ENTITIES.begin(), ENTITIES.end(), std::back_inserter(filteredPlayers), [&entity_t](const Player& player)
			{ return player.health > 0 && !player.isLocalPlayer && !player.isLocalPlayerTeam && player.entity == entity_t; });

		if (filteredPlayers.empty())
			return;

		vec2 out;
		Player player = filteredPlayers.at(0);
		if (Geo::Get2DVector(player.position, out, VM.matrix, GetClientDimension()))
		{
			if (aimAssist && GetAsyncKeyState(VK_SHIFT))
			{
				mouse_event(MOUSEEVENTF_MOVE, out.x, out.y, 0, 0);
			}

			if (clickAssist && GetAsyncKeyState(VK_SHIFT))
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}
		}
	}

public:
	void Render() override {};
};
