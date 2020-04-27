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
	const static double DIAGONAL = std::sqrtf(2.0f);
	static NeighbourIndex LEFT = NeighbourIndex::LEFT;
	static NeighbourIndex RIGHT = NeighbourIndex::RIGHT;
	static NeighbourIndex TOP = NeighbourIndex::TOP;
	static NeighbourIndex BOTTOM = NeighbourIndex::BOTTOM;
	static NeighbourIndex BOTTOM_LEFT = NeighbourIndex::BOTTOM_LEFT;
	static NeighbourIndex BOTTOM_RIGHT = NeighbourIndex::BOTTOM_RIGHT;
	static NeighbourIndex TOP_LEFT = NeighbourIndex::TOP_LEFT;
	static NeighbourIndex TOP_RIGHT = NeighbourIndex::TOP_RIGHT;
}