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

bool GridManager::update()
{
	handleInput();

	if (m_startIndex > -1 && m_goalIndex > -1 && m_updateRequired)
	{
		return true;
	}
	return false;
}


void GridManager::render()
{
	m_window.draw(m_placeModeTxt);
	for (int i = 0; i < m_grid.size(); i++)
	{
		m_grid.at(i)->render(m_window);
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
		if (m_rightBtn && m_updateRequired)
		{
			m_updateRequired = false;
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
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle) && m_middleBtn)
	{
		if (m_middleBtn && m_changedGrid)
		{
			resetNonObstacles();
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
	else if (!m_deleteMode && (m_grid.at(tileIndex)->getType() != GridTile::TileType::Obstacle || m_startIndex != tileIndex || m_goalIndex != tileIndex))
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
	case GridManager::NeighbourIndex::LEFT:
		neighbourIndex = t_index + LEFT_TILE;
		break;
	case GridManager::NeighbourIndex::RIGHT:
		neighbourIndex = t_index + RIGHT_TILE;
		break;
	case GridManager::NeighbourIndex::TOP_LEFT:
		neighbourIndex = t_index + LEFT_TOP_TILE;
		break;
	case GridManager::NeighbourIndex::TOP:
		neighbourIndex = t_index + TOP_TILE;
		break;
	case GridManager::NeighbourIndex::TOP_RIGHT:
		neighbourIndex = t_index + RIGHT_TOP_TILE;
		break;
	case GridManager::NeighbourIndex::BOTTOM_LEFT:
		neighbourIndex = t_index + LEFT_BOTTOM_TILE;
		break;
	case GridManager::NeighbourIndex::BOTTOM:
		neighbourIndex = t_index + BOTTOM_TILE;
		break;
	case GridManager::NeighbourIndex::BOTTOM_RIGHT:
		neighbourIndex = t_index + RIGHT_BOTTOM_TILE;
		break;
	default:
		//invalid neighbour
		neighbourIndex = -1;
		return neighbourIndex;
		break;
	}

	if (neighbourIndex < 0 || neighbourIndex >= MAX_TILES)
	{
		neighbourIndex = -1;
		return neighbourIndex;
	}
	else if (thor::length(m_grid.at(t_index)->getPos() - m_grid.at(neighbourIndex)->getPos()) > m_grid.at(t_index)->getDiagonal())
	{
		neighbourIndex = -1;
		return neighbourIndex;
	}
	else
	{
		return neighbourIndex;
	}
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

void GridManager::updateNotRequired()
{
	m_updateRequired = false;
}

void GridManager::setToPath(int t_index)
{
	if (m_grid.at(t_index)->getType() != GridTile::TileType::Goal && m_grid.at(t_index)->getType() != GridTile::TileType::Start)
	{
		m_grid.at(t_index)->setToPath();
	}
}

void GridManager::resetPath()
{
	resetNonObstacles();
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

void GridManager::init(float t_textOffset)
{
	//use height of the window to make squares as the right side of the screen is used for tooltip info
	m_tileSize.y = m_window.getSize().y / NO_OF_ROWS;
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
	m_placeModeTxt.setCharacterSize((int)(m_window.getSize().y / 31));
	m_placeModeTxt.setPosition(t_textOffset - m_placeModeTxt.getGlobalBounds().width / 2.0f, 0);
	m_placeModeTxt.setFillColor(sf::Color::Green);

	if (MAX_TILES == TEST_LAYOUT)
	{
		setTestLayout();
	}
}

//void GridManager::aStar(std::function<void(GridTile*)> f_visit)
//{
//	if (m_startIndex > -1 && m_goalIndex > -1)
//	{
//		resetNonObstacles();
//
//		std::priority_queue<GridTile*, std::vector<GridTile*>, TileComparer> pq;
//		for (int i = 0; i < m_grid.size(); i++)
//		{
//			if (nullptr != m_grid.at(i) && m_grid.at(i)->getType() != GridTile::TileType::Obstacle)
//			{
//				float estDist = thor::length(m_grid.at(m_goalIndex)->getRowCol() - m_grid.at(i)->getRowCol());
//				//estDist *= 1.1f;
//				m_grid.at(i)->setEstDist(estDist);
//
//				// init g[v] to infinity # dont YET know the distance to these nodes
//				m_grid.at(i)->setCurrDist(std::numeric_limits<int>::max());
//				m_grid.at(i)->setTotalDist(std::numeric_limits<int>::max());
//			}
//		}
//
//		// init g[s] to 0
//		m_grid.at(m_startIndex)->setCurrDist(0);
//		m_grid.at(m_startIndex)->setTotalDist(0);
//		// mark s
//		m_grid.at(m_startIndex)->setMarked(true);
//
//		// Add s to pq
//		GridTile* current;
//		pq.push(m_grid.at(m_startIndex));
//
//		// while the !pq.empty() && pq.top() != goal node
//		while (!pq.empty() && pq.top() != m_grid.at(m_goalIndex))
//		{
//			current = pq.top();
//			//pop from pq
//			pq.pop();
//			current->setVisited(true);
//			f_visit(current);
//
//			//GridTile* closestChild = nullptr;
//
//			if (current->getIndex() == 16)
//			{
//				int test = 1337;
//			}
//
//			/// <summary>
//			/// TO-DO:
//			///		1. Investigate why unneeded nodes are getting marked/expanded when path is more than 4 nodes
//			/// </summary>
//
//			//for each child node c of pq.top() - 8 neighbours
//			for (int i = 0; i < 8; i++)
//			{
//				GridManager::NeighbourIndex neighbour = static_cast<GridManager::NeighbourIndex>(i);
//				int tileIndex = getNeighbourIndex(neighbour, current->getIndex());
//				if (tileIndex > -1 && m_grid.at(tileIndex)->getType() != GridTile::TileType::Obstacle)
//				{
//					GridTile* child = m_grid.at(tileIndex);
//
//					//if child != previous(pq.top())
//					if (child != current->getPrevious()) {
//						//g(child) # g(c) is weight from current to child + distance of previous node
//						int dist;
//						switch (neighbour)
//						{
//						case GridManager::NeighbourIndex::LEFT:
//						case GridManager::NeighbourIndex::RIGHT:
//						case GridManager::NeighbourIndex::BOTTOM:
//						case GridManager::NeighbourIndex::TOP:
//							dist = 100;
//							break;
//						case GridManager::NeighbourIndex::TOP_LEFT:
//						case GridManager::NeighbourIndex::TOP_RIGHT:
//						case GridManager::NeighbourIndex::BOTTOM_LEFT:
//						case GridManager::NeighbourIndex::BOTTOM_RIGHT:
//							dist = 141;
//							break;
//						default:
//							break;
//						}
//
//						float childCurrDist = current->getCurrDist() + dist;//f(c)
//						//let total child dist = h(child) + distance so far
//						float totalChildDist = child->getEstDist() + childCurrDist;
//
//						if (totalChildDist < child->getTotalDist()) {
//							// set current distance
//							child->setCurrDist(childCurrDist);
//
//							//let f[c] = total distance
//							child->setTotalDist(totalChildDist);
//
//							//if (closestChild)
//							//{
//							//	if (closestChild->getTotalDist() > child->getTotalDist())
//							//	{
//							//		closestChild = child;
//							//	}
//							//}
//							//else
//							//{
//							//	closestChild = child;
//							//}
//
//							// set previous pointer of child to pq.top()
//							child->setPrevious(current);
//						}//end if
//
//						if (!child->getMarked())
//						{
//							//mark child
//							child->setMarked(true);
//
//							//add child to pq
//							pq.push(child);
//						}//end if Marked check
//
//					}//end if we're not checking parent
//
//				}//end if index and TileType check
//
//			}//end for
//		}//end while
//
//		if (m_grid.at(m_goalIndex)->getPrevious() != nullptr) {
//			GridTile* ptr = m_grid.at(m_goalIndex);
//
//			//add all nodes with previous to the path
//			while (nullptr != ptr->getPrevious())
//			{
//				if (ptr != m_grid.at(m_goalIndex) && ptr != m_grid.at(m_startIndex))
//				{
//					ptr->setToPath();
//				}
//				ptr = ptr->getPrevious();
//			}
//		}
//	}//end if
//}

void GridManager::reaAlgorithm()
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

	std::priority_queue<GridTile*, std::vector<GridTile*>, TileComparer> pq;
	std::vector<int> neighbours;
	if (insertS(neighbours))
	{
		return;
	}
	//while (!m_reaList.empty())
	//{
	//	//
	//	if (expand(0))
	//	{
	//		return;
	//	}
	//}

	return;
}

bool GridManager::insertS(std::vector<int>& t_neighbours)
{
	std::vector<int> temp;
	GridManager::NeighbourIndex searchStartDirection;

	sf::Vector2f vecToGoal = m_grid.at(m_goalIndex)->getPos() - m_grid.at(m_startIndex)->getPos();
	if (std::abs(vecToGoal.x) >= std::abs(vecToGoal.y))
	{
		if (vecToGoal.x >= 0.f)
		{
			searchStartDirection = NeighbourIndex::RIGHT;
		}
		else
		{
			searchStartDirection = NeighbourIndex::LEFT;
		}
	}
	else
	{
		if (vecToGoal.y >= 0.f)
		{
			searchStartDirection = NeighbourIndex::BOTTOM;
		}
		else
		{
			searchStartDirection = NeighbourIndex::TOP;
		}
	}

	getRectInDirection(temp, searchStartDirection, m_startIndex);



	//std::vector<int> rect; //original rectangle expanded by Start node
	//if ((std::find(rect.begin(), rect.end(), m_goalIndex) != rect.end()))
	//{
	//	return false;
	//}
	//for (const auto& )


	return true;
}

bool GridManager::successor()
{
	return true;
}

float GridManager::getOctileDist(sf::Vector2f t_p1, sf::Vector2f t_p2)
{
	float D = 1.f;
	float D2 = sqrt(2.f);
	float dx = abs(t_p1.x - t_p2.x);
	float dy = abs(t_p1.y - t_p2.y);
	return D * (dx + dy) + (D2 - 2.f * D) * std::min(dx, dy);
}

bool GridManager::getRectInDirection(std::vector<int>& t_rectBoundary, NeighbourIndex t_direction, int t_origin)
{
	int indexInDirection = -1;
	int limitInDirection = -1;
	int sideLimit1 = -1, sideLimit2 = -1;
	bool goalFound = false;

	indexInDirection = getNeighbourIndex(t_direction, t_origin);
	//if there is nothing above the starting point
	if (indexInDirection >= 0 && m_grid.at(indexInDirection)->getType() != GridTile::TileType::Obstacle)
	{
		//expand in the given direction
		int tempIndex = indexInDirection;
		while (tempIndex >= 0 && m_grid.at(tempIndex)->getType() != GridTile::TileType::Obstacle)
		{
			//we're looping again so we have a new top index
			indexInDirection = tempIndex;
			if (m_grid.at(indexInDirection)->getType() != GridTile::TileType::Goal)
			{
				switch (t_direction)
				{
				case GridManager::NeighbourIndex::LEFT:
				case GridManager::NeighbourIndex::RIGHT:
					limitInDirection = m_grid.at(tempIndex)->getColRow().x;
					sideLimit1 = getSideBoundary(NeighbourIndex::TOP, tempIndex, sideLimit1, goalFound);
					sideLimit2 = getSideBoundary(NeighbourIndex::BOTTOM, tempIndex, sideLimit2, goalFound);
					break;
				case GridManager::NeighbourIndex::TOP:
				case GridManager::NeighbourIndex::BOTTOM:
					limitInDirection = m_grid.at(tempIndex)->getColRow().y;
					sideLimit1 = getSideBoundary(NeighbourIndex::LEFT, tempIndex, sideLimit1, goalFound);
					sideLimit2 = getSideBoundary(NeighbourIndex::RIGHT, tempIndex, sideLimit2, goalFound);
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
				break;
			}
		}
	}
	else
	{
		//we cant expand in that direction, just expand sideways
		switch (t_direction)
		{
		case GridManager::NeighbourIndex::LEFT:
		case GridManager::NeighbourIndex::RIGHT:
			limitInDirection = m_grid.at(t_origin)->getColRow().x;
			sideLimit1 = getSideBoundary(NeighbourIndex::TOP, t_origin, sideLimit1, goalFound);
			sideLimit2 = getSideBoundary(NeighbourIndex::BOTTOM, t_origin, sideLimit2, goalFound);
			break;
		case GridManager::NeighbourIndex::TOP:
		case GridManager::NeighbourIndex::BOTTOM:
			limitInDirection = m_grid.at(t_origin)->getColRow().y;
			sideLimit1 = getSideBoundary(NeighbourIndex::LEFT, t_origin, sideLimit1, goalFound);
			sideLimit2 = getSideBoundary(NeighbourIndex::RIGHT, t_origin, sideLimit2, goalFound);
			break;
		default:
			throw std::invalid_argument("received bad direction value");
			break;
		}
	}

	int corner1, corner2;
	switch (t_direction)
	{
	case GridManager::NeighbourIndex::LEFT:
	case GridManager::NeighbourIndex::RIGHT:
		corner1 = limitInDirection + (sideLimit1 * TILES_PER_ROW);
		corner2 = limitInDirection + (sideLimit2 * TILES_PER_ROW);
		break;
	case GridManager::NeighbourIndex::TOP:
	case GridManager::NeighbourIndex::BOTTOM:
		corner1 = sideLimit1 + (limitInDirection * TILES_PER_ROW);
		corner2 = sideLimit2 + (limitInDirection * TILES_PER_ROW);
		break;
	default:
		throw std::invalid_argument("received bad direction value");
		break;
	}
	t_rectBoundary.push_back(corner1);
	t_rectBoundary.push_back(corner2);

	if (m_startIndex != corner1 && m_goalIndex != corner1)
	{
		m_grid.at(corner1)->setToPath();
	}
	if (m_startIndex != corner2 && m_goalIndex != corner2)
	{
		m_grid.at(corner2)->setToPath();
	}

	std::cout << "Limit in direction: " << limitInDirection << std::endl;
	std::cout << "Side Limit 1: " << sideLimit1 << std::endl;
	std::cout << "Side Limit 2: " << sideLimit2 << std::endl;

	if (!goalFound)
	{
		NeighbourIndex oppositeDirection;
		int startPointIndex;
		switch (t_direction)
		{
		case GridManager::NeighbourIndex::LEFT:
			oppositeDirection = NeighbourIndex::RIGHT;
			startPointIndex = getNeighbourIndex(NeighbourIndex::LEFT, t_origin);
			break;
		case GridManager::NeighbourIndex::RIGHT:
			oppositeDirection = NeighbourIndex::LEFT;
			startPointIndex = getNeighbourIndex(NeighbourIndex::RIGHT, t_origin);
			break;
		case GridManager::NeighbourIndex::TOP:
			oppositeDirection = NeighbourIndex::BOTTOM;
			startPointIndex = getNeighbourIndex(NeighbourIndex::TOP, t_origin);
			break;
		case GridManager::NeighbourIndex::BOTTOM:
			oppositeDirection = NeighbourIndex::TOP;
			startPointIndex = getNeighbourIndex(NeighbourIndex::BOTTOM, t_origin);
			break;
		default:
			throw std::invalid_argument("received bad direction value");
			break;
		}
		if (startPointIndex <= -1 || m_grid.at(startPointIndex)->getType() == GridTile::TileType::Obstacle)
		{
			startPointIndex = t_origin;
		}
		getRectInOpposite(t_rectBoundary, oppositeDirection, startPointIndex, sideLimit1, sideLimit2, goalFound);
	}
	if (goalFound)
	{
		std::cout << "Goal found!" << std::endl;
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
	if (indexInDirection >= 0 && m_grid.at(indexInDirection)->getType() != GridTile::TileType::Obstacle)
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
				case GridManager::NeighbourIndex::LEFT:
				case GridManager::NeighbourIndex::RIGHT:
				{
					tempLimit1 = getSideBoundary(NeighbourIndex::TOP, tempIndex, sideLimit1, t_goalFound);
					tempLimit2 = getSideBoundary(NeighbourIndex::BOTTOM, tempIndex, sideLimit2, t_goalFound);

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
				case GridManager::NeighbourIndex::TOP:
				case GridManager::NeighbourIndex::BOTTOM:
				{
					tempLimit1 = getSideBoundary(NeighbourIndex::LEFT, tempIndex, sideLimit1, t_goalFound);
					tempLimit2 = getSideBoundary(NeighbourIndex::RIGHT, tempIndex, sideLimit2, t_goalFound);

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

	int corner1, corner2, tempSideLimit1, tempSideLimit2, tempDirectionLimit;
	if (oppositeLimit1 >= 0 && oppositeLimit2 >= 0)
	{
		tempSideLimit1 = oppositeLimit1;
		tempSideLimit2 = oppositeLimit2;
		//tempDirectionLimit = limitInDirection;
	}
	else
	{
		tempSideLimit1 = sideLimit1;
		tempSideLimit2 = sideLimit2;
		//tempDirectionLimit = t_di
	}
	switch (t_direction)
	{
	case GridManager::NeighbourIndex::LEFT:
	case GridManager::NeighbourIndex::RIGHT:
		corner1 = limitInDirection + (tempSideLimit1 * TILES_PER_ROW);
		corner2 = limitInDirection + (tempSideLimit2 * TILES_PER_ROW);
		break;
	case GridManager::NeighbourIndex::TOP:
	case GridManager::NeighbourIndex::BOTTOM:
		corner1 = tempSideLimit1 + (limitInDirection * TILES_PER_ROW);
		corner2 = tempSideLimit2 + (limitInDirection * TILES_PER_ROW);
		break;
	default:
		throw std::invalid_argument("received bad direction value");
		break;
	}
	t_rectBoundary.push_back(corner1);
	t_rectBoundary.push_back(corner2);

	if (m_startIndex != corner1 && m_goalIndex != corner1)
	{
		m_grid.at(corner1)->setToPath();
	}
	if (m_startIndex != corner2 && m_goalIndex != corner2)
	{
		m_grid.at(corner2)->setToPath();
	}

	std::cout << "Limit in opposite direction: " << limitInDirection << std::endl;
	std::cout << "Side Limit 1: " << tempSideLimit1 << std::endl;
	std::cout << "Side Limit 2: " << tempSideLimit2 << std::endl;
}

int GridManager::getSideBoundary(NeighbourIndex t_direction, int t_expandOrigin, int t_currentLimit, bool& t_goalFound)
{
	//expand in the given direction
	bool expand = true;
	int tempDirectionIndex = t_expandOrigin;
	int tempPreviousDirectionIndex = t_expandOrigin;
	int limit = t_currentLimit;

	while (expand)
	{
		tempDirectionIndex = getNeighbourIndex(t_direction, tempDirectionIndex);
		if (tempDirectionIndex >= 0 && m_grid.at(tempDirectionIndex)->getType() != GridTile::TileType::Obstacle)
		{
			if (limit >= 0)
			{
				switch (t_direction)
				{
				case GridManager::NeighbourIndex::LEFT:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().x <= limit)
					{
						return limit;
					}
					break;
				}
				case GridManager::NeighbourIndex::RIGHT:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().x >= limit)
					{
						return limit;
					}
					break;
				}
				case GridManager::NeighbourIndex::TOP:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().y <= limit)
					{
						return limit;
					}
					break;
				}
				case GridManager::NeighbourIndex::BOTTOM:
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
			tempPreviousDirectionIndex = tempDirectionIndex;
			if (m_grid.at(tempPreviousDirectionIndex)->getType() == GridTile::TileType::Goal)
			{
				t_goalFound = true;
				expand = false;
			}
		}
		else
		{
			//if limit is unset, set it
			if (limit < 0)
			{
				switch (t_direction)
				{
				case GridManager::NeighbourIndex::LEFT:
				case GridManager::NeighbourIndex::RIGHT:
					limit = m_grid.at(tempPreviousDirectionIndex)->getColRow().x;
					break;
				case GridManager::NeighbourIndex::TOP:
				case GridManager::NeighbourIndex::BOTTOM:
					limit = m_grid.at(tempPreviousDirectionIndex)->getColRow().y;
					break;
				default:
					throw std::invalid_argument("received bad direction value");
					break;
				}
			}
			else
			{
				switch (t_direction)
				{
				case GridManager::NeighbourIndex::LEFT:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().x > limit)
					{
						limit = m_grid.at(tempPreviousDirectionIndex)->getColRow().x;
						return limit;
					}
					break;
				}
				case GridManager::NeighbourIndex::RIGHT:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().x < limit)
					{
						limit = m_grid.at(tempPreviousDirectionIndex)->getColRow().x;
						return limit;
					}
					break;
				}
				case GridManager::NeighbourIndex::TOP:
				{
					if (m_grid.at(tempPreviousDirectionIndex)->getColRow().y > limit)
					{
						limit = m_grid.at(tempPreviousDirectionIndex)->getColRow().y;
						return limit;
					}
					break;
				}
				case GridManager::NeighbourIndex::BOTTOM:
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
			expand = false; // stop expanding
		}
	}

	return limit;
}

bool GridManager::expand(int t_cbn)
{
	return true;
}
