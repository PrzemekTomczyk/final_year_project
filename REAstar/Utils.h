#pragma once

enum class NeighbourIndex
{
	LEFT = 0,
	RIGHT = 1,
	TOP = 2,
	BOTTOM = 3,
	TOP_LEFT = 4,
	TOP_RIGHT = 5,
	BOTTOM_LEFT = 6,
	BOTTOM_RIGHT = 7,
	NONE = 8
};
namespace Utils
{

	const float diag = std::sqrtf(2.0f);
}