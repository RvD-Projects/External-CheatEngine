#pragma once

#include "Engine.h"

using namespace Engine;

class Player
{
public:
	bool isInitialized = false;
	bool isLocalPlayer = false;
	bool isLocalPlayerTeam = false;

	bool takesDamage;
	int health, armor, maxHealth, lifeState, hammerID;

	int team;
	std::string name;
	vec3 position, viewCamPos;

	uintptr_t entity, ctrl, pawnCtrl, pawn;

	Player() {};
	Player(const uintptr_t list, const int& index)
	{
		entity = Read<uintptr_t>(EntityEntryDiff(index) + list);
		if (!entity)
			return;

		ctrl = ReadEntity<uintptr_t>(EntityDiff(index));
		if (!ctrl)
			return;

		pawnCtrl = ReadController<uintptr_t>(m_hPlayerPawn);
		if (!pawnCtrl)
			return;

		entity = ReadEntity<uintptr_t>(EntityDiff(pawnCtrl));
		if (!entity)
			return;
		
		team = ReadEntity<int>(m_iTeamNum);
		health = ReadEntity<int>(m_iHealth);
		armor = ReadEntity<int>(m_iPawnArmor);
		maxHealth = ReadEntity<int>(m_iMaxHealth);
		position = ReadEntity<vec3>(m_vOldOrigin);
		viewCamPos = position + ReadEntity<vec3>(m_vecViewOffset);

		isLocalPlayer = ReadClient<uintptr_t>(dwLocalPlayerPawn) == entity;
		name = ReadString(ReadController<uintptr_t>(m_sSanitizedPlayerName));

		isInitialized = true;
	};

	template <typename T>
	T ReadEntity(ptrdiff_t ptr_diff)
	{
		return Read<T>(entity + ptr_diff);
	};

	template <typename T>
	T ReadController(ptrdiff_t ptr_diff)
	{
		return Read<T>(ctrl + ptr_diff);
	};

	template <typename T>
	T ReadEntityPawn(ptrdiff_t ptr_diff)
	{
		return Read<T>(pawn + ptr_diff);
	};
};
