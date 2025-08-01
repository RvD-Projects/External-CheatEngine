#pragma once

#include "Module.h"

/**
 * @brief ESP (Extra Sensory Perception) module for visual player information
 *
 * Features:
 * - Player skeleton rendering
 * - Bounding boxes with health-based coloring
 * - Player information display (name, health, distance)
 * - Snap lines to players
 * - Bomb timer information
 * - Configurable visibility toggles
 */
class EspModule : public Module {
private:
  EspConfig config;

  // Display configuration constants
  static constexpr float HEAD_RADIUS_DIVISOR = 14.0f;
  static constexpr float NAME_CHAR_WIDTH = 8.0f;
  static constexpr float HEALTH_CHAR_WIDTH = 8.0f;
  static constexpr int TEXT_BACKGROUND_PADDING = 6;
  static constexpr int TEXT_LINE_HEIGHT = 16;
  static constexpr int BOMB_INFO_BOX_WIDTH = 180;
  static constexpr int BOMB_INFO_BOX_HEIGHT = 64;

  /**
   * @brief Gets appropriate color for player based on health and state
   * @param player The player to get color for
   * @param isTransparent Whether to return a transparent version
   * @return GuiColor based on player health
   */
  GuiColor GetPlayerColor(const Player &player,
                          bool isTransparent = true) const {
    GuiColor baseColor;

    if (player.health > 75) {
      baseColor = isTransparent ? White75 : White;
    } else if (player.health > 50) {
      baseColor = isTransparent ? Yellow75 : Yellow;
    } else if (player.health > 25) {
      baseColor = isTransparent ? Orange75 : Orange;
    } else {
      baseColor = isTransparent ? Red75 : Red;
    }

    return baseColor;
  }

  /**
   * @brief Calculates optimal text positioning for player info
   * @param player The player to calculate position for
   * @return Calculated position for text placement
   */
  Position CalculateTextPosition(const Player &player) const {
    const Position ESP_P = player.screenBox.pStart;
    const Dimension ESP_D = player.screenBox.d;

    return {ESP_P.x + ESP_D.w / 2 - (player.name.length() * 3.0f),
            ESP_P.y + ESP_D.h + 18};
  }
  void UpdateConfigs() {
    // Toggle skeleton rendering (F1)
    if (GetAsyncKeyState(VK_F1) & 1)
      config.isSkeletonActive = !config.isSkeletonActive;

    // Toggle bounding boxes (F2)
    if (GetAsyncKeyState(VK_F2) & 1)
      config.isBoxActive = !config.isBoxActive;

    // Toggle player information (F3)
    if (GetAsyncKeyState(VK_F3) & 1)
      config.isStatsActive = !config.isStatsActive;

    // Toggle snap lines (F4)
    if (GetAsyncKeyState(VK_F4) & 1)
      config.isSnapLineActive = !config.isSnapLineActive;

    // Toggle entire ESP system (F5)
    if (GetAsyncKeyState(VK_F5) & 1)
      config.isActive = !config.isActive;

    // Toggle game objects display (F6)
    if (GetAsyncKeyState(VK_F6) & 1)
      config.isGameObjectsActive = !config.isGameObjectsActive;
  }

  /**
   * @brief Main execution method for ESP module
   */
  void Execute() override {
    UpdateConfigs();

    if (!config.isActive)
      return;
  }

  /**
   * @brief Renders player skeleton and head indicator
   * @param player The player to render skeleton for
   */
  void RenderPlayerSkeleton(const Player &player) {
    if (!player.screenBones.size() || !config.isSkeletonActive)
      return;

    // Draw skeleton bones
    for (const auto &line : player.screenBones) {
      DrawLine(line.pStart, line.pEnd, GetPlayerColor(player), 1.0f);
    }

    // Draw head circle
    const float headRadius = player.screenBox.d.h / HEAD_RADIUS_DIVISOR;
    const Position headPos = player.screenBones[0].pStart;

    DrawFilledCircle(headPos, headRadius, White25);
    DrawCircle(headPos, headRadius, GetPlayerColor(player), 1.0f);
  }

  /**
   * @brief Renders player information stats
   * @param player The player to render stats for
   */
  void RenderPlayerBoxStats(const Player &player) {
    if (!config.isStatsActive)
      return;

    RenderPlayerInfo(player);
  }

  /**
   * @brief Renders comprehensive player information display
   * @param player The player to render information for
   */
  void RenderPlayerInfo(const Player &player) {
    if (player.name.empty())
      return;

    const Position namePos = CalculateTextPosition(player);

    // Prepare text strings
    const std::string nameText = player.name;
    const std::string healthText = std::to_string(player.health) + "hp";

    // Calculate positions for different text elements
    const Position healthPos = {
        namePos.x + nameText.length() * NAME_CHAR_WIDTH + 4, namePos.y};

    // Calculate background dimensions
    const float nameWidth = nameText.length() * NAME_CHAR_WIDTH;
    const float healthWidth = healthText.length() * NAME_CHAR_WIDTH;
    const float totalWidth = nameWidth + healthWidth + 4; // 4 for spacing

    const Dimension bgSize = {totalWidth + TEXT_BACKGROUND_PADDING * 2,
                              TEXT_LINE_HEIGHT + TEXT_BACKGROUND_PADDING};
    const Position bgPos = {namePos.x - TEXT_BACKGROUND_PADDING,
                            namePos.y - TEXT_BACKGROUND_PADDING / 2.0f};

    // Draw background for better readability
    DrawFilledRectangle(bgPos, bgSize, Black50);
    DrawRectangle(bgPos, bgSize, White75);

    // Draw player information
    DrawTextual(namePos, nameText.data(), White75);
    DrawTextual(healthPos, healthText.data(), GetPlayerColor(player));
  }

  /**
   * @brief Renders player bounding box with health-based coloring
   * @param player The player to render box for
   */
  void RenderPlayerBox(const Player &player) {
    if (!config.isBoxActive)
      return;

    const GuiColor boxColor = GetPlayerColor(player);
    DrawRectangle(player.screenBox.pStart, player.screenBox.d, boxColor);
  }

  /**
   * @brief Renders snap line from screen bottom to player
   * @param player The player to draw snap line to
   */
  void RenderPlayerSnapLine(const Player &player) {
    if (!config.isSnapLineActive)
      return;

    const GuiColor lineColor = White75;
    const Line snapLine{{ClientDimension.w / 2, ClientDimension.h},
                        player.screenFeet};
    DrawLine(snapLine.pStart, snapLine.pEnd, lineColor, 1.0f);
  }

  /**
   * @brief Renders game object information (bomb timers, etc.)
   */
  void RenderGameObjects() {
    if (!config.isGameObjectsActive || !C4Bomb.FuseChrono.IsRunning())
      return;

    const Position boxPos{8, ClientDimension.h - BOMB_INFO_BOX_HEIGHT - 8};
    const Position textStart{boxPos.x + 8, boxPos.y + 8};

    // Draw container box with enhanced styling
    DrawFilledRectangle(boxPos, {BOMB_INFO_BOX_WIDTH, BOMB_INFO_BOX_HEIGHT},
                        Black75);
    DrawRectangle(boxPos, {BOMB_INFO_BOX_WIDTH, BOMB_INFO_BOX_HEIGHT}, Red75);

    // Draw bomb timer information with color coding
    DrawTextual(textStart, ("Bomb: " + C4Bomb.FuseChrono.ToString()).data(),
                GetTimerColor(C4Bomb.FuseChrono));

    DrawTextual({textStart.x, textStart.y + TEXT_LINE_HEIGHT},
                ("Defuse Kit: " + C4Bomb.DefuseKitChrono.ToString()).data(),
                GetTimerColor(C4Bomb.DefuseKitChrono));

    DrawTextual({textStart.x, textStart.y + TEXT_LINE_HEIGHT * 2},
                ("Defuse: " + C4Bomb.DefuseChrono.ToString()).data(),
                GetTimerColor(C4Bomb.DefuseChrono));
  }

public:
  /**
   * @brief Initialize the ESP module
   * @param rootModule Pointer to the root module for shared data access
   */
  void Init(Module *rootModule) override {
    config.isActive = true;
    this->rootModule = rootModule;
    config.isReady = true;
  }

  /**
   * @brief Main render method for all ESP elements
   * Renders all enabled ESP features for valid targets
   */
  void Render() override {
    if (config.isHidden || !config.isActive || !config.isReady)
      return;

    // Render ESP for all valid enemy players
    for (const Player &player : ENEMIES) {
      if (!player.IsValidTarget() || !player.isScreenVisible)
        continue;

      RenderPlayerSkeleton(player);
      RenderPlayerBoxStats(player);
      RenderPlayerBox(player);
      RenderPlayerSnapLine(player);
    }

    // Render game objects (bomb info, etc.)
    RenderGameObjects();
  }
};
