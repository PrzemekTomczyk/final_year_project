#pragma once
#include "GridTile.h"


class GridManager
{
public:
	GridManager(sf::Font& t_font, sf::RenderWindow& t_window);
	~GridManager();
	void render();
	void update();
	void init(float t_textOffset);

private:
	void handleInput();
	void handleKeyboard();
	void handleMouse();
	void handleLeftClick(sf::Vector2i t_mousePos);
	void handleRightClick(sf::Vector2i t_mousePos);
	void handleMiddleClick(sf::Vector2i t_mousePos);
	void resetGrid();
	void resetNonObstacles();
	void findAndSetPath(int t_startIndex);
	int getNeighbourIndex(int t_index, int t_indexOfTileToGetNeighbours);
	void checkIfStartRemoved(int t_tileClicked);
	int getClickedTileIndex(sf::Vector2i t_mousePos);

	//##############################
	//REA* functionality
	void setTestLayout();

	std::string reaAlgorithm();
	std::string insertS();
	std::string successor();
	std::string expand(int t_cbn);

	std::vector<int> m_openList;

	//##############################
	//vectors
	std::vector<GridTile> m_grid;	
	sf::Vector2f m_tileSize;

	//references
	sf::Font& m_font;
	sf::RenderWindow& m_window;

	//text object
	sf::Text m_placeModeTxt;
	std::string m_placeString;
	std::string m_deleteString;

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
	bool m_middleBtn = false;
	bool m_showTooltips = false;
	bool m_tabPressed = false;
	bool m_numOnePressed = false;
	bool m_showCost = false;
	bool m_rPressed = false;
	bool m_updateRequired = false;
	bool m_numThreePressed = false;
	bool m_showVecFields = false;
	bool m_numFourPressed = false;
	bool m_numTwoPressed = false;
	bool m_showHeatmap = true;
	bool m_spacePressed = false;
	bool m_deleteMode = false;

	//ints
	int m_goalIndex = -1;
	int m_startIndex;
	int m_highestCost = -1;
};

