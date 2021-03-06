#include "stdafx.h"
#include "Game.h"

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

	//setup tooltip text
	m_tooltipText.setFont(m_font);
	m_tooltipText.setFillColor(sf::Color::White);
	m_tooltipText.setString(Utils::INSTRUCTIONS);

	m_grid = new GridManager(m_font, m_window, Utils::TEST_TILE_AMOUNT, Utils::TEST_LAYOUT_ROWS, Utils::TEST_LAYOUT_TILES_PER_ROW);
	setupGrid();
}

Game::~Game()
{
}

void Game::run()
{
	m_window.setActive(false);

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
		}
	}
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	if (!m_loadLayout && m_window.hasFocus())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
		{
			m_loadLayout = true;
			m_layout = GridLayout::TEST;
			initLayout();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
		{
			m_loadLayout = true;
			m_layout = GridLayout::SMALL;
			initLayout();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
		{
			m_loadLayout = true;
			m_layout = GridLayout::MEDIUM;
			initLayout();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F4))
		{
			m_loadLayout = true;
			m_layout = GridLayout::LARGE;
			initLayout();
		}
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::F1) && !sf::Keyboard::isKeyPressed(sf::Keyboard::F2) && !sf::Keyboard::isKeyPressed(sf::Keyboard::F3) && !sf::Keyboard::isKeyPressed(sf::Keyboard::F4))
	{
		m_loadLayout = false;
	}

	m_grid->update(m_tooltipText.getPosition());
}

void Game::render()
{
	if (m_layout != GridLayout::LARGE)
	{
		m_window.clear(sf::Color::Black);
	}
	else
	{
		m_window.clear(sf::Color(150,150,150,255));
	}

	m_window.draw(m_textBackground);
	m_window.draw(m_tooltipText);

	m_grid->render();

	m_window.display();
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
		m_grid = new GridManager(m_font, m_window, Utils::TEST_TILE_AMOUNT, Utils::TEST_LAYOUT_ROWS, Utils::TEST_LAYOUT_TILES_PER_ROW);
		break;
	case GridLayout::SMALL:
		m_grid = new GridManager(m_font, m_window, Utils::SMALL_SANDBOX_TILE_AMOUNT, Utils::SMALL_SANDBOX_LAYOUT_TILES_PER_ROW, Utils::SMALL_SANDBOX_LAYOUT_ROWS);
		break;
	case GridLayout::MEDIUM:
		m_grid = new GridManager(m_font, m_window, Utils::MED_SANDBOX_TILE_AMOUNT, Utils::MED_SANDBOX_LAYOUT_TILES_PER_ROW, Utils::MED_SANDBOX_LAYOUT_ROWS);
		break;
	case GridLayout::LARGE:
		m_grid = new GridManager(m_font, m_window, Utils::LARGE_SANDBOX_TILE_AMOUNT, Utils::LARGE_SANDBOX_LAYOUT_TILES_PER_ROW, Utils::LARGE_SANDBOX_LAYOUT_ROWS);
		break;
	default:
		//create test layout by default
		m_grid = new GridManager(m_font, m_window, Utils::TEST_TILE_AMOUNT, Utils::TEST_LAYOUT_ROWS, Utils::TEST_LAYOUT_TILES_PER_ROW);
		break;
	}

	setupGrid();
}

void Game::setupGrid()
{
	if (m_grid)
	{
		//take away 50 to make a window slightly smaller that fullscreen
		int width = sf::VideoMode::getDesktopMode().width - 50;
		int height = sf::VideoMode::getDesktopMode().height - 50;
		unsigned int windowYSize = 0;
		unsigned int windowXSize = 0;
		std::string windowTitle = "";

		switch (m_layout)
		{
		case GridLayout::TEST:
			windowYSize = Utils::TEST_LAYOUT_ROWS * (unsigned)std::ceil(height / Utils::TEST_LAYOUT_ROWS);
			windowXSize = (windowYSize / Utils::TEST_LAYOUT_ROWS) * Utils::TEST_LAYOUT_TILES_PER_ROW;
			windowTitle = "REA* Visualisation - Test Layout";
			break;
		case GridLayout::SMALL:
			windowYSize = Utils::SMALL_SANDBOX_LAYOUT_ROWS * (unsigned)std::ceil(height / Utils::SMALL_SANDBOX_LAYOUT_ROWS);
			windowXSize = (windowYSize / Utils::SMALL_SANDBOX_LAYOUT_ROWS) * Utils::SMALL_SANDBOX_LAYOUT_TILES_PER_ROW;
			windowTitle = "REA* Visualisation - Small Layout";
			break;
		case GridLayout::MEDIUM:
			windowYSize = Utils::MED_SANDBOX_LAYOUT_ROWS * (unsigned)std::ceil(height / Utils::MED_SANDBOX_LAYOUT_ROWS);
			windowXSize = (windowYSize / Utils::MED_SANDBOX_LAYOUT_ROWS) * Utils::MED_SANDBOX_LAYOUT_TILES_PER_ROW;
			windowTitle = "REA* Visualisation - Medium Layout";
			break;
		case GridLayout::LARGE:
			windowYSize = Utils::LARGE_SANDBOX_LAYOUT_ROWS * (unsigned)std::ceil(height / Utils::LARGE_SANDBOX_LAYOUT_ROWS);
			windowXSize = (windowYSize / Utils::LARGE_SANDBOX_LAYOUT_ROWS) * Utils::LARGE_SANDBOX_LAYOUT_TILES_PER_ROW;
			windowTitle = "REA* Visualisation - Large Layout";
			break;
		default:
			break;
		}

		//scale character size to be 1/62 of window height
		m_tooltipText.setCharacterSize((int)(windowYSize / 62));

		float outlineThiccness = thor::length(sf::Vector2f(m_tooltipText.getGlobalBounds().width, (float)windowYSize)) * 0.01f;
		m_textBackground.setSize(sf::Vector2f(m_tooltipText.getGlobalBounds().width + outlineThiccness * 2, (float)windowYSize));

		if (!m_windowCreated)
		{
			m_window.create(sf::VideoMode{ windowXSize + (unsigned int)m_tooltipText.getGlobalBounds().width + (unsigned int)outlineThiccness * 2, windowYSize, 32U }, windowTitle, sf::Style::Titlebar | sf::Style::Close);
			m_windowCreated = true;
		}
		else
		{
			m_window.setSize(sf::Vector2u(windowXSize + (unsigned int)m_tooltipText.getGlobalBounds().width + (unsigned int)outlineThiccness * 2, windowYSize));
			sf::FloatRect visibleArea(0, 0, static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y));
			m_window.setView(sf::View(visibleArea));
		}

		m_window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - m_window.getSize().x / 2, 0));

		m_textBackground.setFillColor(sf::Color(0, 0, 102));
		m_textBackground.setOutlineColor(sf::Color(255, 140, 0));
		m_textBackground.setOutlineThickness(-outlineThiccness);
		m_textBackground.setPosition((float)windowXSize, 0);
		m_tooltipText.setPosition(m_textBackground.getPosition().x + outlineThiccness, m_textBackground.getPosition().y + outlineThiccness);
		m_grid->init(m_textBackground.getPosition().x + m_textBackground.getSize().x / 2.0f, m_tooltipText.getPosition(), m_tooltipText.getCharacterSize());
	}
}