#pragma once

#include "../../Engine/GameRef.h"

using namespace GameRef;

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

	uintptr_t listEntry, ctrl, pawnCtrl, entity, pawn;

	Player() {};
	Player(const uintptr_t &list, const int &index)
	{
		listEntry = Read<uintptr_t>(EntityEntryDiff(index) + list);
		if (!listEntry)
			return;

		ctrl = ReadListEntry<uintptr_t>(EntityDiff(index));
		if (!ctrl)
			return;

		Init(listEntry);
	};

	void Init(const uintptr_t &entry)
	{
		listEntry = entry;

		pawnCtrl = ReadController<uintptr_t>(m_hPlayerPawn);
		if (!pawnCtrl)
			return;

		entity = ReadListEntry<uintptr_t>(EntityDiff(pawnCtrl));
		if (!entity)
			return;

		team = ReadEntity<UINT>(m_iTeamNum);
		health = ReadEntity<UINT>(m_iHealth);
		armor = ReadController<UINT>(m_iPawnArmor);
		maxHealth = ReadEntity<UINT>(m_iMaxHealth);
		lifeState = ReadEntity<UINT>(m_lifeState);
		position = ReadEntity<vec3>(m_vOldOrigin);

		viewCamPos = position + ReadEntity<vec3>(m_vecViewOffset);
		name = ReadString(ReadController<uintptr_t>(m_sSanitizedPlayerName));

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

	template <typename T>
	T ReadEntityPawn(const ptrdiff_t &ptr_diff)
	{
		return Read<T>(pawn + ptr_diff);
	};

	const bool IsAlive()
	{
		return health > 0;
	}

	const bool IsValidTarget()
	{
		return !isLocalPlayerTeam && !isLocalPlayer && health > 0 && takesDamage;
	}

	const bool IsLocalPlayer()
	{
		return isLocalPlayer;
	}

	const bool IsEnemy()
	{
		return !isLocalPlayerTeam && !isLocalPlayer;
	}
};
