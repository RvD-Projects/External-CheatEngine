#include "esp.h"
#include <iostream>
using namespace Gui;

using namespace cs2_dumper::offsets::client_dll;
using namespace cs2_dumper::schemas::client_dll;
using namespace cs2_dumper::schemas::client_dll::C_BaseEntity;
using namespace cs2_dumper::schemas::client_dll::C_BasePlayerPawn;
using namespace cs2_dumper::schemas::client_dll::C_BaseModelEntity;
using namespace cs2_dumper::schemas::client_dll::CCSPlayerController;


void Esp::Frame()
{
	pDevice->Clear(NULL, NULL, D3DCLEAR_TARGET, NULL, 1.0f, NULL);
	pDevice->BeginScene();

	frame();
	Render();

	pDevice->EndScene();
	pDevice->Present(NULL, NULL, NULL, NULL);
}

void Esp::Loop()
{
	auto x = dwEntityList;
	uintptr_t entity_list = memory::Read<uintptr_t>(modBase +  dwEntityList);
	uintptr_t localPlayerPawn = memory::Read<uintptr_t>(modBase + dwLocalPlayerPawn);

	BYTE team = memory::Read<BYTE>(localPlayerPawn + m_iTeamNum);

	while (true)
	{
		std::vector<uintptr_t> buffer = {};

		for (int i = 1; i < 32; i++)
		{
			uintptr_t listEntry = memory::Read<uintptr_t>(entity_list + ((8 * (i & 0x7ff) >> 9) + 16));
			if (!listEntry) continue;

			uintptr_t entityController = memory::Read<uintptr_t>(listEntry + 120 * (i & 0x1ff));
			if (!entityController) continue;

			uintptr_t entityControllerPawn = memory::Read<uintptr_t>(entityController + m_hPlayerPawn);
			if (!entityControllerPawn) continue;

			uintptr_t entity = memory::Read<uintptr_t>(listEntry + 120 * (entityControllerPawn & 0x1ff));
			if (!entity) continue;
			
			buffer.emplace_back(entity);
		}

		entities = buffer;
		Sleep(16);
	}
}

void Esp::Render()
{
	vm = memory::Read<ViewMatrix>(modBase + dwViewMatrix);

	for (uintptr_t entity : entities)
	{
		vec3 absOrigin = ::memory::Read<vec3>(entity + m_vOldOrigin);
		vec3 viewCamPosition = absOrigin + memory::Read<vec3>(entity + m_vecViewOffset);

		vec2 headScreen, feetScreen;
		bool  w2sOrig = WorldToScreen(absOrigin, headScreen, vm.m);
		bool  w2sCam = WorldToScreen(viewCamPosition, feetScreen, vm.m);

		if (!w2sOrig || !w2sCam) {
			continue;
		}

		float entityPawnWidth = headScreen.y - feetScreen.y;
		feetScreen.x += entityPawnWidth;
		feetScreen.y -= entityPawnWidth;

		draw::box(
			D3DXVECTOR2(headScreen.x, headScreen.y),
			D3DXVECTOR2(feetScreen.x, feetScreen.y),
			D3DCOLOR_RGBA(0, 255, 0, 255)
		);
	}
}

bool Esp::WorldToScreen(const vec3& world, vec2& screen, float m[16])
{
	vec4 clipCoords;
	clipCoords.x = world.x * m[0] + world.y * m[1] + world.z * m[2] + m[3];
	clipCoords.y = world.x * m[4] + world.y * m[5] + world.z * m[6] + m[7];
	clipCoords.z = world.x * m[8] + world.y * m[9] + world.z * m[10] + m[11];
	clipCoords.w = world.x * m[12] + world.y * m[13] + world.z * m[14] + m[15];


	if (clipCoords.w < 0.1f) {
		return false;
	}

	vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	int W2 = WINDOW_W / 2;
	int H2 = WINDOW_H / 2;
	screen.x = W2 * NDC.x + NDC.x + W2;
	screen.y = -H2 * NDC.y + NDC.y + H2;

	return true;
}