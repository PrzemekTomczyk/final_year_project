#pragma once
#include <string>
#include "SFML/Graphics.hpp"
class VisNode
{
public:
	VisNode(std::string t_nodeName, sf::Vector2f t_pos, sf::Font & t_font, int t_radius);
	~VisNode();
	void draw(sf::RenderWindow &t_window);
	void highlight();
	void deHighlight();
	void select();
	void deselect();
	void setCurrText(float t_distance);
	void setCurrText(std::string t_text);
	void setTotalText(float t_distance);
	void setTotalText(std::string t_text);
	void setPath(bool t_inPath);
	void setVisited(bool t_visited);
	bool getSelect();
	std::string getCurrentText();
	std::string getTotalText();
	std::string getName();
	sf::Vector2f getPos() const;
	int getRadius() const;
private:
	sf::Color m_unselectedColour;
	sf::Color m_colour;
	sf::Color m_selectedColour;
	sf::Color m_pathColour;
	sf::Color m_visitColour;
	int m_radius;
	sf::CircleShape m_body;
	sf::Text m_name;
	sf::Text m_currDist;
	sf::Text m_totalDist;

	sf::Font &m_font;

	bool m_selected = false;
	bool m_highlight = false;
	bool m_inPath = false;
	bool m_visited = false;
};

