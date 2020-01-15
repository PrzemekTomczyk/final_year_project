#include "Game.h"
#include "Thor/Vectors.hpp"
#include <sstream> 
#include <stdlib.h>

void visitTile(GridTile* tile) {
	std::cout << "Visiting: " << tile->getIndex() << std::endl;
}

//#define TEST_LAYOUT

Game::Game() :
	m_exitGame{ false },
	m_layout(GridLayout::TEST)
{
	m_window.setVerticalSyncEnabled(true);

	if (!m_font.loadFromFile("ariblk.ttf"))
	{
		std::cout << "Error loading font!" << std::endl;
	}

	m_grid = new GridManager(m_font, m_window, TEST_TILE_AMOUNT, TEST_LAYOUT_ROWS, TEST_LAYOUT_TILES_PER_ROW);
	setupGrid();
}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f);
	while (m_window.isOpen())
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update(timePerFrame);
		}
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	processScreenEvents();
	while (m_window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type) // window message
		{
			m_window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == event.type)
		{
			if (!m_loadLayout)
			{
				if (event.key.code == sf::Keyboard::F1)
				{
					m_loadLayout = true;
					m_layout = GridLayout::TEST;
					initLayout();
				}
				if (event.key.code == sf::Keyboard::F2)
				{
					m_loadLayout = true;
					m_layout = GridLayout::SANDBOX;
					initLayout();
				}
			}
		}
		else if (sf::Event::KeyReleased == event.type)
		{
			if (m_loadLayout)
			{
				if (event.key.code == sf::Keyboard::F1 || event.key.code == sf::Keyboard::F2)
				{
					m_loadLayout = false;
				}
			}
		}
	}
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	m_grid->update();
}

void Game::render()
{
	m_window.clear(sf::Color::Black);

	m_window.draw(m_textBackground);
	m_window.draw(m_tooltipText);

	m_grid->render();

	m_window.display();
}

void Game::processScreenEvents()
{
}

void Game::initLayout()
{
	if (m_grid)
	{
		delete m_grid;
	}

	switch (m_layout)
	{
	case GridLayout::TEST:
		m_grid = new GridManager(m_font, m_window, TEST_TILE_AMOUNT, TEST_LAYOUT_ROWS, TEST_LAYOUT_TILES_PER_ROW);
		break;
	case GridLayout::SANDBOX:
		m_grid = new GridManager(m_font, m_window, SANDBOX_TILE_AMOUNT, SANDBOX_LAYOUT_TILES_PER_ROW, SANDBOX_LAYOUT_ROWS);
		break;
	default:
		//create test layout by default
		m_grid = new GridManager(m_font, m_window, TEST_TILE_AMOUNT, TEST_LAYOUT_ROWS, TEST_LAYOUT_TILES_PER_ROW);
		break;
	}

	setupGrid();
}

void Game::setupGrid()
{
	if (m_grid)
	{
		//setup tooltip text
		m_tooltipText.setFont(m_font);
		m_tooltipText.setFillColor(sf::Color::White);
		m_tooltipText.setString("Mouse controls:\n\nPress LMB to place Goal\n\nPress RMB to place Start Tiles\n\nPress/hold MMB to place Obstacles\n\n\nKeyboard controls:\n\nPress SPACE to toggle between\nplacing obstacles and \nreseting tiles using MMB\n\nPress F1 to load Test Layout\n\nPress F2 to load Sandbox Layout\n\nPress R reset the grid");

		//take away 50 to make a window slightly smaller that fullscreen
		int width = sf::VideoMode::getDesktopMode().width - 50;
		int height = sf::VideoMode::getDesktopMode().height - 50;
		unsigned int windowYSize = 0;
		unsigned int windowXSize = 0;
		std::string windowTitle = "";

		switch (m_layout)
		{
		case GridLayout::TEST:
			windowYSize = TEST_LAYOUT_ROWS * (unsigned)std::ceil(height / TEST_LAYOUT_ROWS);
			windowXSize = (windowYSize / TEST_LAYOUT_ROWS) * TEST_LAYOUT_TILES_PER_ROW;
			windowTitle = "REA* Visualisation - Test Layout";
			break;
		case GridLayout::SANDBOX:
			windowYSize = SANDBOX_LAYOUT_ROWS * (unsigned)std::ceil(height / SANDBOX_LAYOUT_ROWS);
			windowXSize = (windowYSize / SANDBOX_LAYOUT_ROWS) * SANDBOX_LAYOUT_TILES_PER_ROW;
			windowTitle = "REA* Visualisation - Sandbox Layout";
			break;
		default:
			break;
		}

		//scale character size to be 1/62 of window height
		m_tooltipText.setCharacterSize((int)(windowYSize / 62));
		float outlineThiccness = thor::length(sf::Vector2f(m_tooltipText.getGlobalBounds().width, (float)windowYSize)) * 0.01f;
		m_textBackground.setSize(sf::Vector2f(m_tooltipText.getGlobalBounds().width + outlineThiccness * 2, (float)windowYSize));

		m_window.create(sf::VideoMode{ windowXSize + (unsigned int)m_tooltipText.getGlobalBounds().width + (unsigned int)outlineThiccness * 2, windowYSize, 32U }, windowTitle, sf::Style::Titlebar | sf::Style::Close);
		m_window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - m_window.getSize().x / 2, 0));

		m_textBackground.setFillColor(sf::Color(0, 0, 102));
		m_textBackground.setOutlineColor(sf::Color(255, 140, 0));
		m_textBackground.setOutlineThickness(-outlineThiccness);
		m_textBackground.setPosition((float)windowXSize, 0);
		m_tooltipText.setPosition(m_textBackground.getPosition().x + outlineThiccness, m_textBackground.getPosition().y + outlineThiccness);

		std::cout << "Starting Grid init" << std::endl;
		m_grid->init(m_textBackground.getPosition().x + m_textBackground.getSize().x / 2.0f);
		std::cout << "Finished Grid init" << std::endl;
	}
}