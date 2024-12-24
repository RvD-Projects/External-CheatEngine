#pragma once

#include "Vector.h"

class Geo
{
	/**
	 * Transforms a 3D coordinate into a 2D coordinate.
	 *
	 * @param v3 The 3D coordinate as a vec3.
	 * @param v2 The resulting 2D coordinate as a vec2.
	 * @param m A 4x4 transformation matrix represented as a float array of size 16.
	 * @param d A float array of size 2 representing the dimensions of the 2D space.
	 * @return True if the transformation is successful and the point is visible on v2, false otherwise.
	 */
	static bool Get2DVector(const vec3 &v3, vec2 &v2, const float m[16], const float d[2])
	{
		vec4 clipCoords;
		clipCoords.x = v3.x * m[0] + v3.y * m[1] + v3.z * m[2] + m[3];
		clipCoords.y = v3.x * m[4] + v3.y * m[5] + v3.z * m[6] + m[7];
		clipCoords.z = v3.x * m[8] + v3.y * m[9] + v3.z * m[10] + m[11];
		clipCoords.w = v3.x * m[12] + v3.y * m[13] + v3.z * m[14] + m[15];

		if (clipCoords.w < 0.1f)
			return false;

		vec3 coords;
		coords.x = clipCoords.x / clipCoords.w;
		coords.y = clipCoords.y / clipCoords.w;
		coords.z = clipCoords.z / clipCoords.w;

		const float W2 = d[0] / 2;
		const float H2 = d[1] / 2;

		v2.x = W2 * coords.x + coords.x + W2;
		v2.y = -H2 * coords.y + coords.y + H2;

		return true;
	}
};
