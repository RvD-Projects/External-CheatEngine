#pragma once

#include <cmath>
#include <numbers>
#include <algorithm>
#include <corecrt_math_defines.h>

struct Dimension
{
	float w = 1, h = 1, d = 1;

	Dimension operator/(float dividend)
	{
		return {this->w / dividend, this->h / dividend, this->d / dividend};
	}
	Dimension operator*(float dividend)
	{
		return {this->w * dividend, this->h * dividend, this->d * dividend};
	}
};

struct Position
{
	float x, y;
	Position operator+(Position other)
	{
		return {this->x + other.x, this->y + other.y};
	}
	Position operator-(Position other)
	{
		return {this->x - other.x, this->y - other.y};
	}
	Position operator*(float other)
	{
		return {this->x * other, this->y * other};
	}
	Position operator/(float other)
	{
		return {this->x / other, this->y / other};
	}

	Position operator+(Position *other)
	{
		return {this->x + other->x, this->y + other->y};
	}
	Position operator-(Position *other)
	{
		return {this->x - other->x, this->y - other->y};
	}
	Position operator*(float *other)
	{
		return {this->x * *other, this->y * *other};
	}
	Position operator/(float *other)
	{
		return {this->x / *other, this->y / *other};
	}
};

struct Vector3
{
	float x, y, z;
	Vector3 operator+(Vector3 other)
	{
		return {this->x + other.x, this->y + other.y, this->z + other.z};
	}
	Vector3 operator-(Vector3 other)
	{
		return {this->x - other.x, this->y - other.y, this->z - other.z};
	}
	Vector3 operator-(const Vector3 &other) const
	{
		return Vector3{x - other.x, y - other.y, z - other.z};
	}
	Vector3 operator*(Vector3 other)
	{
		return {this->x * other.x, this->y * other.y, this->z * other.z};
	}
	Vector3 operator/(Vector3 other)
	{
		return {this->x / other.x, this->y / other.y, this->z / other.z};
	}

	/**
	 * Calculates the distance between two 3D vectors.
	 *
	 * @param other The other vector to calculate the distance to.
	 * @return A Vector3 object containing the distance between the two vectors.
	 */
	Vector3 Distance(Vector3 other)
	{
		return {this->x - other.x, this->y - other.y, this->z - other.z};
	};

	/**
	 * Calculates the Euclidean distance between two 3D vectors.
	 *
	 * @param other The other vector to calculate the distance to.
	 * @return The Euclidean distance between the two vectors.
	 */
	float EuclideanDistance(Vector3 other)
	{
		return std::sqrt(std::pow(this->x - other.x, 2) + std::pow(this->y - other.y, 2) + std::pow(this->z - other.z, 2));
	}

	/**
	 * Calculates the relative angle of a vector in 3D space.
	 *
	 * This method computes the angles in degrees between the vector and the
	 * horizontal plane, as well as the angle in the XY plane, considering an
	 * optional height difference.
	 *
	 * @param heightDifference An optional height difference to adjust the Z-axis
	 *                         component of the vector. Default is 0.
	 * @return A Vector3 object containing the angles in degrees: the angle with
	 *         the horizontal plane, the angle in the XY plane, and a fixed 0.0f
	 *         for the Z-axis.
	 */
	Vector3
	ToAngle(float heightDifference = 0)
	{
		return {
			std::atan2(-(z + heightDifference), std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f};
	}
};

struct Vector4
{
	float w, x, y, z;
};

struct Line3D
{
	Vector3 v1;
	Vector3 v2;
};
