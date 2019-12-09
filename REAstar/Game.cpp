#include "Game.h"
#include <iostream>

/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	//m_window{ },
	m_exitGame{ false }, //when true game will exit
	m_grid(m_font, m_window)
{
	setupFont(); // load font 

	//setup tooltip text
	m_tooltipText.setFont(m_font);
	m_tooltipText.setFillColor(sf::Color::White);
	m_tooltipText.setString("Mouse controls:\n\nPress LMB to place Goal\n\nPress RMB to place Start Tiles\n\nPress/hold MMB to place Obstacles\n\n\nKeyboard controls:\n\nPress SPACE to toggle between\nplacing obstacles and \nreseting tiles using MMB\n\nPress 1 to display cost values\n\nPress 2 to remove Goal tile\n\nPress R reset the grid");

	int width = sf::VideoMode::getDesktopMode().width - 50;
	int height = sf::VideoMode::getDesktopMode().height - 50;
	unsigned int windowYSize = 0;
	unsigned int windowXSize = 0;

	//adjust window size to get rid of black square and fix tile out of bounds when clicking


	windowYSize = 14 * (unsigned)std::ceil(height / 14);
	windowXSize = (windowYSize / 14) * 10;

	m_tooltipText.setCharacterSize((int)(windowYSize / 62));
	float outlineThiccness = thor::length(sf::Vector2f(m_tooltipText.getGlobalBounds().width, (float)windowYSize)) * 0.01f;
	m_textBackground.setSize(sf::Vector2f(m_tooltipText.getGlobalBounds().width + outlineThiccness * 2, (float)windowYSize));

	m_window.create(sf::VideoMode{ windowXSize + (unsigned int)m_tooltipText.getGlobalBounds().width + (unsigned int)outlineThiccness * 2, windowYSize, 32U }, "REA*", sf::Style::Titlebar | sf::Style::Close);
	m_window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - m_window.getSize().x / 2, 0));


	m_textBackground.setFillColor(sf::Color(0, 0, 102));
	m_textBackground.setOutlineColor(sf::Color(255, 140, 0));
	m_textBackground.setOutlineThickness(-outlineThiccness);
	m_textBackground.setPosition((float)windowXSize, 0);
	m_tooltipText.setPosition(m_textBackground.getPosition().x + outlineThiccness, m_textBackground.getPosition().y + outlineThiccness);

	std::cout << "Starting Grid init" << std::endl;
	m_grid.init(m_textBackground.getPosition().x + m_textBackground.getSize().x / 2.0f);
	std::cout << "Finished Grid init" << std::endl;
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	m_grid.update();

}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::Black);

	m_window.draw(m_textBackground);
	m_window.draw(m_tooltipText);

	m_grid.render();

	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFont()
{
	if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
}