#pragma once

#include "Module.h"

/**
 * @brief AimBot module for automated target acquisition and assistance
 *
 * Features:
 * - Aim assist with configurable smoothness
 * - Auto-click when target is in crosshair
 * - Visual aim circle (FOV indicator)
 * - Dynamic target selection based on distance
 */
class AimBotModule : public Module {
private:
  AimConfig config;
  uintptr_t currentTargetPtr;

  // Configuration constants
  static constexpr float SMOOTHNESS_INCREMENT = 0.05f;
  static constexpr float MIN_SMOOTHNESS = 0.1f;
  static constexpr float MAX_SMOOTHNESS = 5.0f;

  /**
   * @brief Updates configuration based on user input
   * @return true if module should continue execution, false otherwise
   */
  bool UpdateConfigs() {
    config.aimCircle.p = ClientCenterPosition;

    // Toggle aim circle visibility (HOME key)
    if (GetAsyncKeyState(VK_HOME) & 1)
      config.showAimCircle = !config.showAimCircle;

    // Adjust smoothness (UP/DOWN keys)
    if (GetAsyncKeyState(VK_UP) & 1) {
      config.smoothness += SMOOTHNESS_INCREMENT;
      if (config.smoothness > MAX_SMOOTHNESS)
        config.smoothness = MAX_SMOOTHNESS;
    }

    if (GetAsyncKeyState(VK_DOWN) & 1) {
      config.smoothness -= SMOOTHNESS_INCREMENT;
      if (config.smoothness < MIN_SMOOTHNESS)
        config.smoothness = MIN_SMOOTHNESS;
    }

    // Toggle aim assist (Mouse 4)
    if (GetAsyncKeyState(VK_XBUTTON1) & 1)
      config.isAimActive = !config.isAimActive;

    // Auto-click when holding Mouse 5
    config.isClickActive = GetAsyncKeyState(VK_XBUTTON2) != 0;

    return config.isActive && config.isReady;
  }

  /**
   * @brief Finds and selects the best target within aim circle
   * Prioritizes current target if still valid, otherwise selects closest enemy
   */
  void UpdateTarget() {
    if (!config.isAimActive) {
      ResetTarget();
      return;
    }

    Player bestTarget;
    uintptr_t bestTargetPtr = NULL;
    float minDistance = FLT_MAX;
    bool foundCurrentTarget = false;

    // Iterate through all enemies to find the best target
    for (const Player &player : ENEMIES) {
      if (!IsValidTarget(player))
        continue;

      if (!IsWithinAimCircle(player))
        continue;

      // Prioritize current target if it's still valid
      if (currentTargetPtr && currentTargetPtr == player.entity) {
        bestTargetPtr = player.entity;
        foundCurrentTarget = true;
        ApplyAimAssist(player);
        break;
      }

      // Find closest target
      float distance = CalculateDistance(player);
      if (distance < minDistance) {
        minDistance = distance;
        bestTargetPtr = player.entity;
        bestTarget = player;
      }
    }

    // Apply aim to selected target if no current target was found
    if (bestTargetPtr && !foundCurrentTarget) {
      currentTargetPtr = bestTargetPtr;
      ApplyAimAssist(bestTarget);
    }
  }

  /**
   * @brief Resets current target
   */
  void ResetTarget() { currentTargetPtr = NULL; }

  /**
   * @brief Checks if a player is a valid target
   * @param player The player to check
   * @return true if player is a valid target
   */
  bool IsValidTarget(const Player &player) const {
    return player.IsValidTarget();
  }

  /**
   * @brief Checks if player is within the aim circle
   * @param player The player to check
   * @return true if player intersects with aim circle
   */
  bool IsWithinAimCircle(const Player &player) const {
    return InterSects(player.screenBox, config.aimCircle);
  }

  /**
   * @brief Calculates distance from local player to target
   * @param player The target player
   * @return Euclidean distance
   */
  float CalculateDistance(const Player &player) const {
    return player.position.EuclideanDistance(MyLocalPlayer.position);
  }

  /**
   * @brief Applies aim assistance to target
   * @param target The target to aim at
   */
  void ApplyAimAssist(const Player &target) {
    MyLocalPlayer.SetViewAngles(target.aimAngle, config.smoothness);
  }

  /**
   * @brief Performs auto-click if target is in crosshair
   */
  void HandleAutoClick() {
    if (MyLocalPlayer.crossIndex >= 0 && config.isClickActive) {
      mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
      mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }
  }

  /**
   * @brief Renders the aim circle (FOV indicator)
   */
  void RenderAimZone() {
    if (!config.showAimCircle)
      return;

    DrawCircle(config.aimCircle.p, config.aimCircle.radius,
               config.aimCircle.color);
    DrawFilledCircle(config.aimCircle.p, config.aimCircle.radius,
                     config.aimCircle.borderColor);
  }

public:
  /**
   * @brief Main execution method called by the module framework
   */
  void Execute() override {
    if (!UpdateConfigs())
      return;

    UpdateTarget();
    HandleAutoClick();
  }

  /**
   * @brief Initialize the aimbot module
   * @param rootModule Pointer to the root module for shared data access
   */
  void Init(Module *rootModule) override {
    config.isActive = true;
    // Initialize module data from root module
    if (rootModule) {
      this->rootModule = rootModule;
    }
    config.isReady = true;
  }

  /**
   * @brief Render method called by the module framework
   */
  void Render() override {
    if (config.isHidden || !config.isActive || !config.isReady)
      return;

    RenderAimZone();
  }
};
