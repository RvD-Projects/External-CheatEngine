#pragma once

#include "Module.h"

class EspModule : public Module {
  EspConfig config;

  void UpdateConfigs() {
    // uses F1 to toggle skeleton
    if (GetAsyncKeyState(VK_F1) & 1)
      config.isSkeletonActive = !config.isSkeletonActive;

    // uses F2 to toggle box
    if (GetAsyncKeyState(VK_F2) & 1)
      config.isBoxActive = !config.isBoxActive;

    // uses F3 to toggle stats
    if (GetAsyncKeyState(VK_F3) & 1)
      config.isStatsActive = !config.isStatsActive;

    // uses F4 to toggle snap line
    if (GetAsyncKeyState(VK_F4) & 1)
      config.isSnapLineActive = !config.isSnapLineActive;

    // uses F5 to toggle esp
    if (GetAsyncKeyState(VK_F5) & 1)
      config.isActive = !config.isActive;
  }

  void Execute() {
    UpdateConfigs();

    if (!config.isActive)
      return;
  }

  void RenderPlayerSkeleton(Player &player) {
    if (!player.screenBones.size())
      return;

    if (config.isSkeletonActive) {
      for (const auto &line : player.screenBones)
        DrawLine(line.pStart, line.pEnd, White50, 2.5f);
    }

    const float headRadius = player.screenBox.d.h / 14;
    const Position headPos = player.screenBones[0].pStart;

    DrawFilledCircle(headPos, headRadius, White25);
    DrawCircle(headPos, headRadius, White50, 2);
  }

  void RenderPlayerBoxStats(Player &player) {
    if (!config.isStatsActive)
      return;

    RenderPlayerInfo(player);
  }

  void RenderPlayerInfo(Player &player) {
    if (player.name.empty())
      return;

    Position ESP_P = player.screenBox.pStart;
    Dimension ESP_D = player.screenBox.d;

    // Determine name color based on health
    GuiColor healthColor;
    if (player.health > 75) {
      healthColor = Green75;
    } else if (player.health > 50) {
      healthColor = Yellow75;
    } else if (player.health > 25) {
      healthColor = Orange75;
    } else {
      healthColor = Red75;
    }

    const std::string nameText = player.name;
    const std::string healthText = std::to_string(player.health) + " HP";
    const std::string fullText = nameText + healthText;

    // Calculate name position (centered below the ESP box)
    // This ensures the name appears centered below the ESP box
    const Position namePos = {ESP_P.x + ESP_D.w / 2 -
                                  (float)nameText.length() * 3,
                              ESP_P.y + ESP_D.h + 18};

    // Calculate health position based on name length
    // This ensures the health text appears right after the name
    const Position healthPos = {namePos.x + (float)nameText.length() * 8 + 4,
                                namePos.y};

    // Draw background rectangle for better readability
    const Dimension bgSize = {(float)fullText.length() * 9, 20};
    const Position bgPos = {namePos.x - 6, namePos.y - 3};
    Gui::DrawFilledRectangle(bgPos, bgSize, Black50);
    Gui::DrawRectangle(bgPos, bgSize, White25);

    // Draw the player name with health
    Gui::DrawTextual(namePos, nameText.data(), White75);
    Gui::DrawTextual(healthPos, healthText.data(), healthColor);
  }

  void RenderPlayerBox(Player &player) {
    if (!config.isBoxActive)
      return;

    DrawRectangle(player.screenBox.pStart, player.screenBox.d, White50);
  }

  void RenderPlayerSnapLine(Player &player) {
    if (!config.isSnapLineActive)
      return;

    const Line snapLine{{ClientDimension.w / 2, ClientDimension.h},
                        player.screenFeet};
    DrawLine(snapLine.pStart, snapLine.pEnd, White50, 1.0f);
  }

  void RenderGameObjects() {
    if (!C4Bomb.FuseChrono.IsRunning())
      return;

    const Position boxPos{8, ClientDimension.h - 80};
    const Position textStart{boxPos.x + 8, boxPos.y + 8};

    // Draw the box based on text size
    DrawFilledRectangle(boxPos, {180, 64}, Black75);
    DrawRectangle(boxPos, {180, 64}, White75);

    DrawTextual(textStart,
                ("Time left: " + C4Bomb.FuseChrono.ToString()).data(),
                GetTimerColor(C4Bomb.FuseChrono));
    DrawTextual({textStart.x, textStart.y + 16},
                ("Defuse Kit: " + C4Bomb.DefuseKitChrono.ToString()).data(),
                GetTimerColor(C4Bomb.DefuseKitChrono));
    DrawTextual({textStart.x, textStart.y + 32},
                ("Defuse: " + C4Bomb.DefuseChrono.ToString()).data(),
                GetTimerColor(C4Bomb.DefuseChrono));
  }

public:
  void Init(Module *rootModule) override {
    this->config.isActive = true;
    this->UpdatePointers(rootModule);
    this->config.isReady = true;
  }

  void Render() override {
    if (config.isHidden || !config.isActive || !config.isReady)
      return;

    for (Player player : ENEMIES) {
      if (!player.IsValidTarget())
        continue;

      RenderPlayerSkeleton(player);
      RenderPlayerBoxStats(player);
      RenderPlayerBox(player);
      RenderPlayerSnapLine(player);
    }

    RenderGameObjects();
  }
};
