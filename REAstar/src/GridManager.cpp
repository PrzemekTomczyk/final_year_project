#include "stdafx.h"
#include "GridManager.h"

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
	m_placeString = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPLACE MODE";
	m_deleteString = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nDELETE MODE";
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

	if (m_updateRequired && !m_middleBtn)
	{
		resetNonObstacles();
		m_updateRequired = false;
	}

	if (m_getPath && m_startIndex > -1 && m_goalIndex > -1)
	{
		m_getPath = false;
		reaAlgorithm();
	}
	else if (m_startIndex == -1 || m_goalIndex == -1 && !m_lines.empty())
	{
		m_lines.clear();
	}
}

void GridManager::render()
{
	m_window.draw(m_placeModeTxt);
	for (int i = 0; i < m_grid.size(); i++)
	{
		m_grid.at(i)->render(m_window);
	}

	for (int i = 0; i < m_lines.size(); i++)
	{
		m_window.draw(&m_lines[0], m_lines.size(), sf::Lines);
		if (i > MAX_TILES)
		{
			m_lines.clear();
			std::cout << "Unexpected error during line render occured!" << std::endl;
		}
	}
}

void GridManager::reaGridRedraw()
{
	for (int i = 0; i < m_grid.size(); i++)
	{
		m_grid.at(i)->render(m_window);
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
	if (!m_rPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		resetGrid();
		m_rPressed = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		m_rPressed = false;
	}

	if (!m_tabPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
	{
		m_tabPressed = true;
		m_useRea = !m_useRea;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
	{
		m_tabPressed = false;
	}

	if (!m_f5Pressed && sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
	{
		m_f5Pressed = true;
		m_debug = !m_debug;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
	{
		m_f5Pressed = false;
	}

	if (!m_getPath && !m_enterPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		m_enterPressed = true;
		m_getPath = true;

	}
	else if (m_enterPressed && !sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		m_enterPressed = false;
	}


	if (!m_spacePressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
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
	if (!m_leftBtn && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		m_leftBtn = true;
		handleLeftClick(sf::Mouse::getPosition(m_window));
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		m_leftBtn = false;
	}

	//handle rmb
	if (!m_rightBtn && sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
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
	m_lines.clear();
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
	else if (thor::squaredLength(m_grid.at(t_index)->getPos() - m_grid.at(neighbourIndex)->getPos()) > m_grid.at(t_index)->getDiagonal())
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
	int col = t_mousePos.x / static_cast<int>(m_tileSize.x);
	int row = t_mousePos.y / static_cast<int>(m_tileSize.y);

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

void GridManager::addLine(sf::Vector2f t_p1, sf::Vector2f t_p2, sf::Color t_colour)
{
	sf::Vertex vertex1;
	sf::Vertex vertex2;

	vertex1 = sf::Vertex(t_p1);
	vertex2 = sf::Vertex(t_p2);
	vertex1.color = t_colour;
	vertex2.color = t_colour;

	m_lines.push_back(vertex1);
	m_lines.push_back(vertex2);
}

void GridManager::backTrack()
{
	m_lines.clear();
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
		//reaGridRedraw();
		//throw std::invalid_argument("goal's previous ptr not set!");
		if (m_debug)
			std::cout << "goal's previous ptr not set!" << std::endl;
	}
}

/// <summary>
/// Function used during debugging to see t_point's current path
/// </summary>
/// <param name="t_point">index value of a grid tile from which to back track</param>
void GridManager::backTrackFrom(int& t_point)
{
	if (m_grid.at(t_point)->getPrevious() != nullptr) {
		GridTile* ptr = m_grid.at(t_point);

		int col1, col2, col3;
		col1 = rand() % 255;
		col2 = rand() % 255;
		col3 = rand() % 255;


		//add all nodes with previous to the path
		while (nullptr != ptr->getPrevious())
		{
			addLine(ptr->getPos(), ptr->getPrevious()->getPos(), sf::Color(col1, col2, col3, 255));

			ptr = ptr->getPrevious();

		}
	}
}

void GridManager::setupRectCorners(std::vector<int>& t_rectCorners)
{
	//sort corner indexes so smallest index is top left and highest is bottom right
	std::sort(t_rectCorners.begin(), t_rectCorners.end());

	//set the corners for to be ordered in order of TopLeft,TopRight,BotLeft,BotRight
	int topLeft = t_rectCorners[0], topRight = t_rectCorners[1], botLeft = t_rectCorners[2], botRight = t_rectCorners[3];

	if (m_grid.at(topRight)->getColRow().y == m_grid.at(botLeft)->getColRow().y)
	{
		std::swap(t_rectCorners[1], t_rectCorners[2]);
	}
}

void GridManager::init(float t_textOffset)
{
	//use height of the window to make squares as the right side of the screen is used for tooltip info
	m_tileSize.y = static_cast<float>(m_window.getSize().y) / static_cast<float>(NO_OF_ROWS);
	m_tileSize.x = m_tileSize.y;
	m_grid.reserve(MAX_TILES);

	for (int i = 0; i < NO_OF_ROWS; i++)
	{
		for (int j = 0; j < TILES_PER_ROW; j++)
		{
			int tileIndex = j + i * TILES_PER_ROW;
			m_grid.push_back(
				new GridTile(
					sf::Vector2f(j * m_tileSize.x + (m_tileSize.x / 2.0f),
						i * m_tileSize.y + (m_tileSize.y / 2.0f)
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
	if (m_debug)
		std::cout << "Starting REA*..." << std::endl;

	//Initialise
	for (int i = m_searchNodes.size() - 1; i >= 0; i--)
	{
		if (m_searchNodes[i])
		{
			delete m_searchNodes[i];
			m_searchNodes[i] = nullptr;
		}
	}
	m_searchNodes.clear();
	m_openlist = std::priority_queue<SearchNode*, std::vector<SearchNode*>, NodeComparer>();

	m_lines.clear();
	resetNonObstacles();

	auto start = std::chrono::high_resolution_clock::now();

	if (insertS())
	{
		//goal has been found, backtrack
		backTrack();
		return;
	}
	if (m_debug)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		reaGridRedraw();
	}

	SearchNode* currentBestNode;
	while (!m_openlist.empty())
	{
		//current best node CBN
		currentBestNode = m_openlist.top();
		m_openlist.pop();

		if (expand(currentBestNode))
		{
			//goal has been found!

			if (!m_debug)
			{
				std::cout << "Expand returned true, backtracking!" << std::endl;

				auto end = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
				std::cout << "REA* time taken: " << duration.count() << "ms" << std::endl;
			}
			backTrack();
			return;
		}

		if (m_debug)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
			reaGridRedraw();
		}
	}

	std::cout << "Failed to find a path to the goal!" << std::endl;
}

double GridManager::getOctileDist(sf::Vector2f t_p1, sf::Vector2f t_p2)
{
	float dx = std::abs(t_p1.x - t_p2.x);
	float dy = std::abs(t_p1.y - t_p2.y);
	return Utils::DIAGONAL * std::min(dx, dy) + std::abs(dx - dy);
}

double GridManager::getOctileDist(sf::Vector2i t_p1, sf::Vector2i t_p2)
{
	int dx = std::abs(t_p1.x - t_p2.x);
	int dy = std::abs(t_p1.y - t_p2.y);
	return Utils::DIAGONAL * std::min(dx, dy) + std::abs(dx - dy);
}

/// <summary>
/// Function to expand a rectangle in a given direction from a origin index
/// </summary>
/// <param name="t_rectBoundary">vector container for 4 corners of the rect IF goal is not found</param>
/// <param name="t_direction">direction to expand the rectangle in</param>
/// <param name="t_origin">origin index to expand from</param>
/// <returns>true if goal has been found in rectangle, false if not</returns>
bool GridManager::getRectInDirection(std::vector<int>& t_rectBoundary, NeighbourIndex& t_direction, int& t_origin, bool t_expandOpposite, int t_sideLimit1, int t_sideLimit2)
{
	int indexInDirection = -1;
	int limitInDirection = -1;
	int sideLimit1 = t_sideLimit1, sideLimit2 = t_sideLimit2;
	bool goalFound = false;

	//indexInDirection = getNeighbourIndex(t_direction, t_origin);
	indexInDirection = t_origin;

	//if there is nothing above the starting point
	if (indexInDirection >= 0 && m_grid.at(indexInDirection)->getType() != GridTile::TileType::Obstacle/* && !m_grid.at(indexInDirection)->getVisited()*/ && m_grid.at(indexInDirection)->getType() != GridTile::TileType::Goal)
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
					sideLimit1 = getSideBoundary(Utils::TOP, tempIndex, sideLimit1, goalFound, t_origin);
					sideLimit2 = getSideBoundary(Utils::BOTTOM, tempIndex, sideLimit2, goalFound, t_origin);
					break;
				case NeighbourIndex::TOP:
				case NeighbourIndex::BOTTOM:
					limitInDirection = m_grid.at(tempIndex)->getColRow().y;
					sideLimit1 = getSideBoundary(Utils::LEFT, tempIndex, sideLimit1, goalFound, t_origin);
					sideLimit2 = getSideBoundary(Utils::RIGHT, tempIndex, sideLimit2, goalFound, t_origin);
					break;
				default:
					throw std::invalid_argument("received bad direction value");
					break;
				}
				if ((t_sideLimit1 == -1 && t_sideLimit2 == -1) || (sideLimit1 == t_sideLimit1 && sideLimit2 == t_sideLimit2))
				{
					tempIndex = getNeighbourIndex(t_direction, tempIndex);
				}
				else
				{
					switch (t_direction)
					{
					case NeighbourIndex::LEFT:
						tempIndex = getNeighbourIndex(Utils::RIGHT, tempIndex);
						limitInDirection = m_grid.at(tempIndex)->getColRow().x;
						sideLimit1 = t_sideLimit1;
						sideLimit2 = t_sideLimit2;
						break;
					case NeighbourIndex::RIGHT:
						tempIndex = getNeighbourIndex(Utils::LEFT, tempIndex);
						limitInDirection = m_grid.at(tempIndex)->getColRow().x;
						sideLimit1 = t_sideLimit1;
						sideLimit2 = t_sideLimit2;
						break;
					case NeighbourIndex::TOP:
						tempIndex = getNeighbourIndex(Utils::BOTTOM, tempIndex);
						limitInDirection = m_grid.at(tempIndex)->getColRow().y;
						sideLimit1 = t_sideLimit1;
						sideLimit2 = t_sideLimit2;
						break;
					case NeighbourIndex::BOTTOM:
						tempIndex = getNeighbourIndex(Utils::TOP, tempIndex);
						limitInDirection = m_grid.at(tempIndex)->getColRow().y;
						sideLimit1 = t_sideLimit1;
						sideLimit2 = t_sideLimit2;
						break;
					default:
						throw std::invalid_argument("received bad direction value");
						break;
					}
					break;
				}
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
			sideLimit1 = getSideBoundary(Utils::TOP, t_origin, sideLimit1, goalFound, t_origin);
			sideLimit2 = getSideBoundary(Utils::BOTTOM, t_origin, sideLimit2, goalFound, t_origin);
			break;
		case NeighbourIndex::TOP:
		case NeighbourIndex::BOTTOM:
			limitInDirection = m_grid.at(t_origin)->getColRow().y;
			sideLimit1 = getSideBoundary(Utils::LEFT, t_origin, sideLimit1, goalFound, t_origin);
			sideLimit2 = getSideBoundary(Utils::RIGHT, t_origin, sideLimit2, goalFound, t_origin);
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

	if (!goalFound)
	{
		if (t_expandOpposite)
		{
			NeighbourIndex oppositeDirection;
			int startPointIndex = t_origin;
			switch (t_direction)
			{
			case NeighbourIndex::LEFT:
				oppositeDirection = NeighbourIndex::RIGHT;
				break;
			case NeighbourIndex::RIGHT:
				oppositeDirection = NeighbourIndex::LEFT;
				break;
			case NeighbourIndex::TOP:
				oppositeDirection = NeighbourIndex::BOTTOM;
				break;
			case NeighbourIndex::BOTTOM:
				oppositeDirection = NeighbourIndex::TOP;
				break;
			default:
				throw std::invalid_argument("received bad direction value");
				break;
			}
			getRectInOpposite(t_rectBoundary, oppositeDirection, startPointIndex, sideLimit1, sideLimit2, goalFound);
		}
		else
		{
			int corner3, corner4;
			switch (t_direction)
			{
			case NeighbourIndex::LEFT:
			case NeighbourIndex::RIGHT:
				corner3 = m_grid.at(t_origin)->getColRow().x + (sideLimit1 * TILES_PER_ROW);
				corner4 = m_grid.at(t_origin)->getColRow().x + (sideLimit2 * TILES_PER_ROW);
				break;
			case NeighbourIndex::TOP:
			case NeighbourIndex::BOTTOM:
				corner3 = sideLimit1 + (m_grid.at(t_origin)->getColRow().y * TILES_PER_ROW);
				corner4 = sideLimit2 + (m_grid.at(t_origin)->getColRow().y * TILES_PER_ROW);
				break;
			default:
				throw std::invalid_argument("received bad direction value");
				break;
			}
			t_rectBoundary.push_back(corner3);
			t_rectBoundary.push_back(corner4);
		}
	}
	else if (m_grid.at(m_goalIndex)->getPrevious() == nullptr)
	{
		m_grid.at(m_goalIndex)->setPrevious(m_grid.at(t_origin));
	}
	if (!goalFound)
	{
		markBorderers(t_rectBoundary);
	}
	return goalFound;
}

void GridManager::getRectInOpposite(std::vector<int>& t_rectBoundary, NeighbourIndex& t_direction, int& t_origin, int& t_limit1, int& t_limit2, bool& t_goalFound)
{
	int indexInDirection = t_origin;
	int limitInDirection = -1;
	int sideLimit1 = t_limit1, sideLimit2 = t_limit2;
	int oppositeLimit1 = -1, oppositeLimit2 = -1;
	bool breakout = false;

	//indexInDirection = getNeighbourIndex(t_direction, t_origin);
	//if there is nothing above the starting point
	if (indexInDirection >= 0 && m_grid.at(indexInDirection)->getType() != GridTile::TileType::Obstacle/* && !m_grid.at(indexInDirection)->getVisited()*/)
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
					tempLimit1 = getSideBoundary(Utils::TOP, tempIndex, sideLimit1, t_goalFound, t_origin);
					tempLimit2 = getSideBoundary(Utils::BOTTOM, tempIndex, sideLimit2, t_goalFound, t_origin);

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
					tempLimit1 = getSideBoundary(Utils::LEFT, tempIndex, sideLimit1, t_goalFound, t_origin);
					tempLimit2 = getSideBoundary(Utils::RIGHT, tempIndex, sideLimit2, t_goalFound, t_origin);

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

	int corner1, corner2, tempSideLimit1, tempSideLimit2/*, tempDirectionLimit*/;
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
}

int GridManager::getSideBoundary(NeighbourIndex& t_direction, int& t_expandOrigin, int& t_currentLimit, bool& t_goalFound, int& t_rectOrigin)
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
		if (tempDirectionIndex >= 0 && m_grid.at(tempDirectionIndex)->getType() != GridTile::TileType::Obstacle/* && m_grid.at(tempDirectionIndex)->m_mode == GridTile::ReaMode::None*/)
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
	setupRectCorners(t_rectBorder);
	int offset = 1;

	int index = t_rectBorder[0];
	while (index <= t_rectBorder[1]/* + offset*/)
	{
		m_grid.at(index)->setToCorner();

		index += offset;
	}

	index = t_rectBorder[2];
	while (index <= t_rectBorder[3]/* + offset*/)
	{
		m_grid.at(index)->setToCorner();

		index += offset;
	}

	index = t_rectBorder[0];
	offset = TILES_PER_ROW;
	while (index <= t_rectBorder[2]/* + offset*/)
	{
		m_grid.at(index)->setToCorner();

		index += offset;
	}

	index = t_rectBorder[1];
	offset = TILES_PER_ROW;
	while (index <= t_rectBorder[3]/* + offset*/)
	{
		m_grid.at(index)->setToCorner();

		index += offset;
	}
}

/// <summary>
/// Tries to update point if better values are being passed in
/// </summary>
/// <param name="t_point">index of a point to try to update</param>
/// <param name="t_dir">direction of the FSI</param>
/// <returns>true if point has been updated, false otherwise</returns>
bool GridManager::tryToUpdateFsiPoint(int& t_point, NeighbourIndex& t_dir)
{
	bool updated = false;
	GridTile* previous = nullptr;
	NeighbourIndex nextFsiDir;
	NeighbourIndex nextFsiDiagDir;

	NeighbourIndex prevFsiDir;
	NeighbourIndex prevFsiDiagDir;

	NeighbourIndex cardinalParentDir;

	switch (t_dir)
	{
	case NeighbourIndex::LEFT:
		nextFsiDir = NeighbourIndex::BOTTOM;
		nextFsiDiagDir = NeighbourIndex::BOTTOM_RIGHT;

		prevFsiDir = NeighbourIndex::TOP;
		prevFsiDiagDir = NeighbourIndex::TOP_RIGHT;

		cardinalParentDir = NeighbourIndex::RIGHT;
		break;
	case NeighbourIndex::RIGHT:
		nextFsiDir = NeighbourIndex::BOTTOM;
		nextFsiDiagDir = NeighbourIndex::BOTTOM_LEFT;

		prevFsiDir = NeighbourIndex::TOP;
		prevFsiDiagDir = NeighbourIndex::TOP_LEFT;

		cardinalParentDir = NeighbourIndex::LEFT;
		break;
	case NeighbourIndex::TOP:
		nextFsiDir = NeighbourIndex::RIGHT;
		nextFsiDiagDir = NeighbourIndex::BOTTOM_RIGHT;

		prevFsiDir = NeighbourIndex::LEFT;
		prevFsiDiagDir = NeighbourIndex::BOTTOM_LEFT;

		cardinalParentDir = NeighbourIndex::BOTTOM;
		break;
	case NeighbourIndex::BOTTOM:
		nextFsiDir = NeighbourIndex::RIGHT;
		nextFsiDiagDir = NeighbourIndex::TOP_RIGHT;

		prevFsiDir = NeighbourIndex::LEFT;
		prevFsiDiagDir = NeighbourIndex::TOP_LEFT;

		cardinalParentDir = NeighbourIndex::TOP;
		break;
	default:
		break;
	}

	//vector to store all gvals
	std::vector<double> gvals;

	//indexes for different neighbours
	int nextFsiIndex = getNeighbourIndex(nextFsiDir, t_point);
	int nextFsiDiagIndex = getNeighbourIndex(nextFsiDiagDir, t_point);

	int prevFsiIndex = getNeighbourIndex(prevFsiDir, t_point);
	int prevFsiDiagIndex = getNeighbourIndex(prevFsiDiagDir, t_point);

	int cardinalIndex = getNeighbourIndex(cardinalParentDir, t_point);


	//int nextDiagCost = m_grid.at(nextFsiDiagIndex)->m_gval + Utils::DIAGONAL;
	//int prevDiagCost = m_grid.at(prevFsiDiagIndex)->m_gval + Utils::DIAGONAL;
	//int cardCost = m_grid.at(cardinalIndex)->m_gval + 1.0f;

	if (nextFsiIndex != -1 && nextFsiDiagIndex != -1 &&
		m_grid.at(nextFsiIndex)->getType() != GridTile::TileType::Obstacle &&
		m_grid.at(nextFsiDiagIndex)->getType() != GridTile::TileType::Obstacle)
	{
		gvals.push_back(m_grid.at(nextFsiDiagIndex)->m_gval + Utils::DIAGONAL);
	}
	else
	{
		gvals.push_back(std::numeric_limits<int>::max());
	}
	if (prevFsiIndex != -1 && prevFsiDiagIndex != -1 &&
		m_grid.at(prevFsiIndex)->getType() != GridTile::TileType::Obstacle &&
		m_grid.at(prevFsiDiagIndex)->getType() != GridTile::TileType::Obstacle)
	{
		gvals.push_back(m_grid.at(prevFsiDiagIndex)->m_gval + Utils::DIAGONAL);
	}
	else
	{
		gvals.push_back(std::numeric_limits<int>::max());
	}
	gvals.push_back(m_grid.at(cardinalIndex)->m_gval + 1.0f);


	//int lowestCost = std::min({ nextDiagCost, prevDiagCost, cardCost });
	auto lowestIter = std::min_element(gvals.begin(), gvals.end());

	if (*lowestIter < m_grid.at(t_point)->m_gval)
	{
		updated = true;
		m_grid.at(t_point)->m_gval = *lowestIter;
		switch (lowestIter - gvals.begin())
		{
		case 0:
			previous = m_grid.at(nextFsiDiagIndex);
			break;
		case 1:
			previous = m_grid.at(prevFsiDiagIndex);
			break;
		case 2:
			previous = m_grid.at(cardinalIndex);
			break;
		default:
			break;
		}
	}

	if (updated && previous)
	{
		m_grid.at(t_point)->setPrevious(previous);
		if (m_debug)
			backTrackFrom(t_point);
	}
	return updated;
}

bool GridManager::fsiSpecialCasePoint(int& t_point, BoundaryNode& t_boundary)
{
	NeighbourIndex directionCheck;
	switch (t_boundary.m_dir)
	{
	case NeighbourIndex::LEFT:
		directionCheck = NeighbourIndex::RIGHT;
		break;
	case NeighbourIndex::RIGHT:
		directionCheck = NeighbourIndex::LEFT;
		break;
	case NeighbourIndex::TOP:
		directionCheck = NeighbourIndex::BOTTOM;
		break;
	case NeighbourIndex::BOTTOM:
		directionCheck = NeighbourIndex::TOP;
		break;
	default:
		break;
	}

	//boundary corners need to check for diagonals.
	if (t_boundary.m_boundary[0] == getNeighbourIndex(directionCheck, t_point) ||
		t_boundary.m_boundary.at(t_boundary.m_boundary.size() - 1) == getNeighbourIndex(directionCheck, t_point))//check if this is a special case
	{
		return true;
	}

	return false;
}

bool GridManager::checkIfWithinRect(int& t_point, int& t_topLeft, int& t_botRight)
{
	if (m_grid.at(t_point)->getColRow().x < m_grid.at(t_topLeft)->getColRow().x ||
		m_grid.at(t_point)->getColRow().x > m_grid.at(t_botRight)->getColRow().x ||
		m_grid.at(t_point)->getColRow().y < m_grid.at(t_topLeft)->getColRow().y ||
		m_grid.at(t_point)->getColRow().y > m_grid.at(t_botRight)->getColRow().y)
	{
		return false;
	}
	return true;;
}

bool GridManager::tryToUpdateSpecialCaseFsiPoint(int& t_point, NeighbourIndex& t_dir, bool t_startPoint)
{
	bool updated = false;
	GridTile* previous = nullptr;
	NeighbourIndex fsiNeighbourDir;
	NeighbourIndex fsiNeighbourDiagDir;

	NeighbourIndex cardinalParentDir;

	switch (t_dir)
	{
	case NeighbourIndex::LEFT:
	{
		cardinalParentDir = NeighbourIndex::RIGHT;
		if (t_startPoint)
		{
			fsiNeighbourDir = NeighbourIndex::BOTTOM;
			fsiNeighbourDiagDir = NeighbourIndex::BOTTOM_RIGHT;
		}
		else
		{
			fsiNeighbourDir = NeighbourIndex::TOP;
			fsiNeighbourDiagDir = NeighbourIndex::TOP_RIGHT;
		}
		break;
	}
	case NeighbourIndex::RIGHT:
	{
		cardinalParentDir = NeighbourIndex::LEFT;
		if (t_startPoint)
		{
			fsiNeighbourDir = NeighbourIndex::BOTTOM;
			fsiNeighbourDiagDir = NeighbourIndex::BOTTOM_LEFT;
		}
		else
		{
			fsiNeighbourDir = NeighbourIndex::TOP;
			fsiNeighbourDiagDir = NeighbourIndex::TOP_LEFT;
		}
		break;
	}
	case NeighbourIndex::TOP:
	{
		cardinalParentDir = NeighbourIndex::BOTTOM;
		if (t_startPoint)
		{
			fsiNeighbourDir = NeighbourIndex::RIGHT;
			fsiNeighbourDiagDir = NeighbourIndex::BOTTOM_RIGHT;
		}
		else
		{
			fsiNeighbourDir = NeighbourIndex::LEFT;
			fsiNeighbourDiagDir = NeighbourIndex::BOTTOM_LEFT;
		}
		break;
	}
	case NeighbourIndex::BOTTOM:
	{
		cardinalParentDir = NeighbourIndex::TOP;
		if (t_startPoint)
		{
			fsiNeighbourDir = NeighbourIndex::RIGHT;
			fsiNeighbourDiagDir = NeighbourIndex::TOP_RIGHT;
		}
		else
		{
			fsiNeighbourDir = NeighbourIndex::LEFT;
			fsiNeighbourDiagDir = NeighbourIndex::TOP_LEFT;
		}
		break;
	}
	default:
		break;
	}

	int fsiNeighbourIndex = getNeighbourIndex(fsiNeighbourDir, t_point);
	int fsiNeighbourDiagIndex = getNeighbourIndex(fsiNeighbourDiagDir, t_point);

	int cardinalIndex = getNeighbourIndex(cardinalParentDir, t_point);

	//check if both indexes are valid and then if so, are both of them not obstacles 
	if (fsiNeighbourIndex != -1 && fsiNeighbourDiagIndex != -1 &&
		m_grid.at(fsiNeighbourIndex)->getType() != GridTile::TileType::Obstacle &&
		m_grid.at(fsiNeighbourDiagIndex)->getType() != GridTile::TileType::Obstacle)
	{
		if (m_grid.at(cardinalIndex)->m_gval + 1.0f > m_grid.at(fsiNeighbourDiagIndex)->m_gval + Utils::DIAGONAL &&
			m_grid.at(t_point)->m_gval > m_grid.at(fsiNeighbourDiagIndex)->m_gval + Utils::DIAGONAL)
		{
			updated = true;
			previous = m_grid.at(fsiNeighbourDiagIndex);
			m_grid.at(t_point)->m_gval = m_grid.at(fsiNeighbourDiagIndex)->m_gval + Utils::DIAGONAL;
		}
	}
	//this tile has to be valid here
	if (m_grid.at(t_point)->m_gval > m_grid.at(cardinalIndex)->m_gval + 1.0f) //hard coded distance between cardinal neighbour tiles
	{
		updated = true;
		previous = m_grid.at(cardinalIndex);
		m_grid.at(t_point)->m_gval = m_grid.at(cardinalIndex)->m_gval + 1.0f;
	}

	if (updated && previous)
	{
		m_grid.at(t_point)->setPrevious(previous);
		if (m_debug)
			backTrackFrom(t_point);
	}
	return updated;
}

bool GridManager::tryToUpdateDiagonalFsiPoint(int& t_point, NeighbourIndex& t_dir, bool t_startPoint)
{
	bool updated = false;
	NeighbourIndex fsiNeighbourCheck;
	NeighbourIndex diagonalParentCheck;
	NeighbourIndex cardinalParentCheck;
	GridTile* previous = nullptr;

	switch (t_dir)
	{
	case NeighbourIndex::LEFT:
	{
		cardinalParentCheck = NeighbourIndex::RIGHT;
		if (t_startPoint)
		{
			fsiNeighbourCheck = NeighbourIndex::BOTTOM;
			diagonalParentCheck = NeighbourIndex::BOTTOM_RIGHT;
		}
		else
		{
			fsiNeighbourCheck = NeighbourIndex::TOP;
			diagonalParentCheck = NeighbourIndex::TOP_RIGHT;
		}
		break;
	}
	case NeighbourIndex::RIGHT:
	{
		cardinalParentCheck = NeighbourIndex::LEFT;
		if (t_startPoint)
		{
			fsiNeighbourCheck = NeighbourIndex::BOTTOM;
			diagonalParentCheck = NeighbourIndex::BOTTOM_LEFT;
		}
		else
		{
			fsiNeighbourCheck = NeighbourIndex::TOP;
			diagonalParentCheck = NeighbourIndex::TOP_LEFT;
		}
		break;
	}
	case NeighbourIndex::TOP:
	{
		cardinalParentCheck = NeighbourIndex::BOTTOM;
		if (t_startPoint)
		{
			fsiNeighbourCheck = NeighbourIndex::RIGHT;
			diagonalParentCheck = NeighbourIndex::BOTTOM_RIGHT;
		}
		else
		{
			fsiNeighbourCheck = NeighbourIndex::LEFT;
			diagonalParentCheck = NeighbourIndex::BOTTOM_LEFT;
		}
		break;
	}
	case NeighbourIndex::BOTTOM:
	{
		cardinalParentCheck = NeighbourIndex::TOP;
		if (t_startPoint)
		{
			fsiNeighbourCheck = NeighbourIndex::RIGHT;
			diagonalParentCheck = NeighbourIndex::TOP_RIGHT;
		}
		else
		{
			fsiNeighbourCheck = NeighbourIndex::LEFT;
			diagonalParentCheck = NeighbourIndex::TOP_LEFT;
		}
		break;
	}
	default:
		break;
	}

	int fsiNeighbour = getNeighbourIndex(fsiNeighbourCheck, t_point);
	int diagonalParent = getNeighbourIndex(diagonalParentCheck, t_point);
	int cardinalParent = getNeighbourIndex(cardinalParentCheck, t_point);

	if (fsiNeighbour > -1 && diagonalParent > -1 && cardinalParent > -1 &&
		m_grid.at(fsiNeighbour)->getType() != GridTile::TileType::Obstacle &&
		m_grid.at(diagonalParent)->getType() != GridTile::TileType::Obstacle &&
		m_grid.at(cardinalParent)->getType() != GridTile::TileType::Obstacle)
	{
		if (m_grid.at(t_point)->m_gval > m_grid.at(diagonalParent)->m_gval + Utils::DIAGONAL)
		{
			updated = true;
			previous = m_grid.at(diagonalParent);
			m_grid.at(t_point)->m_gval = m_grid.at(diagonalParent)->m_gval + Utils::DIAGONAL;
		}
	}
	if (m_grid.at(t_point)->m_gval > m_grid.at(fsiNeighbour)->m_gval + 1.0f) //hardcoded cardinal length between tiles
	{
		updated = true;
		previous = m_grid.at(fsiNeighbour);
		m_grid.at(t_point)->m_gval = m_grid.at(fsiNeighbour)->m_gval + 1.0f;
	}

	if (updated && previous)
	{
		m_grid.at(t_point)->setPrevious(previous);
		if (m_debug)
			backTrackFrom(t_point);
	}
	return updated;
}

void GridManager::tryToUpdateSideBoundaryPoint(int& t_point, int& t_cardinalPoint, int& t_diagonalPoint, double& t_cardinalLen, double& t_diagLen, SearchNode* t_cbn, NeighbourIndex& t_boundaryDir, std::vector<int>& t_rectPoints)
{
	bool updatedCardinal = false;
	GridTile* previous = nullptr;/*m_grid.at(t_point)->getPrevious();*/
	if (t_diagonalPoint > -1 && t_point != t_diagonalPoint)
	{
		if (m_grid.at(t_point)->m_gval > m_grid.at(t_diagonalPoint)->m_gval + t_diagLen)
		{
			previous = m_grid.at(t_diagonalPoint);
			m_grid.at(t_point)->m_gval = m_grid.at(t_diagonalPoint)->m_gval + t_diagLen;
		}
	}
	if (t_cardinalPoint > -1 && t_point != t_cardinalPoint && m_grid.at(t_point)->m_gval > m_grid.at(t_cardinalPoint)->m_gval + t_cardinalLen)
	{
		previous = m_grid.at(t_cardinalPoint);
		m_grid.at(t_point)->m_gval = m_grid.at(t_cardinalPoint)->m_gval + t_cardinalLen;
		updatedCardinal = true;
	}

	if (updatedCardinal && previous)
	{
		bool looping = true;

		while (previous && looping)
		{
			int pointIndex = previous->getIndex();
			if (checkIfWithinRect(pointIndex, t_rectPoints[0], t_rectPoints[1]))
			{
				previous = previous->getPrevious();
			}
			else
			{
				looping = false;
			}
		}
	}
	if (previous)
	{
		m_grid.at(t_point)->setPrevious(previous);
		if (m_debug)
			backTrackFrom(t_point);
	}
}

void GridManager::tryToUpdateOppositeBoundaryPoint(int& t_point, int& t_p1, int& t_p2, double& t_octileP1, double& t_octileP2)
{
	//this here is an utter mess, fix to compare only to t_point.gval. Similar to the new way............
	GridTile* previous = nullptr;

	//if (t_point == 8 && t_p1 == 107)
	//{
	//	double test = static_cast<double>(m_grid.at(t_p1)->m_gval) + static_cast<double>(t_octileP1);
	//	double test1 = static_cast<double>(m_grid.at(t_point)->m_gval);
	//	if (test1 > test)
	//	{
	//		int stopRightHereCriminalScum = 1;
	//	}
	//}

	if (t_p1 > -1 && t_point != t_p1 &&
		m_grid.at(t_point)->m_gval > m_grid.at(t_p1)->m_gval + t_octileP1)
	{
		previous = m_grid.at(t_p1);
		m_grid.at(t_point)->m_gval = m_grid.at(t_p1)->m_gval + t_octileP1;
	}
	if (t_p2 > -1 && t_point != t_p2 && m_grid.at(t_point)->m_gval > m_grid.at(t_p2)->m_gval + t_octileP2)
	{
		previous = m_grid.at(t_p2);
		m_grid.at(t_point)->m_gval = m_grid.at(t_p2)->m_gval + t_octileP2;
	}
	if (previous)
	{
		m_grid.at(t_point)->setPrevious(previous);
	}
}

bool GridManager::processBoundaries(SearchNode* t_cbn, BoundaryNode& t_sideWall1, BoundaryNode& t_sideWall2, BoundaryNode& t_oppositeWall, std::vector<int>& t_rectPoints)
{
	//if cbn expand direction is NORTH
	if (t_cbn->m_dir == Utils::TOP)
	{
		//West Boundary
		{
			BoundaryNode& westBoundary = t_sideWall1;

			//get west most point from t_cbn
			int pw = t_cbn->m_interval[0];

			double diagonal = Utils::DIAGONAL;

			//move up one point
			int p = pw - TILES_PER_ROW;

			int pv = pw;

			int pd = pw + 1;
			if (pd > t_cbn->m_interval.at(t_cbn->m_interval.size() - 1))
			{
				pd = -1;
			}

			while (p >= westBoundary.m_boundary.at(0))
			{
				double octileVert = 1.0f;
				double octileDiag = diagonal;

				tryToUpdateSideBoundaryPoint(p, pv, pd, octileVert, octileDiag, t_cbn, westBoundary.m_dir, t_rectPoints);

				if (m_grid.at(p)->m_mode != GridTile::ReaMode::Hpoint)
				{
					m_grid.at(p)->m_mode = GridTile::ReaMode::Gpoint;
				}
				diagonal += Utils::DIAGONAL;

				p -= TILES_PER_ROW;
				pv -= TILES_PER_ROW;

				//this might not be correct.........
				if (pd > -1 && pd < t_cbn->m_interval.at(t_cbn->m_interval.size() - 1))
				{
					pd += 1;
				}
				else
				{
					pd = -1;
				}
			}

			if (successor(westBoundary))
			{
				return true;
			}
		}
		//East Boundary
		{
			BoundaryNode& eastBoundary = t_sideWall2;

			//get east most point from t_cbn
			int pe = t_cbn->m_interval[t_cbn->m_interval.size() - 1];

			double diagonal = Utils::DIAGONAL;

			//move up one point
			int p = pe - TILES_PER_ROW;

			int pv = pe;

			int pd = pe - 1;
			if (pd < t_cbn->m_interval.at(0))
			{
				pd = -1;
			}

			while (p >= eastBoundary.m_boundary.at(0))
			{
				double octileVert = 1.0f;
				double octileDiag = diagonal;

				tryToUpdateSideBoundaryPoint(p, pv, pd, octileVert, octileDiag, t_cbn, eastBoundary.m_dir, t_rectPoints);

				if (m_grid.at(p)->m_mode != GridTile::ReaMode::Hpoint)
				{
					m_grid.at(p)->m_mode = GridTile::ReaMode::Gpoint;
				}
				diagonal += Utils::DIAGONAL;

				p -= TILES_PER_ROW;
				pv -= TILES_PER_ROW;

				if (pd > -1 && pd > t_cbn->m_interval.at(0))
				{
					pd -= 1;
				}
				else
				{
					pd = -1;
				}
			}

			if (successor(eastBoundary))
			{
				return true;
			}
		}

		//North Boundary
		{
			BoundaryNode& northBoundary = t_oppositeWall;

			for (auto& p : northBoundary.m_boundary)
			{
				double dis = t_cbn->m_minValTile->getColRow().y - m_grid.at(p)->getColRow().y;

				int p1 = p + dis * TILES_PER_ROW;
				int p2 = p1;

				while (dis <= Utils::DIAGONAL * (t_cbn->m_minValTile->getColRow().y - m_grid.at(p)->getColRow().y) && (p1 > -1 || p2 > -1))
				{
					double octilePP1 = dis;
					double octilePP2 = dis;

					tryToUpdateOppositeBoundaryPoint(p, p1, p2, octilePP1, octilePP2);

					if (m_grid.at(p)->m_mode != GridTile::ReaMode::Hpoint)
					{
						m_grid.at(p)->m_mode = GridTile::ReaMode::Gpoint;
					}

					dis += (Utils::DIAGONAL - 1.0f);
					if (p1 > -1 && p1 > t_cbn->m_interval.at(0))
					{
						p1--;
					}
					else
					{
						p1 = -1;
					}
					if (p2 > -1 && p2 < t_cbn->m_interval.at(t_cbn->m_interval.size() - 1))
					{
						p2++;
					}
					else
					{
						p2 = -1;
					}
				}
				if (m_debug)
					backTrackFrom(p);
			}
			if (successor(northBoundary))
			{
				return true;
			}
		}
	}
	//if cbn expand direction is SOUTH	
	else if (t_cbn->m_dir == Utils::BOTTOM)
	{
		//West Boundary
		{
			BoundaryNode& westBoundary = t_sideWall1;

			//get west most point from t_cbn
			int pw = t_cbn->m_interval[0];

			double diagonal = Utils::DIAGONAL;

			//move DOWN one point
			int p = pw + TILES_PER_ROW;

			int pv = pw;

			//move right one point
			int pd = pw + 1;
			if (pd > t_cbn->m_interval.at(t_cbn->m_interval.size() - 1))
			{
				pd = -1;
			}

			while (p <= westBoundary.m_boundary.at(westBoundary.m_boundary.size() - 1))
			{
				double octileVert = 1.0f;
				double octileDiag = diagonal;

				tryToUpdateSideBoundaryPoint(p, pv, pd, octileVert, octileDiag, t_cbn, westBoundary.m_dir, t_rectPoints);

				if (m_grid.at(p)->m_mode != GridTile::ReaMode::Hpoint)
				{
					m_grid.at(p)->m_mode = GridTile::ReaMode::Gpoint;
				}
				diagonal += Utils::DIAGONAL;

				p += TILES_PER_ROW;
				pv += TILES_PER_ROW;

				if (pd > -1 && pd < t_cbn->m_interval.at(t_cbn->m_interval.size() - 1))
				{
					pd += 1;
				}
				else
				{
					pd = -1;
				}
			}

			if (successor(westBoundary))
			{
				return true;
			}
		}

		//East Boundary
		{
			BoundaryNode& eastBoundary = t_sideWall2;

			//get east most point from t_cbn
			int pe = t_cbn->m_interval[t_cbn->m_interval.size() - 1];

			double diagonal = Utils::DIAGONAL;

			//move DOWN one point
			int p = pe + TILES_PER_ROW;

			int pv = pe;

			//move left one point
			int pd = pe - 1;
			if (pd < t_cbn->m_interval.at(0))
			{
				pd = -1;
			}

			while (p <= eastBoundary.m_boundary.at(eastBoundary.m_boundary.size() - 1))
			{
				double octileVert = 1.0f;
				double octileDiag = diagonal;

				tryToUpdateSideBoundaryPoint(p, pv, pd, octileVert, octileDiag, t_cbn, eastBoundary.m_dir, t_rectPoints);

				if (m_grid.at(p)->m_mode != GridTile::ReaMode::Hpoint)
				{
					m_grid.at(p)->m_mode = GridTile::ReaMode::Gpoint;
				}
				diagonal += Utils::DIAGONAL;

				p += TILES_PER_ROW;
				pv += TILES_PER_ROW;

				if (pd > -1 && pd > t_cbn->m_interval.at(0))
				{
					pd -= 1;
				}
				else
				{
					pd = -1;
				}
			}

			if (successor(eastBoundary))
			{
				return true;
			}
		}

		//south boudary
		{
			BoundaryNode& southBoundary = t_oppositeWall;

			for (auto& p : southBoundary.m_boundary)
			{
				double dis = m_grid.at(p)->getColRow().y - t_cbn->m_minValTile->getColRow().y;

				int p1 = p - dis * TILES_PER_ROW;
				int p2 = p1;

				while (dis <= Utils::DIAGONAL * (m_grid.at(p)->getColRow().y - t_cbn->m_minValTile->getColRow().y) && (p1 > -1 || p2 > -1))
				{
					double octilePP1 = dis;
					double octilePP2 = dis;

					tryToUpdateOppositeBoundaryPoint(p, p1, p2, octilePP1, octilePP2);

					if (m_grid.at(p)->m_mode != GridTile::ReaMode::Hpoint)
					{
						m_grid.at(p)->m_mode = GridTile::ReaMode::Gpoint;
					}

					dis += (Utils::DIAGONAL - 1.0f);
					if (p1 > -1 && p1 > t_cbn->m_interval.at(0))
					{
						p1--;
					}
					else
					{
						p1 = -1;
					}
					if (p2 > -1 && p2 < t_cbn->m_interval.at(t_cbn->m_interval.size() - 1))
					{
						p2++;
					}
					else
					{
						p2 = -1;
					}
				}
				if (m_debug)
					backTrackFrom(p);
			}
			if (successor(southBoundary))
			{
				return true;
			}
		}
	}
	//if cbn expand direction is WEST
	else if (t_cbn->m_dir == Utils::LEFT)
	{
		//North boudary
		{
			BoundaryNode& northBoundary = t_sideWall1;

			//get south most point from t_cbn (PointSouth)
			int pn = t_cbn->m_interval[0];

			double diagonal = Utils::DIAGONAL;

			//move LEFT one point
			int p = pn - 1;

			//PointHorizontal
			int ph = pn;

			//move DOWN one point
			int pd = pn + TILES_PER_ROW;
			if (pd > t_cbn->m_interval.at(t_cbn->m_interval.size() - 1))
			{
				pd = -1;
			}

			while (p >= northBoundary.m_boundary.at(0))
			{
				double octileVert = 1.0f;
				double octileDiag = diagonal;

				tryToUpdateSideBoundaryPoint(p, ph, pd, octileVert, octileDiag, t_cbn, northBoundary.m_dir, t_rectPoints);

				if (m_grid.at(p)->m_mode != GridTile::ReaMode::Hpoint)
				{
					m_grid.at(p)->m_mode = GridTile::ReaMode::Gpoint;
				}
				diagonal += Utils::DIAGONAL;

				p -= 1;
				ph -= 1;

				if (pd > -1 && pd < t_cbn->m_interval.at(t_cbn->m_interval.size() - 1))
				{
					pd += TILES_PER_ROW;
				}
				else
				{
					pd = -1;
				}
			}

			if (successor(northBoundary))
			{
				return true;
			}
		}

		//south boudary
		{
			BoundaryNode& southBoundary = t_sideWall2;

			//get south most point from t_cbn (PointSouth)
			int ps = t_cbn->m_interval[t_cbn->m_interval.size() - 1];

			double diagonal = Utils::DIAGONAL;

			//move LEFT one point
			int p = ps - 1;

			//PointHorizontal
			int ph = ps;

			//move UP one point
			int pd = ps - TILES_PER_ROW;
			if (pd < t_cbn->m_interval.at(0))
			{
				pd = -1;
			}

			while (p >= southBoundary.m_boundary.at(0))
			{
				double octileVert = 1.0f;
				double octileDiag = diagonal;

				tryToUpdateSideBoundaryPoint(p, ph, pd, octileVert, octileDiag, t_cbn, southBoundary.m_dir, t_rectPoints);

				if (m_grid.at(p)->m_mode != GridTile::ReaMode::Hpoint)
				{
					m_grid.at(p)->m_mode = GridTile::ReaMode::Gpoint;
				}
				diagonal += Utils::DIAGONAL;

				p -= 1;
				ph -= 1;

				if (pd > -1 && pd > t_cbn->m_interval.at(0))
				{
					pd -= TILES_PER_ROW;
				}
				else
				{
					pd = -1;
				}
			}

			if (successor(southBoundary))
			{
				return true;
			}
		}

		//West Boundary
		{
			BoundaryNode& westBoundary = t_oppositeWall;

			for (auto& p : westBoundary.m_boundary)
			{
				double dis = t_cbn->m_minValTile->getColRow().x - m_grid.at(p)->getColRow().x;

				int p1 = p + dis;
				int p2 = p1;

				while (dis <= Utils::DIAGONAL * (t_cbn->m_minValTile->getColRow().x - m_grid.at(p)->getColRow().x) && (p1 > -1 || p2 > -1))
				{
					double octilePP1 = dis;
					double octilePP2 = dis;

					tryToUpdateOppositeBoundaryPoint(p, p1, p2, octilePP1, octilePP2);

					if (m_grid.at(p)->m_mode != GridTile::ReaMode::Hpoint)
					{
						m_grid.at(p)->m_mode = GridTile::ReaMode::Gpoint;
					}

					dis += (Utils::DIAGONAL - 1.0f);
					if (p1 > -1 && p1 > t_cbn->m_interval.at(0))
					{
						p1 -= TILES_PER_ROW;
					}
					else
					{
						p1 = -1;
					}
					if (p2 > -1 && p2 < t_cbn->m_interval.at(t_cbn->m_interval.size() - 1))
					{
						p2 += TILES_PER_ROW;
					}
					else
					{
						p2 = -1;
					}
				}
				if (m_debug)
					backTrackFrom(p);
			}
			if (successor(westBoundary))
			{
				return true;
			}
		}
	}
	//last direction, direction is EAST
	else
	{
		//North boudary
		{
			BoundaryNode& northBoundary = t_sideWall1;

			//get south most point from t_cbn (PointSouth)
			int pn = t_cbn->m_interval[0];

			double diagonal = Utils::DIAGONAL;

			//move RIGHT one point
			int p = pn + 1;

			//PointHorizontal
			int ph = pn;

			//move DOWN one point
			int pd = pn + TILES_PER_ROW;
			if (pd > t_cbn->m_interval.at(t_cbn->m_interval.size() - 1))
			{
				pd = -1;
			}

			while (p <= northBoundary.m_boundary.at(northBoundary.m_boundary.size() - 1))
			{
				double octileVert = 1.0f;
				double octileDiag = diagonal;

				tryToUpdateSideBoundaryPoint(p, ph, pd, octileVert, octileDiag, t_cbn, northBoundary.m_dir, t_rectPoints);

				if (m_grid.at(p)->m_mode != GridTile::ReaMode::Hpoint)
				{
					m_grid.at(p)->m_mode = GridTile::ReaMode::Gpoint;
				}
				diagonal += Utils::DIAGONAL;

				p += 1;
				ph += 1;

				if (pd > -1 && pd < t_cbn->m_interval.at(t_cbn->m_interval.size() - 1))
				{
					pd += TILES_PER_ROW;
				}
				else
				{
					pd = -1;
				}
			}

			if (successor(northBoundary))
			{
				return true;
			}
		}

		//south boudary
		{
			BoundaryNode& southBoundary = t_sideWall2;

			//get south most point from t_cbn (PointSouth)
			int ps = t_cbn->m_interval[t_cbn->m_interval.size() - 1];

			double diagonal = Utils::DIAGONAL;

			//move RIGHT one point
			int p = ps + 1;

			//PointHorizontal
			int ph = ps;

			//move UP one point
			int pd = ps - TILES_PER_ROW;
			if (pd < t_cbn->m_interval.at(0))
			{
				pd = -1;
			}

			while (p <= southBoundary.m_boundary.at(southBoundary.m_boundary.size() - 1))
			{
				double octileVert = 1.0f;
				double octileDiag = diagonal;

				tryToUpdateSideBoundaryPoint(p, ph, pd, octileVert, octileDiag, t_cbn, southBoundary.m_dir, t_rectPoints);

				if (m_grid.at(p)->m_mode != GridTile::ReaMode::Hpoint)
				{
					m_grid.at(p)->m_mode = GridTile::ReaMode::Gpoint;
				}
				diagonal += Utils::DIAGONAL;

				p += 1;
				ph += 1;

				if (pd > -1 && pd > t_cbn->m_interval.at(0))
				{
					pd -= TILES_PER_ROW;
				}
				else
				{
					pd = -1;
				}
			}

			if (successor(southBoundary))
			{
				return true;
			}
		}

		//East Boundary
		{
			BoundaryNode& eastBoundary = t_oppositeWall;

			for (auto& p : eastBoundary.m_boundary)
			{
				double dis = m_grid.at(p)->getColRow().x - t_cbn->m_minValTile->getColRow().x;

				int p1 = p - dis;
				int p2 = p1;

				while (dis <= Utils::DIAGONAL * (m_grid.at(p)->getColRow().x - t_cbn->m_minValTile->getColRow().x) && (p1 > -1 || p2 > -1))
				{
					double octilePP1 = dis;
					double octilePP2 = dis;

					tryToUpdateOppositeBoundaryPoint(p, p1, p2, octilePP1, octilePP2);

					if (m_grid.at(p)->m_mode != GridTile::ReaMode::Hpoint)
					{
						m_grid.at(p)->m_mode = GridTile::ReaMode::Gpoint;
					}

					dis += (Utils::DIAGONAL - 1.0f);
					if (p1 > -1 && p1 > t_cbn->m_interval.at(0))
					{
						p1 -= TILES_PER_ROW;
					}
					else
					{
						p1 = -1;
					}
					if (p2 > -1 && p2 < t_cbn->m_interval.at(t_cbn->m_interval.size() - 1))
					{
						p2 += TILES_PER_ROW;
					}
					else
					{
						p2 = -1;
					}
				}
				if (m_debug)
					backTrackFrom(p);
			}
			if (successor(eastBoundary))
			{
				return true;
			}
		}
	}

	return false;
}

bool GridManager::expand(SearchNode* t_cbn)
{
	if (m_debug)
		std::cout << "\tExpand..." << std::endl;

	bool foundGoal = false;

	if (checkIfWithinRect(m_goalIndex, t_cbn->m_interval[0], t_cbn->m_interval[0]))
	{
		if (m_debug)
			std::cout << "\t\tApparently Goal found in Expand in CBN" << std::endl;
		return true;
	}

	if (!t_cbn->m_minValTile)
	{
		int stopRightThereCriminalScum = 1;
		return false;
	}

	std::vector<BoundaryNode> boundaryNodes;
	std::vector<int> rectPoints;
	int origin = t_cbn->m_minValTile->getIndex();
	if (getRect(boundaryNodes, t_cbn->m_dir, origin, t_cbn->m_interval, rectPoints))
	{
		if (m_debug)
			std::cout << "\t\tGoal found in a rectangle in Expand" << std::endl;

		return true;
	}

	for (auto& boundary : boundaryNodes)
	{
		boundary.m_previous = t_cbn->m_minValTile;
	}

	if (t_cbn->m_dir == Utils::TOP)
	{
		foundGoal = processBoundaries(t_cbn, boundaryNodes[LEFT], boundaryNodes[RIGHT], boundaryNodes[TOP], rectPoints);
	}
	else if (t_cbn->m_dir == Utils::BOTTOM)
	{
		foundGoal = processBoundaries(t_cbn, boundaryNodes[LEFT], boundaryNodes[RIGHT], boundaryNodes[BOT], rectPoints);
	}
	else if (t_cbn->m_dir == Utils::LEFT)
	{
		foundGoal = processBoundaries(t_cbn, boundaryNodes[TOP], boundaryNodes[BOT], boundaryNodes[LEFT], rectPoints);
	}
	else
	{
		foundGoal = processBoundaries(t_cbn, boundaryNodes[TOP], boundaryNodes[BOT], boundaryNodes[RIGHT], rectPoints);
	}

	return foundGoal;
}

bool GridManager::insertS()
{
	if (m_debug)
		std::cout << "\tInsertS..." << std::endl;

	std::vector<BoundaryNode> boundaryNodes;

	if (getStartRect(boundaryNodes, Utils::TOP, m_startIndex))
	{
		if (m_debug)
			std::cout << "\t\tGoal found in a rectangle in InsertS" << std::endl;
		return true;
	}

	//for each boundary in boundaries
	for (auto& boundary : boundaryNodes)
	{
		//for each index in boundary
		for (auto& index : boundary.m_boundary)
		{
			if (index != m_startIndex)
			{
				m_grid.at(index)->setPrevious(m_grid.at(m_startIndex));
			}
			m_grid.at(index)->m_gval = getOctileDist(m_grid.at(index)->getColRow(), m_grid.at(m_startIndex)->getColRow());
			m_grid.at(index)->m_mode = GridTile::ReaMode::Gpoint;
		}
	}

	//left
	if (boundaryNodes[LEFT].m_boundary.size() > 0 && isValidBoundary(boundaryNodes[LEFT].m_boundary[0], boundaryNodes[LEFT].m_dir))
	{
		boundaryNodes[LEFT].m_previous = m_grid.at(m_startIndex);
		if (successor(boundaryNodes[LEFT]))
		{
			//path found
			return true;
		}
	}
	//right
	if (boundaryNodes[RIGHT].m_boundary.size() > 0 && isValidBoundary(boundaryNodes[RIGHT].m_boundary[0], boundaryNodes[RIGHT].m_dir))
	{
		boundaryNodes[RIGHT].m_previous = m_grid.at(m_startIndex);
		if (successor(boundaryNodes[RIGHT]))
		{
			//path found
			return true;
		}
	}
	//top
	if (boundaryNodes[TOP].m_boundary.size() > 0 && isValidBoundary(boundaryNodes[TOP].m_boundary[0], boundaryNodes[TOP].m_dir))
	{
		boundaryNodes[TOP].m_previous = m_grid.at(m_startIndex);
		if (successor(boundaryNodes[TOP]))
		{
			//path found
			return true;
		}
	}
	//bot
	if (boundaryNodes[BOT].m_boundary.size() > 0 && isValidBoundary(boundaryNodes[BOT].m_boundary[0], boundaryNodes[BOT].m_dir))
	{
		boundaryNodes[BOT].m_previous = m_grid.at(m_startIndex);
		if (successor(boundaryNodes[BOT]))
		{
			//path found
			return true;
		}
	}

	return false;
}

bool GridManager::successor(BoundaryNode& t_parentBoundary)
{
	if (m_debug)
		std::cout << "\t\t\tSuccessor..." << std::endl;

	//ENI = extend neighbour interval of Parent Boundary;
	//calculate ENI
	calcENI(t_parentBoundary);
	//FSI = Free Sub Interval<s> of ENI
	//calculate FSI
	calcFSI(t_parentBoundary);

	if (t_parentBoundary.m_fsi.size() == 0)
	{
		if (m_debug)
			std::cout << "\t\t\t\tSuccessor Quit - FSI = 0..." << std::endl;
		return false;
	}

	//used to check if a FSI was updated at all
	bool fsiUpdated = false;

	//loop through every FreeSubInterval
	for (auto& fsi : t_parentBoundary.m_fsi)
	{
		fsi.m_previous = t_parentBoundary.m_previous;
		fsi.m_dir = t_parentBoundary.m_dir; //just incase
		bool firstDiag = false;
		bool firstDiagStartPoint = false;
		bool lastDiag = false;
		bool secondDiagStartPoint = false;

		for (auto& point : fsi.m_interval)
		{

			if (point == t_parentBoundary.m_eni[0] || point == t_parentBoundary.m_eni[t_parentBoundary.m_eni.size() - 1])
			{
				if (fsiSpecialCasePoint(point, t_parentBoundary))
				{
					if (tryToUpdateSpecialCaseFsiPoint(point, fsi.m_dir, (point == fsi.m_interval[0])))
					{
						fsiUpdated = true;
						setupFsiPoint(point, fsi);
					}
				}
				else if (point == t_parentBoundary.m_eni[0])
				{
					firstDiag = true;
					firstDiagStartPoint = point == fsi.m_interval[0];
				}
				else if (point == t_parentBoundary.m_eni[t_parentBoundary.m_eni.size() - 1])
				{
					lastDiag = true;
				}
			}
			else
			{
				if (tryToUpdateFsiPoint(point, t_parentBoundary.m_dir))
				{
					fsiUpdated = true;
					setupFsiPoint(point, fsi);
				}
			}

			if (point == fsi.m_interval[fsi.m_interval.size() - 1])
			{
				if (firstDiag)
				{
					if (tryToUpdateDiagonalFsiPoint(t_parentBoundary.m_eni[0], fsi.m_dir, firstDiagStartPoint))
					{
						fsiUpdated = true;
						setupFsiPoint(t_parentBoundary.m_eni[0], fsi);
					}
				}
				if (lastDiag)
				{
					if (tryToUpdateDiagonalFsiPoint(t_parentBoundary.m_eni[t_parentBoundary.m_eni.size() - 1], fsi.m_dir, (point == fsi.m_interval[0])))
					{
						fsiUpdated = true;
						setupFsiPoint(t_parentBoundary.m_eni[t_parentBoundary.m_eni.size() - 1], fsi);
					}
				}
			}
		}

		//if goal is inside FSI
		if (std::find(fsi.m_interval.begin(), fsi.m_interval.end(), m_goalIndex) != fsi.m_interval.end())
		{
			//goal's fval is not greater than (PP) parent search node's minfval
			if (t_parentBoundary.m_previous != nullptr && m_grid.at(m_goalIndex)->m_fval <= t_parentBoundary.m_previous->m_fval)
			{
				if (m_debug)
					std::cout << "\t\t\t\Goal found in FSI" << std::endl;

				return true;
			}
		}
	}
	//if a point in a FSI was updated
	if (fsiUpdated)
	{
		//loop through all FSIs to find cheapest one
		for (int i = 0; i < t_parentBoundary.m_fsi.size(); i++)
		{
			SearchNode* PN = new SearchNode(t_parentBoundary.m_fsi[i]);
			m_searchNodes.push_back(PN);
			m_openlist.push(PN);
		}
	}
	return false;
}

void GridManager::setupFsiPoint(int& t_point, SearchNode& t_fsi)
{
	m_grid.at(t_point)->m_mode = GridTile::ReaMode::Hpoint;
	m_grid.at(t_point)->m_hval = getOctileDist(m_grid.at(t_point)->getColRow(), m_grid.at(m_goalIndex)->getColRow());
	m_grid.at(t_point)->m_fval = m_grid.at(t_point)->m_hval + m_grid.at(t_point)->m_gval;
	m_grid.at(t_point)->setMarked(true);

	if (t_fsi.m_minfval > m_grid.at(t_point)->m_fval)
	{
		t_fsi.m_minfval = m_grid.at(t_point)->m_fval;
		t_fsi.m_minValTile = m_grid.at(t_point);
	}
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
	int fsiIndex = -1;
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

	//boundary corners need to check for diagonals.
	if (t_parentBoundary.m_eni.at(0) == getNeighbourIndex(cornerCheck1, t_parentBoundary.m_boundary.at(0)))//check if this is a special case
	{
		if (m_grid.at(index)->getType() != GridTile::TileType::Obstacle &&
			m_grid.at(getNeighbourIndex(diagonalCheck1, index))->getType() != GridTile::TileType::Obstacle)
		{
			t_parentBoundary.m_fsi.emplace_back(index, t_parentBoundary.m_dir);
			fsiIndex++;
			fsiStarted = true;
		}
		index += offset;
	}

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
			fsiIndex++;
		}

		index += offset;
	}
	if (t_parentBoundary.m_eni.at(t_parentBoundary.m_eni.size() - 1) ==
		getNeighbourIndex(cornerCheck2, t_parentBoundary.m_boundary.at(t_parentBoundary.m_boundary.size() - 1))) //check if this is a special case
	{
		if (m_grid.at(index)->getType() != GridTile::TileType::Obstacle &&
			m_grid.at(getNeighbourIndex(diagonalCheck2, index))->getType() != GridTile::TileType::Obstacle)
		{
			t_parentBoundary.m_fsi.at(fsiIndex).m_interval.push_back(index);
		}
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
			fsiIndex++;
			t_parentBoundary.m_fsi.at(fsiIndex).m_interval.push_back(index);
		}
	}
}

void GridManager::getRectBoundaries(std::vector<int>& t_rectBoundary, std::vector<BoundaryNode>& t_boundaries)
{
	//potentially 4 rect boundaries
	//left0
	t_boundaries[LEFT].m_boundary = calcBoundary(t_rectBoundary[0], t_rectBoundary[2], Utils::LEFT);
	//right1
	t_boundaries[RIGHT].m_boundary = calcBoundary(t_rectBoundary[1], t_rectBoundary[3], Utils::RIGHT);
	//top2
	t_boundaries[TOP].m_boundary = calcBoundary(t_rectBoundary[0], t_rectBoundary[1], Utils::TOP);
	//bot3
	t_boundaries[BOT].m_boundary = calcBoundary(t_rectBoundary[2], t_rectBoundary[3], Utils::BOTTOM);
}

std::vector<int> GridManager::calcBoundary(int& t_corner1, int& t_corner2, NeighbourIndex& t_dir)
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

	while (index > -1 && index < MAX_TILES && index <= t_corner2/* + offset*/)
	{
		boundary.push_back(index);
		index += offset;
	}

	return boundary;
}

bool GridManager::isValidBoundary(int& t_boundary, NeighbourIndex& t_directionOfBoundary)
{
	int index = getNeighbourIndex(t_directionOfBoundary, t_boundary);

	if (index >= 0/* && m_grid.at(index)->m_mode == GridTile::ReaMode::None*/)
	{
		return true;
	}
	return false;
}

bool GridManager::getStartRect(std::vector<BoundaryNode>& t_boundaries, NeighbourIndex& t_dir, int& t_origin)
{
	std::vector<int> rectangle;
	t_boundaries.resize(4);

	for (int i = 0; i < 4; i++)
	{
		t_boundaries[i].m_dir = static_cast<NeighbourIndex>(i);
	}

	if (getRectInDirection(rectangle, t_dir, t_origin, true, -1, -1))
		return true; //goal found in rectangle

	getRectBoundaries(rectangle, t_boundaries);

	return false;
}

bool GridManager::getRect(std::vector<BoundaryNode>& t_boundaries, NeighbourIndex& t_dir, int& t_origin, std::vector<int>& t_fsiInterval, std::vector<int>& t_rectPoints)
{
	std::vector<int> rectangle;
	t_boundaries.resize(4);

	for (int i = 0; i < 4; i++)
	{
		t_boundaries[i].m_dir = static_cast<NeighbourIndex>(i);
	}

	int sideLim1;
	int sideLim2;

	switch (t_dir)
	{
	case NeighbourIndex::LEFT:
	case NeighbourIndex::RIGHT:
		sideLim1 = getCurrentSideLimit(t_dir, t_origin, t_fsiInterval, Utils::TOP);
		sideLim2 = getCurrentSideLimit(t_dir, t_origin, t_fsiInterval, Utils::BOTTOM);
		break;
	case NeighbourIndex::TOP:
	case NeighbourIndex::BOTTOM:
		sideLim1 = getCurrentSideLimit(t_dir, t_origin, t_fsiInterval, Utils::LEFT);
		sideLim2 = getCurrentSideLimit(t_dir, t_origin, t_fsiInterval, Utils::RIGHT);
		break;
	default:
		break;
	}

	if (getRectInDirection(rectangle, t_dir, t_origin, false, sideLim1, sideLim2))
		return true; //goal found in rectangle

	getRectBoundaries(rectangle, t_boundaries);

	t_rectPoints.push_back(rectangle[0]);
	t_rectPoints.push_back(rectangle[rectangle.size() - 1]);

	return false;
}

int GridManager::getCurrentSideLimit(NeighbourIndex& t_expandDir, int& t_origin, std::vector<int>& t_interval, NeighbourIndex& t_sideDirection)
{
	int sideLimit = -1;

	switch (t_expandDir)
	{
	case NeighbourIndex::LEFT:
	case NeighbourIndex::RIGHT:
	{
		if (t_sideDirection == Utils::TOP)
		{
			sideLimit = m_grid[t_interval[0]]->getColRow().y;
		}
		else
		{
			sideLimit = m_grid[t_interval[t_interval.size() - 1]]->getColRow().y;
		}
		break;
	}
	case NeighbourIndex::TOP:
	case NeighbourIndex::BOTTOM:
	{
		if (t_sideDirection == Utils::LEFT)
		{
			sideLimit = m_grid[t_interval[0]]->getColRow().x;
		}
		else
		{
			sideLimit = m_grid[t_interval[t_interval.size() - 1]]->getColRow().x;
		}
		break;
	}
	default:
		throw std::invalid_argument("received bad direction value");
		break;
	}

	return sideLimit;
}