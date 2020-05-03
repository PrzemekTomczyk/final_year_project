#include "stdafx.h"
#include "GridTile.h"

GridTile::GridTile(sf::Vector2f t_pos, sf::Vector2f t_size, int t_index, sf::Font& t_font, GridTile* t_previous) :
	m_pos(t_pos),
	m_type(TileType::None),
	m_index(t_index),
	m_previous(t_previous)
{
	m_marked = false;
	m_visited = false;

	m_tile.setFillColor(sf::Color(m_rgb[0], m_rgb[1], m_rgb[2]));
	m_tile.setSize(t_size);
	m_tile.setOrigin(t_size.x / 2.0f, t_size.y / 2.0f);
	m_tile.setPosition(m_pos);

	if (t_size.x > 5)
	{
		m_tile.setOutlineColor(sf::Color::Black);
		m_tile.setOutlineThickness(-1.0f);
	}
	int adjustX = static_cast<int>((m_pos.x / m_tile.getSize().x) + 1);
	int adjustY = static_cast<int>((m_pos.y / m_tile.getSize().y) + 1);

	int x = static_cast<int>(m_pos.x / (m_tile.getSize().x / 2) - adjustX);
	int y = static_cast<int>(m_pos.y / (m_tile.getSize().y / 2) - adjustY);
	m_colRow = sf::Vector2i(x, y);

	m_column = x;
	m_row = y;

	m_type = TileType::None;
	m_mode = ReaMode::None;
	m_fval = std::numeric_limits<int>::max();
	m_gval = std::numeric_limits<int>::max();
	m_hval = std::numeric_limits<int>::max();
	m_estDist = std::numeric_limits<int>::max();
	m_totalDist = std::numeric_limits<int>::max();
	m_currDist = std::numeric_limits<int>::max();
	setColour();
}

GridTile::~GridTile()
{
	m_previous = nullptr;
}

void GridTile::render(sf::RenderWindow& t_window)
{
	if (m_typeChanged)
	{
		setColour();
	}
	t_window.draw(m_tile);
}

void GridTile::setToObstacle()
{
	m_typeChanged = true;
	m_type = TileType::Obstacle;
}

void GridTile::setToStart()
{
	m_typeChanged = true;
	m_type = TileType::Start;
}

void GridTile::setToGoal()
{
	m_typeChanged = true;
	m_type = TileType::Goal;
}

void GridTile::setToPath()
{
	m_typeChanged = true;
	m_type = TileType::Path;
}

void GridTile::setToCorner()
{
	m_typeChanged = true;
	if (m_type == TileType::None)
	{
		m_type = TileType::Corner;
	}
}

void GridTile::reset()
{
	m_typeChanged = true;
	setMarked(false);
	setPrevious(nullptr);
	setVisited(false);
	//reset type
	m_type = TileType::None;
	m_mode = ReaMode::None;
	m_fval = std::numeric_limits<int>::max();
	m_gval = std::numeric_limits<int>::max();
	m_hval = std::numeric_limits<int>::max();
	m_estDist = std::numeric_limits<int>::max();
	m_totalDist = std::numeric_limits<int>::max();
	m_currDist = std::numeric_limits<int>::max();
}

bool GridTile::getVisited() const
{
	return m_visited;
}

bool GridTile::getMarked() const
{
	return m_marked;
}

float GridTile::getEstDist() const
{
	return m_estDist;
}

float GridTile::getCurrDist() const
{
	return m_currDist;
}

float GridTile::getTotalDist() const
{
	return m_totalDist;
}

void GridTile::setVisited(bool t_visited)
{
	if (t_visited && m_type != TileType::Goal && m_type != TileType::Start && m_type != TileType::Corner)
	{
		m_typeChanged = true;
		m_type = TileType::Visited;
	}
	m_visited = t_visited;
}

void GridTile::setMarked(bool t_marked)
{
	if (t_marked && m_type != TileType::Goal && m_type != TileType::Start && m_type != TileType::Corner)
	{
		m_typeChanged = true;
		m_type = TileType::Marked;
	}
	m_marked = t_marked;
}

void GridTile::setEstDist(float t_est)
{
	m_estDist = t_est;
}

void GridTile::setCurrDist(float t_curr)
{
	m_currDist = t_curr;
}

void GridTile::setTotalDist(float t_total)
{
	m_totalDist = t_total;
}

void GridTile::setPrevious(GridTile* t_previous)
{
	if (t_previous != this)
	{
		m_previous = t_previous;
	}
}

GridTile* GridTile::getPrevious() const
{
	return m_previous;
}

void GridTile::printPreviousPath() const
{
	std::cout << "Index: " << m_index << std::endl;

	if (m_previous != nullptr)
	{
		m_previous->printPreviousPath();
	}
}

int GridTile::getIndex() const
{
	return m_index;
}

sf::Vector2f& GridTile::getPos()
{
	return m_pos;
}

/// <summary>
/// Gets a vector holding this tile's column(x) and row(y)
/// </summary>
/// <returns>Vector2i holding row and col</returns>
sf::Vector2i& GridTile::getColRow()
{
	return m_colRow;
}

int& GridTile::getCol()
{
	return m_column;
}

int& GridTile::getRow()
{
	return m_row;
}

GridTile::TileType& GridTile::getType()
{
	return m_type;
}

void GridTile::setColour()
{
	switch (m_type)
	{
	case GridTile::TileType::Start:
		m_rgb[0] = 255;
		m_rgb[1] = 0;
		m_rgb[2] = 255;
		break;
	case GridTile::TileType::Goal:
		m_rgb[0] = 0;
		m_rgb[1] = 255;
		m_rgb[2] = 0;
		break;
	case GridTile::TileType::Obstacle:
		m_rgb[0] = 0;
		m_rgb[1] = 0;
		m_rgb[2] = 0;
		break;
	case GridTile::TileType::None:
		m_rgb[0] = 0;
		m_rgb[1] = 0;
		m_rgb[2] = 255;
		break;
	case GridTile::TileType::Marked:
		m_rgb[0] = 255;
		m_rgb[1] = 0;
		m_rgb[2] = 0;
		break;
	case GridTile::TileType::Visited:
		m_rgb[0] = 255;
		m_rgb[1] = 134;
		m_rgb[2] = 0;
		break;
	case GridTile::TileType::Path:
		m_rgb[0] = 0;
		m_rgb[1] = 255;
		m_rgb[2] = 255;
		break;
	case GridTile::TileType::Corner:
		m_rgb[0] = 150;
		m_rgb[1] = 150;
		m_rgb[2] = 150;
		break;
	default:
		break;
	}
	m_tile.setFillColor(sf::Color(m_rgb[0], m_rgb[1], m_rgb[2]));
	m_typeChanged = false;
}