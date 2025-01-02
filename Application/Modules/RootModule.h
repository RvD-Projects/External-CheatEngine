#pragma once

#include "Module.h"
#include "EspModule.h"
#include "AimBotModule.h"

class RootModule : public Module
{
	EspModule *Esp;
	AimBotModule *AimBot;

	void Init() override
	{
		this->config.isActive = true;

		this->Esp = new EspModule();
		this->AimBot = new AimBotModule();

		this->Esp->Init(this);
		this->AimBot->Init(this);

		this->config.isReady = true;
	}

	void Execute() override
	{
		ClientDimension = GetClientDimension();
		ClientCenterPosition = GetClientCenterPosition();
		VM = ReadClient<ViewMatrix>(dwViewMatrix);

		UpdatePlayerEntities();
		C4Bomb.Update();
	};

	void UpdatePlayerEntities()
	{
		std::vector<Player> b = {};
		std::vector<Player> bE = {};
		std::vector<Player> bF = {};
		ENTITIES_LIST = ReadClient<uintptr_t>(dwEntityList);

		for (int i = 1; i < 32; i++)
		{
			Player player = Player(ENTITIES_LIST, i);

			if (!player.isInitialized)
				continue;

			if (player.isLocalPlayer)
				MyLocalPlayer = player;

			SetPlayerScreen(VM, player);
			b.emplace_back(player);

			if (player.isLocalPlayer)
				continue;

			player.IsEnemy()
				? bE.emplace_back(player)
				: bF.emplace_back(player);
		}

		ENTITIES = b;
		ENEMIES = bE;
		FRIENDLIES = bF;
	}

	void SetPlayerScreen(const ViewMatrix &VM, Player &player) const
	{
		if (!player.IsEnemy())
			return;

		if (!SetPlayerScreenPosition(VM, player))
			return;

		SetPlayerScreenBones(player);
	};

	bool SetPlayerScreenPosition(const ViewMatrix &VM, Player &player) const
	{
		Position FEET, EYES;
		if (!Get2DVector(player.viewCamPos, EYES, VM.matrix, ClientDimension))
			return false;

		if (!Get2DVector(player.position, FEET, VM.matrix, ClientDimension))
			return false;

		player.screenEye = EYES;
		player.screenFeet = FEET;

		const float height = abs(EYES.y - FEET.y) * 1.1777f;
		const float width = height * 0.777f;

		player.screenBox = Box{FEET, Dimension{width, height}};
		player.screenBox.pStart.x -= player.screenBox.d.w * 0.5f;
		player.screenBox.pStart.y -= player.screenBox.d.h;

		player.distance = player.position - MyLocalPlayer.position;

		return true;
	};

	void SetPlayerScreenBones(Player &player) const
	{
		for (const auto &Connection : BoneConnections)
		{
			Line3D boneLine3D{
				Read<Vector3>(player.boneMatrix + Connection.bone1 * 32),
				Read<Vector3>(player.boneMatrix + Connection.bone2 * 32)};

			Position p1, p2;
			if (!Get2DVector(boneLine3D.v1, p1, VM.matrix, ClientDimension))
				break;

			if (!Get2DVector(boneLine3D.v2, p2, VM.matrix, ClientDimension))
				break;

			player.screenBones.emplace_back(Line{p1, p2});
		}
	};

public:
	void Render() override
	{
		if (this->Esp)
			this->Esp->Render();

		if (this->AimBot)
			this->AimBot->Render();

		DrawTextual({2, 2}, "Overlay v 1.0.0");
	}
};
