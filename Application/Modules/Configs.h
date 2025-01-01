#pragma once

#include "../AppEngine.h"
#include "../Gui/Gui.h"

using namespace Gui;
using namespace AppEngine;
using namespace AppEngine::GameRules;

struct ModuleConfig
{
    bool isActive, isReady, isHidden;
    long long refreshRate = 10;
};

struct RootConfig : public ModuleConfig
{
    bool espActive, AimActive;
};

struct EspConfig : public ModuleConfig
{
    bool pBx, pName, pHealth, pArmor, pBxHealth, pBxArmor;
    GuiColor pBxCol, pBxBorderCol, pHealthCol, pArmorCol, pBxHealthCol, pBxArmorCol;
    Position pBxPosOffset, pBxDimOffset;
    std::map<std::string, std::string> gameObjects;
};

struct AimConfig : public ModuleConfig
{
    bool aimAssist, clickAssist;
};