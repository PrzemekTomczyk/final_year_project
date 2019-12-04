#include "GridManager.h"
#include <iostream>
#include <cmath>
#include <algorithm>

GridManager::GridManager(sf::Font& t_font, sf::RenderWindow& t_window) :
	m_font(t_font),
	m_window(t_window)
{
}

GridManager::~GridManager()
{
}

void GridManager::render()
{
	for (int i = 0; i < m_grid.size(); i++)
	{
		m_grid[i].render(m_window);
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !m_rPressed)
	{
		resetGrid();
		m_rPressed = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		m_rPressed = false;
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
}

void GridManager::handleLeftClick(sf::Vector2i t_mousePos)
{
	int tileIndex = getTileIndex(t_mousePos);
	if (m_grid[tileIndex].getType() == GridTile::TileType::None)
	{
		checkIfStartRemoved(tileIndex);
		m_grid[tileIndex].reset();
		m_gridUpdateRequired = true;

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
		m_gridUpdateRequired = true;
		if (m_startIndex > -1)
		{
			m_grid[m_startIndex].reset();
		}
		m_grid[tileIndex].setToStart();
		m_startIndex = tileIndex;
	}
}

void GridManager::resetGrid()
{
	m_goalIndex = -1;
	m_startIndex = -1;

	for (int i = 0; i < m_grid.size(); i++)
	{
		if (m_grid[i].getType() != GridTile::TileType::Obstacle)
		{
			m_grid[i].reset();
		}
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

int GridManager::getStartIndex()
{
	return m_startIndex;
}

int GridManager::getGoalIndex()
{
	return m_goalIndex;
}

void GridManager::updateNotRequired()
{
	m_gridUpdateRequired = false;
}

void GridManager::setVisited(int t_index)
{
	m_grid[t_index].setToUnreachable();
}

void GridManager::setToPath(int t_index)
{
	if (m_grid[t_index].getType() != GridTile::TileType::Goal && m_grid[t_index].getType() != GridTile::TileType::Start)
	{
		m_grid[t_index].setToPath();
	}
}

void GridManager::resetPath()
{
	resetNonObstacles();
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

bool GridManager::update()
{
	handleInput();

	if (m_startIndex >= 0 && m_goalIndex >= 0 && m_gridUpdateRequired)
	{
		return true;
	}
	return false;
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
			GridTile tile(sf::Vector2f(j * m_tileSize.x + (m_tileSize.x / 2.0), i * m_tileSize.y + (m_tileSize.y / 2.0)), m_tileSize);
			m_grid.push_back(tile);
		}
	}

	setTestLayout();
}
