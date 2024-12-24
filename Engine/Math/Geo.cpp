#include "Geo.h"

bool Geo::Get2DVector(const vec3& world, vec2& screen, float m[16])
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

	int W2 = 1920 / 2;
	int H2 = 1080 / 2;
	screen.x = W2 * NDC.x + NDC.x + W2;
	screen.y = -H2 * NDC.y + NDC.y + H2;

	return true;
}
