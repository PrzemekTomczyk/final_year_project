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

	const static std::string INSTRUCTIONS = "Mouse controls:\n\nPress LMB to place Goal\n\nPress RMB to place Start Tiles\n\nPress/hold MMB to place Obstacles\n\n\nKeyboard controls:\n\nPress ENTER to start search\n\t**START and GOAL required**\n\nPress SPACE to toggle between\nplacing obstacles and \nreseting tiles using MMB\n\nPress TAB to switch algorithms\nUsing \n\nPress F1 to load Test Layout\n\nPress F2 to load Sandbox Layout\n\nPress F5 to turn debug ON/OFF\nDEBUG \n\nPress R reset the grid";

	const static std::string DEBUG_ON = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t ON";
	const static std::string DEBUG_OFF = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t OFF";
	const static std::string DEBUG_REA = "\nPress N to step or F5 to breakout";
	const static std::string DEBUG_ASTAR = "\nPress N to breakout";

	const static std::string USE_REA = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\tREA*";
	const static std::string USE_ASTAR = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\tA*";

	const static std::string REA_TIME_TAKEN_ALIGNMENT = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nREA* Stats\n";
	const static std::string ASTAR_TIME_TAKEN_ALIGNMENT = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nA* Stats\n";

	const static std::string REA_DISTANCE_ALIGNMENT = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	const static std::string ASTAR_DISTANCE_ALIGNMENT = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

	const static int TEST_TILE_AMOUNT = 140;
	const static int SMALL_SANDBOX_TILE_AMOUNT = 10000;
	const static int MED_SANDBOX_TILE_AMOUNT = 250000;
	const static int LARGE_SANDBOX_TILE_AMOUNT = 1000000;

	const static int TEST_LAYOUT_TILES_PER_ROW = 10;
	const static int TEST_LAYOUT_ROWS = 14;

	const static int SMALL_SANDBOX_LAYOUT_TILES_PER_ROW = 100;
	const static int SMALL_SANDBOX_LAYOUT_ROWS = 100;

	const static int MED_SANDBOX_LAYOUT_TILES_PER_ROW = 500;
	const static int MED_SANDBOX_LAYOUT_ROWS = 500;

	const static int LARGE_SANDBOX_LAYOUT_TILES_PER_ROW = 1000;
	const static int LARGE_SANDBOX_LAYOUT_ROWS = 1000;

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