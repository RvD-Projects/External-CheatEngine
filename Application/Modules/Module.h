#pragma once

#include "../Gui/Gui.h"
#include "../../Engine/GameRef.h"

using namespace GameRef;

class Module
{
public:
    virtual void Init() {};
    virtual void Loop() {};
    virtual void Render() {};
    virtual void Execute() {};
};
