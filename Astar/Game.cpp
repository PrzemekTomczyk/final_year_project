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
		if (m_window.hasFocus())
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
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	if (m_grid->update())
	{
		handleGridPathfinding();
	}
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

void Game::handleGridPathfinding()
{
	m_grid->resetPath();

	int start = m_grid->getStartIndex();
	std::string startString = getStringIndex(start);
	int goal = m_grid->getGoalIndex();
	std::string goalString = getStringIndex(goal);

	m_grid->updateNotRequired();

	m_start = startString;
	m_end = goalString;

	if (m_start != "" || m_end != "")
	{
		std::cout << "#########################" << std::endl;
		m_grid->aStar(visitTile);

		m_start = "";
		m_end = "";
	}
}

std::string Game::getStringIndex(int t_index)
{
	std::string startString = "";
	if (t_index > 129)
	{
		t_index = (t_index % 130);
		startString = "n" + std::to_string(t_index);
	}
	else if (t_index > 119)
	{
		t_index = (t_index % 120);
		startString = "m" + std::to_string(t_index);
	}
	else if (t_index > 109)
	{
		t_index = (t_index % 110);
		startString = "l" + std::to_string(t_index);
	}
	else if (t_index > 99)
	{
		t_index = (t_index % 100);
		startString = "k" + std::to_string(t_index);
	}
	else if (t_index > 89)
	{
		t_index = (t_index % 90);
		startString = "j" + std::to_string(t_index);
	}
	else if (t_index > 79)
	{
		t_index = (t_index % 80);
		startString = "i" + std::to_string(t_index);
	}
	else if (t_index > 69)
	{
		t_index = (t_index % 70);
		startString = "h" + std::to_string(t_index);
	}
	else if (t_index > 59)
	{
		t_index = (t_index % 60);
		startString = "g" + std::to_string(t_index);
	}
	else if (t_index > 49)
	{
		t_index = (t_index % 50);
		startString = "f" + std::to_string(t_index);
	}
	else if (t_index > 39)
	{
		t_index = (t_index % 40);
		startString = "e" + std::to_string(t_index);
	}
	else if (t_index > 29)
	{
		t_index = (t_index % 30);
		startString = "d" + std::to_string(t_index);
	}
	else if (t_index > 19)
	{
		t_index = (t_index % 20);
		startString = "c" + std::to_string(t_index);
	}
	else if (t_index > 9)
	{
		t_index = (t_index % 10);
		startString = "b" + std::to_string(t_index);
	}
	else if (t_index <= 9) {
		startString = "a" + std::to_string(t_index);
	}
	return startString;
}

int Game::getNumIndex(std::string t_index)
{
	int numericalIndex = (int)t_index.at(1) - 48;

	if (t_index.at(0) == char('a'))
	{
		return numericalIndex;
	}
	else if (t_index.at(0) == char('b'))
	{
		//ssIndex >> numericalIndex;
		numericalIndex += 10;
	}
	else if (t_index.at(0) == char('c'))
	{
		//ssIndex >> numericalIndex;
		numericalIndex += 20;
	}
	else if (t_index.at(0) == char('d'))
	{
		//ssIndex >> numericalIndex;
		numericalIndex += 30;
	}
	else if (t_index.at(0) == char('e'))
	{
		//ssIndex >> numericalIndex;
		numericalIndex += 40;
	}
	else if (t_index.at(0) == char('f'))
	{
		//ssIndex >> numericalIndex;
		numericalIndex += 50;
	}
	else if (t_index.at(0) == char('g'))
	{
		//ssIndex >> numericalIndex;
		numericalIndex += 60;
	}
	else if (t_index.at(0) == char('h'))
	{
		//ssIndex >> numericalIndex;
		numericalIndex += 70;
	}
	else if (t_index.at(0) == char('i'))
	{
		//ssIndex >> numericalIndex;
		numericalIndex += 80;
	}
	else if (t_index.at(0) == char('j'))
	{
		//ssIndex >> numericalIndex;
		numericalIndex += 90;
	}
	else if (t_index.at(0) == char('k'))
	{
		//ssIndex >> numericalIndex;
		numericalIndex += 100;
	}
	else if (t_index.at(0) == char('l'))
	{
		//ssIndex >> numericalIndex;
		numericalIndex += 110;
	}
	else if (t_index.at(0) == char('m'))
	{
		//ssIndex >> numericalIndex;
		numericalIndex += 120;
	}
	else if (t_index.at(0) == char('n'))
	{
		//ssIndex >> numericalIndex;
		numericalIndex += 130;
	}

	return numericalIndex;
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
			windowTitle = "A* Visualisation - Test Layout";
			break;
		case GridLayout::SANDBOX:
			windowYSize = SANDBOX_LAYOUT_ROWS * (unsigned)std::ceil(height / SANDBOX_LAYOUT_ROWS);
			windowXSize = (windowYSize / SANDBOX_LAYOUT_ROWS) * SANDBOX_LAYOUT_TILES_PER_ROW;
			windowTitle = "A* Visualisation - Sandbox Layout";
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

		//setup end points
		m_start = "";
		m_end = "";
	}
}