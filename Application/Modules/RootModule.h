#pragma once

#include "AimBotModule.h"
#include "EspModule.h"
#include "Module.h"

class RootModule : public Module {
  EspModule *Esp;
  AimBotModule *AimBot;
  bool EXEC_ORDER_66 = false;

  void Init() override {
    this->config.isActive = true;

    this->Esp = new EspModule();
    this->AimBot = new AimBotModule();

    this->Esp->Init(this);
    this->AimBot->Init(this);

    this->config.isReady = true;
  }

  void Execute() override {
    // Toggle team selection (Numpad 0)
    if (GetAsyncKeyState(VK_NUMPAD0) & 1) {
      EXEC_ORDER_66 = !EXEC_ORDER_66;
    }

    ClientDimension = GetClientDimension();
    ClientCenterPosition = GetClientCenterPosition();
    VM = ReadClient<ViewMatrix>(dwViewMatrix);

    UpdatePlayerEntities();
    C4Bomb.Update();
  };

  void UpdatePlayerEntities() {
    std::vector<Player> b = {};
    std::vector<Player> bE = {};
    std::vector<Player> bF = {};
    ENTITIES_LIST = ReadClient<uintptr_t>(dwEntityList);

    for (int i = 1; i < 64; i++) {
      Player player = Player(ENTITIES_LIST, i);

      if (!player.isInitialized)
        continue;

      player.isTeammate = player.isTeammate && !EXEC_ORDER_66;

      if (player.isLocalPlayer)
        MyLocalPlayer = player;

      SetPlayerScreen(VM, player);
      b.emplace_back(player);

      if (player.isLocalPlayer)
        continue;

      player.IsEnemy() ? bE.emplace_back(player) : bF.emplace_back(player);
    }

    ENTITIES = b;
    ENEMIES = bE;
    FRIENDLIES = bF;
  }

  void SetPlayerScreen(const ViewMatrix &VM, Player &player) const {
    if (!player.IsEnemy())
      return;

    SetPlayerScreenPosition(VM, player);
    SetPlayerScreenBones(player);
  };

  bool SetPlayerScreenPosition(const ViewMatrix &VM, Player &player) const {
    Position FEET, EYES;
    if (!Get2DVector(player.viewCamPos, EYES, VM.matrix, ClientDimension)) {
      player.isScreenVisible = false;
      return false;
    }

    if (!Get2DVector(player.position, FEET, VM.matrix, ClientDimension)) {
      player.isScreenVisible = false;
      return false;
    }

    player.screenEye = EYES;
    player.screenFeet = FEET;
    player.isScreenVisible = true;

    const float height = abs(EYES.y - FEET.y) * 1.1777f;
    const float width = height * 0.777f;

    player.screenBox = Box{FEET, Dimension{width, height}};
    player.screenBox.pStart.x -= player.screenBox.d.w * 0.5f;
    player.screenBox.pStart.y -= player.screenBox.d.h;

    player.distance = player.position.Distance(MyLocalPlayer.position);
    player.aimAngle =
        player.distance.ToAngle(player.height - MyLocalPlayer.height);

    return true;
  };

  bool SetPlayerScreenBones(Player &player) const {
    if (!player.isScreenVisible)
      return false;

    for (const auto &Connection : BoneConnections) {
      Line3D bone3D{Read<Vector3>(player.boneMatrix + Connection.bone1 * 32),
                    Read<Vector3>(player.boneMatrix + Connection.bone2 * 32)};

      Position p1, p2;
      if (!Get2DVector(bone3D.v1, p1, VM.matrix, ClientDimension))
        return false;

      if (!Get2DVector(bone3D.v2, p2, VM.matrix, ClientDimension))
        return false;

      player.bones.emplace_back(bone3D);
      player.screenBones.emplace_back(Line{p1, p2});
    }

    return player.screenBones.size() > 0;
  };

public:
  void Render() override {
    if (this->Esp)
      this->Esp->Render();

    if (this->AimBot)
      this->AimBot->Render();

    DrawTextual({2, 2}, "Overlay v 1.0.0",
                isMainWindowHidden ? White : GameRed);
  }
};
