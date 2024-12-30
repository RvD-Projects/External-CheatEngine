#pragma once

#include "../Modules/Module.h"

using namespace Modules;
using namespace Modules::GameRules;

class C4Bomb
{
public:
	uintptr_t plantedPtr;
	std::string name = "C4Bomb";
	Timer FuseChrono = Timer(39500);
	Timer DefuseChrono = Timer(29500);
	Timer DefuseKitChrono = Timer(34500);

	UINT team;
	Vector3 position;
	Dimension screen_d, esp_d;
	Position screenPos, esp_p;

	bool isPlanted;
	bool isTeammate;

	void Update()
	{
		isPlanted = BombIsPlanted();

		if (!isPlanted)
		{
			DefuseKitChrono.Reset();
			DefuseChrono.Reset();
			FuseChrono.Reset();
			return;
		}

		if (!FuseChrono.IsRunning())
		{
			DefuseKitChrono.Start();
			DefuseChrono.Start();
			FuseChrono.Start();
		}
	}
};
