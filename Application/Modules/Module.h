#pragma once

#include "../../Engine/Engine.h"
#include "../../Engine/Player.h"

using namespace Engine;

class Module
{
    virtual void Frame() {};

public:
    Player PLAYER;

    std::vector<Player> ENTITIES;
    std::vector<Player> ENEMIES;
    std::vector<Player> FRIENDLIES;

    virtual void Init() {};
    virtual void Loop() {};
    virtual void Render() {};
};
