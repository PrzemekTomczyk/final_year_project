#include "GridTile.h"
#include <iostream>

GridTile::GridTile(sf::Vector2f t_pos, sf::Vector2f t_size) :
	m_pos(t_pos),
	m_type(TileType::None)
{
	m_tile.setFillColor(sf::Color(m_rgb[0], m_rgb[1], m_rgb[2]));
	m_tile.setSize(t_size);
	m_tile.setOrigin(t_size.x / 2.0f, t_size.y / 2.0f);
	m_tile.setPosition(m_pos);
	m_tile.setOutlineColor(sf::Color::Black);
	m_tile.setOutlineThickness(-1.0f);
}

GridTile::~GridTile()
{
}

void GridTile::init(int t_rgb[3])
{
	for (int i = 0; i < 3; i++)
	{
		m_rgb[i] = t_rgb[i];
	}
	m_tile.setFillColor(sf::Color(m_rgb[0], m_rgb[1], m_rgb[2]));
}

void GridTile::render(sf::RenderWindow& t_window)
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
	{
		m_rgb[0] = 0;
		m_rgb[1] = 0;
		m_rgb[2] = 255;
		break;
	}
	case GridTile::TileType::Unreachable:
		m_rgb[0] = 31;
		m_rgb[1] = 31;
		m_rgb[2] = 45;
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
	t_window.draw(m_tile);
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

void GridTile::setToUnreachable()
{
	m_type = TileType::Unreachable;
}

void GridTile::setToPath()
{
	m_type = TileType::Path;
}

void GridTile::reset()
{
	//reset colour
	for (int i = 0; i < 3; i++)
	{
		m_rgb[i] = BLUE[i];
	}

	//reset type
	m_type = TileType::None;
}

sf::Vector2f GridTile::getPos()
{
	return m_pos;
}

GridTile::TileType GridTile::getType()
{
	return m_type;
}