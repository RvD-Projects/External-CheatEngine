#pragma once

#include "Bones.h"
#include "../Modules/Module.h"

using namespace Bones;
using namespace Modules;
using namespace Modules::GameRules;

class Player
{
public:
	bool isInitialized, isLocalPlayer, isTeammate, isAlive, takesDamage = false;
	int team, health, armor, maxHealth, hammerID, crossIndex, lifeState;

	std::string name;
	Vector3 position, viewCamPos, distance;
	Position screenFeet, screenEye, esp_p;
	Dimension screen_d, esp_d;
	std::vector<Line3D> bones;
	std::vector<Line> screenBones;

	uintptr_t listEntry, ctrl, pawnCtrl, entity, sceneNode, boneMatrix;

	Player() {};
	Player(const uintptr_t &list, const int &index)
	{
		listEntry = Read<uintptr_t>(EntityEntryDiff(index) + list);
		if (!listEntry)
			return;

		ctrl = ReadListEntry<uintptr_t>(EntityDiff(index));
		if (!ctrl)
			return;

		pawnCtrl = ReadController<uintptr_t>(m_hPlayerPawn);
		if (!pawnCtrl)
			return;

		entity = ReadListEntry<uintptr_t>(EntityDiff(pawnCtrl));
		if (!entity)
			return;

		Init();
	}

	void Init()
	{
		name = ReadString(ReadController<uintptr_t>(m_sSanitizedPlayerName));
		position = ReadEntity<Vector3>(m_vOldOrigin);
		viewCamPos = position + ReadEntity<Vector3>(m_vecViewOffset);

		team = ReadEntity<int>(m_iTeamNum);
		health = ReadEntity<int>(m_iHealth);
		maxHealth = ReadEntity<int>(m_iMaxHealth);
		armor = ReadController<int>(m_iPawnArmor);
		takesDamage = ReadEntity<bool>(m_bTakesDamage);
		lifeState = ReadEntity<int>(m_lifeState);
		isAlive = lifeState == 256;

		isLocalPlayer = GetLocalPlayer_T() == entity;
		isTeammate = isLocalPlayer || ReadLocalPlayer<int>(m_iTeamNum) == team;
		crossIndex = isLocalPlayer ? ReadLocalPlayer<int>(m_iIDEntIndex) : -1;

		if (!isTeammate)
		{
			sceneNode = ReadEntity<uintptr_t>(m_pGameSceneNode);
			boneMatrix = Read<uintptr_t>(sceneNode + m_modelState + 0x80);

			for (size_t i = 0; i < NbOfBoneConnections; i++)
			{
				Line3D boneLine3D{
					Read<Vector3>(boneMatrix + BoneConnections[i].bone1 * 32),
					Read<Vector3>(boneMatrix + BoneConnections[i].bone2 * 32)};

				bones.emplace_back(boneLine3D);
			}
		}

		isInitialized = true;
	};

	template <typename T>
	T ReadListEntry(const ptrdiff_t &ptr_diff)
	{
		return Read<T>(listEntry + ptr_diff);
	};

	template <typename T>
	T ReadController(const ptrdiff_t &ptr_diff)
	{
		return Read<T>(ctrl + ptr_diff);
	};

	template <typename T>
	T ReadPawnController(const ptrdiff_t &ptr_diff)
	{
		return Read<T>(pawnCtrl + ptr_diff);
	};

	template <typename T>
	T ReadEntity(const ptrdiff_t &ptr_diff)
	{
		return Read<T>(entity + ptr_diff);
	};

	void GetEsp(Position &posOut, Dimension &dimOut)
	{
		posOut = this->esp_p;
		dimOut = this->esp_d;
	}

	const bool IsLocalPlayer()
	{
		return isLocalPlayer;
	}

	const bool IsEnemy()
	{
		return !isLocalPlayer && !isTeammate;
	}

	const bool IsAlive()
	{
		return isAlive && health > 0;
	}

	const bool IsValidTarget()
	{
		return IsEnemy() && IsAlive();
	}
};
