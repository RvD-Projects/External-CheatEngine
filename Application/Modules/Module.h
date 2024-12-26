#pragma once

#include "../../Engine/Engine.h"
#include "../../Engine/Math/Geo.h"

using namespace Memory;
using namespace Engine;

using namespace cs2_dumper::offsets::client_dll;
using namespace cs2_dumper::schemas::client_dll;
using namespace cs2_dumper::schemas::client_dll::C_BaseEntity;
using namespace cs2_dumper::schemas::client_dll::C_BasePlayerPawn;
using namespace cs2_dumper::schemas::client_dll::C_BaseModelEntity;
using namespace cs2_dumper::schemas::client_dll::CCSPlayerController;

class Module
{
    virtual void Frame() {};

public:
    virtual void Init() {};
    virtual void Loop() {};
    virtual void Render() {};

protected:
    template <typename T>
    T ReadClient(ptrdiff_t ptr_diff)
    {
        return Read<T>(CLIENT_DLL + ptr_diff);
    };

    template <typename T>
    T ReadPlayerPawn(ptrdiff_t ptr_diff)
    {
        return Read<T>(PLAYER_PAWN + ptr_diff);
    };

    template <typename T>
    T ReadPlayerController(ptrdiff_t ptr_diff)
    {
        return Read<T>(PLAYER_CTRL + ptr_diff);
    };

    template <typename T>
    T ReadEntities(ptrdiff_t ptr_diff)
    {
        return Read<T>(ENTITIES_LIST + ptr_diff);
    };
};
