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

struct vec3
{
	float x, y, z;
	vec3 operator+(vec3 other)
	{
		return {this->x + other.x, this->y + other.y, this->z + other.z};
	}
	vec3 operator-(vec3 other)
	{
		return {this->x - other.x, this->y - other.y, this->z - other.z};
	}

	vec3 RelativeAngle()
	{
		return {
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f};
	}
};

struct vec4
{
	float w, x, y, z;
};

struct APP_INFO
{
	const wchar_t *w_name;
	const wchar_t *exe_name;
	const wchar_t *dll_name;

	uintptr_t pid;
	uintptr_t dll;

	RECT rect;
	Position position;
	Dimension dimension;
};
