#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

class GridTile
{
private:
	int m_index;

public:
	enum class TileType
	{
		Start,
		Goal,
		Obstacle,
		None,
		Visited,
		Marked,
		Path,
		Corner
	};
	enum class ReaMode
	{
		Gpoint,
		Hpoint,
		None
	};


	const int RED[3]{ 255, 0, 0 };
	const int GREEN[3]{ 0, 255, 0 };
	const int BLUE[3]{ 0, 0, 255 };

	GridTile(sf::Vector2f t_pos, sf::Vector2f t_size, int t_index, sf::Font& t_font,GridTile* t_previous = nullptr);
	~GridTile();
	void render(sf::RenderWindow& t_window);
	void setToObstacle();
	void setToStart();
	void setToGoal();
	void setToPath();
	void setToCorner();
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
	int getIndex() const;

	sf::Vector2f& getPos();
	sf::Vector2i& getColRow();
	int& getCol();
	int& getRow();


	TileType& getType();

private:
	void setColour();

	TileType m_type;
	sf::Vector2f m_pos;
	sf::Vector2i m_colRow;
	int m_column;
	int m_row;
	int m_rgb[3]{ 255, 255, 255};

	sf::RectangleShape m_tile;

	//a* variables
	bool m_marked;
	bool m_visited;
	float m_currDist;
	float m_estDist;
	float m_totalDist;
	
	GridTile* m_previous;

public:
	ReaMode m_mode;
	double m_gval;
	double m_hval;
	double m_fval;
};