#pragma once

#include "../Modules/Module.h"

using namespace Modules;
using namespace Modules::GameRules;

class Player
{
public:
	bool isInitialized, isLocalPlayer, isTeammate, isAlive, takesDamage = false;
	int team, health, armor, maxHealth, hammerID, crossIndex, lifeState;

	std::string name;
	Vector3 position, viewCamPos;
	Position screenFeet, screenEye, esp_p;
	Dimension screen_d, esp_d;

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

		team = ReadEntity<int>(m_iTeamNum);
		health = ReadEntity<int>(m_iHealth);
		armor = ReadController<int>(m_iPawnArmor);
		maxHealth = ReadEntity<int>(m_iMaxHealth);
		lifeState = ReadEntity<int>(m_lifeState);
		takesDamage = ReadEntity<bool>(m_bTakesDamage);
		position = ReadEntity<Vector3>(m_vOldOrigin);
		isAlive = lifeState == 256;

		isLocalPlayer = GetLocalPlayer_T() == entity;
		isTeammate = !isLocalPlayer && ReadLocalPlayer<int>(m_iTeamNum) == team;
		crossIndex = isLocalPlayer ? ReadLocalPlayer<int>(m_iIDEntIndex) : -1;

		viewCamPos = position + ReadEntity<Vector3>(m_vecViewOffset);
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
