#include "Grid.h"
#include <iostream>

Grid::Grid()
{
	sf::RectangleShape line;
	line.setFillColor(sf::Color::White);
	line.setSize(sf::Vector2f(1.f, HEIGHT));
	for (int i{ 0 }; i < NB_CASE_WIDTH; i++)
	{
		line.setPosition(sf::Vector2f(1.f * WIDTH / NB_CASE_WIDTH * i, 0));
		std::cout << 1.f * WIDTH / NB_CASE_WIDTH * i;
		m_lines.push_back(line);
	}

	line.setSize(sf::Vector2f(WIDTH, 1.f));
	for (int i{ 0 }; i < NB_CASE_HEIGHT; i++)
	{
		line.setPosition(sf::Vector2f(0, 1.f * HEIGHT / NB_CASE_HEIGHT * i));
		m_lines.push_back(line);
	}


}

void Grid::drawGrid(sf::RenderWindow& win)
{
	for (const auto& line : m_lines)
		win.draw(line);
}
