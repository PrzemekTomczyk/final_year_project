#pragma once

#include <iostream>
#include <fstream>
#include <utility>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

#include <SFML/Graphics.hpp>

#include "Thor/Vectors.hpp"

#include "boost/thread.hpp"
#include "boost/thread/scoped_thread.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/lock_types.hpp"

#include "GridManager.h"

enum class GridLayout
{
	TEST,
	SANDBOX
};

class Game
{
public:
	Game();
	~Game();

	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:
	const int TEST_TILE_AMOUNT = 140;
	const int SANDBOX_TILE_AMOUNT = 40000;

	const int TEST_LAYOUT_TILES_PER_ROW = 10;
	const int TEST_LAYOUT_ROWS = 14;

	const int SANDBOX_LAYOUT_TILES_PER_ROW = 200;
	const int SANDBOX_LAYOUT_ROWS = 200;

	// functions
	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();
	void processScreenEvents();
	void setupGrid();

	void initLayout();

	// variables
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_font; // font used by message

	bool m_exitGame; // control exiting game
	bool m_loadLayout = false;
	bool m_windowCreated = false;

	GridManager* m_grid;
	GridLayout m_layout;
	//text
	sf::Text m_tooltipText;
	sf::RectangleShape m_textBackground;
};

