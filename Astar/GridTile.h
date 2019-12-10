#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>
#include <iostream>

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
		Marked,
		Path
	};

	const int RED[3]{ 255, 0, 0 };
	const int GREEN[3]{ 0, 255, 0 };
	const int BLUE[3]{ 0, 0, 255 };


public:
	GridTile(sf::Vector2f t_pos, sf::Vector2f t_size, int t_index, GridTile* t_previous = nullptr);
	~GridTile();
	void render(sf::RenderWindow& t_window);
	void setToObstacle();
	void setToStart();
	void setToGoal();
	void setToPath();
	void reset();

	bool getVisited() const;
	bool getMarked() const;
	float getEstDist() const;
	float getCurrDist() const;
	float getTotalDist() const;
	void setVisited(bool t_visited);
	void setMarked(bool t_marked);
	void setEstDist(float t_est);
	void setCurrDist(float t_curr);
	void setTotalDist(float t_total);
	void setPrevious(GridTile* t_previous);
	GridTile* getPrevious() const;
	void printPreviousPath() const;
	float getX() const;
	float getY() const;
	int getIndex() const;
	float getDiagonal() const;

	sf::Vector2f getPos();
	TileType getType();

private:
	void setColour();

	TileType m_type;
	sf::Vector2f m_pos;
	int m_rgb[3]{ 255, 255, 255};

	sf::RectangleShape m_tile;

	//a* variables
	bool m_marked;
	bool m_visited;
	float m_currDist;
	float m_estDist;
	float m_totalDist;
	int m_index;
	
	GridTile* m_previous;
};

