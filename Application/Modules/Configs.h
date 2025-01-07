#pragma once

#include "../AppEngine.h"
#include "../Gui/Gui.h"

using namespace AppEngine;
using namespace AppEngine::GameRules;
using namespace Gui;

struct ModuleConfig
{
    bool isActive = true;
    bool isReady = false, isHidden = false;
    long long refreshRate = 10;
};

struct RootConfig : public ModuleConfig
{
    bool espActive = true, AimActive = true;
};

struct EspConfig : public ModuleConfig
{
    Position pBxPosOffset, pBxDimOffset;
    std::map<std::string, std::string> gameObjects;
    GuiColor pBxCol, pBxBorderCol, pHealthCol, pArmorCol, pBxHealthCol, pBxArmorCol;

    bool isSkeletonActive = true, isBoxActive = true, isStatsActive = true, isGameObjectsActive = true, isSnapLineActive = true;
    bool showHealth = true, showArmor = true, showName = true, showDistance = true;
    bool showHealthBar = true, showArmorBar = true;
    bool showHealthText = true, showArmorText = true;
};

struct AimConfig : public ModuleConfig
{
    bool isAimActive = false, isClickActive = false, showAimCircle = false;
    Circle aimCircle = {.radius = 128.0F, .color = White12, .borderColor = White25};
    float smoothness = 1.25F;
};