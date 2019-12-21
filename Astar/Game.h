#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <utility>
#include <map>
#include <string>
#include <vector>
#include "Graph.h"
#include "GridManager.h"


typedef GraphArc<std::string, int> Arc;
typedef GraphNode<std::string, int> Node;

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
	const int SANDBOX_TILE_AMOUNT = 2500;

	const int TEST_LAYOUT_ROWS = 10;
	const int TEST_LAYOUT_COLS = 14;

	const int SANDBOX_LAYOUT_ROWS = 50;
	const int SANDBOX_LAYOUT_COLS = 50;
	
	// functions
	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();
	void processScreenEvents();
	void handleGridPathfinding();
	void setupGrid();
	std::string getStringIndex(int t_index);
	int getNumIndex(std::string t_index);

	void initLayout();

	std::map<std::string, int> m_nodeMap;
	std::string m_start;
	std::string m_end;

	// variables
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_font; // font used by message

	bool m_exitGame; // control exiting game
	bool m_loadLayout = false;

	//GridManager m_grid;
	GridManager* m_grid;
	GridLayout m_layout;
	//text
	sf::Text m_tooltipText;
	sf::RectangleShape m_textBackground;

};
