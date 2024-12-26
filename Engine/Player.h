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
	int crossIndex;
	UINT team, health, armor, maxHealth, lifeState, hammerID;

	std::string name;
	vec3 position, viewCamPos;

	uintptr_t entity, ctrl, pawnCtrl, pawn;

	Player() {};
	Player(const uintptr_t list, const int &index)
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

		team = ReadEntity<UINT>(m_iTeamNum);
		health = ReadEntity<UINT>(m_iHealth);
		armor = ReadEntity<UINT>(m_iPawnArmor);
		maxHealth = ReadEntity<UINT>(m_iMaxHealth);
		lifeState = ReadEntity<UINT>(m_lifeState);
		position = ReadEntity<vec3>(m_vOldOrigin);
		viewCamPos = position + ReadEntity<vec3>(m_vecViewOffset);

		name = ReadString(ReadController<uintptr_t>(m_sSanitizedPlayerName));

		const uintptr_t LocalPlayer = ReadLocalPlayer<uintptr_t>();
		isLocalPlayer = LocalPlayer == entity;

		if (isLocalPlayer)
		{
			crossIndex = Read<int>(LocalPlayer + m_iIDEntIndex);
		}

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
