#include "GridManager.h"
#include <iostream>
#include <cmath>
#include <algorithm>

GridManager::GridManager(sf::Font& t_font, sf::RenderWindow& t_window) :
	m_font(t_font),
	m_window(t_window)
{
	m_placeString = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPLACE MODE";
	m_deleteString = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nDELETE MODE";
}

GridManager::~GridManager()
{
}

void GridManager::render()
{
	m_window.draw(m_placeModeTxt);
	for (int i = 0; i < m_grid.size(); i++)
	{
		m_grid[i].render(m_window, m_showCost, m_showHeatmap);
	}
	if (m_showVecFields)
	{
		for (int i = 0; i < m_grid.size(); i++)
		{
			if (i != m_goalIndex && m_grid[i].getType() != GridTile::TileType::Obstacle)
			{
				m_window.draw(m_grid[i].getVectorLine());
			}
		}
	}
}

void GridManager::handleInput()
{
	if (m_window.hasFocus())
	{
		handleKeyboard();
		handleMouse();
	}
}

void GridManager::handleKeyboard()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !m_spacePressed)
	{
		m_spacePressed = true;
		m_deleteMode = !m_deleteMode;
		if (m_deleteMode)
		{
			float newOffset = m_placeModeTxt.getGlobalBounds().width;
			m_placeModeTxt.setFillColor(sf::Color::Red);
			m_placeModeTxt.setString(m_deleteString);

			newOffset = newOffset - m_placeModeTxt.getGlobalBounds().width;
			newOffset /= 2;
			m_placeModeTxt.setPosition(m_placeModeTxt.getPosition().x + newOffset, m_placeModeTxt.getPosition().y);
		}
		else
		{
			float newOffset = m_placeModeTxt.getGlobalBounds().width;
			m_placeModeTxt.setFillColor(sf::Color::Green);
			m_placeModeTxt.setString(m_placeString);

			newOffset = newOffset - m_placeModeTxt.getGlobalBounds().width;
			newOffset /= 2;
			m_placeModeTxt.setPosition(m_placeModeTxt.getPosition().x + newOffset, m_placeModeTxt.getPosition().y);
		}
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_spacePressed = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && !m_numOnePressed)
	{
		m_showCost = !m_showCost;
		m_numOnePressed = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		m_numOnePressed = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !m_rPressed)
	{
		resetGrid();
		m_rPressed = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		m_rPressed = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && !m_numTwoPressed)
	{
		m_numTwoPressed = true;
		if (m_goalIndex != -1)
		{
			m_grid[m_goalIndex].reset();
			resetNonObstacles();
			m_goalIndex = -1;
		}
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		m_numTwoPressed = false;
	}
}

void GridManager::handleMouse()
{
	//handle lmb
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !m_leftBtn)
	{
		m_leftBtn = true;
		handleLeftClick(sf::Mouse::getPosition(m_window));
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		m_leftBtn = false;
	}

	//handle rmb
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && !m_rightBtn)
	{
		m_rightBtn = true;
		handleRightClick(sf::Mouse::getPosition(m_window));
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		m_rightBtn = false;
		if (m_rightBtn && m_gridUpdateRequired)
		{
			m_gridUpdateRequired = false;
			resetNonObstacles();
			if (m_goalIndex >= 0)
			{
				//doBrushfireCalc(m_goalIndex);
			}
		}
	}

	//handle mmb
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
	{
		m_middleBtn = true;
		handleMiddleClick(sf::Mouse::getPosition(m_window));
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle) && m_gridUpdateRequired)
	{
		if (m_middleBtn && m_gridUpdateRequired)
		{
			m_gridUpdateRequired = false;
			resetNonObstacles();
			if (m_goalIndex >= 0)
			{

			}
		}
	}
}

void GridManager::handleLeftClick(sf::Vector2i t_mousePos)
{
	int tileIndex = getTileIndex(t_mousePos);
	if (m_grid[tileIndex].getType() == GridTile::TileType::None)
	{
		checkIfStartRemoved(tileIndex);
		m_grid[tileIndex].reset();

		if (m_goalIndex > -1)
		{
			m_grid[m_goalIndex].reset();
		}

		m_grid[tileIndex].setToGoal();
		m_goalIndex = tileIndex;

		resetNonObstacles();
		//doBrushfireCalc(m_goalIndex); 
	}
}

void GridManager::handleRightClick(sf::Vector2i t_mousePos)
{
	int tileIndex = getTileIndex(t_mousePos);

	if (m_grid[tileIndex].getType() == GridTile::TileType::None)
	{
		m_grid[m_startIndex].reset();
		m_grid[tileIndex].setToStart(m_grid[tileIndex].getCost());
		m_startIndex = tileIndex;
	}
}

void GridManager::handleMiddleClick(sf::Vector2i t_mousePos)
{
	int tileIndex = getTileIndex(t_mousePos);

	//change 0 to the cost of the start node after calculations
	if (m_deleteMode && (m_grid[tileIndex].getType() == GridTile::TileType::Obstacle || m_grid[tileIndex].getType() == GridTile::TileType::Start))
	{
		checkIfStartRemoved(tileIndex);

		m_grid[tileIndex].reset();
		m_gridUpdateRequired = true;
	}
	else if (!m_deleteMode && (m_grid[tileIndex].getType() == GridTile::TileType::None || m_grid[tileIndex].getType() == GridTile::TileType::Unreachable || m_grid[tileIndex].getType() == GridTile::TileType::Path))
	{
		m_grid[tileIndex].setToObstacle();

		//if goal is set
		if (m_goalIndex >= 0 && m_grid[tileIndex].getType() != GridTile::TileType::Unreachable)
		{
			m_gridUpdateRequired = true;
		}
		else
		{
			m_gridUpdateRequired = false;
		}
	}
}

void GridManager::resetGrid()
{
	m_goalIndex = -1;
	m_startIndex = -1;

	for (int i = 0; i < m_grid.size(); i++)
	{
		m_grid[i].reset();
	}
}

void GridManager::resetNonObstacles()
{
	for (int i = 0; i < m_grid.size(); i++)
	{
		if (m_grid[i].getType() != GridTile::TileType::Obstacle && m_grid[i].getType() != GridTile::TileType::Start && m_grid[i].getType() != GridTile::TileType::Goal)
		{
			m_grid[i].reset();
		}
	}
}

/// <summary>
/// recursive function that goes from start to goal, or until it meets an already found path, and changes tiles to be of type Path
/// </summary>
/// <param name="t_startIndex"></param>
void GridManager::findAndSetPath(int t_startIndex)
{
	bool pathFinished = false;
	int indexWithLowestCost = -1;
	float smallestCost = INT_MAX;
	float neighbourCost;

	for (int neighboursLoopIndex = 0; neighboursLoopIndex < 8; neighboursLoopIndex++)
	{
		int neighbourIndex = getNeighbourIndex(neighboursLoopIndex, t_startIndex);

		if (neighbourIndex >= m_grid.size() || neighbourIndex < 0 || m_grid[neighbourIndex].getCost() == -1 || m_grid[neighbourIndex].getType() == GridTile::TileType::Obstacle)
		{
			continue;
		}
		//make sure its an actual neighbour and not a "neighbour" that wrapped to the other side of the screen
		else if (thor::length(m_grid[t_startIndex].getPos() - m_grid[neighbourIndex].getPos()) > (m_grid[0].getDiagonal() * 1.1f)) // mult by 1.1f to account for floating point numbers
		{
			continue;
		}

		neighbourCost = m_grid[neighbourIndex].getCost() * 300 + m_grid[neighbourIndex].getHeuristic();
		if (neighbourCost < smallestCost)
		{
			smallestCost = neighbourCost;
			indexWithLowestCost = neighbourIndex;

			//check if that neighbour tile is already a path. This means we have hit an already set path - no further calc needed
			if (m_grid[indexWithLowestCost].getType() == GridTile::TileType::Goal)
			{
				pathFinished = true;
				break;
			}
		}
	}
	//if the neighbour we consider the closest is already a path, we found an already established path to the goal meaning we can stop
	if (indexWithLowestCost > -1)
	{
		if (m_grid[indexWithLowestCost].getType() == GridTile::TileType::Path)
		{
			pathFinished = true;
		}
	}


	//if we didnt come across already marked out path, keep marking tiles
	if (!pathFinished && indexWithLowestCost > -1)
	{
		if (m_grid[indexWithLowestCost].getType() == GridTile::TileType::None)
		{
			m_grid[indexWithLowestCost].setToPath();

		}
		findAndSetPath(indexWithLowestCost);
	}
}

/// <summary>
/// Returns an index of a neighbouring tile <para>0 = left, 1 = right, 2 = top left, 3 = above, 4 = top right, 5 = bottom left, 6 = below, 7 = bottom right</para>
/// </summary>
/// <param name="t_whichNeighbour">int to pick a neighbour</param>
/// <param name="t_indexOfTileToGetNeighbours">index of the tile to get neighbour of</param>
/// <returns>index of neighbouring tile</returns>
int GridManager::getNeighbourIndex(int t_whichNeighbour, int t_indexOfTileToGetNeighbours)
{
	switch (t_whichNeighbour)
	{
	case 0: //index for tile to the left
		return t_indexOfTileToGetNeighbours + LEFT_TILE;
		break;
	case 1: //index for tile to the right
		return t_indexOfTileToGetNeighbours + RIGHT_TILE;
		break;
	case 2: //index for tile to the top left
		return t_indexOfTileToGetNeighbours + LEFT_TOP_TILE;
		break;
	case 3: //index for tile above
		return t_indexOfTileToGetNeighbours + TOP_TILE;
		break;
	case 4: //index for tile to the top right
		return t_indexOfTileToGetNeighbours + RIGHT_TOP_TILE;
		break;
	case 5: //index for tile to the bottom left
		return t_indexOfTileToGetNeighbours + LEFT_BOTTOM_TILE;
		break;
	case 6: //index for tile below
		return t_indexOfTileToGetNeighbours + BOTTOM_TILE;
		break;
	case 7: //index for tile to the bottom right
		return t_indexOfTileToGetNeighbours + RIGHT_BOTTOM_TILE;
		break;
	default:
		return t_indexOfTileToGetNeighbours;
		break;
	}
}

void GridManager::checkIfStartRemoved(int t_tileClicked)
{
	if (m_grid[t_tileClicked].getType() == GridTile::TileType::Start)
	{
		m_startIndex = -1;
	}
}

int GridManager::getTileIndex(sf::Vector2i t_mousePos)
{
	int row = t_mousePos.x / m_tileSize.x;
	int col = t_mousePos.y / m_tileSize.y;

	if (col < 0)
	{
		col = 0;
	}
	else if (col > NO_OF_COLS - 1)
	{
		col = NO_OF_COLS - 1;
	}
	if (row < 0)
	{
		row = 0;
	}
	else if (row > TILES_PER_ROW - 1)
	{
		row = TILES_PER_ROW - 1;
	}
	return  row + (col * TILES_PER_ROW);
}

void GridManager::setTestLayout()
{
	m_grid[26].setToObstacle();
	m_grid[36].setToObstacle();
	m_grid[46].setToObstacle();
	m_grid[56].setToObstacle();
	m_grid[55].setToObstacle();
	m_grid[54].setToObstacle();
	m_grid[53].setToObstacle();
	m_grid[52].setToObstacle();
	m_grid[51].setToObstacle();
	m_grid[50].setToObstacle();

	m_grid[131].setToObstacle();
	m_grid[121].setToObstacle();
	m_grid[111].setToObstacle();
	m_grid[102].setToObstacle();
	m_grid[103].setToObstacle();
	m_grid[104].setToObstacle();
	m_grid[105].setToObstacle();
	m_grid[106].setToObstacle();
}

std::string GridManager::reaAlgorithm()
{
	/* Pseudo code
	Initialize();
	if InsertS() then
	{
		return "path is found";
	}
	while(Openlist != Empty Set)
	{
		CBN := the current best search node;

		if Expand(CBN) then
		{
			return "path is found";
		}
	}
	return "no path is found";
	*/

	//Initialize();
	resetNonObstacles();

	if (insertS() != "")
	{
		return "path is found";
	}
	while (!m_openList.empty())
	{
		//
	}
	return std::string();

	return std::string();
}

std::string GridManager::insertS()
{
	/*
	RECT: = original rectangle expanded by S;
	if G ∈ RECT then
		return "path is found";
	for all point p ∈ boundaries of RECT do
		p.gval : = octile(p, S);
		p.mode : = gpoint;
	for all valid boundaries of RECT do
		if Successor(boundary) then
			return "path is found";
	return NULL;
	*/

	std::vector<int> rect; //original rectangle expanded by Start node
	if ((std::find(rect.begin(), rect.end(), m_goalIndex) != rect.end()))
	{
		return "path is found";
	}
	//for (const auto& )


	return std::string();
}

std::string GridManager::successor()
{
	return std::string();
}

std::string GridManager::expand(int t_cbn)
{
	return std::string();
}

void GridManager::update()
{
	handleInput();
}

void GridManager::init(float t_textOffset)
{
	//use height of the window to make squares as the right side of the screen is used for tooltip info
	m_tileSize.y = m_window.getSize().y / 14.0f;
	m_tileSize.x = m_tileSize.y;
	m_grid.reserve(MAX_TILES);

	for (int i = 0; i < NO_OF_COLS; i++)
	{
		for (int j = 0; j < TILES_PER_ROW; j++)
		{
			GridTile tile(sf::Vector2f(j * m_tileSize.x + (m_tileSize.x / 2.0), i * m_tileSize.y + (m_tileSize.y / 2.0)), m_font, m_highestCost, m_tileSize);
			m_grid.push_back(tile);
		}
	}

	m_placeModeTxt.setFont(m_font);
	m_placeModeTxt.setString(m_placeString);
	m_placeModeTxt.setCharacterSize((int)(m_window.getSize().y / 31));
	m_placeModeTxt.setPosition(t_textOffset - m_placeModeTxt.getGlobalBounds().width / 2.0f, 0);
	m_placeModeTxt.setFillColor(sf::Color::Green);

	setTestLayout();
}
