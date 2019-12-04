#include "VisNode.h"

VisNode::VisNode(std::string t_nodeName, sf::Vector2f t_pos, sf::Font & t_font, int t_radius)	:
	m_font(t_font),
	m_radius(t_radius),
	m_selected(false)

{
	m_colour = sf::Color(0, 0, 255);
	m_unselectedColour = sf::Color(0, 0, 255);
	m_selectedColour = sf::Color(0, 255, 0);
	m_pathColour = sf::Color(66, 244, 241);
	m_visitColour = sf::Color(255, 192, 0);
	m_body.setRadius(m_radius);
	m_body.setOrigin(sf::Vector2f(m_radius, m_radius));
	m_body.setFillColor(m_colour);
	m_body.setPosition(t_pos);

	m_name.setFont(t_font);
	m_name.setString(t_nodeName);
	m_name.setCharacterSize(20);
	m_name.setFillColor(sf::Color::White);
	m_name.setOrigin(m_name.getLocalBounds().width / 2.0f, m_name.getLocalBounds().height / 2.0f);
	m_name.setPosition(t_pos.x, t_pos.y - (m_radius * 0.8f));

	m_currDist.setFont(t_font);
	m_currDist.setString("0");
	m_currDist.setCharacterSize(20);
	m_currDist.setFillColor(sf::Color::White);
	m_currDist.setOrigin(m_currDist.getLocalBounds().width / 2.0f, m_currDist.getLocalBounds().height / 2.0f);
	m_currDist.setPosition(t_pos.x, t_pos.y - (m_radius * 0.2f));

	m_totalDist.setFont(t_font);
	m_totalDist.setString("0");
	m_totalDist.setCharacterSize(20);
	m_totalDist.setFillColor(sf::Color::White);
	m_totalDist.setOrigin(m_totalDist.getLocalBounds().width / 2.0f, m_totalDist.getLocalBounds().height / 2.0f);
	m_totalDist.setPosition(t_pos.x, t_pos.y + (m_radius * 0.4f));
}


VisNode::~VisNode()
{
}

void VisNode::draw(sf::RenderWindow & t_window)
{
	if (m_selected)
	{
		m_colour = m_selectedColour;
	}
	else if (m_inPath) {
		m_colour = m_pathColour;
	}
	else if (m_visited)
	{
		m_colour = m_visitColour;
	}
	else
	{
		m_colour = m_unselectedColour;
	}

	if (m_highlight)
	{
		m_colour.r *= 0.5;
		m_colour.g *= 0.5;
		m_colour.b *= 0.5;
	}

	m_body.setFillColor(m_colour);

	t_window.draw(m_body);
	t_window.draw(m_name);
	t_window.draw(m_currDist);
	t_window.draw(m_totalDist);
}

void VisNode::highlight()
{
	m_highlight = true;
}

void VisNode::deHighlight()
{
	m_highlight = false;
}

void VisNode::select()
{
	m_selected = true;
}

void VisNode::deselect()
{
	m_selected = false;
	m_inPath = false;
	m_visited = false;
}

void VisNode::setCurrText(float t_distance)
{
	int distance = t_distance;
	m_currDist.setString(std::to_string(distance));

	m_currDist.setOrigin(m_currDist.getLocalBounds().width / 2.0f, m_currDist.getLocalBounds().height / 2.0f);
	m_currDist.setPosition(m_body.getPosition().x, m_body.getPosition().y - (m_radius * 0.2f));

}

void VisNode::setCurrText(std::string t_text)
{
	m_currDist.setString(t_text);
	m_currDist.setOrigin(m_currDist.getLocalBounds().width / 2.0f, m_currDist.getLocalBounds().height / 2.0f);
	m_currDist.setPosition(m_body.getPosition().x, m_body.getPosition().y - (m_radius * 0.2f));
}

void VisNode::setTotalText(float t_distance)
{
	int distance = t_distance;
	m_totalDist.setString(std::to_string(distance));

	m_totalDist.setOrigin(m_totalDist.getLocalBounds().width / 2.0f, m_totalDist.getLocalBounds().height / 2.0f);
	m_totalDist.setPosition(m_body.getPosition().x, m_body.getPosition().y + (m_radius * 0.4f));
}

void VisNode::setTotalText(std::string t_text)
{
	m_totalDist.setString(t_text);
	m_totalDist.setOrigin(m_totalDist.getLocalBounds().width / 2.0f, m_totalDist.getLocalBounds().height / 2.0f);
	m_totalDist.setPosition(m_body.getPosition().x, m_body.getPosition().y + (m_radius * 0.4f));

}

void VisNode::setPath(bool t_inPath)
{
	m_inPath = t_inPath;
}

void VisNode::setVisited(bool t_visited)
{
	m_visited = t_visited;
}

bool VisNode::getSelect()
{
	return m_selected;
}

std::string VisNode::getCurrentText()
{
	return m_currDist.getString();
}

std::string VisNode::getTotalText()
{
	return m_totalDist.getString();
}

std::string VisNode::getName()
{
	return m_name.getString();
}

sf::Vector2f VisNode::getPos() const
{
	return m_body.getPosition();
}

int VisNode::getRadius() const
{
	return m_body.getRadius();
}
