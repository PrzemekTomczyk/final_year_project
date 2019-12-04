#include "Game.h"
#include "Thor/Vectors.hpp"
#include <sstream> 
#include <stdlib.h>

void visit(Node* node) {
	std::cout << "Visiting: " << node->data() << std::endl;
}

Game::Game() :
	m_window{ sf::VideoMode{ 1920, 1080, 32 }, "Astar Visualization" },
	m_exitGame{ false },
	m_graph(30),
	m_graphGrid(1400),
	m_grid(m_font, m_window),
	USING_GRID(true)
{
	m_window.setVerticalSyncEnabled(true);


	if (!m_font.loadFromFile("ariblk.ttf"))
	{
		std::cout << "Error loading font!" << std::endl;
	}


	if (!USING_GRID)
	{
		loadTxtFiles();
		setupNodes();
	}
	else
	{
		loadGridTxtFiles();
		setupGrid();
	}
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
	}
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	if (!USING_GRID)
	{
		handleNodes(t_deltaTime);
	}
	else
	{
		if (m_grid.update())
		{
			handleGrid();
		}
	}
}

void Game::render()
{
	m_window.clear(sf::Color::Black);

	if (!USING_GRID)
	{
		drawLines();
		for (int i = 0; i < m_visualNodes.size(); i++)
		{
			m_visualNodes[i].draw(m_window);
		}
		for (int i = 0; i < m_legendNodes.size(); i++)
		{
			m_legendNodes[i].draw(m_window);
		}
	}
	else
	{
		m_window.draw(m_textBackground);
		m_window.draw(m_tooltipText);

		m_grid.render();
	}

	m_window.display();
}

void Game::processScreenEvents()
{
}

void Game::mouseCollision()
{
	for (int i = 0; i < m_visualNodes.size(); i++)
	{
		float x;
		float y;
		x = m_visualNodes.at(i).getPos().x - sf::Vector2f(sf::Mouse::getPosition(m_window)).x;
		y = m_visualNodes.at(i).getPos().y - sf::Vector2f(sf::Mouse::getPosition(m_window)).y;
		float dist = std::sqrt(std::pow(x, 2) + std::pow(y, 2));

		if (dist < m_visualNodes.at(i).getRadius())
		{
			m_visualNodes.at(i).highlight();
		}
		else
		{
			m_visualNodes.at(i).deHighlight();
		}
	}
}

void Game::mouseSelect()
{
	for (int i = 0; i < m_visualNodes.size(); i++)
	{
		float x;
		float y;
		x = m_visualNodes.at(i).getPos().x - sf::Vector2f(sf::Mouse::getPosition(m_window)).x;
		y = m_visualNodes.at(i).getPos().y - sf::Vector2f(sf::Mouse::getPosition(m_window)).y;
		float dist = std::sqrt(std::pow(x, 2) + std::pow(y, 2));

		if (dist < m_visualNodes.at(i).getRadius())
		{
			if (!m_visualNodes.at(i).getSelect())
			{
				m_visualNodes.at(i).select();
				if (m_start == "")
				{
					m_start = m_visualNodes.at(i).getName();
				}
				else if (m_end == "")
				{
					m_end = m_visualNodes.at(i).getName();
				}
			}
			else
			{
				m_visualNodes.at(i).deselect();
			}
		}
	}

	int amountSelected = 0;

	for (int i = 0; i < m_visualNodes.size(); i++)
	{
		if (m_visualNodes.at(i).getSelect())
		{
			amountSelected++;
		}
	}

	if (amountSelected == 2)
	{
		m_twoSelected = true;
	}
}

void Game::deselectAll()
{
	for (int i = 0; i < m_visualNodes.size(); i++)
	{
		m_visualNodes.at(i).deselect();
		m_visualNodes.at(i).setCurrText(0);
		m_visualNodes.at(i).setTotalText(0);
	}

	m_twoSelected = false;
	m_start = "";
	m_end = "";

	std::system("CLS");
}

void Game::drawLines()
{
	std::ifstream myfile;
	int index = 0;

	myfile.open("arcs.txt");

	std::string from, to;
	int	weight;

	while (myfile >> from >> to >> weight) {
		sf::Vector2f fromPos;
		sf::Vector2f toPos;

		for (int i = 0; i < m_visualNodes.size(); i++)
		{
			if (m_visualNodes.at(i).getName() == from)
			{
				fromPos = m_visualNodes.at(i).getPos();
			}
			if (m_visualNodes.at(i).getName() == to)
			{
				toPos = m_visualNodes.at(i).getPos();
			}
		}

		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(fromPos)),
			sf::Vertex(sf::Vector2f(toPos))
		};

		m_window.draw(line, 2, sf::Lines);
	}

	myfile.close();
}

void Game::createNodes()
{
	std::string nodeName;
	int xpos;
	int ypos;
	std::ifstream myfile;

	int i = 0;

	myfile.open("nodes.txt");

	while (myfile >> nodeName >> xpos >> ypos)
	{
		VisNode visualNode(
			nodeName,
			sf::Vector2f( // position vector
			(m_graph.nodeIndex(i)->getX() * 2) + m_window.getSize().x * 0.25f, //x
				(m_graph.nodeIndex(i)->getY() * 2) + m_window.getSize().y * 0.2f), //y
			m_font, // font for text
			50 // radius
		);

		m_visualNodes.push_back(visualNode);
		i++;
	}

	myfile.close();
}

void Game::loadTxtFiles() {
	std::string nodeName;
	int xpos;
	int ypos;
	std::ifstream myfile;

	int index = 0;

	myfile.open("nodes.txt");

	while (myfile >> nodeName >> xpos >> ypos)
	{
		m_graph.addNode(nodeName, xpos, ypos, index);
		m_nodeMap[nodeName] = index;
		index++;
	}

	myfile.close();

	myfile.open("arcs.txt");

	std::string from, to;
	int	weight;

	while (myfile >> from >> to >> weight) {
		m_graph.addArc(m_nodeMap[from], m_nodeMap[to], weight);
	}

	myfile.close();
}

void Game::loadGridTxtFiles()
{
	std::string nodeName;
	int xpos;
	int ypos;
	std::ifstream myfile;

	int index = 0;

	myfile.open("gridNodes.txt");

	while (myfile >> nodeName >> xpos >> ypos)
	{
		m_graphGrid.addNode(nodeName, xpos, ypos, index);
		m_nodeMap[nodeName] = index;
		index++;
	}

	myfile.close();

	myfile.open("gridArcs.txt");

	std::string from, to;
	int	weight;

	while (myfile >> from >> to >> weight) {
		m_graphGrid.addArc(m_nodeMap[from], m_nodeMap[to], weight);
	}

	myfile.close();
}

void Game::setupNodes()
{
	m_start = "";
	m_end = "";
	createNodes();

	std::string name = "Name";

	VisNode node(
		"Name",
		sf::Vector2f( // position vector
			100.0f, //x
			100.0f //y
		),
		m_font, // font for text
		50 // radius
	);
	node.setCurrText("Current");
	node.setTotalText("Total");

	m_legendNodes.push_back(node);

	VisNode visitedNode(
		"Visited",
		sf::Vector2f( // position vector
			100.0f, //x
			210.0f //y
		),
		m_font, // font for text
		50 // radius
	);
	visitedNode.setCurrText(0);
	visitedNode.setTotalText(0);
	visitedNode.setVisited(true);

	m_legendNodes.push_back(visitedNode);

	VisNode pathNode(
		"Path",
		sf::Vector2f( // position vector
			100.0f, //x
			320.0f //y
		),
		m_font, // font for text
		50 // radius
	);
	pathNode.setCurrText(0);
	pathNode.setTotalText(0);
	pathNode.setPath(true);

	m_legendNodes.push_back(pathNode);

	VisNode startNode(
		"Goal",
		sf::Vector2f( // position vector
			100.0f, //x
			430.0f //y
		),
		m_font, // font for text
		50 // radius
	);
	startNode.setCurrText(0);
	startNode.setTotalText(0);
	startNode.select();

	m_legendNodes.push_back(startNode);
}

void Game::handleNodes(sf::Time t_deltaTime)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_clicked)
	{
		m_clicked = true;
		if (!m_twoSelected)
		{
			mouseSelect();
		}
		else
		{
			deselectAll();
		}
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_clicked = false;
	}

	mouseCollision();

	if (m_twoSelected)
	{
		std::vector<Node*> vecPath;
		Node* current;

		if (m_start != "" || m_end != "")
		{
			m_graph.aStar(m_graph.nodeIndex(m_nodeMap[m_start]), m_graph.nodeIndex(m_nodeMap[m_end]), visit, vecPath);

			std::cout << "Node path: " << std::endl;
			for (int i = vecPath.size() - 1; i > -1; i--)
			{
				std::cout << vecPath.at(i)->data() << std::endl;

				for (int index = 0; index < m_visualNodes.size(); index++)
				{
					current = m_graph.nodeIndex(index);

					if (m_visualNodes.at(index).getName() == vecPath.at(i)->data())
					{
						m_visualNodes.at(index).setPath(true);

						m_visualNodes.at(index).setCurrText(vecPath.at(i)->getCurrDist());
						m_visualNodes.at(index).setTotalText(vecPath.at(i)->getTotalDist());
					}

					if (current->getVisited())
					{
						m_visualNodes.at(index).setVisited(true);
					}
				}
			}
			m_start = "";
			m_end = "";
		}
	}
}

void Game::handleGrid()
{
	m_grid.resetPath();

	int start = m_grid.getStartIndex();
	std::string startString = getStringIndex(start);
	int goal = m_grid.getGoalIndex();
	std::string goalString = getStringIndex(goal);

	m_grid.updateNotRequired();

	std::vector<Node*> vecPath;
	Node* current;
	m_start = startString;
	m_end = goalString;

	if (m_start != "" || m_end != "")
	{
		m_graphGrid.aStar(m_graphGrid.nodeIndex(m_nodeMap[m_start]), m_graphGrid.nodeIndex(m_nodeMap[m_end]), visit, vecPath);

		std::cout << "Node path: " << std::endl;
		for (int i = vecPath.size() - 1; i > -1; i--)
		{
			std::cout << vecPath.at(i)->data() << std::endl;

			m_grid.setToPath(getNumIndex(vecPath.at(i)->data()));
		}
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

void Game::setupGrid()
{
	////setup tooltip text
	//m_tooltipText.setFont(m_font);
	//m_tooltipText.setFillColor(sf::Color::White);
	//m_tooltipText.setString("Mouse controls:\n\nPress LMB to place Goal\n\nPress RMB to place Start Tiles\n\nPress/hold MMB to place Obstacles\n\n\nKeyboard controls:\n\nPress SPACE to toggle between\nplacing obstacles and \nreseting tiles using MMB\n\nPress 1 to display cost values\n\nPress 2 to remove Goal tile\n\nPress R reset the grid");

	int width = sf::VideoMode::getDesktopMode().width - 50;
	int height = sf::VideoMode::getDesktopMode().height - 50;
	unsigned int windowYSize = 0;
	unsigned int windowXSize = 0;

	windowYSize = 14 * std::ceil(height / 14);
	windowXSize = (windowYSize / 14) * 10;

	//m_tooltipText.setCharacterSize((int)(windowYSize / 62));
	//float outlineThiccness = thor::length(sf::Vector2f(m_tooltipText.getGlobalBounds().width, windowYSize)) * 0.01f;
	//m_textBackground.setSize(sf::Vector2f(m_tooltipText.getGlobalBounds().width + outlineThiccness * 2, windowYSize));

	m_window.create(sf::VideoMode{ windowXSize /*+ (unsigned int)m_tooltipText.getGlobalBounds().width + (unsigned int)outlineThiccness * 2*/, windowYSize, 32U }, "REA*", sf::Style::Titlebar | sf::Style::Close);
	m_window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - m_window.getSize().x / 2, 0));


	//m_textBackground.setFillColor(sf::Color(0, 0, 102));
	//m_textBackground.setOutlineColor(sf::Color(255, 140, 0));
	//m_textBackground.setOutlineThickness(-outlineThiccness);
	//m_textBackground.setPosition(windowXSize, 0);
	//m_tooltipText.setPosition(m_textBackground.getPosition().x + outlineThiccness, m_textBackground.getPosition().y + outlineThiccness);

	std::cout << "Starting Grid init" << std::endl;
	m_grid.init(m_textBackground.getPosition().x/* + m_textBackground.getSize().x / 2.0f*/);
	std::cout << "Finished Grid init" << std::endl;

	//setup invisible nodes
	m_start = "";
	m_end = "";
}