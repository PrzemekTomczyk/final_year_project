#include "GridTile.h"

GridTile::GridTile(sf::Vector2f t_pos, sf::Vector2f t_size, int t_index, sf::Font& t_font, GridTile* t_previous) :
	m_pos(t_pos),
	m_type(TileType::None),
	m_index(t_index),
	m_previous(t_previous),
	m_font(t_font)
{
	m_marked = false;
	m_visited = false;

	m_tile.setFillColor(sf::Color(m_rgb[0], m_rgb[1], m_rgb[2]));
	m_tile.setSize(t_size);
	m_tile.setOrigin(t_size.x / 2.0f, t_size.y / 2.0f);
	m_tile.setPosition(m_pos);
	m_tile.setOutlineColor(sf::Color::Black);
	m_tile.setOutlineThickness(-1.0f);

	int adjustX = (int)((m_pos.x / m_tile.getSize().x) + 1);
	int adjustY = (int)((m_pos.y / m_tile.getSize().y) + 1);

	int x = (int)((m_pos.x / (m_tile.getSize().x / 2) - adjustX) * 100);
	int y = (int)((m_pos.y / (m_tile.getSize().y / 2) - adjustY) * 100);
	m_pathPos = sf::Vector2f((float)x, (float)y);

	m_text.setFont(m_font);
	m_text.setCharacterSize(12);
	m_text.setFillColor(sf::Color::Black);
}

GridTile::~GridTile()
{
	m_previous = nullptr;
}

void GridTile::render(sf::RenderWindow& t_window)
{
	setColour();
	t_window.draw(m_tile);

	//std::stringstream stream;
	//std::string str;
	//stream << "C: " << std::fixed << std::setprecision(2) << (m_currDist) << "\n" << "E: " << std::fixed << std::setprecision(2) << m_estDist << "\n" << "T: " << std::fixed << std::setprecision(2) << (m_totalDist) << "\n" << "I: " << m_index;
	//str = stream.str();
	//m_text.setPosition(m_pos - m_tile.getSize() / 2.0f);
	//m_text.setString(str);
	//t_window.draw(m_text);
}

void GridTile::setToObstacle()
{
	m_type = TileType::Obstacle;
}

void GridTile::setToStart()
{
	m_type = TileType::Start;
}

void GridTile::setToGoal()
{
	m_type = TileType::Goal;
}

void GridTile::setToPath()
{
	m_type = TileType::Path;
}

void GridTile::reset()
{
	setMarked(false);
	setPrevious(nullptr);
	setVisited(false);
	//reset type
	m_type = TileType::None;
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
	if (m_type != TileType::Goal && m_type != TileType::Start)
	{
		m_type = TileType::Visited;
	}
	m_visited = t_visited;
}

void GridTile::setMarked(bool t_marked)
{
	if (m_type != TileType::Goal && m_type != TileType::Start)
	{
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
	m_previous = t_previous;
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

float GridTile::getX() const
{
	return m_pos.x - m_tile.getSize().x;
}

float GridTile::getY() const
{
	return m_pos.y - m_tile.getSize().y;
}

int GridTile::getIndex() const
{
	return m_index;
}

float GridTile::getDiagonal() const
{
	return thor::length(m_tile.getSize());
}

sf::Vector2f GridTile::getPos()
{
	return m_pos;
}

sf::Vector2f GridTile::getPathfindingPos()
{
	return m_pathPos;
}

GridTile::TileType GridTile::getType()
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
		m_rgb[2] = 0;
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
		m_rgb[1] = 255;
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
	default:
		break;
	}
	m_tile.setFillColor(sf::Color(m_rgb[0], m_rgb[1], m_rgb[2]));
}
