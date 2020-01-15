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

void GridManager::tempRender()
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

void GridManager::aStar(std::function<void(GridTile*)> f_visit)
{
	if (m_startIndex > -1 && m_goalIndex > -1)
	{
		resetNonObstacles();

		std::priority_queue<GridTile*, std::vector<GridTile*>, TileComparer> pq;
		for (int i = 0; i < m_grid.size(); i++)
		{
			if (nullptr != m_grid.at(i) && m_grid.at(i)->getType() != GridTile::TileType::Obstacle)
			{
				float estDist = thor::length(m_grid.at(m_goalIndex)->getPathfindingPos() - m_grid.at(i)->getPathfindingPos());
				//estDist *= 1.1f;
				m_grid.at(i)->setEstDist(estDist);

				// init g[v] to infinity # dont YET know the distance to these nodes
				m_grid.at(i)->setCurrDist(std::numeric_limits<int>::max());
				m_grid.at(i)->setTotalDist(std::numeric_limits<int>::max());
			}
		}

		// init g[s] to 0
		m_grid.at(m_startIndex)->setCurrDist(0);
		m_grid.at(m_startIndex)->setTotalDist(0);
		// mark s
		m_grid.at(m_startIndex)->setMarked(true);

		// Add s to pq
		GridTile* current;
		pq.push(m_grid.at(m_startIndex));

		// while the !pq.empty() && pq.top() != goal node
		while (!pq.empty() && pq.top() != m_grid.at(m_goalIndex))
		{
			current = pq.top();
			//pop from pq
			pq.pop();
			current->setVisited(true);
			f_visit(current);

			//for each child node c of pq.top() - 8 neighbours
			for (int i = 0; i < 8; i++)
			{
				GridManager::NeighbourIndex neighbour = static_cast<GridManager::NeighbourIndex>(i);
				int tileIndex = getNeighbourIndex(neighbour, current->getIndex());
				if (tileIndex > -1 && m_grid.at(tileIndex)->getType() != GridTile::TileType::Obstacle)
				{
					GridTile* child = m_grid.at(tileIndex);

					//if child != previous(pq.top())
					if (child != current->getPrevious()) {
						//g(child) # g(c) is weight from current to child + distance of previous node
						int dist;
						switch (neighbour)
						{
						case GridManager::NeighbourIndex::LEFT:
						case GridManager::NeighbourIndex::RIGHT:
						case GridManager::NeighbourIndex::BOTTOM:
						case GridManager::NeighbourIndex::TOP:
							dist = 100;
							break;
						case GridManager::NeighbourIndex::TOP_LEFT:
						case GridManager::NeighbourIndex::TOP_RIGHT:
						case GridManager::NeighbourIndex::BOTTOM_LEFT:
						case GridManager::NeighbourIndex::BOTTOM_RIGHT:
							dist = 141;
							break;
						default:
							break;
						}

						float childCurrDist = current->getCurrDist() + dist;//f(c)
						//let total child dist = h(child) + distance so far
						float totalChildDist = child->getEstDist() + childCurrDist;

						if (totalChildDist < child->getTotalDist()) {
							// set current distance
							child->setCurrDist(childCurrDist);

							//let f[c] = total distance
							child->setTotalDist(totalChildDist);

							// set previous pointer of child to pq.top()
							child->setPrevious(current);
						}//end if

						if (!child->getMarked())
						{
							//mark child
							child->setMarked(true);

							//add child to pq
							pq.push(child);
						}//end if Marked check

					}//end if we're not checking parent

				}//end if index and TileType check

			}//end for
			tempRender();
		}//end while

		if (m_grid.at(m_goalIndex)->getPrevious() != nullptr) {
			GridTile* ptr = m_grid.at(m_goalIndex);

			//add all nodes with previous to the path
			while (nullptr != ptr->getPrevious())
			{
				if (ptr != m_grid.at(m_goalIndex) && ptr != m_grid.at(m_startIndex))
				{
					ptr->setToPath();
				}
				ptr = ptr->getPrevious();
			}
		}
	}//end if
}

