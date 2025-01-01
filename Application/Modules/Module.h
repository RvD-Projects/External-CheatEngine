#pragma once

#include "Configs.h"
#include "../Instances/Player.h"
#include "../Instances/C4Bomb.h"

class Module
{
	void Loop()
	{
		this->Init(this->rootModule);

		while (this->config.isActive)
		{
			this->UpdatePointers(this->rootModule);
			this->Execute();
			std::this_thread::sleep_for(std::chrono::milliseconds(this->config.refreshRate));
		}
	}

public:
	virtual void Render() {}

	void Init(Module *rootModule)
	{
		this->UpdatePointers(rootModule);
		this->Init();
	}

protected:
	std::thread thRead;
	Module *rootModule = nullptr;
	Module *parentModule = nullptr;

	ModuleConfig config;

	ViewMatrix VM;
	Dimension ClientDimension;
	Position ClientCenterPosition;

	uintptr_t ENTITIES_LIST;
	std::vector<Player> ENTITIES;
	std::vector<Player> ENEMIES;
	std::vector<Player> FRIENDLIES;
	Player MyLocalPlayer;

	C4Bomb C4Bomb;

	virtual void Init()
	{
		this->config.isReady = true;
		this->config.isActive = true;
	};

	virtual void Execute() {};

	void UpdatePointers(Module *rootModule)
	{
		if (rootModule)
		{
			this->rootModule = rootModule;

			this->VM = this->rootModule->VM;
			this->ClientDimension = this->rootModule->ClientDimension;
			this->ClientCenterPosition = this->rootModule->ClientCenterPosition;

			this->ENTITIES_LIST = this->rootModule->ENTITIES_LIST;
			this->ENTITIES = this->rootModule->ENTITIES;
			this->ENEMIES = this->rootModule->ENEMIES;
			this->FRIENDLIES = this->rootModule->FRIENDLIES;
			this->MyLocalPlayer = this->rootModule->MyLocalPlayer;

			this->C4Bomb = this->rootModule->C4Bomb;
		}
	}

	template <typename T>
	T ReadEntities(const ptrdiff_t &ptr_diff)
	{
		return Read<T>(ENTITIES_LIST + ptr_diff);
	};

	const bool GetCrosshairTarget(Player &inOut)
	{
		if (MyLocalPlayer.crossIndex < 0)
			return false;

		const uintptr_t listEntry_t = ReadEntities<uintptr_t>(0x8 * (MyLocalPlayer.crossIndex >> 9) + 16);
		const uintptr_t entity_t = Read<uintptr_t>(listEntry_t + 120 * (MyLocalPlayer.crossIndex & 0x1ff));

		std::vector<Player> filteredPlayers;
		std::copy_if(ENTITIES.begin(), ENTITIES.end(),
					 std::back_inserter(filteredPlayers), [&entity_t](const Player &player)
					 { return player.entity == entity_t; });

		if (filteredPlayers.empty())
			return false;

		inOut = filteredPlayers.at(0);

		return true;
	}

	GuiColor GetTimerColor(const Timer &timer)
	{
		auto timeLeft = std::chrono::duration_cast<std::chrono::milliseconds>(timer.Remaining()).count();
		if (timeLeft < 100)
			return Transparent;

		auto percent = timer.Remaining() / timer.Duration();
		if (percent > 0.5f)
			return White;

		if (percent > 0.15f)
			return Yellow;

		return Red75;
	}

public:
	Module() { thRead = std::thread(&Module::Loop, this); };

	~Module()
	{
		this->config.isReady = false;
		this->config.isActive = false;

		if (this->thRead.joinable())
			this->thRead.join();
	}
};
