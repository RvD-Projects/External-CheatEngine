#pragma once

#include "Module.h"
#include <chrono>

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

  // Enhanced aim assist constants
  static constexpr float TARGET_LOCK_DISTANCE_THRESHOLD =
      50.0f; // Distance to maintain target lock
  static constexpr float HEADSHOT_PREFERENCE_MULTIPLIER =
      0.7f; // Prefer closer headshot targets
  static constexpr float AIM_PREDICTION_FACTOR =
      0.3f; // Velocity prediction strength
  static constexpr float AIM_SMOOTHING_DEADZONE =
      2.0f; // Pixel deadzone for micro-adjustments

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

    // Adjust FOV/Aim Circle size (LEFT/RIGHT keys)
    if (GetAsyncKeyState(VK_LEFT) & 1) {
      config.aimCircle.radius -= 10.0f;
      if (config.aimCircle.radius < 50.0f)
        config.aimCircle.radius = 50.0f;
    }

    if (GetAsyncKeyState(VK_RIGHT) & 1) {
      config.aimCircle.radius += 10.0f;
      if (config.aimCircle.radius > 300.0f)
        config.aimCircle.radius = 300.0f;
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
   * Uses advanced targeting with headshot preference and target persistence
   */
  void UpdateTarget() {
    if (!config.isAimActive) {
      ResetTarget();
      return;
    }

    Player bestTarget;
    uintptr_t bestTargetPtr = NULL;
    float bestTargetScore = FLT_MAX;
    bool foundCurrentTarget = false;

    // Iterate through all enemies to find the best target
    for (const Player &player : ENEMIES) {
      if (!IsValidTarget(player))
        continue;

      if (!IsWithinAimCircle(player))
        continue;

      // Prioritize current target if it's still valid and within lock distance
      if (currentTargetPtr && currentTargetPtr == player.entity) {
        float currentDistance = CalculateDistance(player);
        if (currentDistance <= TARGET_LOCK_DISTANCE_THRESHOLD) {
          bestTargetPtr = player.entity;
          foundCurrentTarget = true;
          ApplyEnhancedAimAssist(player);
          break;
        }
      }

      // Calculate target priority score (lower is better)
      float targetScore = CalculateTargetScore(player);
      if (targetScore < bestTargetScore) {
        bestTargetScore = targetScore;
        bestTargetPtr = player.entity;
        bestTarget = player;
      }
    }

    // Apply aim to selected target if no current target was maintained
    if (bestTargetPtr && !foundCurrentTarget) {
      currentTargetPtr = bestTargetPtr;
      ApplyEnhancedAimAssist(bestTarget);
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
   * @brief Calculates target priority score (lower = higher priority)
   * Considers distance, health, and headshot potential
   * @param player The target player
   * @return Priority score (lower is better)
   */
  float CalculateTargetScore(const Player &player) {
    float distance = CalculateDistance(player);
    float healthFactor = player.health / 100.0f; // Prefer low health targets

    // Calculate headshot distance (distance from crosshair to head)
    float headshotDistance = CalculateHeadshotDistance(player);

    // Combine factors: distance, health, and headshot potential
    float score = distance + (healthFactor * 20.0f) +
                  (headshotDistance * HEADSHOT_PREFERENCE_MULTIPLIER);

    return score;
  }

  /**
   * @brief Calculates distance from crosshair to target's head
   * @param player The target player
   * @return Distance to head hitbox
   */
  float CalculateHeadshotDistance(const Player &player) {
    // Get screen center position
    Position screenCenter = ClientCenterPosition;

    // Use head position from screen bones (first bone is typically head)
    if (!player.screenBones.empty()) {
      Position headPos = player.screenBones[0].pStart;

      float dx = screenCenter.x - headPos.x;
      float dy = screenCenter.y - headPos.y;

      return sqrt(dx * dx + dy * dy);
    }

    // Fallback: estimate head position from screenBox
    Position headPos = {player.screenBox.pStart.x + player.screenBox.d.w / 2.0f,
                        player.screenBox.pStart.y};

    float dx = screenCenter.x - headPos.x;
    float dy = screenCenter.y - headPos.y;

    return sqrt(dx * dx + dy * dy);
  }

  /**
   * @brief Predicts target position based on movement
   * @param player The target player
   * @return Predicted aim position
   */
  Position PredictTargetPosition(const Player &player) {
    // Use head position as primary target
    if (!player.screenBones.empty()) {
      return player.screenBones[0].pStart;
    }

    // Fallback: use estimated head position from screenBox
    Position headPos = {player.screenBox.pStart.x + player.screenBox.d.w / 2.0f,
                        player.screenBox.pStart.y};

    return headPos;
  }

  /**
   * @brief Enhanced aim assistance with prediction and smoothing
   * @param target The target to aim at
   */
  void ApplyEnhancedAimAssist(const Player &target) {
    // Get predicted target position
    Position targetPos = PredictTargetPosition(target);
    Position screenCenter = ClientCenterPosition;

    // Calculate aim delta
    float deltaX = targetPos.x - screenCenter.x;
    float deltaY = targetPos.y - screenCenter.y;

    // Apply deadzone to prevent micro-jitter
    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
    if (distance < AIM_SMOOTHING_DEADZONE) {
      return; // Target is close enough, no adjustment needed
    }

    // Apply dynamic smoothing based on distance
    float dynamicSmoothness = config.smoothness;

    // Increase smoothness for distant targets (more precise aiming)
    if (distance > 50.0f) {
      dynamicSmoothness *= 1.5f;
    }

    // Apply enhanced smoothing with target prediction
    MyLocalPlayer.SetViewAngles(target.aimAngle, dynamicSmoothness);
  }

  /**
   * @brief Legacy aim assistance (kept for compatibility)
   * @param target The target to aim at
   */
  void ApplyAimAssist(const Player &target) { ApplyEnhancedAimAssist(target); }

  /**
   * @brief Enhanced auto-click with timing control and target validation
   */
  void HandleAutoClick() {
    if (!config.isClickActive)
      return;

    // Only auto-click if we have a valid target in crosshair
    if (MyLocalPlayer.crossIndex >= 0) {
      // Verify the target is still valid and within aim circle
      bool targetStillValid = false;
      for (const Player &player : ENEMIES) {
        if (IsValidTarget(player) && IsWithinAimCircle(player)) {
          targetStillValid = true;
          break;
        }
      }

      if (targetStillValid) {
        // Add slight delay to prevent excessive clicking
        static auto lastClickTime = std::chrono::steady_clock::now();
        auto currentTime = std::chrono::steady_clock::now();
        auto timeSinceLastClick =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                currentTime - lastClickTime);

        if (timeSinceLastClick.count() > 25) { // Minimum 25ms between clicks
          mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
          mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
          lastClickTime = currentTime;
        }
      }
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
