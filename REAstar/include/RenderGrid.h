#pragma once
#include "GridTile.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <list>
#include <queue>
#include <vector>
#include <functional>
#include <Thor/Time.hpp>
#include <stdexcept>
#include <chrono>
#include <thread>
#include "BoundaryNode.h"
#include "RectNode.h"
#include "Utils.h"
#include "SearchNode.h"
#include <chrono> 

class RenderGrid
{
public:
	RenderGrid(sf::Font& t_font, sf::RenderWindow& t_window, int t_maxTiles, int t_noOfRows, int t_tilesPerRow);
	~RenderGrid();
	void update();
	void render();
	void init(float t_textOffset, const sf::Vector2f& t_textPos, int t_charSize);

private:
	void initText(float t_textOffset, const sf::Vector2f& t_textPos, int t_charSize);
	void handleInput();
	void handleKeyboard();
	void handleMouse();
	void handleLeftClick(sf::Vector2i t_mousePos);
	void handleRightClick(sf::Vector2i t_mousePos);
	void handleMiddleClick(sf::Vector2i t_mousePos);
	void resetGrid();
	void resetNonObstacles();
	int getNeighbourIndex(NeighbourIndex t_neighbour, int t_index);
	bool cardinalDirectionsAvailable(int t_index);
	void checkIfStartRemoved(int t_tileClicked);
	
	int getClickedTileIndex(sf::Vector2i t_mousePos);
	void setTestLayout();
	
	void addLine(sf::Vector2f t_p1, sf::Vector2f t_p2);
	float backTrack();

	void reaGridRedraw();
	void gridRedraw();
	void setToPath(int t_index);
	void prePathfindReset();
	void updateText(const sf::Vector2f& t_textPos);

	//vectors
	std::vector<GridTile*> m_grid;

	//references
	sf::Font& m_font;
	sf::RenderWindow& m_window;

	//text object
	sf::Text m_placeModeTxt;
	std::string m_placeString;
	std::string m_deleteString;

	sf::Text m_debugText;
	sf::Text m_debugStateText;
	std::string m_debugString;
	std::string m_debugStateString;

	sf::Text m_algTypeText;
	sf::Text m_algText;
	std::string m_algString;
	std::string m_algTypeString;

	sf::Text m_reaTimeTakenText;
	sf::Text m_aStarTimeTakenText;

	sf::Text m_reaDistText;
	sf::Text m_aStarDistText;

	//consts
	const int TEST_LAYOUT = 140;
	const int MAX_TILES;
	const int NO_OF_ROWS;
	const int TILES_PER_ROW;

	//side neighbours
	const int LEFT_TILE = -1;
	const int RIGHT_TILE = 1;
	//top neighbours
	const int TOP_TILE;
	const int LEFT_TOP_TILE;
	const int RIGHT_TOP_TILE;
	//bottom neighbours
	const int BOTTOM_TILE;
	const int LEFT_BOTTOM_TILE;
	const int RIGHT_BOTTOM_TILE;

	//bools
	bool m_leftBtn = false;
	bool m_rightBtn = false;
	bool m_middleBtn = false;
	bool m_rPressed = false;
	bool m_spacePressed = false;
	bool m_changedGrid = false;
	bool m_updateRequired = false;
	bool m_deleteMode = false;
	bool m_enterPressed = false;
	bool m_tabPressed = false;
	bool m_getPath = false;
	bool m_f5Pressed = false;

	//ints
	int m_goalIndex = -1;
	int m_startIndex = -1;

	std::vector<sf::Vertex> m_lines;

	const int LEFT = 0;
	const int RIGHT = 1;
	const int TOP = 2;
	const int BOT = 3;
};