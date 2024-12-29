#pragma once

#include <cmath>
#include <numbers>

struct Dimension
{
	float w, h, d;

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
