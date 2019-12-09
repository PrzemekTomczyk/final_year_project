#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>

class GridTile
{
public:
	enum class TileType
	{
		Start,
		Goal,
		Obstacle,
		None,
		Unreachable,
		Path
	};

	const int RED[3]{ 255, 0, 0 };
	const int GREEN[3]{ 0, 255, 0 };
	const int BLUE[3]{ 0, 0, 255 };


public:
	GridTile(sf::Vector2f t_pos, sf::Vector2f t_size);
	~GridTile();
	void init(int t_rgb[3]);
	void render(sf::RenderWindow& t_window);
	void setToObstacle();
	void setToStart();
	void setToGoal();
	void setToUnreachable();
	void setToPath();
	void reset();
	sf::Vector2f getPos();
	TileType getType();

private:
	TileType m_type;
	sf::Vector2f m_pos;
	int m_rgb[3]{ 255, 255, 255};

	sf::RectangleShape m_tile;

	
};

