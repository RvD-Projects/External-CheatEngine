#pragma once

#include "Engine.h"
#include "Player.h"

using namespace Engine;

namespace GameRef
{
	ViewMatrix VM;
	uintptr_t ENTITIES_LIST;

	Player MyLocalPlayer;
    std::vector<Player> ENTITIES;
    std::vector<Player> ENEMIES;
    std::vector<Player> FRIENDLIES;

	uintptr_t GetLocalPlayer_T(const bool& force = false)
	{
		return ReadClient<uintptr_t>(dwLocalPlayerPawn);
	};

	template <typename T>
	T ReadEntities(const ptrdiff_t& ptr_diff)
	{
		return Read<T>(ENTITIES_LIST + ptr_diff);
	};

	template <typename T>
	T ReadLocalPlayer(const ptrdiff_t& ptr_diff)
	{
		return Read<T>(GetLocalPlayer_T() + ptr_diff);
	};

	bool GameIsDefuse() {
		return ReadClient<bool>(m_bMapHasBombTarget);
	}

	bool GameIsRescue() {
		return ReadClient<bool>(m_bMapHasBombTarget);
	}

	bool GameIsTeamPlay() {
		return GameIsRescue() || GameIsDefuse();
	}
}