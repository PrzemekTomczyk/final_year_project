#pragma once
#include <string>

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

	const static std::string INSTRUCTIONS = "Mouse controls:\n\nPress LMB to place Goal\n\nPress RMB to place Start Tiles\n\nPress/hold MMB to place Obstacles\n\n\nKeyboard controls:\n\nPress ENTER to start search\n\t**START and GOAL required**\n\nPress SPACE to toggle between\nplacing obstacles and \nreseting tiles using MMB\n\nPress F1 to load Test Layout\n\nPress F2 to load Sandbox Layout\n\nPress F5 to turn debug ON/OFF\n\nPress R reset the grid";

	static int DELAY_AMOUNT = 250;

	static NeighbourIndex LEFT = NeighbourIndex::LEFT;
	static NeighbourIndex RIGHT = NeighbourIndex::RIGHT;
	static NeighbourIndex TOP = NeighbourIndex::TOP;
	static NeighbourIndex BOTTOM = NeighbourIndex::BOTTOM;
	static NeighbourIndex BOTTOM_LEFT = NeighbourIndex::BOTTOM_LEFT;
	static NeighbourIndex BOTTOM_RIGHT = NeighbourIndex::BOTTOM_RIGHT;
	static NeighbourIndex TOP_LEFT = NeighbourIndex::TOP_LEFT;
	static NeighbourIndex TOP_RIGHT = NeighbourIndex::TOP_RIGHT;
}