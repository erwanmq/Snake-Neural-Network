#include "Grid.h"
#include "Text.h"
#include <iostream>

Grid::Grid()
{
	sf::RectangleShape line;
	line.setFillColor(sf::Color::White);
	line.setSize(sf::Vector2f(1.f, HEIGHT));
	for (int i{ 0 }; i < NB_CASE_WIDTH; i++)
	{
		line.setPosition(sf::Vector2f(1.f * WIDTH / NB_CASE_WIDTH * i, 0));
		m_lines.push_back(line);
	}

	line.setSize(sf::Vector2f(WIDTH, 1.f));
	for (int i{ 0 }; i < NB_CASE_HEIGHT; i++)
	{
		line.setPosition(sf::Vector2f(0, 1.f * HEIGHT / NB_CASE_HEIGHT * i));
		m_lines.push_back(line);
	}
	m_objects.resize(NB_CASE_WIDTH, std::vector<int>(NB_CASE_HEIGHT));


}

void Grid::addObjectToTheGrid(int value, int column, int line) // 0 nothing, 1 wall, 2 fruit
{
	m_objects[column][line] = value;
}

void Grid::drawGrid(sf::RenderWindow& win)
{
	for (const auto& line : m_lines)
		win.draw(line);

	for (int i{ 0 }; i < m_objects.size(); i++)
	{
		for (int j{ 0 }; j < m_objects[i].size(); j++)
		{
			Text text(std::string("DejaVuSansMono.ttf"), sf::String(std::to_string(m_objects[i][j])), sf::Vector2f(WIDTH - 200.f, 30.f));
		}
	}
		
}
