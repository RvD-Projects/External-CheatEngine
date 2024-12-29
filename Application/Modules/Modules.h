#pragma once

#include "../../Engine/Engine.h"
#include "../../Engine/GameDumper/Dumps/offsets.hpp"
#include "../../Engine/GameDumper/Dumps/client_dll.hpp"

using namespace Engine;

using namespace cs2_dumper::offsets::client_dll;
using namespace cs2_dumper::schemas::client_dll;
using namespace cs2_dumper::schemas::client_dll::C_CSGameRules;
using namespace cs2_dumper::schemas::client_dll::CCSPlayerController;

using namespace cs2_dumper::schemas::client_dll::C_CSPlayerPawn;
using namespace cs2_dumper::schemas::client_dll::C_BasePlayerPawn;
using namespace cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase;

using namespace cs2_dumper::schemas::client_dll::C_BaseEntity;
using namespace cs2_dumper::schemas::client_dll::C_BaseModelEntity;

namespace Modules
{
    ptrdiff_t EntityEntryDiff(const ptrdiff_t &ptr, const ptrdiff_t &mul = 0x7ff)
    {
        return (0x8 * (ptr & mul) >> 0x9) + 16;
    }

    ptrdiff_t EntityDiff(const ptrdiff_t &ptr)
    {
        return 120 * (ptr & 0x1ff);
    }

    uintptr_t GetLocalPlayer_T()
    {
        return ReadDLL<uintptr_t>(dwLocalPlayerPawn);
    };

    template <typename T>
    T ReadLocalPlayer(const ptrdiff_t &ptr_diff)
    {
        return Read<T>(GetLocalPlayer_T() + ptr_diff);
    };

    namespace GameRules
    {
        static bool GameIsDefuse()
        {
            return ReadDLL<bool>(m_bMapHasBombTarget);
        };

        static bool GameIsRescue()
        {
            return ReadDLL<bool>(m_bMapHasBombTarget);
        };

        static bool GameIsTeamPlay()
        {
            return GameIsRescue() || GameIsDefuse();
        };
    }
};