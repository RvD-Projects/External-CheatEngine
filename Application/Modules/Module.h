#pragma once

#include "../../Engine/GameRef.h"
#include "../Instances/Player.h"
#include "../Gui/Gui.h"

using namespace GameRef;

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

	void Init(Module* rootModule) {
		this->UpdatePointers(rootModule);
		this->Init();
	}

protected:
	std::thread thRead;
	DWORD refreshRate = 16;
	std::atomic<bool> isReady;
	std::atomic<bool> isRunning;
	Module* rootModule = nullptr;
	Module* parentModule = nullptr;

	ViewMatrix VM;
	uintptr_t ENTITIES_LIST;

	Player MyLocalPlayer;
	std::vector<Player> ENTITIES;
	std::vector<Player> ENEMIES;
	std::vector<Player> FRIENDLIES;

	virtual void Init() {};
	virtual void Execute() {};

	void UpdatePointers(Module* rootModule) {
		if (rootModule) {
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
	T ReadEntities(const ptrdiff_t& ptr_diff)
	{
		return Read<T>(ENTITIES_LIST + ptr_diff);
	};

public:
	Module() {
		thRead = std::thread(&Module::Loop, this);
	};

	~Module() {
		this->isRunning = false;
		if (this->thRead.joinable()) {
			this->thRead.join();
		}
	}
};
