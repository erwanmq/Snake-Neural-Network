#pragma once
#include "Value.h"

class Grid
{
private:
	std::vector<sf::RectangleShape> m_lines;
	std::vector<std::vector<int>> m_objects;

private:
	

public:
	Grid();

	void addObjectInTheGrid(int value, int column, int line);
	int getObjectInTheGrid(int column, int line);

	bool isThereAWall(int column, int line, Direction dir);
	bool isThereAFruit(int column, int line, Direction dir);

	void drawGrid(sf::RenderWindow& win);
	void initObjectsGrid();
};

