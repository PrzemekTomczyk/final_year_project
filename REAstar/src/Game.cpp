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

	m_grid = new GridManager(m_font, m_window, TEST_TILE_AMOUNT, TEST_LAYOUT_ROWS, TEST_LAYOUT_TILES_PER_ROW);
	setupGrid();

	//ImGui::SFML::Init(m_window);
}

Game::~Game()
{
}

void Game::run()
{
	m_window.setActive(false);

	boost::thread t{ &Game::renderingThread, this };

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
		//imguiUpdate(timePerFrame);
		//ImGui::SFML::Render(m_window);
		//render();
	}
	t.join();
	//ImGui::SFML::Shutdown();
}

void Game::processEvents()
{
	sf::Event event;
	processScreenEvents();
	while (m_window.pollEvent(event))
	{
		//ImGui::SFML::ProcessEvent(event);
		if (sf::Event::Resized == event.type)
		{
			sf::FloatRect visibleArea(0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height));
			m_window.setView(sf::View(visibleArea));
		}
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

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	if (!m_loadLayout)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
		{
			m_creatingGrid = true;
			boost::mutex::scoped_lock lock(m_mutex);

			while (m_rendering)
			{
				m_conditional.wait(lock);
				std::cout << "while in F1" << std::endl;
			}

			m_loadLayout = true;
			m_layout = GridLayout::TEST;
			initLayout();
			m_creatingGrid = false;
			lock.unlock();
			m_conditional.notify_all();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
		{
			m_creatingGrid = true;
			boost::mutex::scoped_lock lock(m_mutex);

			while (m_rendering)
			{
				m_conditional.wait(lock);
				std::cout << "while in F2" << std::endl;
			}

			m_loadLayout = true;
			m_layout = GridLayout::SANDBOX;
			initLayout();
			m_creatingGrid = false;
			lock.unlock();
			m_conditional.notify_all();
		}
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::F1) && !sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
	{
		m_loadLayout = false;
	}


	m_grid->update();
}

void Game::render()
{
	m_window.clear(sf::Color::Black);

	m_window.draw(m_textBackground);
	m_window.draw(m_tooltipText);

	m_grid->render();

	//ImGui::SFML::Render(m_window);

	m_window.display();
}

void Game::renderingThread()
{
	m_window.setActive(true);

	// the rendering loop
	while (m_window.isOpen())
	{
		boost::mutex::scoped_lock lock(m_mutex);

		while (m_creatingGrid)
		{
			m_conditional.wait(lock);
			std::cout << "waiting for grid setup" << std::endl;
		}

		if (!m_creatingGrid)
		{
			m_rendering = true;

			m_window.clear(sf::Color::Black);

			m_window.draw(m_textBackground);
			m_window.draw(m_tooltipText);

			if (m_grid != nullptr)
			{
				m_grid->render();
			}

			m_window.display();
			m_rendering = false;
			m_conditional.notify_all();
		}
	}
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

		if (!m_windowCreated)
		{
			m_window.create(sf::VideoMode{ windowXSize + (unsigned int)m_tooltipText.getGlobalBounds().width + (unsigned int)outlineThiccness * 2, windowYSize, 32U }, windowTitle, sf::Style::Titlebar | sf::Style::Close);
			m_windowCreated = true;
		}
		else
		{
			m_window.setSize(sf::Vector2u(windowXSize + (unsigned int)m_tooltipText.getGlobalBounds().width + (unsigned int)outlineThiccness * 2, windowYSize));
		}

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

void Game::imguiUpdate(sf::Time t_deltaTime)
{
	ImGui::SFML::Update(m_window, t_deltaTime);
	ImGui::Begin("Sample window"); // begin window

	sf::Color bgColor;
	float color[3] = { 0.f, 0.f, 0.f };
	char windowTitle[255] = "ImGui + SFML = <3";


	// Background color edit
	if (ImGui::ColorEdit3("Background color", color)) {
		// this code gets called if color value changes, so
		// the background color is upgraded automatically!
		bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
		bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
		bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
	}

	// Window title text edit
	ImGui::InputText("Window title", windowTitle, 255);

	if (ImGui::Button("Update window title")) {
		// this code gets if user clicks on the button
		// yes, you could have written if(ImGui::InputText(...))
		// but I do this to show how buttons work :)
		m_window.setTitle(windowTitle);
	}
	ImGui::End(); // end window
}
