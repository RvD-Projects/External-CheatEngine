#pragma once

#include "Bones.h"
#include "../Modules/Module.h"

using namespace Bones;

class Player
{
public:
	bool isInitialized, isLocalPlayer, isTeammate, isAlive, takesDamage = false;
	int team, health, armor, maxHealth, hammerID, crossIndex, lifeState;
	float height, width;

	std::string name;
	Vector3 position, viewCamPos, distance, viewAngles, aimAngle;

	std::vector<Line3D> bones;
	std::vector<Line> screenBones;

	Position screenFeet, screenEye;
	Box screenBox;

	uintptr_t listEntry, ctrl, entity, sceneNode, boneMatrix;
	int pawnCtrl;

	Player() {};
	Player(const uintptr_t &list, const int &index)
	{
		listEntry = Read<uintptr_t>(list + 0x10);
		if (!listEntry || listEntry == INTPTR_MAX)
			return;

		ctrl = ReadListEntry<uintptr_t>(index * 0x78);
		if (!ctrl || ctrl == INTPTR_MAX)
			return;

		pawnCtrl = ReadController<int>(m_hPlayerPawn);
		if (!pawnCtrl || pawnCtrl == INT_MAX)
			return;

		listEntry = Read<uintptr_t>(list + (0x8 * ((pawnCtrl & 0x7FFF) >> 9) + 0x10));
		if (!listEntry || listEntry == INTPTR_MAX)
			return;

		entity = ReadListEntry<uintptr_t>(0x78 * (pawnCtrl & 0x1FF));
		if (!entity || entity == INTPTR_MAX)
			return;

		Init();
	}

	void Init()
	{
		name = ReadString(ReadController<uintptr_t>(m_sSanitizedPlayerName));
		position = ReadEntity<Vector3>(m_vOldOrigin);
		viewCamPos = position + ReadEntity<Vector3>(m_vecViewOffset);
		viewAngles = ReadClient<Vector3>(dwViewAngles);
		height = viewCamPos.z - position.z;
		width = height * 0.777f;

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

		sceneNode = ReadEntity<uintptr_t>(m_pGameSceneNode);
		boneMatrix = Read<uintptr_t>(sceneNode + m_modelState + 0x80);

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

	bool SetViewAngles(const Vector3 &targetAngles, float smoothValue = 1.00F)
	{
		smoothValue = smoothValue > 0 ? smoothValue : 1;

		Vector3 deltaAngle = targetAngles - viewAngles;
		while (deltaAngle.y > 180.f)
			deltaAngle.y -= 360.f;

		while (deltaAngle.y < -180.f)
			deltaAngle.y += 360.f;

		if (viewAngles.x != targetAngles.x)
			viewAngles.x += deltaAngle.x / smoothValue;

		if (viewAngles.y != targetAngles.y)
			viewAngles.y += deltaAngle.y / smoothValue;

		return WriteClient<Vector3>(dwViewAngles, viewAngles);
	}
};
