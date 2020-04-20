#include "stdafx.h"
#include "GridManager.h"

#define _DEBUG(x) std::cout << #x << " = " << x << std::endl;

GridManager::GridManager(sf::Font& t_font, sf::RenderWindow& t_window, int t_maxTiles, int t_noOfRows, int t_tilesPerRow) :
	m_font(t_font),
	m_window(t_window),
	m_grid(MAX_TILES, nullptr),
	MAX_TILES(t_maxTiles),
	NO_OF_ROWS(t_noOfRows),
	TILES_PER_ROW(t_tilesPerRow),
	TOP_TILE(-TILES_PER_ROW),
	LEFT_TOP_TILE(TOP_TILE - 1),
	RIGHT_TOP_TILE(TOP_TILE + 1),
	BOTTOM_TILE(TILES_PER_ROW),
	LEFT_BOTTOM_TILE(BOTTOM_TILE - 1),
	RIGHT_BOTTOM_TILE(BOTTOM_TILE + 1)
{
	m_placeString = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPLACE MODE";
	m_deleteString = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nDELETE MODE";
}

GridManager::~GridManager()
{
	for (auto p : m_grid)
	{
		delete p;
	}
	m_grid.clear();
}

void GridManager::update()
{
	handleInput();

	if (!m_redrawNeeded && m_updateRequired && m_startIndex > -1 && m_goalIndex > -1)
	{
		m_updateRequired = false;

		properRea();
		//reaAlgorithm();
	}
	else if (m_startIndex == -1 || m_goalIndex == -1 && !m_lines.empty())
	{
		m_lines.clear();
	}
}


void GridManager::render()
{
	m_redrawNeeded = false;
	m_window.draw(m_placeModeTxt);
	for (int i = 0; i < m_grid.size(); i++)
	{
		m_grid.at(i)->render(m_window);
	}

	for (int i = 0; i < m_lines.size(); i++)
	{
		m_window.draw(&m_lines[0], m_lines.size(), sf::Lines);
	}
}

void GridManager::reaGridRedraw()
{
	for (int i = 0; i < m_grid.size(); i++)
	{
		//if (m_grid.at(i)->getMarked() || m_grid.at(i)->getVisited())
		{
			m_grid.at(i)->render(m_window);
		}
	}
	m_window.display();
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
		//if (m_rightBtn && m_updateRequired)
		//{
		//	m_updateRequired = false;
		//	resetNonObstacles();
		//}
	}

	//handle mmb
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
	{
		m_middleBtn = true;
		handleMiddleClick(sf::Mouse::getPosition(m_window));
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle) && m_middleBtn)
	{
		if (m_middleBtn && m_changedGrid)
		{
			resetNonObstacles();
			m_changedGrid = false;
			m_updateRequired = true;
		}
		m_middleBtn = false;
	}
}

void GridManager::handleLeftClick(sf::Vector2i t_mousePos)
{
	int tileIndex = getClickedTileIndex(t_mousePos);

	if (tileIndex != m_startIndex && tileIndex != m_goalIndex)
	{
		if (m_grid.at(tileIndex)->getType() != GridTile::TileType::Obstacle)
		{
			checkIfStartRemoved(tileIndex);
			m_grid.at(tileIndex)->reset();
			m_updateRequired = true;

			if (m_goalIndex > -1)
			{
				m_grid.at(m_goalIndex)->reset();
			}

			m_grid.at(tileIndex)->setToGoal();
			m_goalIndex = tileIndex;

			resetNonObstacles();
			m_redrawNeeded = true;
		}
	}
}

void GridManager::handleRightClick(sf::Vector2i t_mousePos)
{
	int tileIndex = getClickedTileIndex(t_mousePos);

	if (tileIndex != m_startIndex && tileIndex != m_goalIndex)
	{
		if (m_grid.at(tileIndex)->getType() != GridTile::TileType::Obstacle)
		{
			m_updateRequired = true;
			if (m_startIndex > -1)
			{
				m_grid.at(m_startIndex)->reset();
			}
			m_grid.at(tileIndex)->setToStart();
			m_startIndex = tileIndex;
			m_redrawNeeded = true;
		}
	}
}

void GridManager::handleMiddleClick(sf::Vector2i t_mousePos)
{
	int tileIndex = getClickedTileIndex(t_mousePos);

	if (m_deleteMode && (m_grid.at(tileIndex)->getType() == GridTile::TileType::Obstacle || m_startIndex == tileIndex || m_goalIndex == tileIndex))
	{
		if (m_startIndex == tileIndex)
		{
			m_startIndex = -1;
		}
		else if (m_goalIndex == tileIndex)
		{
			m_goalIndex = -1;
		}

		m_grid.at(tileIndex)->reset();
		m_changedGrid = true;
	}
	else if (!m_deleteMode)
	{
		if (m_startIndex == tileIndex)
		{
			m_startIndex = -1;
		}
		else if (m_goalIndex == tileIndex)
		{
			m_goalIndex = -1;
		}
		m_grid.at(tileIndex)->setToObstacle();
		m_changedGrid = true;
	}
}

void GridManager::resetGrid()
{
	m_goalIndex = -1;
	m_startIndex = -1;

	for (int i = 0; i < m_grid.size(); i++)
	{
		m_grid.at(i)->reset();
	}
	if (MAX_TILES == TEST_LAYOUT)
	{
		setTestLayout();
	}
}

void GridManager::resetNonObstacles()
{
	for (int i = 0; i < m_grid.size(); i++)
	{
		if (m_grid.at(i)->getType() != GridTile::TileType::Obstacle && m_grid.at(i)->getType() != GridTile::TileType::Start && m_grid.at(i)->getType() != GridTile::TileType::Goal)
		{
			m_grid.at(i)->reset();
		}
		else if (m_grid.at(i)->getType() == GridTile::TileType::Start)
		{
			m_grid.at(i)->reset();
			m_grid.at(i)->setToStart();
		}
		else if (m_grid.at(i)->getType() == GridTile::TileType::Goal)
		{
			m_grid.at(i)->reset();
			m_grid.at(i)->setToGoal();
		}
	}
}

/// <summary>
/// Returns an index of a neighbouring tile <para>0 = left, 1 = right, 2 = top left, 3 = above, 4 = top right, 5 = bottom left, 6 = below, 7 = bottom right</para>
/// </summary>
/// <param name="t_neighbour">int to pick a neighbour</param>
/// <param name="t_index">index of the tile to get neighbour of</param>
/// <returns>index of neighbouring tile</returns>
int GridManager::getNeighbourIndex(NeighbourIndex t_neighbour, int t_index)
{
	int neighbourIndex;
	switch (t_neighbour)
	{
	case NeighbourIndex::LEFT:
		neighbourIndex = t_index + LEFT_TILE;
		break;
	case NeighbourIndex::RIGHT:
		neighbourIndex = t_index + RIGHT_TILE;
		break;
	case NeighbourIndex::TOP_LEFT:
		neighbourIndex = t_index + LEFT_TOP_TILE;
		break;
	case NeighbourIndex::TOP:
		neighbourIndex = t_index + TOP_TILE;
		break;
	case NeighbourIndex::TOP_RIGHT:
		neighbourIndex = t_index + RIGHT_TOP_TILE;
		break;
	case NeighbourIndex::BOTTOM_LEFT:
		neighbourIndex = t_index + LEFT_BOTTOM_TILE;
		break;
	case NeighbourIndex::BOTTOM:
		neighbourIndex = t_index + BOTTOM_TILE;
		break;
	case NeighbourIndex::BOTTOM_RIGHT:
		neighbourIndex = t_index + RIGHT_BOTTOM_TILE;
		break;
	default:
		//invalid neighbour
		return -1;
		break;
	}

	if (neighbourIndex < 0 || neighbourIndex >= MAX_TILES)
	{
		return -1;
	}
	else if (thor::length(m_grid.at(t_index)->getPos() - m_grid.at(neighbourIndex)->getPos()) > m_grid.at(t_index)->getDiagonal())
	{
		return -1;
	}
	else
	{
		return neighbourIndex;
	}
}

bool GridManager::cardinalDirectionsAvailable(int t_index)
{
	for (int i = 0; i < 4; i++)
	{
		int neighbourIndex = getNeighbourIndex((NeighbourIndex)i, t_index);
		if (neighbourIndex >= 0 && m_grid.at(neighbourIndex)->getType() == GridTile::TileType::Obstacle)
		{
			return false;
		}
	}
	return true;
}

void GridManager::checkIfStartRemoved(int t_tileClicked)
{
	if (m_grid.at(t_tileClicked)->getType() == GridTile::TileType::Start)
	{
		m_startIndex = -1;
	}
}

int GridManager::getClickedTileIndex(sf::Vector2i t_mousePos)
{
	int col = t_mousePos.x / m_tileSize.x;
	int row = t_mousePos.y / m_tileSize.y;

	if (row < 0)
	{
		row = 0;
	}
	else if (row > NO_OF_ROWS - 1)
	{
		row = NO_OF_ROWS - 1;
	}
	if (col < 0)
	{
		col = 0;
	}
	else if (col > TILES_PER_ROW - 1)
	{
		col = TILES_PER_ROW - 1;
	}
	return  col + (row * TILES_PER_ROW);
}

int GridManager::getStartIndex()
{
	return m_startIndex;
}

int GridManager::getGoalIndex()
{
	return m_goalIndex;
}

void GridManager::setToPath(int t_index)
{
	if (m_grid.at(t_index)->getType() != GridTile::TileType::Goal && m_grid.at(t_index)->getType() != GridTile::TileType::Start)
	{
		m_grid.at(t_index)->setToPath();
	}
}

void GridManager::setTestLayout()
{
	m_grid.at(26)->setToObstacle();
	m_grid.at(36)->setToObstacle();
	m_grid.at(46)->setToObstacle();
	m_grid.at(56)->setToObstacle();
	m_grid.at(55)->setToObstacle();
	m_grid.at(54)->setToObstacle();
	m_grid.at(53)->setToObstacle();
	m_grid.at(52)->setToObstacle();
	m_grid.at(51)->setToObstacle();
	m_grid.at(50)->setToObstacle();

	m_grid.at(131)->setToObstacle();
	m_grid.at(121)->setToObstacle();
	m_grid.at(111)->setToObstacle();
	m_grid.at(102)->setToObstacle();
	m_grid.at(103)->setToObstacle();
	m_grid.at(104)->setToObstacle();
	m_grid.at(105)->setToObstacle();
	m_grid.at(106)->setToObstacle();
}

/// <summary>
/// Adds a vertex to a vector
/// </summary>
/// <param name="t_p1">position of vertex 1</param>
/// <param name="t_p2">position of vertex 2</param>
void GridManager::addLine(sf::Vector2f t_p1, sf::Vector2f t_p2)
{
	sf::Vertex vertex1;
	sf::Vertex vertex2;

	vertex1 = sf::Vertex(t_p1);
	vertex2 = sf::Vertex(t_p2);

	m_lines.push_back(vertex1);
	m_lines.push_back(vertex2);
}

void GridManager::backTrack()
{
	if (m_grid.at(m_goalIndex)->getPrevious() != nullptr) {
		GridTile* ptr = m_grid.at(m_goalIndex);

		//add all nodes with previous to the path
		while (nullptr != ptr->getPrevious())
		{
			if (ptr != m_grid.at(m_goalIndex) && ptr != m_grid.at(m_startIndex))
			{
				ptr->setToPath();
			}

			addLine(ptr->getPos(), ptr->getPrevious()->getPos());

			ptr = ptr->getPrevious();
		}
	}
	else
	{
		reaGridRedraw();
		//throw std::invalid_argument("goal's previous ptr not set!");
		std::cout << "goal's previous ptr not set!" << std::endl;
	}
}

void GridManager::init(float t_textOffset)
{
	//use height of the window to make squares as the right side of the screen is used for tooltip info
	m_tileSize.y = (m_window.getSize().y) / NO_OF_ROWS;
	m_tileSize.x = m_tileSize.y;
	m_grid.reserve(MAX_TILES);

	for (int i = 0; i < NO_OF_ROWS; i++)
	{
		for (int j = 0; j < TILES_PER_ROW; j++)
		{
			int tileIndex = j + i * TILES_PER_ROW;
			m_grid.push_back(
				new GridTile(
					sf::Vector2f(j * m_tileSize.x + (m_tileSize.x / 2.0),
						i * m_tileSize.y + (m_tileSize.y / 2.0)
					),
					m_tileSize,
					tileIndex,
					m_font
				)
			);
		}
	}
	m_placeModeTxt.setFont(m_font);
	m_placeModeTxt.setString(m_placeString);
	m_placeModeTxt.setCharacterSize(static_cast<int>(m_window.getSize().y / 31));
	m_placeModeTxt.setPosition(t_textOffset - m_placeModeTxt.getGlobalBounds().width / 2.0f, 0);
	m_placeModeTxt.setFillColor(sf::Color::Green);

	if (MAX_TILES == TEST_LAYOUT)
	{
		setTestLayout();
	}
}

void GridManager::reaAlgorithm()
{
	std::cout << "#### Finding path ####" << std::endl;

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
	//Initialise tiles
	m_lines.clear();
	resetNonObstacles();

	std::priority_queue<GridTile*, std::vector<GridTile*>, TileComparer> pq;

	properRea();

	pq.push(m_grid.at(m_startIndex));
	GridTile* current = pq.top();
	pq.pop();

	std::vector<int> corners;
	if (insertS(corners))
	{
		std::cout << "Goal has been found in first rectangle!" << std::endl;
		backTrack();

		return;
	}
	else
	{
		//mark tiles in first rectangle
		calculateRectangleNeighbours(corners, pq, current);
		//clear out the vector of corners
		corners.clear();
	}
	//reaGridRedraw();

	while (!pq.empty())
	{
		current = pq.top();
		if (current == m_grid.at(m_goalIndex))
		{
			//goal has been found!
			std::cout << "Goal has been found in a rectangle!" << std::endl;
			backTrack();
			return;
		}
		pq.pop();
		if (!current->getVisited())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(250));

			if (expand(current->getIndex(), corners))
			{
				//goal has been found!
				std::cout << "Goal has been found in a rectangle!" << std::endl;
				backTrack();
				return;
			}
			else
			{
				//mark tiles a rectangle
				calculateRectangleNeighbours(corners, pq, current);
				//clear out the vector of corners
				corners.clear();
			}
			//reaGridRedraw();
		}
	}
	//if we get here, goal has not been found
	std::cout << "Goal has *NOT* been found!" << std::endl;
	return;
}

void GridManager::properRea()
{
	//Initialise
	for (int i = m_rectNodes.size() - 1; i >= 0; i--)
	{
		if (m_rectNodes[i] != nullptr)
		{
			delete m_rectNodes[i];
			m_rectNodes[i] = nullptr;
		}
	}
	m_rectNodes.clear();

	m_openlist = std::priority_queue<SearchNode, std::vector<SearchNode>, NodeComparer>();
	m_lines.clear();
	resetNonObstacles();

	if (insertSProper())
	{
		//goal has been found!
		std::cout << "Goal has been found in a rectangle!" << std::endl;
		backTrack();
		reaGridRedraw();
		return;
	}

	//current best node CBN
	while (!m_openlist.empty())
	{
		//if (expandProper(CBN))
		{
			//goal has been found!
			std::cout << "Goal has been found in a rectangle!" << std::endl;
			backTrack();
			return;
		}
	}

	//path not found
	std::cout << "Failed to find a path to the goal!" << std::endl;
}

bool GridManager::insertS(std::vector<int>& t_corners)
{
	bool goalFound = getRectInDirection(t_corners, NeighbourIndex::TOP/*directionToGoal(m_startIndex)*/, m_startIndex);

	std::vector<std::vector<int>> boundaries;

	if (!goalFound)
	{
		//we have 4 boundaries
		for (auto& boundary : boundaries)
		{
			for (auto& tile : boundary)
			{
				if (tile >= 0 && tile < MAX_TILES && m_grid.at(tile)->getType() != GridTile::TileType::Obstacle)
				{
					m_grid.at(tile)->m_gval = getOctileDist(m_grid.at(tile)->getPos(), m_grid.at(m_startIndex)->getPos());
					m_grid.at(tile)->m_mode = GridTile::ReaMode::Gpoint;
				}
			}
		}
	}

	return goalFound;
}

float GridManager::getOctileDist(sf::Vector2f t_p1, sf::Vector2f t_p2)
{
	float D2 = sqrt(2.f);
	float dx = abs(t_p1.x - t_p2.x);
	float dy = abs(t_p1.y - t_p2.y);
	return (dx + dy) + (D2 - 2.f) * std::min(dx, dy);
}

void GridManager::calculateRectangleNeighbours(std::vector<int>& t_corners, std::priority_queue<GridTile*, std::vector<GridTile*>, TileComparer>& t_pq, GridTile* t_current)
{
	//sort corner indexes so smallest index is top left and highest is bottom right
	std::sort(t_corners.begin(), t_corners.end());

	//set the corners for looping
	int topLeft = t_corners[0], topRight = t_corners[1], botLeft = t_corners[2], botRight = t_corners[3];

	if (m_grid.at(topRight)->getColRow().y == m_grid.at(botLeft)->getColRow().y)
	{
		std::swap(topRight, botLeft);
	}

	int index = topLeft;
	while (index <= botRight)
	{
		//mark all tiles of the rectangle as visited
		m_grid.at(index)->setVisited(true);
		index++;

		if (index < MAX_TILES)
		{
			//if index went past the right most column
			if (m_grid.at(index)->getColRow().x > m_grid.at(botRight)->getColRow().x)
			{
				//move to the left
				index -= (m_grid.at(index)->getColRow().x - m_grid.at(topLeft)->getColRow().x);
				//move down a row
				index += TILES_PER_ROW;
			}
			//if index went past the left most column
			else if (m_grid.at(index)->getColRow().x < m_grid.at(topLeft)->getColRow().x)
			{
				//move to the right
				index += (m_grid.at(topLeft)->getColRow().x - m_grid.at(index)->getColRow().x);
			}
		}
	}

	/// <summary>
	/// TO-DO:
	/// 1. For each side of neighbours, check if neighbour has free cardinal tiles
	/// if not, then set the previous ptr for that neigbour to be the tile that added
	/// it
	/// </summary>

	//handle neighbours
	//check if there is a row available above and then handle those neighbours
	//ABOVE
	markNeighbours(NeighbourIndex::TOP, t_current, topLeft, botRight, t_pq);
	//BELOW
	markNeighbours(NeighbourIndex::BOTTOM, t_current, botLeft, botRight, t_pq);
	//LEFT
	markNeighbours(NeighbourIndex::LEFT, t_current, topLeft, botRight, t_pq);
	//RIGHT
	markNeighbours(NeighbourIndex::RIGHT, t_current, topRight, botRight, t_pq);

	//handle top left and right and bot left and right neighbours separately
	//top-left
	markDiagonal(NeighbourIndex::TOP_LEFT, t_current, topLeft, t_pq);
	//top-right
	markDiagonal(NeighbourIndex::TOP_RIGHT, t_current, topRight, t_pq);
	//bot-left
	markDiagonal(NeighbourIndex::BOTTOM_LEFT, t_current, botLeft, t_pq);
	//bot-right
	markDiagonal(NeighbourIndex::BOTTOM_RIGHT, t_current, botRight, t_pq);
}

void GridManager::markNeighbours(NeighbourIndex t_direction, GridTile* t_current, int t_leftCorner, int t_rightCorner, std::priority_queue<GridTile*, std::vector<GridTile*>, TileComparer>& t_pq)
{
	int tempNeighbourIndex = -1;
	int greaterValue = 0, smallerValue = 0;
	//int leftCorner, rightCorner;
	switch (t_direction)
	{
		//left and right directions dont need any special handling
	case NeighbourIndex::TOP_LEFT:
		break;
	case NeighbourIndex::TOP:
		greaterValue = t_leftCorner;
		smallerValue = TILES_PER_ROW;
		break;
	case NeighbourIndex::TOP_RIGHT:
		break;
	case NeighbourIndex::BOTTOM_LEFT:
		break;
	case NeighbourIndex::BOTTOM:
		smallerValue = t_leftCorner;
		greaterValue = MAX_TILES - TILES_PER_ROW;
		break;
	case NeighbourIndex::BOTTOM_RIGHT:
		break;
	default:
		break;
	}

	//check if there is a row available ABOVE or BELOW, depending on the direction, and then handle those neighbours
	if (t_direction == NeighbourIndex::TOP || t_direction == NeighbourIndex::BOTTOM)
	{
		if (greaterValue > smallerValue)
		{
			tempNeighbourIndex = getNeighbourIndex(t_direction, t_leftCorner);
			bool loop = true;

			while (loop)
			{
				if (m_grid.at(tempNeighbourIndex)->getType() != GridTile::TileType::Obstacle && !m_grid.at(tempNeighbourIndex)->getMarked()
					&& !m_grid.at(tempNeighbourIndex)->getVisited() && tempNeighbourIndex != m_startIndex)
				{
					m_grid.at(tempNeighbourIndex)->setEstDist(getOctileDist(m_grid.at(tempNeighbourIndex)->getPos(), m_grid.at(m_goalIndex)->getPos()));

					int parentOfNeighbour;
					if (t_direction == NeighbourIndex::TOP)
					{
						parentOfNeighbour = getNeighbourIndex(NeighbourIndex::BOTTOM, tempNeighbourIndex);
					}
					else
					{
						parentOfNeighbour = getNeighbourIndex(NeighbourIndex::TOP, tempNeighbourIndex);
					}
					GridTile* parentPtr = m_grid.at(parentOfNeighbour);
					if (parentPtr != t_current)
					{
						parentPtr->setPrevious(t_current);
					}

					if (m_grid.at(tempNeighbourIndex) != parentPtr)
					{
						m_grid.at(tempNeighbourIndex)->setPrevious(parentPtr);
					}
					m_grid.at(tempNeighbourIndex)->setMarked(true);
					t_pq.push(m_grid.at(tempNeighbourIndex));
				}
				if (m_grid.at(tempNeighbourIndex)->getColRow().x < m_grid.at(t_rightCorner)->getColRow().x)
				{
					tempNeighbourIndex++;
					if (tempNeighbourIndex >= MAX_TILES)
					{
						loop = false;
					}
				}
				else
				{
					loop = false;
				}
			}
		}
	}
	//check if there is a column available to the LEFT or RIGHT, depending on the direction, and then handle those neighbours
	else if (t_direction == NeighbourIndex::LEFT || t_direction == NeighbourIndex::RIGHT)
	{
		//when doing left and right neighbours, leftCorner is actually upper corner and rightCorner is lower corner
		if (getNeighbourIndex(t_direction, t_leftCorner) >= 0)
		{
			tempNeighbourIndex = getNeighbourIndex(t_direction, t_leftCorner);
			bool loop = true;

			while (loop)
			{
				if (m_grid.at(tempNeighbourIndex)->getType() != GridTile::TileType::Obstacle && !m_grid.at(tempNeighbourIndex)->getMarked()
					&& !m_grid.at(tempNeighbourIndex)->getVisited() && tempNeighbourIndex != m_startIndex)
				{
					m_grid.at(tempNeighbourIndex)->setEstDist(getOctileDist(m_grid.at(tempNeighbourIndex)->getPos(), m_grid.at(m_goalIndex)->getPos()));

					int parentOfNeighbour;
					if (t_direction == NeighbourIndex::LEFT)
					{
						parentOfNeighbour = getNeighbourIndex(NeighbourIndex::RIGHT, tempNeighbourIndex);
					}
					else
					{
						parentOfNeighbour = getNeighbourIndex(NeighbourIndex::LEFT, tempNeighbourIndex);
					}
					GridTile* parentPtr = m_grid.at(parentOfNeighbour);
					if (parentPtr != t_current)
					{
						parentPtr->setPrevious(t_current);
					}

					if (m_grid.at(tempNeighbourIndex) != parentPtr)
					{
						m_grid.at(tempNeighbourIndex)->setPrevious(parentPtr);
					}
					m_grid.at(tempNeighbourIndex)->setMarked(true);
					t_pq.push(m_grid.at(tempNeighbourIndex));
				}
				if (m_grid.at(tempNeighbourIndex)->getColRow().y < m_grid.at(t_rightCorner)->getColRow().y)
				{
					tempNeighbourIndex += TILES_PER_ROW;
					if (tempNeighbourIndex >= MAX_TILES)
					{
						loop = false;
					}
				}
				else
				{
					loop = false;
				}
			}
		}
	}
}

void GridManager::markDiagonal(NeighbourIndex t_direction, GridTile* t_current, int t_corner, std::priority_queue<GridTile*, std::vector<GridTile*>, TileComparer>& t_pq)
{
	int diagonalIndex = getNeighbourIndex(t_direction, t_corner);
	int cardinalDirection1 = -1;
	int cardinalDirection2 = -1;

	switch (t_direction)
	{
	case NeighbourIndex::TOP_LEFT:
		cardinalDirection1 = getNeighbourIndex(NeighbourIndex::LEFT, t_corner);
		cardinalDirection2 = getNeighbourIndex(NeighbourIndex::TOP, t_corner);
		break;
	case NeighbourIndex::TOP_RIGHT:
		cardinalDirection1 = getNeighbourIndex(NeighbourIndex::RIGHT, t_corner);
		cardinalDirection2 = getNeighbourIndex(NeighbourIndex::TOP, t_corner);
		break;
	case NeighbourIndex::BOTTOM_LEFT:
		cardinalDirection1 = getNeighbourIndex(NeighbourIndex::LEFT, t_corner);
		cardinalDirection2 = getNeighbourIndex(NeighbourIndex::BOTTOM, t_corner);
		break;
	case NeighbourIndex::BOTTOM_RIGHT:
		cardinalDirection1 = getNeighbourIndex(NeighbourIndex::RIGHT, t_corner);
		cardinalDirection2 = getNeighbourIndex(NeighbourIndex::BOTTOM, t_corner);
		break;
	default:
		break;
	}

	//check if diagonal is a valid tile
	if (diagonalIndex >= 0 && diagonalIndex != m_startIndex && m_grid.at(diagonalIndex)->getType() != GridTile::TileType::Obstacle
		&& !m_grid.at(diagonalIndex)->getMarked() && !m_grid.at(diagonalIndex)->getVisited())
	{
		if (cardinalDirection1 >= 0 && cardinalDirection2 >= 0 && m_grid.at(cardinalDirection1)->getType() != GridTile::TileType::Obstacle
			&& m_grid.at(cardinalDirection2)->getType() != GridTile::TileType::Obstacle)
		{
			m_grid.at(diagonalIndex)->setEstDist(getOctileDist(m_grid.at(diagonalIndex)->getPos(), m_grid.at(m_goalIndex)->getPos()));
			m_grid.at(diagonalIndex)->setPrevious(t_current);
			m_grid.at(diagonalIndex)->setMarked(true);
			t_pq.push(m_grid.at(diagonalIndex));
		}
	}
}

/// <summary>
/// Calculates direction in which a rectangle should expand in
/// </summary>
/// <param name="t_tileIndex"></param>
/// <returns>Enum that points in the direction of the goal</returns>
NeighbourIndex GridManager::directionToGoal(int t_tileIndex)
{
	NeighbourIndex direction;

	sf::Vector2f vecToGoal = m_grid.at(m_goalIndex)->getPos() - m_grid.at(t_tileIndex)->getPos();
	if (std::abs(vecToGoal.x) >= std::abs(vecToGoal.y))
	{
		if (vecToGoal.x >= 0.f)
		{
			direction = NeighbourIndex::RIGHT;
		}
		else
		{
			direction = NeighbourIndex::LEFT;
		}
	}
	else
	{
		if (vecToGoal.y >= 0.f)
		{
			direction = NeighbourIndex::BOTTOM;
		}
		else
		{
			direction = NeighbourIndex::TOP;
		}
	}

	return direction;
}

bool GridManager::rightAngleToGoal(int t_tileIndex)
{
	sf::Vector2f pos = m_grid.at(t_tileIndex)->getPos() - m_grid.at(m_goalIndex)->getPos();

	float angle = thor::toDegree(std::atan2(pos.y, pos.x));

	if (angle - static_cast<int>(angle) != 0.0 && static_cast<int>(angle) % 90 == 0)
	{
		return true;
	}

	return false;
}

/// <summary>
/// Function to expand a rectangle in a given direction from a origin index
/// </summary>
/// <param name="t_rectBoundary">vector container for 4 corners of the rect IF goal is not found</param>
/// <param name="t_direction">direction to expand the rectangle in</param>
/// <param name="t_origin">origin index to expand from</param>
/// <returns>true if goal has been found in rectangle, false if not</returns>
bool GridManager::getRectInDirection(std::vector<int>& t_rectBoundary, NeighbourIndex t_direction, int t_origin)
{
	int indexInDirection = -1;
	int limitInDirection = -1;
	int sideLimit1 = -1, sideLimit2 = -1;
	bool goalFound = false;

	indexInDirection = getNeighbourIndex(t_direction, t_origin);
	//if there is nothing above the starting point
	if (indexInDirection >= 0 && m_grid.at(indexInDirection)->getType() != GridTile::TileType::Obstacle && !m_grid.at(indexInDirection)->getVisited() && m_grid.at(indexInDirection)->getType() != GridTile::TileType::Goal)
	{
		//expand in the given direction
		int tempIndex = indexInDirection;
		while (tempIndex >= 0 && m_grid.at(tempIndex)->getType() != GridTile::TileType::Obstacle && !goalFound)
		{
			//we're looping again so we have a new top index
			indexInDirection = tempIndex;
			if (m_grid.at(indexInDirection)->getType() != GridTile::TileType::Goal)
			{
				switch (t_direction)
				{
				case NeighbourIndex::LEFT:
				case NeighbourIndex::RIGHT:
					limitInDirection = m_grid.at(tempIndex)->getColRow().x;
					sideLimit1 = getSideBoundary(NeighbourIndex::TOP, tempIndex, sideLimit1, goalFound, t_origin);
					sideLimit2 = getSideBoundary(NeighbourIndex::BOTTOM, tempIndex, sideLimit2, goalFound, t_origin);
					break;
				case NeighbourIndex::TOP:
				case NeighbourIndex::BOTTOM:
					limitInDirection = m_grid.at(tempIndex)->getColRow().y;
					sideLimit1 = getSideBoundary(NeighbourIndex::LEFT, tempIndex, sideLimit1, goalFound, t_origin);
					sideLimit2 = getSideBoundary(NeighbourIndex::RIGHT, tempIndex, sideLimit2, goalFound, t_origin);
					break;
				default:
					throw std::invalid_argument("received bad direction value");
					break;
				}
				tempIndex = getNeighbourIndex(t_direction, tempIndex);
			}
			else
			{
				goalFound = true;

				//since goal has been immediately found, just set the limits to where the goal is
				switch (t_direction)
				{
				case NeighbourIndex::LEFT:
				case NeighbourIndex::RIGHT:
					limitInDirection = m_grid.at(tempIndex)->getColRow().x;
					sideLimit1 = m_grid.at(m_goalIndex)->getColRow().y;
					sideLimit2 = m_grid.at(m_goalIndex)->getColRow().y;
					break;
				case NeighbourIndex::TOP:
				case NeighbourIndex::BOTTOM:
					limitInDirection = m_grid.at(tempIndex)->getColRow().y;
					sideLimit1 = m_grid.at(m_goalIndex)->getColRow().x;
					sideLimit2 = m_grid.at(m_goalIndex)->getColRow().x;
					break;
				default:
					break;
				}
				//break out of the loop
				break;
			}
		}
	}
	else if (indexInDirection >= 0 && m_grid.at(indexInDirection)->getType() == GridTile::TileType::Goal)
	{
		goalFound = true;
		if (!m_grid.at(m_goalIndex)->getPrevious())
		{
			m_grid.at(m_goalIndex)->setPrevious(m_grid.at(t_origin));
		}
	}
	else
	{
		//we cant expand in that direction, just expand sideways to form a rectangle 1 tall/wide
		switch (t_direction)
		{
		case NeighbourIndex::LEFT:
		case NeighbourIndex::RIGHT:
			limitInDirection = m_grid.at(t_origin)->getColRow().x;
			sideLimit1 = getSideBoundary(NeighbourIndex::TOP, t_origin, sideLimit1, goalFound, t_origin);
			sideLimit2 = getSideBoundary(NeighbourIndex::BOTTOM, t_origin, sideLimit2, goalFound, t_origin);
			break;
		case NeighbourIndex::TOP:
		case NeighbourIndex::BOTTOM:
			limitInDirection = m_grid.at(t_origin)->getColRow().y;
			sideLimit1 = getSideBoundary(NeighbourIndex::LEFT, t_origin, sideLimit1, goalFound, t_origin);
			sideLimit2 = getSideBoundary(NeighbourIndex::RIGHT, t_origin, sideLimit2, goalFound, t_origin);
			break;
		default:
			throw std::invalid_argument("received bad direction value");
			break;
		}
	}

	int corner1, corner2;
	switch (t_direction)
	{
	case NeighbourIndex::LEFT:
	case NeighbourIndex::RIGHT:
		corner1 = limitInDirection + (sideLimit1 * TILES_PER_ROW);
		corner2 = limitInDirection + (sideLimit2 * TILES_PER_ROW);
		break;
	case NeighbourIndex::TOP:
	case NeighbourIndex::BOTTOM:
		corner1 = sideLimit1 + (limitInDirection * TILES_PER_ROW);
		corner2 = sideLimit2 + (limitInDirection * TILES_PER_ROW);
		break;
	default:
		throw std::invalid_argument("received bad direction value");
		break;
	}
	t_rectBoundary.push_back(corner1);
	t_rectBoundary.push_back(corner2);

	if (goalFound)
	{
		corner1 = m_goalIndex;
		corner2 = m_goalIndex;

		if (!m_grid.at(m_goalIndex)->getPrevious())
		{
			m_grid.at(m_goalIndex)->setPrevious(m_grid.at(t_origin));
		}
	}
	if (corner1 < 0 && corner2 < 0)
	{
		throw std::invalid_argument("one of the corners is an invalid index!");
	}

	//std::cout << "Limit in direction: " << limitInDirection << std::endl;
	//std::cout << "Side Limit 1: " << sideLimit1 << std::endl;
	//std::cout << "Side Limit 2: " << sideLimit2 << std::endl;

	if (!goalFound)
	{
		NeighbourIndex oppositeDirection;
		int startPointIndex;
		switch (t_direction)
		{
		case NeighbourIndex::LEFT:
			oppositeDirection = NeighbourIndex::RIGHT;
			startPointIndex = getNeighbourIndex(NeighbourIndex::LEFT, t_origin);
			break;
		case NeighbourIndex::RIGHT:
			oppositeDirection = NeighbourIndex::LEFT;
			startPointIndex = getNeighbourIndex(NeighbourIndex::RIGHT, t_origin);
			break;
		case NeighbourIndex::TOP:
			oppositeDirection = NeighbourIndex::BOTTOM;
			startPointIndex = getNeighbourIndex(NeighbourIndex::TOP, t_origin);
			break;
		case NeighbourIndex::BOTTOM:
			oppositeDirection = NeighbourIndex::TOP;
			startPointIndex = getNeighbourIndex(NeighbourIndex::BOTTOM, t_origin);
			break;
		default:
			throw std::invalid_argument("received bad direction value");
			break;
		}
		if (startPointIndex < 0 || m_grid.at(startPointIndex)->getType() == GridTile::TileType::Obstacle || m_grid.at(startPointIndex)->getVisited())
		{
			startPointIndex = t_origin;
		}
		getRectInOpposite(t_rectBoundary, oppositeDirection, startPointIndex, sideLimit1, sideLimit2, goalFound);
	}
	else if (m_grid.at(m_goalIndex)->getPrevious() == nullptr)
	{
		m_grid.at(m_goalIndex)->setPrevious(m_grid.at(t_origin));
	}
	//if (goalFound)
	//{
	//	std::cout << "Goal found!" << std::endl;
	//}
	if (!goalFound)
	{
		markBorderers(t_rectBoundary);
	}
	return goalFound;
}

void GridManager::getRectInOpposite(std::vector<int>& t_rectBoundary, NeighbourIndex t_direction, int t_origin, int t_limit1, int t_limit2, bool& t_goalFound)
{
	int indexInDirection = t_origin;
	int limitInDirection = -1;
	int sideLimit1 = t_limit1, sideLimit2 = t_limit2;
	int oppositeLimit1 = -1, oppositeLimit2 = -1;
	bool breakout = false;

	//indexInDirection = getNeighbourIndex(t_direction, t_origin);
	//if there is nothing above the starting point
	if (indexInDirection >= 0 && m_grid.at(indexInDirection)->getType() != GridTile::TileType::Obstacle && !m_grid.at(indexInDirection)->getVisited())
	{
		//expand in the given direction
		int tempIndex = indexInDirection;
		while (tempIndex >= 0 && m_grid.at(tempIndex)->getType() != GridTile::TileType::Obstacle && !breakout && !t_goalFound)
		{
			//if tile at tempIndex is a Goal tile, breakout
			if (m_grid.at(tempIndex)->getType() == GridTile::TileType::Goal)
			{
				t_goalFound = true;
				breakout = true;
				break;
			}
			else
			{
				//we're looping again so we have a new top index
				indexInDirection = tempIndex;
				int tempLimit1 = -1, tempLimit2 = -1;
				switch (t_direction)
				{
				case NeighbourIndex::LEFT:
				case NeighbourIndex::RIGHT:
				{
					tempLimit1 = getSideBoundary(NeighbourIndex::TOP, tempIndex, sideLimit1, t_goalFound, t_origin);
					tempLimit2 = getSideBoundary(NeighbourIndex::BOTTOM, tempIndex, sideLimit2, t_goalFound, t_origin);

					if (tempLimit1 > sideLimit1)
					{
						breakout = true;
					}
					if (tempLimit2 < sideLimit2)
					{
						breakout = true;
					}
					if (!breakout)
					{
						limitInDirection = m_grid.at(tempIndex)->getColRow().x;
					}
					break;
				}
				case NeighbourIndex::TOP:
				case NeighbourIndex::BOTTOM:
				{
					tempLimit1 = getSideBoundary(NeighbourIndex::LEFT, tempIndex, sideLimit1, t_goalFound, t_origin);
					tempLimit2 = getSideBoundary(NeighbourIndex::RIGHT, tempIndex, sideLimit2, t_goalFound, t_origin);

					if (tempLimit1 > sideLimit1)
					{
						breakout = true;
					}
					if (tempLimit2 < sideLimit2)
					{
						breakout = true;
					}
					if (!breakout)
					{
						limitInDirection = m_grid.at(tempIndex)->getColRow().y;
					}
					break;
				}
				default:
					throw std::invalid_argument("received bad direction value");
					break;
				}

				if (!breakout)
				{
					tempIndex = getNeighbourIndex(t_direction, tempIndex);
					if (tempLimit1 > -1 && tempLimit2 > -1)
					{
						oppositeLimit1 = tempLimit1;
						oppositeLimit2 = tempLimit2;
					}
				}
			}
		}
	}
	//else
	//{
	//	//we cant expand in the main direction, just expand sideways
	//	int tempLimit1 = -1, tempLimit2 = -1;
	//	switch (t_direction)
	//	{
	//	case NeighbourIndex::LEFT:
	//	case NeighbourIndex::RIGHT:
	//		limitInDirection = m_grid.at(t_origin)->getColRow().x;
	//		
	//		tempLimit1 = getSideBoundary(NeighbourIndex::TOP, t_origin, sideLimit1, t_goalFound);
	//		tempLimit2 = getSideBoundary(NeighbourIndex::BOTTOM, t_origin, sideLimit2, t_goalFound);

	//		if (tempLimit1 > sideLimit1)
	//		{
	//			breakout = true;
	//		}
	//		if (tempLimit2 < sideLimit2)
	//		{
	//			breakout = true;
	//		}

	//		sideLimit1 = getSideBoundary(NeighbourIndex::TOP, t_origin, sideLimit1, t_goalFound);
	//		sideLimit2 = getSideBoundary(NeighbourIndex::BOTTOM, t_origin, sideLimit2, t_goalFound);
	//		break;
	//	case NeighbourIndex::TOP:
	//	case NeighbourIndex::BOTTOM:
	//		limitInDirection = m_grid.at(t_origin)->getColRow().y;
	//		sideLimit1 = getSideBoundary(NeighbourIndex::LEFT, t_origin, sideLimit1, t_goalFound);
	//		sideLimit2 = getSideBoundary(NeighbourIndex::RIGHT, t_origin, sideLimit2, t_goalFound);
	//		break;
	//	default:
	//		throw std::invalid_argument("received bad direction value");
	//		break;
	//	}
	//}

	int corner1, corner2, tempSideLimit1, tempSideLimit2, tempDirectionLimit;
	if (oppositeLimit1 >= 0 && oppositeLimit2 >= 0)
	{
		tempSideLimit1 = oppositeLimit1;
		tempSideLimit2 = oppositeLimit2;
	}
	else
	{
		tempSideLimit1 = sideLimit1;
		tempSideLimit2 = sideLimit2;
	}
	switch (t_direction)
	{
	case NeighbourIndex::LEFT:
	case NeighbourIndex::RIGHT:
		corner1 = limitInDirection + (tempSideLimit1 * TILES_PER_ROW);
		corner2 = limitInDirection + (tempSideLimit2 * TILES_PER_ROW);
		break;
	case NeighbourIndex::TOP:
	case NeighbourIndex::BOTTOM:
		corner1 = tempSideLimit1 + (limitInDirection * TILES_PER_ROW);
		corner2 = tempSideLimit2 + (limitInDirection * TILES_PER_ROW);
		break;
	default:
		throw std::invalid_argument("received bad direction value");
		break;
	}
	t_rectBoundary.push_back(corner1);
	t_rectBoundary.push_back(corner2);

	if (corner1 < 0 && corner2 < 0)
	{
		throw std::invalid_argument("one of the corners is an invalid index!");
	}

	if (t_goalFound && m_grid.at(m_goalIndex)->getPrevious() == nullptr)
	{
		m_grid.at(m_goalIndex)->setPrevious(m_grid.at(t_origin));
	}

	//std::cout << "Limit in opposite direction: " << limitInDirection << std::endl;
	//std::cout << "Side Limit 1: " << tempSideLimit1 << std::endl;
	//std::cout << "Side Limit 2: " << tempSideLimit2 << std::endl;
}

int GridManager::getSideBoundary(NeighbourIndex t_direction, int t_expandOrigin, int t_currentLimit, bool& t_goalFound, int t_rectOrigin)
{
	//expand in the given direction
	bool expand = true;
	int tempDirectionIndex = t_expandOrigin;
	int tempPreviousDirectionIndex = t_expandOrigin;
	int limit = t_currentLimit;

	//loop while (expand = true AND t_goalFound = false)
	while (expand && !t_goalFound)
	{
		tempDirectionIndex = getNeighbourIndex(t_direction, tempDirectionIndex);
		if (tempDirectionIndex >= 0 && m_grid.at(tempDirectionIndex)->getType() != GridTile::TileType::Obstacle && m_grid.at(tempDirectionIndex)->m_mode == GridTile::ReaMode::None)
		{
			//if limit is already set
			if (limit >= 0)
			{
				switch (t_direction)
				{
				case NeighbourIndex::LEFT:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().x <= limit)
					{
						return limit;
					}
					break;
				}
				case NeighbourIndex::RIGHT:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().x >= limit)
					{
						return limit;
					}
					break;
				}
				case NeighbourIndex::TOP:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().y <= limit)
					{
						return limit;
					}
					break;
				}
				case NeighbourIndex::BOTTOM:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().y >= limit)
					{
						return limit;
					}
					break;
				}
				default:
					break;
				}
			}
			//if we got here, keep moving and try to find first limit
			tempPreviousDirectionIndex = tempDirectionIndex;//set previous valid tile
			//if tile we just checked is the goal
			if (m_grid.at(tempPreviousDirectionIndex)->getType() == GridTile::TileType::Goal)
			{
				//stop looping
				t_goalFound = true;

				if (!cardinalDirectionsAvailable(t_rectOrigin))
				{
					m_grid.at(t_expandOrigin)->setPrevious(m_grid.at(t_rectOrigin));
					m_grid.at(m_goalIndex)->setPrevious(m_grid.at(t_expandOrigin));
				}
				else
				{
					m_grid.at(m_goalIndex)->setPrevious(m_grid.at(t_rectOrigin));
				}
				expand = false;
			}
		}
		else
		{
			expand = false; // stop expanding

			//if limit is not set
			if (limit >= 0)
			{
				switch (t_direction)
				{
				case NeighbourIndex::LEFT:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().x > limit)
					{
						limit = m_grid.at(tempPreviousDirectionIndex)->getColRow().x;
						return limit;
					}
					break;
				}
				case NeighbourIndex::RIGHT:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().x < limit)
					{
						limit = m_grid.at(tempPreviousDirectionIndex)->getColRow().x;
						return limit;
					}
					break;
				}
				case NeighbourIndex::TOP:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().y > limit)
					{
						limit = m_grid.at(tempPreviousDirectionIndex)->getColRow().y;
						return limit;
					}
					break;
				}
				case NeighbourIndex::BOTTOM:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().y < limit)
					{
						limit = m_grid.at(tempPreviousDirectionIndex)->getColRow().y;
						return limit;
					}
					break;
				}
				default:
					throw std::invalid_argument("received bad direction value");
					break;
				}
			}
		}
	}

	//incase limit is unset, set it
	if (limit < 0)
	{
		//here tempPreviousDirectionIndex is either
		//equal to t_expandOrigin
		//or a new valid index for a limit
		switch (t_direction)
		{
		case NeighbourIndex::LEFT:
		case NeighbourIndex::RIGHT:
			limit = m_grid.at(tempPreviousDirectionIndex)->getColRow().x;
			break;
		case NeighbourIndex::TOP:
		case NeighbourIndex::BOTTOM:
			limit = m_grid.at(tempPreviousDirectionIndex)->getColRow().y;
			break;
		default:
			throw std::invalid_argument("received bad direction value");
			break;
		}
	}

	return limit;
}

void GridManager::markBorderers(std::vector<int>& t_rectBorder)
{
	int offset = 1;
	int index = t_rectBorder[0];
	while (index != t_rectBorder[1] + offset)
	{
		m_grid.at(index)->setToCorner();

		index += offset;
	}
	index = t_rectBorder[2];
	while (index != t_rectBorder[3] + offset)
	{
		m_grid.at(index)->setToCorner();

		index += offset;
	}
	index = t_rectBorder[0];
	offset = TILES_PER_ROW;
	while (index != t_rectBorder[2] + offset)
	{
		m_grid.at(index)->setToCorner();

		index += offset;
	}
	index = t_rectBorder[1];
	offset = TILES_PER_ROW;
	while (index != t_rectBorder[3] + offset)
	{
		m_grid.at(index)->setToCorner();

		index += offset;
	}
}

void GridManager::markFSI(std::vector<int>& t_fsi, NeighbourIndex t_dir)
{
	int fsiIndex = 0;
	int index = t_fsi[0];
	int offset = 1;
	bool mark = true;
	switch (t_dir)
	{
	case NeighbourIndex::LEFT:
	case NeighbourIndex::RIGHT:
		offset = TILES_PER_ROW;
		break;
	case NeighbourIndex::TOP:
	case NeighbourIndex::BOTTOM:
		offset = 1;
		break;
	default:
		break;
	}

	while (index != t_fsi[t_fsi.size() - 1] + offset)
	{
		if (mark)
		{
			m_grid.at(index)->setMarked(true);
		}
		if (index == t_fsi[fsiIndex + 1] + offset)
		{
			mark = false;
			fsiIndex += offset;
		}
		index += offset;
	}
}

bool GridManager::expandProper()
{
	return false;
}

bool GridManager::insertSProper()
{
	std::vector<int> rectangle;
	RectNode* parentRect = new RectNode();

	std::vector<std::vector<int>> boundaries;

	if (getRectInDirection(rectangle, NeighbourIndex::TOP, m_startIndex))
		return true;

	boundaries = getRectBoundaries(rectangle);

	parentRect->m_parentTile = m_grid.at(m_startIndex);

	parentRect->m_top.m_boundary = boundaries[TOP];
	parentRect->m_top.m_dir = NeighbourIndex::TOP;
	parentRect->m_bot.m_boundary = boundaries[BOT];
	parentRect->m_bot.m_dir = NeighbourIndex::BOTTOM;
	parentRect->m_left.m_boundary = boundaries[LEFT];
	parentRect->m_left.m_dir = NeighbourIndex::LEFT;
	parentRect->m_right.m_boundary = boundaries[RIGHT];
	parentRect->m_right.m_dir = NeighbourIndex::RIGHT;

	//for each boundary in boundaries
	for (auto& boundary : boundaries)
	{
		//for each index in boundary
		for (auto& index : boundary)
		{
			if (index != m_startIndex)
			{
				m_grid.at(index)->setPrevious(m_grid.at(m_startIndex));
			}
			m_grid.at(index)->m_gval = getOctileDist(m_grid.at(index)->getPos(), m_grid.at(m_startIndex)->getPos());
			m_grid.at(index)->m_mode = GridTile::ReaMode::Gpoint;
			m_grid.at(index)->setPrevious(m_grid.at(m_startIndex));
		}
	}

	m_rectNodes.push_back(parentRect);

	//reaGridRedraw();

	//top
	if (isValidBoundary(parentRect->m_top.m_boundary[TOP], NeighbourIndex::TOP))
	{
		parentRect->m_top.m_previous = m_grid.at(m_startIndex);
		if (successor(parentRect->m_top))
		{
			//path found
			return true;
		}
	}
	//bot
	if (isValidBoundary(parentRect->m_bot.m_boundary[0], NeighbourIndex::BOTTOM))
	{
		parentRect->m_bot.m_previous = m_grid.at(m_startIndex);
		if (successor(parentRect->m_bot))
		{
			//path found
			return true;
		}
	}
	//left
	if (isValidBoundary(parentRect->m_left.m_boundary[0], NeighbourIndex::LEFT))
	{
		parentRect->m_left.m_previous = m_grid.at(m_startIndex);
		if (successor(parentRect->m_left))
		{
			//path found
			return true;
		}
	}
	//right
	if (isValidBoundary(parentRect->m_right.m_boundary[0], NeighbourIndex::RIGHT))
	{
		parentRect->m_right.m_previous = m_grid.at(m_startIndex);
		if (successor(parentRect->m_right))
		{
			//path found
			return true;
		}
	}

	return false;
}

bool GridManager::successor(BoundaryNode& t_parentBoundary)
{
	//ENI = extend neighbour interval of Parent Boundary;
	//calculate ENI
	calcENI(t_parentBoundary);
	//FSI = Free Sub Interval<s> of ENI
	//calculate FSI
	calcFSI(t_parentBoundary);

	//for all free subintervals FSI of ENI do
		//for a point p in FSI do
			//Try to update p with PB
			//p.mode = hpoint
			//p.hval = octile(p, G)
			//p.fval = p.hval + p.gval
		//if G in FSI and G.fval <= PP.minval then
			//return path found
	//if p in FSI updated by PB then
		//PN.I = FSI
		//PN.dir = direction of PB
		//PN.minfval = min.gval;??????
		//Openlist.Insert(PN)
	//return null
	if (t_parentBoundary.m_fsi.size() == 0)
	{
		return false;
	}

	bool itering = true; //not used atm
	int index = t_parentBoundary.m_fsi.at(0).m_interval.at(0);
	int offset = 1;
	bool updated = false; //not used atm

	switch (t_parentBoundary.m_dir)
	{
	case NeighbourIndex::LEFT:
	case NeighbourIndex::RIGHT:
		offset = TILES_PER_ROW;
		break;
	case NeighbourIndex::TOP:
	case NeighbourIndex::BOTTOM:
		offset = 1;
		break;
	default:
		break;
	}

	bool fsiUpdated = false;
	//loop through every FreeSubInterval
	for (auto& fsi : t_parentBoundary.m_fsi)
	{
		fsi.m_previous = t_parentBoundary.m_previous;
		fsi.m_dir = t_parentBoundary.m_dir; //just incase

		for (auto& point : fsi.m_interval)
		{
			m_grid.at(point)->m_mode = GridTile::ReaMode::Hpoint;
			m_grid.at(point)->m_hval = getOctileDist(m_grid.at(point)->getPos(), m_grid.at(m_goalIndex)->getPos());
			m_grid.at(point)->m_fval = m_grid.at(point)->m_hval + m_grid.at(point)->m_gval;
			m_grid.at(point)->setMarked(true);
			fsiUpdated = true;

			if (fsi.m_minfval > m_grid.at(point)->m_fval)
			{
				fsi.m_minfval = m_grid.at(point)->m_fval;
				fsi.m_minValTile = m_grid.at(point);
			}

			//if G is an element of FSI
			if (point == m_goalIndex)
			{
				//if parent rect has a boundary with minVal and goal's fval is smaller than it
				if (t_parentBoundary.m_parentRect->m_minValBoundary != nullptr && m_grid.at(m_goalIndex)->m_fval <= t_parentBoundary.m_parentRect->m_minValBoundary->m_minfval)
				{
					std::cout << "Goal found" << std::endl;

					if (rightAngleToGoal(point))
					{
						m_grid.at(m_goalIndex)->setPrevious(t_parentBoundary.m_previous);
					}
					else
					{
						switch (fsi.m_dir)
						{
						case NeighbourIndex::LEFT:
						{
							int prevIndex = getNeighbourIndex(NeighbourIndex::RIGHT, m_goalIndex);
							m_grid.at(m_goalIndex)->setPrevious(m_grid.at(prevIndex));
							break;
						}
						case NeighbourIndex::RIGHT:
						{
							int prevIndex = getNeighbourIndex(NeighbourIndex::LEFT, m_goalIndex);
							m_grid.at(m_goalIndex)->setPrevious(m_grid.at(prevIndex));
							break;
						}
						case NeighbourIndex::TOP:
						{
							int prevIndex = getNeighbourIndex(NeighbourIndex::BOTTOM, m_goalIndex);
							m_grid.at(m_goalIndex)->setPrevious(m_grid.at(prevIndex));
							break;
						}
						case NeighbourIndex::BOTTOM:
						{
							int prevIndex = getNeighbourIndex(NeighbourIndex::TOP, m_goalIndex);
							m_grid.at(m_goalIndex)->setPrevious(m_grid.at(prevIndex));
							break;
						}
						default:
							break;
						}
					}


					return true;
				}
				//else if parent rect has a parenttile
				//only case possible is for the start rect
				else if (t_parentBoundary.m_parentRect->m_parentTile != nullptr && m_grid.at(m_goalIndex)->m_fval <= t_parentBoundary.m_parentRect->m_parentTile->m_fval)
				{
					std::cout << "Goal found" << std::endl;
					m_grid.at(m_goalIndex)->setPrevious(t_parentBoundary.m_parentRect->m_parentTile);
					return true;
				}
			}
		}

		//if (a point updated)
		//then make a new SearchNode
		//and add it to openlist
		if (fsiUpdated)
		{
			SearchNode PN = fsi;
			m_openlist.push(PN);
			fsiUpdated = false;
		}

		////if at least 1 free subinterval in this boundary
		//if (fsi.m_interval.size() > 0)
		//{
		//	//start looping from the first point
		//	index = fsi.m_interval.at(0);

		//	//loop until we're one tile past the last one
		//	while (index != fsi.m_interval.at(fsi.m_interval.size() - 1) + offset)
		//	{
		//		//mark tiles etc
		//		m_grid.at(index)->m_mode = GridTile::ReaMode::Hpoint;
		//		m_grid.at(index)->m_hval = getOctileDist(m_grid.at(index)->getPos(), m_grid.at(m_goalIndex)->getPos());
		//		m_grid.at(index)->m_fval = m_grid.at(index)->m_hval + m_grid.at(index)->m_gval;
		//		//just to mark fsi points different colour
		//		m_grid.at(index)->setMarked(true);


		//		////if there is a new minfval in FSI
		//		//if (fsi.m_previous->m_fval > m_grid.at(index)->m_fval)
		//		//{
		//		//	fsi.m_minfval = m_grid.at(index)->m_fval;
		//		//	fsi.m_minValTile = index;					

		//		//	////set previous up
		//		//	//if (t_parentBoundary.m_p)
		//		//	//{

		//		//	//}

		//		//	//if new minfval is smaller for the boundary
		//		//	if (t_parentBoundary.m_minfval > m_grid.at(index)->m_fval)
		//		//	{
		//		//		t_parentBoundary.m_minfval = m_grid.at(index)->m_fval;
		//		//		t_parentBoundary.m_minValFsi = &fsi;

		//		//		//if minfval is smaller for the entire rectangle
		//		//		if (t_parentBoundary.m_parentRect->m_minfval > m_grid.at(index)->m_fval)
		//		//		{
		//		//			t_parentBoundary.m_parentRect->m_minfval = m_grid.at(index)->m_fval;
		//		//			t_parentBoundary.m_parentRect->m_minValBoundary = &t_parentBoundary;
		//		//		}
		//		//	}
		//		//}

		//		//if G is an element of FSI
		//		if (index == m_goalIndex)
		//		{
		//			//if parent rect has a previous rect and goal's fval is smaller than it
		//			if (t_parentBoundary.m_parentRect->m_minValBoundary != nullptr && m_grid.at(m_goalIndex)->m_fval <= t_parentBoundary.m_parentRect->m_minValBoundary->m_minfval)
		//			{
		//				std::cout << "Goal found" << std::endl;
		//				return true;
		//			}
		//			//else if parent rect has a parenttile
		//			//only case possible is for the start rect
		//			else if (t_parentBoundary.m_parentRect->m_parentTile != nullptr && m_grid.at(m_goalIndex)->m_fval <= t_parentBoundary.m_parentRect->m_parentTile->m_fval)
		//			{
		//				std::cout << "Goal found" << std::endl;
		//				m_grid.at(m_goalIndex)->setPrevious(t_parentBoundary.m_parentRect->m_parentTile);
		//				return true;
		//			}
		//		}

		//		index += offset;
		//	}

		//	//if (a point updated)
		//		//then make a new SearchNode
		//		//and add it to openlist
		//	SearchNode PN = fsi;

		//	m_openlist.push(PN);
		//}
	}


	//while (index != t_parentBoundary.m_fsi.at(t_parentBoundary.m_fsi.size() - 1) + offset)
	//{
	//	m_grid.at(index)->m_mode = GridTile::ReaMode::Hpoint;
	//	m_grid.at(index)->m_hval = getOctileDist(m_grid.at(index)->getPos(), m_grid.at(m_goalIndex)->getPos());
	//	m_grid.at(index)->m_fval = m_grid.at(index)->m_hval + m_grid.at(index)->m_gval;

	//	if (index == m_goalIndex && m_grid.at(m_goalIndex)->m_fval <= 0)
	//	{

	//	}

	//	index += offset;
	//}
	return false;
}

void GridManager::calcENI(BoundaryNode& t_parentBoundary)
{
	int corner1 = t_parentBoundary.m_boundary.at(0);
	int corner2 = t_parentBoundary.m_boundary.at(t_parentBoundary.m_boundary.size() - 1);
	switch (t_parentBoundary.m_dir)
	{
	case NeighbourIndex::TOP:
	{
		corner1 = getNeighbourIndex(NeighbourIndex::TOP_LEFT, corner1);
		if (corner1 == -1)
		{
			corner1 = getNeighbourIndex(NeighbourIndex::TOP, t_parentBoundary.m_boundary.at(0));
		}

		corner2 = getNeighbourIndex(NeighbourIndex::TOP_RIGHT, corner2);
		if (corner2 == -1)
		{
			corner2 = getNeighbourIndex(NeighbourIndex::TOP, t_parentBoundary.m_boundary.at(t_parentBoundary.m_boundary.size() - 1));
		}

		break;
	}
	case NeighbourIndex::BOTTOM:
	{
		corner1 = getNeighbourIndex(NeighbourIndex::BOTTOM_LEFT, corner1);
		if (corner1 == -1)
		{
			corner1 = getNeighbourIndex(NeighbourIndex::BOTTOM, t_parentBoundary.m_boundary.at(0));
		}

		corner2 = getNeighbourIndex(NeighbourIndex::BOTTOM_RIGHT, corner2);
		if (corner2 == -1)
		{
			corner2 = getNeighbourIndex(NeighbourIndex::BOTTOM, t_parentBoundary.m_boundary.at(t_parentBoundary.m_boundary.size() - 1));
		}

		break;
	}
	case NeighbourIndex::LEFT:
	{
		corner1 = getNeighbourIndex(NeighbourIndex::TOP_LEFT, corner1);
		if (corner1 == -1)
		{
			corner1 = getNeighbourIndex(NeighbourIndex::LEFT, t_parentBoundary.m_boundary.at(0));
		}

		corner2 = getNeighbourIndex(NeighbourIndex::BOTTOM_LEFT, corner2);
		if (corner2 == -1)
		{
			corner2 = getNeighbourIndex(NeighbourIndex::LEFT, t_parentBoundary.m_boundary.at(t_parentBoundary.m_boundary.size() - 1));
		}

		break;
	}
	case NeighbourIndex::RIGHT:
	{
		corner1 = getNeighbourIndex(NeighbourIndex::TOP_RIGHT, corner1);
		if (corner1 == -1)
		{
			corner1 = getNeighbourIndex(NeighbourIndex::RIGHT, t_parentBoundary.m_boundary.at(0));
		}

		corner2 = getNeighbourIndex(NeighbourIndex::BOTTOM_RIGHT, corner2);
		if (corner2 == -1)
		{
			corner2 = getNeighbourIndex(NeighbourIndex::RIGHT, t_parentBoundary.m_boundary.at(t_parentBoundary.m_boundary.size() - 1));
		}

		break;
	}
	default:
		break;
	}
	t_parentBoundary.m_eni = calcBoundary(corner1, corner2, t_parentBoundary.m_dir);
}

void GridManager::calcFSI(BoundaryNode& t_parentBoundary)
{
	if (t_parentBoundary.m_eni.size() == 0)
	{
		return;
	}
	bool fsiStarted = false;
	int index = t_parentBoundary.m_eni[0];
	int fsiIndex = 0;
	int offset = 0;
	NeighbourIndex diagonalCheck1;
	NeighbourIndex diagonalCheck2;
	NeighbourIndex cornerCheck1;
	NeighbourIndex cornerCheck2;

	switch (t_parentBoundary.m_dir)
	{
	case NeighbourIndex::TOP:
		offset = 1;
		diagonalCheck1 = NeighbourIndex::RIGHT;
		diagonalCheck2 = NeighbourIndex::LEFT;
		cornerCheck1 = NeighbourIndex::TOP_LEFT;
		cornerCheck2 = NeighbourIndex::TOP_RIGHT;
		break;
	case NeighbourIndex::BOTTOM:
		offset = 1;
		diagonalCheck1 = NeighbourIndex::RIGHT;
		diagonalCheck2 = NeighbourIndex::LEFT;
		cornerCheck1 = NeighbourIndex::BOTTOM_LEFT;
		cornerCheck2 = NeighbourIndex::BOTTOM_RIGHT;
		break;
	case NeighbourIndex::LEFT:
		offset = TILES_PER_ROW;
		diagonalCheck1 = NeighbourIndex::BOTTOM;
		diagonalCheck2 = NeighbourIndex::TOP;
		cornerCheck1 = NeighbourIndex::TOP_LEFT;
		cornerCheck2 = NeighbourIndex::BOTTOM_LEFT;
		break;
	case NeighbourIndex::RIGHT:
		offset = TILES_PER_ROW;
		diagonalCheck1 = NeighbourIndex::BOTTOM;
		diagonalCheck2 = NeighbourIndex::TOP;
		cornerCheck1 = NeighbourIndex::TOP_RIGHT;
		cornerCheck2 = NeighbourIndex::BOTTOM_RIGHT;
		break;
	default:
		break;
	}

	if (static_cast<int>(diagonalCheck1) == -1 || static_cast<int>(diagonalCheck2) == -1 || static_cast<int>(cornerCheck1) == -1 || static_cast<int>(cornerCheck2) == -1)
	{
		std::cout << "REEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;
		//invalid index for neighbour check
	}


	//boundary corners need to check for diagonals.
	if (t_parentBoundary.m_eni.at(0) == getNeighbourIndex(cornerCheck1, t_parentBoundary.m_boundary.at(0)) && //check if this is a special case
		m_grid.at(index)->getType() != GridTile::TileType::Obstacle &&
		m_grid.at(getNeighbourIndex(diagonalCheck1, index))->getType() != GridTile::TileType::Obstacle)
	{
		t_parentBoundary.m_fsi.emplace_back(index, t_parentBoundary.m_dir);
		fsiStarted = true;
		index += offset;
	}
	//else //not needed i think
	//{
	//	//unmark tile as it is invalid
	//	m_grid.at(index)->setMarked(false);
	//}
	while (index != t_parentBoundary.m_eni[t_parentBoundary.m_eni.size() - 1]/* + offset*/)
	{
		if (fsiStarted)
		{
			if (m_grid.at(index)->getType() != GridTile::TileType::Obstacle)
			{
				t_parentBoundary.m_fsi.at(fsiIndex).m_interval.push_back(index);
			}
			else
			{
				fsiStarted = false;
			}
		}
		else if (m_grid.at(index)->getType() != GridTile::TileType::Obstacle)
		{
			fsiStarted = true;
			t_parentBoundary.m_fsi.emplace_back(index, t_parentBoundary.m_dir);
		}
		//if (fsiStarted && m_grid.at(index)->getType() == GridTile::TileType::Obstacle)
		//{
		//	t_parentBoundary.m_fsi.at(fsiIndex).m_interval.push_back(index - offset);
		//	fsiStarted = false;
		//	fsiIndex++;
		//}
		//else if (!fsiStarted && m_grid.at(index)->getType() != GridTile::TileType::Obstacle)
		//{
		//	t_parentBoundary.m_fsi.emplace_back(index, t_parentBoundary.m_dir);
		//	t_parentBoundary.m_fsi.at(fsiIndex).m_dir = t_parentBoundary.m_dir;
		//	fsiStarted = true;
		//}

		index += offset;
	}
	if (static_cast<int>(cornerCheck2) != -1 &&
		t_parentBoundary.m_eni.at(t_parentBoundary.m_eni.size() - 1) ==
		getNeighbourIndex(cornerCheck2, t_parentBoundary.m_boundary.at(t_parentBoundary.m_boundary.size() - 1)) && //check if this is a special case
		m_grid.at(index)->getType() != GridTile::TileType::Obstacle &&
		m_grid.at(getNeighbourIndex(diagonalCheck2, index))->getType() != GridTile::TileType::Obstacle)
	{
		t_parentBoundary.m_fsi.at(fsiIndex).m_interval.push_back(index);
	}
	else if (m_grid.at(index)->getType() != GridTile::TileType::Obstacle)
	{
		if (fsiStarted)
		{
			t_parentBoundary.m_fsi.at(fsiIndex).m_interval.push_back(index);
		}
		else
		{
			t_parentBoundary.m_fsi.emplace_back(index, t_parentBoundary.m_dir);
			t_parentBoundary.m_fsi.at(fsiIndex).m_interval.push_back(index);
		}
	}
}

std::vector<std::vector<int>> GridManager::getRectBoundaries(std::vector<int>& t_rectBoundary/*, NeighbourIndex t_direction, int t_origin*/)
{
	std::vector<std::vector<int>> boundaries;

	//potentially 4 rect boundaries
	//west0
	boundaries.push_back(calcBoundary(t_rectBoundary[0], t_rectBoundary[2], NeighbourIndex::LEFT));
	//east1
	boundaries.push_back(calcBoundary(t_rectBoundary[1], t_rectBoundary[3], NeighbourIndex::RIGHT));
	//north2
	boundaries.push_back(calcBoundary(t_rectBoundary[0], t_rectBoundary[1], NeighbourIndex::TOP));
	//south3
	boundaries.push_back(calcBoundary(t_rectBoundary[2], t_rectBoundary[3], NeighbourIndex::BOTTOM));

	return boundaries;
}

std::vector<int> GridManager::calcBoundary(int t_corner1, int t_corner2, NeighbourIndex t_dir)
{
	std::vector<int> boundary;
	int index = t_corner1;
	int offset = 0;

	switch (t_dir)
	{
	case NeighbourIndex::LEFT:
	case NeighbourIndex::RIGHT:
		offset = TILES_PER_ROW;
		break;
	case NeighbourIndex::TOP:
	case NeighbourIndex::BOTTOM:
		offset = 1;
		break;
	default:
		break;
	}

	while (index != t_corner2 + offset)
	{
		boundary.push_back(index);
		index += offset;
	}

	return boundary;
}

bool GridManager::isValidBoundary(int& t_boundary, NeighbourIndex t_directionOfBoundary)
{
	int index = getNeighbourIndex(t_directionOfBoundary, t_boundary);

	if (index >= 0 && m_grid.at(index)->m_mode == GridTile::ReaMode::None)
	{
		return true;
	}
	return false;
}

bool GridManager::expand(int t_cbn, std::vector<int>& t_corners)
{
	//std::cout << "Expanding tile: " << std::to_string(t_cbn) << std::endl;
	return getRectInDirection(t_corners, NeighbourIndex::TOP/*directionToGoal(t_cbn)*/, t_cbn);
}
