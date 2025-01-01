#pragma once

#include <cmath>
#include <numbers>
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

	Vector3 RelativeAngle()
	{
		return {
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
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
