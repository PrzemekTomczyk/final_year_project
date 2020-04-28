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

class TileComparer
{
public:
	bool operator()(const GridTile* t1, const GridTile* t2)
	{
		return t1->getTotalDist() > t2->getTotalDist();
	}
};

class NodeComparer
{
public:
	bool operator()(const SearchNode* t1, const SearchNode* t2)
	{
		return t1->getFval() > t2->getFval();
	}
};

class BoundaryNode;
class GridManager
{
public:
	GridManager(sf::Font& t_font, sf::RenderWindow& t_window, int t_maxTiles, int t_noOfRows, int t_tilesPerRow);
	~GridManager();
	void update(const sf::Vector2f& t_textPos);
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
	void addLine(sf::Vector2f t_p1, sf::Vector2f t_p2, sf::Color t_colour);
	float backTrack();
	void backTrackFrom(int& t_point);
	void setupRectCorners(std::vector<int>& t_rectCorners);

	void reaGridRedraw();
	void gridRedraw();
	void setToPath(int t_index);
	bool reaAlgorithm();
	bool aStar();
	void prePathfindReset();
	void updateText(const sf::Vector2f& t_textPos);

	bool successor(BoundaryNode& t_parentBoundary);
	void setupFsiPoint(int& t_point, SearchNode& t_fsi);
	void calcENI(BoundaryNode& t_parentBoundary);
	void calcFSI(BoundaryNode& t_parentBoundary);
	double getOctileDist(sf::Vector2f t_p1, sf::Vector2f t_p2);
	double getOctileDist(sf::Vector2i t_p1, sf::Vector2i t_p2);

	//functions for finding rectangle boundaries
	bool getRectInDirection(std::vector<int>& t_rectBoundary, NeighbourIndex& t_direction, int& t_origin, bool t_expandOpposite, int t_sideLimit1, int t_sideLimit2);
	void getRectInOpposite(std::vector<int>& t_rectBoundary, NeighbourIndex& t_direction, int& t_origin, int& t_limit1, int& t_limit2, bool& t_goalFound);
	int getSideBoundary(NeighbourIndex& t_direction, int& t_expandOrigin, int& t_currentLimit, bool& t_goalFound, int& t_rectOrigin);
	void markBorderers(std::vector<int>& t_rectBorder);
	bool tryToUpdateFsiPoint(int& t_point, NeighbourIndex& t_dir);
	bool tryToUpdateSpecialCaseFsiPoint(int& t_point, NeighbourIndex& t_dir, bool t_startPoint);
	bool tryToUpdateDiagonalFsiPoint(int& t_point, NeighbourIndex& t_dir, bool t_startPoint);
	void tryToUpdateSideBoundaryPoint(int& t_point, int& t_cardinalPoint, int& t_diagonalPoint, double& t_cardinalLen, double& t_diagLen, SearchNode* t_cbn, NeighbourIndex& t_boundaryDir);
	void tryToUpdateOppositeBoundaryPoint(int& t_point, int& t_p1, int& t_p2, double& t_octileP1, double& t_octileP2);
	bool processBoundaries(SearchNode* t_cbn, BoundaryNode& t_wall1, BoundaryNode& t_wall2, BoundaryNode& t_wall3);
	bool fsiSpecialCasePoint(int& t_point, BoundaryNode& t_boundary);
	bool checkIfWithinRect(int& t_point, int& t_topLeft, int& t_botRight);

	//new funcs
	bool expand(SearchNode* t_cbn);
	bool insertS();
	void getRectBoundaries(std::vector<int>& t_rectBoundary, std::vector<BoundaryNode>& t_boundaries);
	void calcBoundary(int& t_corner1, int& t_corner2, NeighbourIndex& t_dir, std::vector<int>& t_boundaryContainer);
	bool isValidBoundary(int& t_boundary, NeighbourIndex& t_directionOfBoundary);
	bool getStartRect(std::vector<BoundaryNode>& t_boundaries, NeighbourIndex& t_dir, int& t_origin);
	bool getRect(std::vector<BoundaryNode>& t_boundaries, NeighbourIndex& t_dir, int& t_origin, std::vector<int>& t_fsiInterval);
	int getCurrentSideLimit(NeighbourIndex& t_expandDir, int& t_origin, std::vector<int>& t_interval, NeighbourIndex& t_sideDirection);

	//rea* open list
	std::priority_queue<SearchNode*, std::vector<SearchNode*>, NodeComparer> m_openlist;
	std::vector<SearchNode*> m_searchNodes;
	bool m_useRea = true;
	bool m_debug = false;
	std::vector<int> m_tempRect;
	std::vector<BoundaryNode> m_tempBoundaries;

	//vectors
	std::vector<GridTile*> m_grid;
	sf::Vector2f m_tileSize;

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