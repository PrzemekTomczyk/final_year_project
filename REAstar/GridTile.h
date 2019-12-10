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
		Visited,
		Path
	};

	const int RED[3]{ 255, 0, 0 };
	const int GREEN[3]{ 0, 255, 0 };
	const int BLUE[3]{ 0, 0, 255 };


public:
	GridTile(sf::Vector2f t_pos, sf::Font& t_font, int& t_highestCost, sf::Vector2f t_size);
	~GridTile();
	void init(int t_cost, int t_rgb[3]);
	void render(sf::RenderWindow& t_window, bool t_showCost, bool t_showHeatmap);
	int getCost();
	void setCost(int t_cost);
	void setToObstacle();
	void setToStart(int t_cost);
	void setToGoal();
	void setToVisited();
	void setToPath();
	void setHeuristic(sf::Vector2f t_pos);
	float getHeuristic();
	void reset();
	void setFlowField(sf::Vector2f t_direction);
	sf::Vector2f getPos();
	float getDiagonal();
	TileType getType();
	sf::RectangleShape getVectorLine();

private:
	TileType m_type;
	sf::Vector2f m_pos;
	int m_rgb[3]{ 255, 255, 255};
	int m_cost;
	float m_heuristic = -1;

	//reference to highest cost variable from GridManager
	int& m_highestCost;

	sf::RectangleShape m_tile;
	sf::RectangleShape m_vecFieldLine;

	sf::Font& m_font;
	sf::Text m_costText;
};

