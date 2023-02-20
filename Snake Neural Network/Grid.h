#pragma once
#include "Snake.h"

class Grid
{
private:
	std::vector<sf::RectangleShape> m_lines;
	std::vector<std::vector<int>> m_objects;

public:
	Grid();

	void addObjectToTheGrid(int value, int column, int line);

	void drawGrid(sf::RenderWindow& win);
};

