#pragma once
#include "GridTile.h"


class GridManager
{
public:
	GridManager(sf::Font& t_font, sf::RenderWindow& t_window);
	~GridManager();
	void render();
	bool update();
	void init(float t_textOffset);
	int getStartIndex();
	int getGoalIndex();
	void updateNotRequired();
	void setVisited(int t_index);
	void setToPath(int t_index);
	void resetPath();

private:
	void handleInput();
	void handleKeyboard();
	void handleMouse();
	void handleLeftClick(sf::Vector2i t_mousePos);
	void handleRightClick(sf::Vector2i t_mousePos);
	void resetGrid();
	void resetNonObstacles();
	int getNeighbourIndex(int t_index, int t_indexOfTileToGetNeighbours);
	void checkIfStartRemoved(int t_tileClicked);
	int getTileIndex(sf::Vector2i t_mousePos);
	void setTestLayout();

	//##############################
	//vectors
	std::vector<GridTile> m_grid;
	sf::Vector2f m_tileSize;

	//references
	sf::Font& m_font;
	sf::RenderWindow& m_window;

	//consts
	const int MAX_TILES = 130;
	const int TILES_PER_ROW = 10;
	const int NO_OF_COLS = 14;

	//side neighbours
	const int LEFT_TILE = -1;
	const int RIGHT_TILE = 1;
	//top neighbours
	const int TOP_TILE = -10;
	const int LEFT_TOP_TILE = -11;
	const int RIGHT_TOP_TILE = -9;
	//bottom neighbours
	const int BOTTOM_TILE = 10;
	const int LEFT_BOTTOM_TILE = 9;
	const int RIGHT_BOTTOM_TILE = 11;

	//bools
	bool m_leftBtn = false;
	bool m_rightBtn = false;
	bool m_rPressed = false;
	bool m_gridUpdateRequired = false;

	//ints
	int m_goalIndex = -1;
	int m_startIndex = -1;
};

