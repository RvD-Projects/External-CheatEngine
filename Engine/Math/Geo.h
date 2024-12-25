#pragma once

#include "Vector.h"

class Geo
{
public:
	/**
	 * Transforms a 3D vector into a 2D vector using a transformation matrix and screen dimensions.
	 *
	 * @param v3 The input 3D vector to be transformed.
	 * @param v2 The output 2D vector resulting from the transformation.
	 * @param m A 4x4 transformation matrix used for the projection.
	 * @param sd The screen dimensions, defaulting to 1920x1080.
	 * @return True if the transformation is successful and the vector is within view; false otherwise.
	 */
	static bool Get2DVector(const vec3 &v3, vec2 &v2, const float m[16], const Dimension &sd = {1920, 1080})
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

		const float W2 = sd.w / 2;
		const float H2 = sd.h / 2;

		v2.x = (W2 * coords.x) + coords.x + W2;
		v2.y = -(H2 * coords.y) + coords.y + H2;

		return true;
	}
};
