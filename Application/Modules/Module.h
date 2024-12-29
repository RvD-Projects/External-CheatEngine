#pragma once

#include "../Modules/Modules.h"
#include "../Instances/Player.h"
#include "../Gui/Gui.h"

using namespace Modules;
using namespace Modules::GameRules;

class Module
{
	void Loop()
	{
		this->Init(this->rootModule);

		this->isRunning = true;
		while (this->isRunning)
		{
			this->Execute();
			std::this_thread::sleep_for(std::chrono::milliseconds(this->refreshRate));
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
	DWORD refreshRate = 16;
	std::atomic<bool> isReady;
	std::atomic<bool> isRunning;
	Module *rootModule = nullptr;
	Module *parentModule = nullptr;

	ViewMatrix VM;
	uintptr_t ENTITIES_LIST;

	Player MyLocalPlayer;
	std::vector<Player> ENTITIES;
	std::vector<Player> ENEMIES;
	std::vector<Player> FRIENDLIES;

	virtual void Init() {};
	virtual void Execute() {};

	void UpdatePointers(Module *rootModule)
	{
		if (rootModule)
		{
			this->rootModule = rootModule;
			this->ENEMIES = this->rootModule->ENEMIES;
			this->ENTITIES = this->rootModule->ENTITIES;
			this->ENTITIES_LIST = this->rootModule->ENTITIES_LIST;
			this->FRIENDLIES = this->rootModule->FRIENDLIES;
			this->MyLocalPlayer = this->rootModule->MyLocalPlayer;
			this->VM = this->rootModule->VM;
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

public:
	Module()
	{
		thRead = std::thread(&Module::Loop, this);
	};

	~Module()
	{
		this->isRunning = false;
		if (this->thRead.joinable())
		{
			this->thRead.join();
		}
	}
};
