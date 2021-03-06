//#include "stdafx.h"
//#include "RenderGrid.h"
//
//RenderGrid::RenderGrid(sf::Font& t_font, sf::RenderWindow& t_window, int t_maxTiles, int t_noOfRows, int t_tilesPerRow) :
//	m_font(t_font),
//	m_window(t_window),
//	m_grid(MAX_TILES, nullptr),
//	MAX_TILES(t_maxTiles),
//	NO_OF_ROWS(t_noOfRows),
//	TILES_PER_ROW(t_tilesPerRow),
//	TOP_TILE(-TILES_PER_ROW),
//	LEFT_TOP_TILE(TOP_TILE - 1),
//	RIGHT_TOP_TILE(TOP_TILE + 1),
//	BOTTOM_TILE(TILES_PER_ROW),
//	LEFT_BOTTOM_TILE(BOTTOM_TILE - 1),
//	RIGHT_BOTTOM_TILE(BOTTOM_TILE + 1)
//{
//	m_placeString = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPLACE MODE";
//	m_deleteString = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nDELETE MODE";
//	m_tempRect.reserve(4);
//	m_tempBoundaries.reserve(4);
//	m_tempBoundaries.resize(4);
//	for (int i = 0; i < 4; i++)
//	{
//		m_tempBoundaries[i].m_dir = static_cast<NeighbourIndex>(i);
//		m_tempBoundaries[i].m_boundary.reserve(TILES_PER_ROW);
//		m_tempBoundaries[i].m_eni.reserve(TILES_PER_ROW);
//		m_tempBoundaries[i].m_fsi.reserve(TILES_PER_ROW);
//	}
//}
//
//RenderGrid::~RenderGrid()
//{
//	for (GridTile* p : m_grid)
//	{
//		delete p;
//	}
//	m_grid.clear();
//	m_grid.shrink_to_fit();
//
//	for (SearchNode* p : m_searchNodes)
//	{
//		delete p;
//	}
//	m_searchNodes.clear();
//	m_searchNodes.shrink_to_fit();
//}
//
//void RenderGrid::update(const sf::Vector2f& t_textPos)
//{
//	handleInput();
//
//	updateText(t_textPos);
//
//	if (m_startIndex > -1 && m_goalIndex > -1 && m_updateRequired && !m_middleBtn)
//	{
//		resetNonObstacles();
//		m_lines.clear();
//		m_updateRequired = false;
//	}
//
//	if (m_getPath && m_startIndex > -1 && m_goalIndex > -1)
//	{
//		std::cout << "####################################" << std::endl;
//		std::cout << "Running ";
//		m_getPath = false;
//		prePathfindReset();
//		bool pathFound = false;
//
//		auto start = std::chrono::high_resolution_clock::now();
//		auto end = std::chrono::high_resolution_clock::now();
//
//		if (m_useRea)
//		{
//			std::cout << "REA*" << std::endl;
//			start = std::chrono::high_resolution_clock::now();
//
//			pathFound = reaAlgorithm();
//			end = std::chrono::high_resolution_clock::now();
//		}
//		else
//		{
//			std::cout << "A*" << std::endl;
//			start = std::chrono::high_resolution_clock::now();
//			pathFound = aStar();
//			end = std::chrono::high_resolution_clock::now();
//		}
//
//		if (pathFound)
//		{
//			if (!m_debug)
//			{
//				auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//				std::cout << "Time taken to find goal: " << duration.count() << " micro-seconds" << std::endl;
//
//				if (m_useRea)
//				{
//					m_reaTimeTakenText.setString(Utils::REA_TIME_TAKEN_ALIGNMENT + std::to_string(duration.count()) + " micro-seconds");
//					m_reaDistText.setString(Utils::REA_DISTANCE_ALIGNMENT + std::to_string(backTrack()));
//				}
//				else if (!m_useRea)
//				{
//					m_aStarTimeTakenText.setString(Utils::ASTAR_TIME_TAKEN_ALIGNMENT + std::to_string(duration.count()) + " micro-seconds");
//					m_aStarDistText.setString(Utils::ASTAR_DISTANCE_ALIGNMENT + std::to_string(backTrack()));
//				}
//			}
//			else
//			{
//				if (m_useRea)
//				{
//					m_reaTimeTakenText.setString(Utils::REA_TIME_TAKEN_ALIGNMENT + "DEBUGGED");
//					m_reaDistText.setString(Utils::REA_DISTANCE_ALIGNMENT + std::to_string(backTrack()));
//				}
//				else if (!m_useRea)
//				{
//					m_aStarTimeTakenText.setString(Utils::ASTAR_TIME_TAKEN_ALIGNMENT + "DEBUGGED");
//					m_aStarDistText.setString(Utils::ASTAR_DISTANCE_ALIGNMENT + std::to_string(backTrack()));
//				}
//			}
//		}
//
//	}
//	else if (m_startIndex == -1 || m_goalIndex == -1 && !m_lines.empty())
//	{
//		m_lines.clear();
//	}
//}
//
//void RenderGrid::render()
//{
//	m_window.draw(m_placeModeTxt);
//	m_window.draw(m_debugText);
//	m_window.draw(m_debugStateText);
//	m_window.draw(m_algText);
//	m_window.draw(m_algTypeText);
//	m_window.draw(m_reaTimeTakenText);
//	m_window.draw(m_aStarTimeTakenText);
//	m_window.draw(m_reaDistText);
//	m_window.draw(m_aStarDistText);
//
//	if (m_grid.size() < Utils::LARGE_SANDBOX_TILE_AMOUNT)
//	{
//		for (int i = 0; i < m_grid.size(); i++)
//		{
//			m_grid[i]->render(m_window);
//		}
//	}
//	else
//	{
//		if (m_goalIndex > -1 && m_goalIndex < MAX_TILES)
//		{
//			m_grid[m_goalIndex]->render(m_window);
//
//		}
//		if (m_startIndex > -1 && m_startIndex < MAX_TILES)
//		{
//			m_grid[m_startIndex]->render(m_window);
//		}
//	}
//
//	for (int i = 0; i < m_lines.size(); i++)
//	{
//		m_window.draw(&m_lines[0], m_lines.size(), sf::Lines);
//		if (i > MAX_TILES)
//		{
//			m_lines.clear();
//			std::cout << "Unexpected error during line render occured!" << std::endl;
//			break;
//		}
//	}
//}
//
//void RenderGrid::reaGridRedraw()
//{
//	for (int i = 0; i < m_grid.size(); i++)
//	{
//		m_grid[i]->render(m_window);
//	}
//	for (int i = 0; i < m_lines.size(); i++)
//	{
//		m_window.draw(&m_lines[0], m_lines.size(), sf::Lines);
//		if (i > MAX_TILES)
//		{
//			m_lines.clear();
//			std::cout << "Unexpected error during line render occured!" << std::endl;
//			break;
//		}
//	}
//	m_window.display();
//
//	bool loop = true;
//	bool nPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::N);
//	while (loop && m_debug)
//	{
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
//		{
//			nPressed = true;
//		}
//		else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::N) && nPressed)
//		{
//			loop = false;
//		}
//		if (!m_f5Pressed && sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
//		{
//			m_f5Pressed = true;
//			loop = false;
//			m_debug = false;
//		}
//		else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
//		{
//			m_f5Pressed = false;
//		}
//	}
//}
//
//void RenderGrid::gridRedraw()
//{
//	for (int i = 0; i < m_grid.size(); i++)
//	{
//		m_grid[i]->render(m_window);
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
//		{
//			m_debug = false;
//			return;
//		}
//	}
//	m_window.display();
//}
//
//void RenderGrid::handleInput()
//{
//	if (m_window.hasFocus())
//	{
//		handleKeyboard();
//		handleMouse();
//	}
//}
//
//void RenderGrid::handleKeyboard()
//{
//	if (!m_rPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::R))
//	{
//		resetGrid();
//		m_rPressed = true;
//	}
//	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::R))
//	{
//		m_rPressed = false;
//	}
//
//	if (!m_tabPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
//	{
//		m_tabPressed = true;
//		m_useRea = !m_useRea;
//	}
//	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
//	{
//		m_tabPressed = false;
//	}
//
//	if (!m_f5Pressed && sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
//	{
//		m_f5Pressed = true;
//		m_debug = !m_debug;
//	}
//	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
//	{
//		m_f5Pressed = false;
//	}
//
//	if (m_startIndex > -1 && m_goalIndex > -1 && !m_getPath && !m_enterPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
//	{
//		m_enterPressed = true;
//		m_getPath = true;
//
//	}
//	else if (m_enterPressed && !sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
//	{
//		m_enterPressed = false;
//	}
//
//
//	if (!m_spacePressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
//	{
//		m_spacePressed = true;
//		m_deleteMode = !m_deleteMode;
//		if (m_deleteMode)
//		{
//			float newOffset = m_placeModeTxt.getGlobalBounds().width;
//			m_placeModeTxt.setFillColor(sf::Color::Red);
//			m_placeModeTxt.setString(m_deleteString);
//
//			newOffset = newOffset - m_placeModeTxt.getGlobalBounds().width;
//			newOffset /= 2;
//			m_placeModeTxt.setPosition(m_placeModeTxt.getPosition().x + newOffset, m_placeModeTxt.getPosition().y);
//		}
//		else
//		{
//			float newOffset = m_placeModeTxt.getGlobalBounds().width;
//			m_placeModeTxt.setFillColor(sf::Color::Green);
//			m_placeModeTxt.setString(m_placeString);
//
//			newOffset = newOffset - m_placeModeTxt.getGlobalBounds().width;
//			newOffset /= 2;
//			m_placeModeTxt.setPosition(m_placeModeTxt.getPosition().x + newOffset, m_placeModeTxt.getPosition().y);
//		}
//	}
//	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
//	{
//		m_spacePressed = false;
//	}
//
//}
//
//void RenderGrid::handleMouse()
//{
//	//handle lmb
//	if (!m_leftBtn && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
//	{
//		m_leftBtn = true;
//		handleLeftClick(sf::Mouse::getPosition(m_window));
//	}
//	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
//	{
//		m_leftBtn = false;
//	}
//
//	//handle rmb
//	if (!m_rightBtn && sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
//	{
//		m_rightBtn = true;
//		handleRightClick(sf::Mouse::getPosition(m_window));
//	}
//	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
//	{
//		m_rightBtn = false;
//	}
//
//	//handle mmb
//	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
//	{
//		m_middleBtn = true;
//		handleMiddleClick(sf::Mouse::getPosition(m_window));
//	}
//	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle) && m_middleBtn)
//	{
//		if (m_middleBtn && m_changedGrid)
//		{
//			resetNonObstacles();
//			m_changedGrid = false;
//			m_updateRequired = true;
//		}
//		m_middleBtn = false;
//	}
//}
//
//void RenderGrid::handleLeftClick(sf::Vector2i t_mousePos)
//{
//	int tileIndex = getClickedTileIndex(t_mousePos);
//
//	if (tileIndex != m_startIndex && tileIndex != m_goalIndex)
//	{
//		if (m_grid[tileIndex]->getType() != GridTile::TileType::Obstacle)
//		{
//			checkIfStartRemoved(tileIndex);
//			m_grid[tileIndex]->reset();
//			m_updateRequired = true;
//
//			if (m_goalIndex > -1)
//			{
//				m_grid[m_goalIndex]->reset();
//			}
//
//			m_grid[tileIndex]->setToGoal();
//			m_goalIndex = tileIndex;
//		}
//	}
//}
//
//void RenderGrid::handleRightClick(sf::Vector2i t_mousePos)
//{
//	int tileIndex = getClickedTileIndex(t_mousePos);
//
//	if (tileIndex != m_startIndex && tileIndex != m_goalIndex)
//	{
//		if (m_grid[tileIndex]->getType() != GridTile::TileType::Obstacle)
//		{
//			m_updateRequired = true;
//			if (m_startIndex > -1)
//			{
//				m_grid[m_startIndex]->reset();
//			}
//			m_grid[tileIndex]->setToStart();
//			m_startIndex = tileIndex;
//		}
//	}
//}
//
//void RenderGrid::handleMiddleClick(sf::Vector2i t_mousePos)
//{
//	int tileIndex = getClickedTileIndex(t_mousePos);
//
//	if (m_deleteMode && (m_grid[tileIndex]->getType() == GridTile::TileType::Obstacle || m_startIndex == tileIndex || m_goalIndex == tileIndex))
//	{
//		if (m_startIndex == tileIndex)
//		{
//			m_startIndex = -1;
//		}
//		else if (m_goalIndex == tileIndex)
//		{
//			m_goalIndex = -1;
//		}
//
//		m_grid[tileIndex]->reset();
//		m_changedGrid = true;
//	}
//	else if (!m_deleteMode)
//	{
//		if (m_startIndex == tileIndex)
//		{
//			m_startIndex = -1;
//		}
//		else if (m_goalIndex == tileIndex)
//		{
//			m_goalIndex = -1;
//		}
//		m_grid[tileIndex]->setToObstacle();
//		m_changedGrid = true;
//	}
//}
//
//void RenderGrid::resetGrid()
//{
//	m_goalIndex = -1;
//	m_startIndex = -1;
//
//	for (int i = 0; i < m_grid.size(); i++)
//	{
//		m_grid[i]->reset();
//	}
//	if (MAX_TILES == TEST_LAYOUT)
//	{
//		setTestLayout();
//	}
//}
//
//void RenderGrid::resetNonObstacles()
//{
//	for (int i = 0; i < m_grid.size(); i++)
//	{
//		if (m_grid[i]->getType() != GridTile::TileType::Obstacle && m_grid[i]->getType() != GridTile::TileType::Start && m_grid[i]->getType() != GridTile::TileType::Goal)
//		{
//			m_grid[i]->reset();
//		}
//		else if (m_grid[i]->getType() == GridTile::TileType::Start)
//		{
//			m_grid[i]->reset();
//			m_grid[i]->setToStart();
//		}
//		else if (m_grid[i]->getType() == GridTile::TileType::Goal)
//		{
//			m_grid[i]->reset();
//			m_grid[i]->setToGoal();
//		}
//	}
//}
//
///// <summary>
///// Returns an index of a neighbouring tile <para>0 = left, 1 = right, 2 = top left, 3 = above, 4 = top right, 5 = bottom left, 6 = below, 7 = bottom right</para>
///// </summary>
///// <param name="t_neighbour">int to pick a neighbour</param>
///// <param name="t_index">index of the tile to get neighbour of</param>
///// <returns>index of neighbouring tile</returns>
//int RenderGrid::getNeighbourIndex(NeighbourIndex t_neighbour, int t_index)
//{
//	int neighbourIndex;
//	switch (t_neighbour)
//	{
//	case NeighbourIndex::LEFT:
//		neighbourIndex = t_index + LEFT_TILE;
//		break;
//	case NeighbourIndex::RIGHT:
//		neighbourIndex = t_index + RIGHT_TILE;
//		break;
//	case NeighbourIndex::TOP_LEFT:
//		neighbourIndex = t_index + LEFT_TOP_TILE;
//		break;
//	case NeighbourIndex::TOP:
//		neighbourIndex = t_index + TOP_TILE;
//		break;
//	case NeighbourIndex::TOP_RIGHT:
//		neighbourIndex = t_index + RIGHT_TOP_TILE;
//		break;
//	case NeighbourIndex::BOTTOM_LEFT:
//		neighbourIndex = t_index + LEFT_BOTTOM_TILE;
//		break;
//	case NeighbourIndex::BOTTOM:
//		neighbourIndex = t_index + BOTTOM_TILE;
//		break;
//	case NeighbourIndex::BOTTOM_RIGHT:
//		neighbourIndex = t_index + RIGHT_BOTTOM_TILE;
//		break;
//	default:
//		//invalid neighbour
//		return -1;
//		break;
//	}
//
//	if (neighbourIndex < 0 || neighbourIndex >= MAX_TILES)
//	{
//		return -1;
//	}
//	else
//	{
//		//int col = m_grid[t_index]->getCol() - m_grid[neighbourIndex]->getCol();
//		//int row = m_grid[t_index]->getRow() - m_grid[neighbourIndex]->getRow();
//		if (std::abs(m_grid[t_index]->getCol() - m_grid[neighbourIndex]->getCol()) > 1 ||
//			std::abs(m_grid[t_index]->getRow() - m_grid[neighbourIndex]->getRow()) > 1)
//			return -1;
//		else
//			return neighbourIndex;
//	}
//}
//
//bool RenderGrid::cardinalDirectionsAvailable(int t_index)
//{
//	for (int i = 0; i < 4; i++)
//	{
//		int neighbourIndex = getNeighbourIndex((NeighbourIndex)i, t_index);
//		if (neighbourIndex >= 0 && m_grid[neighbourIndex]->getType() == GridTile::TileType::Obstacle)
//		{
//			return false;
//		}
//	}
//	return true;
//}
//
//void RenderGrid::checkIfStartRemoved(int t_tileClicked)
//{
//	if (m_grid[t_tileClicked]->getType() == GridTile::TileType::Start)
//	{
//		m_startIndex = -1;
//	}
//}
//
//int RenderGrid::getClickedTileIndex(sf::Vector2i t_mousePos)
//{
//	int col = t_mousePos.x / static_cast<int>(m_tileSize.x);
//	int row = t_mousePos.y / static_cast<int>(m_tileSize.y);
//
//	if (row < 0)
//	{
//		row = 0;
//	}
//	else if (row > NO_OF_ROWS - 1)
//	{
//		row = NO_OF_ROWS - 1;
//	}
//	if (col < 0)
//	{
//		col = 0;
//	}
//	else if (col > TILES_PER_ROW - 1)
//	{
//		col = TILES_PER_ROW - 1;
//	}
//	return  col + (row * TILES_PER_ROW);
//}
//
//void RenderGrid::setToPath(int t_index)
//{
//	if (m_grid[t_index]->getType() != GridTile::TileType::Goal && m_grid[t_index]->getType() != GridTile::TileType::Start)
//	{
//		m_grid[t_index]->setToPath();
//	}
//}
//
//void RenderGrid::setTestLayout()
//{
//	m_grid[26]->setToObstacle();
//	m_grid[36]->setToObstacle();
//	m_grid[46]->setToObstacle();
//	m_grid[56]->setToObstacle();
//	m_grid[55]->setToObstacle();
//	m_grid[54]->setToObstacle();
//	m_grid[53]->setToObstacle();
//	m_grid[52]->setToObstacle();
//	m_grid[51]->setToObstacle();
//	m_grid[50]->setToObstacle();
//
//	m_grid[131]->setToObstacle();
//	m_grid[121]->setToObstacle();
//	m_grid[111]->setToObstacle();
//	m_grid[102]->setToObstacle();
//	m_grid[103]->setToObstacle();
//	m_grid[104]->setToObstacle();
//	m_grid[105]->setToObstacle();
//	m_grid[106]->setToObstacle();
//}
//
///// <summary>
///// Adds a vertex to a vector
///// </summary>
///// <param name="t_p1">position of vertex 1</param>
///// <param name="t_p2">position of vertex 2</param>
//void RenderGrid::addLine(sf::Vector2f t_p1, sf::Vector2f t_p2)
//{
//	m_lines.emplace_back(t_p1);
//	m_lines.emplace_back(t_p2);
//}
//
//void RenderGrid::addLine(sf::Vector2f t_p1, sf::Vector2f t_p2, sf::Color t_colour)
//{
//	m_lines.emplace_back(t_p1, t_colour);
//	m_lines.emplace_back(t_p2, t_colour);
//}
//
//float RenderGrid::backTrack()
//{
//	int breakoutCheck = 0;
//	m_lines.clear();
//	if (m_grid[m_goalIndex]->getPrevious() != nullptr) {
//		GridTile* ptr = m_grid[m_goalIndex];
//		std::cout << "Path taken: ";
//		float pathDistance = 0.0f;
//
//		//add all nodes with previous to the path
//		while (nullptr != ptr->getPrevious())
//		{
//			breakoutCheck++;
//			if (breakoutCheck > MAX_TILES)
//			{
//				std::cout << "Exitted through breakout check!" << std::endl;
//				m_lines.clear();
//				return 0.0f;
//			}
//			if (ptr != m_grid[m_goalIndex] && ptr != m_grid[m_startIndex])
//			{
//				ptr->setToPath();
//				std::cout << ptr->getIndex();
//				std::cout << " -> ";
//			}
//
//			addLine(ptr->getPos(), ptr->getPrevious()->getPos());
//
//			pathDistance += thor::length(static_cast<sf::Vector2f>(ptr->getColRow() - ptr->getPrevious()->getColRow()));
//
//			ptr = ptr->getPrevious();
//		}
//		std::cout << ptr->getIndex() << std::endl;
//		return pathDistance;
//	}
//	else
//	{
//		if (m_debug)
//			std::cout << "goal's previous ptr not set!" << std::endl;
//	}
//	return 0;
//}
//
///// <summary>
///// Function used during debugging to see t_point's current path
///// </summary>
///// <param name="t_point">index value of a grid tile from which to back track</param>
//void RenderGrid::backTrackFrom(int& t_point)
//{
//	if (m_grid[t_point]->getPrevious() != nullptr) {
//		GridTile* ptr = m_grid[t_point];
//
//		int col1, col2, col3;
//		col1 = rand() % 255;
//		col2 = rand() % 255;
//		col3 = rand() % 255;
//
//
//		//add all nodes with previous to the path
//		while (nullptr != ptr->getPrevious())
//		{
//			addLine(ptr->getPos(), ptr->getPrevious()->getPos(), sf::Color(col1, col2, col3, 255));
//
//			ptr = ptr->getPrevious();
//		}
//	}
//}
//
//void RenderGrid::setupRectCorners(std::vector<int>& t_rectCorners)
//{
//	//sort corner indexes so smallest index is top left and highest is bottom right
//	std::sort(t_rectCorners.begin(), t_rectCorners.end());
//
//	//set the corners for to be ordered in order of TopLeft,TopRight,BotLeft,BotRight
//	int topLeft = t_rectCorners[0], topRight = t_rectCorners[1], botLeft = t_rectCorners[2], botRight = t_rectCorners[3];
//
//	if (m_grid[topRight]->getRow() == m_grid[botLeft]->getRow())
//	{
//		std::swap(t_rectCorners[1], t_rectCorners[2]);
//	}
//}
//
//void RenderGrid::init(float t_textOffset, const sf::Vector2f& t_textPos, int t_charSize)
//{
//	//use height of the window to make squares as the right side of the screen is used for tooltip info
//	m_tileSize.y = static_cast<float>(m_window.getSize().y) / static_cast<float>(NO_OF_ROWS);
//	m_tileSize.x = m_tileSize.y;
//	//m_tileSize = sf::Vector2f(1.0f, 1.0f);
//	m_grid.reserve(MAX_TILES);
//	m_searchNodes.reserve(MAX_TILES);
//
//	for (int i = 0; i < NO_OF_ROWS; i++)
//	{
//		for (int j = 0; j < TILES_PER_ROW; j++)
//		{
//			int tileIndex = j + i * TILES_PER_ROW;
//			GridTile* tile = new GridTile(
//				sf::Vector2f(j * m_tileSize.x + (m_tileSize.x / 2.0f),
//					i * m_tileSize.y + (m_tileSize.y / 2.0f)),
//				m_tileSize,
//				tileIndex,
//				m_font
//			);
//
//			try
//			{
//				m_grid.push_back(tile);
//			}
//			catch (const std::exception&)
//			{
//				delete tile;
//				throw;
//			}
//		}
//	}
//
//	initText(t_textOffset, t_textPos, t_charSize);
//
//	if (MAX_TILES == TEST_LAYOUT)
//	{
//		setTestLayout();
//	}
//}
//
//bool RenderGrid::reaAlgorithm()
//{
//	if (m_debug)
//		std::cout << "Starting REA*..." << std::endl;
//
//	if (insertS())
//	{
//		//goal has been found, backtrack
//		return true;
//	}
//
//	SearchNode* currentBestNode;
//	while (!m_openlist.empty())
//	{
//		//current best node CBN
//		currentBestNode = m_openlist.top();
//		m_openlist.pop();
//
//		if (expand(currentBestNode))
//		{
//			//goal has been found!
//
//			if (m_debug)
//			{
//				std::cout << "Expand returned true!" << std::endl;
//			}
//			return true;
//		}
//	}
//
//	std::cout << "Failed to find a path to the goal!" << std::endl;
//	return false;
//}
//
//double RenderGrid::getOctileDist(sf::Vector2f t_p1, sf::Vector2f t_p2)
//{
//	float dx = std::abs(t_p1.x - t_p2.x);
//	float dy = std::abs(t_p1.y - t_p2.y);
//	return Utils::DIAGONAL * std::min(dx, dy) + std::abs(dx - dy);
//}
//
//double RenderGrid::getOctileDist(sf::Vector2i t_p1, sf::Vector2i t_p2)
//{
//	int dx = std::abs(t_p1.x - t_p2.x);
//	int dy = std::abs(t_p1.y - t_p2.y);
//	return Utils::DIAGONAL * std::min(dx, dy) + std::abs(dx - dy);
//}
//
///// <summary>
///// Function to expand a rectangle in a given direction from a origin index
///// </summary>
///// <param name="t_rectBoundary">vector container for 4 corners of the rect IF goal is not found</param>
///// <param name="t_direction">direction to expand the rectangle in</param>
///// <param name="t_origin">origin index to expand from</param>
///// <returns>true if goal has been found in rectangle, false if not</returns>
//bool RenderGrid::getRectInDirection(std::vector<int>& t_rectBoundary, NeighbourIndex& t_direction, int& t_origin, bool t_expandOpposite, int t_sideLimit1, int t_sideLimit2)
//{
//	int indexInDirection = -1;
//	int limitInDirection = -1;
//	int sideLimit1 = t_sideLimit1, sideLimit2 = t_sideLimit2;
//	bool goalFound = false;
//
//	if (sideLimit1 > -1 && t_sideLimit2 > -1)
//	{
//		indexInDirection = getNeighbourIndex(t_direction, t_origin);
//	}
//	else
//	{
//		indexInDirection = t_origin;
//	}
//
//	//if there is nothing above the starting point
//	if (indexInDirection >= 0 && m_grid[indexInDirection]->getType() != GridTile::TileType::Obstacle && m_grid[indexInDirection]->m_mode == GridTile::ReaMode::None && m_grid[indexInDirection]->getType() != GridTile::TileType::Goal)
//	{
//		//expand in the given direction
//		int tempIndex = indexInDirection;
//		while (tempIndex >= 0 && m_grid[tempIndex]->getType() != GridTile::TileType::Obstacle && !goalFound)
//		{
//			//we're looping again so we have a new top index
//			indexInDirection = tempIndex;
//			if (m_grid[indexInDirection]->getType() != GridTile::TileType::Goal)
//			{
//				switch (t_direction)
//				{
//				case NeighbourIndex::LEFT:
//				case NeighbourIndex::RIGHT:
//					limitInDirection = m_grid[tempIndex]->getCol();
//					sideLimit1 = getSideBoundary(Utils::TOP, tempIndex, sideLimit1, goalFound, t_origin);
//					sideLimit2 = getSideBoundary(Utils::BOTTOM, tempIndex, sideLimit2, goalFound, t_origin);
//					break;
//				case NeighbourIndex::TOP:
//				case NeighbourIndex::BOTTOM:
//					limitInDirection = m_grid[tempIndex]->getRow();
//					sideLimit1 = getSideBoundary(Utils::LEFT, tempIndex, sideLimit1, goalFound, t_origin);
//					sideLimit2 = getSideBoundary(Utils::RIGHT, tempIndex, sideLimit2, goalFound, t_origin);
//					break;
//				default:
//					throw std::invalid_argument("received bad direction value");
//					break;
//				}
//				if ((t_sideLimit1 == -1 && t_sideLimit2 == -1) || (sideLimit1 == t_sideLimit1 && sideLimit2 == t_sideLimit2))
//				{
//					tempIndex = getNeighbourIndex(t_direction, tempIndex);
//				}
//				else
//				{
//					switch (t_direction)
//					{
//					case NeighbourIndex::LEFT:
//						tempIndex = getNeighbourIndex(Utils::RIGHT, tempIndex);
//						limitInDirection = m_grid[tempIndex]->getCol();
//						sideLimit1 = t_sideLimit1;
//						sideLimit2 = t_sideLimit2;
//						break;
//					case NeighbourIndex::RIGHT:
//						tempIndex = getNeighbourIndex(Utils::LEFT, tempIndex);
//						limitInDirection = m_grid[tempIndex]->getCol();
//						sideLimit1 = t_sideLimit1;
//						sideLimit2 = t_sideLimit2;
//						break;
//					case NeighbourIndex::TOP:
//						tempIndex = getNeighbourIndex(Utils::BOTTOM, tempIndex);
//						limitInDirection = m_grid[tempIndex]->getRow();
//						sideLimit1 = t_sideLimit1;
//						sideLimit2 = t_sideLimit2;
//						break;
//					case NeighbourIndex::BOTTOM:
//						tempIndex = getNeighbourIndex(Utils::TOP, tempIndex);
//						limitInDirection = m_grid[tempIndex]->getRow();
//						sideLimit1 = t_sideLimit1;
//						sideLimit2 = t_sideLimit2;
//						break;
//					default:
//						throw std::invalid_argument("received bad direction value");
//						break;
//					}
//					break;
//				}
//			}
//			else
//			{
//				goalFound = true;
//
//				//since goal has been immediately found, just set the limits to where the goal is
//				switch (t_direction)
//				{
//				case NeighbourIndex::LEFT:
//				case NeighbourIndex::RIGHT:
//					limitInDirection = m_grid[tempIndex]->getCol();
//					sideLimit1 = m_grid[m_goalIndex]->getRow();
//					sideLimit2 = m_grid[m_goalIndex]->getRow();
//					break;
//				case NeighbourIndex::TOP:
//				case NeighbourIndex::BOTTOM:
//					limitInDirection = m_grid[tempIndex]->getRow();
//					sideLimit1 = m_grid[m_goalIndex]->getCol();
//					sideLimit2 = m_grid[m_goalIndex]->getCol();
//					break;
//				default:
//					break;
//				}
//				//break out of the loop
//				break;
//			}
//		}
//	}
//	else if (indexInDirection >= 0 && m_grid[indexInDirection]->getType() == GridTile::TileType::Goal)
//	{
//		goalFound = true;
//		if (!m_grid[m_goalIndex]->getPrevious())
//		{
//			m_grid[m_goalIndex]->setPrevious(m_grid[t_origin]);
//		}
//	}
//	else
//	{
//		//we cant expand in that direction, just expand sideways to form a rectangle 1 tall/wide
//		switch (t_direction)
//		{
//		case NeighbourIndex::LEFT:
//		case NeighbourIndex::RIGHT:
//		{
//			limitInDirection = m_grid[t_origin]->getCol();
//			if (sideLimit1 == -1)
//			{
//				sideLimit1 = getSideBoundary(Utils::TOP, t_origin, sideLimit1, goalFound, t_origin);
//			}
//			if (sideLimit2 == -1)
//			{
//				sideLimit2 = getSideBoundary(Utils::BOTTOM, t_origin, sideLimit2, goalFound, t_origin);
//			}
//			break;
//		}
//		case NeighbourIndex::TOP:
//		case NeighbourIndex::BOTTOM:
//			limitInDirection = m_grid[t_origin]->getRow();
//			if (sideLimit1 == -1)
//			{
//				sideLimit1 = getSideBoundary(Utils::LEFT, t_origin, sideLimit1, goalFound, t_origin);
//			}
//			if (sideLimit2 == -1)
//			{
//				sideLimit2 = getSideBoundary(Utils::RIGHT, t_origin, sideLimit2, goalFound, t_origin);
//			}
//			break;
//		default:
//			throw std::invalid_argument("received bad direction value");
//			break;
//		}
//	}
//
//	int corner1, corner2;
//	switch (t_direction)
//	{
//	case NeighbourIndex::LEFT:
//	case NeighbourIndex::RIGHT:
//		corner1 = limitInDirection + (sideLimit1 * TILES_PER_ROW);
//		corner2 = limitInDirection + (sideLimit2 * TILES_PER_ROW);
//		break;
//	case NeighbourIndex::TOP:
//	case NeighbourIndex::BOTTOM:
//		corner1 = sideLimit1 + (limitInDirection * TILES_PER_ROW);
//		corner2 = sideLimit2 + (limitInDirection * TILES_PER_ROW);
//		break;
//	default:
//		throw std::invalid_argument("received bad direction value");
//		break;
//	}
//	t_rectBoundary.emplace_back(corner1);
//	t_rectBoundary.emplace_back(corner2);
//
//	if (goalFound)
//	{
//		corner1 = m_goalIndex;
//		corner2 = m_goalIndex;
//
//		if (!m_grid[m_goalIndex]->getPrevious())
//		{
//			m_grid[m_goalIndex]->setPrevious(m_grid[t_origin]);
//		}
//	}
//	if (corner1 < 0 && corner2 < 0)
//	{
//		throw std::invalid_argument("one of the corners is an invalid index!");
//	}
//
//	if (!goalFound)
//	{
//		if (t_expandOpposite)
//		{
//			NeighbourIndex oppositeDirection;
//			int startPointIndex = t_origin;
//			switch (t_direction)
//			{
//			case NeighbourIndex::LEFT:
//				oppositeDirection = NeighbourIndex::RIGHT;
//				break;
//			case NeighbourIndex::RIGHT:
//				oppositeDirection = NeighbourIndex::LEFT;
//				break;
//			case NeighbourIndex::TOP:
//				oppositeDirection = NeighbourIndex::BOTTOM;
//				break;
//			case NeighbourIndex::BOTTOM:
//				oppositeDirection = NeighbourIndex::TOP;
//				break;
//			default:
//				throw std::invalid_argument("received bad direction value");
//				break;
//			}
//			getRectInOpposite(t_rectBoundary, oppositeDirection, startPointIndex, sideLimit1, sideLimit2, goalFound);
//		}
//		else
//		{
//			int corner3, corner4;
//			switch (t_direction)
//			{
//			case NeighbourIndex::LEFT:
//			case NeighbourIndex::RIGHT:
//				corner3 = m_grid[t_origin]->getCol() + (sideLimit1 * TILES_PER_ROW);
//				corner4 = m_grid[t_origin]->getCol() + (sideLimit2 * TILES_PER_ROW);
//				break;
//			case NeighbourIndex::TOP:
//			case NeighbourIndex::BOTTOM:
//				corner3 = sideLimit1 + (m_grid[t_origin]->getRow() * TILES_PER_ROW);
//				corner4 = sideLimit2 + (m_grid[t_origin]->getRow() * TILES_PER_ROW);
//				break;
//			default:
//				throw std::invalid_argument("received bad direction value");
//				break;
//			}
//			t_rectBoundary.emplace_back(corner3);
//			t_rectBoundary.emplace_back(corner4);
//		}
//	}
//	else if (m_grid[m_goalIndex]->getPrevious() == nullptr)
//	{
//		m_grid[m_goalIndex]->setPrevious(m_grid[t_origin]);
//	}
//	if (!goalFound)
//	{
//		markBorderers(t_rectBoundary);
//	}
//	return goalFound;
//}
//
//void RenderGrid::getRectInOpposite(std::vector<int>& t_rectBoundary, NeighbourIndex& t_direction, int& t_origin, int& t_limit1, int& t_limit2, bool& t_goalFound)
//{
//	int indexInDirection = t_origin;
//	int limitInDirection = -1;
//	int sideLimit1 = t_limit1, sideLimit2 = t_limit2;
//	int oppositeLimit1 = -1, oppositeLimit2 = -1;
//	bool breakout = false;
//
//	//indexInDirection = getNeighbourIndex(t_direction, t_origin);
//	//if there is nothing above the starting point
//	if (indexInDirection >= 0 && m_grid[indexInDirection]->getType() != GridTile::TileType::Obstacle)
//	{
//		//expand in the given direction
//		int tempIndex = indexInDirection;
//		while (tempIndex >= 0 && m_grid[tempIndex]->getType() != GridTile::TileType::Obstacle && !breakout && !t_goalFound)
//		{
//			//if tile at tempIndex is a Goal tile, breakout
//			if (m_grid[tempIndex]->getType() == GridTile::TileType::Goal)
//			{
//				t_goalFound = true;
//				breakout = true;
//				break;
//			}
//			else
//			{
//				//we're looping again so we have a new top index
//				indexInDirection = tempIndex;
//				int tempLimit1 = -1, tempLimit2 = -1;
//				switch (t_direction)
//				{
//				case NeighbourIndex::LEFT:
//				case NeighbourIndex::RIGHT:
//				{
//					tempLimit1 = getSideBoundary(Utils::TOP, tempIndex, sideLimit1, t_goalFound, t_origin);
//					tempLimit2 = getSideBoundary(Utils::BOTTOM, tempIndex, sideLimit2, t_goalFound, t_origin);
//
//					if (tempLimit1 > sideLimit1)
//					{
//						breakout = true;
//					}
//					if (tempLimit2 < sideLimit2)
//					{
//						breakout = true;
//					}
//					if (!breakout)
//					{
//						limitInDirection = m_grid[tempIndex]->getCol();
//					}
//					break;
//				}
//				case NeighbourIndex::TOP:
//				case NeighbourIndex::BOTTOM:
//				{
//					tempLimit1 = getSideBoundary(Utils::LEFT, tempIndex, sideLimit1, t_goalFound, t_origin);
//					tempLimit2 = getSideBoundary(Utils::RIGHT, tempIndex, sideLimit2, t_goalFound, t_origin);
//
//					if (tempLimit1 > sideLimit1)
//					{
//						breakout = true;
//					}
//					if (tempLimit2 < sideLimit2)
//					{
//						breakout = true;
//					}
//					if (!breakout)
//					{
//						limitInDirection = m_grid[tempIndex]->getRow();
//					}
//					break;
//				}
//				default:
//					throw std::invalid_argument("received bad direction value");
//					break;
//				}
//
//				if (!breakout)
//				{
//					tempIndex = getNeighbourIndex(t_direction, tempIndex);
//					if (tempLimit1 > -1 && tempLimit2 > -1)
//					{
//						oppositeLimit1 = tempLimit1;
//						oppositeLimit2 = tempLimit2;
//					}
//				}
//			}
//		}
//	}
//
//	int corner1, corner2, tempSideLimit1, tempSideLimit2/*, tempDirectionLimit*/;
//	if (oppositeLimit1 >= 0 && oppositeLimit2 >= 0)
//	{
//		tempSideLimit1 = oppositeLimit1;
//		tempSideLimit2 = oppositeLimit2;
//	}
//	else
//	{
//		tempSideLimit1 = sideLimit1;
//		tempSideLimit2 = sideLimit2;
//	}
//	switch (t_direction)
//	{
//	case NeighbourIndex::LEFT:
//	case NeighbourIndex::RIGHT:
//		corner1 = limitInDirection + (tempSideLimit1 * TILES_PER_ROW);
//		corner2 = limitInDirection + (tempSideLimit2 * TILES_PER_ROW);
//		break;
//	case NeighbourIndex::TOP:
//	case NeighbourIndex::BOTTOM:
//		corner1 = tempSideLimit1 + (limitInDirection * TILES_PER_ROW);
//		corner2 = tempSideLimit2 + (limitInDirection * TILES_PER_ROW);
//		break;
//	default:
//		throw std::invalid_argument("received bad direction value");
//		break;
//	}
//	t_rectBoundary.emplace_back(corner1);
//	t_rectBoundary.emplace_back(corner2);
//
//	if (corner1 < 0 && corner2 < 0)
//	{
//		throw std::invalid_argument("one of the corners is an invalid index!");
//	}
//
//	if (t_goalFound && m_grid[m_goalIndex]->getPrevious() == nullptr)
//	{
//		m_grid[m_goalIndex]->setPrevious(m_grid[t_origin]);
//	}
//}
//
//int RenderGrid::getSideBoundary(NeighbourIndex& t_direction, int& t_expandOrigin, int& t_currentLimit, bool& t_goalFound, int& t_rectOrigin)
//{
//	//expand in the given direction
//	bool expand = true;
//	int tempDirectionIndex = t_expandOrigin;
//	int tempPreviousDirectionIndex = t_expandOrigin;
//	int limit = t_currentLimit;
//
//	//loop while (expand = true AND t_goalFound = false)
//	while (expand && !t_goalFound)
//	{
//		tempDirectionIndex = getNeighbourIndex(t_direction, tempDirectionIndex);
//		if (tempDirectionIndex >= 0 && m_grid[tempDirectionIndex]->getType() != GridTile::TileType::Obstacle && m_grid[tempDirectionIndex]->m_mode == GridTile::ReaMode::None)
//		{
//			//if limit is already set
//			if (limit >= 0)
//			{
//				switch (t_direction)
//				{
//				case NeighbourIndex::LEFT:
//				{
//					if (m_grid[tempPreviousDirectionIndex]->getCol() <= limit)
//					{
//						return limit;
//					}
//					break;
//				}
//				case NeighbourIndex::RIGHT:
//				{
//					if (m_grid[tempPreviousDirectionIndex]->getCol() >= limit)
//					{
//						return limit;
//					}
//					break;
//				}
//				case NeighbourIndex::TOP:
//				{
//					if (m_grid[tempPreviousDirectionIndex]->getRow() <= limit)
//					{
//						return limit;
//					}
//					break;
//				}
//				case NeighbourIndex::BOTTOM:
//				{
//					if (m_grid[tempPreviousDirectionIndex]->getRow() >= limit)
//					{
//						return limit;
//					}
//					break;
//				}
//				default:
//					break;
//				}
//			}
//			//if we got here, keep moving and try to find first limit
//			tempPreviousDirectionIndex = tempDirectionIndex;//set previous valid tile
//			//if tile we just checked is the goal
//			if (m_grid[tempPreviousDirectionIndex]->getType() == GridTile::TileType::Goal)
//			{
//				//stop looping
//				t_goalFound = true;
//				expand = false;
//			}
//		}
//		else
//		{
//			expand = false; // stop expanding
//
//			//if limit is not set
//			if (limit >= 0)
//			{
//				switch (t_direction)
//				{
//				case NeighbourIndex::LEFT:
//				{
//					if (m_grid[tempPreviousDirectionIndex]->getCol() > limit)
//					{
//						limit = m_grid[tempPreviousDirectionIndex]->getCol();
//						return limit;
//					}
//					break;
//				}
//				case NeighbourIndex::RIGHT:
//				{
//					if (m_grid[tempPreviousDirectionIndex]->getCol() < limit)
//					{
//						limit = m_grid[tempPreviousDirectionIndex]->getCol();
//						return limit;
//					}
//					break;
//				}
//				case NeighbourIndex::TOP:
//				{
//					if (m_grid[tempPreviousDirectionIndex]->getRow() > limit)
//					{
//						limit = m_grid[tempPreviousDirectionIndex]->getRow();
//						return limit;
//					}
//					break;
//				}
//				case NeighbourIndex::BOTTOM:
//				{
//					if (m_grid[tempPreviousDirectionIndex]->getRow() < limit)
//					{
//						limit = m_grid[tempPreviousDirectionIndex]->getRow();
//						return limit;
//					}
//					break;
//				}
//				default:
//					throw std::invalid_argument("received bad direction value");
//					break;
//				}
//			}
//		}
//	}
//
//	//incase limit is unset, set it
//	if (limit < 0)
//	{
//		//here tempPreviousDirectionIndex is either
//		//equal to t_expandOrigin
//		//or a new valid index for a limit
//		switch (t_direction)
//		{
//		case NeighbourIndex::LEFT:
//		case NeighbourIndex::RIGHT:
//			limit = m_grid[tempPreviousDirectionIndex]->getCol();
//			break;
//		case NeighbourIndex::TOP:
//		case NeighbourIndex::BOTTOM:
//			limit = m_grid[tempPreviousDirectionIndex]->getRow();
//			break;
//		default:
//			throw std::invalid_argument("received bad direction value");
//			break;
//		}
//	}
//
//	return limit;
//}
//
//void RenderGrid::markBorderers(std::vector<int>& t_rectBorder)
//{
//	setupRectCorners(t_rectBorder);
//	int offset = 1;
//
//	int index = t_rectBorder[0];
//	while (index <= t_rectBorder[1]/* + offset*/)
//	{
//		m_grid[index]->setToCorner();
//
//		index += offset;
//	}
//
//	index = t_rectBorder[2];
//	while (index <= t_rectBorder[3]/* + offset*/)
//	{
//		m_grid[index]->setToCorner();
//
//		index += offset;
//	}
//
//	index = t_rectBorder[0];
//	offset = TILES_PER_ROW;
//	while (index <= t_rectBorder[2]/* + offset*/)
//	{
//		m_grid[index]->setToCorner();
//
//		index += offset;
//	}
//
//	index = t_rectBorder[1];
//	offset = TILES_PER_ROW;
//	while (index <= t_rectBorder[3]/* + offset*/)
//	{
//		m_grid[index]->setToCorner();
//
//		index += offset;
//	}
//}
//
///// <summary>
///// Tries to update point if better values are being passed in
///// </summary>
///// <param name="t_point">index of a point to try to update</param>
///// <param name="t_dir">direction of the FSI</param>
///// <returns>true if point has been updated, false otherwise</returns>
//bool RenderGrid::tryToUpdateFsiPoint(int& t_point, NeighbourIndex& t_dir)
//{
//	GridTile* previous = nullptr;
//	NeighbourIndex nextFsiDir;
//	NeighbourIndex nextFsiDiagDir;
//
//	NeighbourIndex prevFsiDir;
//	NeighbourIndex prevFsiDiagDir;
//
//	NeighbourIndex cardinalParentDir;
//
//	switch (t_dir)
//	{
//	case NeighbourIndex::LEFT:
//		nextFsiDir = NeighbourIndex::BOTTOM;
//		nextFsiDiagDir = NeighbourIndex::BOTTOM_RIGHT;
//
//		prevFsiDir = NeighbourIndex::TOP;
//		prevFsiDiagDir = NeighbourIndex::TOP_RIGHT;
//
//		cardinalParentDir = NeighbourIndex::RIGHT;
//		break;
//	case NeighbourIndex::RIGHT:
//		nextFsiDir = NeighbourIndex::BOTTOM;
//		nextFsiDiagDir = NeighbourIndex::BOTTOM_LEFT;
//
//		prevFsiDir = NeighbourIndex::TOP;
//		prevFsiDiagDir = NeighbourIndex::TOP_LEFT;
//
//		cardinalParentDir = NeighbourIndex::LEFT;
//		break;
//	case NeighbourIndex::TOP:
//		nextFsiDir = NeighbourIndex::RIGHT;
//		nextFsiDiagDir = NeighbourIndex::BOTTOM_RIGHT;
//
//		prevFsiDir = NeighbourIndex::LEFT;
//		prevFsiDiagDir = NeighbourIndex::BOTTOM_LEFT;
//
//		cardinalParentDir = NeighbourIndex::BOTTOM;
//		break;
//	case NeighbourIndex::BOTTOM:
//		nextFsiDir = NeighbourIndex::RIGHT;
//		nextFsiDiagDir = NeighbourIndex::TOP_RIGHT;
//
//		prevFsiDir = NeighbourIndex::LEFT;
//		prevFsiDiagDir = NeighbourIndex::TOP_LEFT;
//
//		cardinalParentDir = NeighbourIndex::TOP;
//		break;
//	default:
//		break;
//	}
//
//	//indexes for different neighbours
//	int nextFsiIndex = getNeighbourIndex(nextFsiDir, t_point);
//	int nextFsiDiagIndex = getNeighbourIndex(nextFsiDiagDir, t_point);
//
//	int prevFsiIndex = getNeighbourIndex(prevFsiDir, t_point);
//	int prevFsiDiagIndex = getNeighbourIndex(prevFsiDiagDir, t_point);
//
//	int cardinalIndex = getNeighbourIndex(cardinalParentDir, t_point);
//	double lowestGval = std::numeric_limits<int>::max();
//
//	if (nextFsiIndex != -1 && nextFsiDiagIndex != -1 &&
//		m_grid[nextFsiIndex]->getType() != GridTile::TileType::Obstacle &&
//		m_grid[nextFsiDiagIndex]->getType() != GridTile::TileType::Obstacle)
//	{
//		lowestGval = m_grid[nextFsiDiagIndex]->m_gval + Utils::DIAGONAL;
//		previous = m_grid[nextFsiDiagIndex];
//	}
//
//	if (prevFsiIndex != -1 && prevFsiDiagIndex != -1 &&
//		m_grid[prevFsiIndex]->getType() != GridTile::TileType::Obstacle &&
//		m_grid[prevFsiDiagIndex]->getType() != GridTile::TileType::Obstacle &&
//		lowestGval > m_grid[prevFsiDiagIndex]->m_gval + Utils::DIAGONAL)
//	{
//		lowestGval = m_grid[prevFsiDiagIndex]->m_gval + Utils::DIAGONAL;
//		previous = m_grid[prevFsiDiagIndex];
//	}
//
//	if (lowestGval > m_grid[cardinalIndex]->m_gval + 1.0f)
//	{
//		lowestGval = m_grid[cardinalIndex]->m_gval + 1.0f;
//		previous = m_grid[cardinalIndex];
//	}
//
//	if (previous && m_grid[t_point]->m_gval > lowestGval)
//	{
//		m_grid[t_point]->setPrevious(previous);
//		m_grid[t_point]->m_gval = lowestGval;
//		return true;
//	}
//	return false;
//}
//
//bool RenderGrid::fsiSpecialCasePoint(int& t_point, BoundaryNode& t_boundary)
//{
//	NeighbourIndex directionCheck;
//	switch (t_boundary.m_dir)
//	{
//	case NeighbourIndex::LEFT:
//		directionCheck = NeighbourIndex::RIGHT;
//		break;
//	case NeighbourIndex::RIGHT:
//		directionCheck = NeighbourIndex::LEFT;
//		break;
//	case NeighbourIndex::TOP:
//		directionCheck = NeighbourIndex::BOTTOM;
//		break;
//	case NeighbourIndex::BOTTOM:
//		directionCheck = NeighbourIndex::TOP;
//		break;
//	default:
//		break;
//	}
//
//	//boundary corners need to check for diagonals.
//	if (t_boundary.m_boundary[0] == getNeighbourIndex(directionCheck, t_point) ||
//		t_boundary.m_boundary[t_boundary.m_boundary.size() - 1] == getNeighbourIndex(directionCheck, t_point))//check if this is a special case
//	{
//		return true;
//	}
//
//	return false;
//}
//
//bool RenderGrid::checkIfWithinRect(int& t_point, int& t_topLeft, int& t_botRight)
//{
//	if (m_grid[t_point]->getCol() < m_grid[t_topLeft]->getCol() ||
//		m_grid[t_point]->getCol() > m_grid[t_botRight]->getCol() ||
//		m_grid[t_point]->getRow() < m_grid[t_topLeft]->getRow() ||
//		m_grid[t_point]->getRow() > m_grid[t_botRight]->getRow())
//	{
//		return false;
//	}
//	return true;;
//}
//
//bool RenderGrid::tryToUpdateSpecialCaseFsiPoint(int& t_point, NeighbourIndex& t_dir, bool t_startPoint)
//{
//	bool updated = false;
//	GridTile* previous = nullptr;
//	NeighbourIndex fsiNeighbourDir;
//	NeighbourIndex fsiNeighbourDiagDir;
//
//	NeighbourIndex cardinalParentDir;
//
//	switch (t_dir)
//	{
//	case NeighbourIndex::LEFT:
//	{
//		cardinalParentDir = NeighbourIndex::RIGHT;
//		if (t_startPoint)
//		{
//			fsiNeighbourDir = NeighbourIndex::BOTTOM;
//			fsiNeighbourDiagDir = NeighbourIndex::BOTTOM_RIGHT;
//		}
//		else
//		{
//			fsiNeighbourDir = NeighbourIndex::TOP;
//			fsiNeighbourDiagDir = NeighbourIndex::TOP_RIGHT;
//		}
//		break;
//	}
//	case NeighbourIndex::RIGHT:
//	{
//		cardinalParentDir = NeighbourIndex::LEFT;
//		if (t_startPoint)
//		{
//			fsiNeighbourDir = NeighbourIndex::BOTTOM;
//			fsiNeighbourDiagDir = NeighbourIndex::BOTTOM_LEFT;
//		}
//		else
//		{
//			fsiNeighbourDir = NeighbourIndex::TOP;
//			fsiNeighbourDiagDir = NeighbourIndex::TOP_LEFT;
//		}
//		break;
//	}
//	case NeighbourIndex::TOP:
//	{
//		cardinalParentDir = NeighbourIndex::BOTTOM;
//		if (t_startPoint)
//		{
//			fsiNeighbourDir = NeighbourIndex::RIGHT;
//			fsiNeighbourDiagDir = NeighbourIndex::BOTTOM_RIGHT;
//		}
//		else
//		{
//			fsiNeighbourDir = NeighbourIndex::LEFT;
//			fsiNeighbourDiagDir = NeighbourIndex::BOTTOM_LEFT;
//		}
//		break;
//	}
//	case NeighbourIndex::BOTTOM:
//	{
//		cardinalParentDir = NeighbourIndex::TOP;
//		if (t_startPoint)
//		{
//			fsiNeighbourDir = NeighbourIndex::RIGHT;
//			fsiNeighbourDiagDir = NeighbourIndex::TOP_RIGHT;
//		}
//		else
//		{
//			fsiNeighbourDir = NeighbourIndex::LEFT;
//			fsiNeighbourDiagDir = NeighbourIndex::TOP_LEFT;
//		}
//		break;
//	}
//	default:
//		break;
//	}
//
//	int fsiNeighbourIndex = getNeighbourIndex(fsiNeighbourDir, t_point);
//	int fsiNeighbourDiagIndex = getNeighbourIndex(fsiNeighbourDiagDir, t_point);
//
//	int cardinalIndex = getNeighbourIndex(cardinalParentDir, t_point);
//
//	//check if both indexes are valid and then if so, are both of them not obstacles 
//	if (fsiNeighbourIndex != -1 && fsiNeighbourDiagIndex != -1 &&
//		m_grid[fsiNeighbourIndex]->getType() != GridTile::TileType::Obstacle &&
//		m_grid[fsiNeighbourDiagIndex]->getType() != GridTile::TileType::Obstacle)
//	{
//		if (m_grid[cardinalIndex]->m_gval + 1.0f > m_grid[fsiNeighbourDiagIndex]->m_gval + Utils::DIAGONAL &&
//			m_grid[t_point]->m_gval > m_grid[fsiNeighbourDiagIndex]->m_gval + Utils::DIAGONAL)
//		{
//			updated = true;
//			previous = m_grid[fsiNeighbourDiagIndex];
//			m_grid[t_point]->m_gval = m_grid[fsiNeighbourDiagIndex]->m_gval + Utils::DIAGONAL;
//		}
//	}
//	//this tile has to be valid here
//	if (m_grid[t_point]->m_gval > m_grid[cardinalIndex]->m_gval + 1.0f) //hard coded distance between cardinal neighbour tiles
//	{
//		updated = true;
//		previous = m_grid[cardinalIndex];
//		m_grid[t_point]->m_gval = m_grid[cardinalIndex]->m_gval + 1.0f;
//	}
//
//	if (updated && previous)
//	{
//		m_grid[t_point]->setPrevious(previous);
//	}
//	return updated;
//}
//
//bool RenderGrid::tryToUpdateDiagonalFsiPoint(int& t_point, NeighbourIndex& t_dir, bool t_startPoint)
//{
//	bool updated = false;
//	NeighbourIndex fsiNeighbourCheck;
//	NeighbourIndex diagonalParentCheck;
//	NeighbourIndex cardinalParentCheck;
//	GridTile* previous = nullptr;
//
//	switch (t_dir)
//	{
//	case NeighbourIndex::LEFT:
//	{
//		cardinalParentCheck = NeighbourIndex::RIGHT;
//		if (t_startPoint)
//		{
//			fsiNeighbourCheck = NeighbourIndex::BOTTOM;
//			diagonalParentCheck = NeighbourIndex::BOTTOM_RIGHT;
//		}
//		else
//		{
//			fsiNeighbourCheck = NeighbourIndex::TOP;
//			diagonalParentCheck = NeighbourIndex::TOP_RIGHT;
//		}
//		break;
//	}
//	case NeighbourIndex::RIGHT:
//	{
//		cardinalParentCheck = NeighbourIndex::LEFT;
//		if (t_startPoint)
//		{
//			fsiNeighbourCheck = NeighbourIndex::BOTTOM;
//			diagonalParentCheck = NeighbourIndex::BOTTOM_LEFT;
//		}
//		else
//		{
//			fsiNeighbourCheck = NeighbourIndex::TOP;
//			diagonalParentCheck = NeighbourIndex::TOP_LEFT;
//		}
//		break;
//	}
//	case NeighbourIndex::TOP:
//	{
//		cardinalParentCheck = NeighbourIndex::BOTTOM;
//		if (t_startPoint)
//		{
//			fsiNeighbourCheck = NeighbourIndex::RIGHT;
//			diagonalParentCheck = NeighbourIndex::BOTTOM_RIGHT;
//		}
//		else
//		{
//			fsiNeighbourCheck = NeighbourIndex::LEFT;
//			diagonalParentCheck = NeighbourIndex::BOTTOM_LEFT;
//		}
//		break;
//	}
//	case NeighbourIndex::BOTTOM:
//	{
//		cardinalParentCheck = NeighbourIndex::TOP;
//		if (t_startPoint)
//		{
//			fsiNeighbourCheck = NeighbourIndex::RIGHT;
//			diagonalParentCheck = NeighbourIndex::TOP_RIGHT;
//		}
//		else
//		{
//			fsiNeighbourCheck = NeighbourIndex::LEFT;
//			diagonalParentCheck = NeighbourIndex::TOP_LEFT;
//		}
//		break;
//	}
//	default:
//		break;
//	}
//
//	int fsiNeighbour = getNeighbourIndex(fsiNeighbourCheck, t_point);
//	int diagonalParent = getNeighbourIndex(diagonalParentCheck, t_point);
//	int cardinalParent = getNeighbourIndex(cardinalParentCheck, t_point);
//
//	if (fsiNeighbour > -1 && diagonalParent > -1 && cardinalParent > -1 &&
//		m_grid[fsiNeighbour]->getType() != GridTile::TileType::Obstacle &&
//		m_grid[diagonalParent]->getType() != GridTile::TileType::Obstacle &&
//		m_grid[cardinalParent]->getType() != GridTile::TileType::Obstacle)
//	{
//		if (m_grid[t_point]->m_gval > m_grid[diagonalParent]->m_gval + Utils::DIAGONAL)
//		{
//			updated = true;
//			previous = m_grid[diagonalParent];
//			m_grid[t_point]->m_gval = m_grid[diagonalParent]->m_gval + Utils::DIAGONAL;
//		}
//	}
//	if (m_grid[t_point]->m_gval > m_grid[fsiNeighbour]->m_gval + 1.0f) //hardcoded cardinal length between tiles
//	{
//		updated = true;
//		previous = m_grid[fsiNeighbour];
//		m_grid[t_point]->m_gval = m_grid[fsiNeighbour]->m_gval + 1.0f;
//	}
//
//	if (updated && previous)
//	{
//		m_grid[t_point]->setPrevious(previous);
//	}
//	return updated;
//}
//
//void RenderGrid::tryToUpdateSideBoundaryPoint(int& t_point, int& t_cardinalPoint, int& t_diagonalPoint, double& t_cardinalLen, double& t_diagLen, SearchNode* t_cbn, NeighbourIndex& t_boundaryDir)
//{
//	bool updatedCardinal = false;
//	GridTile* previous = nullptr;/*m_grid[t_point]->getPrevious();*/
//	if (t_diagonalPoint > -1 && t_point != t_diagonalPoint)
//	{
//		if (m_grid[t_point]->m_gval > m_grid[t_diagonalPoint]->m_gval + t_diagLen)
//		{
//			previous = m_grid[t_diagonalPoint];
//			m_grid[t_point]->m_gval = m_grid[t_diagonalPoint]->m_gval + t_diagLen;
//		}
//	}
//	if (t_cardinalPoint > -1 && t_point != t_cardinalPoint && m_grid[t_point]->m_gval > m_grid[t_cardinalPoint]->m_gval + t_cardinalLen)
//	{
//		previous = m_grid[t_cardinalPoint];
//		m_grid[t_point]->m_gval = m_grid[t_cardinalPoint]->m_gval + t_cardinalLen;
//		updatedCardinal = true;
//	}
//
//	if (updatedCardinal && previous)
//	{
//		bool looping = true;
//
//		while (previous && looping)
//		{
//			int pointIndex = previous->getIndex();
//			if (checkIfWithinRect(pointIndex, m_tempRect[0], m_tempRect[m_tempRect.size() - 1]))
//			{
//				previous = previous->getPrevious();
//			}
//			else
//			{
//				looping = false;
//			}
//		}
//	}
//	if (previous)
//	{
//		m_grid[t_point]->setPrevious(previous);
//	}
//}
//
//void RenderGrid::tryToUpdateOppositeBoundaryPoint(int& t_point, int& t_p1, int& t_p2, double& t_octileP1, double& t_octileP2)
//{
//	GridTile* previous = nullptr;
//
//	if (t_p1 > -1 && t_point != t_p1 &&
//		m_grid[t_point]->m_gval > m_grid[t_p1]->m_gval + t_octileP1)
//	{
//		previous = m_grid[t_p1];
//		m_grid[t_point]->m_gval = m_grid[t_p1]->m_gval + t_octileP1;
//	}
//	if (t_p2 > -1 && t_point != t_p2 && m_grid[t_point]->m_gval > m_grid[t_p2]->m_gval + t_octileP2)
//	{
//		previous = m_grid[t_p2];
//		m_grid[t_point]->m_gval = m_grid[t_p2]->m_gval + t_octileP2;
//	}
//	if (previous)
//	{
//		m_grid[t_point]->setPrevious(previous);
//	}
//}
//
//bool RenderGrid::processBoundaries(SearchNode* t_cbn, BoundaryNode& t_sideWall1, BoundaryNode& t_sideWall2, BoundaryNode& t_oppositeWall)
//{
//	//if cbn expand direction is NORTH
//	if (t_cbn->m_dir == Utils::TOP)
//	{
//		//West Boundary
//		{
//			BoundaryNode& westBoundary = t_sideWall1;
//
//			//get west most point from t_cbn
//			int pw = t_cbn->m_interval[0];
//
//			double diagonal = Utils::DIAGONAL;
//
//			//move up one point
//			int p = pw - TILES_PER_ROW;
//
//			int pv = pw;
//
//			int pd = pw + 1;
//			if (pd > t_cbn->m_interval[t_cbn->m_interval.size() - 1])
//			{
//				pd = -1;
//			}
//
//			while (p >= westBoundary.m_boundary[0])
//			{
//				double octileVert = 1.0f;
//				double octileDiag = diagonal;
//
//				tryToUpdateSideBoundaryPoint(p, pv, pd, octileVert, octileDiag, t_cbn, westBoundary.m_dir);
//
//				if (m_grid[p]->m_mode != GridTile::ReaMode::Hpoint)
//				{
//					m_grid[p]->m_mode = GridTile::ReaMode::Gpoint;
//				}
//				diagonal += Utils::DIAGONAL;
//
//				p -= TILES_PER_ROW;
//				pv -= TILES_PER_ROW;
//
//				//this might not be correct.........
//				if (pd > -1 && pd < t_cbn->m_interval[t_cbn->m_interval.size() - 1])
//				{
//					pd += 1;
//				}
//				else
//				{
//					pd = -1;
//				}
//			}
//
//			if (successor(westBoundary))
//			{
//				return true;
//			}
//		}
//		//East Boundary
//		{
//			BoundaryNode& eastBoundary = t_sideWall2;
//
//			//get east most point from t_cbn
//			int pe = t_cbn->m_interval[t_cbn->m_interval.size() - 1];
//
//			double diagonal = Utils::DIAGONAL;
//
//			//move up one point
//			int p = pe - TILES_PER_ROW;
//
//			int pv = pe;
//
//			int pd = pe - 1;
//			if (pd < t_cbn->m_interval[0])
//			{
//				pd = -1;
//			}
//
//			while (p >= eastBoundary.m_boundary[0])
//			{
//				double octileVert = 1.0f;
//				double octileDiag = diagonal;
//
//				tryToUpdateSideBoundaryPoint(p, pv, pd, octileVert, octileDiag, t_cbn, eastBoundary.m_dir);
//
//				if (m_grid[p]->m_mode != GridTile::ReaMode::Hpoint)
//				{
//					m_grid[p]->m_mode = GridTile::ReaMode::Gpoint;
//				}
//				diagonal += Utils::DIAGONAL;
//
//				p -= TILES_PER_ROW;
//				pv -= TILES_PER_ROW;
//
//				if (pd > -1 && pd > t_cbn->m_interval[0])
//				{
//					pd -= 1;
//				}
//				else
//				{
//					pd = -1;
//				}
//			}
//
//			if (successor(eastBoundary))
//			{
//				return true;
//			}
//		}
//
//		//North Boundary
//		{
//			BoundaryNode& northBoundary = t_oppositeWall;
//
//			for (auto& p : northBoundary.m_boundary)
//			{
//				double dis = t_cbn->m_minValTile->getRow() - m_grid[p]->getRow();
//
//				int p1 = p + dis * TILES_PER_ROW;
//				int p2 = p1;
//
//				while (dis <= Utils::DIAGONAL * (t_cbn->m_minValTile->getRow() - m_grid[p]->getRow()) && (p1 > -1 || p2 > -1))
//				{
//					double octilePP1 = dis;
//					double octilePP2 = dis;
//
//					tryToUpdateOppositeBoundaryPoint(p, p1, p2, octilePP1, octilePP2);
//
//					if (m_grid[p]->m_mode != GridTile::ReaMode::Hpoint)
//					{
//						m_grid[p]->m_mode = GridTile::ReaMode::Gpoint;
//					}
//
//					dis += (Utils::DIAGONAL - 1.0f);
//					if (p1 > -1 && p1 > t_cbn->m_interval[0])
//					{
//						p1--;
//					}
//					else
//					{
//						p1 = -1;
//					}
//					if (p2 > -1 && p2 < t_cbn->m_interval[t_cbn->m_interval.size() - 1])
//					{
//						p2++;
//					}
//					else
//					{
//						p2 = -1;
//					}
//				}
//			}
//			if (successor(northBoundary))
//			{
//				return true;
//			}
//		}
//	}
//	//if cbn expand direction is SOUTH	
//	else if (t_cbn->m_dir == Utils::BOTTOM)
//	{
//		//West Boundary
//		{
//			BoundaryNode& westBoundary = t_sideWall1;
//
//			//get west most point from t_cbn
//			int pw = t_cbn->m_interval[0];
//
//			double diagonal = Utils::DIAGONAL;
//
//			//move DOWN one point
//			int p = pw + TILES_PER_ROW;
//
//			int pv = pw;
//
//			//move right one point
//			int pd = pw + 1;
//			if (pd > t_cbn->m_interval[t_cbn->m_interval.size() - 1])
//			{
//				pd = -1;
//			}
//
//			while (p <= westBoundary.m_boundary[westBoundary.m_boundary.size() - 1])
//			{
//				double octileVert = 1.0f;
//				double octileDiag = diagonal;
//
//				tryToUpdateSideBoundaryPoint(p, pv, pd, octileVert, octileDiag, t_cbn, westBoundary.m_dir);
//
//				if (m_grid[p]->m_mode != GridTile::ReaMode::Hpoint)
//				{
//					m_grid[p]->m_mode = GridTile::ReaMode::Gpoint;
//				}
//				diagonal += Utils::DIAGONAL;
//
//				p += TILES_PER_ROW;
//				pv += TILES_PER_ROW;
//
//				if (pd > -1 && pd < t_cbn->m_interval[t_cbn->m_interval.size() - 1])
//				{
//					pd += 1;
//				}
//				else
//				{
//					pd = -1;
//				}
//			}
//
//			if (successor(westBoundary))
//			{
//				return true;
//			}
//		}
//
//		//East Boundary
//		{
//			BoundaryNode& eastBoundary = t_sideWall2;
//
//			//get east most point from t_cbn
//			int pe = t_cbn->m_interval[t_cbn->m_interval.size() - 1];
//
//			double diagonal = Utils::DIAGONAL;
//
//			//move DOWN one point
//			int p = pe + TILES_PER_ROW;
//
//			int pv = pe;
//
//			//move left one point
//			int pd = pe - 1;
//			if (pd < t_cbn->m_interval[0])
//			{
//				pd = -1;
//			}
//
//			while (p <= eastBoundary.m_boundary[eastBoundary.m_boundary.size() - 1])
//			{
//				double octileVert = 1.0f;
//				double octileDiag = diagonal;
//
//				tryToUpdateSideBoundaryPoint(p, pv, pd, octileVert, octileDiag, t_cbn, eastBoundary.m_dir);
//
//				if (m_grid[p]->m_mode != GridTile::ReaMode::Hpoint)
//				{
//					m_grid[p]->m_mode = GridTile::ReaMode::Gpoint;
//				}
//				diagonal += Utils::DIAGONAL;
//
//				p += TILES_PER_ROW;
//				pv += TILES_PER_ROW;
//
//				if (pd > -1 && pd > t_cbn->m_interval[0])
//				{
//					pd -= 1;
//				}
//				else
//				{
//					pd = -1;
//				}
//			}
//
//			if (successor(eastBoundary))
//			{
//				return true;
//			}
//		}
//
//		//south boudary
//		{
//			BoundaryNode& southBoundary = t_oppositeWall;
//
//			for (auto& p : southBoundary.m_boundary)
//			{
//				double dis = m_grid[p]->getRow() - t_cbn->m_minValTile->getRow();
//
//				int p1 = p - dis * TILES_PER_ROW;
//				int p2 = p1;
//
//				while (dis <= Utils::DIAGONAL * (m_grid[p]->getRow() - t_cbn->m_minValTile->getRow()) && (p1 > -1 || p2 > -1))
//				{
//					double octilePP1 = dis;
//					double octilePP2 = dis;
//
//					tryToUpdateOppositeBoundaryPoint(p, p1, p2, octilePP1, octilePP2);
//
//					if (m_grid[p]->m_mode != GridTile::ReaMode::Hpoint)
//					{
//						m_grid[p]->m_mode = GridTile::ReaMode::Gpoint;
//					}
//
//					dis += (Utils::DIAGONAL - 1.0f);
//					if (p1 > -1 && p1 > t_cbn->m_interval[0])
//					{
//						p1--;
//					}
//					else
//					{
//						p1 = -1;
//					}
//					if (p2 > -1 && p2 < t_cbn->m_interval[t_cbn->m_interval.size() - 1])
//					{
//						p2++;
//					}
//					else
//					{
//						p2 = -1;
//					}
//				}
//			}
//			if (successor(southBoundary))
//			{
//				return true;
//			}
//		}
//	}
//	//if cbn expand direction is WEST
//	else if (t_cbn->m_dir == Utils::LEFT)
//	{
//		//North boudary
//		{
//			BoundaryNode& northBoundary = t_sideWall1;
//
//			//get south most point from t_cbn (PointSouth)
//			int pn = t_cbn->m_interval[0];
//
//			double diagonal = Utils::DIAGONAL;
//
//			//move LEFT one point
//			int p = pn - 1;
//
//			//PointHorizontal
//			int ph = pn;
//
//			//move DOWN one point
//			int pd = pn + TILES_PER_ROW;
//			if (pd > t_cbn->m_interval[t_cbn->m_interval.size() - 1])
//			{
//				pd = -1;
//			}
//
//			while (p >= northBoundary.m_boundary[0])
//			{
//				double octileVert = 1.0f;
//				double octileDiag = diagonal;
//
//				tryToUpdateSideBoundaryPoint(p, ph, pd, octileVert, octileDiag, t_cbn, northBoundary.m_dir);
//
//				if (m_grid[p]->m_mode != GridTile::ReaMode::Hpoint)
//				{
//					m_grid[p]->m_mode = GridTile::ReaMode::Gpoint;
//				}
//				diagonal += Utils::DIAGONAL;
//
//				p -= 1;
//				ph -= 1;
//
//				if (pd > -1 && pd < t_cbn->m_interval[t_cbn->m_interval.size() - 1])
//				{
//					pd += TILES_PER_ROW;
//				}
//				else
//				{
//					pd = -1;
//				}
//			}
//
//			if (successor(northBoundary))
//			{
//				return true;
//			}
//		}
//
//		//south boudary
//		{
//			BoundaryNode& southBoundary = t_sideWall2;
//
//			//get south most point from t_cbn (PointSouth)
//			int ps = t_cbn->m_interval[t_cbn->m_interval.size() - 1];
//
//			double diagonal = Utils::DIAGONAL;
//
//			//move LEFT one point
//			int p = ps - 1;
//
//			//PointHorizontal
//			int ph = ps;
//
//			//move UP one point
//			int pd = ps - TILES_PER_ROW;
//			if (pd < t_cbn->m_interval[0])
//			{
//				pd = -1;
//			}
//
//			while (p >= southBoundary.m_boundary[0])
//			{
//				double octileVert = 1.0f;
//				double octileDiag = diagonal;
//
//				tryToUpdateSideBoundaryPoint(p, ph, pd, octileVert, octileDiag, t_cbn, southBoundary.m_dir);
//
//				if (m_grid[p]->m_mode != GridTile::ReaMode::Hpoint)
//				{
//					m_grid[p]->m_mode = GridTile::ReaMode::Gpoint;
//				}
//				diagonal += Utils::DIAGONAL;
//
//				p -= 1;
//				ph -= 1;
//
//				if (pd > -1 && pd > t_cbn->m_interval[0])
//				{
//					pd -= TILES_PER_ROW;
//				}
//				else
//				{
//					pd = -1;
//				}
//			}
//
//			if (successor(southBoundary))
//			{
//				return true;
//			}
//		}
//
//		//West Boundary
//		{
//			BoundaryNode& westBoundary = t_oppositeWall;
//
//			for (auto& p : westBoundary.m_boundary)
//			{
//				double dis = t_cbn->m_minValTile->getCol() - m_grid[p]->getCol();
//
//				int p1 = p + dis;
//				int p2 = p1;
//
//				while (dis <= Utils::DIAGONAL * (t_cbn->m_minValTile->getCol() - m_grid[p]->getCol()) && (p1 > -1 || p2 > -1))
//				{
//					double octilePP1 = dis;
//					double octilePP2 = dis;
//
//					tryToUpdateOppositeBoundaryPoint(p, p1, p2, octilePP1, octilePP2);
//
//					if (m_grid[p]->m_mode != GridTile::ReaMode::Hpoint)
//					{
//						m_grid[p]->m_mode = GridTile::ReaMode::Gpoint;
//					}
//
//					dis += (Utils::DIAGONAL - 1.0f);
//					if (p1 > -1 && p1 > t_cbn->m_interval[0])
//					{
//						p1 -= TILES_PER_ROW;
//					}
//					else
//					{
//						p1 = -1;
//					}
//					if (p2 > -1 && p2 < t_cbn->m_interval[t_cbn->m_interval.size() - 1])
//					{
//						p2 += TILES_PER_ROW;
//					}
//					else
//					{
//						p2 = -1;
//					}
//				}
//			}
//			if (successor(westBoundary))
//			{
//				return true;
//			}
//		}
//	}
//	//last direction, direction is EAST
//	else
//	{
//		//North boudary
//		{
//			BoundaryNode& northBoundary = t_sideWall1;
//
//			//get south most point from t_cbn (PointSouth)
//			int pn = t_cbn->m_interval[0];
//
//			double diagonal = Utils::DIAGONAL;
//
//			//move RIGHT one point
//			int p = pn + 1;
//
//			//PointHorizontal
//			int ph = pn;
//
//			//move DOWN one point
//			int pd = pn + TILES_PER_ROW;
//			if (pd > t_cbn->m_interval[t_cbn->m_interval.size() - 1])
//			{
//				pd = -1;
//			}
//
//			while (p <= northBoundary.m_boundary[northBoundary.m_boundary.size() - 1])
//			{
//				double octileVert = 1.0f;
//				double octileDiag = diagonal;
//
//				tryToUpdateSideBoundaryPoint(p, ph, pd, octileVert, octileDiag, t_cbn, northBoundary.m_dir);
//
//				if (m_grid[p]->m_mode != GridTile::ReaMode::Hpoint)
//				{
//					m_grid[p]->m_mode = GridTile::ReaMode::Gpoint;
//				}
//				diagonal += Utils::DIAGONAL;
//
//				p += 1;
//				ph += 1;
//
//				if (pd > -1 && pd < t_cbn->m_interval[t_cbn->m_interval.size() - 1])
//				{
//					pd += TILES_PER_ROW;
//				}
//				else
//				{
//					pd = -1;
//				}
//			}
//
//			if (successor(northBoundary))
//			{
//				return true;
//			}
//		}
//
//		//south boudary
//		{
//			BoundaryNode& southBoundary = t_sideWall2;
//
//			//get south most point from t_cbn (PointSouth)
//			int ps = t_cbn->m_interval[t_cbn->m_interval.size() - 1];
//
//			double diagonal = Utils::DIAGONAL;
//
//			//move RIGHT one point
//			int p = ps + 1;
//
//			//PointHorizontal
//			int ph = ps;
//
//			//move UP one point
//			int pd = ps - TILES_PER_ROW;
//			if (pd < t_cbn->m_interval[0])
//			{
//				pd = -1;
//			}
//
//			while (p <= southBoundary.m_boundary[southBoundary.m_boundary.size() - 1])
//			{
//				double octileVert = 1.0f;
//				double octileDiag = diagonal;
//
//				tryToUpdateSideBoundaryPoint(p, ph, pd, octileVert, octileDiag, t_cbn, southBoundary.m_dir);
//
//				if (m_grid[p]->m_mode != GridTile::ReaMode::Hpoint)
//				{
//					m_grid[p]->m_mode = GridTile::ReaMode::Gpoint;
//				}
//				diagonal += Utils::DIAGONAL;
//
//				p += 1;
//				ph += 1;
//
//				if (pd > -1 && pd > t_cbn->m_interval[0])
//				{
//					pd -= TILES_PER_ROW;
//				}
//				else
//				{
//					pd = -1;
//				}
//			}
//
//			if (successor(southBoundary))
//			{
//				return true;
//			}
//		}
//
//		//East Boundary
//		{
//			BoundaryNode& eastBoundary = t_oppositeWall;
//
//			for (auto& p : eastBoundary.m_boundary)
//			{
//				double dis = m_grid[p]->getCol() - t_cbn->m_minValTile->getCol();
//
//				int p1 = p - dis;
//				int p2 = p1;
//
//				while (dis <= Utils::DIAGONAL * (m_grid[p]->getCol() - t_cbn->m_minValTile->getCol()) && (p1 > -1 || p2 > -1))
//				{
//					double octilePP1 = dis;
//					double octilePP2 = dis;
//
//					tryToUpdateOppositeBoundaryPoint(p, p1, p2, octilePP1, octilePP2);
//
//					if (m_grid[p]->m_mode != GridTile::ReaMode::Hpoint)
//					{
//						m_grid[p]->m_mode = GridTile::ReaMode::Gpoint;
//					}
//
//					dis += (Utils::DIAGONAL - 1.0f);
//					if (p1 > -1 && p1 > t_cbn->m_interval[0])
//					{
//						p1 -= TILES_PER_ROW;
//					}
//					else
//					{
//						p1 = -1;
//					}
//					if (p2 > -1 && p2 < t_cbn->m_interval[t_cbn->m_interval.size() - 1])
//					{
//						p2 += TILES_PER_ROW;
//					}
//					else
//					{
//						p2 = -1;
//					}
//				}
//			}
//			if (successor(eastBoundary))
//			{
//				return true;
//			}
//		}
//	}
//
//	return false;
//}
//
//bool RenderGrid::expand(SearchNode* t_cbn)
//{
//	if (m_debug)
//		std::cout << "\tExpand..." << std::endl;
//
//	if (checkIfWithinRect(m_goalIndex, t_cbn->m_interval[0], t_cbn->m_interval[t_cbn->m_interval.size() - 1]))
//	{
//		if (m_debug)
//			std::cout << "\t\tApparently Goal found in Expand in CBN" << std::endl;
//		return true;
//	}
//
//	if (!t_cbn->m_minValTile)
//	{
//		int stopRightThereCriminalScum = 1;
//		return false;
//	}
//
//	int origin = t_cbn->m_minValTile->getIndex();
//	for (auto& boundary : m_tempBoundaries)
//	{
//		boundary.m_boundary.clear();
//		boundary.m_eni.clear();
//		boundary.m_fsi.clear();
//		boundary.m_minfval = std::numeric_limits<int>::max();
//		boundary.m_previous = nullptr;
//	}
//
//	if (getRect(m_tempBoundaries, t_cbn->m_dir, origin, t_cbn->m_interval))
//	{
//		if (m_debug)
//			std::cout << "\t\tGoal found in a rectangle in Expand" << std::endl;
//
//		return true;
//	}
//
//	for (auto& boundary : m_tempBoundaries)
//	{
//		boundary.m_previous = t_cbn->m_minValTile;
//	}
//
//	if (t_cbn->m_dir == Utils::TOP)
//	{
//		return processBoundaries(t_cbn, m_tempBoundaries[LEFT], m_tempBoundaries[RIGHT], m_tempBoundaries[TOP]);
//	}
//	else if (t_cbn->m_dir == Utils::BOTTOM)
//	{
//		return processBoundaries(t_cbn, m_tempBoundaries[LEFT], m_tempBoundaries[RIGHT], m_tempBoundaries[BOT]);
//	}
//	else if (t_cbn->m_dir == Utils::LEFT)
//	{
//		return processBoundaries(t_cbn, m_tempBoundaries[TOP], m_tempBoundaries[BOT], m_tempBoundaries[LEFT]);
//	}
//	else
//	{
//		return processBoundaries(t_cbn, m_tempBoundaries[TOP], m_tempBoundaries[BOT], m_tempBoundaries[RIGHT]);
//	}
//}
//
//bool RenderGrid::insertS()
//{
//	if (m_debug)
//		std::cout << "\tInsertS..." << std::endl;
//
//	std::vector<BoundaryNode> boundaryNodes;
//
//	if (getStartRect(boundaryNodes, Utils::TOP, m_startIndex))
//	{
//		if (m_debug)
//			std::cout << "\t\tGoal found in a rectangle in InsertS" << std::endl;
//		return true;
//	}
//
//	//for each boundary in boundaries
//	for (auto& boundary : boundaryNodes)
//	{
//		//for each index in boundary
//		for (auto& index : boundary.m_boundary)
//		{
//			if (index != m_startIndex)
//			{
//				m_grid[index]->setPrevious(m_grid[m_startIndex]);
//			}
//			m_grid[index]->m_gval = getOctileDist(m_grid[index]->getColRow(), m_grid[m_startIndex]->getColRow());
//			m_grid[index]->m_mode = GridTile::ReaMode::Gpoint;
//		}
//	}
//
//	//left
//	if (boundaryNodes[LEFT].m_boundary.size() > 0 && isValidBoundary(boundaryNodes[LEFT].m_boundary[0], boundaryNodes[LEFT].m_dir))
//	{
//		boundaryNodes[LEFT].m_previous = m_grid[m_startIndex];
//		if (successor(boundaryNodes[LEFT]))
//		{
//			//path found
//			return true;
//		}
//	}
//	//right
//	if (boundaryNodes[RIGHT].m_boundary.size() > 0 && isValidBoundary(boundaryNodes[RIGHT].m_boundary[0], boundaryNodes[RIGHT].m_dir))
//	{
//		boundaryNodes[RIGHT].m_previous = m_grid[m_startIndex];
//		if (successor(boundaryNodes[RIGHT]))
//		{
//			//path found
//			return true;
//		}
//	}
//	//top
//	if (boundaryNodes[TOP].m_boundary.size() > 0 && isValidBoundary(boundaryNodes[TOP].m_boundary[0], boundaryNodes[TOP].m_dir))
//	{
//		boundaryNodes[TOP].m_previous = m_grid[m_startIndex];
//		if (successor(boundaryNodes[TOP]))
//		{
//			//path found
//			return true;
//		}
//	}
//	//bot
//	if (boundaryNodes[BOT].m_boundary.size() > 0 && isValidBoundary(boundaryNodes[BOT].m_boundary[0], boundaryNodes[BOT].m_dir))
//	{
//		boundaryNodes[BOT].m_previous = m_grid[m_startIndex];
//		if (successor(boundaryNodes[BOT]))
//		{
//			//path found
//			return true;
//		}
//	}
//
//	return false;
//}
//
//bool RenderGrid::successor(BoundaryNode& t_parentBoundary)
//{
//	if (m_debug)
//		std::cout << "\t\t\tSuccessor..." << std::endl;
//
//	//ENI = extend neighbour interval of Parent Boundary;
//	//calculate ENI
//	calcENI(t_parentBoundary);
//	//FSI = Free Sub Interval<s> of ENI
//	//calculate FSI
//	calcFSI(t_parentBoundary);
//
//	if (t_parentBoundary.m_fsi.size() == 0)
//	{
//		if (m_debug)
//			std::cout << "\t\t\t\tSuccessor Quit - FSI = 0..." << std::endl;
//		return false;
//	}
//
//
//	//loop through every FreeSubInterval
//	for (auto& fsi : t_parentBoundary.m_fsi)
//	{
//		//used to check if a FSI was updated at all
//		bool fsiUpdated = false;
//		fsi.m_previous = t_parentBoundary.m_previous;
//		fsi.m_dir = t_parentBoundary.m_dir; //just incase
//		bool firstDiag = false;
//		bool firstDiagStartPoint = false;
//		bool lastDiag = false;
//		bool secondDiagStartPoint = false;
//
//		for (auto& point : fsi.m_interval)
//		{
//
//			if (point == t_parentBoundary.m_eni[0] || point == t_parentBoundary.m_eni[t_parentBoundary.m_eni.size() - 1])
//			{
//				if (fsiSpecialCasePoint(point, t_parentBoundary))
//				{
//					if (tryToUpdateSpecialCaseFsiPoint(point, fsi.m_dir, (point == fsi.m_interval[0])))
//					{
//						fsiUpdated = true;
//						setupFsiPoint(point, fsi);
//					}
//				}
//				else if (point == t_parentBoundary.m_eni[0])
//				{
//					firstDiag = true;
//					firstDiagStartPoint = point == fsi.m_interval[0];
//				}
//				else if (point == t_parentBoundary.m_eni[t_parentBoundary.m_eni.size() - 1])
//				{
//					lastDiag = true;
//				}
//			}
//			else
//			{
//				if (tryToUpdateFsiPoint(point, t_parentBoundary.m_dir))
//				{
//					fsiUpdated = true;
//					setupFsiPoint(point, fsi);
//				}
//			}
//
//			if (point == fsi.m_interval[fsi.m_interval.size() - 1])
//			{
//				if (firstDiag)
//				{
//					if (tryToUpdateDiagonalFsiPoint(t_parentBoundary.m_eni[0], fsi.m_dir, firstDiagStartPoint))
//					{
//						fsiUpdated = true;
//						setupFsiPoint(t_parentBoundary.m_eni[0], fsi);
//					}
//				}
//				if (lastDiag)
//				{
//					if (tryToUpdateDiagonalFsiPoint(t_parentBoundary.m_eni[t_parentBoundary.m_eni.size() - 1], fsi.m_dir, (point == fsi.m_interval[0])))
//					{
//						fsiUpdated = true;
//						setupFsiPoint(t_parentBoundary.m_eni[t_parentBoundary.m_eni.size() - 1], fsi);
//					}
//				}
//			}
//		}
//
//		//if goal is inside FSI
//		if (checkIfWithinRect(m_goalIndex, fsi.m_interval[0], fsi.m_interval[fsi.m_interval.size() - 1]))
//		{
//			//goal's fval is not greater than (PP) parent search node's minfval
//			if (t_parentBoundary.m_previous != nullptr && m_grid[m_goalIndex]->m_fval <= t_parentBoundary.m_previous->m_fval)
//			{
//				if (m_debug)
//					std::cout << "\t\t\t\Goal found in FSI" << std::endl;
//				return true;
//			}
//		}
//
//		//if a point in a FSI was updated
//		if (fsiUpdated)
//		{
//			SearchNode* PN = new SearchNode(fsi);
//			try
//			{
//				m_searchNodes.emplace_back(PN);
//				m_openlist.push(PN);
//			}
//			catch (const std::exception&)
//			{
//				delete PN;
//				throw;
//			}
//			if (m_debug)
//				reaGridRedraw();
//		}
//	}
//	return false;
//}
//
//void RenderGrid::setupFsiPoint(int& t_point, SearchNode& t_fsi)
//{
//	m_grid[t_point]->m_mode = GridTile::ReaMode::Hpoint;
//	m_grid[t_point]->m_hval = getOctileDist(m_grid[t_point]->getColRow(), m_grid[m_goalIndex]->getColRow());
//	m_grid[t_point]->m_fval = m_grid[t_point]->m_hval + m_grid[t_point]->m_gval;
//	m_grid[t_point]->setMarked(true);
//
//	if (t_fsi.m_minfval > m_grid[t_point]->m_fval)
//	{
//		t_fsi.m_minfval = m_grid[t_point]->m_fval;
//		t_fsi.m_minValTile = m_grid[t_point];
//	}
//}
//
//void RenderGrid::calcENI(BoundaryNode& t_parentBoundary)
//{
//	int corner1 = t_parentBoundary.m_boundary[0];
//	int corner2 = t_parentBoundary.m_boundary[t_parentBoundary.m_boundary.size() - 1];
//	switch (t_parentBoundary.m_dir)
//	{
//	case NeighbourIndex::TOP:
//	{
//		corner1 = getNeighbourIndex(NeighbourIndex::TOP_LEFT, corner1);
//		if (corner1 == -1)
//		{
//			corner1 = getNeighbourIndex(NeighbourIndex::TOP, t_parentBoundary.m_boundary[0]);
//		}
//
//		corner2 = getNeighbourIndex(NeighbourIndex::TOP_RIGHT, corner2);
//		if (corner2 == -1)
//		{
//			corner2 = getNeighbourIndex(NeighbourIndex::TOP, t_parentBoundary.m_boundary[t_parentBoundary.m_boundary.size() - 1]);
//		}
//
//		break;
//	}
//	case NeighbourIndex::BOTTOM:
//	{
//		corner1 = getNeighbourIndex(NeighbourIndex::BOTTOM_LEFT, corner1);
//		if (corner1 == -1)
//		{
//			corner1 = getNeighbourIndex(NeighbourIndex::BOTTOM, t_parentBoundary.m_boundary[0]);
//		}
//
//		corner2 = getNeighbourIndex(NeighbourIndex::BOTTOM_RIGHT, corner2);
//		if (corner2 == -1)
//		{
//			corner2 = getNeighbourIndex(NeighbourIndex::BOTTOM, t_parentBoundary.m_boundary[t_parentBoundary.m_boundary.size() - 1]);
//		}
//
//		break;
//	}
//	case NeighbourIndex::LEFT:
//	{
//		corner1 = getNeighbourIndex(NeighbourIndex::TOP_LEFT, corner1);
//		if (corner1 == -1)
//		{
//			corner1 = getNeighbourIndex(NeighbourIndex::LEFT, t_parentBoundary.m_boundary[0]);
//		}
//
//		corner2 = getNeighbourIndex(NeighbourIndex::BOTTOM_LEFT, corner2);
//		if (corner2 == -1)
//		{
//			corner2 = getNeighbourIndex(NeighbourIndex::LEFT, t_parentBoundary.m_boundary[t_parentBoundary.m_boundary.size() - 1]);
//		}
//
//		break;
//	}
//	case NeighbourIndex::RIGHT:
//	{
//		corner1 = getNeighbourIndex(NeighbourIndex::TOP_RIGHT, corner1);
//		if (corner1 == -1)
//		{
//			corner1 = getNeighbourIndex(NeighbourIndex::RIGHT, t_parentBoundary.m_boundary[0]);
//		}
//
//		corner2 = getNeighbourIndex(NeighbourIndex::BOTTOM_RIGHT, corner2);
//		if (corner2 == -1)
//		{
//			corner2 = getNeighbourIndex(NeighbourIndex::RIGHT, t_parentBoundary.m_boundary[t_parentBoundary.m_boundary.size() - 1]);
//		}
//
//		break;
//	}
//	default:
//		break;
//	}
//	calcBoundary(corner1, corner2, t_parentBoundary.m_dir, t_parentBoundary.m_eni);
//}
//
//void RenderGrid::calcFSI(BoundaryNode& t_parentBoundary)
//{
//	if (t_parentBoundary.m_eni.size() == 0)
//	{
//		return;
//	}
//	t_parentBoundary.m_fsi.reserve(static_cast<int>(1 + t_parentBoundary.m_eni.size() / 2));
//	bool fsiStarted = false;
//	int index = t_parentBoundary.m_eni[0];
//	int fsiIndex = -1;
//	int offset = 0;
//	NeighbourIndex diagonalCheck1;
//	NeighbourIndex diagonalCheck2;
//	NeighbourIndex cornerCheck1;
//	NeighbourIndex cornerCheck2;
//
//	switch (t_parentBoundary.m_dir)
//	{
//	case NeighbourIndex::TOP:
//		offset = 1;
//		diagonalCheck1 = NeighbourIndex::RIGHT;
//		diagonalCheck2 = NeighbourIndex::LEFT;
//		cornerCheck1 = NeighbourIndex::TOP_LEFT;
//		cornerCheck2 = NeighbourIndex::TOP_RIGHT;
//		break;
//	case NeighbourIndex::BOTTOM:
//		offset = 1;
//		diagonalCheck1 = NeighbourIndex::RIGHT;
//		diagonalCheck2 = NeighbourIndex::LEFT;
//		cornerCheck1 = NeighbourIndex::BOTTOM_LEFT;
//		cornerCheck2 = NeighbourIndex::BOTTOM_RIGHT;
//		break;
//	case NeighbourIndex::LEFT:
//		offset = TILES_PER_ROW;
//		diagonalCheck1 = NeighbourIndex::BOTTOM;
//		diagonalCheck2 = NeighbourIndex::TOP;
//		cornerCheck1 = NeighbourIndex::TOP_LEFT;
//		cornerCheck2 = NeighbourIndex::BOTTOM_LEFT;
//		break;
//	case NeighbourIndex::RIGHT:
//		offset = TILES_PER_ROW;
//		diagonalCheck1 = NeighbourIndex::BOTTOM;
//		diagonalCheck2 = NeighbourIndex::TOP;
//		cornerCheck1 = NeighbourIndex::TOP_RIGHT;
//		cornerCheck2 = NeighbourIndex::BOTTOM_RIGHT;
//		break;
//	default:
//		break;
//	}
//
//	//boundary corners need to check for diagonals.
//	if (t_parentBoundary.m_eni[0] == getNeighbourIndex(cornerCheck1, t_parentBoundary.m_boundary[0]))//check if this is a special case
//	{
//		if (m_grid[index]->getType() != GridTile::TileType::Obstacle &&
//			m_grid[getNeighbourIndex(diagonalCheck1, index)]->getType() != GridTile::TileType::Obstacle)
//		{
//			t_parentBoundary.m_fsi.emplace_back(index, t_parentBoundary.m_dir);
//			fsiIndex++;
//			fsiStarted = true;
//		}
//		index += offset;
//	}
//
//	while (index != t_parentBoundary.m_eni[t_parentBoundary.m_eni.size() - 1]/* + offset*/)
//	{
//		if (fsiStarted)
//		{
//			if (m_grid[index]->getType() != GridTile::TileType::Obstacle)
//			{
//				t_parentBoundary.m_fsi[fsiIndex].m_interval.emplace_back(index);
//			}
//			else
//			{
//				fsiStarted = false;
//			}
//		}
//		else if (m_grid[index]->getType() != GridTile::TileType::Obstacle)
//		{
//			fsiStarted = true;
//			t_parentBoundary.m_fsi.emplace_back(index, t_parentBoundary.m_dir);
//			fsiIndex++;
//		}
//
//		index += offset;
//	}
//	if (t_parentBoundary.m_eni[t_parentBoundary.m_eni.size() - 1] ==
//		getNeighbourIndex(cornerCheck2, t_parentBoundary.m_boundary[t_parentBoundary.m_boundary.size() - 1])) //check if this is a special case
//	{
//		if (m_grid[index]->getType() != GridTile::TileType::Obstacle &&
//			m_grid[getNeighbourIndex(diagonalCheck2, index)]->getType() != GridTile::TileType::Obstacle)
//		{
//			t_parentBoundary.m_fsi[fsiIndex].m_interval.emplace_back(index);
//		}
//	}
//	else if (m_grid[index]->getType() != GridTile::TileType::Obstacle)
//	{
//		if (fsiStarted)
//		{
//			t_parentBoundary.m_fsi[fsiIndex].m_interval.emplace_back(index);
//		}
//		else
//		{
//			t_parentBoundary.m_fsi.emplace_back(index, t_parentBoundary.m_dir);
//			fsiIndex++;
//			t_parentBoundary.m_fsi[fsiIndex].m_interval.emplace_back(index);
//		}
//	}
//}
//
//void RenderGrid::getRectBoundaries(std::vector<int>& t_rectBoundary, std::vector<BoundaryNode>& t_boundaries)
//{
//	//potentially 4 rect boundaries
//	//left0
//	calcBoundary(t_rectBoundary[0], t_rectBoundary[2], Utils::LEFT, t_boundaries[LEFT].m_boundary);
//	//right1
//	calcBoundary(t_rectBoundary[1], t_rectBoundary[3], Utils::RIGHT, t_boundaries[RIGHT].m_boundary);
//	//top2
//	calcBoundary(t_rectBoundary[0], t_rectBoundary[1], Utils::TOP, t_boundaries[TOP].m_boundary);
//	//bot3
//	calcBoundary(t_rectBoundary[2], t_rectBoundary[3], Utils::BOTTOM, t_boundaries[BOT].m_boundary);
//}
//
//void RenderGrid::calcBoundary(int& t_corner1, int& t_corner2, NeighbourIndex& t_dir, std::vector<int>& t_boundaryContainer)
//{
//	int index = t_corner1;
//	int offset = 0;
//
//	switch (t_dir)
//	{
//	case NeighbourIndex::LEFT:
//	case NeighbourIndex::RIGHT:
//		offset = TILES_PER_ROW;
//		break;
//	case NeighbourIndex::TOP:
//	case NeighbourIndex::BOTTOM:
//		offset = 1;
//		break;
//	default:
//		break;
//	}
//
//	while (index > -1 && index < MAX_TILES && index <= t_corner2)
//	{
//		t_boundaryContainer.emplace_back(index);
//		index += offset;
//	}
//}
//
//bool RenderGrid::isValidBoundary(int& t_boundary, NeighbourIndex& t_directionOfBoundary)
//{
//	int index = getNeighbourIndex(t_directionOfBoundary, t_boundary);
//
//	if (index >= 0/* && m_grid[index]->m_mode == GridTile::ReaMode::None*/)
//	{
//		return true;
//	}
//	return false;
//}
//
//bool RenderGrid::getStartRect(std::vector<BoundaryNode>& t_boundaries, NeighbourIndex& t_dir, int& t_origin)
//{
//	m_tempRect.clear();
//	t_boundaries.resize(4);
//
//	for (int i = 0; i < 4; i++)
//	{
//		t_boundaries[i].m_dir = static_cast<NeighbourIndex>(i);
//	}
//
//	if (getRectInDirection(m_tempRect, t_dir, t_origin, true, -1, -1))
//		return true; //goal found in rectangle
//
//	getRectBoundaries(m_tempRect, t_boundaries);
//
//	return false;
//}
//
//bool RenderGrid::getRect(std::vector<BoundaryNode>& t_boundaries, NeighbourIndex& t_dir, int& t_origin, std::vector<int>& t_fsiInterval)
//{
//	m_tempRect.clear();
//	int sideLim1;
//	int sideLim2;
//
//	switch (t_dir)
//	{
//	case NeighbourIndex::LEFT:
//	case NeighbourIndex::RIGHT:
//		sideLim1 = getCurrentSideLimit(t_dir, t_origin, t_fsiInterval, Utils::TOP);
//		sideLim2 = getCurrentSideLimit(t_dir, t_origin, t_fsiInterval, Utils::BOTTOM);
//		break;
//	case NeighbourIndex::TOP:
//	case NeighbourIndex::BOTTOM:
//		sideLim1 = getCurrentSideLimit(t_dir, t_origin, t_fsiInterval, Utils::LEFT);
//		sideLim2 = getCurrentSideLimit(t_dir, t_origin, t_fsiInterval, Utils::RIGHT);
//		break;
//	default:
//		break;
//	}
//
//	if (getRectInDirection(m_tempRect, t_dir, t_origin, false, sideLim1, sideLim2))
//		return true; //goal found in rectangle
//
//	getRectBoundaries(m_tempRect, t_boundaries);
//
//	//t_rectPoints.emplace_back(m_tempRect[0]);
//	//t_rectPoints.emplace_back(m_tempRect[m_tempRect.size() - 1]);
//
//	return false;
//}
//
//int RenderGrid::getCurrentSideLimit(NeighbourIndex& t_expandDir, int& t_origin, std::vector<int>& t_interval, NeighbourIndex& t_sideDirection)
//{
//	int sideLimit = -1;
//
//	switch (t_expandDir)
//	{
//	case NeighbourIndex::LEFT:
//	case NeighbourIndex::RIGHT:
//	{
//		if (t_sideDirection == Utils::TOP)
//		{
//			sideLimit = m_grid[t_interval[0]]->getRow();
//		}
//		else
//		{
//			sideLimit = m_grid[t_interval[t_interval.size() - 1]]->getRow();
//		}
//		break;
//	}
//	case NeighbourIndex::TOP:
//	case NeighbourIndex::BOTTOM:
//	{
//		if (t_sideDirection == Utils::LEFT)
//		{
//			sideLimit = m_grid[t_interval[0]]->getCol();
//		}
//		else
//		{
//			sideLimit = m_grid[t_interval[t_interval.size() - 1]]->getCol();
//		}
//		break;
//	}
//	default:
//		throw std::invalid_argument("received bad direction value");
//		break;
//	}
//
//	return sideLimit;
//}
//
//bool RenderGrid::aStar()
//{
//	if (m_startIndex > -1 && m_goalIndex > -1)
//	{
//		std::priority_queue<GridTile*, std::vector<GridTile*>, TileComparer> pq;
//		for (int i = 0; i < m_grid.size(); i++)
//		{
//			if (nullptr != m_grid[i] && m_grid[i]->getType() != GridTile::TileType::Obstacle)
//			{
//				float estDist = thor::length(static_cast<sf::Vector2f>(m_grid[m_goalIndex]->getColRow() - m_grid[i]->getColRow()));
//				//estDist *= 1.1f;
//				m_grid[i]->setEstDist(estDist);
//
//				// init g[v] to infinity # dont YET know the distance to these nodes
//				m_grid[i]->setCurrDist(std::numeric_limits<int>::max());
//				m_grid[i]->setTotalDist(std::numeric_limits<int>::max());
//			}
//		}
//
//		// init g[s] to 0
//		m_grid[m_startIndex]->setCurrDist(0);
//		m_grid[m_startIndex]->setTotalDist(0);
//		// mark s
//		m_grid[m_startIndex]->setMarked(true);
//
//		// Add s to pq
//		GridTile* current;
//		pq.push(m_grid[m_startIndex]);
//
//		// while the !pq.empty() && pq.top() != goal node
//		while (!pq.empty() && pq.top() != m_grid[m_goalIndex])
//		{
//			current = pq.top();
//			//pop from pq
//			pq.pop();
//			current->setVisited(true);
//
//			if (m_debug)
//				std::cout << "Visiting: " << current->getIndex() << std::endl;
//
//			//for each child node c of pq.top() - 8 neighbours
//			bool skipTopLeft = false, skipTopRight = false, skipBotLeft = false, skipBotRight = false;
//			for (int i = 0; i < 8; i++)
//			{
//				NeighbourIndex neighbour = static_cast<NeighbourIndex>(i);
//				int tileIndex = getNeighbourIndex(neighbour, current->getIndex());
//				if (tileIndex > -1 && m_grid[tileIndex]->getType() != GridTile::TileType::Obstacle)
//				{
//					if (static_cast<NeighbourIndex>(i) == NeighbourIndex::TOP_LEFT && skipTopLeft)
//					{
//						continue;
//					}
//					else if (static_cast<NeighbourIndex>(i) == NeighbourIndex::TOP_RIGHT && skipTopRight)
//					{
//						continue;
//					}
//					else if (static_cast<NeighbourIndex>(i) == NeighbourIndex::BOTTOM_LEFT && skipBotLeft)
//					{
//						continue;
//					}
//					else if (static_cast<NeighbourIndex>(i) == NeighbourIndex::BOTTOM_RIGHT && skipBotRight)
//					{
//						continue;
//					}
//
//					GridTile* child = m_grid[tileIndex];
//
//					//if child != previous(pq.top())
//					if (child != current->getPrevious()) {
//						//g(child) # g(c) is weight from current to child + distance of previous node
//						float dist;
//						switch (neighbour)
//						{
//						case NeighbourIndex::LEFT:
//						case NeighbourIndex::RIGHT:
//						case NeighbourIndex::BOTTOM:
//						case NeighbourIndex::TOP:
//							dist = 1.0f;
//							break;
//						case NeighbourIndex::TOP_LEFT:
//						case NeighbourIndex::TOP_RIGHT:
//						case NeighbourIndex::BOTTOM_LEFT:
//						case NeighbourIndex::BOTTOM_RIGHT:
//							dist = 1.414f;
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
//				else if (tileIndex > -1)
//				{
//					if (static_cast<NeighbourIndex>(i) == NeighbourIndex::LEFT)
//					{
//						skipTopLeft = true;
//						skipBotLeft = true;
//					}
//					else if (static_cast<NeighbourIndex>(i) == NeighbourIndex::RIGHT)
//					{
//						skipTopRight = true;
//						skipBotRight = true;
//					}
//					else if (static_cast<NeighbourIndex>(i) == NeighbourIndex::TOP)
//					{
//						skipTopLeft = true;
//						skipTopRight = true;
//					}
//					else if (static_cast<NeighbourIndex>(i) == NeighbourIndex::BOTTOM)
//					{
//						skipBotLeft = true;
//						skipBotRight = true;
//					}
//				}
//
//			}//end for
//			if (m_debug)
//			{
//				std::this_thread::sleep_for(std::chrono::milliseconds(Utils::DELAY_AMOUNT));
//				gridRedraw();
//			}
//		}//end while
//
//		if (m_grid[m_goalIndex]->getPrevious() != nullptr)
//		{
//			return true;
//		}
//	}//end if
//	return false;
//}
//
//void RenderGrid::prePathfindReset()
//{
//	//Initialise
//	for (SearchNode* p : m_searchNodes)
//	{
//		delete p;
//	}
//	m_searchNodes.clear();
//
//	m_openlist = std::priority_queue<SearchNode*, std::vector<SearchNode*>, NodeComparer>();
//
//	m_lines.clear();
//	resetNonObstacles();
//}
//
//void RenderGrid::updateText(const sf::Vector2f& t_textPos)
//{
//	if (m_debug)
//	{
//		m_debugStateString = Utils::DEBUG_ON;
//		if (m_useRea)
//		{
//			m_debugStateString += Utils::DEBUG_REA;
//		}
//		else
//		{
//			m_debugStateString += Utils::DEBUG_ASTAR;
//		}
//		m_debugStateText.setString(m_debugStateString);
//		m_debugStateText.setFillColor(sf::Color::Red);
//		m_debugStateText.setPosition(t_textPos);
//	}
//	else
//	{
//		m_debugStateString = Utils::DEBUG_OFF;
//		m_debugStateText.setString(m_debugStateString);
//		m_debugStateText.setFillColor(sf::Color::Green);
//		m_debugStateText.setPosition(t_textPos);
//	}
//
//	if (m_useRea)
//	{
//		m_algTypeString = Utils::USE_REA;
//		m_algTypeText.setString(m_algTypeString);
//		m_algTypeText.setPosition(t_textPos);
//	}
//	else
//	{
//		m_algTypeString = Utils::USE_ASTAR;
//		m_algTypeText.setString(m_algTypeString);
//		m_algTypeText.setPosition(t_textPos);
//	}
//}
//
//void RenderGrid::initText(float t_textOffset, const sf::Vector2f& t_textPos, int t_charSize)
//{
//	m_algText.setFont(m_font);
//	m_algText.setCharacterSize(t_charSize);
//	m_algText.setPosition(t_textPos.x, 0.0f);
//
//	m_algTypeText.setFont(m_font);
//	m_algTypeText.setCharacterSize(t_charSize);
//	m_algTypeText.setPosition(t_textPos.x, 0.0f);
//	m_algTypeText.setFillColor(sf::Color::Green);
//
//	m_debugText.setFont(m_font);
//	m_debugText.setCharacterSize(t_charSize);
//	m_debugText.setPosition(t_textPos.x, 0.0f);
//
//	m_debugStateText.setFont(m_font);
//	m_debugStateText.setCharacterSize(t_charSize);
//	m_debugStateText.setPosition(t_textPos.x, 0.0f);
//
//	m_reaTimeTakenText.setFont(m_font);
//	m_reaTimeTakenText.setCharacterSize(t_charSize);
//	m_reaTimeTakenText.setPosition(t_textPos.x, 0.0f);
//
//	m_aStarTimeTakenText.setFont(m_font);
//	m_aStarTimeTakenText.setCharacterSize(t_charSize);
//	m_aStarTimeTakenText.setPosition(t_textPos.x, 0.0f);
//
//	m_reaDistText.setFont(m_font);
//	m_reaDistText.setCharacterSize(t_charSize);
//	m_reaDistText.setPosition(t_textPos.x, 0.0f);
//
//	m_aStarDistText.setFont(m_font);
//	m_aStarDistText.setCharacterSize(t_charSize);
//	m_aStarDistText.setPosition(t_textPos.x, 0.0f);
//
//	m_placeModeTxt.setFont(m_font);
//	m_placeModeTxt.setString(m_placeString);
//	m_placeModeTxt.setCharacterSize(static_cast<int>(m_window.getSize().y / 31));
//	m_placeModeTxt.setPosition(t_textOffset - m_placeModeTxt.getGlobalBounds().width / 2.0f, 0.0f);
//	m_placeModeTxt.setFillColor(sf::Color::Green);
//}