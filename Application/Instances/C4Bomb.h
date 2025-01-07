#pragma once

#include "../Modules/Module.h"

class C4Bomb
{
public:
	uintptr_t entity, scene;
	std::string name = "C4Bomb";
	Timer FuseChrono = Timer(40500);
	Timer DefuseChrono = Timer(30500);
	Timer DefuseKitChrono = Timer(35500);

	int team;
	Vector3 position;
	Dimension esp_d;
	Position esp_p;

	bool isPlanted;
	bool isTeammate;

	template <typename T>
	T ReadEntity(const ptrdiff_t &ptr_diff)
	{
		return Read<T>(entity + ptr_diff);
	};

	template <typename T>
	T ReadScene(const ptrdiff_t &ptr_diff)
	{
		return Read<T>(scene + ptr_diff);
	};

	void Update()
	{
		isPlanted = BombIsPlanted();

		if (!isPlanted)
		{
			scene = NULL;
			entity = NULL;
			DefuseKitChrono.Reset();
			DefuseChrono.Reset();
			FuseChrono.Reset();
			return;
		}

		entity = entity ? entity : NULL;
		scene = scene ? scene : NULL;

		if (!FuseChrono.IsRunning())
		{
			DefuseKitChrono.Start();
			DefuseChrono.Start();
			FuseChrono.Start();
		}
	}
};
