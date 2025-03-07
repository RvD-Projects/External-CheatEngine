#pragma once

#include "../Engine/Engine.h"
#include "../Engine/GameDumper/Dumps/offsets.hpp"
#include "../Engine/GameDumper/Dumps/client_dll.hpp"
#include "../Engine/GameDumper/Dumps/server_dll.hpp"

using namespace Engine;

namespace CLIENT_OFFSETS = cs2_dumper::offsets::client_dll;
namespace CLIENT_SCHEMAS = cs2_dumper::schemas::client_dll;
using namespace CLIENT_OFFSETS;
using namespace CLIENT_SCHEMAS;

namespace SERVER_SCHEMAS = cs2_dumper::schemas::server_dll;
using namespace SERVER_SCHEMAS;

using namespace CLIENT_SCHEMAS::C_PlantedC4;
using namespace CLIENT_SCHEMAS::C_Multimeter;
using namespace CLIENT_SCHEMAS::C_CSGameRules;
using namespace CLIENT_SCHEMAS::CSkeletonInstance;
using namespace CLIENT_SCHEMAS::CCSPlayerController;

using namespace CLIENT_SCHEMAS::C_CSPlayerPawn;
using namespace CLIENT_SCHEMAS::C_BasePlayerPawn;
using namespace CLIENT_SCHEMAS::C_CSPlayerPawnBase;

using namespace CLIENT_SCHEMAS::C_BaseEntity;
using namespace CLIENT_SCHEMAS::C_BaseModelEntity;

namespace AppEngine
{
    uintptr_t GetDLL_Ptr(const std::string &name)
    {
        return Target.dlls[name];
    }

    template <typename T>
    T ReadDLL(const std::string &name, const ptrdiff_t &ptr_diff)
    {
        return Read<T>(GetDLL_Ptr(name) + ptr_diff);
    };

    template <typename T>
    bool WriteDLL(const std::string &name, const ptrdiff_t &ptr_diff, const T &value)
    {
        return Write<T>(GetDLL_Ptr(name) + ptr_diff, value);
    };

    template <typename T>
    T ReadClient(const ptrdiff_t &ptr_diff)
    {
        return ReadDLL<T>("client.dll", ptr_diff);
    };

    template <typename T>
    bool WriteClient(const ptrdiff_t &ptr_diff, const T &value)
    {
        return WriteDLL<T>("client.dll", ptr_diff, value);
    };

    template <typename T>
    T ReadServer(const ptrdiff_t &ptr_diff)
    {
        return ReadDLL<T>("server.dll", ptr_diff);
    };

    template <typename T>
    bool WriteServer(const ptrdiff_t &ptr_diff, const T &value)
    {
        return WriteDLL<T>("server.dll", ptr_diff, value);
    };

    uintptr_t GetLocalPlayer_T()
    {
        return ReadClient<uintptr_t>(dwLocalPlayerPawn);
    };

    template <typename T>
    T ReadLocalPlayer(const ptrdiff_t &ptr_diff)
    {
        return Read<T>(GetLocalPlayer_T() + ptr_diff);
    };

    namespace GlobalVars
    {
        template <typename T>
        T ReadGlobalVars(const ptrdiff_t &ptr_diff)
        {
            uintptr_t ptr = ReadClient<uintptr_t>(dwGlobalVars);

            return Read<T>(ptr + ptr_diff);
        }
    }

    namespace GameRules
    {
        template <typename T>
        T ReadGameRules(const ptrdiff_t &ptr_diff)
        {
            return Read<T>(ReadClient<uintptr_t>(dwGameRules) + ptr_diff);
        }

        template <typename T>
        T ReadGameModeRules(const ptrdiff_t &ptr_diff)
        {
            return Read<T>(ReadGameRules<uintptr_t>(m_pGameModeRules) + ptr_diff);
        }

        bool MapHasBombSite()
        {
            return ReadGameRules<bool>(m_bMapHasBombTarget);
        };

        bool MapHasRescueSite()
        {
            return ReadGameRules<bool>(m_bMapHasRescueZone);
        };

        bool BombIsPlanted()
        {
            if (!MapHasBombSite())
                return false;

            return ReadGameRules<bool>(m_bBombPlanted);
        };
    }
};