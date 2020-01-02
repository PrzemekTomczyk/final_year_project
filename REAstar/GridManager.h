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

class TileComparer
{
public:
	bool operator()(const GridTile* t1, const GridTile* t2)
	{
		return t1->getEstDist() > t2->getEstDist();
	}
};

class GridManager
{
	enum class NeighbourIndex
	{
		LEFT = 0,
		RIGHT = 1,
		TOP_LEFT = 2,
		TOP = 3,
		TOP_RIGHT = 4,
		BOTTOM_LEFT = 5,
		BOTTOM = 6,
		BOTTOM_RIGHT = 7
	};

public:
	GridManager(sf::Font& t_font, sf::RenderWindow& t_window, int t_maxTiles, int t_noOfRows, int t_tilesPerRow);
	~GridManager();
	bool update();
	void render();
	void init(float t_textOffset);
	int getStartIndex();
	int getGoalIndex();
	void updateNotRequired();
	void setToPath(int t_index);
	void resetPath();
	void aStar(std::function<void(GridTile*)> f_visit);
	void reaAlgorithm();

private:
	void handleInput();
	void handleKeyboard();
	void handleMouse();
	void handleLeftClick(sf::Vector2i t_mousePos);
	void handleRightClick(sf::Vector2i t_mousePos);
	void handleMiddleClick(sf::Vector2i t_mousePos);
	void resetGrid();
	void resetNonObstacles();
	int getNeighbourIndex(NeighbourIndex t_neighbour, int t_index);
	void checkIfStartRemoved(int t_tileClicked);
	int getClickedTileIndex(sf::Vector2i t_mousePos);
	void setTestLayout();

	bool insertS(std::vector<int>& t_neighbours);
	bool expand(int t_cbn);
	bool successor();
	float getOctileDist(sf::Vector2f t_p1, sf::Vector2f t_p2);

	//functions for finding rectangle boundaries
	bool getRectInDirection(std::vector<int>& t_rectBoundary, NeighbourIndex t_direction, int t_origin);
	void getRectInOpposite(std::vector<int>& t_rectBoundary, NeighbourIndex t_direction, int t_origin, int t_limit1, int t_limit2, bool& t_goalFound);
	int getSideBoundary(NeighbourIndex t_direction, int t_expandOrigin, int t_currentLimit, bool& t_goalFound);

	//##############################
	//vectors
	std::vector<GridTile*> m_grid;
	std::vector<int> m_reaList;
	sf::Vector2f m_tileSize;

	//references
	sf::Font& m_font;
	sf::RenderWindow& m_window;

	//text object
	sf::Text m_placeModeTxt;
	std::string m_placeString;
	std::string m_deleteString;

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

	thor::Timer m_stepTimer;

	//ints
	int m_goalIndex = -1;
	int m_startIndex = -1;
};


