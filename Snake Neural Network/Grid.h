#pragma once
#include "Snake.h"

class Grid
{
private:
	std::vector<sf::RectangleShape> m_lines;

public:
	Grid();

	void drawGrid(sf::RenderWindow& win);
};

