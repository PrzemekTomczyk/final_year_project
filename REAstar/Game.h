#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include <GridManager.h>

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupFont();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_font; // font used by message
	bool m_exitGame; // control exiting game

	GridManager m_grid;

	//text
	sf::Text m_tooltipText;
	sf::RectangleShape m_textBackground;


};

#endif // !GAME_HPP

