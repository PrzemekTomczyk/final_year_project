#pragma once

#include <iostream>
#include <fstream>
#include <utility>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <array>

#include <SFML/Graphics.hpp>

#include "Thor/Vectors.hpp"

#include "GridManager.h"

enum class GridLayout
{
	TEST,
	SMALL,
	MEDIUM,
	LARGE
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

	// functions
	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();
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

