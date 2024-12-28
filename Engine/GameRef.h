#pragma once

#include "Engine.h"
#include "GameDumper/Dumps/offsets.hpp"
#include "GameDumper/Dumps/client_dll.hpp"

using namespace cs2_dumper::offsets::client_dll;
using namespace cs2_dumper::schemas::client_dll;
using namespace cs2_dumper::schemas::client_dll::C_BaseEntity;
using namespace cs2_dumper::schemas::client_dll::C_CSGameRules;
using namespace cs2_dumper::schemas::client_dll::C_BasePlayerPawn;
using namespace cs2_dumper::schemas::client_dll::C_BaseModelEntity;
using namespace cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase;
using namespace cs2_dumper::schemas::client_dll::CCSPlayerController;

using namespace Engine;

namespace GameRef
{
	ptrdiff_t EntityEntryDiff(const ptrdiff_t& ptr, const ptrdiff_t& mul = 0x7ff)
	{
		return (0x8 * (ptr & mul) >> 0x9) + 16;
	}

	ptrdiff_t EntityDiff(const ptrdiff_t& ptr)
	{
		return 120 * (ptr & 0x1ff);
	}

	uintptr_t GetLocalPlayer_T()
	{
		return ReadDLL<uintptr_t>(dwLocalPlayerPawn);
	};

	template <typename T>
	T ReadLocalPlayer(const ptrdiff_t& ptr_diff)
	{
		return Read<T>(GetLocalPlayer_T() + ptr_diff);
	};

	bool GameIsDefuse() {
		return ReadDLL<bool>(m_bMapHasBombTarget);
	}

	bool GameIsRescue() {
		return ReadDLL<bool>(m_bMapHasBombTarget);
	}

	bool GameIsTeamPlay() {
		return GameIsRescue() || GameIsDefuse();
	}
}