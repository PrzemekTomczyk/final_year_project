#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <utility>
#include <map>
#include <string>
#include <vector>
#include "VisNode.h"
#include "Graph.h"
#include "GridManager.h"


typedef GraphArc<std::string, int> Arc;
typedef GraphNode<std::string, int> Node;


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
	const int MAX_TILES;
	const int TILES_PER_ROW;
	const int NO_OF_COLS;
	
	// functions
	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();
	void processScreenEvents();
	void mouseCollision();
	void mouseSelect();
	void deselectAll();

	void drawLines();

	void createNodes();
	void loadTxtFiles();

	void setupNodes();
	void handleNodes(sf::Time t_deltaTime);
	void handleGridPathfinding();

	std::string getStringIndex(int t_index);
	int getNumIndex(std::string t_index);

	Graph<std::string, int> m_graph;
	std::map<std::string, int> m_nodeMap;

	std::vector<VisNode> m_visualNodes;
	std::vector<VisNode> m_legendNodes;

	std::string m_start;
	std::string m_end;

	// variables
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_font; // font used by message

	bool m_exitGame; // control exiting game
	bool m_clicked = false;
	bool m_twoSelected = false;

	GridManager m_grid;
	//text
	sf::Text m_tooltipText;
	sf::RectangleShape m_textBackground;

	void setupGrid();

	const bool USING_GRID;

};

